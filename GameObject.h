#ifndef _GameObject_h
#define _GameObject_h
#include "Arduino.h"
class GameObject{
    int x;
    int y;
    char type;
    int xPrev;
    int yPrev;
    int speed;
    GameObject(){};
protected:
    GameObject(int _x, int _y, char _type);
public:
    int getX();
    void setX(int _x);
    int getY();
    void setY(int _y);
    int getXPrev();
    void setXPrev(int _xP);
    int getYPrev();
    void setYPrev(int _yP);
    int getSpeed();
    void setSpeedMovement(int s);
    char getType();
    void setType(char c);
    virtual void move(int direction);
};
#endif
