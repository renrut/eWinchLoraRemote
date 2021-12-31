#include <Wire.h>
#include "Controller/ControllerInput.h"
#include "Radio/RadioHandler.h"
#include <Servo.h> 



// OLED FeatherWing buttons map to different pins depending on board:
#define VBATPIN A0
#define POTPIN A1

//Motor Control
#define THROTTLE_PIN 5
#define AUX_SWITCH_PIN 6
#define MID_POINT 1500
#define TIMEOUT 500

Servo throttle;
Servo aux;
unsigned long timeOfLastPacket;


//Pins for feather LoRa
#define RFM95_CS 8
#define RFM95_RST 4
#define RFM95_INT 3

// RX's freq!
#define RF95_FREQ 915.0

//Radio
RadioHandler* radioHandler;


void sendFailsafePWM(){
  //FAILSAFE
  throttle.writeMicroseconds(MID_POINT);
}

//Check how long its been since the last valid packet.
void checkSendFailsafePWM(){
  unsigned long timeSinceLastPacket = millis() - timeOfLastPacket;
  if(timeSinceLastPacket > TIMEOUT) {
    Serial.println("Timeout:Sending failsafe.");
    sendFailsafePWM();
  }
}

void sendControllerPWM(ControllerInput* input){
  //Check for aux button (lower button)
  if(input->getLowerButton()){
    aux.writeMicroseconds(2000);
  } else {
    aux.writeMicroseconds(1000);
  }

  //Check for the upper button. This will tell to send a signal.
  if(input->getUpperButton()){
    
    /**
     * 
     * In:
     * Mid:500
     * Up: 1000
     * Low: 0
     * 
     * Out -
     * Mid 1500
     * Up: 2000
     * Low: 1000
     */
    int val = map(input->getPotValueFlipped(), 0, 1000, 1000, 2000);
    Serial.println("Sending value: " + (String) val);
    throttle.writeMicroseconds(val);
  } else {
    //FAILSAFE
    sendFailsafePWM();
  }
}


// initialize radio and PWM outputs
void setup() {
  Serial.begin(9600);
  radioHandler = new RadioHandler(RFM95_RST, RFM95_CS, RFM95_INT, RF95_FREQ);
  throttle.attach(THROTTLE_PIN);
  aux.attach(AUX_SWITCH_PIN);
}

void handleInput() {
  //check for a message
  char* charInput = radioHandler->checkForMessage();
  //message will be nullptr unless it's valid meaning the correct length and ends with RadioHandler::END_OF_DATA.
  if(charInput != nullptr){
    //check type of message
    //Handshake type. Just needs an ack back.
    if (RadioHandler::HANDSHAKE == charInput[0]) 
    {
        radioHandler->sendAck(charInput);
    }
    //data type, should deserialize into a ControllerInput object.
    if(RadioHandler::INPUT_DATA == charInput[0]){
      radioHandler->sendAck(charInput);
      ControllerInput* input = ControllerInput::fromCharArray(&(charInput[1]));
      if(input != nullptr){
        sendControllerPWM(input);
        delete input;
      }
    }
    timeOfLastPacket=millis();
    delete charInput;
  }
}

//Check for input and check for lack of input.
void loop()
{
  handleInput();
  //Check that we are still connected.
  checkSendFailsafePWM();
}

