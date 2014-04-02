char val;         // variable to receive data from the serial port
int ledpin = 12; 

void setup()
{
  pinMode(ledpin, OUTPUT);  // pin 13 (on-board LED) as OUTPUT
 
  Serial.begin(9600); 
 digitalWrite(ledpin,LOW);
}
 
void loop() {
  if( Serial.available() )       // if data is available to read
  {;}
    val = Serial.read();         // read it and store it in 'val'
 
  if( val == '0' )               // if '0' was received led 13 is switched off

  {
   digitalWrite(ledpin, LOW);    // turn Off pin 13 off
  delay(1000);                  // waits for a second   
  Serial.println("13 off");
  }

if( val == '1' )               // if '1' was received led 13 on
 {
    digitalWrite(ledpin, HIGH);  // turn ON pin 13 on
    delay(1000);                  // waits for a second
    Serial.println("13 on");
  }
}
