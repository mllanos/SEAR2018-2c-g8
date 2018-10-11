#include <NewPing.h>
#include <Servo.h>
#include <LiquidCrystal.h>

#define TRIG_PIN A4
#define ECHO_PIN A5
#define MAX_DISTANCE 200

// Entradas de modo
#define NORMAL_MODE_INPUT 1
#define TEST_MODE_INPUT 2
#define MAINTENANCE_MODE_INPUT 3

// Definición de modos
#define NORMAL_MODE 1
#define TEST_MODE 2
#define MAINTENANCE_MODE 3
#define UNKNOWN_MODE 3

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

NewPing sonar(TRIG_PIN, ECHO_PIN, MAX_DISTANCE);
Servo myservo;

boolean goesForward = false;
int distance = 100;
int speedSet = 0;

const int motorPin1  = 11;
const int motorPin2  = 10;
//Motor B
const int motorPin3  = 6;
const int motorPin4  = 5;

// Retorna el modo de operacion actual.
byte currentMode() {
  if (digitalRead(NORMAL_MODE_INPUT) == HIGH) {
    return NORMAL_MODE;
  } else if (digitalRead(TEST_MODE_INPUT) == HIGH) {
    return TEST_MODE;
  } else if (digitalRead(MAINTENANCE_MODE_INPUT) == HIGH) {
    return MAINTENANCE_MODE;
  } else {
    return UNKNOWN_MODE;
  }
}

int lookRight() {
  myservo.write(50);
  delay(500);
  int distance = readPing();
  delay(100);
  myservo.write(115);
  return distance;
}

int lookLeft() {
  myservo.write(170);
  delay(500);
  int distance = readPing();
  delay(100);
  myservo.write(115);
  return distance;
  delay(100);
}

int readPing() {
  delay(70);
  int cm = sonar.ping_cm();
  if (cm == 0)
  {
    cm = 250;
  }
  return cm;
}

void moveStop() {
  if (currentMode() == MAINTENANCE_MODE) {
    lcd.print("STOP");
    return;
  }
  analogWrite(motorPin1, 0);
  analogWrite(motorPin2, 0);
  analogWrite(motorPin3, 0);
  analogWrite(motorPin4, 0);
}

void moveForward() {
  if (currentMode() == MAINTENANCE_MODE) {
    lcd.print("FORWARD");
    return;
  }
  analogWrite(motorPin1, 180);
  analogWrite(motorPin2, 0);
  analogWrite(motorPin3, 180);
  analogWrite(motorPin4, 0);

}

void moveBackward() {
  if (currentMode() == MAINTENANCE_MODE) {
    lcd.print("BACKWARD");
    return;
  }
  analogWrite(motorPin1, 0);
  analogWrite(motorPin2, 180);
  analogWrite(motorPin3, 0);
  analogWrite(motorPin4, 180);

}

void turnRight() {
  if (currentMode() == MAINTENANCE_MODE) {
    lcd.print("TURN RIGHT");
    return;
  }
  analogWrite(motorPin1, 180);
  analogWrite(motorPin2, 0);
  analogWrite(motorPin3, 0);
  analogWrite(motorPin4, 180);
  delay(300);
  moveForward();

}

void turnLeft() {
  if (currentMode() == MAINTENANCE_MODE) {
    lcd.print("TURN LEFT");
    return;
  }
  analogWrite(motorPin1, 0);
  analogWrite(motorPin2, 180);
  analogWrite(motorPin3, 180);
  analogWrite(motorPin4, 0);
  delay(300);
  moveForward();
}

void setup() {
  lcd.begin(16, 2);
  myservo.attach(9);
  myservo.write(115);
  delay(2000);
  distance = readPing();
  delay(100);
  distance = readPing();
  delay(100);
  distance = readPing();
  delay(100);
  distance = readPing();
  delay(100);
}

void loop() {
  int distanceR = 0;
  int distanceL =  0;
  delay(40);

  if (distance <= 20) {
    moveStop();
    delay(100);
    moveBackward();
    delay(300);
    moveStop();
    delay(200);
    distanceR = lookRight();
    delay(200);
    distanceL = lookLeft();
    delay(200);

    if (distanceR >= distanceL) {
      turnRight();
      moveStop();
    } else {
      turnLeft();
      moveStop();
    }
  } else {
    moveForward();
  }
  distance = readPing();
}
