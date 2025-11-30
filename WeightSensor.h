#ifndef WEIGHT_SENSOR_H
#define WEIGHT_SENSOR_H

#include "HX711.h"
#include "config.h"

class WeightSensor {
private:
    HX711 scale1, scale2;
    float calFactor1, calFactor2;
    
public:
    WeightSensor();
    void begin();
    void tare();
    void setCalibrationFactor(int sensorNum, float factor);
    void readValues(float& value1, float& value2); // осторожно, ссылка!
    void takeMeasurement();
};

extern WeightSensor weightSensor;

#endif
