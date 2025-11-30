#include "AngleSensor.h"

AngleSensor angleSensor; // Глобальный экземпляр датчика угла

AngleSensor::AngleSensor() : magnetDetected(false), angleOffset(0) {
} // конструктор со списокм инициализаций

void AngleSensor::begin() {
    // Проверяем наличие магнита (// ← эта проверка уже неявно есть в getAngle())
    // if(ams5600.detectMagnet() == 0) {
    //     Serial.println("Magnet not detected!"); // в этом if проверяем наличие магнита, если его нет -- выбрасываем рпедупреждение, если нет -- скипаем
    //     magnetDetected = false;
    //     return;
    // }
    
    magnetDetected = true; // есть магнит    
  // Устанавливаем текущее положение как 0
    angleOffset = convertRawAngleToDegrees(ams5600.getRawAngle()); // 
    Serial.print("Angle calibrated. Offset: ");
    Serial.println(angleOffset);
}

void AngleSensor::printAngle() {
    if (!magnetDetected) {
        Serial.println("No magnet detected!");
        return;
    }
    
    float angle = getAngle();
    Serial.print("Current angle: ");
    Serial.print(angle, 1);
    Serial.println("°");
}

// Преобразование сырого значения (0-4095) в градусы (0-360)
float AngleSensor::convertRawAngleToDegrees(word newAngle) { 
    return newAngle * 0.087890625;
}

// Получение калиброванного угла относительно нулевой точки
float AngleSensor::getAngle() {          
    word rawAngle = ams5600.getRawAngle();
    float angle = convertRawAngleToDegrees(rawAngle) - angleOffset;
    
    // Нормализация угла к диапазону -180 до +180
    if (angle > 180) angle -= 360;
    else if (angle < -180) angle += 360;
    
    return -angle;
}

// Альтернативная версия - всегда возвращает положительные углы (0-360)
float AngleSensor::getPositiveAngle() {          
    word rawAngle = ams5600.getRawAngle();
    float angle = convertRawAngleToDegrees(rawAngle) - angleOffset;
    if (angle < 0) angle += 360;
    return angle;
}

// Сброс нулевого положения (калибровка)
void AngleSensor::resetAngle() {
    if (!magnetDetected) {
        Serial.println("Cannot reset angle - no magnet!");
        return;
    }
    
    angleOffset = convertRawAngleToDegrees(ams5600.getRawAngle());
    Serial.print("Angle reset to zero. Offset: ");
    Serial.println(angleOffset);
}