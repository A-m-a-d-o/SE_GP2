
#define TRIGER 2
#define ECHO 4
#define time_thr 5E3
long passes=0;
int diferent = 0;
float rotacoes = 0, rpm = 0;
bool time_set = false, same = false;
unsigned long time_before = 0, time_passed = 0;


void setup() {
   
 pinMode(TRIGER,OUTPUT);
 pinMode(ECHO,INPUT);
 Serial.begin(9600);

}

void loop() {


  if (time_set == false)
  {
    passes = 0;
    time_before = millis();
    
    time_set = true;
  }
  else if ( ( time_passed = (millis() - time_before)) >= time_thr )
  {
   
    rotacoes = passes / 3;
    rpm = ((rotacoes * 60) / time_passed);
    
    time_set = false;
        Serial.println(rpm); 
  }
  //Serial.println(time_passed);

  digitalWrite(TRIGER,LOW);
  delayMicroseconds(2);
  digitalWrite(TRIGER,HIGH);
  delayMicroseconds(10);
   

  long x =pulseIn(ECHO,HIGH);
  long inches =   x / 74 /2;
  long cm = x /29/2;
 
 //Serial.print(cm);
 //Serial.println("cm");
 //Serial.println(passes);
//Serial.println(same);
  if (cm < 4 )
  {
    if(same == false)
    {
      passes++;
      same = true;
    }
    
  }
  else
  {
    
    diferent ++;
    if (diferent > 5)
    {
    diferent = 0;
    same = false;
    }
  }

 

}
