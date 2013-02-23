// MaxSonar Base Code From Bruce Allen, http://playground.arduino.cc/Main/MaxSonar

// analog pin for reading sonar
const int sonar_an_pin = 7;
const int right_an_pin = 0;
const int left_an_pin = 1;


long sonar_an_volt, inches, cm;
int sum=0;
int avgrange=4;

void setup() {
    Serial.begin(9600);
}

void loop() {
    pinMode(sonar_an_pin, INPUT);
    pinMode(right_an_pin, INPUT);
    pinMode(left_an_pin, INPUT);

    //MaxSonar Analog reads are known to be very sensitive. See the Arduino forum for more information.
    //A simple fix is to average out a sample of n readings to get a more consistant reading.\\
    //Even with averaging I still find it to be less accurate than the pw method.\\
    //This loop gets 60 reads and averages them
    for(int i = 0; i < avgrange ; i++) {
        //Used to read in the analog voltage output that is being sent by the MaxSonar device.
        //Scale factor is (Vcc/512) per inch. A 5V supply yields ~9.8mV/in
        //Arduino analog pin goes from 0 to 1024, so the value has to be divided by 2 to get the actual inches
        sonar_an_volt = analogRead(sonar_an_pin)/2;
        sum += sonar_an_volt;
        delay(10);
    }

    inches = sum/avgrange;
    cm = inches * 2.54;
    Serial.print(inches);
    Serial.print("in, ");
    Serial.print(cm);
    Serial.print("cm");
    Serial.println();

    sum = 0;



    int rval = analogRead(right_an_pin);
    int lval = analogRead(left_an_pin);
    Serial.println(rval);
    Serial.println(lval);

    delay(500);
}

