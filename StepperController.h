#ifndef STEPPER_CONTROLLER_H
#define STEPPER_CONTROLLER_H

#include <Arduino.h>
//#include "AccelStepper.h"
#include "AngleSensor.h"
#include "config.h"

class StepperController {
private:
    // AccelStepper stepper;
    static const long HORIZONTAL_POS = 590;     // Горизонтальное положение
    static const long MAX_STEPS_FROM_HOME = 500;
    long horisontPos = 590;
    long totalSteps;  // ← ДОБАВИТЬ СЧЕТЧИК шагов относительно положения home
    bool isHomed;
    bool isEnabled = false; // ← ДОБАВИТЬ флаг состояния
    unsigned long lastActivityTime = 0; // ← Таймер внутри класса
    
public:
    StepperController();
    void begin();
    void home();
    void moveSteps(int steps);
    void printStatus();
    void run();
    long getTotalSteps() { return totalSteps; }  // ← ГЕТТЕР
    long getCurrentPosition() { return totalSteps - HORIZONTAL_POS; } // Позиция относительно home

    // Ограничение движения
    bool canMoveTo(long targetSteps);

    void enable();   // Включить драйвер (мотор активен)
    void disable();  // Выключить драйвер (режим покоя)
    //bool isMotorEnabled() { return isEnabled; }
    void checkIdle(); // ← Автоматическая проверка простоя
};

extern StepperController stepperController;

#endif