#include "ControllerInput.h"

ControllerInput::ControllerInput()
{
    ControllerInput::potValue = -1;
    ControllerInput::upperButton = false;
    ControllerInput::lowerButton = false;
}

ControllerInput::ControllerInput(int potVal, bool upperButton, bool lowerButton)
{
    ControllerInput::potValue = potVal;
    ControllerInput::upperButton = upperButton;
    ControllerInput::lowerButton = lowerButton;
}

int ControllerInput::getPotValue(){
    return potValue;
}

// Return the flipped pot value between the min and the max. Ie if the pot was 0-1000 and the
// value was 475, it'll return 525.
int ControllerInput::getPotValueFlipped(){
     return POT_MAX_VALUE - getPotValue() + POT_MIN_VALUE;
}

bool ControllerInput::getUpperButton(){
    return upperButton;
}

bool ControllerInput::getLowerButton(){
    return lowerButton;
}

void ControllerInput::setPotValue(int val){
    ControllerInput::potValue = val;

}

void ControllerInput::setUpperButton(bool val){
    ControllerInput::upperButton = val;

}

void ControllerInput::setLowerButton(bool val){
    ControllerInput::lowerButton = val;
}

String ControllerInput::toString(){
    return (String)ControllerInput::potValue + "," + (String) ControllerInput::upperButton + "," + (String) ControllerInput::lowerButton; 
}

char* ControllerInput::getAsRadioPacket(){
    char* bytes = new char[6];
    char* potBuffer = (char*)&potValue;

    bytes[0] = potBuffer[0];
    bytes[1] = potBuffer[1];
    bytes[2] = potBuffer[2];
    bytes[3] = potBuffer[3];

    bytes[4] = (char) upperButton;
    bytes[5] = (char) lowerButton;

    return bytes;
}
