#ifndef RFID_PLANT
#define RFID_PLANT

#include <MFRC522.h>
#include "Models.h"

/*
 * RFID PLANT
 * Handles reading and checking of plant RFID tags
 */

class RfidPlant {
  private:
    int _ss;
    int _rst;
    bool _tag_found;
    int _counter;
    PlantModel _plant;
    
    MFRC522 _rfidReader; // reader object for reading plant rfid tags
    MFRC522::MIFARE_Key _key; // required to authenticate against the rfid card
    MFRC522::MIFARE_Key defaultRfidTagKey();

    void loadPlant(); // load a plant from the RFID tag
    //const byte* readBlock(byte block); // helper function for reading blocks from an RFID tag
  public:
    void init(int ss, int rst); // we can only create MFRC522 object after SPI.begin() call
    RfidPlant();
    bool checkPlant();
    bool hasPlant();
    bool stateChanged;
    
    PlantModel getState();
};

#endif
