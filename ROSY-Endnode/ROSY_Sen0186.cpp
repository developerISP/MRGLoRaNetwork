#include "ROSY_Sen0186.h"

bool ROSY_Sen0186::begin()
{
  return this->read();
}

bool ROSY_Sen0186::read()
{
  unsigned long int start = millis();
  uint8_t index = 0;
  bool isEnd = false;
  this-> isRead = false;
  for (int i = 0; i < __BUFFER_SIZE__; i++) databuffer[i] = NAN;
  while (!isEnd)
  {
    if(index >= 35)
    {
      isEnd = true;
      this->isRead = true;
    }
    else if((start > millis()) ? (millis() + (pow(2, 32) - 1) - start) : (millis() - start) > 10 * 1000)
    {
      isEnd = true;
      this->isRead = false;
    }
    else if (Serial1.available())
    {
      this->databuffer[index++] = Serial1.read();
      if (this->databuffer[0] != 'c') index = -1;
    }
  }
  if(!this->isRead) for(int i = 0; i < __BUFFER_SIZE__; i++) databuffer[i] = NAN;
  return this->isRead;
}

int ROSY_Sen0186::transCharToInt(char *_buffer, int _start, int _stop)
{
  int result = 0;
  int num = _stop - _start + 1;
  int arr_temp[num];
  for (int _index = _start; _index <= _stop; _index++)
  {
    arr_temp[_index - _start] = _buffer[_index] - '0';
    result = 10 * result + arr_temp[_index - _start];
  }
  return result;
}


float ROSY_Sen0186::WindDirection() 
{
  return (this-> isRead) ? (this->transCharToInt(this->databuffer, 1, 3)) :  NAN;
}

float ROSY_Sen0186::WindSpeedAverage() 
{
  return (this-> isRead) ? (0.44704 * this->transCharToInt(this->databuffer, 5, 7)) : NAN;
}

float ROSY_Sen0186::WindSpeedMax() 
{
  return (this-> isRead) ? (0.44704 * this->transCharToInt(this->databuffer, 9, 11))  : NAN;
}

float ROSY_Sen0186::Temperature() 
{
  return (this-> isRead) ? ((this->transCharToInt(this->databuffer, 13, 15) - 32.00) * 5.00 / 9.00)  : NAN;
}

float ROSY_Sen0186::RainfallOneHour()
{
  return (this-> isRead) ? (this->transCharToInt(this->databuffer, 17, 19) * 25.40 * 0.01) :  NAN;
}

float ROSY_Sen0186::RainfallOneDay()
{
  return (this-> isRead) ? (this->transCharToInt(this->databuffer, 21, 23) * 25.40 * 0.01) :  NAN;
}

float ROSY_Sen0186::Humidity()
{
  return (this-> isRead) ? (this->transCharToInt(this->databuffer, 25, 26)) :  NAN;
}

float ROSY_Sen0186::BarPressure()
{
  return (this-> isRead) ? (this->transCharToInt(this->databuffer, 28, 32) / 10.00) :  NAN;
}
