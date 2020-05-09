#include <SPI.h>

#include "Lighting.h"
#include "MoistureSensor.h"
#include "WaterPump.h"
#include "RfidPlant.h"
#include "Config.h"

// Setup our sensor classes
Lighting lights(L_DIN, L_CS, L_CLK);
MoistureSensor moisture(M_PIN);
WaterPump pump(P_PIN, P_DIR1, P_DIR2);
RfidPlant plant;

void setup() {
  Serial.begin(9600);
  SPI.begin();
  plant.init(R_SS, R_RST);
}

/*
 * Run our internal sensor and plant logic
 */
void handlePump() {
  // don't bother watering if there is no plant to water
  if (!plant.hasPlant()) {
    pump.off();
  }
  else if (moisture.isDry()) {
    pump.on();
  }
};

/*
 * Device response json string builder
 * Edge host needs to know which smartplant device the message came from
 */
int buildDeviceResponse(char* out, size_t out_len, char* data) {
  if (out_len < 75)
    return 0;

  strcpy(out, "{\"g\":\"");
  strcat(out, guid);
  strcat(out, "\",\"d\":");
  strcat(out, data);
  strcat(out, "}");

  return 1;
};

/*
 * Send smart plant regular state update info
 */
void sendRegularState() {
  Serial.flush();

  char data[30];
  moisture.getState().getJsonString(data, 30);

  char out[75];
  buildDeviceResponse(out, 75, data);

  Serial.println(out);
};

// send a full state update
void sendFullStateUpdate() {
  StateUpdateModel stateUpdate;
  stateUpdate.lightsState = lights.getState();
  stateUpdate.pumpState = pump.getState();
  stateUpdate.plantState = plant.getState();
  stateUpdate.moistState = moisture.getState();

  char data[255];
  stateUpdate.getJsonString(data, 255);

  char out[512];
  buildDeviceResponse(out, 512, data);
  Serial.println(out);
};

/*
 * Send serial data through TX if we have a reportable event
 */
void handleEventTransmitTriggers() {

  if (plant.stateChanged) {
    char data[90];
    plant.getState().getJsonString(data, 90);

    char out[140];
    buildDeviceResponse(out, 140, data);
    Serial.println(out);
  };
  
  // new watering event
  if (pump.stateChanged) {
    char data[55];
    pump.getState().getJsonString(data, 55);

    char out[128];
    buildDeviceResponse(out, 128, data);
    Serial.println(out);
  };

  // new light event
  if (lights.stateChanged) {
    char data[35];
    lights.getState().getJsonString(data, 35);

    char out[128];
    buildDeviceResponse(out, 128, data);
    Serial.println(out);
  };
};

byte readByteParam() {
  byte result;

  Serial.setTimeout(250);
  Serial.readBytes(&result, 1);
  Serial.setTimeout(1000);

  return result;
};

/*
 * Handle incoming commands on the RX serial line
 */
void handleRx() {

  // limit to processing 3 commands from the RX line per loop
  // so that user commands are responsive without potential to
  // block the rest of the program e.g. flooding the command stream
  int executionCredit = 3;
  
  while(executionCredit && Serial.available()) {
    int command = Serial.read();
    if (command < 0)
      return; // nothing could be read in for some reason
    else executionCredit -= 1;

    char str[2];
    switch(command) {
      /*
       * POLL FOR FULL STATE
       */
      case 0:
        sendFullStateUpdate();
        break;

      /*
       * WATE PUMP COMMANDS
       */
      // change the water pump mode between manual/automatic
      case 1:
        {
          byte mode = readByteParam();
          mode = int(mode);

          if (mode)
            pump.autoMode();
          else pump.manualMode();
          break;
        }
      // turn the water pump on
      case 2:
        pump.on(true);
        break;
      // turn the water pump off
      case 3:
        pump.off(true);
        break;
      // change pump speed
      case 4:
        {
          byte newSpeed = readByteParam();
          pump.changeSpeed((int)newSpeed);
          break;
        }

      /*
       * LED LIGHTS COMMANDS
       */
      // turn the light on
      case 5:
        lights.on();
        break;
      // turn the light off
      case 6:
        lights.off();
        break;
      // change light brightness
      case 7:
        {
          byte newMode = readByteParam();
          switch((int)newMode) {
            case 0:
              lights.changeMode(LOW_POWER);
              break;
            case 1:
              lights.changeMode(MEDIUM_POWER);
              break;
            case 2:
              lights.changeMode(HIGH_POWER);
              break;
          };
          break;
        };
        // respond to smart plant discovery query
        case 8:
          char data[4] = {"\"y\""};
          char out[128];
          buildDeviceResponse(out, 128, data);
          Serial.println(out);
          break;
      };    
  };
};

/*
 * Check for RX commands every loop
 * Run internal logic every 5th loop
 * Check and send regular state updates every 6th loop
 */
void loop() {
  // process and respond to incoming polls and commands
  handleRx();
  plant.checkPlant();
  handlePump();

  // Handle event triggers and send a regular update every 5 minutes
  // e.g. if the watering pump was turned on, send this event to the edge hub
  if (!loopCount) {
    sendRegularState();
    handleEventTransmitTriggers();
    loopCount = 10;
  };
  loopCount -= 1;

  //Serial.println(plant.hasPlant());

  delay(3000);
}
