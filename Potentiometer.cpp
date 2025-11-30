#include "Potentiometer.h"
#include "DisplayHandler.h"
#include "WeightSensor.h"   
#include "AngleSensor.h"

// Конструктор: инициализация всех переменных-членов класса
// Использует список инициализации для эффективной установки значений
Potentiometer::Potentiometer(uint8_t potPin, int minVal, int maxVal, int maxStepsVal) 
    : pin(potPin), minPot(minVal), maxPot(maxVal), maxSteps(maxStepsVal), lastValue(0), currentPosition(0) {
      // Все переменные инициализированы в списке инициализации выше 
}

// Инициализация потенциометра
void Potentiometer::begin() {
    pinMode(pin, INPUT);  // Настраиваем пин как вход
    lastValue = readRaw(); // Считываем начальное значение для отслеживания изменений

    int potValue = readSteps();
    if (potValue > 0) {
        while (potValue > 0) {
            delay(200);
            potValue = readSteps();
        }        
    }
}

int Potentiometer::readSteps() {
    int rawValue = readRaw();
    rawValue = constrain(rawValue, minPot, maxPot);
    return map(rawValue, minPot, maxPot, 0, maxSteps);
}

int Potentiometer::readRaw() {
    return analogRead(pin);
}

bool Potentiometer::hasChanged(int threshold) {
    int currentValue = readRaw();
    if (abs(currentValue - lastValue) >= threshold) {
        lastValue = currentValue;
        return true;
    }
    return false;
}

int Potentiometer::calculateDelta() {
    int potValue = readSteps();
    return potValue - currentPosition;
}

void Potentiometer::updatePosition(int steps) {
    currentPosition += steps;
}