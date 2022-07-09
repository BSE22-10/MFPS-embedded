#include<Servo.h>
int inches = 0;
int inches2 = 0;
int inches3 = 0;
int number_of_slots=1;
bool car;
bool secondCar;
int cm = 0;
int secondDistance = 0;

Servo servo;

long readUltrasonicDistance(int triggerPin, int echoPin)
{
  pinMode(triggerPin, OUTPUT); 
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2);
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);
  pinMode(echoPin, INPUT);
  return pulseIn(echoPin, HIGH);
}

void checkSlotStatus(){
 int distance = (0.01723 * readUltrasonicDistance(12, 12))/2.54;
  if(distance < 80 && number_of_slots > 0){
  number_of_slots --;
  }else if(distance > 80 && number_of_slots < 1){
  number_of_slots ++;
  }
}

void setup()
{
  Serial.begin(9600);
  servo.attach(10);
  servo.write(0);
}

void loop(){

  cm = 0.01723 * readUltrasonicDistance(7, 7);
  inches = (cm / 2.54);
   secondDistance = 0.01723 * readUltrasonicDistance(8, 8);
  inches2 = (secondDistance / 2.54);
if(number_of_slots > 0){
  if(inches < 80){
  servo.write(90);
   car = true;
  }else if(inches2 > 80 && secondCar == true){
   Serial.print("New position");
  servo.write(0);
  }else if(inches2 > 80 && car == true){
     Serial.println("Second car");
    servo.write(90);
  }else if(inches2 < 80 && car == true){
    Serial.println("We are here");
   secondCar = true;
  }else{
  servo.write(0);
  }
}else{
servo.write(0);
}
  checkSlotStatus();
  delay(10); 
}
