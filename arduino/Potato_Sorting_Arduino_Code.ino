#include <Servo.h>
Servo servo_1;                  //Naming our servo motor servo1
String sentData;               //Serial data sent to Arduino
#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_MS_PWMServoDriver.h"
int s;

Adafruit_MotorShield AFMS = Adafruit_MotorShield();
// Select which 'port' M1, M2, M3 or M4. In this case, M3 & link to variable (myMotor)
Adafruit_DCMotor *myMotor = AFMS.getMotor(1);


void setup() {
  Serial.begin(9600);           // set up Serial library at 9600 bps
  Serial.println("Start of Test");

  servo_1.attach(10);    //On Adafruit Motor shield v3.2, 'Servo 1' pins is #10
  AFMS.begin();                               //create with the default frequency 1.6KHz
  myMotor->setSpeed(255);                      //Set speed to start from 0(stop) to 255(max)
}

void loop() {
  if (Serial.available() > 0) {
    sentData = Serial.readStringUntil('\n');

    //Initiating the operation (Start DC and wait for class type)
    if (sentData == "Start") {                //Initiates operation
      //We need to think of something that will make the DC run continuously
      //Make go forward gradually
      myMotor->run(FORWARD);
      Serial.println("DC ramping up");
      for (s = 0; s < 255; s++) {         //This allows to make the speed increase gradually; from 0 to 150
        myMotor->setSpeed(s);             //Inputing the increment of the speed
        delay(50);                       //By changing the 'delay time' we change how gradual it is. small number = rapid change
      }
      Serial.println("DC Constant speed");   //We have now reached the desired speed.
      //delay(2000);

      //Read incoming data
      if (Serial.available() > 0) {
        sentData = Serial.readStringUntil('\n');  //Now we look for the class types or for a 'go back' instruction
        Serial.println("Am I reading");
      }
      }


      //Go back command
      if (sentData == "Reverse") {                //Tells us to go back; see if we write it as a string ('go back')
        Serial.println("DC slowing down");
        for (s = 255; s != 0; s--) {        //This allows to make the speed decrease gradually; from 150 to 0
          myMotor->setSpeed(s);             //Inputing the increment of the speed
          delay(50);
        }
        myMotor->run(RELEASE);            //Releasing the DC motor not to damage it while going backward
        delay(10);
        myMotor->run(BACKWARD);
        Serial.println("Going up, in Reverse");
        for (s = 0; s < 100; s++) {
          myMotor->setSpeed(s);
          delay(50);                      //We can change the delay to make the potato stop at the right place
        }

        //Now the potato has gone back, we need to make it stop near the camera
        //Stop the reverse command
        sentData = Serial.read();
        if (sentData == "Pause") {        //'90' makes the DC stop when it was going back.
          Serial.println("Pausing");
          myMotor->run(RELEASE);    //Would make it stop, do not know exactly where the potato will be at this point
          Serial.println("DC stopped, during pausing");
          delay(5000);
        }

        //Make the DC go forward again
        myMotor->run(FORWARD);
        Serial.println("DC ramping up, after the mess");
        for (s = 0; s < 255; s++) {         //This allows to make the speed increase gradually; from 0 to 150
          myMotor->setSpeed(s);             //Inputing the increment of the speed
          delay(50);                       //By changing the 'delay time' we change how gradual it is. small number = rapid change
        }
        Serial.println("DC Constant speed, after the mess");   //We have now reached the desired speed.
      }

      //Moving the servo commands
      if (sentData == "1") {                 //Class 1: smallest
        servo_1.write(60);                //36 deg. is first angle
        Serial.println("Rotating at Class 1 (36)");
        delay(1000);
      }
      if (sentData == "2") {                 //Class 2 mid 1
        servo_1.write(80);                //36 deg. is second angle
        Serial.println("Rotating at Class 2 (72)");
        delay(1000);
      }
      if (sentData == "3") {                 //Class 3: mid 2
        servo_1.write(100);                //108 deg. is third angle
        Serial.println("Rotating at Class 3 (108)");
        delay(1000);
      }
      if (sentData == "4") {                 //Class 4: largest
        servo_1.write(118);                //144 deg. is first angle
        Serial.println("Rotating at Class 4 (144)");
        delay(1000);
      }

      //Stop everything command
      if (sentData == "Stop") {                //'25' Tells us to STOP
        Serial.println("DC slowing down to Stop");
        for (s = 100; s != 0; s--) {        //This allows to make the speed decrease gradually; from 150 to 0
          myMotor->setSpeed(s);             //Inputing the increment of the speed
          delay(50);
        }
        myMotor->run(RELEASE);            //Releasing the DC motor not to damage it while going backward
        Serial.println("DC Full stop");
      }
    }
  }




//  Serial.println("Rotating initiated"); //Making the servo go CCW from into 4 positions, from 30 deg to 120 deg
//  for (i = 1; i <= 4; i++) {            //Making i go from 1 to 4
//    String S1 = "iteration = ";
//    String S2 = S1 + i;                 //Concatenating 2 strings
//    Serial.println(S2);                 //Printing on Serial Port
//    ang = map(i, 1, 4, 30, 120);        //Distributing range (1,4) onto range (30,120) => 1=30, 2=60, ..
//    String S3 = "angle = ";
//    String S4 = S3 + ang;               //Concatenating 2 strings
//    Serial.println(S4);                 //Printing on Serial Port
//    servo_1.write(ang);                  //Turning the servo to the desired angle
//    delay(1000);
//  }
//  Serial.println("Going back");         //Same procedure, now CW
//  for (i = 4; i != 1; i--) {            //Making i go from 4 to 1
//    String S1 = "iteration = ";
//    String S2 = S1 + i;
//    Serial.println(S2);
//    ang = map(i, 1, 4, 30, 120);
//    String S3 = "angle = ";
//    String S4 = S3 + ang;
//    Serial.println(S4);
//    servo_1.write(ang);
//    delay(1000);
//  }
//}
