#include "CommandParser.h"
#include "WeightSensor.h"
#include "StepperController.h"
#include "AngleSensor.h"
#include "Potentiometer.h"

CommandParser commandParser; // Создание глобального экземпляра парсера команд

CommandParser::CommandParser() // КОНСТРУКТОР
    : continuousMeasure(false), potMode(false), autoMeasure(false),
      startAngle(0), endAngle(0), angleStep(0), currentTargetAngle(0),
      lastStepTime(0) {
}

// Функция перемещения 
void CommandParser::moveToAngle(float targetAngle) {
    const float TOLERANCE = 0.3f;
    const float STEPS_PER_DEGREE = 16.0f; /////////
    const int MAX_STEPS_PER_MOVE = 65;
    
    float currentAngle = angleSensor.getAngle();
    int iteration = 0;
    const int MAX_ITERATIONS = 10;
    
    Serial.print("Target: ");
    Serial.print(targetAngle, 1);
    Serial.print("°, Current: ");
    Serial.print(currentAngle, 1);
    Serial.println("°");
    
    while (abs(currentAngle - targetAngle) > TOLERANCE && iteration < MAX_ITERATIONS) {
        iteration++;
        
        float angleDiff = targetAngle - currentAngle;
        
        if (angleDiff > 180.0f) angleDiff -= 360.0f;
        else if (angleDiff < -180.0f) angleDiff += 360.0f;
        
        int steps = round(angleDiff * STEPS_PER_DEGREE) * -1; /////
        
        if (abs(steps) > MAX_STEPS_PER_MOVE) {
            steps = (steps > 0) ? MAX_STEPS_PER_MOVE : -MAX_STEPS_PER_MOVE;
        }
        
        if (abs(steps) < 3) break;
        
        // ПРОВЕРЯЕМ ОГРАНИЧЕНИЯ ДВИЖЕНИЯ
        long newPosition = stepperController.getTotalSteps() + steps;
        if (!stepperController.canMoveTo(newPosition)) {
            Serial.println("Movement limit reached! Stopping.");
            break;
        }
        
        Serial.print("Step ");
        Serial.print(iteration);
        Serial.print(": ");
        Serial.print(steps);
        Serial.print(" steps");
        
        stepperController.moveSteps(steps);
        delay(800);
        
        currentAngle = angleSensor.getAngle();
        
        Serial.print(" -> ");
        Serial.print(currentAngle, 1);
        Serial.println("°");
    }
    
    Serial.print("Final: ");
    Serial.println(angleSensor.getAngle(), 1);
}

void CommandParser::handleCommand(const String& command, Potentiometer& pot) {
    String cmd = command;
    cmd.trim();
    
    
    if (cmd == "stop") {
        continuousMeasure = false;
        Serial.println("=== MEASUREMENTS STOPPED ===");
    } 
    else if (cmd == "tare") {
        weightSensor.tare();
        Serial.println("Tare reset!");
    } 
    else if (cmd == "status") {
        stepperController.printStatus();
        float currentAngle = angleSensor.getAngle();
        Serial.print("Current angle: ");
        Serial.print(currentAngle, 1);
        Serial.println("°");
    } 

    else if (cmd == "home") {
        stepperController.home();
    }

    else if (cmd == "steps") {  
        Serial.print("Total steps from home: ");
        Serial.println(stepperController.getTotalSteps());
    }

    // else if (cmd == "start-measure") { 
    //     // Получаем данные
    //     float angle = angleSensor.getAngle();
    //     float weight1, weight2;
    //     weightSensor.readValues(weight1, weight2);

    //     continuousMeasure = true;
    //     potMode = false;
    //     autoMeasure = false; // ←  выключаем авто-режим
        
    //     Serial.println("=== CONTINUOUS MEASUREMENTS STARTED ===");
    //     // Вывод в формате таблицы
    //     Serial.println("Angle\t\t1TD\t\t2TD");
    //     Serial.print(angle, 1);
    //     Serial.print("°\t\t");
    //     Serial.print(weight1, 2);
    //     Serial.print("g\t\t");
    //     Serial.print(weight2, 2);
    //     Serial.println("g");
    // }

    // ↓↓↓ ДОБАВИТЬ НОВУЮ КОМАНДУ measure С ПАРАМЕТРАМИ ↓↓↓
    else if (cmd.startsWith("measure ")) {
        // Парсим параметры: "measure startAngle endAngle step"
        int firstSpace = cmd.indexOf(' ');
        int secondSpace = cmd.indexOf(' ', firstSpace + 1);
        int thirdSpace = cmd.indexOf(' ', secondSpace + 1);
        
        if (firstSpace == -1 || secondSpace == -1) {
            Serial.println("Error: Usage: measure startAngle endAngle step");
            return;
        }

        startAngle = cmd.substring(firstSpace + 1, secondSpace).toFloat();
        endAngle = cmd.substring(secondSpace + 1, thirdSpace).toFloat();
        
        if (thirdSpace != -1) {
            angleStep = cmd.substring(thirdSpace + 1).toFloat();
        } else {
            angleStep = 1.0; // шаг по умолчанию
        }
        
        // Валидация параметров
        if (angleStep == 0) {
            Serial.println("Error: Step cannot be zero!");
            return;
        }
        
        // Настраиваем автоматические измерения
        autoMeasure = true;
        continuousMeasure = false; // Выключаем непрерывный режим
        potMode = false;
        
        currentTargetAngle = startAngle;
        lastStepTime = millis();
        
        Serial.println("=== AUTO MEASUREMENT STARTED ===");
        Serial.print("Range: ");
        Serial.print(startAngle);
        Serial.print("° to ");
        Serial.print(endAngle);
        Serial.print("°, Step: ");
        Serial.print(angleStep);
        Serial.println("°");
        Serial.println("Time\tAngle\t\t1TD\t\t2TD");
        
        // Двигаем к начальному углу
        moveToAngle(startAngle);
    }

    else if (cmd == "mode_auto") {
        potMode = false;  //  РЕЖИМ КОМАНД
        Serial.println("AUTO MODE: Use commands for control");
    }

    else if (cmd == "mode_pot") {
        potMode = true;   // ← РЕЖИМ ПОТЕНЦИОМЕТРА
        continuousMeasure = false;
        autoMeasure = false;
        
        // ВЫВОД СООБЩЕНИЯ ПРИ ВХОДЕ В РЕЖИМ
        displayHandler.displayMessage("Turn pot to", "min position!");
        Serial.println("POTENTIOMETER MODE: Turn potentiometer to minimum position!");

        // Ждем установки в минимальное положение
        int potValue = pot.readSteps();
        while (potValue > 0) {
            delay(200);
            potValue = pot.readSteps();
        }

         displayHandler.displayMessage("Pot ready!", "Thank you!");
        Serial.println("Potentiometer initialized! Ready for control.");
        delay(1000);
    
        // Очищаем и возвращаем нормальный вид дисплеям
        displayHandler.clear();
    
        Serial.println("POTENTIOMETER MODE: Use pot for control");        
    }

    else if (cmd == "pot") {
        Serial.print("Pot position: ");
        Serial.println(pot.getPosition());
    }
    else if (cmd == "pot_reset") {
        pot.setPosition(0);
        Serial.println("Pot position reset to 0");
    }

    else if (cmd == "angle") {
        float currentAngle = angleSensor.getAngle();
        Serial.print("Current angle: ");
        Serial.print(currentAngle, 1);
        Serial.println("°");
    }
    else if (cmd == "time"){
        unsigned long currentTime = millis();
        Serial.print(currentTime / 1000);
        Serial.print("s\t");
    }

    else if (cmd == "help") {
        printHelp();
    } 
    else if (cmd.toInt() != 0 || cmd.startsWith("-")) {
        stepperController.moveSteps(cmd.toInt());
    }
     
    else {
        Serial.println("Unknown command: " + cmd);
        printHelp();
    }
}

void CommandParser::printHelp() {
    Serial.println();
    Serial.println(F("=== HELP ==="));
    Serial.println(F("mode_auto   - command mode (default)"));
    Serial.println(F("mode_pot    - potentiometer mode"));
    Serial.println(F("measure (start end step) - auto measurements (degrees)"));
    Serial.println(F("stop        - stop measurements"));  // в mode_auto
    Serial.println(F("tare        - reset tare for sensors"));  // в mode_auto
    Serial.println(F("status      - show stepper status"));  // в mode_auto
    Serial.println(F("steps       - show total steps from home"));  // в mode_auto
    Serial.println(F("home        - home stepper"));  // в mode_auto
    Serial.println(F("N           - move N steps"));  // в mode_auto
    //Serial.println(F("time"));
    //Serial.println(F("start-measure - read angle + weights")); // в mode_auto
    //Serial.println(F("angle        - return angle"));
    Serial.println(F("help        - show this help"));
    Serial.println(F("=================="));
}
