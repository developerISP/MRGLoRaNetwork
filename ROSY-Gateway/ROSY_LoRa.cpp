#include "ROSY_LoRa.h"

#if !LoRa_gateway

void ROSY_Lora_End::LoRaReciver(int size) {
	static bool receivedHead = false;
	static uint8_t CUaddr = 0;
	static uint8_t type=0;
	serialEnabler (true, false, true);
	if(dropReceived){
		while (LoRa.available()){
			LoRa.read();
		}
		return;
	}
	arrI++;
	if (arrI > 5) {
		arrI = 0;
	}
	arrRSSI[arrI] = LoRa.packetRssi();
	arrSN[arrI] = LoRa.packetSnr();
	if(!receivedHead){
		if (!(size >= 2)) {
			return;
		}
		while (!LoRa.available());
		CUaddr = LoRa.read();
		Serial.println("Addr "+String(CUaddr));
		while (!LoRa.available());
		type = LoRa.read();
		Serial.println("Type "+String(type));
		receivedHead=true;
	}
	if (CUaddr == packet[0]) {
		if (type == PKT_type_RQST) {
			if (!(size >= 6)) {
				return;
			}
			uint32_t t_stamp=0;
			uint8_t *ptstamp=(uint8_t*)&t_stamp;
			for(int j=0;j<4;j++){
				while (!LoRa.available());
	 			ptstamp[j] = LoRa.read();
			}
			received_timestamp=t_stamp;
			PKT_RCV** pt = &pktRX;
			if (pktRX != NULL) {
				while (((*pt)->next) != NULL) {
					pt = &((*pt)->next);
				}
				pt = &((*pt)->next);
			}
			*pt = new PKT_RCV;
			(*pt)->pkt_rcv = PKT_type_RQST;
		}
		else if (type == PKT_type_ACK) {
			PKT_RCV** pt = &pktRX;
			if (pktRX != NULL) {
				while (((*pt)->next) != NULL) {
					pt = &((*pt)->next);
				}
				pt = &((*pt)->next);
			}
			*pt = new PKT_RCV;
			(*pt)->pkt_rcv = PKT_type_ACK;
		}
	}
	if(type==PKT_type_LET){
		uint8_t LETtype=LoRa.peek();
		uint8_t n = 0;
		if ((LETtype & INF_LET_1) == INF_LET_1) {
			n++;
			if ((LETtype & INF_LET_2) == INF_LET_2) {
				n++;
				if ((LETtype & INF_LET_3) == INF_LET_3) {
					n++;
				}
			}
		}
		if (!(LoRa.available() >= n * OFFSET_PKT_END - (n - 1) * OFFSET_PKT_NEW - 3)) {
			return;
		}
		while (LoRa.available()){
			LoRa.read();
		}
	}
	receivedHead=false;
	CUaddr = 0;
	type=0;
	_delay_ms(100);
	logger(LOGACT_lora_rx);
	_delay_ms(100);
	serialEnabler(false, false, true);
}

void ROSY_Lora_End::def() {

}

void ROSY_Lora_End::LoRaInit(uint8_t id, uint8_t ss, uint8_t rs, uint8_t dio0, void(*serCB)(bool, bool, bool), void(*logCB)(uint8_t)) {
	logger = logCB;
	LoRa.setPins(ss, rs, dio0);
	while (!LoRa.begin(866E6));
	logger(LOGACT_init_LoRa);
	LoRa.setSyncWord(LoRa_NetID);
	LoRa.enableCrc();
	LoRa.onReceive(LoRaReciver);
	serialEnabler = serCB;
	packet[0] = id;
	packet[1]=PKT_type_LET;
	LoRa.receive();
	dropReceived = false;
}

void ROSY_Lora_End::PKT_setStatus(uint8_t INF, uint16_t SN, uint16_t RSSI, uint16_t BV, uint16_t BA) {
	packet[OFFSET_PKT_STATUS] = INF;
	uint8_t* temp = (uint8_t*)&BV;
	packet[OFFSET_PKT_STATUS + 1] = temp[0];
	packet[OFFSET_PKT_STATUS + 2] = temp[1];
	temp = (uint8_t*)&BA;
	packet[OFFSET_PKT_STATUS + 3] = temp[0];
	packet[OFFSET_PKT_STATUS + 4] = temp[1];
	temp = (uint8_t*)&SN;
	packet[OFFSET_PKT_STATUS + 5] = temp[0];
	packet[OFFSET_PKT_STATUS + 6] = temp[1];
	temp = (uint8_t*)&RSSI;
	packet[OFFSET_PKT_STATUS + 7] = temp[0];
	packet[OFFSET_PKT_STATUS + 8] = temp[1];
}

void ROSY_Lora_End::PKT_setData(uint8_t n, uint8_t* buff) {
	for (uint16_t i = 0; i < DIM_EEPROM_BUFF - 1; i++) {
		packet[n * (OFFSET_PKT_END - OFFSET_PKT_NEW) + OFFSET_PKT_NEW + i] = buff[i];
	}
}

void ROSY_Lora_End::PKT_send(uint8_t nLET) {
	LoRa.beginPacket();
	LoRa.write(packet, nLET * OFFSET_PKT_END - (nLET - 1) * OFFSET_PKT_NEW);
	LoRa.endPacket();
	logger(LOGACT_lora_tx);
	_delay_ms(2000);
	LoRa.receive();
}

void ROSY_Lora_End::LoRa_getINFO(float* SN, float* RSSI) {
	*SN = (arrSN[0] + arrSN[1] + arrSN[2] + arrSN[3] + arrSN[4]) / 5;
	*RSSI = (arrRSSI[0] + arrRSSI[1] + arrRSSI[2] + arrRSSI[3] + arrRSSI[4]) / 5;
}

uint8_t ROSY_Lora_End::packet[TOT_LET_PER_PKT * OFFSET_PKT_END - (TOT_LET_PER_PKT - 1) * OFFSET_PKT_NEW] = { 0 };
uint32_t ROSY_Lora_End::received_timestamp=0;

int ROSY_Lora_End::arrRSSI[5] = { 0 };
float ROSY_Lora_End::arrSN[5] = { 0 };
uint8_t ROSY_Lora_End::arrI = 0;
ROSY_Lora_End::PKT_RCV* ROSY_Lora_End::pktRX = NULL;
int ROSY_Lora_End::dropReceived=false;

void(*ROSY_Lora_End::serialEnabler)(bool, bool, bool) = (void(*)(bool, bool, bool)) ROSY_Lora_End::def;
void(*ROSY_Lora_End::logger)(uint8_t) = (void(*)(uint8_t)) ROSY_Lora_End::def;

#else

void ROSY_Lora_Gat::LoRaReciver(int size) {
	if (!isAvailable) {
		if (!(size >= 3)) {
			return;
		}
		while (!LoRa.available());
		uint8_t CUaddr = LoRa.read();
		Serial.println("Addr "+String(CUaddr));
		while (!LoRa.available());
		uint8_t type = LoRa.read();
		Serial.println("Type "+String(type));
		if (CUaddr == TargetAddr) {
			if(type == PKT_type_LET){
				while (!LoRa.available());
				uint8_t LETtype=LoRa.read();
				uint8_t n = 0;
				if ((LETtype & INF_LET_1) == INF_LET_1) {
					n++;
					if ((LETtype & INF_LET_2) == INF_LET_2) {
						n++;
						if ((LETtype & INF_LET_3) == INF_LET_3) {
							n++;
						}
					}
				}
				if (!(LoRa.available() >= n * OFFSET_PKT_END - (n - 1) * OFFSET_PKT_NEW - 3)) {
					while (LoRa.available()) {
						LoRa.read();
					}
					return;
				}
				buffer[0] = CUaddr;
				buffer[1] = LETtype;
				for (uint8_t i = 0; i < TOT_LET_PER_PKT * OFFSET_PKT_END - (TOT_LET_PER_PKT - 1) * OFFSET_PKT_NEW - 3; i++) {
					if(i< n * OFFSET_PKT_END - (n - 1) * OFFSET_PKT_NEW - 3){
						while (!LoRa.available());
						buffer[i + 3] = LoRa.read();
					}
					else{
						buffer[i + 3]=0;
					}
				}
				isAvailable = true;
				isACK = true;
				rcvPKT();
				startRequest = 0;				
			}
			else{
				while (LoRa.available()) {
					LoRa.read();
				}	
			}
		}
		else {
			while (LoRa.available()) {
				LoRa.read();
			}
		}
	}
	else {
		while (LoRa.available()) {
			LoRa.read();
		}
	}
}

void ROSY_Lora_Gat::def() {

}

void ROSY_Lora_Gat::LoRaInit(uint8_t ss, uint8_t rs, uint8_t dio0, void(*rcvCB)(void)) {
	LoRa.setPins(ss, rs, dio0);
	while (!LoRa.begin(866E6));
	LoRa.setSyncWord(LoRa_NetID);
	LoRa.enableCrc();
	LoRa.onReceive(LoRaReciver);
	rcvPKT = rcvCB;
	LoRa.receive();
}

bool ROSY_Lora_Gat::failedRQST() {
	cli();
	if (!isACK && !isAvailable) {
		if (startRequest > millis()) {
			if (millis() + (pow(2, sizeof(unsigned long) * 8) - startRequest) > LoRa_time_RQST) {
				isAvailable = true;
				startRequest = 0;
				sei();
				return true;
			}
		}
		else {
			if (millis() - startRequest > LoRa_time_RQST) {
				isAvailable = true;
				startRequest = 0;
				sei();
				return true;
			}
		}
	}
	sei();
	return false;
}

bool ROSY_Lora_Gat::requestData(uint8_t id, uint32_t datetime) {
	failedRQST();
	if (isAvailable && !isACK) {
		TargetAddr = id;
		isAvailable = false;
		LoRa.beginPacket();
		LoRa.write(TargetAddr);
		LoRa.write(PKT_type_RQST);
		uint8_t *pt=(uint8_t*) &datetime;
		for(int i=0;i<4;i++){
			LoRa.write(pt[i]);
		}
		LoRa.endPacket();
		startRequest = millis();
		_delay_ms(200);
		LoRa.receive();
		return true;
	}
	return false;
}

void ROSY_Lora_Gat::sendACK(bool isLast) {
	if (isACK) {
		LoRa.beginPacket();
		LoRa.write(TargetAddr);
		LoRa.write(PKT_type_ACK);
		LoRa.endPacket();
		if (isLast) {
			isACK = false;
		}
	}
}

uint8_t ROSY_Lora_Gat::TargetAddr = 0;
bool ROSY_Lora_Gat::isAvailable = true;
bool ROSY_Lora_Gat::isACK = false;
unsigned long ROSY_Lora_Gat::startRequest = 0;
uint8_t ROSY_Lora_Gat::buffer[TOT_LET_PER_PKT * OFFSET_PKT_END - (TOT_LET_PER_PKT - 1) * OFFSET_PKT_NEW] = { 0 };
void(*ROSY_Lora_Gat::rcvPKT)(void) = ROSY_Lora_Gat::def;

#endif
