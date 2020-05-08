#ifndef LIGHTING
#define LIGHTING

#include <LedControl.h>
#include "Models.h"

enum LightingMode {
  LOW_POWER,
  MEDIUM_POWER,
  HIGH_POWER
};

class Lighting {
  private:
    LedControl _lights;
    byte pattern[8] = {
      B11111111,
      B11111111,
      B11111111,
      B11111111,
      B11111111,
      B11111111,
      B11111111,
      B11111111
    };
    byte emptyPattern[8] = {
      B00000000,
      B00000000,
      B00000000,
      B00000000,
      B00000000,
      B00000000,
      B00000000,
      B00000000
    };
    LightingMode mode;
    
    void printPattern();
    void printEmpty();
    void changeBrightness();
    
    bool _isOn;
  public:
    Lighting(int din, int cs, int clk);
    void on();
    void off();

    LightingModel getState();
    bool stateChanged;

    void changeMode(LightingMode mode);
    void Test();
};

#endif
