#include <SPI.h>
#include <TFT_ST7735.h>
#include <TimerOne.h>

#define	 GREEN   0x07E0
#define YELLOW  0x07FF

#define DC  9
#define RST 10
#define CS  11

#define pinA  A0
volatile long valA = 0;
volatile unsigned long Rev1 = 0;
const byte Sen1 = 2;
unsigned long prT = 0;

TFT_ST7735 tft = TFT_ST7735(CS, DC, RST);

void CountS(void);
void RPMc(void);
void Draw_Table(void);
void Print_Data(void);

void setup() {
  tft.begin();
  tft.setRotation(1);
  tft.clearScreen();
  tft.setTextWrap(true);
  tft.setTextColor(YELLOW, BLACK);
  tft.setCursor(0, 0);
  Draw_Table();
  
  pinMode(Sen1, INPUT);
  Timer1.initialize(240000);
  Timer1.attachInterrupt(RPMc);
  attachInterrupt(digitalPinToInterrupt(Sen1), CountS, RISING);
}

void loop() {
  Print_Data();
}

void CountS(){
    Rev1++;
}

void RPMc(){
    valA = Rev1;
    Rev1 = 0;
}

void  Draw_Table(){
  tft.drawFastVLine(22, 0, 128, WHITE);
  for ( int i=0; i<11; i+=1 ){
    tft.drawFastHLine( 20, 5+i*12, 4, WHITE);
    if (!(i&1)){
      tft.setCursor( 0, i*12 + 2);
      tft.print((10.0-i)*5.0/10.0, 1);
      }
    }
  tft.drawFastHLine( 20, 125, 128, WHITE);
  tft.setCursor(55, 107);
  tft.setTextSize(2);
  tft.print("K-RPM");
}

void Print_Data(){
  unsigned long nwT = millis();
  if ((nwT - prT) > 100){
    prT = nwT;
    tft.setCursor(55, 88);
    //RPM
    tft.print(valA*25.00/1000,3);
    long valG = map(valA, 0, 200, 0, 120);
    tft.fillRect(30, 5, 20, 120 - valG, BLACK);
    tft.fillRect(30, 125 - valG, 20, valG, GREEN);
    }
}