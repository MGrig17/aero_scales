/*
Ахтунг! 
Перед запуском:
    1. Проверь схему, все ли туда воткнуто. Особенно касается магнитного датчика
    2. Перепиши пины в config.h
*/

#include "config.h"
#include "WeightSensor.h"
#include "StepperController.h"
#include "AngleSensor.h"
#include "DisplayHandler.h"
#include "CommandParser.h"
#include "Potentiometer.h"

float weight1 = 0, weight2 = 0;
float angle = 0;
unsigned long lastMeasurementTime = 0;
unsigned long lastDisplayUpdate = 0;
unsigned long lastStepTime = 0;

// unsigned long lastMotorActivity = 0;
// const unsigned long MOTOR_IDLE_TIMEOUT = 5000; // 5 секунд бездействия

Potentiometer pot(POT_PIN);

void pot_func();
void handleContinuousMeasurements();
void handleAutoMeasurements();
void updateDisplays();


void setup() {
    Serial.begin(SERIAL_BAUD_RATE);

    delay(2000);  // Важно: дать время на открытие Serial Monitor    

//========DEBUG START=========(проверка подключения 2х экранов и магнитного датчика)

 // Сначала I2C сканер
    Serial.println("=== I2C SCANNER ===");
    Wire.begin();
    byte error, address;
    int nDevices = 0;
    for(address = 1; address < 127; address++ ) {
        Wire.beginTransmission(address);
        error = Wire.endTransmission();
        if (error == 0) {
            Serial.print("I2C device found at 0x");
            Serial.println(address, HEX);
            nDevices++;
        }
    }
    if (nDevices == 0) {
        Serial.println("No I2C devices found");
    }
    Serial.println("=== SCAN COMPLETE ===");

//========DEBUG END=========

    // Инициализация всех модулей
    Serial.println(F("Initializing displayHandler..."));
    displayHandler.begin();
    Serial.println(F("DisplayHandler initialized!"));
    Serial.println(F("Initializing weightSensor..."));
    weightSensor.begin();
    Serial.println(F("WeightSensor initialized!"));
    Serial.println(F("Initializing stepperController..."));
    stepperController.begin();
    Serial.println(F("StepperController initialized!"));
    Serial.println(F("Before angle sensor..."));
    angleSensor.begin(); // ← Если зависнет здесь, будем знать
    Serial.println(F("After angle sensor..."));
    Serial.println(F("Initializing potentiometer..."));
    pot.begin();
    Serial.println(F("Potentiometer initialized!"));
    Serial.println(F("=== INTEGRATED SYSTEM READY ==="));
    commandParser.printHelp();
}

void loop() {

    // Обработка команд (всегда активна)
    if (Serial.available() > 0) {
        String command = Serial.readStringUntil('\n');
        commandParser.handleCommand(command, pot);
        
    }
        
    // ИЗМЕРЕНИЯ ТОЛЬКО В АВТО-РЕЖИМЕ 
    pot_func(); // режим управления через потенциометр
    handleContinuousMeasurements();  // НЕПРЕРЫВНЫЕ ИЗМЕРЕНИЯ В ФОРМАТЕ Angle+Weights    
    handleAutoMeasurements();// ДЛЯ АВТО-ИЗМЕРЕНИЙ  
    updateDisplays(); // ОБНОВЛЕНИЕ ДИСПЛЕЕВ (ВСЕГДА)

    stepperController.run();
    delay(10); // если ставить больше, чем 10 -- help перестает реагировать на команды ._.
}

//==================================МЕТОДЫ===========================================

// Автоматическое управление от потенциометра (ТОЛЬКО в режиме pot)
void pot_func() {
    if (commandParser.isPotMode()) {
        // Прямое преобразование вместо calculateDelta()
        int targetSteps = map(pot.readRaw(), 0, 1023, -500, 500); // не от 0 до 1000 
        int currentSteps = pot.getPosition();
        int delta = targetSteps - currentSteps;
    
        if (abs(delta) > 2) {
            
            stepperController.moveSteps(delta); // 
            pot.setPosition(targetSteps); // точная установка позиции 
            
        }
    }
}

// ФУНКЦИЯ АВТО-ИЗМЕРЕНИЙ
void handleAutoMeasurements() {
    if (commandParser.isAutoMeasure()) {
        unsigned long currentTime = millis();
        
        // Проверяем интервал (3 секунды)
        if (currentTime - lastStepTime >= 3000) {
            
            // Получаем текущие данные
            float currentAngle = angleSensor.getAngle();
            float weight1, weight2;
            weightSensor.readValues(weight1, weight2);
            
            
            Serial.print(currentAngle, 1);
            Serial.print("°\t\t");
            Serial.print(weight1, 2);
            Serial.print("g\t\t");
            Serial.print(weight2, 2);
            Serial.println("g");
            
            // Проверяем достижение конечного угла
            float currentTarget = commandParser.getCurrentTargetAngle();
            
            if ((commandParser.getAngleStep() > 0 && currentTarget >= commandParser.getEndAngle()) ||
                (commandParser.getAngleStep() < 0 && currentTarget <= commandParser.getEndAngle())) {
                
                // Достигли конечного угла - завершаем
                Serial.println("=== AUTO MEASUREMENT COMPLETED ===");
                commandParser.stopAutoMeasure();
                
            } else {
                // Переходим к следующему углу
                float nextTarget = currentTarget + commandParser.getAngleStep();
                commandParser.setCurrentTargetAngle(nextTarget);
                commandParser.moveToAngle(nextTarget);
                lastStepTime = currentTime;
            }
        }
    }
}

// ФУНКЦИЯ НЕПРЕРЫВНЫХ ИЗМЕРЕНИЙ
void handleContinuousMeasurements() {
    if (commandParser.isContinuousMeasure() && 
        millis() - lastMeasurementTime >= MEASUREMENT_INTERVAL) {
    
        float angle = angleSensor.getAngle();
        float weight1, weight2;
        weightSensor.readValues(weight1, weight2);
    
        // Вывод в формате таблицы
        Serial.print(angle, 1);
        Serial.print(F("°\t\t"));
        Serial.print(weight1, 2);
        Serial.print(F("g\t\t"));
        Serial.print(weight2, 2);
        Serial.println(F("g"));
    
        lastMeasurementTime = millis();
    }
}

// ФУНКЦИЯ ОБНОВЛЕНИЯ ДИСПЛЕЕВ
void updateDisplays() {
    if (millis() - lastDisplayUpdate >= 500) {
        // ПРОВЕРКА МАГНИТА:
        if (angleSensor.isMagnetDetected()) {
            float angle = angleSensor.getAngle();
            displayHandler.displayAngle(angle);        
        } else {
            displayHandler.displayNoMagnet();
        }
    
        // ВЫВОД ВЕСОВ на отдельный дисплей (0x3F)
        weightSensor.readValues(weight1, weight2);
        displayHandler.displayWeights(weight1, weight2);
    
        lastDisplayUpdate = millis();
    }
}
