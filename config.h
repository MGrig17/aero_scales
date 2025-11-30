#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

// Пины для тензодатчиков
const int SENSOR1_DT_PIN = 4;
const int SENSOR1_SCK_PIN = 6;
const int SENSOR2_DT_PIN = 3;
const int SENSOR2_SCK_PIN = 2;

// Пины для степпера
const int STEPPER_STEP_PIN = 11;
const int STEPPER_DIR_PIN = 12;
const int STEPPER_ENDSTOP_PIN = 13;
const int TMC_ENABLE_PIN = 10;  // Новый пин для включения драйвера EN

// Пин для потенциометра
const int POT_PIN = A0;

// Настройки системы
// const int MAX_VIRTUAL_POSITION = 360;
const unsigned long MEASUREMENT_INTERVAL = 3000;
const int SERIAL_BAUD_RATE = 9600;
const int LCD_I2C_ADDRESS_1 = 0x27; //  на этом LCD будут выводится показания с весов
const int LCD_I2C_ADDRESS_2 = 0x23; //-=- с магнитного датчика
const int AS5600_I2C_ADDRESS = 0x36;

// Калибровочные коэффициенты по умолчанию
extern float CALIBRATION_FACTOR_1; // проверить, нужны ли как extern
extern float CALIBRATION_FACTOR_2;

#endif
