#define CLOSERIGHT A0
#define CLOSELEFT A1
#define FARRIGHT A2
#define FARLEFT A3
#define LENGTH_CACHE 20
#define DIFFERENCE 5
// rotation signals
#define ROTATE_RIGHT 0
#define ROTATE_LEFT 1
#define ROTATE_STOP 2

#define RIGHT 3
#define LEFT 4
#define FORWARD 5
#define BACK 6
#define STOP 7
 
#define F_RIGHT 8
#define F_LEFT 19
//motor control
#define GO 1
#define STILL 0
#define REV 2
#define SLOW_GO 3
#define SLOW_REV 4
// motor pins
#define rMotorf 3
#define rMotorb 5
#define lMotorf 6
#define lMotorb 9

#define LED 12
#define IR A3


#define DARK_ON_LIGHT  0
#define LIGHT_ON_DARK  1

const int MODE = DARK_ON_LIGHT;

int closeLeftValues[LENGTH_CACHE];
int closeRightValues[LENGTH_CACHE];
int farLeftValues[LENGTH_CACHE];
int farRightValues[LENGTH_CACHE];

int index = 0;
int closeLeft_average=0,closeRight_average=0,farLeft_average=0,farRight_average=0;
int closeLeft_total=0, closeRight_total=0, farLeft_total=0, farRight_total=0;

int t_speed=255;
//Contains non-line values
int initCloseRight=0;
int initCloseLeft=0;
int initFarRight=0;
int initFarLeft=0;

unsigned int sensorSpeed = 50; 
unsigned long sensorTimer;     
//function definitions


boolean initial;
void read_ldrs();
void average_func();
void check_values();
void turn(int);
void total_change();
void start_line();

void setup()
{
  Serial.begin(9600);  //Begin serial communcation
  pinMode(rMotorf,OUTPUT);
  pinMode(rMotorb,OUTPUT);
  pinMode(lMotorf,OUTPUT);
  pinMode(lMotorb,OUTPUT);
  pinMode(LED, OUTPUT);
  initial=false;

  initCloseRight=0; 
  initCloseLeft=0;
  initFarRight=0;
  initFarLeft=0;

  index = 0;
  closeLeft_average=0;
  closeRight_average=0;
  farLeft_average=0;
  farRight_average=0;
  closeLeft_total=0; 
  closeRight_total=0; 
  farLeft_total=0; 
  farRight_total=0;

  sensorTimer = millis();
}

void loop()
{  
  if (millis() >= sensorTimer) {   // pingSpeed milliseconds since last ping, do another ping.
    sensorTimer += sensorSpeed;  
    digitalWrite(LED, HIGH);
    read_ldrs();
    index += 1;
    index=index%20;
    if (index==19 && initial==true){
      check_values();
      closeLeft_total=0; 
      closeRight_total=0; 
      farLeft_total=0; 
      farRight_total=0;
    }
    if(index==19 && initial==false){
      average_func();
      initCloseRight= closeLeft_average;
      initCloseLeft= closeRight_average;
      initFarRight=farLeft_average=0;
      initFarLeft= farRight_average=0;
      initial==true;

    }
  }

}

void read_ldrs()
{
  closeLeftValues[index] = analogRead(CLOSELEFT);
  closeRightValues[index] = analogRead(CLOSERIGHT);
  farLeftValues[index] = analogRead(FARLEFT);
  farRightValues[index] = analogRead(FARRIGHT);
  closeLeft_total+=closeLeftValues[index];
  closeRight_total+=closeRightValues[index];
  farLeft_total+=farLeftValues[index];
  farRight_total+=farRightValues[index];
  return;
}

void check_values()
{
  average_func();
  if(abs(closeLeft_average-initCloseLeft)>DIFFERENCE)//this won't work for gradual/slow changes
  {
    Serial.print(closeLeft_average);
    Serial.print("|");
    Serial.println(closeRight_average);

    turn(RIGHT);
  }
  else if(abs(closeRight_average -initCloseRight)>DIFFERENCE)
  {
    Serial.print(closeLeft_average);
    Serial.print("|");
    Serial.println(closeRight_average);
    turn(LEFT);
  }
  else
  {
    Serial.print(closeLeft_average);
    Serial.print("|");
    Serial.println(closeRight_average);
    turn(FORWARD);
  }
  return;
}

void average_func()
{
  closeLeft_average = closeLeft_total/LENGTH_CACHE;
  closeRight_average = closeRight_total/LENGTH_CACHE;
  farLeft_average = farLeft_total/LENGTH_CACHE;
  farRight_average = farRight_total/LENGTH_CACHE;
  return;
}

void turn(int turn_signal) {
  int rmotor = 0;
  int lmotor = 0;
  int slowSpeed = t_speed/2;
  switch (turn_signal){
  case RIGHT:
    Serial.println("Turn Right");
    rmotor = GO;
    lmotor = STILL;
    break;

  case LEFT:
    Serial.println("Turn Left");
    rmotor = STILL;
    lmotor = GO;
    break;

  case FORWARD:
    Serial.println("Go Forward");
    rmotor = GO;
    lmotor = GO;
    break;

  case BACK:
    Serial.println("Go Backwards");
    rmotor = REV;
    lmotor = REV;
    break;

  case STOP:
    Serial.println("Stop");
    rmotor = STILL;
    lmotor = STILL;
    break;

  case ROTATE_RIGHT:
    Serial.println("Rotate Right");
    rmotor = REV;
    lmotor = GO;
    break;

  case ROTATE_LEFT:
    Serial.println("Rotate Left");
    rmotor = GO;
    lmotor = REV;
    break;

  case ROTATE_STOP:
    Serial.println("Stop");
    rmotor = STILL;
    lmotor = STILL;
    break;

  case F_RIGHT:
    Serial.println("Turn Right while Moving");
    rmotor = GO;
    lmotor = SLOW_GO;
    break;

  case F_LEFT:
    Serial.println("Turn Left while Moving");
    rmotor = SLOW_GO;
    lmotor = GO;
    break;    
  }
  switch (rmotor){
  case GO:
    analogWrite(rMotorf, t_speed);
    // digitalWrite(rMotorf, HIGH);
    digitalWrite(rMotorb, LOW);
    break;

  case STILL:
    digitalWrite(rMotorf, LOW);
    digitalWrite(rMotorb, LOW);
    break;

  case REV:
    digitalWrite(rMotorf, LOW);
    //digitalWrite(rMotorb, HIGH);
    analogWrite(rMotorb, t_speed);
    break;

  case SLOW_GO:
    analogWrite(rMotorf, slowSpeed);
    digitalWrite(rMotorb, LOW);
    break;

  case SLOW_REV:
    digitalWrite(rMotorf, LOW);
    analogWrite(rMotorb, slowSpeed);
    break;
  }

  switch (lmotor){
  case GO:
    analogWrite(lMotorf, t_speed);
    // digitalWrite(lMotorf, HIGH);
    digitalWrite(lMotorb, LOW);
    break;

  case STILL:
    digitalWrite(lMotorf, LOW);
    digitalWrite(lMotorb, LOW);
    break;

  case REV:
    digitalWrite(lMotorf, LOW);
    //digitalWrite(lMotorb, HIGH);
    analogWrite(lMotorb, t_speed);
    break;

  case SLOW_GO:
    analogWrite(lMotorf, slowSpeed);
    digitalWrite(lMotorb, LOW);
    break;

  case SLOW_REV:
    digitalWrite(lMotorf, LOW);
    analogWrite(lMotorb, slowSpeed);  
    break;    
  }
}





