#include "WaterPump.h"

WaterPump::WaterPump(int pin, int dir1, int dir2, int mSpeed)
  : _pin(pin), _dir1(dir1), _dir2(dir2), _mSpeed(mSpeed), _mode(AUTO), _isOn(false), stateChanged(false) {
  pinMode(_pin, OUTPUT);
  pinMode(_dir1, OUTPUT);
  pinMode(_dir2, OUTPUT);
  this->off();
};

void WaterPump::autoMode() {
  if (!_mode == AUTO) {
    _mode = AUTO;
    stateChanged = true;
  }
};

void WaterPump::manualMode() {
  if (!_mode == MANUAL) {
    _mode = MANUAL;
    stateChanged = true;
  }
}

void WaterPump::on(bool isUserCmd) {
  if (isUserCmd && _mode == AUTO)
    return;

  if (!isUserCmd && _mode == MANUAL)
    return;

  if (!_isOn) {
    digitalWrite(_dir1, HIGH);
    digitalWrite(_dir2, LOW);
    analogWrite(_pin, _mSpeed);
    _isOn = true;
    stateChanged = true;
  }
};

void WaterPump::off(bool isUserCmd) {
  if (isUserCmd && _mode == AUTO)
    return;

  if (!isUserCmd && _mode == MANUAL)
    return;

  if (_isOn) {
    digitalWrite(_dir1, LOW);
    digitalWrite(_dir2, LOW);
    _isOn = false;
    stateChanged = true;
  }
};

WaterPumpModel WaterPump::getState() {
  WaterPumpModel result;
  if (_isOn)
    strcpy(result.state, "1");
  else strcpy(result.state, "0");

  if (_mode == AUTO)
    strcpy(result.mode, "a");
  else strcpy(result.mode, "m");

  result.mSpeed = _mSpeed;
  stateChanged = false;

  return result;
};

void WaterPump::changeSpeed(int newSpeed) {
  _mSpeed = constrain(newSpeed, 0, 255);
  analogWrite(_pin, _mSpeed);
};
