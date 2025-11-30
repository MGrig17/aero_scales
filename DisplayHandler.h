#ifndef DISPLAY_HANDLER_H
#define DISPLAY_HANDLER_H

#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include "config.h"

class DisplayHandler {
private:
    // LiquidCrystal_I2C lcd;
    LiquidCrystal_I2C lcdWeights;  // Для весов 0x3F
    LiquidCrystal_I2C lcdAngle;    // Для угла 0x27
    

public:
    DisplayHandler();
    void begin();
    void displayWeights(float weight1, float weight2);
    void displayAngle(float angle);
    void displayNoMagnet();
    void displayMessage(const String& line1, const String& line2);
    void clear(); // очистка дисплеев
};

extern DisplayHandler displayHandler;

#endif
