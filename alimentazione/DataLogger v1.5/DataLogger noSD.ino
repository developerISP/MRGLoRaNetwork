#include <Wire.h>
#include <Adafruit_INA219.h>

#define CommunicationBaudrate 115200
#define SerialTimeout 1500

Adafruit_INA219 ina219(0x45);

enum States {Iniz, SetDelay, Ready, Waiting, ComVoltage, ComCurrent,
            ComPower, SendCompl} state, statepre;
uint32_t Delay=0, TimerExpired=0;
String DelayStr;
bool INA219Error=false;
bool bEntry=false;
char Voltage[10];
String voltagestr;

void setup() {
  Serial.begin(CommunicationBaudrate);
  Serial.setTimeout(SerialTimeout);
  state=statepre=Iniz;
  statepre=1;
  if(!ina219.begin()){
    INA219Error=true;
  }
}

void loop() {
  if(state!=statepre){
    bEntry=true;
    statepre=state;
  }
  else{
    bEntry=false;
  }
  if(Serial.available()){
    if(Serial.peek()=='A'){
      state=Iniz;
      Serial.read();
    }
  }
  switch(state){
    case Iniz:
        if(Serial.available()){
          if(Serial.read()=='S'){
            state=SetDelay;
          }
        }
        break;
    case SetDelay:
        if(Serial.available()){
          DelayStr=Serial.readString();
          Delay=1000*DelayStr.toInt();
          state=Ready;
        }
        break;
    case Ready:
        if(INA219Error){
          Serial.print('E');
          state=Iniz;
        }
        else{
          Serial.print('R');
          state=Waiting;
        }
        break;
    case Waiting:
        if(bEntry){
          TimerExpired=millis()+Delay;
        }
        if(millis()>=TimerExpired){
          state=ComVoltage;
        }
        break;
    case ComVoltage:
        Serial.print('V');
        voltagestr=String(ina219.getBusVoltage_V(), 3);
        voltagestr.toCharArray(Voltage, 10);
        Serial.print(Voltage);
        state=ComCurrent;
        break;
    case ComCurrent:
        Serial.print('C');
        Serial.print(ina219.getCurrent_mA(), 3);
        state=ComPower;
        break;
    case ComPower:
        Serial.print('P');
        Serial.print(ina219.getPower_mW(), 3);
        state=SendCompl;
        break;
    case SendCompl:
        Serial.print('F');
        state=Waiting;
        break;
  }
}
