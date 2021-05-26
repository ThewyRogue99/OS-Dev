#include "Keyboard.h"

#define US_KEYBOARD_KEY_COUNT 90

bool Keyboard::scanningEnabled = true;

int Keyboard::keyStates[US_KEYBOARD_KEY_COUNT] = { 0 };

bool Keyboard::keys[US_KEYBOARD_KEY_COUNT] = { false };
bool Keyboard::keysChanged[US_KEYBOARD_KEY_COUNT] = { false };
bool Keyboard::toggleKeys[US_KEYBOARD_KEY_COUNT] = { false };

int Keyboard::lastScanCode = -1;

void (*Keyboard::keyCallback)(int state, int scan_code, int key_code) = nullptr;

bool Keyboard::sendCommand(uint8_t command)
{
    KeyboardStatus status = getKeyboardStatus();

    int attempts = 0;

    if(status.isInputBufferFull)
    {
        uint8_t response;
        do
        {
            IO::port_byte_out(0x64, command);
            response = IO::port_word_in(0x60);
            ++attempts;
        } while ((response != 0xFA || response == 0xFE) && attempts < 10);
    }

    return attempts >= 10 ? false : true;
}

bool Keyboard::sendCommand(uint8_t command, uint8_t data)
{
    KeyboardStatus status = getKeyboardStatus();

    int attempts = 0;

    if(status.isInputBufferFull)
    {
        uint8_t response;
        do
        {
            IO::port_byte_out(0x64, command);
            IO::port_byte_out(0x60, data);

            response = IO::port_word_in(0x60);
        } while ((response != 0xFA || response == 0xFE) && attempts < 10);
    }

    return attempts >= 10 ? false : true;
}

bool Keyboard::enableScanning()
{
    bool result = sendCommand(0xF4);
    scanningEnabled = result ? true : false;

    return result;
}

bool Keyboard::disableScanning()
{
    bool result = sendCommand(0xF5);
    scanningEnabled = result ? false : true;

    return result;
}

bool Keyboard::setLED(bool ScrollLock, bool NumLock, bool CapsLock)
{
    uint8_t data = 0;

    data = ScrollLock ? data | 1 : data;
    data = NumLock ? data | 2 : data;
    data = CapsLock ? data | 4 : data;

    return sendCommand(0xED, data);
}

Keyboard::KeyboardStatus Keyboard::getKeyboardStatus()
{
    KeyboardStatus result;
    uint8_t status = IO::port_word_in(0x64);

    for(int i = 0; i < 8; i++)
        *((uint8_t*)&result + i) = (status >> i) & 1;
    
    return result;
}

void Keyboard::registerKeyCallback(void (*func)(int state, int scan_code, int key_code))
{
    if(keyCallback == nullptr)
        keyCallback = func;
}

void Keyboard::handleKey()
{
    uint8_t scan_code = getScanCode();

    if(scan_code >= 0x60)
		scan_code = 0;
    
    if(keyStates[scan_code] == STATE_RELEASE || keyStates[scan_code] == 0)
        keyStates[scan_code] = STATE_PRESS;
    else if(keyStates[scan_code] == STATE_PRESS)
        keyStates[scan_code] = STATE_REPEAT;
    
    if(lastScanCode != -1 && lastScanCode != scan_code)
    {
        keyStates[lastScanCode] = STATE_RELEASE;

        if(keyCallback)
            keyCallback(STATE_RELEASE, lastScanCode, scan_code_US[lastScanCode]);
    }

    lastScanCode = scan_code;
    
    if(keyCallback && scan_code != 0)
        keyCallback(keyStates[scan_code], scan_code, scan_code_US[scan_code]);
}

uint8_t Keyboard::getScanCode()
{
    KeyboardStatus status = getKeyboardStatus();

    uint8_t scan_code = IO::port_byte_in(0x60);

    if(!status.isOutputBufferFull)
        return scan_code;
    else
        return 0;
}

void Keyboard::setKeys(int state, int scan_code, int key_code)
{
    if (state != STATE_RELEASE)
	{
		if (!keys[scan_code])
			keys[scan_code] = true;
	}
	else
		keys[scan_code] = false;

	if (state == STATE_PRESS)
		toggleKeys[scan_code] = !toggleKeys[scan_code];

	keysChanged[scan_code] = state != STATE_REPEAT;
}

bool Keyboard::key(int key)
{
    return keys[key];
}

bool Keyboard::keyChanged(int key)
{
    bool ret = keysChanged[key];
	keysChanged[key] = false;
	return ret;
}

bool Keyboard::keyWentUp(int key)
{
    return !keys[key] && keyChanged(key);
}

bool Keyboard::keyWentDown(int key)
{
    return keys[key] && keyChanged(key);
}

bool Keyboard::keyToggle(int key)
{
    return toggleKeys[key];
}

char Keyboard::scan_code_US[] =
{
    0, 27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',   
    '\t', /* <-- Tab */
    'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',     
    0, /* <-- control key */
    'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`',  0, '\\',
    'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/',   0,
    '*',
    0,  /* Alt */
    ' ',  /* Space bar */
    0,  /* Caps lock */
    0,  /* 59 - F1 key ... > */
    0,   0,   0,   0,   0,   0,   0,   0,
    0,  /* < ... F10 */
    0,  /* 69 - Num lock*/
    0,  /* Scroll Lock */
    0,  /* Home key */
    0,  /* Up Arrow */
    0,  /* Page Up */
    '-',
    0,  /* Left Arrow */
    0,
    0,  /* Right Arrow */
    '+',
    0,  /* 79 - End key*/
    0,  /* Down Arrow */
    0,  /* Page Down */
    0,  /* Insert Key */
    0,  /* Delete Key */
    0,   0,   0,
    0,  /* F11 Key */
    0,  /* F12 Key */
    0,  /* All other keys are undefined */
};