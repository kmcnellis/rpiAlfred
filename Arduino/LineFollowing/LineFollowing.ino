#define FRONTRIGHT A0
#define FRONTLEFT A1
#define BACKRIGHT A2
#define BACKLEFT A3
#define MOTOR1 2
#define MOTOR2 3
#define LENGTH_CACHE 20
#define DIFFERENCE 10
const int DARK_ON_LIGHT = 0;
const int LIGHT_ON_DARK = 1;
const int MODE = DARK_ON_LIGHT;

int front_leftvalues[LENGTH_CACHE];
int front_rightvalues[LENGTH_CACHE];
int back_leftvalues[LENGTH_CACHE];
int back_rightvalues[LENGTH_CACHE];
int index = 0;
int fl_average,fr_average,bl_average,br_average;

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
  if (index = 20)
  {
    index =0;
  }
}

void check_values()
{
  average_func();
  if(abs(front_leftvalues[index-1]-fl_average)>DIFFERENCE)
  {
    //Turn Right
    Serial.println("Right");
  }
  else if(abs(front_rightvalues[index-1] -fr_average)>DIFFERENCE)
  {
    //Turn LEFT
    Serial.println("Left");
  }
}

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
  delay(100);
  check_values();
}
