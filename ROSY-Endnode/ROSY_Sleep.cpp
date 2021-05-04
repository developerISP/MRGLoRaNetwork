#include "ROSY_Sleep.h"

void ROSY_Sleep::def() {

}

void ROSY_Sleep::setUpdater(void(*PwEn)(void), void(*PwDis)(void), void(*SeEn)(bool, bool, bool), bool(*UpSleep)(void), void(*logCB)(uint8_t)) {
	serialEnabler = SeEn;
	UpdateInSleep = UpSleep;
	SensorPowerEnable = PwEn;
	SensorPowerDisable = PwDis;
	logger = logCB;
}

uint16_t ROSY_Sleep::rtc2let(uint8_t hour, uint8_t minute, uint8_t second) {
	uint32_t t = (float)second + ((float)minute) * 60 + ((float)hour) * 60 * 60;
	uint8_t n = 1;
	while (t >= SENSOR_TIME + SLEEP_TIME) {
		t -= SENSOR_TIME + SLEEP_TIME;
		n++;
	}
	return n;
}

uint32_t ROSY_Sleep::rtcSYNCsleep(uint16_t let, uint8_t hour, uint8_t minute, uint8_t second) {
	uint32_t t = (float)second + ((float)minute) * 60 + ((float)hour) * 60 * 60;
	uint32_t tl = ((uint32_t)let) * (SENSOR_TIME + SLEEP_TIME);
	if (tl - t > 0) {
		if (tl - t > (SENSOR_TIME + SLEEP_TIME) * SLEEP_CYCLE_INCLUDED) {
			return 0;
		}
		else {
			return tl - t;
		}

	}
	else {
		if ((tl <= (SENSOR_TIME + SLEEP_TIME) * SLEEP_CYCLE_INCLUDED) && (24 * 60 * 60 - t <= (SENSOR_TIME + SLEEP_TIME) * (SLEEP_CYCLE_INCLUDED - let))) {
			return 24 * 60 * 60 - t + (SENSOR_TIME + SLEEP_TIME) * let;

		}
		else {
			return 0;
		}
	}
}

void ROSY_Sleep::SerialOFF(bool flush)
{
#if SLEEP_ENABLE
	if (isSerialON && isSleeping) {
		for(int i=0; i< 10; i++){
			_delay_ms(300);
		}
		logger(LOGACT_serial_off);
		cli();
		if (flush)
		{
			Wire.flush();
#ifdef HAVE_HWSERIAL0
#if HS0enable
			Serial.flush();
#endif
#endif
#ifdef HAVE_HWSERIAL1
#if HS1enable
			Serial1.flush();
#endif
#endif
#ifdef HAVE_HWSERIAL2
#if HS2enable
			Serial2.flush();
#endif
#endif
#ifdef HAVE_HWSERIAL3
#if HS3enable
			Serial3.flush();
#endif
#endif
		}
		else {
			_delay_ms(SERIAL_TIME);
		}

		TWCR &= ~(bit(TWEN) | bit(TWIE) | bit(TWEA));
#ifdef HAVE_HWSERIAL0
#if HS0enable
		Serial.end();
#endif
#endif
#ifdef HAVE_HWSERIAL1
#if HS1enable
		Serial1.end();
#endif
#endif
#ifdef HAVE_HWSERIAL2
#if HS2enable
		Serial2.end();
#endif
#endif
#ifdef HAVE_HWSERIAL3
#if HS3enable
		Serial3.end();
#endif
#endif

		SPI.end();

		sei();

		isSerialON = 0;
	}
#endif
}


void ROSY_Sleep::SerialON()
{
#if SLEEP_ENABLE
	if (!isSerialON && isSleeping) {
		for(int i=0; i< 10; i++){
			_delay_ms(300);
		}
		cli();
		Wire.begin();
		_delay_ms(SERIAL_TIME);
#ifdef HAVE_HWSERIAL0
#if HS0enable
		if (HS0baudRate != 0) {
			Serial.begin(HS0baudRate);
			_delay_ms(SERIAL_TIME);
		}
#endif
#endif
#ifdef HAVE_HWSERIAL1
#if HS1enable
		if (HS1baudRate != 0) {
			Serial1.begin(HS1baudRate);
			_delay_ms(SERIAL_TIME);
		}
#endif
#endif
#ifdef HAVE_HWSERIAL2
#if HS2enable
		if (HS2baudRate != 0) {
			Serial2.begin(HS2baudRate);
			_delay_ms(SERIAL_TIME);
		}
#endif
#endif
#ifdef HAVE_HWSERIAL3
#if HS3enable
		if (HS3baudRate != 0) {
			Serial3.begin(HS3baudRate);
			_delay_ms(SERIAL_TIME);
		}
#endif
#endif

		SPI.begin();
		_delay_ms(SERIAL_TIME);
		sei();
		logger(LOGACT_serial_on);

		isSerialON = 1;
	}
#endif
}


void ROSY_Sleep::sleepMode(uint16_t t)
{
#if SLEEP_ENABLE
	logger(LOGACT_start_sleep);
	isSleeping = 1;

	logger(LOGACT_sensor_off);
	SensorPowerDisable();

	SerialOFF(true);

	while (true)
	{
		cli();
		wdt_reset();
		if (t >= 8)
		{
			wdt_enable(WDTO_8S);
			t -= 8;
		}
		else if (t >= 4)
		{
			wdt_enable(WDTO_4S);
			t -= 4;
		}
		else if (t >= 2)
		{
			wdt_enable(WDTO_2S);
			t -= 2;
		}
		else
		{
			break;
		}
		WDTCSR |= (1 << 6);

		power_adc_disable();
#ifdef power_vadc_disable
		power_vadc_disable();
#endif
		set_sleep_mode(SLEEP_MODE_PWR_DOWN);
		sleep_enable();

#ifdef sleep_bod_disable
		sleep_bod_disable();
#endif

		sei();

		sleep_mode();
		
		sleep_disable();
		power_adc_enable();
#ifdef power_vadc_enable
		power_vadc_enable();
#endif

		wdt_disable();

		unsigned long startUpdate = millis();

		serialEnabler(true, true, false);
		Serial.println("updatel1 "+String(t));
		UpdateInSleep();
		serialEnabler(false, true, false);

		if (startUpdate > millis()) {
			if (t > (millis() + (pow(2, sizeof(unsigned long) * 8) - startUpdate)) / 1000) {
				t -= (millis() + (pow(2, sizeof(unsigned long) * 8) - startUpdate)) / 1000;
			}
			else {
				t = 0;
			}
		}
		else {
			if (t > (millis() - startUpdate) / 1000) {
				t -= (millis() - startUpdate) / 1000;
			}
			else {
				t = 0;
			}
		}
	}

	SerialON();

	logger(LOGACT_sensor_on);
	SensorPowerEnable();

	isSleeping = 0;

	logger(LOGACT_stop_sleep);
#else
	while (true)
	{
		if (t >= 8)
		{
			delay(8000);
			t-=8;
		}
		else if (t >= 4)
		{
			delay(4000);
			t-=4;
		}
		else if (t >= 2)
		{
			delay(2000);
			t-=2;
		}
		else{
			break;
		}
		Serial.println("update");
		UpdateInSleep();
	}
#endif
}

void(*ROSY_Sleep::SensorPowerDisable)(void) = ROSY_Sleep::def;

void(*ROSY_Sleep::SensorPowerEnable)(void) = ROSY_Sleep::def;

void(*ROSY_Sleep::serialEnabler)(bool, bool, bool) = (void(*)(bool, bool, bool)) ROSY_Sleep::def;

bool(*ROSY_Sleep::UpdateInSleep)(void) = (bool(*)()) ROSY_Sleep::def;

void(*ROSY_Sleep::logger)(uint8_t) = (void(*)(uint8_t)) ROSY_Sleep::def;

int ROSY_Sleep::isSerialON = 0;

int ROSY_Sleep::isSleeping = 3;

#if SLEEP_ENABLE
ISR(WDT_vect)
{
}
#endif