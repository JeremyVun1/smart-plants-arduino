#ifndef SP_CONFIG
#define SP_CONFIG

/*
 * All the pin numbers
 */
// LED Matrix pins
int L_DIN = 8;
int L_CS = 7;
int L_CLK = 6;

// Moisture sensor pin
int M_PIN = A5;

// Water pump pins
int P_PIN = 3;
int P_DIR1 = 2;
int P_DIR2 = 4;

// RFID reader pins
int R_RST = 9;
int R_SS = 10;

// loop counter
int loopCount = 10;

/*
 * Device GUID
 */
char guid[25] = {"pO+3TrWndUe1aSX3XmF40Q=="};

#endif
