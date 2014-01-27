#define CLOSERIGHT A2
#define CLOSELEFT A1
#define FARRIGHT A3
#define FARLEFT A0
#define CENTER A4
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
int closeLeft_total=0, closeRight_total=0, farLeft_total=0, farRight_total=0;
int t_speed=255;
int initCloseRight=0; //Contains intial values
int initCloseLeft=0;
int initFarRight=0;
int initFarLeft=0;
int initCenter=0;

#define sensorSpeed 10   
unsigned long sensorTimer;  
//function definitions
boolean turnModeR;
boolean turnModeL;
void read_ldrs();
void check_values();
void turn(int);
void total_change();
void start_line();
void initial();

void setup()
{
  Serial.begin(9600);  //Begin serial communcation
  pinMode(rMotorf,OUTPUT);
  pinMode(rMotorb,OUTPUT);
  pinMode(lMotorf,OUTPUT);
  pinMode(lMotorb,OUTPUT);
  pinMode(LED, OUTPUT);
  turnModeR=false;
  turnModeL=false;
  initCloseRight=0; 
  initCloseLeft=0;
  initFarRight=0;
  initFarLeft=0;
  initCenter=0;

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
  }
}
void initial(){
  for (int a=0; a<10; a++)
  {
    
  }

}
void read_ldrs()
{
  closeLeft_total= analogRead(CLOSELEFT)*.5 + closeLeft_total*.5;
  closeRight_total+= analogRead(CLOSERIGHT)*.5 + closeRight_total*.5;
  farLeft_total+= analogRead(FARLEFT)*.5 + farLeft_total*.5;
  farRight_total+= analogRead(FARRIGHT*.5 + farRight_total*.5);
  return;
}

void check_values()
{
  int diffLeft=abs(closeLeft_total-initCloseLeft);
  int diffRight=abs(closeRight_total -initCloseRight);
  int diffLeftFar=abs(farLeft_total-initFarLeft);
  int diffRightFar=abs(farRight_total -initFarRight);

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
    if (turnModeL){
      turn(F_RIGHT);
    }
    else{  
      turn(RIGHT);
    }
    turnModeR=false;
  }
  else if(diffRight>diffLeft && diffRight>DIFFERENCE)
  {
    if (turnModeR){
      turn(F_LEFT);
    }
    else{  
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

