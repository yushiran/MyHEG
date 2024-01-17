#include "src/MAX30105.h"
#include <Wire.h>

MAX30105 PPG1;
MAX30105 PPG2;

#define SDA 21
#define SCL 22

#define SDA1 4
#define SCL1 5

long long currentMillis = 0;

int maxR2 = 0;
int maxIR2 = 0;
byte redct = 0;
byte irct = 0;
int smpct = 0;

byte irctmax = 32; //number of samples before the IR oscillates back to zero (due to desynchronized MAX30102 sensors with one taking ambient measurements trying to catch the LEDs from the other MAX30102)

void setup() {
  delay(2000);
  
  Wire.setPins(SDA,SCL);
  Wire1.setPins(SDA1,SCL1);

  Serial.begin(115200);
  Serial.println("Sensors starting");
  
  bool p1 = PPG1.begin(Wire,  I2C_SPEED_FAST);
  bool p2 = PPG2.begin(Wire1, I2C_SPEED_FAST);

  if(!p1) Serial.println("Pulse Ox 1 not found");
  if(!p2) Serial.println("Pulse Ox 2 not found");

  uint8_t LEDpwr = 255;
  uint8_t sAvg = 8;
  uint16_t sRate = 1600;
  uint8_t ledMode = 2;
  uint8_t pulseWidth = 69;
  uint16_t adcRange = 16384;

  PPG1.softReset();
  PPG2.softReset();

  PPG1.shutDown();
  PPG2.shutDown();

  //user the same settings except PPG1 flashes the LEDs
  PPG1.setup(LEDpwr, 1, ledMode, 800, 118, adcRange);

  //delayMicros(100) ///??? How to sync devices
  PPG2.setup(0x00, sAvg, ledMode, sRate, pulseWidth, adcRange);

  PPG1.wakeUp();
  PPG2.wakeUp();
}

int R2_0 = 0;
int R2_1 = 0;
int R2_2 = 0;
int R2_3 = 0;
int R2_4 = 0;


int IR2_0 = 0;
int IR2_1 = 0;
int IR2_2 = 0;
int IR2_3 = 0;
int IR2_4 = 0;

//bool cycled = false;

void readPulseOx() {
  PPG1.check();
  PPG2.check();

  while(PPG2.available()) {

  //  int R1 = PPG1.getFIFORed();
   int R2 = PPG2.getFIFORed();
  //  int IR1 = PPG1.getFIFOIR();
   int IR2 = PPG2.getFIFOIR();


    R2_4 = R2_3;
    R2_3 = R2_2;
    R2_2 = R2_1;
    R2_1 = R2_0;
    R2_0 = R2;

    IR2_4 = IR2_3;
    IR2_3 = IR2_2;
    IR2_2 = IR2_1;
    IR2_1 = IR2_0;
    IR2_0 = IR2;
      
    if(R2_2 > IR2_2) {
      if(maxR2 < R2_2 && R2_2 > R2_0 && R2_2 > R2_1 && R2_2 > R2_3 && R2_2 > R2_4) maxR2 = R2_2;
    }
    if(IR2_2 > R2_2) {
      if(maxIR2 < IR2_2 && IR2_2 > IR2_0 && IR2_2 > IR2_1 && IR2_2 > IR2_3 && IR2_2 > IR2_4) maxIR2 = IR2_2;
    }

    // if( R2_4 > 0 && IR2_4 > 0 && (IR2 > R2 && IR2_4 < R2_4) && maxR2 > 0 && maxIR2 > 0) { //switching
    //   //the lower of the two is the Red LED pulse
    //   if(maxR2 > maxIR2) {
    //     int temp = maxR2;
    //     maxR2 = maxIR2;
    //     maxIR2 = temp;  
    //   }
      
      char outputArr[200];
      sprintf(outputArr,"%d,%d",
        R2, IR2
      );
      Serial.println(outputArr);
        
      R2_4 = 0;
      R2_3 = 0;
      R2_2 = 0;
      R2_1 = 0;
      R2_0 = 0;

      IR2_4 = 0;
      IR2_3 = 0;
      IR2_2 = 0;
      IR2_1 = 0;
      IR2_0 = 0;
        
      maxR2 = 0;
      maxIR2 = 0;
    // }

    PPG1.nextSample();
    PPG2.nextSample();
  }
}

void loop() {  
  delay(50);
  readPulseOx();
}
