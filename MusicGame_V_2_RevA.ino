//Verison 2: Revision A
// 6.26.22 Jacob LaSpada
/*
 * Current Problems:
 * Ships draw over each other (in contact AND in creation)
 * Randomization should be improved  ^^
 * Instruction screen formatting
 * Implement newLifeTimer
 */


//           Setup                  //

//  LIBRARIES   //
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
Toggle* toggle = new Toggle(32, -1);
TempButtons* t1 = new TempButtons(28, 0);
TempButtons* t2 = new TempButtons(29, 1);
TempButtons* t3 = new TempButtons(30, 2);
TempButtons* t4 = new TempButtons(31, 3);
TempButtons* t5 = new TempButtons(41, 4);
TempButtons* t6 = new TempButtons(40, 5);
TempButtons* t7 = new TempButtons(33, 6);
TempButtons* t8 = new TempButtons(34, 7);
TempButtons* t9 = new TempButtons(35, 8);
TempButtons* t10 = new TempButtons(36, 9);
TempButtons* t11 = new TempButtons(37, 10);
TempButtons* t12 = new TempButtons(38, 11);

Node *n1 = new Node(0);
Node *n2 = new Node(1, n1);
Node *n3 = new Node(2, n2);
Node *n4 = new Node(3, n3);
Node *n5 = new Node(4, n4);
Node *n6 = new Node(5, n5);
Node *n7 = new Node(6, n6);
Node *n8 = new Node(7, n7);
Node *n9 = new Node(8, n8);
Node *n10 = new Node(9, n9);
Node *n11 = new Node(10, n10);
Node *n12 = new Node(11, n11);
    
    
Node *curr = n12;
int LEDArray[4] = {0, 1, 2, 3};
TempButtons* buttonArr[12] = {t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11, t12};
int arrSize = 12;
bool gameOn = false;
int menuStep = 0;
Player player1(0, 208, 'p');
Player* playerPtr = &player1;
int nextIndex = 0;
int directionFlag = 0; //0 forward (false), 1 backwards
unsigned long gameTimer = 0;
unsigned long newLifeTimer = 0;
unsigned long differenceTime = 0;
int numEnemies = 0;
int numEnemiesKilled = 0;
Enemy* allEnemies[MAX_ENEMIES];
//Enemy* validEnemies[MAX_ENEMIES];
//Enemy* invalidEnemies[MAX_ENEMIES];
int secs = 0;

//srand(static_cast<unsigned int> (time(0)));


//      Function Prototypes         //

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
void writeText(int x, int y, uint16_t color, int sizeText, String message);
void draw(int x, int y, const uint16_t *bitmap, int wd, int ht);

//----------------------SETUP-------------------//
void setup() {
    
    SPI.setMOSI(TFT_MOSI);
    SPI.setSCK(TFT_CLK);
    Serial.begin(9600);
    tft.begin();
    tft.fillScreen(ILI9341_WHITE);
    n1->next = n2;
    n2->next = n3;
    n3->next = n4;
    n4->next = n5;
    n5->next = n6;
    n6->next = n7;
    n7->next = n8;
    n8->next = n9;
    n9->next = n10;
    n10->next = n11;
    n11->next = n12;
    n12->next = n1;
    n1->prev = n12;
    for (int i = 0; i < 4; i++){
        pinMode(LEDArray[i], INPUT);
    }
    for (int i = 0; i < arrSize; i++){
        buttonArr[i]->pressHandler(interpretInput);
        buttonArr[i]->releaseHandler(onRelease);
        buttonArr[i]->preGamePress(advanceMenu);
    }
    
    toggle->pressHandler(onSwitch);
    toggle->releaseHandler(onPoweroff);
    Serial.println("GAME LOG");
}


void loop() {
    digitalWrite(3, HIGH);
    readInput();
    if (gameOn){
        if (player1.getLives() != 0){
            gameTimer = millis() - differenceTime;
            if (gameTimer % 1000 == 0){
              Serial.println("\n\n");
              checkEnemyConditions();
              secs++;
              redrawObject(300, 0);
              writeText(300, 0, ILI9341_GREEN, 1, secs);
            }
           
            if ((numEnemies < 3) && (gameTimer > (5000*numEnemies)) && (numEnemies != 10)){
                Enemy* enemySpawn = new Enemy(random(0, 288), 0, 'e');
                Serial.printf("%d::Enemy #%d Spawn at (%d, %d)\n", gameTimer, numEnemies+1, enemySpawn->getX(), enemySpawn->getY());
                draw(enemySpawn->getX(), enemySpawn->getY(), EnemyBitmap, 32, 32);
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


//-----------------USER FUNCTIONS DEFINITIONS--------------//

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

void checkHitboxes(){
  
  for (int i = 0; i < MAX_ENEMIES; i++){
    if (allEnemies[i] != NULL){
      if (allEnemies[i]->getY()+32 >= 216){
        if ( (player1.getX() < allEnemies[i]->getX() && player1.getX()+32 > allEnemies[i]->getX()) 
        || (player1.getX() > allEnemies[i]->getX() && allEnemies[i]->getX()+32 < player1.getX()+32))
        {
          redrawObject(allEnemies[i]->getX(), allEnemies[i]->getY());
          draw(player1.getX(), player1.getY(), SpaceShipBitmap, 32, 32);
          Serial.println("ENEMY HIT");
          allEnemies[i]=NULL;
          numEnemies--;
          player1.setLives(player1.getLives()-1);
        }
        else{
          Serial.println("ENEMY ESCAPE");
          Serial.printf("X: %d Y: %d\n", allEnemies[i]->getX(), allEnemies[i]->getY());
          numEnemies--;
          numEnemiesKilled++;
          redrawObject(allEnemies[i]->getX(), allEnemies[i]->getY());
          draw(player1.getX(), player1.getY(), SpaceShipBitmap, 32, 32);
          allEnemies[i] = NULL;
        }
      }
    }
  }
}
void moveEnemies(){
  for (int i = 0; i < MAX_ENEMIES; i++){
    if (allEnemies[i] != NULL){
      tft.drawPixel(allEnemies[i]->getX(), allEnemies[i]->getY(), ILI9341_YELLOW);
      if (!(allEnemies[i]->getX() < 0 || allEnemies[i]->getX() > 288 || allEnemies[i]->getY() < 0 || allEnemies[i]->getY() > 208))
      {
        allEnemies[i]->move(player1.getX(), player1.getY(), gameTimer);
        redrawObject(allEnemies[i]->getXPrev(), allEnemies[i]->getYPrev());
        draw(allEnemies[i]->getX(), allEnemies[i]->getY(), EnemyBitmap, 32, 32);
      }
    }
  }
}

void readInput(){
    toggle->process();
    for (int i = 0; i < arrSize; i++){
       buttonArr[i]->process(gameOn);
    }
}
void interpretInput(int id){
  if (curr->next->data == id){
    curr = curr->next;
    player1.move(0);
  }else if (curr->prev->data == id){
   curr = curr->prev;
    player1.move(1);
  }
  redrawObject(player1.getXPrev(), player1.getYPrev());
  draw(player1.getX(), player1.getY(), SpaceShipBitmap, 32, 32);
  
}


void advanceMenu(){
    //This code isn't pretty and I don't like it but it works for now
    //Maybe enum the opts???
    if (menuStep ==0){
        menuStep++;
        tft.fillScreen(ILI9341_BLACK);
        writeText(25, 25, ILI9341_WHITE, 1, "Play the correct sequence of keys to move the space ship");
        writeText(25, 50, ILI9341_WHITE, 1, "The direction of the sequence determines the direction of the ship");
        delay(1000); //make player wait 1 sec
    }else{
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
    curr = n12;
    numEnemies = 0;
    numEnemiesKilled = 0;
   
}
void onRelease(){ //Empty method
}

//      DISPLAYING THINGS //
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
