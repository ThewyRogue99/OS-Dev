#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "io.h"
#include <stdint.h>

#define FOR_PS2_DEVICE 0
#define FOR_PS2_CONTROLLER 1

#define STATE_RELEASE 1
#define STATE_PRESS 2
#define STATE_REPEAT 3

class Keyboard
{
public:
    static void handleKey();

    static bool enableScanning();

    static bool disableScanning();

    static bool setLED(bool ScrollLock, bool NumLock,bool CapsLock);

    static void registerKeyCallback(void (*func)(int state, int scan_code, int key_code));
protected:

    static void getDataResponse();

    typedef struct KeyboardStatus
    {
        bool isOutputBufferFull;
        bool isInputBufferFull;
        bool commandData;
        bool unknown1;
        bool unknown2;
        bool time_out_error;
        bool parity_error;
    }KeyboardStatus;

    static KeyboardStatus getKeyboardStatus();

    static bool sendCommand(uint8_t command);

    static bool sendCommand(uint8_t command, uint8_t data);

    static bool key(int key);
	static bool keyChanged(int key);
	static bool keyWentUp(int key);
	static bool keyWentDown(int key);
	static bool keyToggle(int key);

protected:
    static void setKeys(int state, int scan_code, int key_code);

private:
    static bool scanningEnabled;

    static uint8_t getScanCode();

    static char scan_code_US[];

    static int keyStates[];

    static int lastScanCode;

    static void (*keyCallback)(int state, int scan_code, int key_code);

    static bool keys[];
	static bool keysChanged[];
	static bool toggleKeys[];
};

#endif