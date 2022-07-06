#ifndef _Enemy_h
#define _Enemy_h
#include "GameObject.h"
#include "Arduino.h"
class Enemy : public GameObject{
  bool hit;
  bool valid = true;
  int distance;
  float m;
  int randDirection;
public:
    Enemy(int _x, int _y, int _type) : GameObject(_x, _y, _type){};
    void move(int xPlayer, int yPlayer, unsigned long t);
    void timeToSpeed(unsigned long time);
    void setHit(bool h);
    bool getHit();
    void setValid(bool b);
    bool getValid();
    void setDistance(int d);
    int getDistance(int xPlayer, int yPlayer);
    float recalcSlope(int xPlayer);
    void setM(float _m);
    float getM();
    void setDirection(int _r);
    int getDirection();
};
#endif
