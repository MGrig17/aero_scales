#ifndef POTENTIOMETER_H
#define POTENTIOMETER_H

#include <Arduino.h>

class Potentiometer {
private:
    uint8_t pin; // Аналоговый пин, к которому подключен потенциометр
    int minPot; // Минимальное значение с потенциометра (обычно 0)
    int maxPot; // Максимальное значение с потенциометра (обычно 1023)
    int maxSteps; // Максимальное количество шагов для мотора (по умолчанию 500)
    int lastValue; // Последнее прочитанное значение для отслеживания изменений
    int currentPosition;
    
public:
 // Конструктор: инициализирует параметры потенциометра
    // potPin - аналоговый пин (A0, A1, etc.)
    // minVal - минимальное значение АЦП (по умолчанию 0)
    // maxVal - максимальное значение АЦП (по умолчанию 1023)
    // maxStepsVal - максимальное количество шагов мотора (по умолчанию 500)
    Potentiometer(uint8_t POT_PIN, int minVal = 0, int maxVal = 1023, int maxStepsVal = 1090);
  //Методы:
    void begin(); // Инициализация: настраивает пин и считывает начальное значение
    int readSteps(); // Чтение значения, преобразованного в шаги мотора (0 - maxSteps)
    int readRaw(); // Чтение "сырого" значения с АЦП (0 - 1023)

    // Проверка, изменилось ли значение больше чем на порог
    // threshold - минимальное изменение для регистрации (по умолчанию 5)
    // возвращает true, если изменение превысило порог
    bool hasChanged(int threshold = 5);
    void setPosition(int position) { currentPosition = position; }
    int getPosition() { return currentPosition; }
    int calculateDelta();
    void updatePosition(int steps);
};

#endif