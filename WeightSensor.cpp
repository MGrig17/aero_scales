#include "WeightSensor.h"

float CALIBRATION_FACTOR_1 = 4500.0;
float CALIBRATION_FACTOR_2 = 1690.0;

WeightSensor weightSensor;

WeightSensor::WeightSensor() 
    : calFactor1(CALIBRATION_FACTOR_1), calFactor2(CALIBRATION_FACTOR_2) {
}

void WeightSensor::begin() {
    scale1.begin(SENSOR1_DT_PIN, SENSOR1_SCK_PIN);
    scale2.begin(SENSOR2_DT_PIN, SENSOR2_SCK_PIN);
    
    scale1.set_scale(calFactor1);
    scale2.set_scale(calFactor2);
    scale1.tare();
    scale2.tare();
}

// обнуление показаний
void WeightSensor::tare() {
    scale1.tare();
    scale2.tare();
}

// типо "калибровка"
void WeightSensor::setCalibrationFactor(int sensorNum, float factor) {
   if (sensorNum == 1) {
       calFactor1 = factor;
       scale1.set_scale(calFactor1);
   } else {
       calFactor2 = factor;
       scale2.set_scale(calFactor2);
   }
}

// чтение данных с ТД
void WeightSensor::readValues(float& value1, float& value2) {
    value1 = scale1.get_units(5);
    value2 = scale2.get_units(5);
    
    if (abs(value1) < 0.5) value1 = 0;
    if (abs(value2) < 0.5) value2 = 0;
}

// Измерение
void WeightSensor::takeMeasurement() {
    float weight1 = scale1.get_units(5);
    float weight2 = scale2.get_units(5);
    
    Serial.print(millis());
    Serial.print("\t");
    Serial.print(weight1, 2);
    Serial.print("\t\t");
    Serial.print(weight2, 2);
    Serial.println();
}
