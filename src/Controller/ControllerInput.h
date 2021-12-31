#include <Arduino.h>      /* printf, fgets */
#include <stdlib.h>     /* atoi */

class ControllerInput {
    public:
        static const int POT_MIN_VALUE = 0;
        static const int POT_MAX_VALUE = 1000;

        ControllerInput();
        ControllerInput(int potVal, bool upperButton, bool lowerButton);
        int getPotValue();
        int getPotValueFlipped();
        bool getUpperButton();
        bool getLowerButton();
        void setPotValue(int val);
        void setUpperButton(bool val);
        void setLowerButton(bool val);
        char* getAsRadioPacket();
        String toString();

        static ControllerInput* fromCharArray(char* arr){
            
            char potBuffer[4];
            potBuffer[0] = arr[0];
            potBuffer[1] = arr[1];
            potBuffer[2] = arr[2];
            potBuffer[3] = arr[3];
            int* potValPtr = (int*)potBuffer;

            int potValue = *potValPtr;
            bool upperButton = (bool) arr[4];
            bool lowerButton = (bool) arr[5];
            return new ControllerInput(potValue, upperButton, lowerButton);
        }
    private:
        int potValue;
        bool upperButton;
        bool lowerButton;
};