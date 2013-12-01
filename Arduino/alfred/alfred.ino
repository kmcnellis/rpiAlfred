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

#define BLUETOOTH_UPDATE_TIME 100 // Time between bluetooth messages

const int MODE = DARK_ON_LIGHT;

int closeLeftValues;
int closeRightValues;
int farLeftValues;
int farRightValues;
int centerValues;

int index = 0;
int closeLeft_average=0,closeRight_average=0,farLeft_average=0,farRight_average=0,center_average=0;
int closeLeft_total=0, closeRight_total=0, farLeft_total=0, farRight_total=0, center_total;

int t_speed=255;
//Contains intial values
int initCloseRight=0;
int initCloseLeft=0;
int initFarRight=0;
int initFarLeft=0;
int initCenter=0;

int currentMotorState = STOP;

// Time (in milliseconds since the programs start) at which the last bluetooth message was sent
int lastSendTime = 0;

#define sensorSpeed 10   
unsigned long sensorTimer;  
const int numSensor = LENGTH_CACHE-1;// number of values in array


boolean debugMode = true;
boolean dispensing = false;

//function definitions
boolean initial;
boolean turnModeR;
boolean turnModeL;
void read_ldrs();
void average_func();
void check_values();
void turn(int);
void total_change();
void start_line();


void debugPrintln(String string){
  if (debugMode){
    Serial.println(string);
  }
}
void debugPrint(String string){
  if (debugMode){
    Serial.print(string);
  }
}

void debugPrintln(int integer){
  if (debugMode){
    Serial.println(integer);
  }
}
void debugPrint(int integer){
  if (debugMode){
    Serial.print(integer);
  }
}

// Send bluetooth message to android device
void bluetoothWrite(int motorState, int cupLevel){
  if (!debugMode){
    // Send bluetooth device message
    Serial.println(String(motorState) + "," + String(cupLevel));
    Serial.flush();
  }
}

// Read the LAST message sent from the android and interpret
void bluetoothRead(){

  // Fill line variable with serial contents
  // Reset line if there is a newline character (so it only interprets the last message sent)

  String line = "";
  char character;

  while (Serial.available()){
    character = Serial.read();
    if (character == '\n'){
      line = "";
    }else{
      line.concat(character);
    }
  }

  // Interpret contents of last message

  // At the moment, there are only two possible cases
  // "0" => Not Dispensing (Move)
  // "1" => Dispensing (Don't Move)

  if (line != ""){

    int mode = line.toInt();

    switch(mode){
      case 0:
      dispensing = false;
      break;
      case 1:
      dispensing = true;
      currentMotorState = STOP;
      break;
    }

  }

}

void setup()
{
  Serial.begin(9600);  //Begin serial communcation
  pinMode(rMotorf,OUTPUT);
  pinMode(rMotorb,OUTPUT);
  pinMode(lMotorf,OUTPUT);
  pinMode(lMotorb,OUTPUT);
  pinMode(LED, OUTPUT);
  initial=false;
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
  center_average=0;
  
  closeLeft_total=0; 
  closeRight_total=0; 
  farLeft_total=0; 
  farRight_total=0;
  center_total=0;

  sensorTimer = millis()+100;
}

void loop()
{ 
  // Send a bluetooth update every BLUETOOTH_UPDATE_TIME milliseconds 
  if (millis() - lastSendTime >= BLUETOOTH_UPDATE_TIME){
    bluetoothWrite(currentMotorState,0); // TODO replace 0 with dispensing sensor state
    lastSendTime = millis();
  }

  bluetoothRead();

  if (dispensing){



  }else{
    
    // Non-dispensing Mode (Alfred Moves)


    if (millis() >= sensorTimer) {   // pingSpeed milliseconds since last ping, do another ping.
      sensorTimer += sensorSpeed;  
      digitalWrite(LED, HIGH);
      read_ldrs();
      if (initial==false){
        debugPrint(closeLeftValues);
        debugPrint(" , ");
        debugPrint(closeRightValues);
        debugPrint(" | ");

      }
      
      if (index==numSensor && initial==true){// initial = true means it was intialized index==numSensor
        check_values();
        closeLeft_total=0; 
        closeRight_total=0; 
        farLeft_total=0; 
        farRight_total=0;
        center_total = 0;
      }
      else if(index==numSensor && initial==false){
        
        average_func();
        
        initCloseLeft= closeLeft_average;
        initCloseRight= closeRight_average;
        initFarLeft=farLeft_average;
        initFarRight= farRight_average;
        initCenter = center_average;
        
        
        debugPrint(" = ");
        debugPrintln(closeLeft_average);
        
        closeLeft_total=0; 
        closeRight_total=0; 
        farLeft_total=0; 
        farRight_total=0;
        initial=true;
      }
      index += 1;
      index=index%(numSensor+1);
    }

  }

}

void read_ldrs()
{
  closeLeftValues = analogRead(CLOSELEFT);
  closeRightValues = analogRead(CLOSERIGHT);
  farLeftValues = analogRead(FARLEFT);
  farRightValues = analogRead(FARRIGHT);
  //debugPrint(closeLeftValues);
  //debugPrint("|");
  
  centerValues = analogRead(CENTER);
  center_total += centerValues;

  closeLeft_total+=closeLeftValues;
  closeRight_total+=closeRightValues;
  farLeft_total+=farLeftValues;
  farRight_total+=farRightValues;
  return;
}

void check_values()
{
  average_func();

  int diffLeft=(closeLeft_average-initCloseLeft);
  int diffRight=(closeRight_average -initCloseRight);
  int diffLeftFar=(farLeft_average-initFarLeft);
  int diffRightFar=(farRight_average -initFarRight);
  int diffCenter=(center_average -initCenter);

/*
  debugPrint(diffLeft);
  debugPrint("|");
  debugPrint(diffRight);
  debugPrint("|");
  debugPrint(closeLeft_average);
  debugPrint("|");
  debugPrint(closeRight_average);
  debugPrint("|");
  debugPrint(initCloseLeft);
  debugPrint("|");
  debugPrintln(initCloseRight);
*/
  if((diffLeftFar-diffCenter)>DIFFERENCE)
  {
    turnModeL=true;
    turnModeR=false;
    turn(F_RIGHT);
  }
  else if(abs(diffRightFar-diffCenter)>DIFFERENCE)
  {
    turnModeR=true;
    turnModeL=false;
    turn(F_LEFT);

  }
  else if(abs(diffLeft)>abs(diffRight) && abs(diffLeft-diffCenter)>DIFFERENCE)//this won't work for gradual/slow changes
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
  else if (abs(diffRight)>abs(diffLeft) && abs(diffRight-diffCenter)>DIFFERENCE)
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
  center_average = center_total/LENGTH_CACHE;

  return;
}

void turn(int turn_signal) {
  int rmotor = 0;
  int lmotor = 0;
  int slowSpeed = t_speed/2;
  currentMotorState = turn_signal;
  switch (turn_signal){
  case RIGHT:
    debugPrintln("Turn Right");
    rmotor = GO;
    lmotor = STILL;
    break;

  case LEFT:
    debugPrintln("Turn Left");
    rmotor = STILL;
    lmotor = GO;
    break;

  case FORWARD:
    debugPrintln("Go Forward");
    rmotor = GO;
    lmotor = GO;
    break;

  case BACK:
    debugPrintln("Go Backwards");
    rmotor = REV;
    lmotor = REV;
    break;

  case STOP:
    debugPrintln("Stop");
    rmotor = STILL;
    lmotor = STILL;
    break;

  case ROTATE_RIGHT:
    debugPrintln("Rotate Right");
    rmotor = REV;
    lmotor = GO;
    break;

  case ROTATE_LEFT:
    debugPrintln("Rotate Left");
    rmotor = GO;
    lmotor = REV;
    break;

  case ROTATE_STOP:
    debugPrintln("Stop");
    rmotor = STILL;
    lmotor = STILL;
    break;

  case F_RIGHT:
    debugPrintln("Turn Sharp Right");
    rmotor = GO;
    lmotor = SLOW_GO;
    break;

  case F_LEFT:
    debugPrintln("Turn Sharp Left");
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
