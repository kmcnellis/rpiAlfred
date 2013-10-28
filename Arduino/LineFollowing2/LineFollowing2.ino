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

int front_leftvalues[LENGTH_CACHE];
int front_rightvalues[LENGTH_CACHE];
int back_leftvalues[LENGTH_CACHE];
int back_rightvalues[LENGTH_CACHE];

int index = 0;
int fl_average=0,fr_average=0,bl_average=0,br_average=0;
int fl_total=0, fr_total=0, bl_total=0, br_total=0;
  
int t_speed=255;

//function definitions
int total_func(int);
void average_func();
void read_ldrs();
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
    Serial.println("Setup");
    for (index = 0;index<20;index++)
    {
      
      read_ldrs();
      fl_total+=front_leftvalues[index];
      fr_total+=front_rightvalues[index];
      bl_total+=back_leftvalues[index];
      br_total+=back_rightvalues[index];

    }
    Serial.println();
    Serial.print("Front Left");
    for (int i = 0;i<20;i++)
    {
      
      Serial.print(front_leftvalues[i]);
      Serial.print(" | ");

    }
    Serial.println();
    Serial.print("Front Right");
    for (int i = 0;i<20;i++)
    {
      
      Serial.print(front_rightvalues[i]);
      Serial.print(" | ");

    }
    Serial.println();
    Serial.print("Back Left");
    for (int i = 0;i<20;i++)
    {
      
      Serial.print(back_leftvalues[i]);
      Serial.print(" | ");

    }
    Serial.println();
    Serial.print("Back Right");
    for (int i = 0;i<20;i++)
    {
      
      Serial.print(back_rightvalues[i]);
      Serial.print(" | ");

    }
    Serial.println();
}

void loop()
{
  digitalWrite(LED, HIGH);
  read_ldrs();
  check_values();
  index += 1;
  index=index%20;
  
  //Serial.println(analogRead(IR));
  
  delay(200);
}



int total_func()
{

  fl_total =0;
     
      fr_total =0;
      bl_total =0;
      br_total=0;
  for (int i=0; i <= LENGTH_CACHE; i++)
  {
    
    {
      fl_total += front_leftvalues[i];
      //Serial.println(front_leftvalues[i]);
      fr_total += front_rightvalues[i];
      bl_total += back_leftvalues[i];
      br_total += back_rightvalues[i];
    }
  }
  //Serial.println();
  //Serial.print("fl ");
  //Serial.print(fl_total);
 // Serial.print(" | fr ");
  //Serial.println(fr_total);
}
/*
int total_change()
{
  int last_index=0;
  last_index=index+1;
  if (last_index==20)
    last_index=0;
  
    
  fl_total += front_leftvalues[index];
  fl_total -= front_leftvalues[last_index];
  
  fr_total += front_rightvalues[index];
  fr_total -= front_rightvalues[last_index];
  
  bl_total += back_leftvalues[index];
  bl_total -= back_leftvalues[last_index];
  
  br_total += back_rightvalues[index];
  br_total -= back_rightvalues[last_index]; 
}
*/
void average_func()
{
  total_func();

  
  fl_average = fl_total/LENGTH_CACHE;
  //Serial.print(fl_average);
  //Serial.print("  |  ");
  fr_average = fr_total/LENGTH_CACHE;
  //Serial.println(fr_average);
  bl_average = bl_total/LENGTH_CACHE;
  br_average = br_total/LENGTH_CACHE;
}

void read_ldrs()
{
  front_leftvalues[index] = analogRead(FRONTLEFT);
  
  
  //Serial.println(front_leftvalues[LENGTH_CACHE-1]);
  front_rightvalues[index] = analogRead(FRONTRIGHT);
  
  //Serial.println(front_rightvalues[LENGTH_CACHE]);
  back_leftvalues[index] = analogRead(BACKLEFT);
  back_rightvalues[index] = analogRead(BACKRIGHT);
}

void check_values()
{
  average_func();

  if(abs(front_leftvalues[index]-fl_average)>DIFFERENCE)//this won't work for gradual/slow changes
  {
    //Turn Right
//    Serial.println("Right");
    Serial.print(fr_average);
    Serial.print(" | ");
    Serial.print(fr_total);
    Serial.print(" | ");
    Serial.print(fl_average);
    Serial.print(" | ");
    Serial.print(fl_total);
    Serial.print(" | ");
    turn(RIGHT);
    Serial.println();
  }
  else if(abs(front_rightvalues[index] -fr_average)>DIFFERENCE)
  {
    //Turn LEFT
    //Serial.println("Left");

    Serial.print(fr_average);
    Serial.print(" | ");
    Serial.print(fr_total);
    Serial.print(" | ");
    Serial.print(fl_average);
    Serial.print(" | ");
    Serial.print(fl_total);
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
