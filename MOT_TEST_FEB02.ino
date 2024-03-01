#include<MobaTools.h>
const byte stepPin =18;
const byte dirPin =17;

#define limitPin A11
#define startPin 14

const int stepsperRev = 1000;


long  nextPos;
const long targetPos =2000;

int count = 0;
int limit_count =0;

bool limswitch =0;

int hOme =0;
int pos =0;
int sTart =0;

MoToStepper stepper1(stepsperRev, STEPDIR);
MoToTimer stepperPause;

bool stepperRunning;

void setup() {
  Serial.begin(9600);
  stepper1.attach(stepPin,dirPin);
  stepper1.setRampLen(100);

  pinMode(8,OUTPUT);

  pinMode(limitPin, INPUT_PULLUP);
  pinMode(startPin, INPUT_PULLUP);
  homing();
  set_pos();
  stepperRunning = true;
}

void loop() {
    while(digitalRead(startPin)==LOW)
    {sTart =1;}

    
    if((hOme ==1)&& (limit_count>0)&&(count ==0)&&(pos=1)&& (sTart ==1))
  {
    stepper1.setSpeed(200);
    motor();
  }
}

void homing() {
  stepper1.setSpeed(800);
  while(digitalRead(limitPin) != HIGH && limit_count==0) { stepper1.rotate(1);Serial.print(limswitch);Serial.println("Finding END");}

  if(digitalRead(limitPin) == HIGH && limit_count ==0) {stepper1.stop();Serial.println("END POSITION");limit_count+=1;limswitch=HIGH;hOme =1;}
  Serial.print("limit_count:");Serial.println(limit_count);
  Serial.print("limswitch:");Serial.println(limswitch);
  Serial.print("hOme:");Serial.println(hOme);
  Serial.print("POS:");Serial.println(pos);
  delay(2000);
  }

void set_pos() {
  
  while(hOme ==1 && limit_count >0 && limswitch ==HIGH && pos ==0) { 
    stepper1.rotate(-1);
    delay(3000);
    stepper1.stop();
    stepper1.setZero();
    pos =1;
    Serial.println("StartPositionSet, READY TO USE");
    }
    Serial.print("POS:");Serial.println(pos);
}


void motor() {

  if ( stepperRunning ) {
    // Wait till stepper has reached target, then set pause time
    if ( !stepper1.moving() ) {
      // stepper has reached target, start pause
      stepperPause.setTime( 1000 );
      stepperRunning = false;
    }
  } else {
    // stepper doesn't move, wait till stepperPause time expires
    if ( stepperPause.expired() ) {
      // stepperPause time expired. Start stepper in opposite direction
      if ( nextPos == 0 ) {
        nextPos = targetPos;
      } else {
        nextPos = 0;
      }
      stepper1.moveTo(-nextPos );
      stepperRunning = true;
    }
  }
}
