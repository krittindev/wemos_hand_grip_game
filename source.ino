#include <Adafruit_GFX.h>    //!! Core graphics library
#include <Adafruit_ST7735.h> //!! Hardware-specific library for ST7735
#include <Adafruit_ST7789.h> //!! Hardware-specific library for ST7789
#include <SPI.h>

//!! LCD pin
#define TFT_CS D4
#define TFT_RST D3                                            
#define TFT_DC D2
#define TFT_MOSI D1
#define TFT_SCLK RX

//!! Flex pin
#define FLX A0

//!! Button pin
#define BTN_1 D0
#define BTN_2 D5
#define BTN_3 D6
#define BTN_4 D7

//!! Buzzer pin
#define BZZ D8

//!! For ST7735-based displays, we will use this call
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);

int flx;
int btn1;
int btn2;
int btn3;
int btn4;
int btn1_old = 1;
int buzzerState = 0;

//int oldPage = -1;
int currentPage = 0;
int score = 0;
int roundCount = 0;
int stateBtnPress = LOW;
int randNum = 0;

int timePerLevel = 10;
int scorePerTime = 3;

//home 0
//level_1 1
//level_2 2
//level_3 3
//score 4

void setup(void) {
  Serial.begin(9600);

  //!! init TFT LCD
  tft.initR(INITR_BLACKTAB);
  tft.fillScreen(ST77XX_BLACK);
  // tftPrintAllNum();
  // tftPrintSingleNum(1);
  // tftPrintSingleNum(2);
  // tftPrintSingleNum(3);
  // tftPrintSingleNum(4);

  //!! init Flex Sensor
  pinMode(FLX, INPUT);

  //!! init Button
  pinMode(BTN_1, INPUT);
  pinMode(BTN_2, INPUT_PULLUP);
  pinMode(BTN_3, INPUT_PULLUP);
  pinMode(BTN_4, INPUT_PULLUP);
  
  tftPrintHome();
}

void loop() {
  flx = analogRead(FLX);
  btn1 = digitalRead(BTN_1);
  btn2 = digitalRead(BTN_2);
  btn3 = digitalRead(BTN_3);
  btn4 = digitalRead(BTN_4);
  delay(100);
  play();
  Serial.printf("btn1: %d, btn2: %d, btn3: %d, btn4: %d, flx: %d \n", btn1, btn2, btn3, btn4, flx);
}

void play(){

  // page check
  switch(currentPage){
    case 0: // Home
      if(btn1 == stateBtnPress)
          currentPage = 1;
      else if(btn2 == stateBtnPress)
          currentPage = 2;
      else if(btn3 == stateBtnPress)
          currentPage = 3;
      score = 0;
      roundCount = 0;
      break;
    case 4: // Score
      tftPrintScore();
      buttonWait(1);
      tftPrintHome();
      currentPage = 0;
      break;
    case 1: // Level 1
    case 2: // Level 2
    case 3: // Level 3
      for(int i = 0; i < timePerLevel; i++){
        randNum = random(1, 5);
        tftPrintSingleNum(randNum);
        for(int j = 0; j < 1 + 3 - currentPage; j++){
          flx = analogRead(FLX);
          btn1 = digitalRead(BTN_1);
          btn2 = digitalRead(BTN_2);
          btn3 = digitalRead(BTN_3);
          btn4 = digitalRead(BTN_4);
          Serial.printf("%d%d%d%d %d page: %d round: %d score: %d \n", btn1, btn2, btn3, btn4, flx, currentPage, roundCount, score);
          delay(900);
          tone(BZZ, 100);
          delay(100);
          noTone(BZZ);
        }
        buttonWait(randNum);
      }
      currentPage = 4;
      break;
  }

//  oldPage = currentPage;
}

void buttonWait(int num){
  switch(num){
    case 1:
    case 4:
      while(1){
        flx = analogRead(FLX);
        btn1 = digitalRead(BTN_1);
        btn2 = digitalRead(BTN_2);
        btn3 = digitalRead(BTN_3);
        btn4 = digitalRead(BTN_4);
        if ((num == 1 && btn1 == stateBtnPress) || (num == 4 && btn4 == stateBtnPress)){
          score += 3;
          roundCount++;
          tone(BZZ, 100);
          delay(100);
          noTone(BZZ);
          return;
        }
      }
      break;
    case 2:
    case 3:
      while(1){
        flx = analogRead(FLX);
        btn1 = digitalRead(BTN_1);
        btn2 = digitalRead(BTN_2);
        btn3 = digitalRead(BTN_3);
        btn4 = digitalRead(BTN_4);
        if ((num == 2 && btn2 == stateBtnPress) || (num == 3 && btn3 == stateBtnPress)){
          score += (flx - 380) * scorePerTime / 15 > scorePerTime? scorePerTime: (flx - 380) * scorePerTime / 15;
          roundCount++;
          tone(BZZ, 100);
          delay(100);
          noTone(BZZ);
          return;
        }
      }
      break;
  }
}

void tftPrintScore() {
  tft.fillScreen(ST77XX_BLACK);
  tft.setCursor(0, 20);
  tft.setTextColor(ST77XX_GREEN);
  tft.setTextSize(2);
  tft.print("Score");
  tft.setCursor(0, 60);
  tft.setTextColor(ST77XX_WHITE);
  tft.printf("%d/%d", score, timePerLevel*scorePerTime);
  tft.setTextSize(1);
  tft.setCursor(0, 100);
  tft.print("Press 1 To Exit");
}

void tftPrintLevel() {
  tft.fillScreen(ST77XX_BLACK);
  tft.setCursor(0, 20);
  tft.setTextColor(ST77XX_GREEN);
  tft.setTextSize(1);
  tft.printf("Level %d", currentPage);
  tft.setCursor(0, 40);
  tft.setTextColor(ST77XX_WHITE);
  tft.printf("Round %d/10", roundCount);
  tft.setCursor(0, 60);
  tft.setTextColor(ST77XX_WHITE);
  tft.printf("Score %d/30", score);
}

void tftPrintHome() {
  tft.fillScreen(ST77XX_BLACK);
  tft.setCursor(0, 20);
  tft.setTextColor(ST77XX_GREEN);
  tft.setTextSize(1);
  tft.print("Please Select");
  tft.setCursor(0, 40);
  tft.setTextColor(ST77XX_WHITE);
  tft.print("Level 1 (Easy)");
  tft.setCursor(0, 60);
  tft.setTextColor(ST77XX_WHITE);
  tft.print("Level 2 (Medium)");
  tft.setCursor(0, 80);
  tft.setTextColor(ST77XX_WHITE);
  tft.print("Level 3 (Hard)");
}

void tftPrintSingleNum(int number) {  
  tft.fillScreen(ST77XX_BLACK);
  tft.setCursor(30, 50);
  tft.setTextColor(ST77XX_WHITE);
  tft.setTextSize(10);
  tft.print(number);
}

void tftPrintAllNum() {
  tft.fillScreen(ST77XX_BLACK);
  tft.setCursor(0, 70);
  tft.setTextColor(ST77XX_WHITE);
  tft.setTextSize(3);
  tft.print("1 2 3 4");
}

void tftPrintSmallNum(int number) {
  tft.fillScreen(ST77XX_BLACK);
  tft.setCursor(0, 70);
  tft.setTextColor(ST77XX_WHITE);
  tft.setTextSize(3);
  tft.print(number);
}

void tftPrintBool(bool bl) {
  tft.fillScreen(ST77XX_BLACK);
  tft.setCursor(0, 70);
  tft.setTextColor(ST77XX_WHITE);
  tft.setTextSize(3);
  tft.print(bl);
}
