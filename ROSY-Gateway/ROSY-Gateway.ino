#define GATEWAY_ID 12
#define ENDNODE_MAX_PER_GATEWAY 10

#include "ROSY_Config.h"
#include "ControlUnitConfig.h"
#include "Compressor.h"
#include "SendDecompressedPacket.h"
#include "ROSY_LoRa.h"
#include "Rtclib.h"

#define ENABLE_DEBUG_MODE true
#define ENABLE_PACKET_DEBUG true 
#define ENABLE_INPUT_ID false
#define ENABLE_INPUT_DATETIME false

bool isRCV = false;
bool moreData = false;
unsigned long StartTime = 0;
unsigned long start = 0;
uint8_t *ADDRESSES=0;
uint32_t datetime = 0;
DateTime dt(0,0,0,0,0,0); 

void rcvCallback()
{
	isRCV = true;
#if ENABLE_DEBUG_MODE
	Serial.println("Packet Received");
#endif
#if ENABLE_PACKET_DEBUG
	for(auto a: ROSY_Lora_Gat::buffer) Serial.print(String(a) + "  ");
	Serial.println();
#endif
	ROSY::_send_decompressed_packet(ROSY_Lora_Gat::buffer);
	moreData = ((ROSY_Lora_Gat::buffer[1] & INF_MORE_DATA) == INF_MORE_DATA) ? true : false; 
#if ENABLE_PACKET_DEBUG
	if(moreData) Serial.println("MORE DATA Request");
#endif
	ROSY_Lora_Gat::sendACK(false);
	_delay_ms(LoRa_time_ACK);
	ROSY_Lora_Gat::sendACK(true);
}


int main()
{
	init();
	Serial.begin(9600);
	ROSY_Lora_Gat::LoRaInit(53, 11, 18, rcvCallback);
#if ENABLE_DEBUG_MODE
	Serial.println("Gateway begin successful");
#endif
	StartTime = millis();
	uint8_t number_ID = 0;
	for (;;)
	{
		_delay_ms(10000);
		
		bool isEnd = false;
		String temp = "";
#if ENABLE_INPUT_ID
		start = millis();
		bool isEndIdRequest = false;
		while(!isEndIdRequest)
		{
			if ((start > millis()) ?( millis() + (pow(2, 32) - 1) - start) : (millis() - start) > 10 * 10000) 
			{
				#if ENABLE_DEBUG_MODE
				Serial.println("Serial timeout ");
				#endif
				isEndIdRequest = true;
			}
			else
			{
				while(!Serial.available())
				{
					if ((start > millis()) ? (millis() + (pow(2, 32) - 1) - start) : (millis() - start) > 10 * 1000) 
					{
						#if ENABLE_DEBUG_MODE
						Serial.println("Serial timeout (not available)");
						#endif
						break;
					}
					else
					{
						Serial.println("Request ID " + String(GATEWAY_ID));
						delay(1000);
					}	
				}
				
				while(Serial.available())
				{
					if ((start > millis()) ?( millis() + (pow(2, 32) - 1) - start) :( millis() - start) > 10 * 1000) 
					{
						#if ENABLE_DEBUG_MODE
						Serial.println("Serial timeout");
						#endif
						break;
					}
					if (Serial.readStringUntil(',') == "ID" && Serial.readStringUntil(',') == String(GATEWAY_ID))
					{
						uint8_t number = (uint8_t) Serial.readStringUntil(',').toInt();
						if(number > ENDNODE_MAX_PER_GATEWAY || number <= 0) break;
						String stringID  = Serial.readStringUntil('\n');
						int counter = 0;
						for (int i = 0; i < stringID.length(); i++) if (stringID.charAt(i) == ',') ++counter;
						if(counter == number)
						{
							number_ID = number;
							delete[] ADDRESSES;
							ADDRESSES = new uint8_t[number];
							for(int i = 0; i < number; i++)
							{
								ADDRESSES[i] = (uint8_t) stringID.substring(0, stringID.indexOf(',')).toInt();
								stringID.replace( stringID.substring( 0, stringID.indexOf(',') + 1 ), "" );
							}
							isEndIdRequest = true;
							Serial.println("ack");
						}
						#if ENABLE_DEBUG_MODE
						else Serial.println("Wrong number ID");
						#endif
					}
				}	
			}
		}
#else 
		number_ID = 1;
		delete[] ADDRESSES;
		ADDRESSES = new uint8_t[number_ID] { 1};
#endif
		if ((StartTime > millis()) ? (millis() + (pow(2, 32) - 1) - StartTime) : (millis() - StartTime) > 10 * 1000)
		{
			StartTime = millis();
			
			for(int i = 0; i < number_ID; i++)
			{
				uint8_t CUaddress = ADDRESSES[i];
			#if ENABLE_DEBUG_MODE
				Serial.println("Endnode ID-> " + String(CUaddress));
			#endif
				moreData = true;
				for (uint8_t j = 0; (j < 3) && moreData; j++) 
				{
					moreData = false;
					isRCV = false;
					for (uint8_t i = 0; (i < 3) && !isRCV; i++)
					{
					#if ENABLE_DEBUG_MODE
						Serial.println("Request type MoreData: " + ( moreData ? String("True") : String("False")));
					#endif
						isRCV = false;
						moreData = false;
					#if ENABLE_DEBUG_MODE
						Serial.println("Request starting to Endnode ID: " + String(CUaddress));
					#endif
						bool isEnd1 = false;
						start = millis();
#if ENABLE_INPUT_DATETIME
						Serial.println("Request DateTime " + String(GATEWAY_ID));
						bool isEndDateTimeRequest = false;
						while(!isEndDateTimeRequest)
						{
							if ((start > millis()) ? (millis() + (pow(2, 32) - 1 - start)) : (millis() - start) > 10 * 1000) 
							{
								#if ENABLE_DEBUG_MODE
								Serial.println("Serial timeout");
								#endif
								isEndDateTimeRequest = true;
							}
							else
							{
								while(!Serial.available())
								{
									if ((start > millis()) ? (millis() + (pow(2, 32) - 1) - start) : (millis() - start) > 10 * 1000) 
									{
										#if ENABLE_DEBUG_MODE
										Serial.println("attesa troppo lunga");
										#endif
										break;
									}
									else
									{
										Serial.println("Request DateTime " + String(GATEWAY_ID));
										delay(1000);
									}	
								}
								
								while(Serial.available())
								{
									if ((start > millis()) ? (millis() + (pow(2, 32) - 1) - start) : (millis() - start) > 10 * 1000) 
									{
										#if ENABLE_DEBUG_MODE
										Serial.println("dati non corretti");
										#endif
										break;
									}
									if (Serial.readStringUntil(',') == "DateTime" && Serial.readStringUntil(',') == String(GATEWAY_ID))
									{
										datetime = Serial.readStringUntil('\n').toInt();
										isEnd1 = true;
										Serial.println("ack");
									}
									else 
									{
										datetime = NULL;
									}
								}	
							}
						}
#else 
						datetime = NULL;
#endif
						ROSY_Lora_Gat::requestData(CUaddress, datetime);
					#if ENABLE_DEBUG_MODE
						Serial.println("Request done to Endnode ID: " + String(CUaddress));
					#endif
						while (!ROSY_Lora_Gat::failedRQST() && !isRCV) _delay_ms(LoRa_time_RQST / 100);
					}
					if (!isRCV) Serial.println("error with " + String(CUaddress) + " code: " + String(NULL));
					if(moreData) _delay_ms(LoRa_time_MORE_DATA*1000);
				}
				_delay_ms(LoRa_time_NEXT_RQST * 1000);
			}
		}
	}
}
