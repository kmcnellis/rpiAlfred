#define FRONTRIGHT A0
#define FRONTLEFT A1
#define BACKRIGHT A2
#define BACKLEFT A3
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


const int DARK_ON_LIGHT = 0;
const int LIGHT_ON_DARK = 1;
const int MODE = DARK_ON_LIGHT;

int fl_average,fr_average,bl_average,br_average;
int fl_total=0, fr_total=0, bl_total=0, br_total=0;
  
int t_speed=255;

//function definitions
int total_func(int);
void check_values();
void turn(int);

void setup()
{
    Serial.begin(9600);  //Begin serial communcation
    
    pinMode(rMotorf,OUTPUT);
    pinMode(rMotorb,OUTPUT);
    pinMode(lMotorf,OUTPUT);
    pinMode(lMotorb,OUTPUT);
    pinMode(LED, OUTPUT);
    
    int index = 0;
    
    while (index<20)
    {  
      fl_total+= analogRead(FRONTLEFT);;
      fr_total+=analogRead(FRONTRIGHT);;
      index +=1;
    }
    fl_average = fl_total/20;
    fr_average = fr_total/20;
}

void loop()
{
  digitalWrite(LED, HIGH);
  check_values();
  delay(200);
}

void check_values()
{
  Serial.print(analogRead(FRONTLEFT));
  Serial.print("|");
  Serial.println(analogRead(FRONTRIGHT));
  if(abs(analogRead(FRONTLEFT)-fl_average)>DIFFERENCE)//this won't work for gradual/slow changes
  {
    Serial.print(fl_average);
    Serial.print(" | ");
    turn(RIGHT);
  }
  else if(abs(analogRead(FRONTRIGHT) -fr_average)>DIFFERENCE)
  {
    Serial.print(fr_average);
    Serial.print(" | ");
    turn(LEFT);
  }
  else
    turn(FORWARD);
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
    Serial.println("Rotate Left");cd 
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
