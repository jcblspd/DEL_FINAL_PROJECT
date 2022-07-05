#include "Enemy.h"
#include "Arduino.h"
#include <cstdlib>
#include <cmath>
using namespace std;
void Enemy::move(int xPlayer, int yPlayer, unsigned long t){
    setXPrev(getX());
    setYPrev(getY());
    if (t % 2000 == 0 || t < 2000){
      setM(recalcSlope(xPlayer));
    }
    float b = getY()-(getM()*getX());
    setY(getY()+5);
    xPlayer < getX() ? setX(getX()-getSpeed()) : setX(getX()+getSpeed());
    //setY((int)getM()*getX()+b); //THIS LINE IS THE PROBLEM
    
    
    
}
float Enemy::recalcSlope(int xPlayer){
  return ((getY()-288) / (getX() - xPlayer));
}
void Enemy::setM(float _m){
  m = _m;
}
float Enemy::getM(){
  return m;
}
void Enemy::timeToSpeed(unsigned long time){
    int s = time / 10000;
    if (s == 0 || s == 1){
        setSpeedMovement(3);
    }else{
        setSpeedMovement(3*s);
    }
}
void Enemy::setHit(bool h){
  hit = h;
}
bool Enemy::getHit(){
  return hit;
}
bool Enemy::getValid(){
  return valid;
}
void Enemy::setValid(bool b){
  valid = b;
}
void Enemy::setDistance(int d){
  distance = d;
}
int Enemy::getDistance(int xPlayer, int yPlayer){
  int a = abs(getX() - xPlayer);
  int b = getY() - yPlayer;
  int c = (int)sqrt(pow(a, 2)+pow(b, 2));
  return c;
}
