#define FRONTRIGHT A0
#define FRONTLEFT A1
#define BACKRIGHT A2
#define BACKLEFT A3
#define MOTOR1 2
#define MOTOR2 3
#define LENGTH_CACHE 20
#define DIFFERENCE 10

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


const int DARK_ON_LIGHT = 0;
const int LIGHT_ON_DARK = 1;
const int MODE = DARK_ON_LIGHT;

int front_leftvalues[LENGTH_CACHE];
int front_rightvalues[LENGTH_CACHE];
int back_leftvalues[LENGTH_CACHE];
int back_rightvalues[LENGTH_CACHE];

int onLine=255;
int onLine=0;
int absDiff=5;

int index = 0;
int fl_average,fr_average,bl_average,br_average;

int num_vals=0;


//function definitions
int total_func(int);
void average_func();
void read_ldrs();
void check_values();
void turn(int,int);

void setup()
{
    Serial.begin(9600);  //Begin serial communcation
    
    pinMode(MOTOR1,OUTPUT);
  
    pinMode(MOTOR2,OUTPUT);
    
    for (int i = 0;i<20;i++)
    {
      read_ldrs();
    }
}

void loop()
{
  read_ldrs();
  check_values();
  delay(100);
}



int total_func(int array[])
{
  int total = 0;
  for (int i=0; i < LENGTH_CACHE; i++)
  {
    if (array[i] >0)
    {
      total += array[i];
    }
  }
  return total;
}

void average_func()
{
  int fl_total = total_func(front_leftvalues);
  int fr_total = total_func(front_rightvalues);
  int bl_total = total_func(back_leftvalues);
  int br_total = total_func(back_rightvalues);
  
  fl_average = fl_total/LENGTH_CACHE;
  fr_average = fr_total/LENGTH_CACHE;
  bl_average = bl_total/LENGTH_CACHE;
  br_average = br_total/LENGTH_CACHE;
}

void read_ldrs()
{
  front_leftvalues[index] = analogRead(FRONTLEFT);
  front_rightvalues[index] = analogRead(FRONTRIGHT);
  back_leftvalues[index] = analogRead(BACKLEFT);
  back_rightvalues[index] = analogRead(BACKRIGHT);  
  
  index += 1;
  if (index == 20)
  {
    index =0;
  }
}

void check_values()
{
  average_func();
  if((abs(front_leftvalues[index-1]-fl_average)>DIFFERENCE)//this won't work for gradual/slow changes
  //if((abs(front_leftvalues[index-1]-fl_average)>DIFFERENCE) || (((abs(front_leftvalues[index-1]-offLine))<absDiff) && (abs(front_leftvalues[index-1]-onLine))>absDiff) )
  {
    //Turn Right
    Serial.println("Right");
    turn(RIGHT);
  }
  else if(abs(front_rightvalues[index-1] -fr_average)>DIFFERENCE)
  {
    //Turn LEFT
    Serial.println("Left");
    turn(LEFT);
  }
  else
    turn(FORWARD);
}

void turn(int turn_signal, int t_speed=255) {
  int rmotor = 0;
  int lmotor = 0;
  int slowSpeed = t_speed/2;
  switch (turn_signal){
  case RIGHT:
    Serial.write("Turn Right");
    rmotor = GO;
    lmotor = STILL;
    break;

  case LEFT:
    Serial.write("Turn Left");
    rmotor = STILL;
    lmotor = GO;
    break;

  case FORWARD:
    Serial.write("Go Forward");
    rmotor = GO;
    lmotor = GO;
    break;

  case BACK:
    Serial.write("Go Backwards");
    rmotor = REV;
    lmotor = REV;
    break;

  case STOP:
    Serial.write("Stop");
    rmotor = STILL;
    lmotor = STILL;
    break;

  case ROTATE_RIGHT:
    Serial.write("Rotate Right");
    rmotor = REV;
    lmotor = GO;
    break;

  case ROTATE_LEFT:
    Serial.write("Rotate Left");
    rmotor = GO;
    lmotor = REV;
    break;

  case ROTATE_STOP:
    Serial.write("Stop");
    rmotor = STILL;
    lmotor = STILL;
    break;

  case F_RIGHT:
    Serial.write("Turn Right while Moving");
    rmotor = GO;
    lmotor = SLOW_GO;
    break;

  case F_LEFT:
    Serial.write("Turn Left while Moving");
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
