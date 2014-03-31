#include <Arduino.h>

void setup();
void loop();
#line 1 "src/test.ino"


#define MODE_MOVE 0
#define MODE_STOP 1
#define MODE_MOTOR_CONTROL 2
//Followed by motor #, speed

void setup(){
  Serial.begin(9600);
  Serial1.begin(9600); 
}

void loop(){
	if (Serial1.available()){
                char c = Serial1.read();
                
		Serial.println(c);
                Serial1.print("ACK ");
                Serial1.println(c);
	}
}
//void setup()
//{
//  pinMode(ledpin, OUTPUT);  // pin 13 (on-board LED) as OUTPUT
// 
//  Serial1.begin(9600); 
// digitalWrite(ledpin,LOW);
//}
 
//void loop() {
//  if( Serial1.available() )       // if data is available to read
//    {
//      val = Serial1.read();         // read it and store it in 'val'
//   
//    if( val == '0' )               // if '0' was received led 13 is switched off
//  
//    {
//     digitalWrite(ledpin, LOW);    // turn Off pin 13 off
//    delay(1000);                  // waits for a second   
//    Serial1.println("13 off");
//    }
//  
//    if( val == '1' )               // if '1' was received led 13 on
//       {
//      digitalWrite(ledpin, HIGH);  // turn ON pin 13 on
//      delay(1000);                  // waits for a second
//      Serial1.println("13 on");
//      }
//  }
//}
