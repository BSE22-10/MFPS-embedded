#include "NewPing.h"
#include<Servo.h>

#define TRIGGER_PIN 3 // Trigger and Echo both on pin 9
#define ECHO_PIN 3

#define TRIGGER_PIN_1 5 // Trigger and Echo both on pin 3
#define ECHO_PIN_1 5

#define TRIGGER_PIN_2 12 // Trigger and Echo both on pin 12
#define ECHO_PIN_2 12

#define TRIGGER_PIN_3 11 // Trigger and Echo both on pin 11
#define ECHO_PIN_3 11

// Maximum distance we want to ping for (in centimeters).
#define MAX_DISTANCE 400  
// NewPing setup of pins and maximum distance.
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);
NewPing sonar2(TRIGGER_PIN_1, ECHO_PIN_1, MAX_DISTANCE);
NewPing sonar3(TRIGGER_PIN_2, ECHO_PIN_2, MAX_DISTANCE);
NewPing sonar4(TRIGGER_PIN_3, ECHO_PIN_3, MAX_DISTANCE);

bool car;
bool secondCar;
bool carEntering = false,carLeft,carLeaving;
bool sensor1, sensor2;
// Number of slots in the parking
int number_of_slots=2;
//Distances of the sensors
int inches, inches2, inches3, inches4;
Servo servo;

void checkSlotStatus(){
 int distance = sonar3.ping_cm();
 int distance2 = sonar4.ping_cm(); 
  if((distance < 5 && number_of_slots > 0)){
  number_of_slots --;
  }else if((distance > 5 && number_of_slots < 1)){
  number_of_slots ++;
  }else if((distance2 < 5 && number_of_slots > 0)){
   number_of_slots --;
  }else if((distance2 > 5 && number_of_slots < 1)){
   number_of_slots ++;
  }
}

void checkNewStatus(){
 int distance = sonar3.ping_cm();
 int distance2 = sonar4.ping_cm(); 
  if(distance < 5){
  sensor1 = true;
  }else if(distance > 5){
  sensor1 = false;
  }
  
  if(distance2 < 5){
   sensor2 = true;
  }else if(distance2 > 5){
   sensor2 = false;
  }
  }

void vehicleExiting(){
  if(inches2 < 80 &&carEntering == true){
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

void setup() {
  Serial.begin(9600);
   servo.attach(13);
   servo.write(0);  
}

int getDistance(int trigPin, int echoPin) // returns the distance (cm)
{
long duration, distance;

digitalWrite(trigPin, HIGH); // We send a 10us pulse
delayMicroseconds(10);
digitalWrite(trigPin, LOW);

duration = pulseIn(echoPin, HIGH, 20000); // We wait for the echo to come back, with a timeout of 20ms, which corresponds approximately to 3m

// pulseIn will only return 0 if it timed out. (or if echoPin was already to 1, but it should not happen)
if(duration == 0) // If we timed out
{
pinMode(echoPin, OUTPUT); // Then we set echo pin to output mode
digitalWrite(echoPin, LOW); // We send a LOW pulse to the echo pin
delayMicroseconds(200);
pinMode(echoPin, INPUT); // And finaly we come back to input mode
}

distance = (duration/2) / 29.1; // We calculate the distance (sound speed in air is aprox. 291m/s), /2 because of the pulse going and coming

return distance; //We return the result. Here you can find a 0 if we timed out
}

void loop() {
   inches = sonar.ping_cm();
   inches2 = sonar2.ping_cm();
   inches3 = sonar3.ping_cm();
   inches4 = sonar4.ping_cm();
   
   Serial.print("Distance = ");
  Serial.println(inches4);
//  Serial.println(getDistance(11,11));
  checkNewStatus();
 if(sensor2 == false){
   Serial.println("In the loop");
  if(inches <= 5){
    Serial.println("Car near");
    servo.write(90);
    car = true;
   }else if(inches2 > 5 && secondCar == true){
   Serial.println("New position");
   carEntering = true;
  servo.write(0);
   }else if(inches2 > 5 && car == true && carEntering == false){
     Serial.println("Second car");
    servo.write(90);
  }else if(inches2 < 5 && car == true && carEntering == false){
    Serial.println("We are here");
   secondCar = true;
  }else if(inches2 < 5){
  Serial.println("Car at the gate");
  servo.write(90);
  }else if(inches < 5){
  servo.write(90);
  }
 }else{
    if(inches2 < 5){
  carLeaving = true;
  Serial.println("Car exiting");
  servo.write(90);
  }else if(inches < 5 && carLeaving){
   Serial.println("Car has left");
  servo.write(0);
  }
  }
  delay(500);
}

