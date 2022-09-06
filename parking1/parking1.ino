#include<Servo.h>
#include <SPI.h>
#include <Ethernet.h>
#include "NewPing.h"

#define TRIGGER_PIN 3 // Trigger and Echo both on pin 9
#define ECHO_PIN 3

#define TRIGGER_PIN_1 5 // Trigger and Echo both on pin 3
#define ECHO_PIN_1 5

#define TRIGGER_PIN_2 6 // Trigger and Echo both on pin 12
#define ECHO_PIN_2 6

#define TRIGGER_PIN_3 7 // Trigger and Echo both on pin 11
#define ECHO_PIN_3 7

// Maximum distance we want to ping for (in centimeters).
#define MAX_DISTANCE 400  
// NewPing setup of pins and maximum distance.
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);
NewPing sonar2(TRIGGER_PIN_1, ECHO_PIN_1, MAX_DISTANCE);
NewPing sonar3(TRIGGER_PIN_2, ECHO_PIN_2, MAX_DISTANCE);
NewPing sonar4(TRIGGER_PIN_3, ECHO_PIN_3, MAX_DISTANCE);

const unsigned long HTTP_TIMEOUT = 10000; //Maximum response from the server
const size_t MAX_CONTENT_SIZE = 512; //Max size of the HTTP response

byte mac[] = {0xAA, 0xBA, 0xCC, 0xCA, 0xDE, 0xED};

//IP Address of server
IPAddress serv(192, 168, 1, 126);

//IP address of the server
IPAddress ip(192, 168, 1, 120);
char server[] = "192.168.1.126";

//Instance of ethernet client
EthernetClient client;

bool car;
bool secondCar;
bool carEntering = false,carLeft,carLeaving;
bool sensor1, sensor2;
bool check1 = false;
// Number of slots in the parking
int number_of_slots=2;
//Distances of the sensors
int inches, inches2, inches3, inches4;
Servo servo;
struct vehicleData{
  char error[50];
 };

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

bool skipResponseHeaders(){
  char endOfHeaders[] = "\r\n";  

  client.setTimeout()
}

//Checking status of parking slots
void checkNewStatus(){
 int distance = sonar3.ping_cm();
 int distance2 = sonar4.ping_cm(); 
 
  if(distance < 5){
  sensor1 = true;
//  setSlotStatus(7, true);
  }else if(distance > 5){
  sensor1 = false;
  }
  
  if(distance2 < 5){
   sensor2 = true;
    setSlotStatus(9, "true");
  }else if(distance2 > 5){
   sensor2 = false;
    setSlotStatus(9, "false");
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

void makeConnection(){
   if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    Ethernet.begin(mac,ip);
    if (Ethernet.hardwareStatus() == EthernetNoHardware) {
      Serial.println("Ethernet shield was not found.  Sorry, can't run without hardware. :(");
    } else if (Ethernet.linkStatus() == LinkOFF) {
      Serial.println("Ethernet cable is not connected.");
    }
    while (true) {
      delay(1);
    }
  }
Ethernet.begin(mac,ip);
Serial.println(Ethernet.localIP()); 
check1 = true;
 }

void setup() {
  Serial.begin(9600); 
   Serial.println("Configuring");
   //Configuring the mac with the Ethernet
   
   servo.attach(9);
   servo.write(180);  
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

void setSlotStatus(int id, char stat[]){
  if(check1 == false){
    makeConnection();
    }
delay(100);
if (client.connect(server,8000)) { //Connecting at the IP address and port we saved before
Serial.println("connected");
client.print("PUT /slots/updateStatus?"); //Connecting and Sending values to database
client.print("id=");
client.print(id);
client.print("&status=");
client.print(stat);
Serial.println(stat);
client.println(" HTTP/1.1");
client.print("Host: ");
client.println(server);
Serial.print(server);
Serial.println("sdgdf");
client.println("Connection: close");
client.println("Content-Type: application/json");
client.println("Content-Length: 12\r\n");
client.print("{\"on\":false}");

client.stop(); //Closing the connection
}else {
// if you didn’t get a connection to the server:
Serial.println("Error");
}
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
    servo.write(100);
    car = true;
   }else if(inches2 > 5 && secondCar == true){
   Serial.println("New position");
   carEntering = true;
  servo.write(180);
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
