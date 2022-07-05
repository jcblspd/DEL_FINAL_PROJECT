#ifndef _TempButtons_h
#define _TempButtons_h
#include "InputDevice.h"
#include "Arduino.h"
class TempButtons : public InputDevice{
public:
    TempButtons(int b, int _id) : InputDevice(b, _id){};
    void process(bool gameOn);
    void pressHandler(void (*f)(int));
    //void releaseHandler(void (*f)());
    void preGamePress(void (*f)());
    void (*pressCallback)(int);
    //void (*releaseCallback)();
    void (*preGameCallback)();

};
#endif
