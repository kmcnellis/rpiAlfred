#include <Arduino.h>
#include "bt_serial.h"
void setup();
void loop();
#line 1 "src/test.ino"

//#include "bt_serial.h"

//Followed by motor #, speed

void setup(){

    // Initialize both serial ports for bluetooth and computer
    Serial.begin(9600);
    Serial1.begin(9600); 
}

void loop(){

    // While there are motor updates from bluetooth
    while(motor_update()){

        // Read the type of update e.g. start, stop, move
        byte type = motor_read();
        if (type == MODE_MOVE){

            // Activate or enable motors mode
            Serial.println("Motors activated");

        }else if (type == MODE_STOP){

            // Deactivate or stop motors mode
            Serial.println("Motors deactivated");

        }else if (type == MODE_MOTOR_CONTROL){

            // Control motor speed mode
            byte motor_num = motor_read();
            byte motor_speed = motor_read();
            Serial.print("Change speed of motor ");
            Serial.print(motor_num);
            Serial.print(" to " );
            Serial.println(motor_speed);
        }
    }
}