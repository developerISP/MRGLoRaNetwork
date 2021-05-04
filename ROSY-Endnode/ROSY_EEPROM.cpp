#include "ROSY_EEPROM.h"


void ROSY_EEPROM::AT24C256::writeByte(uint16_t memoryAddress, uint8_t data)
{
	if(memoryAddress >= __EEPROM_LENGTH__) return;
#if EEPROM_RAM
	queue[memoryAddress] = data;
#else
	Wire.beginTransmission(EEPROM_I2C_ADDRESS);
	Wire.write(memoryAddress >> 8);
	Wire.write(memoryAddress & 255);
	Wire.write(data);
	Wire.endTransmission();
#endif
	_delay_ms(EEPROM_TIME_WRITE);
}
void ROSY_EEPROM::AT24C256::writeArray(uint16_t memoryAddress, uint8_t data[], uint16_t arrayLength)
{
	for (int i = 0; i < arrayLength; i++)
		writeByte(i + memoryAddress, data[i]);
}
uint8_t ROSY_EEPROM::AT24C256::readByte(uint16_t memoryAddress)
{
	if(memoryAddress >= __EEPROM_LENGTH__) return;
#if EEPROM_RAM
	return queue[memoryAddress];
#else
	Wire.beginTransmission(EEPROM_I2C_ADDRESS);
	Wire.write(memoryAddress >> 8);
	Wire.write(memoryAddress & 255);
	Wire.endTransmission();
	Wire.requestFrom(EEPROM_I2C_ADDRESS, 1);
	for (uint8_t idx = 0; idx < EEPROM_MAX_FAILED_READ; idx++) {
		_delay_ms(EEPROM_TIME_READ);
		if (Wire.available()) {
			return Wire.read();
		}
	}
	return 0;
#endif
}


bool ROSY_EEPROM::resetEEPROM()
{
	for(uint16_t i = 0; i < __EEPROM_LENGTH__; i++) 
	{
#if ENABLE_DEBUG_MODE
		Serial.println("Scritti" + String(i) + "Byte su " + String(__EEPROM_LENGTH__) + "Byte");
#endif
		ROSY_EEPROM::AT24C256::writeByte(i, 0);
	}
}

bool ROSY_EEPROM::readAllEEPROM()
{
	for(uint16_t i = 0; i < __EEPROM_LENGTH__; i++) 
	{
#if ENABLE_DEBUG_MODE
		Serial.print(String(ROSY_EEPROM::AT24C256::readByte(i)) + String(" "));
#endif
		if( i % 64 == 0 ) Serial.println();
	}
}

void ROSY_EEPROM::setLogger(void(*logCB)(uint8_t)) {
	logger = logCB;
}

void ROSY_EEPROM::LET_init() {
	bool startFound=false;
	bool stopFound=false;
	int16_t startADDR = 0;
	int16_t stopADDR = 0;
	uint8_t truncADDR = 0;
	for (int i = 0; i < QUEUE_EEPROM_TOT_DATA; i++) {
		if ((AT24C256::readByte(QUEUE_EEPROM_OFFSET + DIM_EEPROM_BUFF * i) & ADDR_STOP) == ADDR_STOP) {
			Serial.println(String(i)+" "+String(AT24C256::readByte(QUEUE_EEPROM_OFFSET + DIM_EEPROM_BUFF * i)));
			if (stopFound) {
				stopFound=false;
				startFound=false;
				Serial.println("a");
				break;
			}
			else {
				stopFound=true;
				stopADDR = QUEUE_EEPROM_OFFSET + DIM_EEPROM_BUFF * (i + 1) - 1;
				if ((AT24C256::readByte(QUEUE_EEPROM_OFFSET + DIM_EEPROM_BUFF * i) & ADDR_TRUNC) == ADDR_TRUNC) {
					if (truncADDR == 3) {
						truncADDR = 4;
					}
					else
					{
						if (truncADDR != 1 && truncADDR != 2) {
							truncADDR = 3;
						}
						else {
							truncADDR = 0;
							stopFound=false;
							startFound=false;
							Serial.println("b");
							break;
						}
					}
				}
				else {
					if (truncADDR == 1) {
						truncADDR = 2;
					}
					else
					{
						if (truncADDR != 3 && truncADDR != 4) {
							truncADDR = 1;
						}
						else {
							truncADDR = 0;
							stopFound=false;
							startFound=false;
							Serial.println("c");
							break;
						}
					}
				}
			}
		}
		if ((AT24C256::readByte(QUEUE_EEPROM_OFFSET + DIM_EEPROM_BUFF * i) & ADDR_START) == ADDR_START) {
			if (startFound) {
				stopFound=false;
				startFound=false;
				Serial.println("d");
				break;
			}
			else {
				startFound=true;
				startADDR = QUEUE_EEPROM_OFFSET + DIM_EEPROM_BUFF * i;
				if ((AT24C256::readByte(QUEUE_EEPROM_OFFSET + DIM_EEPROM_BUFF * i) & ADDR_TRUNC) == ADDR_TRUNC) {
					if (truncADDR == 3) {
						truncADDR = 4;
					}
					else
					{
						if (truncADDR != 1 && truncADDR != 2) {
							truncADDR = 3;
						}
						else {
							truncADDR = 0;
							stopFound=false;
							startFound=false;
							Serial.println("e");
							break;
						}
					}
				}
				else {
					if (truncADDR == 1) {
						truncADDR = 2;
					}
					else
					{
						if (truncADDR != 3 && truncADDR != 4) {
							truncADDR = 1;
						}
						else {
							truncADDR = 0;
							stopFound=false;
							startFound=false;
							Serial.println("f");
							break;
						}
					}
				}
			}
		}
	}	
	if ((truncADDR == 2 || truncADDR == 4) && startFound && stopFound) {
		QUEUEaddrStart = startADDR;
		QUEUEaddrFinish = stopADDR; 
		if (truncADDR == 4) {
			QUEUEtrunc = 1;
		}
		else {
			QUEUEtrunc = 0;
		}
	}
	else {
		QUEUEaddrStart = QUEUE_EEPROM_OFFSET;
		QUEUEaddrFinish = QUEUE_EEPROM_OFFSET;
		QUEUEtrunc = 0;
		for (int i = 0; i < QUEUE_EEPROM_TOT_DATA; i++) {
			AT24C256::writeByte(QUEUE_EEPROM_OFFSET + DIM_EEPROM_BUFF * i, 0);
		}
	}
}

void ROSY_EEPROM::LET_appendBufferToQueue() {
	if (isEEPROMcom) {
		Serial.println("LETa cattivo");
	}
	isEEPROMcom = true;
	if (QUEUEaddrFinish - DIM_EEPROM_BUFF + 1 >= QUEUE_EEPROM_OFFSET) {
		AT24C256::writeByte(QUEUEaddrFinish - DIM_EEPROM_BUFF + 1, ADDR_NULL);
	}
	if (QUEUEaddrFinish >= QUEUE_EEPROM_OFFSET + DIM_EEPROM_BUFF * QUEUE_EEPROM_TOT_DATA - 1) {
		QUEUEaddrFinish = QUEUE_EEPROM_OFFSET;
		QUEUEtrunc = 1;
	}
	uint8_t off = (QUEUEaddrFinish == QUEUE_EEPROM_OFFSET ? 0 : 1);
	for (int i = 0; i < DIM_EEPROM_BUFF-1; i++) {
		AT24C256::writeByte(QUEUEaddrFinish + i + 1 + off, buffer[i]);
	}
	QUEUEaddrFinish += DIM_EEPROM_BUFF - 1 + off;
	if (QUEUEaddrFinish >= QUEUEaddrStart && QUEUEtrunc) {
		AT24C256::writeByte(QUEUEaddrStart, ADDR_NULL);
		QUEUEaddrStart += DIM_EEPROM_BUFF;
		if (QUEUEaddrStart >= QUEUE_EEPROM_OFFSET + DIM_EEPROM_BUFF * QUEUE_EEPROM_TOT_DATA) {
			if (QUEUEtrunc == 0) {
				QUEUEaddrStart = QUEUE_EEPROM_OFFSET;
				QUEUEaddrFinish = QUEUE_EEPROM_OFFSET;
				QUEUEtrunc = 0;
			}
			else {
				QUEUEaddrStart = QUEUE_EEPROM_OFFSET;
				QUEUEtrunc = 0;
			}
		}
	}
	if (QUEUEaddrStart != QUEUEaddrFinish + 1 && QUEUEaddrStart != QUEUEaddrFinish) {
		if (QUEUEaddrStart != (QUEUEaddrFinish - DIM_EEPROM_BUFF + 1)) {
			AT24C256::writeByte(QUEUEaddrStart, ADDR_START | (QUEUEtrunc ? ADDR_TRUNC : 0));
			if (QUEUEaddrFinish - DIM_EEPROM_BUFF + 1 >= QUEUE_EEPROM_OFFSET) {
				AT24C256::writeByte(QUEUEaddrFinish - DIM_EEPROM_BUFF + 1, ADDR_STOP | (QUEUEtrunc ? ADDR_TRUNC : 0));
			}
		}
		else {
			AT24C256::writeByte(QUEUEaddrStart, ADDR_START | (QUEUEtrunc ? ADDR_TRUNC : 0));
			if (QUEUEaddrFinish - DIM_EEPROM_BUFF + 1 >= QUEUE_EEPROM_OFFSET) {
				AT24C256::writeByte(QUEUEaddrFinish - DIM_EEPROM_BUFF + 1, AT24C256::readByte(QUEUEaddrFinish - DIM_EEPROM_BUFF + 1) | ADDR_STOP);
			}
		}
	}
	isEEPROMcom = false;
	logger(LOGACT_eeprom_buff_app);
}

bool ROSY_EEPROM::LET_getBufferFromQueue(uint8_t n, uint8_t* tmpbuff) {
	if (LET_getQueueLength() > 0 && n + 1 <= LET_getQueueLength()) {
		if (isEEPROMcom) {
			Serial.println("LETg cattivo");
		}
		isEEPROMcom = true;
		if (QUEUEaddrStart + n * DIM_EEPROM_BUFF >= QUEUE_EEPROM_OFFSET + DIM_EEPROM_BUFF * QUEUE_EEPROM_TOT_DATA) {
			n = ((QUEUEaddrStart + n * DIM_EEPROM_BUFF) - (QUEUE_EEPROM_OFFSET + DIM_EEPROM_BUFF * QUEUE_EEPROM_TOT_DATA)) / DIM_EEPROM_BUFF;
			for (int i = 0; i < DIM_EEPROM_BUFF - 1; i++) {
				tmpbuff[i] = AT24C256::readByte(QUEUE_EEPROM_OFFSET + n * DIM_EEPROM_BUFF + i);
			}
		}
		else {
			for (int i = 0; i < DIM_EEPROM_BUFF - 1; i++) {
				tmpbuff[i] = AT24C256::readByte(QUEUEaddrStart + n * DIM_EEPROM_BUFF + i + 1);
			}
		}
		isEEPROMcom = false;
		logger(LOGACT_eeprom_buff_get);
		return true;
	}
	logger(LOGACT_eeprom_buff_get);
	return false;
}

void ROSY_EEPROM::LET_delBufferFromQueue() {
	if (LET_getQueueLength() > 0) {
		if (isEEPROMcom) {
			Serial.println("LETd cattivo");
		}
		isEEPROMcom = true;
		AT24C256::writeByte(QUEUEaddrStart, ADDR_NULL);
		QUEUEaddrStart += DIM_EEPROM_BUFF;
		if (QUEUEaddrStart >= QUEUE_EEPROM_OFFSET + DIM_EEPROM_BUFF * QUEUE_EEPROM_TOT_DATA) {
			if (QUEUEtrunc == 0) {
				QUEUEaddrStart = QUEUE_EEPROM_OFFSET;
				QUEUEaddrFinish = QUEUE_EEPROM_OFFSET;
				QUEUEtrunc = 0;
			}
			else {
				QUEUEaddrStart = QUEUE_EEPROM_OFFSET;
				QUEUEtrunc = 0;
			}
		}
		if (QUEUEaddrStart != QUEUEaddrFinish + 1 && QUEUEaddrStart != QUEUEaddrFinish) {
			if (QUEUEaddrStart != (QUEUEaddrFinish - DIM_EEPROM_BUFF + 1)) {
				AT24C256::writeByte(QUEUEaddrStart, ADDR_START | (QUEUEtrunc ? ADDR_TRUNC : 0));
				if (QUEUEaddrFinish - DIM_EEPROM_BUFF + 1 >= QUEUE_EEPROM_OFFSET) {
					AT24C256::writeByte(QUEUEaddrFinish - DIM_EEPROM_BUFF + 1, ADDR_STOP | (QUEUEtrunc ? ADDR_TRUNC : 0));
				}
			}
			else {
				AT24C256::writeByte(QUEUEaddrStart, ADDR_START | (QUEUEtrunc ? ADDR_TRUNC : 0));
				if (QUEUEaddrFinish - DIM_EEPROM_BUFF + 1 >= QUEUE_EEPROM_OFFSET) {
					AT24C256::writeByte(QUEUEaddrFinish - DIM_EEPROM_BUFF + 1, AT24C256::readByte(QUEUEaddrFinish - DIM_EEPROM_BUFF + 1) | ADDR_STOP);
				}
			}
		}
		isEEPROMcom = false;
		logger(LOGACT_eeprom_buff_del);
	}
}

uint16_t ROSY_EEPROM::LET_getQueueLength() {
	Serial.println("Trunc: " + String(QUEUEtrunc));
	Serial.println("Start " + String(QUEUEaddrStart));
	Serial.println("Finish: " + String(QUEUEaddrFinish));
	if (QUEUEtrunc) {
		return ((DIM_EEPROM_BUFF * QUEUE_EEPROM_TOT_DATA - (QUEUEaddrStart - QUEUE_EEPROM_OFFSET)) + (QUEUEaddrFinish + 1 - QUEUE_EEPROM_OFFSET)) / DIM_EEPROM_BUFF;
	}
	else {
		return (QUEUEaddrFinish + 1 - QUEUEaddrStart) / DIM_EEPROM_BUFF;
	}
}


void ROSY_EEPROM::BUFF_setTime(uint32_t timestamp) {
	uint8_t* pt = (uint8_t*)&timestamp;
	buffer[OFFSET_PKT_TIME - OFFSET_PKT_NEW] = pt[0];

	buffer[OFFSET_PKT_TIME - OFFSET_PKT_NEW + 1] = pt[1];

	buffer[OFFSET_PKT_TIME - OFFSET_PKT_NEW + 2] = pt[2];

	buffer[OFFSET_PKT_TIME - OFFSET_PKT_NEW + 3] = pt[3];
}


void ROSY_EEPROM::BUFF_setTemperature_M(uint16_t _est, uint16_t _int) {
	uint8_t* temp = (uint8_t*)&_est;
	buffer[OFFSET_PKT_TEMPERATURE - OFFSET_PKT_NEW] = temp[0];
	buffer[OFFSET_PKT_TEMPERATURE - OFFSET_PKT_NEW + 1] = temp[1];

	temp = (uint8_t*)&_int;
	buffer[OFFSET_PKT_TEMPERATURE - OFFSET_PKT_NEW + 2] = temp[0];
	buffer[OFFSET_PKT_TEMPERATURE - OFFSET_PKT_NEW + 3] = temp[1];
}

void ROSY_EEPROM::BUFF_setTemperature_SD(uint16_t _est, uint16_t _int) {
	uint8_t* temp = (uint8_t*)&_est;
	buffer[OFFSET_PKT_TEMPERATURE - OFFSET_PKT_NEW + 4] = temp[0];
	buffer[OFFSET_PKT_TEMPERATURE - OFFSET_PKT_NEW + 5] = temp[1];

	temp = (uint8_t*)&_int;
	buffer[OFFSET_PKT_TEMPERATURE - OFFSET_PKT_NEW + 6] = temp[0];
	buffer[OFFSET_PKT_TEMPERATURE - OFFSET_PKT_NEW + 7] = temp[1];
}


void ROSY_EEPROM::BUFF_setHumidity_M(uint16_t _est, uint16_t _int) {
	uint8_t* temp = (uint8_t*)&_est;
	buffer[OFFSET_PKT_HUMIDITY - OFFSET_PKT_NEW] = temp[0];
	buffer[OFFSET_PKT_HUMIDITY - OFFSET_PKT_NEW + 1] = temp[1];

	temp = (uint8_t*)&_int;
	buffer[OFFSET_PKT_HUMIDITY - OFFSET_PKT_NEW + 2] = temp[0];
	buffer[OFFSET_PKT_HUMIDITY - OFFSET_PKT_NEW + 3] = temp[1];
}

void ROSY_EEPROM::BUFF_setHumidity_SD(uint16_t _est, uint16_t _int) {
	uint8_t* temp = (uint8_t*)&_est;
	buffer[OFFSET_PKT_HUMIDITY - OFFSET_PKT_NEW + 4] = temp[0];
	buffer[OFFSET_PKT_HUMIDITY - OFFSET_PKT_NEW + 5] = temp[1];

	temp = (uint8_t*)&_int;
	buffer[OFFSET_PKT_HUMIDITY - OFFSET_PKT_NEW + 6] = temp[0];
	buffer[OFFSET_PKT_HUMIDITY - OFFSET_PKT_NEW + 7] = temp[1];
}

void ROSY_EEPROM::BUFF_setPressure_M(uint16_t P) {
	uint8_t* temp = (uint8_t*)&P;
	buffer[OFFSET_PKT_PRESSURE - OFFSET_PKT_NEW] = temp[0];
	buffer[OFFSET_PKT_PRESSURE - OFFSET_PKT_NEW + 1] = temp[1];
}

void ROSY_EEPROM::BUFF_setPressure_SD(uint16_t P) {
	uint8_t* temp = (uint8_t*)&P;
	buffer[OFFSET_PKT_PRESSURE - OFFSET_PKT_NEW + 2] = temp[0];
	buffer[OFFSET_PKT_PRESSURE - OFFSET_PKT_NEW + 3] = temp[1];
}


void ROSY_EEPROM::BUFF_setPM_M(uint16_t pm1, uint16_t pm25, uint16_t pm10) {
	uint8_t* temp = (uint8_t*)&pm1;
	buffer[OFFSET_PKT_PM - OFFSET_PKT_NEW] = temp[0];
	buffer[OFFSET_PKT_PM - OFFSET_PKT_NEW + 1] = temp[1];

	temp = (uint8_t*)&pm25;
	buffer[OFFSET_PKT_PM - OFFSET_PKT_NEW + 2] = temp[0];
	buffer[OFFSET_PKT_PM - OFFSET_PKT_NEW + 3] = temp[1];

	temp = (uint8_t*)&pm10;
	buffer[OFFSET_PKT_PM - OFFSET_PKT_NEW + 4] = temp[0];
	buffer[OFFSET_PKT_PM - OFFSET_PKT_NEW + 5] = temp[1];
}

void ROSY_EEPROM::BUFF_setPM_SD(uint16_t pm1, uint16_t pm25, uint16_t pm10) {
	uint8_t* temp = (uint8_t*)&pm1;
	buffer[OFFSET_PKT_PM - OFFSET_PKT_NEW + 6] = temp[0];
	buffer[OFFSET_PKT_PM - OFFSET_PKT_NEW + 7] = temp[1];

	temp = (uint8_t*)&pm25;
	buffer[OFFSET_PKT_PM - OFFSET_PKT_NEW + 8] = temp[0];
	buffer[OFFSET_PKT_PM - OFFSET_PKT_NEW + 9] = temp[1];

	temp = (uint8_t*)&pm10;
	buffer[OFFSET_PKT_PM - OFFSET_PKT_NEW + 10] = temp[0];
	buffer[OFFSET_PKT_PM - OFFSET_PKT_NEW + 11] = temp[1];
}


void ROSY_EEPROM::BUFF_setOzone_M(uint16_t S1_1, uint16_t S1_2, uint16_t S2_1, uint16_t S2_2, uint16_t S3_1, uint16_t S3_2, uint16_t S4_1, uint16_t S4_2) {
	uint8_t* temp = (uint8_t*)&S1_1;
	buffer[OFFSET_PKT_OZONE - OFFSET_PKT_NEW] = temp[0];
	buffer[OFFSET_PKT_OZONE - OFFSET_PKT_NEW + 1] = temp[1];

	temp = (uint8_t*)&S1_2;
	buffer[OFFSET_PKT_OZONE - OFFSET_PKT_NEW + 2] = temp[0];
	buffer[OFFSET_PKT_OZONE - OFFSET_PKT_NEW + 3] = temp[1];

	temp = (uint8_t*)&S2_1;
	buffer[OFFSET_PKT_OZONE - OFFSET_PKT_NEW + 4] = temp[0];
	buffer[OFFSET_PKT_OZONE - OFFSET_PKT_NEW + 5] = temp[1];

	temp = (uint8_t*)&S2_2;
	buffer[OFFSET_PKT_OZONE - OFFSET_PKT_NEW + 6] = temp[0];
	buffer[OFFSET_PKT_OZONE - OFFSET_PKT_NEW + 7] = temp[1];

	temp = (uint8_t*)&S3_1;
	buffer[OFFSET_PKT_OZONE - OFFSET_PKT_NEW + 8] = temp[0];
	buffer[OFFSET_PKT_OZONE - OFFSET_PKT_NEW + 9] = temp[1];

	temp = (uint8_t*)&S3_2;
	buffer[OFFSET_PKT_OZONE - OFFSET_PKT_NEW + 10] = temp[0];
	buffer[OFFSET_PKT_OZONE - OFFSET_PKT_NEW + 11] = temp[1];

	temp = (uint8_t*)&S4_1;
	buffer[OFFSET_PKT_OZONE - OFFSET_PKT_NEW + 12] = temp[0];
	buffer[OFFSET_PKT_OZONE - OFFSET_PKT_NEW + 13] = temp[1];

	temp = (uint8_t*)&S4_2;
	buffer[OFFSET_PKT_OZONE - OFFSET_PKT_NEW + 14] = temp[0];
	buffer[OFFSET_PKT_OZONE - OFFSET_PKT_NEW + 15] = temp[1];
}

void ROSY_EEPROM::BUFF_setOzone_SD(uint16_t S1_1, uint16_t S1_2, uint16_t S2_1, uint16_t S2_2, uint16_t S3_1, uint16_t S3_2, uint16_t S4_1, uint16_t S4_2) {
	uint8_t* temp = (uint8_t*)&S1_1;
	buffer[OFFSET_PKT_OZONE - OFFSET_PKT_NEW + 16] = temp[0];
	buffer[OFFSET_PKT_OZONE - OFFSET_PKT_NEW + 17] = temp[1];

	temp = (uint8_t*)&S1_2;
	buffer[OFFSET_PKT_OZONE - OFFSET_PKT_NEW + 18] = temp[0];
	buffer[OFFSET_PKT_OZONE - OFFSET_PKT_NEW + 19] = temp[1];

	temp = (uint8_t*)&S2_1;
	buffer[OFFSET_PKT_OZONE - OFFSET_PKT_NEW + 20] = temp[0];
	buffer[OFFSET_PKT_OZONE - OFFSET_PKT_NEW + 21] = temp[1];

	temp = (uint8_t*)&S2_2;
	buffer[OFFSET_PKT_OZONE - OFFSET_PKT_NEW + 22] = temp[0];
	buffer[OFFSET_PKT_OZONE - OFFSET_PKT_NEW + 23] = temp[1];

	temp = (uint8_t*)&S3_1;
	buffer[OFFSET_PKT_OZONE - OFFSET_PKT_NEW + 24] = temp[0];
	buffer[OFFSET_PKT_OZONE - OFFSET_PKT_NEW + 25] = temp[1];

	temp = (uint8_t*)&S3_2;
	buffer[OFFSET_PKT_OZONE - OFFSET_PKT_NEW + 26] = temp[0];
	buffer[OFFSET_PKT_OZONE - OFFSET_PKT_NEW + 27] = temp[1];

	temp = (uint8_t*)&S4_1;
	buffer[OFFSET_PKT_OZONE - OFFSET_PKT_NEW + 28] = temp[0];
	buffer[OFFSET_PKT_OZONE - OFFSET_PKT_NEW + 29] = temp[1];

	temp = (uint8_t*)&S4_2;
	buffer[OFFSET_PKT_OZONE - OFFSET_PKT_NEW + 30] = temp[0];
	buffer[OFFSET_PKT_OZONE - OFFSET_PKT_NEW + 31] = temp[1];
}

void ROSY_EEPROM::BUFF_setAccel_M(uint16_t x, uint16_t y, uint16_t z) {
	uint8_t* temp = (uint8_t*)&x;
	buffer[OFFSET_PKT_ACCEL - OFFSET_PKT_NEW] = temp[0];
	buffer[OFFSET_PKT_ACCEL - OFFSET_PKT_NEW + 1] = temp[1];

	temp = (uint8_t*)&y;
	buffer[OFFSET_PKT_ACCEL - OFFSET_PKT_NEW + 2] = temp[0];
	buffer[OFFSET_PKT_ACCEL - OFFSET_PKT_NEW + 3] = temp[1];

	temp = (uint8_t*)&z;
	buffer[OFFSET_PKT_ACCEL - OFFSET_PKT_NEW + 4] = temp[0];
	buffer[OFFSET_PKT_ACCEL - OFFSET_PKT_NEW + 5] = temp[1];
}

void ROSY_EEPROM::BUFF_setAccel_SD(uint16_t x, uint16_t y, uint16_t z) {
	uint8_t* temp = (uint8_t*)&x;
	buffer[OFFSET_PKT_ACCEL - OFFSET_PKT_NEW + 6] = temp[0];
	buffer[OFFSET_PKT_ACCEL - OFFSET_PKT_NEW + 7] = temp[1];

	temp = (uint8_t*)&y;
	buffer[OFFSET_PKT_ACCEL - OFFSET_PKT_NEW + 8] = temp[0];
	buffer[OFFSET_PKT_ACCEL - OFFSET_PKT_NEW + 9] = temp[1];

	temp = (uint8_t*)&z;
	buffer[OFFSET_PKT_ACCEL - OFFSET_PKT_NEW + 10] = temp[0];
	buffer[OFFSET_PKT_ACCEL - OFFSET_PKT_NEW + 11] = temp[1];
}


void ROSY_EEPROM::BUFF_setMag_M(uint16_t x, uint16_t y, uint16_t z) {
	uint8_t* temp = (uint8_t*)&x;
	buffer[OFFSET_PKT_MAG - OFFSET_PKT_NEW] = temp[0];
	buffer[OFFSET_PKT_MAG - OFFSET_PKT_NEW + 1] = temp[1];

	temp = (uint8_t*)&y;
	buffer[OFFSET_PKT_MAG - OFFSET_PKT_NEW + 2] = temp[0];
	buffer[OFFSET_PKT_MAG - OFFSET_PKT_NEW + 3] = temp[1];

	temp = (uint8_t*)&z;
	buffer[OFFSET_PKT_MAG - OFFSET_PKT_NEW + 4] = temp[0];
	buffer[OFFSET_PKT_MAG - OFFSET_PKT_NEW + 5] = temp[1];
}

void ROSY_EEPROM::BUFF_setMag_SD(uint16_t x, uint16_t y, uint16_t z) {
	uint8_t* temp = (uint8_t*)&x;
	buffer[OFFSET_PKT_MAG - OFFSET_PKT_NEW + 6] = temp[0];
	buffer[OFFSET_PKT_MAG - OFFSET_PKT_NEW + 7] = temp[1];

	temp = (uint8_t*)&y;
	buffer[OFFSET_PKT_MAG - OFFSET_PKT_NEW + 8] = temp[0];
	buffer[OFFSET_PKT_MAG - OFFSET_PKT_NEW + 9] = temp[1];

	temp = (uint8_t*)&z;
	buffer[OFFSET_PKT_MAG - OFFSET_PKT_NEW + 10] = temp[0];
	buffer[OFFSET_PKT_MAG - OFFSET_PKT_NEW + 11] = temp[1];
}


void ROSY_EEPROM::BUFF_setGyro_M(uint16_t x, uint16_t y, uint16_t z) {
	uint8_t* temp = (uint8_t*)&x;
	buffer[OFFSET_PKT_GYRO - OFFSET_PKT_NEW] = temp[0];
	buffer[OFFSET_PKT_GYRO - OFFSET_PKT_NEW + 1] = temp[1];

	temp = (uint8_t*)&y;
	buffer[OFFSET_PKT_GYRO - OFFSET_PKT_NEW + 2] = temp[0];
	buffer[OFFSET_PKT_GYRO - OFFSET_PKT_NEW + 3] = temp[1];

	temp = (uint8_t*)&z;
	buffer[OFFSET_PKT_GYRO - OFFSET_PKT_NEW + 4] = temp[0];
	buffer[OFFSET_PKT_GYRO - OFFSET_PKT_NEW + 5] = temp[1];
}

void ROSY_EEPROM::BUFF_setGyro_SD(uint16_t x, uint16_t y, uint16_t z) {
	uint8_t* temp = (uint8_t*)&x;
	buffer[OFFSET_PKT_GYRO - OFFSET_PKT_NEW + 6] = temp[0];
	buffer[OFFSET_PKT_GYRO - OFFSET_PKT_NEW + 7] = temp[1];

	temp = (uint8_t*)&y;
	buffer[OFFSET_PKT_GYRO - OFFSET_PKT_NEW + 8] = temp[0];
	buffer[OFFSET_PKT_GYRO - OFFSET_PKT_NEW + 9] = temp[1];

	temp = (uint8_t*)&z;
	buffer[OFFSET_PKT_GYRO - OFFSET_PKT_NEW + 10] = temp[0];
	buffer[OFFSET_PKT_GYRO - OFFSET_PKT_NEW + 11] = temp[1];
}

void ROSY_EEPROM::BUFF_setWindRain_M(uint16_t speed, uint16_t direction, uint16_t rain) {
	uint8_t* temp = (uint8_t*)&speed;
	buffer[OFFSET_PKT_WIND - OFFSET_PKT_NEW] = temp[0];
	buffer[OFFSET_PKT_WIND - OFFSET_PKT_NEW + 1] = temp[1];

	temp = (uint8_t*)&direction;
	buffer[OFFSET_PKT_WIND - OFFSET_PKT_NEW + 2] = temp[0];
	buffer[OFFSET_PKT_WIND - OFFSET_PKT_NEW + 3] = temp[1];

	temp = (uint8_t*)&rain;
	buffer[OFFSET_PKT_WIND - OFFSET_PKT_NEW + 4] = temp[0];
	buffer[OFFSET_PKT_WIND - OFFSET_PKT_NEW + 5] = temp[1];
}

void ROSY_EEPROM::BUFF_setWindRain_SD(uint16_t speed, uint16_t direction,uint16_t rain) {
	uint8_t* temp = (uint8_t*)&speed;
	buffer[OFFSET_PKT_WIND - OFFSET_PKT_NEW + 6] = temp[0];
	buffer[OFFSET_PKT_WIND - OFFSET_PKT_NEW + 7] = temp[1];

	temp = (uint8_t*)&direction;
	buffer[OFFSET_PKT_WIND - OFFSET_PKT_NEW + 8] = temp[0];
	buffer[OFFSET_PKT_WIND - OFFSET_PKT_NEW + 9] = temp[1];

	temp = (uint8_t*)&rain;
	buffer[OFFSET_PKT_WIND - OFFSET_PKT_NEW + 10] = temp[0];
	buffer[OFFSET_PKT_WIND - OFFSET_PKT_NEW + 11] = temp[1];
}


void ROSY_EEPROM::LOG_init() {
	int16_t startADDR = 0;
	int16_t stopADDR = 0;
	uint8_t truncADDR = 0;
	for (int i = 0; i < LOG_EEPROM_TOT_DATA; i++) {
		if ((AT24C256::readByte(LOG_EEPROM_OFFSET + DIM_EEPROM_LOG * i) & ADDR_STOP) == ADDR_STOP) {
			if (stopADDR != 0) {
				stopADDR = 0;
				break;
			}
			else {
				stopADDR = LOG_EEPROM_OFFSET + DIM_EEPROM_LOG * (i + 1) - 1;
				if ((AT24C256::readByte(LOG_EEPROM_OFFSET + DIM_EEPROM_LOG * i) & ADDR_TRUNC) == ADDR_TRUNC) {
					if (truncADDR == 3) {
						truncADDR = 4;
					}
					else
					{
						if (truncADDR != 1 && truncADDR != 2) {
							truncADDR = 3;
						}
						else {
							truncADDR = 0;
							break;
						}
					}
				}
				else {
					if (truncADDR == 1) {
						truncADDR = 2;
					}
					else
					{
						if (truncADDR != 3 && truncADDR != 4) {
							truncADDR = 1;
						}
						else {
							truncADDR = 0;
							break;
						}
					}
				}
			}
		}
		if ((AT24C256::readByte(LOG_EEPROM_OFFSET + DIM_EEPROM_LOG * i) & ADDR_START) == ADDR_START) {
			if (startADDR != 0) {
				startADDR = 0;
				break;
			}
			else {
				startADDR = LOG_EEPROM_OFFSET + DIM_EEPROM_LOG * i;
				if ((AT24C256::readByte(LOG_EEPROM_OFFSET + DIM_EEPROM_LOG * i) & ADDR_TRUNC) == ADDR_TRUNC) {
					if (truncADDR == 3) {
						truncADDR = 4;
					}
					else
					{
						if (truncADDR != 1 && truncADDR != 2) {
							truncADDR = 3;
						}
						else {
							truncADDR = 0;
							break;
						}
					}
				}
				else {
					if (truncADDR == 1) {
						truncADDR = 2;
					}
					else
					{
						if (truncADDR != 3 && truncADDR != 4) {
							truncADDR = 1;
						}
						else {
							truncADDR = 0;
							break;
						}
					}
				}
			}
		}
	}
	if ((truncADDR == 2 || truncADDR == 4) && startADDR != 0 && stopADDR != 0) {
		LOGaddrStart = startADDR;
		LOGaddrFinish = stopADDR;
		if (truncADDR == 4) {
			LOGtrunc = 1;
		}
		else {
			LOGtrunc = 0;
		}
	}
	else {
		LOGaddrStart = LOG_EEPROM_OFFSET;
		LOGaddrFinish = LOG_EEPROM_OFFSET;
		LOGtrunc = 0;
		for (int i = 0; i < LOG_EEPROM_TOT_DATA; i++) {
			AT24C256::writeByte(LOG_EEPROM_OFFSET + DIM_EEPROM_LOG * i, 0);
		}
	}
}

void ROSY_EEPROM::LOG_addActivity(uint8_t activity, uint32_t timestamp) {
	cli();
	if (isEEPROMcom) {
		PKT_LOG** pt = &pktLOG;
		if (pktLOG != NULL) {
			while (((*pt)->next) != NULL) {
				pt = &((*pt)->next);
			}
			pt = &((*pt)->next);
		}
		*pt = new PKT_LOG;
		(*pt)->act = activity;
		(*pt)->ts = timestamp;
		sei();
	}
	else {
		isEEPROMcom = true;
		sei();
		if (LOGaddrFinish - DIM_EEPROM_LOG + 1 >= LOG_EEPROM_OFFSET) {
			AT24C256::writeByte(LOGaddrFinish - DIM_EEPROM_LOG + 1, ADDR_NULL);
		}
		if (LOGaddrFinish >= LOG_EEPROM_OFFSET + DIM_EEPROM_LOG * LOG_EEPROM_TOT_DATA - 1) {
			LOGaddrFinish = LOG_EEPROM_OFFSET;
			LOGtrunc = 1;
		}
		uint8_t off = (LOGaddrFinish == LOG_EEPROM_OFFSET ? 0 : 1);

		AT24C256::writeArray(LOGaddrFinish + 1 + off, (uint8_t*)&timestamp, 4);

		AT24C256::writeByte(LOGaddrFinish + 5 + off, activity);

		LOGaddrFinish += DIM_EEPROM_LOG - 1 + off;
		if (LOGaddrFinish >= LOGaddrStart && LOGtrunc) {
			AT24C256::writeByte(LOGaddrStart, ADDR_NULL);
			LOGaddrStart += DIM_EEPROM_LOG;
			if (LOGaddrStart >= LOG_EEPROM_OFFSET + DIM_EEPROM_LOG * LOG_EEPROM_TOT_DATA) {
				if (LOGtrunc == 0) {
					LOGaddrStart = LOG_EEPROM_OFFSET;
					LOGaddrFinish = LOG_EEPROM_OFFSET;
					LOGtrunc = 0;
				}
				else {
					LOGaddrStart = LOG_EEPROM_OFFSET;
					LOGtrunc = 0;
				}
			}
		}
		if (LOGaddrStart != LOGaddrFinish + 1 && LOGaddrStart != LOGaddrFinish) {
			if (LOGaddrStart != (LOGaddrFinish - DIM_EEPROM_LOG + 1)) {
				AT24C256::writeByte(LOGaddrStart, ADDR_START | (LOGtrunc ? ADDR_TRUNC : 0));
				if (LOGaddrFinish - DIM_EEPROM_LOG + 1 >= LOG_EEPROM_OFFSET) {
					AT24C256::writeByte(LOGaddrFinish - DIM_EEPROM_LOG + 1, ADDR_STOP | (LOGtrunc ? ADDR_TRUNC : 0));
				}
			}
			else {
				AT24C256::writeByte(LOGaddrStart, ADDR_START | (LOGtrunc ? ADDR_TRUNC : 0));
				if (LOGaddrFinish - DIM_EEPROM_LOG + 1 >= LOG_EEPROM_OFFSET) {
					AT24C256::writeByte(LOGaddrFinish - DIM_EEPROM_LOG + 1, AT24C256::readByte(LOGaddrFinish - DIM_EEPROM_LOG + 1) | ADDR_STOP);
				}
			}
		}
		cli();
		isEEPROMcom = false;
		if (pktLOG != NULL) {
			uint8_t Lact = pktLOG->act;
			uint32_t Lts = pktLOG->ts;
			PKT_LOG* pktDEL = pktLOG;
			pktLOG = pktLOG->next;
			delete pktDEL;
			sei();
			LOG_addActivity(Lact, Lts);
		}
		else {
			sei();
		}
	}
}

uint16_t ROSY_EEPROM::LOG_getQueueLength() {
	if (LOGtrunc) {
		return ((DIM_EEPROM_LOG * LOG_EEPROM_TOT_DATA - (LOGaddrStart - LOG_EEPROM_OFFSET)) + (LOGaddrFinish + 1 - LOG_EEPROM_OFFSET)) / DIM_EEPROM_LOG;
	}
	else {
		return (LOGaddrFinish + 1 - LOGaddrStart) / DIM_EEPROM_LOG;
	}
}

bool ROSY_EEPROM::LOG_getFromQueue(uint8_t n, uint8_t* tmpbuff) {
	if (LOG_getQueueLength() > 0 && n + 1 <= LOG_getQueueLength()) {
		if (isEEPROMcom) {
			Serial.println("LOGg cattivo");
		}
		isEEPROMcom = true;
		if (LOGaddrStart + n * DIM_EEPROM_LOG >= LOG_EEPROM_OFFSET + DIM_EEPROM_LOG * LOG_EEPROM_TOT_DATA) {
			n = ((LOGaddrStart + n * DIM_EEPROM_LOG) - (LOG_EEPROM_OFFSET + DIM_EEPROM_LOG * LOG_EEPROM_TOT_DATA)) / DIM_EEPROM_LOG;
			for (int i = 0; i < DIM_EEPROM_LOG - 1; i++) {
				tmpbuff[i] = AT24C256::readByte(LOG_EEPROM_OFFSET + n * DIM_EEPROM_LOG + i);
			}
		}
		else {
			for (int i = 0; i < DIM_EEPROM_LOG - 1; i++) {
				tmpbuff[i] = AT24C256::readByte(LOGaddrStart + n * DIM_EEPROM_LOG + i + 1);
			}
		}
		isEEPROMcom = false;
		logger(LOGACT_eeprom_log_get);
		return true;
	}
	logger(LOGACT_eeprom_log_get);
	return false;
}



void ROSY_EEPROM::INF_init() {
	int16_t startADDR = 0;
	int16_t stopADDR = 0;
	uint8_t truncADDR = 0;
	for (int i = 0; i < INF_EEPROM_TOT_DATA; i++) {
		if ((AT24C256::readByte(INF_EEPROM_OFFSET + DIM_EEPROM_INF * i) & ADDR_STOP) == ADDR_STOP) {
			if (stopADDR != 0) {
				stopADDR = 0;
				break;
			}
			else {
				stopADDR = INF_EEPROM_OFFSET + DIM_EEPROM_INF * (i + 1) - 1;
				if ((AT24C256::readByte(INF_EEPROM_OFFSET + DIM_EEPROM_INF * i) & ADDR_TRUNC) == ADDR_TRUNC) {
					if (truncADDR == 3) {
						truncADDR = 4;
					}
					else
					{
						if (truncADDR != 1 && truncADDR != 2) {
							truncADDR = 3;
						}
						else {
							truncADDR = 0;
							break;
						}
					}
				}
				else {
					if (truncADDR == 1) {
						truncADDR = 2;
					}
					else
					{
						if (truncADDR != 3 && truncADDR != 4) {
							truncADDR = 1;
						}
						else {
							truncADDR = 0;
							break;
						}
					}
				}
			}
		}
		if ((AT24C256::readByte(INF_EEPROM_OFFSET + DIM_EEPROM_INF * i) & ADDR_START) == ADDR_START) {
			if (startADDR != 0) {
				startADDR = 0;
				break;
			}
			else {
				startADDR = INF_EEPROM_OFFSET + DIM_EEPROM_INF * i;
				if ((AT24C256::readByte(INF_EEPROM_OFFSET + DIM_EEPROM_INF * i) & ADDR_TRUNC) == ADDR_TRUNC) {
					if (truncADDR == 3) {
						truncADDR = 4;
					}
					else
					{
						if (truncADDR != 1 && truncADDR != 2) {
							truncADDR = 3;
						}
						else {
							truncADDR = 0;
							break;
						}
					}
				}
				else {
					if (truncADDR == 1) {
						truncADDR = 2;
					}
					else
					{
						if (truncADDR != 3 && truncADDR != 4) {
							truncADDR = 1;
						}
						else {
							truncADDR = 0;
							break;
						}
					}
				}
			}
		}
	}
	if ((truncADDR == 2 || truncADDR == 4) && startADDR != 0 && stopADDR != 0) {
		INFaddrStart = startADDR;
		INFaddrFinish = stopADDR;
		if (truncADDR == 4) {
			INFtrunc = 1;
		}
		else {
			INFtrunc = 0;
		}
	}
	else {
		INFaddrStart = INF_EEPROM_OFFSET;
		INFaddrFinish = INF_EEPROM_OFFSET;
		INFtrunc = 0;
		for (int i = 0; i < INF_EEPROM_TOT_DATA; i++) {
			AT24C256::writeByte(INF_EEPROM_OFFSET + DIM_EEPROM_INF * i, 0);
		}
	}
}

void ROSY_EEPROM::INF_add(uint16_t BV, uint16_t BA, uint16_t SN, uint16_t RSSI, uint32_t timestamp) {
	if (isEEPROMcom) {
		Serial.println("INFa cattivo");
	}
	isEEPROMcom = true;
	if (INFaddrFinish - DIM_EEPROM_INF + 1 >= INF_EEPROM_OFFSET) {
		AT24C256::writeByte(INFaddrFinish - DIM_EEPROM_INF + 1, ADDR_NULL);
	}
	if (INFaddrFinish >= INF_EEPROM_OFFSET + DIM_EEPROM_INF * INF_EEPROM_TOT_DATA - 1) {
		INFaddrFinish = INF_EEPROM_OFFSET;
		INFtrunc = 1;
	}
	uint8_t off = (INFaddrFinish == INF_EEPROM_OFFSET ? 0 : 1);
	AT24C256::writeArray(INFaddrFinish + 1 + off, (uint8_t*)&timestamp, 4);
	AT24C256::writeArray(INFaddrFinish + 5 + off, (uint8_t*)&BV, 2);
	AT24C256::writeArray(INFaddrFinish + 7 + off, (uint8_t*)&BA, 2);
	AT24C256::writeArray(INFaddrFinish + 9 + off, (uint8_t*)&SN, 2);
	AT24C256::writeArray(INFaddrFinish + 11 + off, (uint8_t*)&RSSI, 2);

	INFaddrFinish += DIM_EEPROM_INF - 1 + off;

	if (INFaddrFinish >= INFaddrStart && INFtrunc) {
		AT24C256::writeByte(INFaddrStart, ADDR_NULL);
		INFaddrStart += DIM_EEPROM_INF;
		if (INFaddrStart >= INF_EEPROM_OFFSET + DIM_EEPROM_INF * INF_EEPROM_TOT_DATA) {
			if (INFtrunc == 0) {
				INFaddrStart = INF_EEPROM_OFFSET;
				INFaddrFinish = INF_EEPROM_OFFSET;
				INFtrunc = 0;
			}
			else {
				INFaddrStart = INF_EEPROM_OFFSET;
				INFtrunc = 0;
			}
		}
	}
	if (INFaddrStart != INFaddrFinish + 1 && INFaddrStart != INFaddrFinish) {
		if (INFaddrStart != (INFaddrFinish - DIM_EEPROM_INF + 1)) {
			AT24C256::writeByte(INFaddrStart, ADDR_START | (INFtrunc ? ADDR_TRUNC : 0));
			if (INFaddrFinish - DIM_EEPROM_INF + 1 >= INF_EEPROM_OFFSET) {
				AT24C256::writeByte(INFaddrFinish - DIM_EEPROM_INF + 1, ADDR_STOP | (INFtrunc ? ADDR_TRUNC : 0));
			}
		}
		else {
			AT24C256::writeByte(INFaddrStart, ADDR_START | (INFtrunc ? ADDR_TRUNC : 0));
			if (INFaddrFinish - DIM_EEPROM_INF + 1 >= INF_EEPROM_OFFSET) {
				AT24C256::writeByte(INFaddrFinish - DIM_EEPROM_INF + 1, AT24C256::readByte(INFaddrFinish - DIM_EEPROM_INF + 1) | ADDR_STOP);
			}
		}
	}
	isEEPROMcom = false;
	logger(LOGACT_eeprom_inf_app);
}

uint16_t ROSY_EEPROM::INF_getQueueLength() {
	if (INFtrunc) {
		return ((DIM_EEPROM_INF * INF_EEPROM_TOT_DATA - (INFaddrStart - INF_EEPROM_OFFSET)) + (INFaddrFinish + 1 - INF_EEPROM_OFFSET)) / DIM_EEPROM_INF;
	}
	else {
		return (INFaddrFinish + 1 - INFaddrStart) / DIM_EEPROM_INF;
	}
}

bool ROSY_EEPROM::INF_getFromQueue(uint8_t n, uint8_t* tmpbuff) {
	if (INF_getQueueLength() > 0 && n + 1 <= INF_getQueueLength()) {
		if (isEEPROMcom) {
			Serial.println("INFg cattivo");
		}
		isEEPROMcom = true;
		if (INFaddrStart + n * DIM_EEPROM_INF >= INF_EEPROM_OFFSET + DIM_EEPROM_INF * INF_EEPROM_TOT_DATA) {
			n = ((INFaddrStart + n * DIM_EEPROM_INF) - (INF_EEPROM_OFFSET + DIM_EEPROM_INF * INF_EEPROM_TOT_DATA)) / DIM_EEPROM_INF;
			for (int i = 0; i < DIM_EEPROM_INF - 1; i++) {
				tmpbuff[i] = AT24C256::readByte(INF_EEPROM_OFFSET + n * DIM_EEPROM_INF + i);
			}
		}
		else {
			for (int i = 0; i < DIM_EEPROM_INF - 1; i++) {
				tmpbuff[i] = AT24C256::readByte(INFaddrStart + n * DIM_EEPROM_INF + i + 1);
			}
		}
		isEEPROMcom = false;
		logger(LOGACT_eeprom_inf_get);
		return true;
	}
	logger(LOGACT_eeprom_inf_get);
	return false;
}

void ROSY_EEPROM::def() {

}

ROSY_EEPROM::PKT_LOG* ROSY_EEPROM::pktLOG;
bool ROSY_EEPROM::isEEPROMcom;

uint8_t ROSY_EEPROM::buffer[DIM_EEPROM_BUFF - 1] = { 0 };

int16_t ROSY_EEPROM::QUEUEaddrStart = QUEUE_EEPROM_OFFSET;
int16_t ROSY_EEPROM::QUEUEaddrFinish = QUEUE_EEPROM_OFFSET;
uint8_t ROSY_EEPROM::QUEUEtrunc = 0;

int16_t ROSY_EEPROM::LOGaddrStart = LOG_EEPROM_OFFSET;
int16_t ROSY_EEPROM::LOGaddrFinish = LOG_EEPROM_OFFSET;
uint8_t ROSY_EEPROM::LOGtrunc = 0;

int16_t ROSY_EEPROM::INFaddrStart = INF_EEPROM_OFFSET;
int16_t ROSY_EEPROM::INFaddrFinish = INF_EEPROM_OFFSET;
uint8_t ROSY_EEPROM::INFtrunc = 0;

void(*ROSY_EEPROM::logger)(uint8_t) = (void(*)(uint8_t)) ROSY_EEPROM::def;

#if EEPROM_RAM
uint8_t  ROSY_EEPROM::queue[__EEPROM_LENGTH__] = { 0 };
#endif