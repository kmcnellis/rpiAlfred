#define S0 A2
#define S1 A1
#define S2 A3
#define S3 A0
#define S4 A4
#define LED 12

#define DIFFERENCE 15
#define NUMSENSOR 5
// rotation signals
#define ROTATE_RIGHT 0
#define ROTATE_LEFT 1
#define ROTATE_STOP 2

//motor control
#define GO 1
#define STILL 0
#define REV 2
#define SLOW_GO 3
#define SLOW_REV 4
#define MOVE 12
// directions
#define RIGHT 5
#define LEFT 6
#define FORWARD 7
#define BACK 8
#define STOP 9
#define F_RIGHT 10
#define F_LEFT 11

// motor pins
#define rMotorf 3
#define rMotorb 5
#define lMotorf 6
#define lMotorb 9

#define DARK_ON_LIGHT  1
#define LIGHT_ON_DARK  0

const int MODE = DARK_ON_LIGHT;
int t_speed=255;

#define sensorSpeed 10
unsigned long sensorTimer;

void read_ldrs();
int check_values();
int PID();
void turn(int, int);
bool sensor[NUMSENSOR];
int initial[NUMSENSOR]={0};
int currError=0;
int prevError=0;
int integral;
int Ki, Kd, Kp;

void setup()
{
    Serial.begin(9600);  //Begin serial communcation
    pinMode(S0,OUTPUT);
    pinMode(S1,OUTPUT);
    pinMode(S2,OUTPUT);
    pinMode(S3,OUTPUT);
    pinMode(S4,OUTPUT);
    pinMode(LED, OUTPUT);
    for (int a=0; a<NUMSENSOR; a++) {
        initial[a]=MODE*255;
    }
    currError=0;
    prevError=0;
    integral=0;
    Ki=0;
    Kd=0;
    Kp=1;
    sensorTimer = millis()+100;
}

void loop()
{
    if (millis() >= sensorTimer) {   // pingSpeed milliseconds since last ping, do another ping.
        sensorTimer += sensorSpeed;
        digitalWrite(LED, HIGH);
        read_ldrs();
        turn(MOVE,PID());
        
    }
}

void read_ldrs()
{
    int sensorRead[NUMSENSOR];
    
    sensorRead[0]=analogRead(S0);
    sensorRead[1]=analogRead(S1);
    sensorRead[2]=analogRead(S2);
    sensorRead[3]=analogRead(S3);
    sensorRead[4]=analogRead(S4);
    
    for (int a=0; a<NUMSENSOR; a++) {
        if (MODE==LIGHT_ON_DARK) {
            if (sensorRead[a]=initial[0]+DIFFERENCE) {
                sensor[a]=true;
            }
            else{
                sensor[a]=false;
            }
        }
        else {
            if (sensorRead[a]=initial[0]-DIFFERENCE) {
                sensor[a]=true;
            }
            else{
                sensor[a]=false;
            }
        }
    }
    return;
}

int check_values()
{
    int error=0;
    // 0 0 0 0 0 | 0
    // 0 0 0 0 1 | 4
    // 0 0 0 1 0 | 2
    // 0 0 1 0 0 | 0
    // 0 1 0 0 0 | -2
    // 1 0 0 0 0 | -4
    
    // 0 0 0 1 1 | 3
    // 0 0 1 1 0 | 1
    // 0 1 1 0 0 | -1
    // 1 1 0 0 0 | -3
    
    // 0 0 1 1 1 | 5
    // 0 1 1 1 0 | 0
    // 1 1 1 0 0 | -5
    
    // 0 1 1 1 1 | 5
    // 1 1 1 1 0 | -5
    // 1 1 1 1 1 | 5
    
    if (!sensor[0] && !sensor[1] && !sensor[2] && !sensor[3] && !sensor[4]) {
        error= 0;
    }
    else if (!sensor[0] && !sensor[1] && !sensor[2] && !sensor[3] && sensor[4]) {
        error= 4;
    }
    else if (!sensor[0] && !sensor[1] && !sensor[2] && sensor[3] && !sensor[4]) {
        error= 2;
    }
    else if (!sensor[0] && !sensor[1] && sensor[2] && !sensor[3] && !sensor[4]) {
        error= 0;
    }
    else if (!sensor[0] && sensor[1] && !sensor[2] && !sensor[3] && !sensor[4]) {
        error= -2;
    }
    else if (sensor[0] && !sensor[1] && !sensor[2] && !sensor[3] && !sensor[4]) {
        error= -4;
    }
    
    
    else if (!sensor[0] && !sensor[1] && !sensor[2] && sensor[3] && sensor[4]) {
        error= 3;
    }
    else if (!sensor[0] && !sensor[1] && sensor[2] && sensor[3] && !sensor[4]) {
        error= 1;
    }
    else if (!sensor[0] && sensor[1] && sensor[2] && !sensor[3] && !sensor[4]) {
        error= -1;
    }
    else if (sensor[0] && sensor[1] && !sensor[2] && !sensor[3] && !sensor[4]) {
        error= -3;
    }
    
    
    else if (!sensor[0] && !sensor[1] && sensor[2] && sensor[3] && sensor[4]) {
        error= 5;
    }
    else if (sensor[0] && sensor[1] && sensor[2] && sensor[3] && !sensor[4]) {
        error= -5;
    }
    
    
    else if (sensor[0] && sensor[1] && sensor[2] && sensor[3] && sensor[4]) {
        error= 5;
    }
    
    else{
        error=0;
    }
    return error;
}

int PID(){
    currError=check_values();
    int diff=currError-prevError;
    integral+=currError;
    prevError=currError;
    int power= Kp*currError + Ki*integral + Kd*diff;
    return power;
}
void turn(int turn_signal, int power=255) {
    int rmotor = 0;
    int lmotor = 0;
    int slowSpeed = t_speed/2;
    switch (turn_signal){
        case MOVE:
            Serial.println("Turn PID");
            Serial.println(power);
            rmotor=MOVE;
            lmotor=MOVE;
            break;
            
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
        case MOVE:
            if (power<0) {
                analogWrite(rMotorf, t_speed);
                digitalWrite(rMotorb, LOW);
            }
            else if (power>0) {
                analogWrite(rMotorf, t_speed-power);
                digitalWrite(rMotorb, LOW);
            }
            else {
                analogWrite(rMotorf, t_speed);
                digitalWrite(rMotorb, LOW);
            }
            break;
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
        case MOVE:
            if (power<0) {
                analogWrite(lMotorf, t_speed+power);
                digitalWrite(lMotorb, LOW);
            }
            else if (power>0) {
                analogWrite(lMotorf, t_speed);
                digitalWrite(lMotorb, LOW);
            }
            else {
                analogWrite(lMotorf, t_speed);
                digitalWrite(lMotorb, LOW);
            }
            break;
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
