
/***************************************************************************
 *ENGELDEN KACAN ROBOT
 *ABDULLAH KAAN 
***************************************************************************/

// TANIMLAMALAR   -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#include <AFMotor.h> 
#include <Servo.h>              
#include <NewPing.h> 
#include <SoftwareSerial.h>

#define TRIG_PIN A8 // ultrasonik trig a8
#define ECHO_PIN A9 // ultrasonik echo a9
#define MAX_DISTANCE 290 // maksimum ultrasonik mesafesi 290
#define MAX_SPEED 210 // DC çıkış 210
#define MAX_SPEED_OFFSET 40 // iki motor arası ofset 40
#define COLL_DIST 30 // dönüş mesafesi 30
#define TURN_DIST COLL_DIST+20 // ne kadar uzaklaştığını algılar

NewPing sonar(TRIG_PIN, ECHO_PIN, MAX_DISTANCE); // sensör kütüphanesini ayarlama
AF_DCMotor leftMotor1(4, MOTOR34_1KHZ); // sol motor m4 e bağlanır
AF_DCMotor rightMotor1(3, MOTOR34_1KHZ);// sağ motor m3 e bağlanır
Servo myservo;  // bir servoyu kontrol etmek için servo nesnesi yarat 

int leftDistance, rightDistance; // her iki taraftaki mesafeleri
int curDist = 0;
String motorSet = "";
int speedSet = 0;


void setup() { 
  myservo.attach(10); // SERVO10 shielddaki 1
  myservo.write(90); 
  delay(1000);
  Serial.begin(9600);
}

void loop() {
  myservo.write(90); 
  delay(90);
  curDist = readPing();  
  if (curDist < COLL_DIST) {changePath();} 
  moveForward(); 
  delay(500);
 }

void changePath() { 
  moveStop(); 
  myservo.write(36); 
    delay(500);
    rightDistance = readPing();
    delay(500);
    myservo.write(144); 
    delay(700);
    leftDistance = readPing();
    delay(500);
    myservo.write(90); 
    delay(100);
    compareDistance();
  }

  
void compareDistance()   
{
  if (leftDistance>rightDistance) 
  {
    turnLeft();
  }
  else if (rightDistance>leftDistance) 
  {
    turnRight();
  }
   else 
  {
    turnAround();
  }
}


int readPing() { 
  delay(70);   
  unsigned int uS = sonar.ping();
  int cm = uS/US_ROUNDTRIP_CM;
  return cm;
}
void moveStop() {leftMotor1.run(RELEASE); rightMotor1.run(RELEASE);}  

void moveForward() { 
    motorSet = "FORWARD";
    leftMotor1.run(FORWARD);    
    rightMotor1.run(FORWARD);
  for (speedSet = 0; speedSet < MAX_SPEED; speedSet +=2) 
  {
    leftMotor1.setSpeed(speedSet);
    rightMotor1.setSpeed(speedSet); 
    delay(5);
  }
}

void moveBackward() { 
    motorSet = "BACKWARD";
    leftMotor1.run(BACKWARD);    
    rightMotor1.run(BACKWARD);  
  for (speedSet = 0; speedSet < MAX_SPEED; speedSet +=2) 
  {
    leftMotor1.setSpeed(speedSet);
    rightMotor1.setSpeed(speedSet); 
    delay(5);
  }
}  

void turnRight() { 
  motorSet = "RIGHT";
  leftMotor1.run(FORWARD);   
  rightMotor1.run(BACKWARD);  
  rightMotor1.setSpeed(speedSet+MAX_SPEED_OFFSET);         
  delay(1500);       
  motorSet = "FORWARD";
  leftMotor1.run(FORWARD);   
  rightMotor1.run(FORWARD);
      
}  

void turnLeft() { 
  motorSet = "LEFT";
  leftMotor1.run(BACKWARD);    
  leftMotor1.setSpeed(speedSet+MAX_SPEED_OFFSET);      
  rightMotor1.run(FORWARD);   
  delay(1500);  
  motorSet = "FORWARD";
  leftMotor1.run(FORWARD);  
  rightMotor1.run(FORWARD); 
}  

void turnAround() { 
  motorSet = "RIGHT";
  leftMotor1.run(FORWARD);
  rightMotor1.run(BACKWARD);
  rightMotor1.setSpeed(speedSet+MAX_SPEED_OFFSET);      
  delay(1700);
  motorSet = "FORWARD";
  leftMotor1.run(FORWARD);
  rightMotor1.run(FORWARD); 
}
