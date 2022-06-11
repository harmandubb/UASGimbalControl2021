#include <Servo.h>
#define SERVO_PIN 9
#define SERVO_PIN2 10
#define GROUND_JOY_PIN A3            //joystick ground pin will connect to Arduino analog pin A3
#define VOUT_JOY_PIN A2              //joystick +5 V pin will connect to Arduino analog pin A2
#define XJOY_PIN A1          //X axis reading from joystick will go into analog pin A1
#define YJOY_PIN A0
//Servo myservo ;
//Servo myservo2;
#include "ServoEasing.hpp"
ServoEasing Servotest1;
ServoEasing Servotest2;
int moveflag = 0;
int msgflag = 0;
int xangle = 90;
int zangle = 90;
int currentXangle = xangle;
int currentZangle = zangle;

int speedd=45;
int xdirection=0;
int zdirection=0;
int xspeed = 90;
int zspeed = 90;
int x_raw;
int z_raw;
int angleIncrement = 1;
int startAngleIncrement = 5; 
int upperLimit = 180;
int lowerLimit = 0;
unsigned long startTime = 0;
int timerFlag = 0;
int firstMoveFlag = 0;

void setup()
{
 Serial.begin(115200);
 pinMode(VOUT_JOY_PIN, OUTPUT) ;    //pin A3 shall be used as output
 pinMode(GROUND_JOY_PIN, OUTPUT) ;  //pin A2 shall be used as output
 digitalWrite(VOUT_JOY_PIN, HIGH) ; //set pin A3 to high (+5V)
 digitalWrite(GROUND_JOY_PIN,LOW) ; //set pin A3 to low (ground)
 //myservo.attach(9);
 //myservo2.attach(10);
 
 Servotest1.attach(9, 60);
 Servotest2.attach(10, 60);
 Serial.setTimeout(25);
}

String rx_msg = "";
String prev_msg = "";
String initialmsg = "move";


 
void loop()
{
  if (Serial.available() > 0) {
   rx_msg = Serial.readString();
   if (rx_msg == "UAS")
   {
    Serial.println("UAS");
   }
   else {
    moveflag = 0;

    //check if the messgae is received 
    
  if (rx_msg.startsWith(initialmsg)){
    Serial.print("Now Moving\n");
    msgflag = 1;
    };
    
  if (msgflag==1){
    int indl1 = rx_msg.indexOf('_');
    int indl2= rx_msg.indexOf('_', indl1+1);
    int indl3 = rx_msg.indexOf('\0');
    String msg1 = rx_msg.substring(indl1+1, indl2);
    String msg2 = rx_msg.substring(indl2+1,indl3);
    z_raw = msg1.toInt();
    x_raw = msg2.toInt();
    Serial.print("Angle 1= ");
    Serial.print(x_raw);
    Serial.print("\nAngle 2= ");
    Serial.print(z_raw);

    xspeed = map(x_raw,-100,100,-400,400);
    zspeed = map(z_raw,-100,100,-400,400);
    Serial.print("\nxspeed = ");
    Serial.print(xspeed);
    Serial.print("\nzspeed = ");
    Serial.println(zspeed);
    
    if(xspeed < 0){
      xdirection = -startAngleIncrement;}
    else if(xspeed == 0){
      xdirection = 0;}
    else{
      xdirection = startAngleIncrement;}

    if(zspeed < 0){
      zdirection = -startAngleIncrement;}
    else if(zspeed == 0){
      zdirection = 0;}
    else{
      zdirection = startAngleIncrement;   
    } 
    firstMoveFlag = 1;
    msgflag = 0;
    moveflag = 1;
    
    timerFlag = 1;
    }
  }
  }
  //Serial.println("At the move");
  if(moveflag){
    if(currentXangle + xdirection > upperLimit){
      currentXangle = upperLimit;
    } 
    else if(currentXangle + xdirection < lowerLimit) {
      currentXangle = lowerLimit;
    } else {
      currentXangle = xdirection + currentXangle;
    }
  
    if(currentZangle + zdirection > upperLimit){
      currentZangle = upperLimit;
    } 
    else if(currentZangle + zdirection < lowerLimit) {
      currentZangle = lowerLimit;
    } else {
      currentZangle = zdirection + currentZangle;
    }

    unsigned long startTime = micros();
    Servotest1.easeTo(currentXangle, abs(xspeed));
    //Servotest1.write(currentXangle);
/*
    if (timerFlag){
      unsigned long endTime = micros();
      unsigned long delta = endTime - startTime; 
      Serial.print("Parse Time = ");
      Serial.println(delta);
      timerFlag = 0;
     }
     */
    
     Servotest2.easeTo(currentZangle, abs(zspeed));
     //Servotest2.write(currentZangle);
     
 
    
    

    if(firstMoveFlag == 1){
      if(xspeed < 0){
      xdirection = -angleIncrement;}
    else if(xspeed == 0){
      xdirection = 0;}
    else{
      xdirection = angleIncrement;}

    if(zspeed < 0){
      zdirection = -angleIncrement;}
    else if(zspeed == 0){
      zdirection = 0;}
    else{
      zdirection = angleIncrement;   
    } 
    firstMoveFlag = 0;
    }
    
                 
  }
  

 
}
