#ifndef MOISTURE_SENSOR
#define MOISTURE_SENSOR

#include "Models.h"

class MoistureSensor {
  private:
    int _pin;
    int _lastReading;
    int _dryThreshold;
  public:
    MoistureSensor(int pin);
    int readMoisture();
    void setDryThreshold(int threshold);
    bool isDry();
    MoistureModel getState();
};

#endif
