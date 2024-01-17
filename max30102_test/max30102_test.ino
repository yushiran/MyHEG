#include <Wire.h>
#include "src/MAX30105.h"

MAX30105 Sensor1;
MAX30105 Sensor2;
#define SDA_1 21
#define SCL_1 22
#define SDA_2 4
#define SCL_2 5
TwoWire I2C_1 = TwoWire(0);
TwoWire I2C_2 = TwoWire(1);
#define debug Serial

void setup()
{
  delay(2000);
  debug.begin(115200);
  debug.println("MAX30105 Basic Readings Example");
  // Initialize sensor
  I2C_1.begin(SDA_1, SCL_1);
  I2C_2.begin(SDA_2, SCL_2); 
  Sensor1.begin(I2C_1);
  Sensor2.begin(I2C_2);


  uint8_t LEDpwr = 255;
  uint8_t sAvg = 8;
  uint16_t sRate = 3200;
  uint8_t ledMode = 2;
  uint8_t pulseWidth = 69;
  uint16_t adcRange = 16384;

  // Sensor1.setup(LEDpwr, 1, ledMode, 800, 118, adcRange);
  // Sensor2.setup(0x00, sAvg, ledMode, sRate, pulseWidth, adcRange);

  Sensor1.setup(); //Configure sensor. Use 6.4mA for LED drive
  Sensor2.setup();
}

void loop()
{
  // Serial.print(Sensor1.getRed());
  // Serial.print(", ");
  // Serial.print(Sensor1.getIR());
  // Serial.print(", ");
  Serial.print(Sensor2.getRed());
  Serial.print(", ");
  Serial.print(Sensor2.getIR());
  Serial.println();

}
