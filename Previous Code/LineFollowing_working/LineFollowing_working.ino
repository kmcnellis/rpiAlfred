#define CLOSERIGHT A2
#define CLOSELEFT A1
#define FARRIGHT A3
#define FARLEFT A0
#define CENTER A4
#define LENGTH_CACHE 20
#define DIFFERENCE 15
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
int index = 0;
int closeLeft_average=0,closeRight_average=0,farLeft_average=0,farRight_average=0,center_average=0;
int closeLeft_total=0, closeRight_total=0, farLeft_total=0, farRight_total=0;
int t_speed=255;
int initCloseRight=0; //Contains intial values
int initCloseLeft=0;
int initFarRight=0;
int initFarLeft=0;
int initCenter=0;

#define sensorSpeed 10   
unsigned long sensorTimer;  
const int numSensor = LENGTH_CACHE-1;// number of values in array
//function definitions
boolean initial;
boolean first;
boolean turnModeR;
boolean turnModeL;
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
  first=false;
  turnModeR=false;
  turnModeL=false;
  initCloseRight=0; 
  initCloseLeft=0;
  initFarRight=0;
  initFarLeft=0;
  initCenter=0;

  index = 0;
  closeLeft_average=0;
  closeRight_average=0;
  farLeft_average=0;
  farRight_average=0;
  
  closeLeft_total=0; 
  closeRight_total=0; 
  farLeft_total=0; 
  farRight_total=0;

  sensorTimer = millis()+100;
}

void loop()
{  
  if (millis() >= sensorTimer) {   // pingSpeed milliseconds since last ping, do another ping.
    sensorTimer += sensorSpeed;  
    digitalWrite(LED, HIGH);
    read_ldrs();   
    if (index==numSensor && initial==true){// initial = true means it was intialized index==numSensor
      check_values();
      closeLeft_total=0; 
      closeRight_total=0; 
      farLeft_total=0; 
      farRight_total=0;
    }
    else if(index==numSensor && initial==false){
      average_func();
      initCloseLeft= closeLeft_average;
      initCloseRight= closeRight_average;
      initFarLeft=farLeft_average;
      initFarRight= farRight_average;
      initCenter = center_average;
      Serial.print(" = ");
      Serial.println(closeLeft_average);
      
      closeLeft_total=0; 
      closeRight_total=0; 
      farLeft_total=0; 
      farRight_total=0;
      initial=true;
    }
    else if(index==numSensor &&first==false){
      first==true;}
    index += 1;
    index=index%(numSensor+1);
  }
}

void read_ldrs()
{
  closeLeft_total+= analogRead(CLOSELEFT);
  closeRight_total+= analogRead(CLOSERIGHT);
  farLeft_total+= analogRead(FARLEFT);
  farRight_total+= analogRead(FARRIGHT);
  return;
}

void check_values()
{
  average_func();
  int diffLeft=abs(closeLeft_average-initCloseLeft);
  int diffRight=abs(closeRight_average -initCloseRight);
  int diffLeftFar=abs(farLeft_average-initFarLeft);
  int diffRightFar=abs(farRight_average -initFarRight);

  if(diffLeftFar>DIFFERENCE)
  {
    turnModeL=true;
    turnModeR=false;
    turn(F_RIGHT);
  }
  else if(diffRightFar>DIFFERENCE)
  {
    turnModeR=true;
    turnModeL=false;
    turn(F_LEFT);

  }
  else if(diffLeft>diffRight && diffLeft>DIFFERENCE)//this won't work for gradual/slow changes
  {
    if (turnModeL)
    {
      turn(F_RIGHT);
    }
    else
    {  
      turn(RIGHT);
    }
    turnModeR=false;
  }
  else if(diffRight>diffLeft && diffRight>DIFFERENCE)
  {
    if (turnModeR)
    {
      turn(F_LEFT);
    }
    else
    {  
      turn(LEFT);
    }
    turnModeL=false;
  }
  else
  { 
    turnModeR=false;
    turnModeL=false;
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
    Serial.println("Turn Sharp Right");
    rmotor = GO;
    lmotor = SLOW_GO;
    break;

  case F_LEFT:
    Serial.println("Turn Sharp Left");
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
