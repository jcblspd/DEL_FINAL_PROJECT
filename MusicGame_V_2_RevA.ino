//Jacob LaSpada

//-----------------------------------------Setup--------------------------------------------------//

//  LIBRARIES   //
#include "USBHost_t36.h"
#include <Encoder.h>
#include "SPI.h"
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"

//  BITMAPS   //
#include "StarImage.h"
#include "SpaceShipImage.h"
#include "TextTitle.h"
#include "Enemy1Bitmap.h"

//  USER DEFINED CLASSES //
#include "InputDevice.h"
#include "Toggle.h"
#include "TempButtons.h"
#include "GameObject.h"
#include "Player.h"
#include "Enemy.h"
#include "Node.h"
#include "Scale.h"

//  DEFINITIONS //
#define TFT_DC      20
#define TFT_CS      21
#define TFT_RST    255  
#define TFT_MOSI    26
#define TFT_CLK    27
#define TFT_MISO    39

using namespace std;

//      Constants       //

const int MAX_WIDTH = 288;
const int MAX_HEIGHT = 208;
const int MIN_WIDTH = 32;
const int MIN_HEIGHT = 32;
const int MAX_ENEMIES = 10;

//          Global Vars             //

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_MOSI, TFT_CLK, TFT_RST, TFT_MISO);
USBHost midiKeyboard;
USBHost hub1(midiKeyboard);
USBHost hub2(midiKeyboard);
USBHost hub3(midiKeyboard);
MIDIDevice midi1(midiKeyboard);
Encoder enc(33, 34);
int oldPosition = 0;
int currKey = 0;
int currMode = 0;
Toggle* toggle = new Toggle(32, -1);
Node *c = new Node(48);
Node *cd = new Node(49);
Node *d = new Node(50);
Node *de = new Node(51);
Node *e = new Node(52);
Node *f = new Node(53);
Node *fg = new Node(54);
Node *g = new Node(55);
Node *ga = new Node(56);
Node *a = new Node(57);
Node *ab = new Node(58);
Node *b = new Node(59);
Scale *currScale = NULL;
Node *letters[12] = {c, cd, d, de, e, f, fg, g, ga, a, ab, b};
String arrLetters[12] = {"C", "Db", "D", "Eb", "E", "F", "Gb", "G", "Ab", "A", "Bb", "B"};
String modes[2]={"Major", "Minor"};
Node *curr = NULL;
int LEDArray[4] = {0, 1, 2, 3};
int arrSize = 12;
bool gameOn = false;
int menuStep = 0;
Player player1(0, 208, 'p');
Player* playerPtr = &player1;
unsigned long gameTimer = 0;
unsigned long differenceTime = 0;
int numEnemies = 0;
int numEnemiesKilled = 0;
Enemy* allEnemies[MAX_ENEMIES];
int secs = 0;
bool selection = false;


//      Function Prototypes         //
bool checkMove(int j);
void enemyEscape(int i);
void enemyDisposal(int i);
void displayKeyChoice();
void oneClickRight();
void oneClickLeft();
void onNoteOn(byte channel, byte note, byte velocity);
void onNoteOff(byte channel, byte note, byte velocity);
void onControlChange(byte channel, byte control, byte value);
void checkEnemyConditions();
void EndingSequence();
void DisplayScore();
void readInput();
void checkHitBoxes();
void interpretInput(int id);
void advanceMenu();
void onPress();
void onRelease();
void onSwitch();
void onPoweroff();
void onRelease();
void redrawObject(int xP, int yP);
void writeText(int x, int y, uint16_t color, int sizeText, String message);
void draw(int x, int y, const uint16_t *bitmap, int wd, int ht);

//----------------------SETUP-------------------//
void setup() {
    SPI.setMOSI(TFT_MOSI);
    SPI.setSCK(TFT_CLK);
    Serial.begin(9600);
    while(!Serial);
    midiKeyboard.begin();
    tft.begin();
    midi1.setHandleNoteOff(onNoteOff);
    midi1.setHandleNoteOn(onNoteOn);
    midi1.setHandleControlChange(onControlChange);
    tft.fillScreen(ILI9341_WHITE);
    for (int i = 0; i < 4; i++){
        pinMode(LEDArray[i], INPUT);
    }
    randomSeed(analogRead(0));
    toggle->pressHandler(onSwitch);
    toggle->releaseHandler(onPoweroff);
    Serial.println("GAME LOG");
}

//-------------------------------------------------------LOOP---------------------------------//
void loop() {
    digitalWrite(3, HIGH);
    readInput();
    if (gameOn){
        if (player1.getLives() != 0){
            gameTimer = millis() - differenceTime;
            if (gameTimer % 1000 == 0){
              //checkEnemyConditions();
              secs++;
              redrawObject(300, 0);
              writeText(300, 0, ILI9341_GREEN, 1, secs);
            }
           
            if ((numEnemies == 0 && gameTimer < 5000) || ((gameTimer > (5000*numEnemies)) && (numEnemies != 10))){
                Enemy* enemySpawn = new Enemy(random(0, 288), 0, 'e');
                //Serial.printf("%d::Enemy #%d Spawn at (%d, %d)\n", gameTimer, numEnemies+1, enemySpawn->getX(), enemySpawn->getY());
                draw(enemySpawn->getX(), enemySpawn->getY(), EnemyBitmap, 32, 32);
                enemySpawn->setDirection(random(0, 2));
                enemySpawn->setSpeedMovement(5);
                enemySpawn->setValid(true);
                int index = 0;
                for (int i = 0; i < 10; i++){
                  if (allEnemies[i] == NULL){
                    index = i;
                    break;
                  }else if(!allEnemies[i]->getValid()){
                    index = i;
                    break;
                  }
                }
                allEnemies[index] = enemySpawn;
                numEnemies++;
            }
            if (gameTimer % 250 == 0){
              redrawObject(0,0);
              writeText(0,0, ILI9341_RED, 1, player1.getLives());
              moveEnemies();
              checkHitboxes();
              
            }
        }
        else{
          EndingSequence();
        }
    }
}



//-----------------------------------------------------Enemy Functions------------------------------------------------------------//

void checkEnemyConditions(){
  for (int i = 0; i < 10; i++){
    if (allEnemies[i] != NULL){
      Serial.printf("Enemy #%d Valid %d (%d, %d)\n", i, allEnemies[i]->getValid(), allEnemies[i]->getX(), allEnemies[i]->getY());
    }else{
      Serial.printf("Enemy #%d is NULL\n", i);
    }
  }
  Serial.printf("Player: (%d, %d)\n", player1.getX(), player1.getY());
}

void checkHitboxes(){
  
  for (int i = 0; i < MAX_ENEMIES; i++){
    if (allEnemies[i] != NULL){
      if (allEnemies[i]->getY()+32 >= 216){
        if ( (player1.getX() < allEnemies[i]->getX() && player1.getX()+32 > allEnemies[i]->getX()) 
        || (player1.getX() > allEnemies[i]->getX() && allEnemies[i]->getX()+32 < player1.getX()+32))
        {
          enemyDisposal(i);
        }
        else{
          enemyEscape(i);
        }
      }
      else if(allEnemies[i]->getX()+32 <= 32)
      {
        enemyEscape(i);
      }
    }
  }
}

void enemyDisposal(int i){
  redrawObject(allEnemies[i]->getX(), allEnemies[i]->getY());
  draw(player1.getX(), player1.getY(), SpaceShipBitmap, 32, 32);
  Serial.println("ENEMY HIT");
  allEnemies[i]=NULL;
  numEnemies--;
  player1.setLives(player1.getLives()-1);
}

void enemyEscape(int i){
  Serial.println("ENEMY ESCAPE");
  Serial.printf("X: %d Y: %d\n", allEnemies[i]->getX(), allEnemies[i]->getY());
  numEnemies--;
  numEnemiesKilled++;
  redrawObject(allEnemies[i]->getX(), allEnemies[i]->getY());
  draw(player1.getX(), player1.getY(), SpaceShipBitmap, 32, 32);
  allEnemies[i] = NULL;
}

void moveEnemies(){
  for (int i = 0; i < MAX_ENEMIES; i++){
    if (allEnemies[i] != NULL){
      
      if (!(allEnemies[i]->getX() < 0 || allEnemies[i]->getX() > 288 || allEnemies[i]->getY() < 0 || allEnemies[i]->getY() > 208))
      {
        allEnemies[i]->move(player1.getX(), player1.getY(), gameTimer);
        if (!checkMove(i)){
          if (allEnemies[i]->getDirection() > 0){
            allEnemies[i]->setX(allEnemies[i]->getX()+10);
          }else{
            allEnemies[i]->setX(allEnemies[i]->getX()-10);
          }
        }
        redrawObject(allEnemies[i]->getXPrev(), allEnemies[i]->getYPrev());
        draw(allEnemies[i]->getX(), allEnemies[i]->getY(), EnemyBitmap, 32, 32);
      }
    }
  }
}

bool checkMove(int j){
  for (int i = 0; i < MAX_ENEMIES; i++){
    if (allEnemies[i] != NULL && i != j && allEnemies[i]->getValid()){
      if ( (allEnemies[j]->getX() >= allEnemies[i]->getX() && allEnemies[j]->getX() <= allEnemies[i]->getX()+32) 
      || (allEnemies[j]->getX()+32 >= allEnemies[i]->getX() && allEnemies[j]->getX()+32 <= allEnemies[i]->getX()+32) ){
        if ( allEnemies[j]->getY()+32 <= allEnemies[i]->getY()+32 && allEnemies[j]->getY() <= allEnemies[i]->getY()){
          return false;
        }
      }
    }
  }
  return true;
}
//-------------------------------------------------------INPUT METHODS----------------------------------------------------------//
void readInput(){
    toggle->process();
    if (selection){
      long newPosition = enc.read();
      if (newPosition - oldPosition >= 4) {
        oldPosition = newPosition; // update the encoder's position
        oneClickLeft();            
      }
      if (newPosition - oldPosition <= -4) {
          oldPosition = newPosition;
          oneClickRight();
      }  
    }
    midiKeyboard.Task();
    midi1.read();
}
void interpretInput(int note){  
  if (curr->next->data == note){
    curr = curr->next;
    player1.move(0);
  }else if (curr->prev->data == note){
   curr = curr->prev;
    player1.move(1);
  }
  redrawObject(player1.getXPrev(), player1.getYPrev());
  draw(player1.getX(), player1.getY(), SpaceShipBitmap, 32, 32);
}

//----------------------------------------------------MIDI FUNCTIONS---------------------------------------------//
void onNoteOn(byte channel, byte note, byte velocity){
  if (!gameOn){
    advanceMenu(note);
    menuStep++;
  }
  else{
    interpretInput(note);
    usbMIDI.sendNoteOn(note, velocity, channel);
  }
}
void onNoteOff(byte channel, byte note, byte velocity){
  usbMIDI.sendNoteOff(note, 0, channel);
}

void onControlChange(byte channel, byte control, byte value){}

 //----------------------------------------------------------Rotary Methods------------------------------------//
void oneClickLeft() {
  if (menuStep == 2){
      if (currKey-1 >= 0){
        currKey -= 1;
      }else{
        currKey =11;
      }
  }else{
    if (!currMode){currMode = 1;}
    else{currMode = 0;}
  }
  displayKeyChoice();
}

void oneClickRight() {
  if (menuStep == 2){
    if (currKey+1 <= 11){
        currKey += 1;
    }else{
      currKey = 0;
    }
  }else{
    if (!currMode){currMode = 1;}
    else{currMode = 0;}
  }
  displayKeyChoice();
}
//-----------------------------------------------------TOGGLE METHODS----------------------------------------------------//

void advanceMenu(int note){
    if (menuStep == 0){
        tft.fillScreen(ILI9341_BLACK);
        writeText(25, 25, ILI9341_WHITE, 1, "Play the correct sequence of keys to move");
        writeText(25, 35, ILI9341_WHITE, 1, "the space ship.");
        writeText(120, 35, ILI9341_WHITE, 1, "Avoid enemies for as long as");
        writeText(25, 45, ILI9341_WHITE, 1, "possible to gain the highest score");
        selection = true;
        delay(1000); //make player wait 1 sec
        
    }
    else if(menuStep == 1){
      tft.fillScreen(ILI9341_BLACK);
      writeText(25, 25, ILI9341_WHITE, 1, "Choose a Scale");
      writeText(25, 50, ILI9341_WHITE, 1, "Press any key to confirm your choices");
      displayKeyChoice();
    }
    else if (menuStep == 2){
      displayKeyChoice();
    }
    else{
        selection = false;
        currScale = new Scale(letters[currKey], currMode);
        curr = currScale->getStartingNote();
        currScale->SanityCheckNodes();
        tft.fillScreen(ILI9341_BLACK);
        for (int i = 3; i != 0; i--){
            writeText(50, 60, ILI9341_WHITE, 3, i);
            delay(1000);
            tft.fillRect(50, 60, 100, 100, ILI9341_BLACK);
        }
        draw(player1.getX(), player1.getY(), SpaceShipBitmap, SPACESHIP_WIDTH, SPACESHIP_HEIGHT);
        player1.setLives(3);
        gameOn = true;
        menuStep = 0;
        differenceTime = millis();
    }
}
void onSwitch(){ //Toggle on Press
    tft.setRotation(3);
    draw(0, 0, starImageBitmap, STARIMAGE_WIDTH, STARIMAGE_HEIGHT);
    draw(60, 50, textTitleBitmap, TEXTTITLE_WIDTH, TEXTTITLE_HEIGHT);
    writeText(100, 150, ILI9341_WHITE, 1, "Press any key to begin");
    
}
void onPoweroff(){ //Toggle on Off
    tft.fillScreen(ILI9341_BLACK);
    gameOn = false;
    for (int i = 0; i < MAX_ENEMIES; i++){
      allEnemies[i] = NULL;
    }
    player1.setLives(3);
    player1.setX(0);
    player1.setY(208);
    secs = 0;
    numEnemies = 0;
    numEnemiesKilled = 0;
    oldPosition = 0;
    currKey = 0;
    currMode = 0;
   
}

void onRelease(){}//Empty method

//-------------------------------------------DISPLAYING THINGS-------------------------------------------//
void displayKeyChoice(){
  tft.fillRect(100, 100, 300, 100, ILI9341_BLACK);
  writeText(100, 100, ILI9341_YELLOW, 2, arrLetters[currKey]);
  writeText(150, 100, ILI9341_RED, 2, modes[currMode]);
}
void displayScore(){
  int score  = (int)gameTimer * numEnemiesKilled;
  tft.fillScreen(ILI9341_BLACK);
  writeText(80, 50, ILI9341_GREEN, 3, score);
  delay(3000);
  draw(0, 0, starImageBitmap, STARIMAGE_WIDTH, STARIMAGE_HEIGHT);
  writeText(50, 120, ILI9341_WHITE, 2, "THANKS FOR PLAYING");
  delay(2000);
  gameOn = false;
}
void EndingSequence(){
  tft.fillScreen(ILI9341_BLACK);
  for (int i = 0; i < 3; i++){
    writeText(60, 50, ILI9341_GREEN, 3, "GAME OVER");
    delay(500);
    writeText(60, 50, ILI9341_BLACK, 3, "GAME OVER");
    delay(500);
  }
  displayScore();
}
void writeText(int x, int y, uint16_t color, int sizeText, String message){
  tft.setCursor(x, y);
  tft.setTextColor(color);
  tft.setTextSize(sizeText);
  tft.println(message);
}
void redrawObject(int xP, int yP){
    tft.fillRect(xP, yP, 32, 32, ILI9341_BLACK);
}
void draw(int x, int y, const uint16_t *bitmap, int wd, int ht){
    tft.drawRGBBitmap
    (x, y,
#if defined(__AVR__) || defined(ESP8266)
    bitmap,
#else
    (uint16_t*)bitmap,
#endif
    wd, ht);
}
