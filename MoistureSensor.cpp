#include "MoistureSensor.h"
#include "Util.h"

MoistureSensor::MoistureSensor(int pin) : _pin(pin), _dryThreshold(400) {
  pinMode(_pin, OUTPUT);
};

MoistureModel MoistureSensor::getState() {
  MoistureModel result;
  result.moistureLevel = _lastReading;
  return result;
};

// take 5 measurements and return the average
int MoistureSensor::readMoisture() {
  double val = 0;

  for (int i=0; i<5; i++) {
    val = rollingAverage(val, analogRead(_pin), i+1);
  };

  return val;
};

void MoistureSensor::setDryThreshold(int threshold) {
  _dryThreshold = constrain(threshold, 0, 1024);
};

bool MoistureSensor::isDry() {
  _lastReading = readMoisture();
  return (_lastReading < _dryThreshold);
};
