#ifndef WATER_PUMP
#define WATER_PUMP

#include "Models.h"

enum PumpMode {
  MANUAL,
  AUTO
};

class WaterPump {
  private:
    int _pin;
    int _dir1;
    int _dir2;
    int _mSpeed;
    bool _isOn;
    PumpMode _mode;
  public:
    WaterPump(int pin, int dir1, int dir2, int mSpeed = 255);
    void on(bool isUserCmd=false);
    void off(bool isUserCmd=false);

    void autoMode();
    void manualMode();
    void changeSpeed(int newSpeed);
    
    WaterPumpModel getState();
    bool stateChanged;
};

#endif
