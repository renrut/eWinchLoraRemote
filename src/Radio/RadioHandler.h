#include <SPI.h>
#include <RH_RF95.h>

//Pins for feather LoRa
#define RFM95_CS 8
#define RFM95_RST 4
#define RFM95_INT 3

#define RF95_FREQ 915.0

#define TIMEOUT_MILLIS 100
#define MESSAGE_LEN 8

class RadioHandler {
    public:
        static const char INPUT_DATA = 'd';
        static const char HANDSHAKE = 'h';
        static const char END_OF_DATA = 'e';

        RadioHandler();
        RadioHandler(int resetPin, int csPin, int intPin, float frequency);
        void initializeRadio();
        
        void sendAck(char * buf);
        int sendInput(char* inputData);
        int16_t sendRadioData(char* radioPacket, int size);

        char* checkForMessage();
        int getSignalStrength();
        
    private:
        int resetPin;
        int csPin;
        int intPin;
        float frequency;
        RH_RF95* rf95;        
};