#include <Adafruit_NeoPixel.h>
#include <Servo.h>
#ifdef __AVR__
#include <avr/power.h>
#endif

#define PIN 6
#define NUM_LEDS 27

Servo myservo;
int brightness = 200;
const int brightPin = A0;
const int daynightPin = A1;

const int buttonON = HIGH;    // ボタンが押されているとピンの値はHIGH
const int buttonOFF = LOW;  // ボタンが押されていないとピンの値はLOW

const int buttonPin = 2;
int buttonState = 0;
int angle = 1;

int lastState = LOW;

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, PIN, NEO_GRBW + NEO_KHZ800);

byte neopix_gamma[] = {
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,
  1,  1,  1,  1,  1,  1,  1,  1,  1,  2,  2,  2,  2,  2,  2,  2,
  2,  3,  3,  3,  3,  3,  3,  3,  4,  4,  4,  4,  4,  5,  5,  5,
  5,  6,  6,  6,  6,  7,  7,  7,  7,  8,  8,  8,  9,  9,  9, 10,
  10, 10, 11, 11, 11, 12, 12, 13, 13, 13, 14, 14, 15, 15, 16, 16,
  17, 17, 18, 18, 19, 19, 20, 20, 21, 21, 22, 22, 23, 24, 24, 25,
  25, 26, 27, 27, 28, 29, 29, 30, 31, 32, 32, 33, 34, 35, 35, 36,
  37, 38, 39, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 50,
  51, 52, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 66, 67, 68,
  69, 70, 72, 73, 74, 75, 77, 78, 79, 81, 82, 83, 85, 86, 87, 89,
  90, 92, 93, 95, 96, 98, 99, 101, 102, 104, 105, 107, 109, 110, 112, 114,
  115, 117, 119, 120, 122, 124, 126, 127, 129, 131, 133, 135, 137, 138, 140, 142,
  144, 146, 148, 150, 152, 154, 156, 158, 160, 162, 164, 167, 169, 171, 173, 175,
  177, 180, 182, 184, 186, 189, 191, 193, 196, 198, 200, 203, 205, 208, 210, 213,
  215, 218, 220, 223, 225, 228, 231, 233, 236, 239, 241, 244, 247, 249, 252, 255
};


void setup() {
  pinMode(buttonPin, INPUT_PULLUP);
  //myservo.attach(9);
  angle = 1;

  strip.setBrightness(brightness);
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  Serial.begin(9600);
}

void loop() {
  buttonState = digitalRead(buttonPin);
  if (buttonState == buttonON) {     // ボタンが押されていたら
    //if (lastState != buttonState) {
      byte tmp = 1;
      Serial.write(tmp);
    //}
    LightEmitting();
  } else {
    //if (lastState != buttonState) {
      byte tmp = 0;
      Serial.write(tmp);
    //}
    fullOff();
  }
  serialServo();
  delay(30);
  lastState = buttonState;
}

void LightEmitting() {
  int b = getBright();
  strip.setBrightness(b);

  int c = getDayNight();

  for (uint16_t i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, strip.Color(c, c * 0.3, 0, 255 - c) );
  }
  strip.show();
}

void fullWhite() {

  for (uint16_t i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, strip.Color(0, 0, 0, 255 ) );
  }
  strip.show();
}
void fullOff() {

  for (uint16_t i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, strip.Color(0, 0, 0, 0 ) );
  }
  strip.show();
}

int getBright() {
  int sensorValue = 0;
  int outputValue = 0;

  sensorValue = analogRead(brightPin);
  outputValue = map(sensorValue, 0, 1023, 0, 255);

  outputValue = 255 - outputValue;
  return outputValue;
}

int getDayNight() {
  int sensorValue = 0;
  int outputValue = 0;

  sensorValue = analogRead(daynightPin);
  outputValue = map(sensorValue, 0, 1023, 0, 255);

  return outputValue;
}

void sendRotation(int rotAngle){
  myservo.attach(9);
  delay(30);
  myservo.write(rotAngle);
  delay(2000);
  myservo.detach();
}

void serialServo() {
  if (Serial.available() > 0) {
    int inByte = Serial.read();
    if (inByte == 0) {
      angle = 10;
    } else if (inByte == 1) {
      angle = 28;
    } else if (inByte == 2) {
      angle = 47;
    } else if (inByte == 3) {
      angle = 70;
    }
    sendRotation(angle);
  }
}
