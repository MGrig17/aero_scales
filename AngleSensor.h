#ifndef ANGLE_SENSOR_H
#define ANGLE_SENSOR_H

#include <Arduino.h>
#include <Wire.h>
#include <AS5600.h>

class AngleSensor {
private:
    AMS_5600 ams5600;          // Драйвер датчика AS5600
    bool magnetDetected;       // Флаг наличия магнита
    float angleOffset;         // Смещение для калибровки нуля

    float convertRawAngleToDegrees(word newAngle); // Преобразование сырого значения угла в градусы
    
public:
    AngleSensor();             // Конструктор
    void begin();              // Инициализация датчика
    void printAngle();         // Вывод угла в Serial
    float getAngle();          // Получение калиброванного угла в пределах (-180--180)
    float getPositiveAngle();          // Получение калиброванного угла в пределах (0-360)
    bool isMagnetDetected() { return magnetDetected; } // Проверка магнита
    void resetAngle();         // Сброс угла (калибровка текущего положения как 0)
};

extern AngleSensor angleSensor; // Глобальный экземпляр

#endif
