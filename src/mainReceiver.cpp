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

//PWM uS values
#define MID_POINT 1400
#define DEADZONE 500
#define DEADZONE_BUFFER 75
#define AUX_ON 2000
#define AUX_OFF 1000
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

//Aux Button
boolean auxDepressed;

//FAILSAFE Method in case radio loses connection. Shut off motor and 
void sendFailsafePWM(){
  // auxDepressed = false;
  throttle.writeMicroseconds(MID_POINT);
  // aux.writeMicroseconds(AUX_OFF);
}

//Check how long its been since the last valid packet.
void checkSendFailsafePWM(){
  unsigned long timeSinceLastPacket = millis() - timeOfLastPacket;
  if(timeSinceLastPacket > TIMEOUT) {
    Serial.println("Timeout:Sending failsafe.");
    sendFailsafePWM();
  }
}

// Map the controller pot value so the controller maps 500+/- 75 to 1400.
int mapPotValue(int potVal){
  //Define controller deadzone
  if (potVal <= DEADZONE + DEADZONE_BUFFER && potVal >= DEADZONE - DEADZONE_BUFFER) {
    return MID_POINT;
  }
  // Crawl Mode
  if (potVal < DEADZONE - DEADZONE_BUFFER) {
    return map(potVal, 0, DEADZONE + DEADZONE_BUFFER, 1000, 1400);
  }
  // Tow Mode
  if (potVal > DEADZONE + DEADZONE_BUFFER) {
    return map(potVal, DEADZONE + DEADZONE_BUFFER, 900, 1400, 2000);
  }
  return MID_POINT;
}


//Send the autostop cuttoff on aux based on if the lower button is depressed.
void sendCurrentAuxVal(){
  if(auxDepressed) {
    aux.writeMicroseconds(AUX_ON);
  } else {
    aux.writeMicroseconds(AUX_OFF);
  }
}

// Send the PWM for the motor.
void sendControllerPWM(ControllerInput* input){
  //Check for aux button change (lower button)
  if(input->getLowerButton() != auxDepressed){
    auxDepressed = input->getLowerButton();
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
    int val = mapPotValue(input->getPotValueFlipped());
    Serial.println("Throttle: " + (String) val + " Aux Button: " + (String) auxDepressed);
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
  aux.attach(AUX_SWITCH_PIN);
  throttle.attach(THROTTLE_PIN);
  auxDepressed = false;
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
  sendCurrentAuxVal();
  handleInput();
  //Check that we are still connected.
  checkSendFailsafePWM();
}

