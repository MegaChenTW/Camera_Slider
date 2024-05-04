#include <Arduino.h>
#include <StepperDriver.h>
#include <Menu.h>

//attach pin interrupt
void EmergencyStop();

float Duration;
Vector2 Size(10,20);
Menu menu(Duration,Size);

void setup() {
    //initailize

}

void loop() {
    //Scan all Pins for new input
    if(menu.input != ButtonInput::None){
        
    }
}

void EmergencyStop(){
    
}