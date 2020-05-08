#ifndef MODELS
#define MODELS
#include <Arduino.h>

struct LightingModel {
  bool state = false;
  char mode[2] = {"n"};

  int getJsonString(char* out, size_t out_len) {
    if (out_len < 35)
      return 0;

    strcpy(out, "{\"m\":\"l\",\"d\":{\"s\":\"");

    char str[2];
    if (state)
      strcpy(str, "1");
    else strcpy(str, "0");
    strcat(out, str);

    strcat(out, "\",\"e\":\"");
    strcat(out, mode);
    strcat(out, "\"}}");

    return 1;
  }
};

struct MoistureModel {
  int moistureLevel = 0;

  int getJsonString(char* out, size_t out_len) {
    if (out_len < 30)
      return 0;

    strcpy(out, "{\"m\":\"m\",\"d\":{\"v\":\"");
    
    char num[17];
    itoa(moistureLevel, num, 10);
    strcat(out, num);

    strcat(out, "\"}}");

    return 1;
  };
};

struct WaterPumpModel {
  char state[2] = {"1"};
  char mode[2] = {"a"};
  int mSpeed = 255;

  int getJsonString(char* out, size_t out_len) {
    if (out_len < 55)
      return 0;

    strcpy(out, "{\"m\":\"w\",\"d\":{\"s\":\"");
    strcat(out, state);
    strcat(out, "\",\"e\":\"");
    strcat(out, mode);
    strcat(out, "\",\"v\":\"");

    char num[17];
    itoa(mSpeed, num, 10);
    strcat(out, num);

    strcat(out, "\"}}");

    return 1;
  };
};

struct PlantModel {
  char uid[9] = {"00000000"};
  int pId = -1;
  char pName[14] = "NULL";
  char pDesc[20] = "NULL";

  int getJsonString(char* out, size_t out_len) {
    if (out_len < 90)
      return 0;

    strcpy(out, "{\"m\":\"p\",\"d\":{\"u\":\"");
    strcat(out, uid);
    
    strcat(out, "\",\"i\":\"");
    char num[17];
    itoa(pId, num, 10);
    strcat(out, num);

    strcat(out, "\",\"n\":\"");
    strcat(out, pName);
    strcat(out, "\",\"c\":\"");
    strcat(out, pDesc);
    strcat(out, "\"}}");

    return 1;
  };

  bool matchUID(byte other[]) {
    // if this is then first plant read in
    if (uid == NULL) {
      strncpy(uid, other, 4);
      return false;
    }

    // check if the other uid is different
    for (int i=0; i < 4; i++) {
      if (other[i] != uid[i]) {
        //Serial.println("UID's do not match");
        return false;
      }
    };
    //Serial.println("UID's match!");
    return true;
  };
};

struct StateUpdateModel {
  LightingModel lightsState;
  WaterPumpModel pumpState;
  PlantModel plantState;
  MoistureModel moistState;
  
  int getJsonString(char* out, size_t out_len) {
    if (out_len < 250)
      return 0;

    char buffer[100];
    strcpy(out, "{\"m\":\"u\",\"d\":[");

    lightsState.getJsonString(buffer, 100);
    strcat(out, buffer);
    strcat(out, ",");

    pumpState.getJsonString(buffer, 100);
    strcat(out, buffer);
    strcat(out, ",");

    moistState.getJsonString(buffer, 100);
    strcat(out, buffer);
    strcat(out, ",");

    plantState.getJsonString(buffer, 100);
    strcat(out, buffer);
    strcat(out, "]}");

    return 1;
  }
};

#endif
