#define SODA_1_PIN 4
#define SODA_2_PIN 5
#define SODA_3_PIN 6
#define SODA_4_PIN 7

#define SODA1 1
#define SODA2 2
#define SODA3 3
#define SODA4 4

int dispensing = 0;

void bluetoothRead();

void setup() {
	Serial.begin(9600);  //Begin serial communcation
	// Initialize Ports
	pinMode(SODA_1_PIN, OUTPUT);
	pinMode(SODA_2_PIN, OUTPUT);
	pinMode(SODA_3_PIN, OUTPUT);
	pinMode(SODA_4_PIN, OUTPUT);
}

void loop() {
	readBluetooth(); //TODO Get bluetooth read working
	if(dispensing){
		switch(dispensing){
			case SODA1:
				Serial.println("Soda 1 Dispensing");
				digitalWrite(SODA_1_PIN, HIGH);
				break;
			case SODA2:
				Serial.println("Soda 2 Dispensing");
				digitalWrite(SODA_2_PIN, HIGH);
				break;	
			case SODA3:
				Serial.println("Soda 3 Dispensing");
				digitalWrite(SODA_3_PIN, HIGH);
				break;
			case SODA4:
				Serial.println("Soda 4 Dispensing");
				digitalWrite(SODA_4_PIN, HIGH);
				break;
		}
	}

	delay(100);
	
	digitalWrite(SODA_1_PIN, LOW);
	digitalWrite(SODA_2_PIN, LOW);
	digitalWrite(SODA_3_PIN, LOW);
	digitalWrite(SODA_4_PIN, LOW);
}

// Read the LAST message sent from the android and interpret
void readBluetooth(){

  // Fill line variable with serial contents
  // Reset lines if there is a newline character (so it only interprets the last message sent)

  String line = "";
  byte mode;

  while (Serial.available()){
    mode = Serial.read();
  }

  // Interpret contents of last message

  // At the moment, there are only two possible cases
  // "0" => Not Dispensing (Move)
  // "1" => Dispensing Soda 1
  // "2" => Dispensing Soda 2
  // "3" => Dispensing Soda 3
  // "4" => Dispensing Soda 4

  if (mode > 0 || mode < 5){
    dispensing = mode;
  }
  else{
	Serial.println("Stopped Dispensing!");  	
  	dispensing = 0;
  }

}
