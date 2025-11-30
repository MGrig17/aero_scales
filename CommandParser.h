#ifndef COMMAND_PARSER_H
#define COMMAND_PARSER_H

#include <Arduino.h>
#include "config.h"
#include "Potentiometer.h"
  #include "DisplayHandler.h"

class CommandParser {
private:
    
    bool continuousMeasure;
    bool potMode; // ФЛАГ РЕЖИМА
    bool autoMeasure;        // ← ДОБАВИТЬ: флаг авто-измерений
    float startAngle;        // ← ДОБАВИТЬ: начальный угол
    float endAngle;          // ← ДОБАВИТЬ: конечный угол  
    float angleStep;         // ← ДОБАВИТЬ: шаг изменения
    float currentTargetAngle;// ← ДОБАВИТЬ: текущий целевой угол
    unsigned long lastStepTime; // ← ДОБАВИТЬ: время последнего шага

public:
    CommandParser();
    void handleCommand(const String& command, Potentiometer& pot); // осторожно, ссылка!
    void printHelp();
    
    bool isContinuousMeasure() { return continuousMeasure; }
    bool isPotMode() { return potMode; }  // ← ГЕТТЕР
    bool isAutoMeasure() { return autoMeasure; }  // ← ДОБАВИТЬ геттер

    float getStartAngle() { return startAngle; }
    float getEndAngle() { return endAngle; }
    float getAngleStep() { return angleStep; }
    float getCurrentTargetAngle() { return currentTargetAngle; }
    unsigned long getLastStepTime() { return lastStepTime; }

    void setCurrentTargetAngle(float angle) { currentTargetAngle = angle; }
    void setLastStepTime(unsigned long time) { lastStepTime = time; }
    void stopAutoMeasure() { autoMeasure = false; }

    void moveToAngle(float targetAngle); // перемещение от данного до данного угла
};

extern CommandParser commandParser;

#endif