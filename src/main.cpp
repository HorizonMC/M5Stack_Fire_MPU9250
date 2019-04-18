
#include <M5Stack.h>
// #include "utility/MPU9250.h"
#include "sensor.h"
#include <Adafruit_NeoPixel.h>
#define M5STACK_FIRE_NEO_NUM_LEDS  10
#define M5STACK_FIRE_NEO_DATA_PIN  15
#define M5STACKFIRE_MICROPHONE_PIN 34
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(M5STACK_FIRE_NEO_NUM_LEDS, M5STACK_FIRE_NEO_DATA_PIN, NEO_GRB + NEO_KHZ800);
#define M5STACKFIRE_MICROPHONE_PIN 34
#define M5STACKFIRE_SPEAKER_PIN 25 // speaker DAC, only 8 Bit

#define HORIZONTAL_RESOLUTION 320
#define VERTICAL_RESOLUTION   240
#define POSITION_OFFSET_Y      20
#define SIGNAL_LENGTH HORIZONTAL_RESOLUTION

uint16_t oldSignal[SIGNAL_LENGTH];
uint16_t adcBuffer_ax[SIGNAL_LENGTH];
uint16_t adcBuffer_ay[SIGNAL_LENGTH];
uint16_t adcBuffer_az[SIGNAL_LENGTH];

#define SAMPLING_TIME_US           10



void setup()
{
  M5.begin();
  pixels.begin();
  Wire.begin();
  pixels.clear();
  pixels.show();
  Serial.begin(115200);

  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setTextColor(GREEN , BLACK);
  M5.Lcd.setTextSize(2);

  setupMPU9250();
}

void showSignal_ax()
{
  int n;

  int oldx;
  unsigned int oldy;
  unsigned int oldSig;
  unsigned int x, y;

  for (n = 0; n < SIGNAL_LENGTH; n++)
  {
    x = n;
    y = map(adcBuffer_ax[n], 0, 2300,VERTICAL_RESOLUTION ,POSITION_OFFSET_Y );

    if (n > 0)
    {
      // delete old line element
      M5.Lcd.drawLine(oldx , oldSig, x, oldSignal[n], BLACK );

      // draw new line element
      if (n < SIGNAL_LENGTH - 1) // don't draw last element because it would generate artifacts
      {
        M5.Lcd.drawLine(oldx,    oldy, x,            y, GREEN );
      }
    }
    // M5.Lcd.setCursor(0, 200); M5.Lcd.print(y);
    oldx = x;
    oldy = y;
    oldSig = oldSignal[n];
    oldSignal[n] = y;
    // Serial.println(y);
  }
}

void loop()
{

  int ax;

  uint32_t nextTime_ax = 0;

  for (ax = 0; ax < SIGNAL_LENGTH; ax++)
  {
    adcBuffer_ax[ax] = ((int)(1000 * IMU.ax) + (int)1400);

    // wait for next sample
    while (micros() < nextTime_ax);
    nextTime_ax = micros() + SAMPLING_TIME_US;
  }
  showSignal_ax();

    int x=64+10;
    int y=128+20;
    int z=192+30;


    // M5.Lcd.fillScreen(BLACK);
    M5.Lcd.setTextColor(WHITE , BLACK);
    M5.Lcd.setTextSize(2);
    M5.Lcd.setCursor(0, 0); M5.Lcd.printf("Smart maintenance");
    M5.Lcd.setCursor(0, 32); M5.Lcd.printf("x");
    M5.Lcd.setCursor(x, 32); M5.Lcd.printf("y");
    M5.Lcd.setCursor(y, 32); M5.Lcd.printf("z");

    M5.Lcd.setTextColor(YELLOW , BLACK);
    M5.Lcd.setCursor(0, 30 * 2); M5.Lcd.print((int)(1000 * IMU.ax));
    M5.Lcd.setCursor(x, 30 * 2); M5.Lcd.print((int)(1000 * IMU.ay));
    M5.Lcd.setCursor(y, 30 * 2); M5.Lcd.print((int)(1000 * IMU.az));
    M5.Lcd.setCursor(z, 30 * 2); M5.Lcd.print("mg");

    M5.Lcd.setCursor(0, 40 * 2); M5.Lcd.print((int)(IMU.gx));
    M5.Lcd.setCursor(x, 40 * 2); M5.Lcd.print((int)(IMU.gy));
    M5.Lcd.setCursor(y, 40 * 2); M5.Lcd.print((int)(IMU.gz));
    M5.Lcd.setCursor(z, 40 * 2); M5.Lcd.print("o/s");

    M5.Lcd.setCursor(0, 50 * 2); M5.Lcd.print((int)(IMU.mx));
    M5.Lcd.setCursor(x, 50 * 2); M5.Lcd.print((int)(IMU.my));
    M5.Lcd.setCursor(y, 50 * 2); M5.Lcd.print((int)(IMU.mz));
    M5.Lcd.setCursor(z, 50 * 2); M5.Lcd.print("mG");


    delay(100);
  // }
readMPU9250();
// delay(50);
}
