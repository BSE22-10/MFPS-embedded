#include<Servo.h>
#include<Wire.h>
#include <Adafruit_LiquidCrystal.h>

#define LCD output
Adafruit_LiquidCrystal lcd_1(0);

int inches = 0;
int inches2 = 0;
int inches3 = 0;
int inches4 = 0;
int number_of_slots=2;
bool car;
bool secondCar;
bool carEntering = false,carLeft,carLeaving;
int cm = 0;
int secondDistance = 0;
char message[50];

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
 int distance2 = (0.01723 * readUltrasonicDistance(13, 13))/2.54; 
  if((distance < 80 && number_of_slots > 0)){
  number_of_slots --;
  }else if((distance > 80 && number_of_slots < 1)){
  number_of_slots ++;
  }else if((distance2 < 80 && number_of_slots > 0)){
   number_of_slots --;
  }else if((distance2 > 80 && number_of_slots < 1)){
   number_of_slots ++;
  }
}

void vehicleExiting(){
  if(inches2 <80 &&carEntering == true){
    Serial.print("Car leaving now");
    car = false;
    servo.write(90);
  }
  else if((inches < 80 || inches2 > 80) && carEntering == true){
    Serial.print("Car leaving");
    carLeft = true;
    servo.write(90);
  }else if(carLeft == true && inches > 80){
    servo.write(0);
  }
}

void setup()
{
  lcd_1.begin(16, 2);

  lcd_1.print("hello world");
  
  Serial.begin(9600);
  servo.attach(10);
  servo.write(0);  
}

void loop(){

  cm = 0.01723 * readUltrasonicDistance(7, 7);
  inches = (cm / 2.54);
   secondDistance = 0.01723 * readUltrasonicDistance(8, 8);
  inches2 = (secondDistance / 2.54);
    checkSlotStatus();
  //vehicleExiting();
if(number_of_slots > 0){
  lcd_1.clear();
  sprintf(message,"%d slot", number_of_slots);
  lcd_1.print(message);
  if(inches < 80){
  servo.write(90);
   car = true;
  }else if(inches2 > 80 && secondCar == true){
   Serial.println("New position");
   carEntering = true;
  servo.write(0);
  }else if(inches2 > 80 && car == true && carEntering == false){
     Serial.println("Second car");
    servo.write(90);
  }else if(inches2 < 80 && car == true && carEntering == false){
    Serial.println("We are here");
   secondCar = true;
  }else if(inches2 < 80){
  Serial.println("Car at the gate");
  servo.write(90);
  }else if(inches < 80){
  servo.write(90);
  }
}else{
Serial.println("Else statement");
lcd_1.clear();
lcd_1.print("Parking full");
  if(inches2 < 80){
  carLeaving = true;
  Serial.println("Car at the gate");
  servo.write(90);
  }else if(inches < 80 && carLeaving){
  servo.write(0);
  }
}

  delay(5); 
}
