#ifndef _ROSY_LORA_H_
#define _ROSY_LORA_H_

#include <Arduino.h>
#include "LoRa.h"
#include "ROSY_Config.h"

#if !LoRa_gateway

class ROSY_Lora_End {
private:
	static uint8_t packet[TOT_LET_PER_PKT * OFFSET_PKT_END - (TOT_LET_PER_PKT - 1) * OFFSET_PKT_NEW];
	static int arrRSSI[5];
	static float arrSN[5];
	static uint8_t arrI;

	static void(*serialEnabler)(bool, bool, bool);
	static void LoRaReciver(int size);
	static void(*logger)(uint8_t);
public:
	class PKT_RCV {
	public:
		uint8_t pkt_rcv;
		PKT_RCV* next = NULL;
	};
	static PKT_RCV* pktRX;
	static uint32_t received_timestamp;
	static int dropReceived;
	static void def();
	static void LoRaInit(uint8_t id, uint8_t ss, uint8_t rs, uint8_t dio0, void(*serCB)(bool, bool, bool), void(*logCB)(uint8_t));

	static void PKT_setStatus(uint8_t INF, uint16_t SN, uint16_t RSSI, uint16_t BV, uint16_t BA);

	static void PKT_setData(uint8_t n, uint8_t* buff);

	static void PKT_send(uint8_t nLET);

	static void LoRa_getINFO(float* SN, float* RSSI);
};

#else
class ROSY_Lora_Gat {
private:
	static uint8_t TargetAddr;
	static bool isAvailable;
	static bool isACK;
	static unsigned long startRequest;
	static void(*rcvPKT)(void);
	static void LoRaReciver(int size);
public:
	static uint8_t buffer[TOT_LET_PER_PKT * OFFSET_PKT_END - (TOT_LET_PER_PKT - 1) * OFFSET_PKT_NEW];

	static void def();

	static void LoRaInit(uint8_t ss, uint8_t rs, uint8_t dio0, void(*rcvCB)(void));

	static bool failedRQST();

	static bool requestData(uint8_t id, uint32_t datetime);

	static void sendACK(bool isLast);
};

#endif
#endif