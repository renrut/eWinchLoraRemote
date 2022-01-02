#include <Wire.h>
#include <SPI.h>
#include <RH_RF95.h>
#include "Controller/ControllerInput.h"
#include "Radio/RadioHandler.h"
#include "SimpleTimer.h"

//Pins for feather LoRa
#define RFM95_CS 8
#define RFM95_RST 4
#define RFM95_INT 3

// RX's freq!
#define RF95_FREQ 915.0

//Pins for the LED
#define RED_LED 6
#define BLUE_LED 9
#define GREEN_LED 10

//Pins for buttons
#define UPPER_BUTTON 17
#define LOWER_BUTTON 19
#define POTPIN A1

#define STRENGTH_FLASH_INTERVAL_MILLIS 5000
#define DEFAULT_FLASH_MILLIS 250
#define CONN_WAIT_TIME_MILLIS 1000
#define STRENGTH_THRESHHOLD 150

SimpleTimer t;

RadioHandler* radioHandler;
int currentStrength;
int strengthTimer;

//LED Handler Functions.
void turnOffLeds(){
  digitalWrite(RED_LED, LOW);
  digitalWrite(BLUE_LED, LOW);
  digitalWrite(GREEN_LED, LOW);
}

//Non-Blocking led flash.
void flashLed(int ledPin, int delayTime) {
  digitalWrite(ledPin, HIGH);
  t.setTimeout(delayTime, turnOffLeds);
}

void flashLed(int ledPin) {
  flashLed(ledPin, DEFAULT_FLASH_MILLIS);
}

void solidLed(int ledPin) {
  digitalWrite(ledPin, HIGH);
}

void initLeds(){
  pinMode(RED_LED, OUTPUT);
  pinMode(BLUE_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  turnOffLeds();
}

void flashCurrentStrength(){
  if(currentStrength < 80){
    flashLed(GREEN_LED);
  } else if(currentStrength < 140) {
    flashLed(BLUE_LED);
  } else {
    flashLed(RED_LED);
  }
}

//Initialize the nunchuck buttons.
void initButtons(){
  // initialize the pushbutton pin as an input:
  Serial.println("Initializing Buttons");
  pinMode(UPPER_BUTTON, INPUT_PULLUP);
  pinMode(LOWER_BUTTON, INPUT_PULLUP);
}

// Get the controller values to send on to the reciever
ControllerInput* getInput(){
  ControllerInput* inputPtr = new ControllerInput();
  
  int upperButtonState = digitalRead(UPPER_BUTTON);
  int lowerButtonState = digitalRead(LOWER_BUTTON);

  inputPtr->setPotValue(analogRead(POTPIN));
  //For whatever reason the nunchuck buttons read low...
  if (upperButtonState == LOW) {
    inputPtr->setUpperButton(true);
  } 
  if (lowerButtonState == LOW) {
    inputPtr->setLowerButton(true);
  }

  return inputPtr;
}

void waitForConnection(){
  currentStrength = radioHandler->getSignalStrength();
  while (currentStrength > STRENGTH_THRESHHOLD)
  {
    currentStrength = radioHandler->getSignalStrength();
    flashLed(RED_LED);
    delay(CONN_WAIT_TIME_MILLIS);
  }
  flashLed(GREEN_LED);
}

// initialize
void setup() {
  Serial.begin(9600);
  initLeds();
  solidLed(RED_LED);
  radioHandler = new RadioHandler(RFM95_RST, RFM95_CS, RFM95_INT, RF95_FREQ);
  initButtons();
  //Set current strength.
  currentStrength = STRENGTH_THRESHHOLD + 50;
  waitForConnection();
  //Set interval for it to show it's strength
  strengthTimer = t.setInterval(STRENGTH_FLASH_INTERVAL_MILLIS, flashCurrentStrength);
}

void loop()
{
  //Run our timer
  t.run();
  if(currentStrength > STRENGTH_THRESHHOLD) {
    Serial.println("Handshaking...");
    waitForConnection();
  }
  ControllerInput* input = getInput();
  // Serial.println("Got " + input->toString());
  char* radioInput = input->getAsRadioPacket();
  //Design decision to be made -> keep transmitting if 
  //current strength gets low or just try to send handshake?
  currentStrength = radioHandler->sendInput(radioInput);

  delete input;
}

