#include "Arduino.h"
#include <Wire.h>
#include "Adafruit_INA219.h"
Adafruit_INA219::Adafruit_INA219(uint8_t addr) 
{
  ina219_i2caddr = addr;
  ina219_currentDivider_mA = 0;
  ina219_powerMultiplier_mW = 0.0f;
}
bool Adafruit_INA219::begin(TwoWire *theWire) 
{
  i2c_dev = new Adafruit_I2CDevice(ina219_i2caddr, theWire);
  if (!i2c_dev->begin())  return false;
  init();
  return true;
}
void Adafruit_INA219::init() 
{
  setCalibration_32V_2A();
}
int16_t Adafruit_INA219::getBusVoltage_raw()
 {
  uint16_t value;
  Adafruit_BusIO_Register bus_voltage_reg = Adafruit_BusIO_Register(i2c_dev, INA219_REG_BUSVOLTAGE, 2, MSBFIRST);
  bus_voltage_reg.read(&value);
  return (int16_t)((value >> 3) * 4);
}
int16_t Adafruit_INA219::getShuntVoltage_raw() 
{
  uint16_t value;
  Adafruit_BusIO_Register shunt_voltage_reg = Adafruit_BusIO_Register(i2c_dev, INA219_REG_SHUNTVOLTAGE, 2, MSBFIRST);
  shunt_voltage_reg.read(&value);
  return value;
}
int16_t Adafruit_INA219::getCurrent_raw() 
{
  uint16_t value;
  Adafruit_BusIO_Register calibration_reg = Adafruit_BusIO_Register(i2c_dev, INA219_REG_CALIBRATION, 2, MSBFIRST);
  calibration_reg.write(ina219_calValue, 2);
  Adafruit_BusIO_Register current_reg = Adafruit_BusIO_Register(i2c_dev, INA219_REG_CURRENT, 2, MSBFIRST);
  current_reg.read(&value);
  return value;
}
int16_t Adafruit_INA219::getPower_raw() 
{
  uint16_t value;
  Adafruit_BusIO_Register calibration_reg = Adafruit_BusIO_Register(i2c_dev, INA219_REG_CALIBRATION, 2, MSBFIRST);
  calibration_reg.write(ina219_calValue, 2);
  Adafruit_BusIO_Register power_reg = Adafruit_BusIO_Register(i2c_dev, INA219_REG_POWER, 2, MSBFIRST);
  power_reg.read(&value);
  return value;
}
float Adafruit_INA219::getShuntVoltage_mV() 
{
  int16_t value;
  value = getShuntVoltage_raw();
  return value * 0.01;
}
float Adafruit_INA219::getBusVoltage_V() 
{
  int16_t value = getBusVoltage_raw();
  return value * 0.001;
}
float Adafruit_INA219::getCurrent_mA() 
{
  float valueDec = getCurrent_raw();
  valueDec /= ina219_currentDivider_mA;
  return valueDec;
}
float Adafruit_INA219::getPower_mW()
{
  float valueDec = getPower_raw();
  valueDec *= ina219_powerMultiplier_mW;
  return valueDec;
}
void Adafruit_INA219::setCalibration_32V_2A() 
{
  ina219_calValue = 4096;
  ina219_currentDivider_mA = 10;
  ina219_powerMultiplier_mW = 2;
  Adafruit_BusIO_Register calibration_reg = Adafruit_BusIO_Register(i2c_dev, INA219_REG_CALIBRATION, 2, MSBFIRST);
  calibration_reg.write(ina219_calValue, 2);
  uint16_t config = INA219_CONFIG_BVOLTAGERANGE_32V | 
                    INA219_CONFIG_GAIN_8_320MV | INA219_CONFIG_BADCRES_12BIT |
                    INA219_CONFIG_SADCRES_12BIT_1S_532US |
                    INA219_CONFIG_MODE_SANDBVOLT_CONTINUOUS;
  Adafruit_BusIO_Register config_reg = Adafruit_BusIO_Register(i2c_dev, INA219_REG_CONFIG, 2, MSBFIRST);
  config_reg.write(config, 2);
}
void Adafruit_INA219::powerSave(bool on) 
{
  Adafruit_BusIO_Register config_reg = Adafruit_BusIO_Register(i2c_dev, INA219_REG_CONFIG, 2, MSBFIRST);
  Adafruit_BusIO_RegisterBits mode_bits = Adafruit_BusIO_RegisterBits(&config_reg, 3, 0);
  if (on) mode_bits.write(INA219_CONFIG_MODE_POWERDOWN);
  else mode_bits.write(INA219_CONFIG_MODE_SANDBVOLT_CONTINUOUS);
}
void Adafruit_INA219::setCalibration_32V_1A() 
{
  ina219_calValue = 10240;
  ina219_currentDivider_mA = 25;
  ina219_powerMultiplier_mW = 0.8f;
  Adafruit_BusIO_Register calibration_reg = Adafruit_BusIO_Register(i2c_dev, INA219_REG_CALIBRATION, 2, MSBFIRST);
  calibration_reg.write(ina219_calValue, 2);
  uint16_t config = INA219_CONFIG_BVOLTAGERANGE_32V |
                    INA219_CONFIG_GAIN_8_320MV | INA219_CONFIG_BADCRES_12BIT |
                    INA219_CONFIG_SADCRES_12BIT_1S_532US |
                    INA219_CONFIG_MODE_SANDBVOLT_CONTINUOUS;
  Adafruit_BusIO_Register config_reg = Adafruit_BusIO_Register(i2c_dev, INA219_REG_CONFIG, 2, MSBFIRST);
  config_reg.write(config, 2);
}
void Adafruit_INA219::setCalibration_16V_400mA() 
{
  ina219_calValue = 8192;
  ina219_currentDivider_mA = 20;
  ina219_powerMultiplier_mW = 1.0f;
  Adafruit_BusIO_Register calibration_reg = Adafruit_BusIO_Register(i2c_dev, INA219_REG_CALIBRATION, 2, MSBFIRST);
  calibration_reg.write(ina219_calValue, 2);
  uint16_t config = INA219_CONFIG_BVOLTAGERANGE_16V |
                    INA219_CONFIG_GAIN_1_40MV | INA219_CONFIG_BADCRES_12BIT |
                    INA219_CONFIG_SADCRES_12BIT_1S_532US |
                    INA219_CONFIG_MODE_SANDBVOLT_CONTINUOUS;
  Adafruit_BusIO_Register config_reg = Adafruit_BusIO_Register(i2c_dev, INA219_REG_CONFIG, 2, MSBFIRST);
  config_reg.write(config, 2);
}
