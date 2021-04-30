/***********************************************************************
  CSCI 4287 Final Project
  Name: Amanuel Chukala
  Intructor: Dr. Salim Lakhani
  Description: The robot car software is based on mainly: DC motor,
  Servo motor, Bluetooth module, and Ultrasonic sensor module.
  The software has the following features:
    => Maneuver the car in any of the four directions: forward,
       backward, right and left
    => Lets the car drive itself by avoiding obstacle/s
    => The ability to control the car using bluetooth
***********************************************************************/

#include <Servo.h>

// define global variables
int pos;
Servo Myservo;
const int trigPin = A5; // a variable for trigger pin
const int echoPin = A4; // a variable for echo pin

float rightDistance = 0.0, leftDistance = 0.0, frontDistance = 0.0;

// define constants
#define LEFTMOTORPIN1 7 // pin for IN1
#define LEFTMOTORPIN2 8 // pin for IN2
#define RIGHTMOTORPIN1 9 // pin for IN3
#define RIGHTMOTORPIN2 11 // pin for IN4
#define ENA 5 // controls the speed of left motor
#define ENB 6 // controls the speed of right motor

// define the routines
void driveForward()
{
  // setup the speed of the car
  analogWrite(ENA, 130);
  analogWrite(ENB, 130);

  digitalWrite(LEFTMOTORPIN1, HIGH);
  digitalWrite(LEFTMOTORPIN2, LOW);
  digitalWrite(RIGHTMOTORPIN1, LOW);
  digitalWrite(RIGHTMOTORPIN2, HIGH);

}
void driveBackward()
{
  // setup the speed of the car
  analogWrite(ENA, 130);
  analogWrite(ENB, 130);
  digitalWrite(LEFTMOTORPIN1, LOW);
  digitalWrite(LEFTMOTORPIN2, HIGH);
  digitalWrite(RIGHTMOTORPIN1, HIGH);
  digitalWrite(RIGHTMOTORPIN2, LOW);
}
void driveRight()
{
  // setup the speed of the car
  analogWrite(ENA, 150);
  analogWrite(ENB, 150);
  digitalWrite(LEFTMOTORPIN1, HIGH);
  digitalWrite(LEFTMOTORPIN2, LOW);
  digitalWrite(RIGHTMOTORPIN1, HIGH);
  digitalWrite(RIGHTMOTORPIN2, LOW);
}
void driveLeft()
{
  // setup the speed of the car
  analogWrite(ENA, 150);
  analogWrite(ENB, 150);
  digitalWrite(LEFTMOTORPIN1, LOW);
  digitalWrite(LEFTMOTORPIN2, HIGH);
  digitalWrite(RIGHTMOTORPIN1, LOW);
  digitalWrite(RIGHTMOTORPIN2, HIGH);
}

void setup()
{
  // setup the pins mode
  Serial.begin(9600);
  pinMode(LEFTMOTORPIN1, OUTPUT);
  pinMode(LEFTMOTORPIN2, OUTPUT);
  pinMode(RIGHTMOTORPIN1, OUTPUT);
  pinMode(RIGHTMOTORPIN2, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  stopDriving();

  // set the signal pin of servo motor to arduino pin
  Myservo.attach(3, 700, 2400); // attached to pin=3, min-angle=700 and max-angle=2400

  // set up the ultrasonic sensor
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

void stopDriving()
{
  digitalWrite(ENA, LOW);
  digitalWrite(ENB, LOW);
}

int lookForObstacle()
{
  float duration, distance; // variables for holding the length of sound wave and how far away the object is
  // set the trigger pin to low for 2 microseconds at first
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  // set the trigger pin to high for 10 microseconds which will send out 8 cycle sonic burst
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);

  // set the trigger pin to low to start receiving the bounced wave
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH); // read for how long the echo pin has stayed high

  distance = (duration * 0.0343) / 2; // speed of sound in centimeters per microseconds is .0343 multiplied by duration will give us the total distance but we divide by 2 since it's back and forth

  return distance;
}

void loop() {
  //  if (Serial.available())
  //  {
  //    char getstr = Serial.read();
  //    switch (getstr) {
  //      case 'f': driveForward(); break;
  //      case 'b': driveBackward();   break;
  //      case 'l': driveLeft();   break;
  //      case 'r': driveRight();  break;
  //      case 's': stopDriving();   break;
  //      default:  break;
  //    }
  //  }

  Myservo.write(90);  //setservo position according to scaled value
  delay(500);
  // get distance of any object, if any,
  frontDistance = lookForObstacle();

  if (frontDistance <= 40)
  {
    stopDriving();
    delay(500);
    Myservo.write(10);
    delay(1000);
    rightDistance = lookForObstacle();

    delay(500);
    Myservo.write(90);
    delay(1000);
    Myservo.write(180);
    delay(1000);
    leftDistance = lookForObstacle();

    delay(500);
    Myservo.write(90);
    delay(1000);
    if (rightDistance > leftDistance) {
      driveRight();
      delay(360);
    }
    else if (rightDistance < leftDistance) {
      driveLeft();
      delay(360);
    }
    else if ((rightDistance <= 40) || (leftDistance <= 40)) {
      driveBackward();
      delay(180);
    }
    else {
      driveForward();
    }
  }
  else {
    driveForward();
  }
}
