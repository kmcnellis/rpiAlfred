#define FRONTRIGHT A0
#define FRONTLEFT A1
#define BACKRIGHT A2
#define BACKLEFT A3

int lightPin1 = A5;  //define a pin for Photo resistor
int lightPin2 = A4; 
int ledPin=A0;     //define a pin for LED

int motor1= 2;
int motor2 =3;

int leftvalues[20];
int rightvalues[20];
int index = 0;
int average =0;

void average_func()
{
  int l_total = 0;
  int r_total = 0;
  for (int i=0; i < sizeof(leftvalues); i++)
  {
    l_total+= leftvalues[i];
    r_total += rightvalues[i];
  }
  average = l_total/sizeof(values);
}

void setup()
{
    Serial.begin(9600);  //Begin serial communcation
    pinMode( ledPin, OUTPUT );
    
    pinMode(motor1,OUTPUT);
  
    pinMode(motor2,OUTPUT);
}

void loop()
{
    Serial.print(analogRead(lightPin1)); //Write the value of the photoresistor to the serial monitor.
    Serial.print("      |      ");
    Serial.println(analogRead(lightPin2));
    analogWrite(ledPin, analogRead(lightPin1)/4);  //send the value to the ledPin. Depending on value of resistor 
                                                //you have  to divide the value. for example, 
                                                //with a 10k resistor divide the value by 2, for 100k resistor divide by 4.
   delay(100); //short delay for faster response to light.
   
   digitalWrite(motor1,HIGH);
   digitalWrite(motor2,LOW);
}
