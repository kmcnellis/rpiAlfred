#define ROTATION_CLOCKWISE 0
#define ROTATION_COUNTERCLOCKWISE 1
#define ROTATION_STOP 2

#define RIGHT 3
#define LEFT 4
#define FORWARD 5
#define BACK 6
#define STOP 7

#define S_RIGHT 8
#define S_LEFT 9
#define S_FORWARD 10
#define S_BACK 11

#define F_RIGHT 12
#define F_LEFT 13

#define GO 1
#define STILL 0
#define REV 2
#define SLOW_GO 3
#define SLOW_REV 4

#define rMotorf 3
#define rMotorb 5
#define lMotorf 6
#define lMotorb 9

#define slowSpeed 128


void turn(int turn_signal) {
  int rmotor = 0;
  int lmotor = 0;

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

  case ROTATION_CLOCKWISE:
    Serial.write("ROTATION_CLOCKWISE");
    rmotor = REV;
    lmotor = GO;
    break;

  case ROTATION_COUNTERCLOCKWISE:
    Serial.write("ROTATION_COUNTERCLOCKWISE");
    rmotor = GO;
    lmotor = REV;
    break;

  case ROTATION_STOP:
    Serial.write("ROTATION_STOP");
    rmotor = STILL;
    lmotor = STILL;
    break;

  case S_RIGHT:
    Serial.write("Turn Right Slowly");
    rmotor = SLOW_GO;
    lmotor = STILL;
    break;

  case S_LEFT:
    Serial.write("Turn Left Slowly");
    rmotor = STILL;
    lmotor = SLOW_GO;
    break;

  case S_FORWARD:
    Serial.write("Go Forward Slowly");
    rmotor = SLOW_GO;
    lmotor = SLOW_GO;
    break;

  case S_BACK:
    Serial.write("Go Backwards Slowly");
    rmotor = SLOW_REV;
    lmotor = SLOW_REV;
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
    digitalWrite(rMotorf, HIGH);
    digitalWrite(rMotorb, LOW);
    break;

  case STILL:
    digitalWrite(rMotorf, LOW);
    digitalWrite(rMotorb, LOW);
    break;

  case REV:
    digitalWrite(rMotorf, LOW);
    digitalWrite(rMotorb, HIGH);
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
    digitalWrite(lMotorf, HIGH);
    digitalWrite(lMotorb, LOW);
    break;

  case STILL:
    digitalWrite(lMotorf, LOW);
    digitalWrite(lMotorb, LOW);
    break;

  case REV:
    digitalWrite(lMotorf, LOW);
    digitalWrite(lMotorb, HIGH);
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

