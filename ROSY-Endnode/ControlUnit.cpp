#include "ControlUnit.h"

void ControlUnit::ControlUnitInit(bool(*updateCB)(), void(*loggerCallback)(uint8_t)) 
{
#if ENABLE_BME280
	this->enableBME280 = true;
#endif
#if ENABLE_AM2315
	this->enableAM2315 = true;
#endif
#if ENABLE_OPCN3
	this->enableOPC = true;
#endif
#if ENABLE_SENSOR_1 || ENABLE_SENSOR_2
	this->enableADS1115_1 = true;
#endif
#if ENABLE_SENSOR_3 || ENABLE_SENSOR_4
	this->enableADS1115_2 = true;
#endif
#if ENABLE_SENSOR_5 || ENABLE_SENSOR_6
	this->enableADS1115_3 = true;
#endif
#if ENABLE_LSM9DS0
	this->enableLSM9DS0 = true;
#endif
#if ENABLE_DS3231
	this->enableDS3231 = true;
#endif
#if ENABLE_INA219
	this->enableINA219 = true;
#endif
#if ENABLE_SEN0186
	this->enableSen0186 = true;
#endif

	this->logger = loggerCallback;
	this->initDS3231();
	this->BME280Init();
	this->AM2315Init();
	this->OPCN3Init();
	this->initADS1115_1();
	this->initADS1115_2();
	this->initADS1115_3();
	this->initLSM9DS0();
	this->INA219Init();
	this->initSen0186();
	this->update = updateCB;


#if ENABLE_DEBUG_MODE && ENABLE_I2CSCANNER
	ROSY_i2cScanner::scanner();
#endif 
#if ENABLE_DEBUG_MODE
	Serial.println("SENSOR ENABLE LIST: ");
#if !(ENABLE_AM2315 || ENABLE_BME280 || ENABLE_OPCN3 || ENABLE_SENSOR_1 || ENABLE_SENSOR_2 || ENABLE_SENSOR_3 || ENABLE_SENSOR_4||ENABLE_SENSOR_5||ENABLE_SENSOR_6|| ENABLE_LSM9DS0 ||ENABLE_DS3231 || ENABLE_INA219 ||ENABLE_SEN0186 )
	Serial.println("NO SENSOR: ");
#endif
#if ENABLE_AM2315
	Serial.print("SENSOR AM2315: "); Serial.println(ENABLE_AM2315);
#endif
#if ENABLE_BME280
	Serial.print("SENSOR BME280: "); Serial.println(ENABLE_BME280);
#endif
#if ENABLE_OPCN3 
	Serial.print("SENSOR OPCN3: "); Serial.println(ENABLE_OPCN3);
#endif
#if ENABLE_SENSOR_1 || ENABLE_SENSOR_2
	Serial.print("ADS1115_1"); Serial.println(ENABLE_SENSOR_1 || ENABLE_SENSOR_2);
#endif
#if ENABLE_SENSOR_3 || ENABLE_SENSOR_4
	Serial.print("ADS1115_2"); Serial.println(ENABLE_SENSOR_3 || ENABLE_SENSOR_4);
#endif
#if ENABLE_SENSOR_5 || ENABLE_SENSOR_6
	Serial.print("ADS1115_3"); Serial.println(ENABLE_SENSOR_5 || ENABLE_SENSOR_6);
#endif
#if ENABLE_SENSOR_1
	Serial.print("SENSOR 1"); Serial.println(ENABLE_SENSOR_1);
#endif
#if ENABLE_SENSOR_2
	Serial.print("SENSOR 2"); Serial.println(ENABLE_SENSOR_2);
#endif
#if ENABLE_SENSOR_3
	Serial.print("SENSOR 3"); Serial.println(ENABLE_SENSOR_3);
#endif
#if ENABLE_SENSOR_4
	Serial.print("SENSOR 4"); Serial.println(ENABLE_SENSOR_4);
#endif
#if ENABLE_SENSOR_5
	Serial.print("SENSOR 5"); Serial.println(ENABLE_SENSOR_5);
#endif
#if ENABLE_SENSOR_6
	Serial.print("SENSOR 6"); Serial.println(ENABLE_SENSOR_6);
#endif
#if ENABLE_LSM9DS0
	Serial.print("SENSOR LSM9DS0"); Serial.println(ENABLE_LSM9DS0);
#endif
#if ENABLE_DS3231
	Serial.print("RTC DS3231"); Serial.println(ENABLE_DS3231);
#endif
#if ENABLE_INA219
	Serial.print("INA219"); Serial.println(ENABLE_INA219);
#endif
#if ENABLE_SEN0186
	Serial.print("SEN0186"); Serial.println(ENABLE_SEN0186);
#endif
#endif

}

void ControlUnit::INA219Init()
{
#if ENABLE_INA219
	bool isEnd = false;
	unsigned long int start = millis();
	while (!isEnd)
	{
		if ((start > millis()) ? (millis() + (pow(2, 32) - 1) - start) : (millis() - start) > 10 * 1000)
		{
			isEnd = true;
			this->enableINA219 = false;
#if  ENABLE_DEBUG_MODE
			Serial.println("Failed to find INA219 chip");
#endif
			this->logger(LOGACT_error_init_INA219);
		}

		if (this->INA219.begin())
		{
			isEnd = true;
			this->enableINA219 = true;
#if  ENABLE_DEBUG_MODE
			Serial.println("DEBUG: INA219 begin successful");
#endif	
			this->logger(LOGACT_init_INA219);
		}
		else
		{
			isEnd = false;
			this->enableINA219 = false;
		}
	}
	return;
#else
	return;
#endif
}

bool ControlUnit::getINA219status()
{
#if ENABLE_INA219
	return this->enableINA219;
#else
	return false;
#endif
}

float ControlUnit::getBatteryCurrent()
{
#if ENABLE_INA219
	if (this->enableINA219)
	{
#if  ENABLE_DEBUG_MODE
		float temp = this->INA219.getCurrent_mA();
		Serial.println("DEBUG: INA219 current successful>>>>value: " + String(temp));
		return temp;
#else
		return this->INA219.getCurrent_mA();
#endif
		this->logger(LOGACT_get_bat_mA);
	}
	else return NAN;
#else
	return NAN;
#endif
}

float ControlUnit::getBatteryVoltage()
{
	this->logger(LOGACT_get_bat_V);
	return NAN;
}

void ControlUnit::BME280Init()
{
#if  ENABLE_BME280
	bool isEnd = false;
	unsigned long int start = millis();
	while (!isEnd)
	{
		if (this->BME280.begin())
		{
			isEnd = true;
			this->enableBME280 = true;
#if  ENABLE_DEBUG_MODE
			Serial.println("DEBUG: BME280 begin successful");
#endif
			this->logger(LOGACT_init_BME280);
		}
		else
		{
			isEnd = false;
			this->enableBME280 = false;
		}

		if ((start > millis()) ? (millis() + (pow(2, 32) - 1) - start) : (millis() - start) > 10 * 1000)
		{
			isEnd = true;
			this->enableBME280 = false;
#if  ENABLE_DEBUG_MODE
			Serial.println("ERROR  BME280: Could not find a valid BME280 sensor");
#endif  
			this->logger(LOGACT_error_init_BME280);
		}
	}

#endif
	return;
}

bool ControlUnit::getBME280status()
{
#if ENABLE_BME280
	return this->enableBME280;
#else
	return false;
#endif
}


float ControlUnit::BME280ReadTemperature()
{

#if ENABLE_BME280
	if (this->enableBME280)
	{
		float temp = 0;
		if(this->BME280.readTemperature(&temp))
		{
#if  ENABLE_DEBUG_MODE
			Serial.println("DEBUG: BME280 Temperature successful>>>>value: " + String(temp));
			Serial.println(temp);
#endif 
			this->logger(LOGACT_read_BME280_temp);
		}
		else
		{
#if  ENABLE_DEBUG_MODE
				Serial.println("DEBUG: BME280 Temperature error");
#endif
			temp=NAN;
			this->logger(LOGACT_error_read_BME280_temp);
		}
		return temp;
	}
	else
	{
		return NAN;
	}
#else
	return NAN;
#endif
}

float ControlUnit::BME280ReadHumidity() {
#if  ENABLE_BME280
	if (this->enableBME280)
	{
		float temp = 0; 	
		if(this->BME280.readHumidity(&temp))
		{
		#if ENABLE_DEBUG_MODE
			Serial.println("DEBUG: BME280 Humidity successful>>>>value: " + String(temp));
		#endif
			this->logger(LOGACT_read_BME280_hum);
		}
		else 
		{
		#if ENABLE_DEBUG_MODE
			Serial.println("DEBUG: BME280 Humidity error");
		#endif
			this->logger(LOGACT_error_read_BME280_hum);
			temp = NAN;
		}
		return temp;
	}
	else
	{
		return NAN;
	}
#else
	return NAN;
#endif
}

float ControlUnit::BME280ReadPressure()
{
#if  ENABLE_BME280
	if (this->enableBME280)
	{
		float temp = 0;
		if(this->BME280.readPressure(&temp))
		{
			temp/=100.0;
			#if ENABLE_DEBUG_MODE
			Serial.println("DEBUG: BME280 Pressure successful>>>>value: " + String(temp));
			#endif
			this->logger(LOGACT_read_BME280_pressure);
		}
		else 
		{
			#if ENABLE_DEBUG_MODE
			Serial.println("DEBUG: BME280 Pressure error");
			#endif
			this->logger(LOGACT_error_read_BME280_pressure);
			temp = NAN;
		}
		return temp;
	}
	else
	{
		return NAN;
	}
#else
	return NAN;
#endif
}

void ControlUnit::AM2315Init() {
#if  ENABLE_AM2315
	bool isEnd = false;
	unsigned long int start = millis();
	while (!isEnd)
	{
		if ((start > millis()) ? (millis() + (pow(2, 32) - 1) - start) : (millis() - start) > 10 * 1000)
		{
			isEnd = true;
			this->enableAM2315 = false;
#if  ENABLE_DEBUG_MODE
			Serial.println("DEBUG: AM2315 begin failed");
#endif
			this->logger(LOGACT_error_init_AM2315);
		}

		if (this->AM2315.begin())
		{
			isEnd = true;
			this->enableAM2315 = true;
#if  ENABLE_DEBUG_MODE
			Serial.println("DEBUG: AM2315 begin successful");
#endif
			this->logger(LOGACT_init_AM2315);

		}
		else
		{
			isEnd = false;
			this->enableAM2315 = false;
		}

	}
#endif
	return;
}
bool ControlUnit::getAM2315status()
{
#if ENABLE_AM2315
	return this->enableAM2315;
#else 
	return false;
#endif
}

void ControlUnit::AM2315Read()
{
#if  ENABLE_AM2315
	if (this->enableAM2315)
	{
		unsigned long start = millis();
		bool temp = false;
		while ((!temp) && ((start > millis()) ? (millis() + (pow(2, 32) - 1) - start ):( millis() - start) <= 10 * 1000)) temp = this->AM2315.readData();
#if ENABLE_DEBUG_MODE
		temp ? Serial.println("DEBUG: AM2315 read successful") : Serial.println("DEBUG: AM2315 error while reading");
#endif
		temp ? this->logger(LOGACT_read_AM2315) : this->logger(LOGACT_error_read_AM2315);
	}
#endif
}

float ControlUnit::AM2315GetTemperature()
{
#if  ENABLE_AM2315
	if (this->enableAM2315)
	{
#if  ENABLE_DEBUG_MODE
		float temp = this->AM2315.getTemperature();
		Serial.println("DEBUG: AM2315 Temperature successful>>>>value: " + String(temp));
		return temp;
#else
		return this->AM2315.getTemperature();
#endif
	}
	else
	{
		return NAN;
	}
#else
	return NAN;
#endif
}

float ControlUnit::AM2315GetHumidity()
{
#if ENABLE_AM2315
	if (this->enableAM2315)
	{
#if ENABLE_DEBUG_MODE
		float temp = this->AM2315.getHumidity();
		Serial.println("DEBUG: AM2315 Humidity successful>>>>value: " + String(temp));
		return temp;
#else
		return this->AM2315.getHumidity();
#endif
	}
	else
	{
		return NAN;
	}
#else
	return NAN;
#endif
}

void ControlUnit::OPCN3Init()
{
#if  ENABLE_OPCN3
	bool isEnd = false;
	unsigned long int start = millis();
	while (!isEnd)
	{
		if ((start > millis()) ? (millis() + (pow(2, 32) - 1) - start) : (millis() - start) > 10 * 1000)
		{
			isEnd = true;
			this->enableOPC = false;
#if ENABLE_DEBUG_MODE
			Serial.println("DEBUG: OPCN3 begin failed");
#endif
			this->logger(LOGACT_error_init_OPCN3);

		}
		if (this->OPC.initialize())
		{
			isEnd = true;
			this->enableOPC = true;
#if  ENABLE_DEBUG_MODE
			Serial.println("DEBUG: OPCN3 begin successful");
#endif
			this->logger(LOGACT_init_OPCN3);

		}
		else
		{
			isEnd = false;
			this->enableOPC = false;
		}

	}
#endif
	return;
}

bool ControlUnit::getOPCN3status()
{
#if ENABLE_OPCN3
	return this->enableOPC;
#else
	return false;
#endif
}


void ControlUnit::OPCN3SetHistogramData()
{
#if ENABLE_OPCN3	
	if (this->enableOPC)
	{
		this->OPChist = this->OPC.readHistogramData();
#if ENABLE_DEBUG_MODE
		Serial.println("DEBUG: OPCN3 readHistogramData successful");
#endif
	}
#endif
	return;
}

float ControlUnit::OPCN3PM1()
{
#if ENABLE_OPCN3
	if (this->enableOPC)
	{
#if ENABLE_DEBUG_MODE
		float temp = this->OPChist.pm1;
		Serial.println("DEBUG: OPCN3 pm1 successful>>>>value: " + String(temp));
		return temp;
#else
		return this->OPChist.pm1;
#endif
	}
	else
	{
		return NAN;
	}
#else
	return NAN;
#endif
}
float ControlUnit::OPCN3PM2_5()
{
#if ENABLE_OPCN3
	if (this->enableOPC)
	{
#if ENABLE_DEBUG_MODE
		float temp = this->OPChist.pm2_5;
		Serial.println("DEBUG: OPCN3 pm1 successful>>>>value: " + String(temp));
		return temp;
#else
		return this->OPChist.pm2_5;
#endif	
	}
	else
	{
		return NAN;
	}
#else
	return NAN;
#endif	
}
float ControlUnit::OPCN3PM10()
{
#if ENABLE_OPCN3
	if (this->enableOPC)
	{
#if ENABLE_DEBUG_MODE
		float temp = this->OPChist.pm10;
		Serial.println("DEBUG: OPCN3 pm1 successful>>>>value: " + String(temp));
		return temp;
#else
		return this->OPChist.pm10;
#endif
	}
	else
	{
		return NAN;
	}
#else
	return NAN;
#endif	
}

void ControlUnit::initADS1115_1()
{
#if  ENABLE_SENSOR_1 || ENABLE_SENSOR_2
	bool isEnd = false;
	unsigned long int start = millis();
	while (!isEnd)
	{
		if ((start > millis()) ? (millis() + (pow(2, 32) - 1) - start) : (millis() - start) > 10 * 1000)
		{
			isEnd = true;
			this->enableADS1115_1 = false;
#if ENABLE_DEBUG_MODE
			Serial.println("DEBUG: ADS1115_1 begin failed");
#endif
			this->logger(LOGACT_error_init_ADS1115_1);

		}
		if (this->ADS1115_1.begin())
		{
			isEnd = true;
			this->enableADS1115_1 = true;
#if  ENABLE_DEBUG_MODE
			Serial.println("DEBUG: ADS1115_1 begin successful");
#endif
			this->logger(LOGACT_init_ADS1115_1);

		}
		else
		{
			isEnd = false;
			this->enableADS1115_1 = false;
		}

	}
#endif
	return;
}

bool ControlUnit::getADS1115_1status()
{
#if ENABLE_SENSOR_1 || ENABLE_SENSOR_2
	return this->enableADS1115_1;
#else 
	return false;
#endif
}


void ControlUnit::initADS1115_2()
{
#if  ENABLE_SENSOR_3 || ENABLE_SENSOR_4
	bool isEnd = false;
	unsigned long int start = millis();
	while (!isEnd)
	{
		if ((start > millis()) ? (millis() + (pow(2, 32) - 1) - start) : (millis() - start) > 10 * 1000)
		{
			isEnd = true;
			this->enableADS1115_2 = false;
#if ENABLE_DEBUG_MODE
			Serial.println("DEBUG: ADS1115_2 begin failed");
#endif
			this->logger(LOGACT_error_init_ADS1115_2);

		}
		if (this->ADS1115_2.begin())
		{
			isEnd = true;
			this->enableADS1115_2 = true;
#if  ENABLE_DEBUG_MODE
			Serial.println("DEBUG: ADS1115_2 begin successful");
#endif
			this->logger(LOGACT_init_ADS1115_2);

		}
		else
		{
			isEnd = false;
			this->enableADS1115_2 = false;
		}
	}
#endif	
	return;
}

bool ControlUnit::getADS1115_2status()
{
#if ENABLE_SENSOR_3 || ENABLE_SENSOR_4
	return this->enableADS1115_2;
#else 
	return false;
#endif
}

void ControlUnit::initADS1115_3()
{
#if  ENABLE_SENSOR_5 || ENABLE_SENSOR_6
	bool isEnd = false;
	unsigned long int start = millis();
	while (!isEnd)
	{
		if ((start > millis()) ? (millis() + (pow(2, 32) - 1) - start) : (millis() - start) > 10 * 1000)
		{
			isEnd = true;
			this->enableADS1115_3 = false;
#if ENABLE_DEBUG_MODE
			Serial.println("DEBUG: ADS1115_3 begin failed");
#endif
			this->logger(LOGACT_error_init_ADS1115_3);

		}
		if (this->ADS1115_3.begin())
		{
			isEnd = true;
			this->enableADS1115_3 = true;
#if  ENABLE_DEBUG_MODE
			Serial.println("DEBUG: ADS1115_3 begin successful");
#endif
			this->logger(LOGACT_init_ADS1115_3);

		}
		else
		{
			isEnd = false;
			this->enableADS1115_3 = false;
		}

	}
#endif
	return;
}

bool ControlUnit::getADS1115_3status()
{
#if ENABLE_SENSOR_5 || ENABLE_SENSOR_6
	return this->enableADS1115_3;
#else 
	return false;
#endif
}



float ControlUnit::readSensor1_1()
{
#if  ENABLE_SENSOR_1
	if (this->enableADS1115_1)
	{
		uint16_t temp = 0;
		bool isEnd = false;
		unsigned long int start = millis();
		while (!isEnd)
		{
			if ((start > millis()) ? (millis() + (pow(2, 32) - 1) - start) : (millis() - start) > 10 * 1000)
			{
				this->logger(LOGACT_error_read_ADS1115_1_1);
#if ENABLE_DEBUG_MODE
				Serial.println("DEBUG: SENSOR_1_1 read failed");
#endif
				return NAN;
			}
			if (this->ADS1115_1.readADC_SingleEnded(0, &temp))
			{
				this->logger(LOGACT_read_ADS1115_1_1);
#if ENABLE_DEBUG_MODE
				Serial.println("DEBUG: SENSOR_1_1 read successful>>>> value: " + String(temp));
#endif
				return (float)(temp * ADS1115_CONVERSION_mV);
			}
			else
			{
				isEnd = false;
			}
		}
	}
	else
	{
		return NAN;
	}
#else 
	return NAN;
#endif
}

float ControlUnit::readSensor1_2()
{
#if  ENABLE_SENSOR_1
	if (this->enableADS1115_1)
	{
		uint16_t temp = 0;
		bool isEnd = false;
		unsigned long int start = millis();
		while (!isEnd)
		{
			if ((start > millis()) ? (millis() + (pow(2, 32) - 1) - start) : (millis() - start) > 10 * 1000)
			{
				this->logger(LOGACT_error_read_ADS1115_1_2);
#if ENABLE_DEBUG_MODE
				Serial.println("DEBUG: SENSOR_1_2 read failed");
#endif
				return NAN;
			}
			if (this->ADS1115_1.readADC_SingleEnded(1, &temp))
			{
				this->logger(LOGACT_read_ADS1115_1_2);
#if ENABLE_DEBUG_MODE
				Serial.println("DEBUG: SENSOR_1_2 read successful>>>> value: " + String(temp));
#endif
				return (float)(temp * ADS1115_CONVERSION_mV);
			}
			else
			{
				isEnd = false;
			}
		}
	}
	else
	{
		return NAN;
	}
#else 
	return NAN;
#endif
}

float ControlUnit::readSensor1_3()
{
#if  ENABLE_SENSOR_2
	if (this->enableADS1115_1)
	{
		uint16_t temp = 0;
		bool isEnd = false;
		unsigned long int start = millis();
		while (!isEnd)
		{
			if ((start > millis()) ? (millis() + (pow(2, 32) - 1) - start) : (millis() - start) > 10 * 1000)
			{
				this->logger(LOGACT_error_read_ADS1115_1_3);
#if ENABLE_DEBUG_MODE
				Serial.println("DEBUG: SENSOR_1_3 read failed");
#endif
				return NAN;
			}
			if (this->ADS1115_1.readADC_SingleEnded(2, &temp))
			{
				this->logger(LOGACT_read_ADS1115_1_3);
#if ENABLE_DEBUG_MODE
				Serial.println("DEBUG: SENSOR_1_3 read successful>>>> value: " + String(temp));
#endif
				return (float)(temp * ADS1115_CONVERSION_mV);
			}
			else
			{
				isEnd = false;
			}
		}
	}
	else
	{
		return NAN;
	}
#else 
	return NAN;
#endif
}
float ControlUnit::readSensor1_4()
{
#if  ENABLE_SENSOR_2
	if (this->enableADS1115_1)
	{
		uint16_t temp = 0;
		bool isEnd = false;
		unsigned long int start = millis();
		while (!isEnd)
		{
			if ((start > millis()) ? (millis() + (pow(2, 32) - 1) - start) : (millis() - start) > 10 * 1000)
			{
				this->logger(LOGACT_error_read_ADS1115_1_4);
#if ENABLE_DEBUG_MODE
				Serial.println("DEBUG: SENSOR_1_4 read failed");
#endif
				return NAN;
			}
			if (this->ADS1115_1.readADC_SingleEnded(3, &temp))
			{
				this->logger(LOGACT_read_ADS1115_1_4);
#if ENABLE_DEBUG_MODE
				Serial.println("DEBUG: SENSOR_1_4 read successful>>>> value: " + String(temp));
#endif
				return (float)(temp * ADS1115_CONVERSION_mV);
			}
			else
			{
				isEnd = false;
			}
		}
	}
	else
	{
		return NAN;
	}
#else 
	return NAN;
#endif
}

float ControlUnit::readSensor2_1()
{
#if  ENABLE_SENSOR_3	
	if (this->enableADS1115_2)
	{
		uint16_t temp = 0;
		bool isEnd = false;
		unsigned long int start = millis();
		while (!isEnd)
		{
			if ((start > millis()) ? (millis() + (pow(2, 32) - 1) - start) : (millis() - start) > 10 * 1000)
			{
				this->logger(LOGACT_error_read_ADS1115_2_1);
#if ENABLE_DEBUG_MODE
				Serial.println("DEBUG: SENSOR_2_1 read failed");
#endif
				return NAN;
			}
			if (this->ADS1115_2.readADC_SingleEnded(0, &temp))
			{
				this->logger(LOGACT_read_ADS1115_2_1);
#if ENABLE_DEBUG_MODE
				Serial.println("DEBUG: SENSOR_2_1 read successful>>>> value: " + String(temp));
#endif
				return (float)(temp * ADS1115_CONVERSION_mV);
			}
			else
			{
				isEnd = false;
			}
		}
	}
	else
	{
		return NAN;
	}
#else 
	return NAN;
#endif
}
float ControlUnit::readSensor2_2()
{
#if  ENABLE_SENSOR_3	
	if (this->enableADS1115_2)
	{
		uint16_t temp = 0;
		bool isEnd = false;
		unsigned long int start = millis();
		while (!isEnd)
		{
			if ((start > millis()) ? (millis() + (pow(2, 32) - 1) - start) : (millis() - start) > 10 * 1000)
			{
				this->logger(LOGACT_error_read_ADS1115_2_2);
#if ENABLE_DEBUG_MODE
				Serial.println("DEBUG: SENSOR_2_2 read failed");
#endif
				return NAN;
			}
			if (this->ADS1115_2.readADC_SingleEnded(1, &temp))
			{
				this->logger(LOGACT_read_ADS1115_2_2);
#if ENABLE_DEBUG_MODE
				Serial.println("DEBUG: SENSOR_2_2 read successful>>>> value: " + String(temp));
#endif
				return (float)(temp * ADS1115_CONVERSION_mV);
			}
			else
			{
				isEnd = false;
			}
		}
	}
	else
	{
		return NAN;
	}
#else 
	return NAN;
#endif
}

float ControlUnit::readSensor2_3()
{
#if ENABLE_SENSOR_4
	if (this->enableADS1115_2)
	{
		uint16_t temp = 0;
		bool isEnd = false;
		unsigned long int start = millis();
		while (!isEnd)
		{
			if ((start > millis()) ? (millis() + (pow(2, 32) - 1) - start) : (millis() - start) > 10 * 1000)
			{
				this->logger(LOGACT_error_read_ADS1115_2_3);
#if ENABLE_DEBUG_MODE
				Serial.println("DEBUG: SENSOR_2_3 read failed");
#endif
				return NAN;
			}
			if (this->ADS1115_2.readADC_SingleEnded(2, &temp))
			{
				this->logger(LOGACT_read_ADS1115_2_3);
#if ENABLE_DEBUG_MODE
				Serial.println("DEBUG: SENSOR_2_3 read successful>>>> value: " + String(temp));
#endif
				return (float)(temp * ADS1115_CONVERSION_mV);
			}
			else
			{
				isEnd = false;
			}
		}
	}
	else
	{
		return NAN;
	}
#else 
	return NAN;
#endif
}
float ControlUnit::readSensor2_4()
{
#if  ENABLE_SENSOR_4	
	if (this->enableADS1115_2)
	{
		uint16_t temp = 0;
		bool isEnd = false;
		unsigned long int start = millis();
		while (!isEnd)
		{
			if ((start > millis()) ? (millis() + (pow(2, 32) - 1) - start) : (millis() - start) > 10 * 1000)
			{
				this->logger(LOGACT_error_read_ADS1115_2_4);
#if ENABLE_DEBUG_MODE
				Serial.println("DEBUG: SENSOR_2_4 read failed");
#endif
				return NAN;
			}
			if (this->ADS1115_2.readADC_SingleEnded(3, &temp))
			{
				this->logger(LOGACT_read_ADS1115_2_4);
#if ENABLE_DEBUG_MODE
				Serial.println("DEBUG: SENSOR_2_4 read successful>>>> value: " + String(temp));
#endif
				return (float)(temp * ADS1115_CONVERSION_mV);
			}
			else
			{
				isEnd = false;
			}
		}
	}
	else
	{
		return NAN;
	}
#else 
	return NAN;
#endif
}



float ControlUnit::readSensor3_1()
{
#if  ENABLE_SENSOR_5
	if (this->enableADS1115_3)
	{
		uint16_t temp = 0;
		bool isEnd = false;
		unsigned long int start = millis();
		while (!isEnd)
		{
			if ((start > millis()) ? (millis() + (pow(2, 32) - 1) - start) : (millis() - start) > 10 * 1000)
			{
				this->logger(LOGACT_error_read_ADS1115_3_1);
#if ENABLE_DEBUG_MODE
				Serial.println("DEBUG: SENSOR_3_1 read failed");
#endif
				return NAN;
			}
			if (this->ADS1115_3.readADC_SingleEnded(0, &temp))
			{
				this->logger((LOGACT_read_ADS1115_3_1);
#if ENABLE_DEBUG_MODE
				Serial.println("DEBUG: SENSOR_3_1 read successful>>>> value: " + String(temp));
#endif
				return (float)(temp * ADS1115_CONVERSION_mV);
			}
			else
			{
				isEnd = false;
			}
		}
	}
	else
	{
		return NAN;
	}
#else 
	return NAN;
#endif
}

float ControlUnit::readSensor3_2()
{
#if  ENABLE_SENSOR_5
	if (this->enableADS1115_3)
	{
		uint16_t temp = 0;
		bool isEnd = false;
		unsigned long int start = millis();
		while (!isEnd)
		{
			if ((start > millis()) ? (millis() + (pow(2, 32) - 1) - start) : (millis() - start) > 10 * 1000)
			{
				this->logger(LOGACT_error_read_ADS1115_3_2);
#if ENABLE_DEBUG_MODE
				Serial.println("DEBUG: SENSOR_3_2 read failed");
#endif
				return NAN;
			}
			if (this->ADS1115_3.readADC_SingleEnded(1, &temp))
			{
				this->logger((LOGACT_read_ADS1115_3_2);
#if ENABLE_DEBUG_MODE
				Serial.println("DEBUG: SENSOR_3_2 read successful>>>> value: " + String(temp));
#endif
				return (float)(temp * ADS1115_CONVERSION_mV);
			}
			else
			{
				isEnd = false;
			}
		}
	}
	else
	{
		return NAN;
	}
#else 
	return NAN;
#endif
}


float ControlUnit::readSensor3_3()
{
#if  ENABLE_SENSOR_6
	if (this->enableADS1115_3)
	{
		uint16_t temp = 0;
		bool isEnd = false;
		unsigned long int start = millis();
		while (!isEnd)
		{
			if ((start > millis()) ? (millis() + (pow(2, 32) - 1) - start) : (millis() - start) > 10 * 1000)
			{
				this->logger(LOGACT_error_read_ADS1115_3_3);
#if ENABLE_DEBUG_MODE
				Serial.println("DEBUG: SENSOR_3_3 read failed");
#endif
				return NAN;
			}
			if (this->ADS1115_3.readADC_SingleEnded(2, &temp))
			{
				this->logger((LOGACT_read_ADS1115_3_3);
#if ENABLE_DEBUG_MODE
				Serial.println("DEBUG: SENSOR_3_3 read successful>>>> value: " + String(temp));
#endif
				return (float)(temp * ADS1115_CONVERSION_mV);
			}
			else
			{
				isEnd = false;
			}
		}
	}
	else
	{
		return NAN;
	}
#else 
	return NAN;
#endif
}

float ControlUnit::readSensor3_4()
{
#if  ENABLE_SENSOR_6
	if (this->enableADS1115_3)
	{
		uint16_t temp = 0;
		bool isEnd = false;
		unsigned long int start = millis();
		while (!isEnd)
		{
			if ((start > millis()) ? (millis() + (pow(2, 32) - 1) - start) : (millis() - start) > 10 * 1000)
			{
				this->logger(LOGACT_error_read_ADS1115_3_4);
#if ENABLE_DEBUG_MODE
				Serial.println("DEBUG: SENSOR_3_4 read failed");
#endif
				return NAN;
			}
			if (this->ADS1115_3.readADC_SingleEnded(3, &temp))
			{
				this->logger((LOGACT_read_ADS1115_3_4);
#if ENABLE_DEBUG_MODE
				Serial.println("DEBUG: SENSOR_3_4 read successful>>>> value: " + String(temp));
#endif
				return (float)(temp * ADS1115_CONVERSION_mV);
			}
			else
			{
				isEnd = false;
			}
		}
	}
	else
	{
		return NAN;
	}
#else 
	return NAN;
#endif
}


void ControlUnit::initLSM9DS0()
{
#if ENABLE_LSM9DS0
	bool isEnd = false;
	unsigned long int start = millis();
	while (!isEnd)
	{
		if ((start > millis()) ? (millis() + (pow(2, 32) - 1) - start) : (millis() - start) > 10 * 1000)
		{
			isEnd = true;
			this->enableLSM9DS0 = false;
#if ENABLE_DEBUG_MODE
			Serial.println("ERROR  LSM9DS0: Could not find a valid LSM9DS0 sensor");
#endif	
			this->logger(LOGACT_error_init_LSM9DS0);

		}

		if (this->LSM9DS0.begin())
		{
			isEnd = true;
			this->enableLSM9DS0 = true;
#if ENABLE_DEBUG_MODE
			Serial.println("DEBUG: LSM9DS0 begin successful");
#endif
			this->LSM9DS0.setupAccel(this->LSM9DS0.LSM9DS1_ACCELRANGE_2G);
			this->LSM9DS0.setupMag(this->LSM9DS0.LSM9DS1_MAGGAIN_4GAUSS);
			this->LSM9DS0.setupGyro(this->LSM9DS0.LSM9DS1_GYROSCALE_245DPS);
#if ENABLE_DEBUG_MODE
			Serial.println("DEBUG: LSM9DS0 set successful");
#endif
			this->logger(LOGACT_init_LSM9DS0);
		}
		else
		{
			isEnd = false;
			this->enableLSM9DS0 = false;
		}
	}
	return;
#endif

}

bool ControlUnit::getLSM9DS0status()
{
#if ENABLE_LSM9DS0
	return this->enableLSM9DS0;
#else 
	return false;
#endif
}

void ControlUnit::LSM9DS0Read()
{
#if ENABLE_LSM9DS0
	if (this->enableLSM9DS0)
	{
		this->LSM9DS0.read();
		LSM9DS0.getEvent(&this->accel, &this->mag, &this->gyro, &this->temp);
#if ENABLE_DEBUG_MODE
		Serial.println("DEBUG: LSM9DS0 read successful");
#endif
	}
#else 
	return;
#endif
	return;
}

float ControlUnit::LSM9DS0GetAccelX()
{
#if ENABLE_LSM9DS0
	if (this->enableLSM9DS0)
	{
#if ENABLE_DEBUG_MODE
		float temp = this->accel.acceleration.x;
		Serial.println("DEBUG: LSM9DS0 AccelX successful>>>>value: " + String(temp));
		return temp;
#else 
		return this->accel.acceleration.x;
#endif	
	}
	else
	{
		return NAN;
	}
#else 
	return NAN;
#endif
}

float ControlUnit::LSM9DS0GetAccelY()
{
#if ENABLE_LSM9DS0
	if (this->enableLSM9DS0)
	{
#if ENABLE_DEBUG_MODE
		float temp = this->accel.acceleration.y;
		Serial.println("DEBUG: LSM9DS0 AccelY successful>>>>value: " + String(temp));
		return temp;
#else 
		return this->accel.acceleration.y;
#endif	
	}
	else
	{
		return NAN;
	}
#else 
	return NAN;
#endif
}
float ControlUnit::LSM9DS0GetAccelZ()
{
#if ENABLE_LSM9DS0
	if (this->enableLSM9DS0)
	{
#if ENABLE_DEBUG_MODE
		float temp = this->accel.acceleration.z;
		Serial.println("DEBUG: LSM9DS0 AccelZ successful>>>>value: " + String(temp));
		return temp;
#else 
		return this->accel.acceleration.z;
#endif
	}
	else
	{
		return NAN;
	}
#else 
	return NAN;
#endif
}
float ControlUnit::LSM9DS0GetMagX()
{
#if ENABLE_LSM9DS0
	if (this->enableLSM9DS0)
	{
#if ENABLE_DEBUG_MODE
		float temp = this->mag.magnetic.x;
		Serial.println("DEBUG: LSM9DS0 MagX successful>>>>value: " + String(temp));
		return temp;
#else 
		return this->mag.magnetic.x;
#endif
	}
	else
	{
		return NAN;
	}
#else 
	return NAN;
#endif
}
float ControlUnit::LSM9DS0GetMagY()
{
#if ENABLE_LSM9DS0
	if (this->enableLSM9DS0)
	{
#if ENABLE_DEBUG_MODE
		float temp = this->mag.magnetic.y;
		Serial.println("DEBUG: LSM9DS0 MagY successful>>>>value: " + String(temp));
		return temp;
#else 
		return this->mag.magnetic.y;
#endif	
	}
	else
	{
		return NAN;
	}
#else 
	return NAN;
#endif
}
float ControlUnit::LSM9DS0GetMagZ()
{
#if ENABLE_LSM9DS0
	if (this->enableLSM9DS0)
	{
#if ENABLE_DEBUG_MODE
		float temp = this->mag.magnetic.z;
		Serial.println("DEBUG: LSM9DS0 MagZ successful>>>>value: " + String(temp));
		return temp;
#else 
		return this->mag.magnetic.z;
#endif
	}
	else
	{
		return NAN;
	}
#else 
	return NAN;
#endif
}
float ControlUnit::LSM9DS0GetGyroX()
{
#if ENABLE_LSM9DS0
	if (this->enableLSM9DS0)
	{
#if ENABLE_DEBUG_MODE
		float temp = this->gyro.gyro.x;
		Serial.println("DEBUG: LSM9DS0 GyroX successful>>>>value: " + String(temp));
		return temp;
#else 
		return this->gyro.gyro.x;
#endif
	}
	else
	{
		return NAN;
	}
#else 
	return NAN;
#endif
}
float ControlUnit::LSM9DS0GetGyroY()
{
#if ENABLE_LSM9DS0	
	if (this->enableLSM9DS0)
	{
#if ENABLE_DEBUG_MODE
		float temp = this->gyro.gyro.y;
		Serial.println("DEBUG: LSM9DS0 GyroY successful>>>>value: " + String(temp));
		return temp;
#else 
		return this->gyro.gyro.y;
#endif
	}
	else
	{
		return NAN;
	}
#else 
	return NAN;
#endif
}
float ControlUnit::LSM9DS0GetGyroZ()
{
#if ENABLE_LSM9DS0
	if (this->enableLSM9DS0)
	{
#if ENABLE_DEBUG_MODE
		float temp = this->gyro.gyro.z;
		Serial.println("DEBUG: LSM9DS0 GyroZ successful>>>>value: " + String(temp));
		return temp;
#else 
		return this->gyro.gyro.z;
#endif
	}
	else
	{
		return NAN;
	}
#else 
	return NAN;
#endif
}


void ControlUnit::initDS3231()
{
#if ENABLE_DS3231
	bool isEnd = false;
	unsigned long int start = millis();
	while (!isEnd)
	{
		if ((start > millis()) ? (millis() + (pow(2, 32) - 1) - start) : (millis() - start) > 10 * 1000)
		{
			isEnd = true;
			this->enableDS3231 = false;
#if  ENABLE_DEBUG_MODE
			Serial.println("ERROR  DS3231: Could not find a valid DS3231 Real Time Clock");
#endif
			this->logger(LOGACT_error_init_DS3231);

		}
		if (this->DS3231.begin())
		{
			isEnd = true;
			this->enableDS3231 = true;
#if  ENABLE_DEBUG_MODE
			Serial.println("DEBUG: DS3231 begin successful");
#endif

#if AUTO_TIME_ADJUST
			DateTime temp = DateTime(F(__DATE__), F(__TIME__));
#else
			DateTime temp = DateTime(2021, 4, 25, 0, 0, 0);
#endif

			this->DS3231.adjust(temp);
#if  ENABLE_DEBUG_MODE
			Serial.println("DateTime set: " + String(temp.year()) + "/" + String(temp.month()) + "/" + String(temp.day()) + "    " + String(temp.hour()) + ":" + String(temp.minute()) + ":" + String(temp.second()));
#endif

			this->logger(LOGACT_init_DS3231);
			_delay_ms(100);
		}
		else
		{
			isEnd = false;
			this->enableDS3231 = false;
		}
	}
#endif
}

bool ControlUnit::getDS3231status()
{
#if ENABLE_DS3231
	return this->enableDS3231;
#else 
	return false;
#endif
}

DateTime ControlUnit::DS3231getDateTime()
{
#if ENABLE_DS3231
	if (this->enableDS3231)
	{
#if  ENABLE_DEBUG_MODE
		DateTime temp = this->DS3231.now();
		Serial.println("DateTime: " + String(temp.year()) + "/" + String(temp.month()) + "/" + String(temp.day()) + "    " + String(temp.hour()) + ":" + String(temp.minute()) + ":" + String(temp.second()));
		this->ntime = temp;
		return temp;
#else 
		DateTime temp =this->DS3231.now();
		return temp;
#endif
	}
	else
	{
		this->ntime = DateTime(0, 0, 0, 0, 0, 0);
		return DateTime(0, 0, 0, 0, 0, 0);
	}
#else 
	return DateTime(0, 0, 0, 0, 0, 0);
#endif

}


void  ControlUnit::DS3231setDateTime(uint32_t timedata)
{
#if ENABLE_DS3231
	if (this->enableDS3231)
	{	
#if  ENABLE_DEBUG_MODE
		DateTime temp = DateTime(timedata);
		this->DS3231.adjust(temp);
		Serial.println("SET DateTime >>>> " + String(temp.year()) + "/" + String(temp.month()) + "/" + String(temp.day()) + "    " + String(temp.hour()) + ":" + String(temp.minute()) + ":" + String(temp.second()));
		this->ntime = temp;
		return;
#else 
		this->DS3231.adjust(timedata);
		this->ntime = temp;
		return;
#endif
	}
#endif
}

void ControlUnit::initSen0186()
{
#if ENABLE_SEN0186
	bool isEnd = false;
	unsigned long int start = millis();
	while (!isEnd)
	{
		if ((start > millis()) ? (millis() + (pow(2, 32) - 1) - start) : (millis() - start) > 10 * 1000)
		{
			isEnd = true;
			this->enableSen0186 = false;
#if ENABLE_DEBUG_MODE
			Serial.println("ERROR  SEN0186: Could not find a valid Sen0186 sensor");
#endif	
			this->logger(LOGACT_error_init_Sen0186);
		}

		if (this->Sen0186.begin())
		{
			isEnd = true;
			this->enableSen0186 = true;
#if ENABLE_DEBUG_MODE
			Serial.println("DEBUG: SEN0186 begin successful");
#endif
			this->logger(LOGACT_init_Sen0186);
		}
		else
		{
			isEnd = false;
			this->enableSen0186 = false;
		}
	}
	return;
#endif
}


void ControlUnit::Sen0186Read()
{
#if ENABLE_SEN0186
	if (this->enableSen0186)
	{
		this->Sen0186.read() ? this->logger(LOGACT_read_SEN0186) : this->logger(LOGACT_error_read_SEN0186)
#if ENABLE_DEBUG_MODE
		Serial.println("DEBUG: READ SEN0186 read successful");
#endif
	}
#else 
	return;
#endif
	return;	
}


float ControlUnit::Sen0186WindDirection()
{
#if ENABLE_SEN0186
	if (this->enableSen0186)
	{
#if ENABLE_DEBUG_MODE
		float temp = this->Sen0186.WindDirection();
		Serial.println("DEBUG: SEN0186 WindDirection successful>>>>value: " + String(temp));
		return temp;
#else 
		temp != NAN ? this->logger(LOGACT_read_wind_direction) : this->logger(LOGACT_error_read_wind_direction)
		return this->Sen0186.WindDirection();
#endif
	}
	else
	{
		return NAN;
	}
#else  ok
	return NAN;
#endif
}






float ControlUnit::Sen0186WindSpeedAverage()
{
#if ENABLE_SEN0186
	if (this->enableSen0186)
	{
#if ENABLE_DEBUG_MODE
		float temp = this->Sen0186.WindSpeedAverage();
		Serial.println("DEBUG: SEN0186 WindSpeedAverage successful>>>>value: " + String(temp));
		return temp;
#else 
		temp != NAN ? this->logger(LOGACT_read_wind_speed) : this->logger(LOGACT_error_read_wind_speed) 
#endif
	}
	else
	{
		return NAN;
	}
#else 
	return NAN;
#endif
}
float ControlUnit::Sen0186Temperature()
{
#if ENABLE_SEN0186
	if (this->enableSen0186)
	{
#if ENABLE_DEBUG_MODE
		float temp = this->Sen0186.Temperature();
		Serial.println("DEBUG: SEN0186 temperature successful>>>>value: " + String(temp));
		return temp;
#else 
		return this->Sen0186.Temperature();
#endif
	}
	else
	{
		return NAN;
	}
#else 
	return NAN;
#endif
}
float ControlUnit::Sen0186RainfallOneHour()
{
#if ENABLE_SEN0186
	if (this->enableSen0186)
	{
#if ENABLE_DEBUG_MODE
		float temp = this->Sen0186.RainfallOneHour();
		Serial.println("DEBUG: SEN0186 RainfallOneHour successful>>>>value: " + String(temp));
		return temp;
#else
		temp != NAN ? this->logger(LOGACT_read_rain_fall) : this->logger(LOGACT_error_read_rain_fall)
		return this->Sen0186.RainfallOneHour();
#endif
	}
	else
	{
		return NAN;
	}
#else 
	return NAN;
#endif
}
float ControlUnit::Sen0186Humidity()
{
#if ENABLE_SEN0186
	if (this->enableSen0186)
	{
#if ENABLE_DEBUG_MODE
		float temp = this->Sen0186.Humidity();
		Serial.println("DEBUG: SEN0186 Humidity successful>>>>value: " + String(temp));
		return temp;
#else 
		return this->Sen0186.Humidity();
#endif
	}
	else
	{
		return NAN;
	}
#else 
	return NAN;
#endif
}
float ControlUnit::Sen0186BarPressure()
{
#if ENABLE_SEN0186
	if (this->enableSen0186)
	{
#if ENABLE_DEBUG_MODE
		float temp = this->Sen0186.BarPressure();
		Serial.println("DEBUG: SEN0186 Humidity successful>>>>value: " + String(temp));
		return temp;
#else 
		return this->Sen0186.BarPressure();
#endif
	}
	else
	{
		return NAN;
	}
#else 
	return NAN;
#endif
}

bool ControlUnit::getSen0186status()
{
#if ENABLE_SEN0186
	return this->enableSen0186;
#else 
	return false;
#endif
}


void ControlUnit::readSensor()
{
	
	Serial.println("LET_A " + String(ROSY_EEPROM::LET_getQueueLength()));
	double AM2315Temperature = 0;
	double AM2315Temperature_POW = 0;
	int AM2315Temperature_TOT = 0;
	double AM2315Humidity = 0;
	double AM2315Humidity_POW = 0;
	int AM2315Humidity_TOT = 0;
	double BME280Temperature = 0;
	double BME280Temperature_POW = 0;
	int BME280Temperature_TOT = 0;

	double BME280Humidity = 0;
	double BME280Humidity_POW = 0;
	int BME280Humidity_TOT = 0;

	double BME280Pressure = 0;
	double BME280Pressure_POW = 0;
	int BME280Pressure_TOT = 0;

	double OPCN3PM1 = 0;
	double OPCN3PM1_POW = 0;
	int OPCN3PM1_TOT = 0;
	double OPCN3PM2_5 = 0;
	double OPCN3PM2_5_POW = 0;
	int OPCN3PM2_5_TOT = 0;
	double OPCN3PM10 = 0;
	double OPCN3PM10_POW = 0;
	int OPCN3PM10_TOT = 0;

	double Sen0186WindDirection = 0;
	double Sen0186WindDirection_POW = 0;
	int Sen0186WindDirection_TOT = 0;
	double Sen0186WindSpeed = 0;
	double Sen0186WindSpeed_POW = 0;
	int Sen0186WindSpeed_TOT = 0;
	double Sen0186RainFall = 0;
	double Sen0186RainFall_POW = 0;
	int Sen0186RainFall_TOT = 0;

	double LSM9DS0AccelX = 0;
	double LSM9DS0AccelX_POW = 0;
	int LSM9DS0AccelX_TOT = 0;
	double LSM9DS0AccelY = 0;
	double LSM9DS0AccelY_POW = 0;
	int LSM9DS0AccelY_TOT = 0;
	double LSM9DS0AccelZ = 0;
	double LSM9DS0AccelZ_POW = 0;
	int LSM9DS0AccelZ_TOT = 0;
	double LSM9DS0MagX = 0;
	double LSM9DS0MagX_POW = 0;
	int LSM9DS0MagX_TOT = 0;
	double LSM9DS0MagY = 0;
	double LSM9DS0MagY_POW = 0;
	int LSM9DS0MagY_TOT = 0;
	double LSM9DS0MagZ = 0;
	double LSM9DS0MagZ_POW = 0;
	int LSM9DS0MagZ_TOT = 0;
	double LSM9DS0GetGyroX = 0;
	double LSM9DS0GetGyroX_POW = 0;
	int LSM9DS0GetGyroX_TOT = 0;
	double LSM9DS0GetGyroY = 0;
	double LSM9DS0GetGyroY_POW = 0;
	int LSM9DS0GetGyroY_TOT = 0;
	double LSM9DS0GetGyroZ = 0;
	double LSM9DS0GetGyroZ_POW = 0;
	int LSM9DS0GetGyroZ_TOT = 0;

	double sensor1_1 = 0;
	double sensor1_1_POW = 0;
	int sensor1_1_TOT = 0;
	double sensor1_2 = 0;
	double sensor1_2_POW = 0;
	int sensor1_2_TOT = 0;
	
	double sensor2_1 = 0;
	double sensor2_1_POW = 0;
	int sensor2_1_TOT = 0;
	double sensor2_2 = 0;
	double sensor2_2_POW = 0;
	int sensor2_2_TOT = 0;
	
	double sensor3_1 = 0;
	double sensor3_1_POW = 0;
	int sensor3_1_TOT = 0;
	double sensor3_2 = 0;
	double sensor3_2_POW = 0;
	int sensor3_2_TOT = 0;

	double sensor4_1 = 0;
	double sensor4_1_POW = 0;
	int sensor4_1_TOT = 0;
	double sensor4_2 = 0;
	double sensor4_2_POW = 0;
	int sensor4_2_TOT = 0;

	double sensor5_1 = 0;
	double sensor5_1_POW = 0;
	int sensor5_1_TOT = 0;
	double sensor5_2 = 0;
	double sensor5_2_POW = 0;
	int sensor5_2_TOT = 0;

	for (int COUNTER = 0; COUNTER < NUMBER_OF_READ_FOR_STEP; COUNTER++)
	{
		float tmp = 0;
		_delay_ms(85000);
		this->AM2315Read();
		
		tmp = this->AM2315GetTemperature();
		if (!isnan(tmp) && tmp >= MIN_TEMPERATURE_AM2315 && tmp <= MAX_TEMPERATURE_AM2315)
		{
			AM2315Temperature += tmp;
			AM2315Temperature_POW += pow(tmp, 2);
			AM2315Temperature_TOT++;
			this->logger(LOGACT_get_ext_temp);
		}
		else this->logger(LOGACT_error_get_ext_temp);

		tmp = this->AM2315GetHumidity();
		if (!isnan(tmp) && tmp >= MIN_HUMIDITY_AM2315 && tmp <= MAX_HUMIDITY_AM2315)
		{
			AM2315Humidity += tmp;
			AM2315Humidity_POW += pow(tmp, 2);
			AM2315Humidity_TOT++;
			this->logger(LOGACT_get_ext_hum);
		}
		else this->logger(LOGACT_error_get_ext_hum);
		
		tmp = this->BME280ReadTemperature();
		if (!isnan(tmp) && tmp >= MIN_TEMPERATURE_BME280 && tmp <= MAX_TEMPERATURE_BME280)
		{
			BME280Temperature += tmp;
			BME280Temperature_POW += pow(tmp, 2);
			BME280Temperature_TOT++;
			this->logger(LOGACT_get_int_temp);
		}
		else this->logger(LOGACT_error_get_int_temp);		
		
		tmp = this->BME280ReadHumidity();
		if (!isnan(tmp) && tmp >= MIN_HUMIDITY_BME280 && tmp <= MAX_HUMIDITY_BME280)
		{
			BME280Humidity += tmp;
			BME280Humidity_POW += pow(tmp, 2);
			BME280Humidity_TOT++;
			this->logger(LOGACT_get_int_hum);
		}
		else this->logger(LOGACT_error_get_int_hum);
		
		tmp = this->BME280ReadPressure();
		if (!isnan(tmp) && tmp >= MIN_PRESSURE_BME280 && tmp <= MAX_PRESSURE_BME280)
		{
			BME280Pressure += tmp;
			BME280Pressure_POW += pow(tmp, 2);
			BME280Pressure_TOT++;
			this->logger(LOGACT_get_int_pressure);
		}
		else this->logger(LOGACT_error_get_int_pressure);
		this->OPCN3SetHistogramData();
		
		tmp = this->OPCN3PM1();
		if (!isnan(tmp) && tmp >= MIN_PM1_OPCN3 && tmp <= MAX_PM1_OPCN3)
		{
			OPCN3PM1 += tmp;
			OPCN3PM1_POW += pow(tmp, 2);
			OPCN3PM1_TOT++;
			this->logger(LOGACT_get_pm_1);
		}
		else this->logger(LOGACT_error_get_pm_1);
		
		tmp = this->OPCN3PM2_5();
		if (!isnan(tmp) && tmp >= MIN_PM2_5_OPCN3 && tmp <= MAX_PM2_5_OPCN3)
		{
			OPCN3PM2_5 += tmp;
			OPCN3PM2_5_POW += pow(tmp, 2);
			OPCN3PM2_5_TOT++;
			this->logger(LOGACT_get_pm_2_5);
		}
		else this->logger(LOGACT_error_get_pm_2_5);
		
		tmp = this->OPCN3PM10();
		if (!isnan(tmp) && tmp >= MIN_PM10_OPCN3 && tmp <= MAX_PM10_OPCN3)
		{
			OPCN3PM10 += tmp;
			OPCN3PM10_POW += pow(tmp, 2);
			OPCN3PM10_TOT++;
			this->logger(LOGACT_get_pm_10);
		}
		else this->logger(LOGACT_error_get_pm_10);
		this->Sen0186Read();
		
		tmp = this->Sen0186WindDirection();
		if (!isnan(tmp) && tmp >= MIN_SEN0186_WIND_DIRECTION && tmp <= MAX_SEN0186_WIND_DIRECTION)
		{
			Sen0186WindDirection += tmp;
			Sen0186WindDirection_POW += pow(tmp, 2);
			Sen0186WindDirection_TOT++;
			this->logger(LOGACT_get_wind_direction);
		}
		else this->logger(LOGACT_error_get_wind_direction);
		
		tmp = this->Sen0186WindSpeedAverage();
		if (!isnan(tmp) && tmp >= MIN_SEN0186_WIND_SPEED && tmp <= MAX_SEN0186_WIND_SPEED)
		{
			Sen0186WindSpeed += tmp;
			Sen0186WindSpeed_POW += pow(tmp, 2);
			Sen0186WindSpeed_TOT++;
			this->logger(LOGACT_get_wind_speed);
		}
		else this->logger(LOGACT_error_get_wind_speed);
		
		tmp = this->Sen0186RainfallOneHour();
		if (!isnan(tmp) && tmp >= MIN_SEN0186_RAIN_FALL_ONE_HOURS && tmp <= MAX_SEN0186_RAIN_FALL_ONE_HOURS)
		{
			Sen0186RainFall += tmp;
			Sen0186RainFall_POW += pow(tmp, 2);
			Sen0186RainFall_TOT++;
			this->logger(LOGACT_get_rain_fall);
		}
		else this->logger(LOGACT_error_get_rain_fall);
		this->LSM9DS0Read();
		
		tmp = this->LSM9DS0GetAccelX();
		if (!isnan(tmp) && tmp >= MIN_LSM9DS0_ACCELRANGE_2G && tmp <= MAX_LSM9DS0_ACCELRANGE_2G)
		{
			LSM9DS0AccelX += tmp;
			LSM9DS0AccelX_POW += pow(tmp, 2);
			LSM9DS0AccelX_TOT++;
			this->logger(LOGACT_get_acc_x);
		}
		else this->logger(LOGACT_error_get_acc_x);
		
		tmp = this->LSM9DS0GetAccelY();
		if (!isnan(tmp) && tmp >= MIN_LSM9DS0_ACCELRANGE_2G && tmp <= MAX_LSM9DS0_ACCELRANGE_2G)
		{
			LSM9DS0AccelY += tmp;
			LSM9DS0AccelY_POW += pow(tmp, 2);
			LSM9DS0AccelY_TOT++;
			this->logger(LOGACT_get_acc_y);
		}
		else this->logger(LOGACT_error_get_acc_y);
		
		tmp = this->LSM9DS0GetAccelZ();
		if (!isnan(tmp) && tmp >= MIN_LSM9DS0_ACCELRANGE_2G && tmp <= MAX_LSM9DS0_ACCELRANGE_2G)
		{
			LSM9DS0AccelZ += tmp;
			LSM9DS0AccelZ_POW += pow(tmp, 2);
			LSM9DS0AccelZ_TOT++;
			this->logger(LOGACT_get_acc_z);
		}
		else this->logger(LOGACT_error_get_acc_z);
		
		tmp = this->LSM9DS0GetMagX();
		if (!isnan(tmp) && tmp >= MIN_LSM9DS0_MAGGAIN_4GAUSS && tmp <= MAX_LSM9DS0_MAGGAIN_4GAUSS)
		{
			LSM9DS0MagX += tmp;
			LSM9DS0MagX_POW += pow(tmp, 2);
			LSM9DS0MagX_TOT++;
			this->logger(LOGACT_get_mag_x);
		}
		else this->logger(LOGACT_error_get_mag_x);
		
		tmp = this->LSM9DS0GetMagY();
		if (!isnan(tmp) && tmp >= MIN_LSM9DS0_MAGGAIN_4GAUSS && tmp <= MAX_LSM9DS0_MAGGAIN_4GAUSS)
		{
			LSM9DS0MagY += tmp;
			LSM9DS0MagY_POW += pow(tmp, 2);
			LSM9DS0MagY_TOT++;
			this->logger(LOGACT_get_mag_y);
		}
		else this->logger(LOGACT_error_get_mag_y);
		
		tmp = this->LSM9DS0GetMagZ();
		if (!isnan(tmp) && tmp >= MIN_LSM9DS0_MAGGAIN_4GAUSS && tmp <= MAX_LSM9DS0_MAGGAIN_4GAUSS)
		{
			LSM9DS0MagZ += tmp;
			LSM9DS0MagZ_POW += pow(tmp, 2);
			LSM9DS0MagZ_TOT++;
			this->logger(LOGACT_get_mag_z);
		}
		else this->logger(LOGACT_error_get_mag_z);
		
		tmp = this->LSM9DS0GetGyroX();
		if (!isnan(tmp) && tmp >= MIN_LSM9DS0_GYROSCALE_245DPS && tmp <= MAX_LSM9DS0_GYROSCALE_245DPS) {
			LSM9DS0GetGyroX += tmp;
			LSM9DS0GetGyroX_POW += pow(tmp, 2);
			LSM9DS0GetGyroX_TOT++;
			this->logger(LOGACT_get_gyro_x);
		}
		else this->logger(LOGACT_error_get_gyro_x);
		
		tmp = this->LSM9DS0GetGyroY();
		if (!isnan(tmp) && tmp >= MIN_LSM9DS0_GYROSCALE_245DPS && tmp <= MAX_LSM9DS0_GYROSCALE_245DPS)
		{
			LSM9DS0GetGyroY += tmp;
			LSM9DS0GetGyroY_POW += pow(tmp, 2);
			LSM9DS0GetGyroY_TOT++;
			this->logger(LOGACT_get_gyro_y);
		}
		else this->logger(LOGACT_error_get_gyro_y);
		
		tmp = this->LSM9DS0GetGyroZ();
		if (!isnan(tmp) && tmp >= MIN_LSM9DS0_GYROSCALE_245DPS && tmp <= MAX_LSM9DS0_GYROSCALE_245DPS)
		{
			LSM9DS0GetGyroZ += tmp;
			LSM9DS0GetGyroZ_POW += pow(tmp, 2);
			LSM9DS0GetGyroZ_TOT++;
			this->logger(LOGACT_get_gyro_z);
		}
		else this->logger(LOGACT_error_get_gyro_z);
		
		tmp = this->readSensor1_1();
		if (!isnan(tmp) && tmp >= MIN_VOLTAGE_ADS1115_1_mV && tmp <= MAX_VOLTAGE_ADS1115_1_mV)
		{
			sensor1_1 += tmp;
			sensor1_1_POW += pow(tmp, 2);
			sensor1_1_TOT++;
			this->logger(LOGACT_get_o3_1_1);
		}
		else this->logger(LOGACT_error_get_o3_1_1);
		
		tmp = this->readSensor1_2();
		if (!isnan(tmp) && tmp >= MIN_VOLTAGE_ADS1115_1_mV && tmp <= MAX_VOLTAGE_ADS1115_1_mV)
		{
			sensor1_2 += tmp;
			sensor1_2_POW += pow(tmp, 2);
			sensor1_2_TOT++;
			this->logger(LOGACT_get_o3_1_2);
		}
		else this->logger(LOGACT_error_get_o3_1_2);
		
		tmp = this->readSensor1_3();
		if (!isnan(tmp) && tmp >= MIN_VOLTAGE_ADS1115_1_mV && tmp <= MAX_VOLTAGE_ADS1115_1_mV)
		{
			sensor2_1 += tmp;
			sensor2_1_POW += pow(tmp, 2);
			sensor2_1_TOT++;
			this->logger(LOGACT_get_o3_1_3);
		}
		else this->logger(LOGACT_error_get_o3_1_3);
		
		tmp = this->readSensor1_4();
		if (!isnan(tmp) && tmp >= MIN_VOLTAGE_ADS1115_1_mV && tmp <= MAX_VOLTAGE_ADS1115_1_mV)
		{
			sensor2_2 += tmp;
			sensor2_2_POW += pow(tmp, 2);
			sensor2_2_TOT++;
			this->logger(LOGACT_get_o3_1_4);
		}
		else this->logger(LOGACT_error_get_o3_1_4);
		
		tmp = this->readSensor2_1();
		if (!isnan(tmp) && tmp >= MIN_VOLTAGE_ADS1115_2_mV && tmp <= MAX_VOLTAGE_ADS1115_2_mV)
		{
			sensor3_1 += tmp;
			sensor3_1_POW += pow(tmp, 2);
			sensor3_1_TOT++;
			this->logger(LOGACT_get_o3_2_1);
		}
		else this->logger(LOGACT_error_get_o3_2_1);
		
		tmp = this->readSensor2_2();
		if (!isnan(tmp) && tmp >= MIN_VOLTAGE_ADS1115_2_mV && tmp <= MAX_VOLTAGE_ADS1115_2_mV)
		{
			sensor3_2 += tmp;
			sensor3_2_POW += pow(tmp, 2);
			sensor3_2_TOT++;
			this->logger(LOGACT_get_o3_2_2);
		}
		else this->logger(LOGACT_error_get_o3_2_2);
		
		tmp = this->readSensor2_3();
		if (!isnan(tmp) && tmp >= MIN_VOLTAGE_ADS1115_2_mV && tmp <= MAX_VOLTAGE_ADS1115_2_mV)
		{
			sensor4_1 += tmp;
			sensor4_1_POW += pow(tmp, 2);
			sensor4_1_TOT++;
			this->logger(LOGACT_get_o3_2_3);
		}
		else this->logger(LOGACT_error_get_o3_2_3);
		
		tmp = this->readSensor2_4();
		if (!isnan(tmp) && tmp >= MIN_VOLTAGE_ADS1115_2_mV && tmp <= MAX_VOLTAGE_ADS1115_2_mV)
		{
			sensor4_2 += tmp;
			sensor4_2_POW += pow(tmp, 2);
			sensor4_2_TOT++;
			this->logger(LOGACT_get_o3_2_4);
		}
		else this->logger(LOGACT_error_get_o3_2_4);
	}
	DateTime dt = this->DS3231getDateTime();
	ROSY_EEPROM::BUFF_setTime(dt.unixtime());
	ROSY_EEPROM::BUFF_setTemperature_M
	(
		ROSY::Compressor::floatToIntEncode(ROSY::Statistics::calcAverage<double>(AM2315Temperature, AM2315Temperature_TOT), MIN_TEMPERATURE_AM2315, MAX_TEMPERATURE_AM2315),
		ROSY::Compressor::floatToIntEncode(ROSY::Statistics::calcAverage<double>(BME280Temperature, BME280Temperature_TOT), MIN_TEMPERATURE_BME280, MAX_TEMPERATURE_BME280)
	);

	ROSY_EEPROM::BUFF_setTemperature_SD
	(
		ROSY::Compressor::floatToIntEncode(ROSY::Statistics::calcMSD<double>(AM2315Temperature, AM2315Temperature_POW, AM2315Temperature_TOT), MIN_MSD_TEMPERATURE_AM2315, MAX_MSD_TEMPERATURE_AM2315),
		ROSY::Compressor::floatToIntEncode(ROSY::Statistics::calcMSD<double>(BME280Temperature, BME280Temperature_POW, BME280Temperature_TOT), MIN_MSD_TEMPERATURE_BME280, MAX_MSD_TEMPERATURE_BME280)
	);
	
	ROSY_EEPROM::BUFF_setHumidity_M
	(
		ROSY::Compressor::floatToIntEncode(ROSY::Statistics::calcAverage<double>(AM2315Humidity, AM2315Humidity_TOT), MIN_HUMIDITY_AM2315, MAX_HUMIDITY_AM2315),
		ROSY::Compressor::floatToIntEncode(ROSY::Statistics::calcAverage<double>(BME280Humidity, BME280Humidity_TOT), MIN_HUMIDITY_BME280, MAX_HUMIDITY_BME280)
	);
	
	ROSY_EEPROM::BUFF_setHumidity_SD
	(
		ROSY::Compressor::floatToIntEncode(ROSY::Statistics::calcMSD<double>(AM2315Humidity, AM2315Humidity_POW, AM2315Humidity_TOT), MIN_MSD_HUMIDITY_AM2315, MAX_MSD_HUMIDITY_AM2315),
		ROSY::Compressor::floatToIntEncode(ROSY::Statistics::calcMSD<double>(BME280Humidity, BME280Humidity_POW, BME280Humidity_TOT), MIN_MSD_HUMIDITY_BME280, MAX_MSD_HUMIDITY_BME280)
	);
	
	ROSY_EEPROM::BUFF_setPressure_M
	(
		ROSY::Compressor::floatToIntEncode(ROSY::Statistics::calcAverage<double>(BME280Pressure, BME280Pressure_TOT), MIN_PRESSURE_BME280, MAX_PRESSURE_BME280)
	);
	
	ROSY_EEPROM::BUFF_setPressure_SD
	(
		ROSY::Compressor::floatToIntEncode(ROSY::Statistics::calcMSD<double>(BME280Pressure, BME280Pressure_POW, BME280Pressure_TOT), MIN_MSD_PRESSURE_BME280, MAX_MSD_PRESSURE_BME280)
	);
	
	ROSY_EEPROM::BUFF_setPM_M
	(
		ROSY::Compressor::floatToIntEncode(ROSY::Statistics::calcAverage<double>(OPCN3PM1, OPCN3PM1_TOT), MIN_PM1_OPCN3, MAX_PM1_OPCN3),
		ROSY::Compressor::floatToIntEncode(ROSY::Statistics::calcAverage<double>(OPCN3PM2_5, OPCN3PM2_5_TOT), MIN_PM2_5_OPCN3, MAX_PM2_5_OPCN3),
		ROSY::Compressor::floatToIntEncode(ROSY::Statistics::calcAverage<double>(OPCN3PM10, OPCN3PM10_TOT), MIN_PM10_OPCN3, MAX_PM10_OPCN3)
	);
	
	ROSY_EEPROM::BUFF_setPM_SD
	(
		ROSY::Compressor::floatToIntEncode(ROSY::Statistics::calcMSD<double>(OPCN3PM1, OPCN3PM1_POW, OPCN3PM1_TOT), MIN_MSD_PM1_OPCN3, MAX_MSD_PM1_OPCN3),
		ROSY::Compressor::floatToIntEncode(ROSY::Statistics::calcMSD<double>(OPCN3PM2_5, OPCN3PM2_5_POW, OPCN3PM2_5_TOT), MIN_MSD_PM2_5_OPCN3, MAX_MSD_PM2_5_OPCN3),
		ROSY::Compressor::floatToIntEncode(ROSY::Statistics::calcMSD<double>(OPCN3PM10, OPCN3PM10_POW, OPCN3PM10_TOT), MIN_MSD_PM10_OPCN3, MAX_MSD_PM10_OPCN3)
	);
	
	ROSY_EEPROM::BUFF_setOzone_M
	(
		ROSY::Compressor::floatToIntEncode(ROSY::Statistics::calcAverage<double>(sensor1_1, sensor1_1_TOT), MIN_VOLTAGE_ADS1115_1_mV, MAX_VOLTAGE_ADS1115_1_mV),
		ROSY::Compressor::floatToIntEncode(ROSY::Statistics::calcAverage<double>(sensor1_2, sensor1_2_TOT), MIN_VOLTAGE_ADS1115_1_mV, MAX_VOLTAGE_ADS1115_1_mV),
		ROSY::Compressor::floatToIntEncode(ROSY::Statistics::calcAverage<double>(sensor2_1, sensor2_1_TOT), MIN_VOLTAGE_ADS1115_1_mV, MAX_VOLTAGE_ADS1115_1_mV),
		ROSY::Compressor::floatToIntEncode(ROSY::Statistics::calcAverage<double>(sensor2_2, sensor2_2_TOT), MIN_VOLTAGE_ADS1115_1_mV, MAX_VOLTAGE_ADS1115_1_mV),
		ROSY::Compressor::floatToIntEncode(ROSY::Statistics::calcAverage<double>(sensor3_1, sensor3_1_TOT), MIN_VOLTAGE_ADS1115_2_mV, MAX_VOLTAGE_ADS1115_2_mV),
		ROSY::Compressor::floatToIntEncode(ROSY::Statistics::calcAverage<double>(sensor3_2, sensor3_2_TOT), MIN_VOLTAGE_ADS1115_2_mV, MAX_VOLTAGE_ADS1115_2_mV),
		ROSY::Compressor::floatToIntEncode(ROSY::Statistics::calcAverage<double>(sensor4_1, sensor4_1_TOT), MIN_VOLTAGE_ADS1115_2_mV, MAX_VOLTAGE_ADS1115_2_mV),
		ROSY::Compressor::floatToIntEncode(ROSY::Statistics::calcAverage<double>(sensor4_2, sensor4_2_TOT), MIN_VOLTAGE_ADS1115_2_mV, MAX_VOLTAGE_ADS1115_2_mV)
	);
	
	ROSY_EEPROM::BUFF_setOzone_SD
	(
		ROSY::Compressor::floatToIntEncode(ROSY::Statistics::calcMSD<double>(sensor1_1, sensor1_1_POW, sensor1_1_TOT), MIN_MSD_VOLTAGE_ADS1115_1_mV, MAX_MSD_VOLTAGE_ADS1115_1_mV),
		ROSY::Compressor::floatToIntEncode(ROSY::Statistics::calcMSD<double>(sensor1_2, sensor1_2_POW, sensor1_2_TOT), MIN_MSD_VOLTAGE_ADS1115_1_mV, MAX_MSD_VOLTAGE_ADS1115_1_mV),
		ROSY::Compressor::floatToIntEncode(ROSY::Statistics::calcMSD<double>(sensor2_1, sensor2_1_POW, sensor2_1_TOT), MIN_MSD_VOLTAGE_ADS1115_1_mV, MAX_MSD_VOLTAGE_ADS1115_1_mV),
		ROSY::Compressor::floatToIntEncode(ROSY::Statistics::calcMSD<double>(sensor2_2, sensor2_2_POW, sensor2_2_TOT), MIN_MSD_VOLTAGE_ADS1115_1_mV, MAX_MSD_VOLTAGE_ADS1115_1_mV),
		ROSY::Compressor::floatToIntEncode(ROSY::Statistics::calcMSD<double>(sensor3_1, sensor3_1_POW, sensor3_1_TOT), MIN_MSD_VOLTAGE_ADS1115_2_mV, MAX_MSD_VOLTAGE_ADS1115_2_mV),
		ROSY::Compressor::floatToIntEncode(ROSY::Statistics::calcMSD<double>(sensor3_2, sensor3_2_POW, sensor3_2_TOT), MIN_MSD_VOLTAGE_ADS1115_2_mV, MAX_MSD_VOLTAGE_ADS1115_2_mV),
		ROSY::Compressor::floatToIntEncode(ROSY::Statistics::calcMSD<double>(sensor4_1, sensor4_1_POW, sensor4_1_TOT), MIN_MSD_VOLTAGE_ADS1115_2_mV, MAX_MSD_VOLTAGE_ADS1115_2_mV),
		ROSY::Compressor::floatToIntEncode(ROSY::Statistics::calcMSD<double>(sensor4_2, sensor4_2_POW, sensor4_2_TOT), MIN_MSD_VOLTAGE_ADS1115_2_mV, MAX_MSD_VOLTAGE_ADS1115_2_mV)
	);
	
	ROSY_EEPROM::BUFF_setAccel_M
	(
		ROSY::Compressor::floatToIntEncode(ROSY::Statistics::calcAverage<double>(LSM9DS0AccelX, LSM9DS0AccelX_TOT), MIN_LSM9DS0_ACCELRANGE_2G, MAX_LSM9DS0_ACCELRANGE_2G),
		ROSY::Compressor::floatToIntEncode(ROSY::Statistics::calcAverage<double>(LSM9DS0AccelY, LSM9DS0AccelY_TOT), MIN_LSM9DS0_ACCELRANGE_2G, MAX_LSM9DS0_ACCELRANGE_2G),
		ROSY::Compressor::floatToIntEncode(ROSY::Statistics::calcAverage<double>(LSM9DS0AccelZ, LSM9DS0AccelZ_TOT), MIN_LSM9DS0_ACCELRANGE_2G, MAX_LSM9DS0_ACCELRANGE_2G)
	);
	
	ROSY_EEPROM::BUFF_setAccel_SD
	(
		ROSY::Compressor::floatToIntEncode(ROSY::Statistics::calcMSD<double>(LSM9DS0AccelX, LSM9DS0AccelX_POW, LSM9DS0AccelX_TOT), MIN_MSD_LSM9DS0_ACCELRANGE_2G, MAX_MSD_LSM9DS0_ACCELRANGE_2G),
		ROSY::Compressor::floatToIntEncode(ROSY::Statistics::calcMSD<double>(LSM9DS0AccelY, LSM9DS0AccelY_POW, LSM9DS0AccelY_TOT), MIN_MSD_LSM9DS0_ACCELRANGE_2G, MAX_MSD_LSM9DS0_ACCELRANGE_2G),
		ROSY::Compressor::floatToIntEncode(ROSY::Statistics::calcMSD<double>(LSM9DS0AccelZ, LSM9DS0AccelZ_POW, LSM9DS0AccelZ_TOT), MIN_MSD_LSM9DS0_ACCELRANGE_2G, MAX_MSD_LSM9DS0_ACCELRANGE_2G)
	);
	
	ROSY_EEPROM::BUFF_setMag_M
	(
		ROSY::Compressor::floatToIntEncode(ROSY::Statistics::calcAverage<double>(LSM9DS0MagX, LSM9DS0MagX_TOT), MIN_LSM9DS0_MAGGAIN_2GAUSS, MAX_LSM9DS0_MAGGAIN_2GAUSS),
		ROSY::Compressor::floatToIntEncode(ROSY::Statistics::calcAverage<double>(LSM9DS0MagY, LSM9DS0MagY_TOT), MIN_LSM9DS0_MAGGAIN_2GAUSS, MAX_LSM9DS0_MAGGAIN_2GAUSS),
		ROSY::Compressor::floatToIntEncode(ROSY::Statistics::calcAverage<double>(LSM9DS0MagZ, LSM9DS0MagZ_TOT), MIN_LSM9DS0_MAGGAIN_2GAUSS, MAX_LSM9DS0_MAGGAIN_2GAUSS)
	);
	
	ROSY_EEPROM::BUFF_setMag_SD
	(
		ROSY::Compressor::floatToIntEncode(ROSY::Statistics::calcMSD<double>(LSM9DS0MagX, LSM9DS0MagX_POW, LSM9DS0MagX_TOT), MIN_MSD_LSM9DS0_MAGGAIN_2GAUSS, MAX_MSD_LSM9DS0_MAGGAIN_2GAUSS),
		ROSY::Compressor::floatToIntEncode(ROSY::Statistics::calcMSD<double>(LSM9DS0MagY, LSM9DS0MagY_POW, LSM9DS0MagY_TOT), MIN_MSD_LSM9DS0_MAGGAIN_2GAUSS, MAX_MSD_LSM9DS0_MAGGAIN_2GAUSS),
		ROSY::Compressor::floatToIntEncode(ROSY::Statistics::calcMSD<double>(LSM9DS0MagZ, LSM9DS0MagZ_POW, LSM9DS0MagZ_TOT), MIN_MSD_LSM9DS0_MAGGAIN_2GAUSS, MAX_MSD_LSM9DS0_MAGGAIN_2GAUSS)
	);
	
	ROSY_EEPROM::BUFF_setGyro_M
	(
		ROSY::Compressor::floatToIntEncode(ROSY::Statistics::calcAverage<double>(LSM9DS0GetGyroX, LSM9DS0GetGyroX_TOT), MIN_LSM9DS0_GYROSCALE_245DPS, MAX_LSM9DS0_GYROSCALE_245DPS),
		ROSY::Compressor::floatToIntEncode(ROSY::Statistics::calcAverage<double>(LSM9DS0GetGyroY, LSM9DS0GetGyroY_TOT), MIN_LSM9DS0_GYROSCALE_245DPS, MAX_LSM9DS0_GYROSCALE_245DPS),
		ROSY::Compressor::floatToIntEncode(ROSY::Statistics::calcAverage<double>(LSM9DS0GetGyroZ, LSM9DS0GetGyroZ_TOT), MIN_LSM9DS0_GYROSCALE_245DPS, MAX_LSM9DS0_GYROSCALE_245DPS)
	);
	
	ROSY_EEPROM::BUFF_setGyro_SD
	(
		ROSY::Compressor::floatToIntEncode(ROSY::Statistics::calcMSD<double>(LSM9DS0GetGyroX, LSM9DS0GetGyroX_POW, LSM9DS0GetGyroX_TOT), MIN_MSD_LSM9DS0_GYROSCALE_245DPS, MAX_MSD_LSM9DS0_GYROSCALE_245DPS),
		ROSY::Compressor::floatToIntEncode(ROSY::Statistics::calcMSD<double>(LSM9DS0GetGyroY, LSM9DS0GetGyroY_POW, LSM9DS0GetGyroY_TOT), MIN_MSD_LSM9DS0_GYROSCALE_245DPS, MAX_MSD_LSM9DS0_GYROSCALE_245DPS),
		ROSY::Compressor::floatToIntEncode(ROSY::Statistics::calcMSD<double>(LSM9DS0GetGyroZ, LSM9DS0GetGyroZ_POW, LSM9DS0GetGyroZ_TOT), MIN_MSD_LSM9DS0_GYROSCALE_245DPS, MAX_MSD_LSM9DS0_GYROSCALE_245DPS)
	);
	
	ROSY_EEPROM::BUFF_setWindRain_M
	(
		ROSY::Compressor::floatToIntEncode(ROSY::Statistics::calcAverage<double>(Sen0186WindDirection, Sen0186WindDirection_TOT), MIN_SEN0186_WIND_DIRECTION, MAX_SEN0186_WIND_DIRECTION),
		ROSY::Compressor::floatToIntEncode(ROSY::Statistics::calcAverage<double>(Sen0186WindSpeed, Sen0186WindSpeed_TOT), MIN_SEN0186_WIND_SPEED, MAX_SEN0186_WIND_SPEED),
		ROSY::Compressor::floatToIntEncode(ROSY::Statistics::calcAverage<double>(Sen0186RainFall, Sen0186RainFall_TOT), MIN_SEN0186_RAIN_FALL_ONE_HOURS, MAX_SEN0186_RAIN_FALL_ONE_HOURS)
	);
	
	ROSY_EEPROM::BUFF_setWindRain_SD
	(
		ROSY::Compressor::floatToIntEncode(ROSY::Statistics::calcMSD<double>(Sen0186WindDirection, Sen0186WindDirection_POW, Sen0186WindDirection_TOT), MIN_MSD_SEN0186_WIND_DIRECTION, MAX_MSD_SEN0186_WIND_DIRECTION),
		ROSY::Compressor::floatToIntEncode(ROSY::Statistics::calcMSD<double>(Sen0186WindSpeed, Sen0186WindSpeed_POW, Sen0186WindSpeed_TOT), MIN_MSD_SEN0186_WIND_SPEED, MAX_MSD_SEN0186_WIND_SPEED),
		ROSY::Compressor::floatToIntEncode(ROSY::Statistics::calcMSD<double>(Sen0186RainFall, Sen0186RainFall_POW, Sen0186RainFall_TOT), MIN_MSD_SEN0186_RAIN_FALL_ONE_HOUR, MAX_MSD_SEN0186_RAIN_FALL_ONE_HOUR)
	);
	
	ROSY_EEPROM::LET_appendBufferToQueue();
	
	float RSSI = 0;
	float SN = 0;
	ROSY_Lora_End::LoRa_getINFO(&SN, &RSSI);
	ROSY_EEPROM::INF_add
	(
		ROSY::Compressor::floatToIntEncode(this->getBatteryVoltage(), MIN_BATTERY_VOLTAGE_V, MAX_BATTERY_VOLTAGE_V),
		ROSY::Compressor::floatToIntEncode(this->getBatteryCurrent(), MIN_BATTERY_CURRENT_mA, MAX_BATTERY_CURRENT_mA),
		ROSY::Compressor::floatToIntEncode(SN, MIN_LoRa_SN, MAX_LoRa_SN),
		ROSY::Compressor::floatToIntEncode(RSSI, MIN_LoRa_RSSI, MAX_LoRa_RSSI),
		dt.unixtime()
	);
#if ENABLE_DEBUG_MODE
	Serial.println("LET_B " + String(ROSY_EEPROM::LET_getQueueLength()));
#endif
}
