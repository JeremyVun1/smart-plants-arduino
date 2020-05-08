#include "RfidPlant.h"

RfidPlant::RfidPlant() : _tag_found(false), _counter(0) { };

/*
* INITIALISATION
*/
void RfidPlant::init(int ss, int rst) {
  _rfidReader = MFRC522(ss, rst);
  _rfidReader.PCD_Init();

  // create default auth key
  for (byte i = 0; i < 6; i++) {
    _key.keyByte[i] = 0xFF;
  }
};


// ENTRY POINT
bool RfidPlant::checkPlant() {
  //char str[3];

  if (!_rfidReader.PICC_IsNewCardPresent() && !_rfidReader.PICC_ReadCardSerial()) {
    if (_counter > 2) {
      _tag_found = false;
      //strcpy(str, "rm");
      //Serial.println(str);
      _counter = 0;
    };

    return false;
  }
  else if (!_tag_found) {
    //strcpy(str, "tf");
    //Serial.println(str);
    _rfidReader.PICC_ReadCardSerial();
    loadPlant();

    _tag_found = true;
    stateChanged = true;
    _counter = 0;
  }

  if (!_rfidReader.PICC_IsNewCardPresent()) {
    if (_plant.matchUID(_rfidReader.uid.uidByte)) {
      //strcpy(str, "kp");
      //Serial.println(str);
      return true;
    } else {
      //strcpy(str, "np");
      //Serial.println(str);
      _counter = 3;
      return true;
    }
  }
};

void RfidPlant:: loadPlant() {
  // authenticate against block 7
  byte trailerBlock = 7;
  MFRC522::StatusCode status = (MFRC522::StatusCode)_rfidReader.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, trailerBlock, &_key, &(_rfidReader.uid));
  if (status != MFRC522::STATUS_OK) {
    //Serial.print("PCD_Authenticate() failed: ");
    //Serial.println(_rfidReader.GetStatusCodeName(status));
    return;
  }

  /*
   * READ PLANT DATA FROM THE RFID TAG
   *  - plant UID is in block 0 (the RFID tag UID)
   *  - plant id is in block 4
   *  - plant name is in block 5
   *  - plant description is in block 6
   */
  strncpy(_plant.uid, _rfidReader.uid.uidByte, 4);
  char uid[9] = {""};
  for (byte i=0; i<4; i++) {
    if (_rfidReader.uid.uidByte[i] < 0x10)
      strcat(uid, "0");
    else strcat(uid, "");

    char buffer[3];
    sprintf(buffer, "%x", _rfidReader.uid.uidByte[i]);
    strcat(uid, buffer);
  }

  //Serial.println(uid);
  // create our plant data
  if (strcmp(uid, "3985ba6e")) {
    _plant.pId = 1;
    strcpy(_plant.uid, uid);
    strcpy(_plant.pName, "Rose");
    strcpy(_plant.pDesc, "Red");
  }
  else if (strcmp(uid,"9b2d86e")) {
    _plant.pId = 2;
    strcpy(_plant.uid, uid);
    strcpy(_plant.pName, "Sunflower");
    strcpy(_plant.pDesc, "Bright");
  }
  else {
    strcpy(_plant.uid, "00000000");
    _plant.pId = -1;
    strcpy(_plant.pName, "NULL");
    strcpy(_plant.pDesc, "NULL");
  }

  //Serial.println(_plant.pName);

  //_rfidReader.PICC_HaltA();
  _rfidReader.PCD_StopCrypto1();

  /*
  _plant.pId = String((char*)readBlock(4));
  _plant.pName = String((char*)readBlock(5));
  _plant.pDesc = String((char*)readBlock(6));
  _isPlantPresent = true;
  */
};

/*
const byte* RfidPlant::readBlock(byte block) {
  byte buffer[18];
  byte size = sizeof(buffer);

  //MIFARE_Read(blockAddr, buffer, &size);
  //(byte x) (byte x[n])) &(byte x = sizeof(buffer))

  MFRC522::StatusCode status = _rfidReader.MIFARE_Read(block, buffer, &size);
  if (status != MFRC522::STATUS_OK) {
    //Serial.print("MIFARE_read() failed: ");
    //Serial.println(_rfidReader.GetStatusCodeName(status));
    return NULL;
  }
  return buffer;
};
*/

PlantModel RfidPlant::getState() {
  stateChanged = false; // reset event flag
  return _plant;
};

bool RfidPlant::hasPlant() {
  return _tag_found;
};

MFRC522::MIFARE_Key RfidPlant::defaultRfidTagKey() {
  MFRC522::MIFARE_Key result;
  
  for (byte i=0; i<6; i++)
    result.keyByte[i] = 0xFF;
  return result;
};
