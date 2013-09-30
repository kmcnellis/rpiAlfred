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

#define GO 1
#define STILL 0
#define REV 2
#define SLOW_GO 3
#define SLOW_REV 4

#define rMotorf 3
#define rMotorb 5
#define lMotorf 6
#define lMotorb 9



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
void setup()
{
  Serial.begin(9600);
  pinMode(rMotorf,OUTPUT);
  pinMode(rMotorb,OUTPUT);
  pinMode(lMotorf,OUTPUT);
  pinMode(lMotorb,OUTPUT);
}

void loop()
{
 
}

