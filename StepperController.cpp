#include "StepperController.h"
#include "AngleSensor.h"

StepperController stepperController;

StepperController::StepperController() : isHomed(false) {
}

void StepperController::enable() {
    digitalWrite(TMC_ENABLE_PIN, LOW);  // Включить драйвер
    isEnabled = true;
    lastActivityTime = millis(); // ← СБРОС ТАЙМЕРА при любом движении
    delay(10); // Дать время на включение
}

void StepperController::disable() {
    digitalWrite(TMC_ENABLE_PIN, HIGH); // Выключить драйвер
    isEnabled = false;
}

void StepperController::checkIdle() {
    const unsigned long IDLE_TIMEOUT = 10000; // 10 секунд
    
    if (isEnabled && (millis() - lastActivityTime > IDLE_TIMEOUT)) {
        disable();
        Serial.println("Motor auto-disabled (idle)");
    }
}

void StepperController::begin() {
    pinMode(STEPPER_ENDSTOP_PIN, INPUT_PULLUP);
    pinMode(STEPPER_STEP_PIN, OUTPUT);
    pinMode(STEPPER_DIR_PIN, OUTPUT);
    pinMode(TMC_ENABLE_PIN, OUTPUT);
    
    digitalWrite(TMC_ENABLE_PIN, LOW);  // Включить драйвер
    disable(); // начинаем в выключенном сост
    home();
}

// поиск "нуля"
void StepperController::home() {
    Serial.println("Homing...");
    enable(); // ← ВКЛЮЧИТЬ ПЕРЕД homing
    
    // Движение к концевику
    digitalWrite(STEPPER_DIR_PIN, LOW);
    while(digitalRead(STEPPER_ENDSTOP_PIN) != HIGH) {
        digitalWrite(STEPPER_STEP_PIN, HIGH);
        delayMicroseconds(2000);
        digitalWrite(STEPPER_STEP_PIN, LOW);
        delayMicroseconds(2000);
    }

    totalSteps = 0; // СБРОС СЧЕТЧИКА ПРИ homing
    
    // Отъезд от концевика
    digitalWrite(STEPPER_DIR_PIN, HIGH);
    for(int i = 0; i < horisontPos; i++) {
        digitalWrite(STEPPER_STEP_PIN, HIGH);
        delayMicroseconds(2000);
        digitalWrite(STEPPER_STEP_PIN, LOW);
        delayMicroseconds(2000);
    }
    
    totalSteps += HORIZONTAL_POS; // УЧЕСТЬ ШАГИ ОТЪЕЗДА
    
    isHomed = true;
    angleSensor.resetAngle();
    Serial.println("Homing complete!");
    Serial.println(getCurrentPosition());

    // НЕ выключаем после homing - оставляем включенным
}

// Проверка возможности движения
bool StepperController::canMoveTo(long targetSteps) {
    long relativePos = targetSteps - HORIZONTAL_POS;
    return (abs(relativePos) <= MAX_STEPS_FROM_HOME);
}

// отход на фиксированное количество шагов
void StepperController::moveSteps(int steps) {
    if (!isHomed) {
        Serial.println("Error: Stepper not homed!");
        return;
    }

    // Проверяем не выйдем ли за пределы
    long newTotalSteps = totalSteps + steps;
    if (!canMoveTo(newTotalSteps)) {
        Serial.println("Error: Movement limit reached!");
        return;
    }

    enable(); // ← ВКЛЮЧИТЬ ПЕРЕД ДВИЖЕНИЕМ

    totalSteps += steps; // ОБНОВЛЯЕМ СЧЕТЧИК
    
    Serial.print("Moving ");
    Serial.print(steps);
    Serial.println(" steps");
    
    // Установка направления
    digitalWrite(STEPPER_DIR_PIN, steps > 0 ? HIGH : LOW);
    
    // Выполнение шагов (взят код из tmc-driver.ino)
    for (int i = 0; i < abs(steps); i++) {
        digitalWrite(STEPPER_STEP_PIN, HIGH);
        delayMicroseconds(2000);
        digitalWrite(STEPPER_STEP_PIN, LOW);
        delayMicroseconds(2000);
    }

    // отладка
    Serial.print("Moving ");
    Serial.print(steps);
    Serial.print(" steps (pos: ");
    Serial.print(getCurrentPosition());
    Serial.println(")");
    
    Serial.println("Move complete!");
    // НЕ выключаем сразу - ждем когда мотор действительно остановится
}

void StepperController::printStatus() {
    Serial.print("Stepper status: Homed, Total steps: ");
    Serial.println(totalSteps);
}

void StepperController::run() {
    // Пустая функция, но должна быть объявлена для совместимости с другими компонентами
    checkIdle(); // ← ПРОВЕРЯЕМ ПРОСТОЙ КАЖДЫЙ ЦИКЛ
}


