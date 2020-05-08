#include "Lighting.h"

Lighting::Lighting(int din, int cs, int clk) : _lights(LedControl(din,clk,cs,0)), stateChanged(false), _isOn(false) {
  // go out of power saving mode and start in high power mode
  _lights.shutdown(0, false);
  changeMode(MEDIUM_POWER);
  _lights.clearDisplay(0);
 
  on();
};

void Lighting::on() {
  if (_isOn)
    return;

  printPattern();
  //lights.shutdown(0,false);
  _isOn = true;
  stateChanged = true;
};

void Lighting::off() {
  if (!_isOn)
    return;

  printEmpty();
  //lights.shutdown(0,true);
  _isOn = false;
  stateChanged = true;
};

// return state as a json string
LightingModel Lighting::getState() {
  LightingModel result;

  result.state = _isOn;

  switch(mode) {
    case HIGH_POWER:
      strcpy(result.mode, "h");
      break;
    case MEDIUM_POWER:
      strcpy(result.mode, "m");
      break;
    case LOW_POWER:
      strcpy(result.mode, "l");
      break;
  };

  stateChanged = false; //set the event trigger flag false
  return result;
};

void Lighting::changeMode(LightingMode newMode) {
  if (mode != newMode) {
    mode = newMode;
    stateChanged = true;
    changeBrightness();
  }
};

void Lighting::printEmpty() {
  for (int i=0; i<8; i++)
    _lights.setRow(0,i,emptyPattern[i]);
};

void Lighting::printPattern() {
  for (int i=0; i<8; i++)
    _lights.setRow(0,i,pattern[i]);
};

void Lighting::changeBrightness() {
  switch(mode) {
    case HIGH_POWER:
      _lights.setIntensity(0, 15);
      break;
    case MEDIUM_POWER:
      _lights.setIntensity(0, 5);
      break;
    case LOW_POWER:
      _lights.setIntensity(0, 1);
      break;
  }
};
