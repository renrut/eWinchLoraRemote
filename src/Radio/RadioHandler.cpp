#include "RadioHandler.h"


RadioHandler::RadioHandler(){
  new RadioHandler(RFM95_RST, RFM95_CS, RFM95_INT, RF95_FREQ);
}

RadioHandler::RadioHandler(int resetPin, int csPin, int intPin, float frequency) {
  RadioHandler::resetPin = resetPin;
  RadioHandler::csPin = csPin;
  RadioHandler::intPin = intPin;
  RadioHandler::frequency = frequency;
  rf95 = new RH_RF95(csPin, intPin);
  initializeRadio();
}

void RadioHandler::initializeRadio(){
  Serial.println("Initializing Radio");
  pinMode(resetPin, OUTPUT);
  digitalWrite(csPin, HIGH);

  // manual reset
  digitalWrite(resetPin, LOW);
  delay(10);
  digitalWrite(resetPin, HIGH);
  delay(10);

  while (!rf95->init()) {
    Serial.println("LoRa radio init failed");
    while (1);
  }

  // Defaults after init are 434.0MHz, modulation GFSK_Rb250Fd250, +13dbM
  if (!rf95->setFrequency(frequency)) {
    Serial.println("setFrequency failed");
    while (1);
  }
  Serial.println("Set Freq to: " + (String) frequency);
  rf95->setTxPower(23, false);
  Serial.println("Ready...");
}

// Send acknowledge back in the form of resending the message.
void RadioHandler::sendAck(char * buf) {
  rf95->send((uint8_t *) buf, MESSAGE_LEN);
  delay(10);
  rf95->waitPacketSent();      
}

/**
 * @brief Check for a message from the transmitter.
 * 
 * @return char* 
 */
char* RadioHandler::checkForMessage()
{  
  if (rf95->available())
  {
    // Should be a message for us now
    uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
    uint8_t len = sizeof(buf);

    if (rf95->recv(buf, &len))
    {
      //Check that we got a complete frame of what we're expecting
      if(len == MESSAGE_LEN &&  (buf[7] == END_OF_DATA)){
        char* message = new char[MESSAGE_LEN];
        for(int i = 0; i < MESSAGE_LEN; i++){
          message[i] = (char) buf[i];
        }
        return message;
      }
    }
    else
    {
      Serial.println("Receive failed");
    }
  } 
  return nullptr;
}


// Sends radio packet of length size. Returns an int16_t value of the receiver strength in dBm.
int16_t RadioHandler::sendRadioData(char* radioPacket, int size){
  int16_t rssi = 200;

  rf95->send((uint8_t *)radioPacket, size);
  // Serial.println("Waiting for packet to complete..."); 
  delay(10);
  rf95->waitPacketSent();
  // Now wait for a reply
  uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
  uint8_t len = sizeof(buf);

  // Serial.println("Waiting for reply...");
  if (rf95->waitAvailableTimeout(TIMEOUT_MILLIS))
  { 
    // Should be a reply message for us now   
    if (rf95->recv(buf, &len))
    {
      rssi = rf95->lastRssi();
    } else {
      Serial.println("Receive failed");
    }
  } else {
    Serial.println("No reply.");
  }
  return rssi;
}

/**
 * @brief Takes char* array of input data. This expects the output of the ControllerInput::getAsRadioPacket()
 * 
 * @param inputData 
 */
int RadioHandler::sendInput(char* inputData)
{  
  //Send frame - frametype, potval, upperbutton, lowerbutton
  char* radioPacket = new char[8];

  // Let the reciever know its input data
  radioPacket[0] = INPUT_DATA;
  // Theres probably a better way to do this
  radioPacket[1] = inputData[0];
  radioPacket[2] = inputData[1];
  radioPacket[3] = inputData[2];
  radioPacket[4] = inputData[3];
  radioPacket[5] = inputData[4];
  radioPacket[6] = inputData[5];

  // End
  radioPacket[7] = END_OF_DATA;
  
  int strength = sendRadioData(radioPacket, 8);

  delete radioPacket;
  delete inputData;
  
  return strength;
}

/**
 * @brief Sends a handshake and returns the rssi signal in dBa.
 * 
 */
int RadioHandler::getSignalStrength(){
  char* radioPacket = new char[8];
  radioPacket[0] = 'h';
  radioPacket[7] = 'e';
  return sendRadioData(radioPacket, 8);
}