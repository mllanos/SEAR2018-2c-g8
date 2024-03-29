//#include <Servo.h>
#include <NewPing.h>
#include <LiquidCrystal.h>
#include <SoftwareSerial.h>

#define TRIG_PIN_FRONTAL 47
#define ECHO_PIN_FRONTAL 46
#define TRIG_PIN_TRASERO 45
#define ECHO_PIN_TRASERO 44
#define MAX_DISTANCE 200

// Definición de modos
#define NORMAL_MODE 0
#define TEST_MODE 1
#define MAINTENANCE_MODE 2

#define ADELANTE 97
#define ATRAS 101
#define IZQUIERDA 98
#define DERECHA 100
#define ENCENDIDO 102
#define APAGADO 103
#define OPCION 99

SoftwareSerial BT(10, 11);

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to

int current_mode = -1;
String modos[] = {"NORMAL", "TEST", "MANTENIMIENTO"};
const int rs = 7, en = 6, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

NewPing sonar_frontal(TRIG_PIN_FRONTAL, ECHO_PIN_FRONTAL, MAX_DISTANCE);
NewPing sonar_trasero(TRIG_PIN_TRASERO, ECHO_PIN_TRASERO, MAX_DISTANCE);
//Servo myservo;

boolean goesForward = false;
int distance = 100;
int speedSet = 0;

const int motorPin1  = 31;
const int motorPin2  = 30;
//Motor B
const int motorPin3  = 33;
const int motorPin4  = 32;

// Retorna el modo de operacion actual.

byte change_mode() {
  current_mode = ++current_mode % 3;

  analogWrite(motorPin1, 0);
  analogWrite(motorPin2, 0);
  analogWrite(motorPin3, 0);
  analogWrite(motorPin4, 0);

  if (current_mode == TEST_MODE) {
    test_pantalla();
  }
  return current_mode;
}

int leer_bluetooth() {
  if (BT.available())   // Si llega un dato por el puerto BT se envía al monitor serial
  {
    return BT.read();
  }
  else
  {
    return -1;
  }
}

void escribir_pantalla(String mensaje) {
  lcd.clear();
  lcd.print(mensaje);
}

void mover_adelante() {
  switch (current_mode) {
    case NORMAL_MODE:
      analogWrite(motorPin1, 0);
      analogWrite(motorPin2, 180);
      analogWrite(motorPin3, 0);
      analogWrite(motorPin4, 180);
      break;
    case TEST_MODE:
      escribir_pantalla("ADELANTE");
      break;
    case MAINTENANCE_MODE:
      break;
  }
}

void mover_atras() {
  switch (current_mode) {
    case NORMAL_MODE:
      analogWrite(motorPin1, 180);
      analogWrite(motorPin2, 0);
      analogWrite(motorPin3, 180);
      analogWrite(motorPin4, 0);
      break;
    case TEST_MODE:
      escribir_pantalla("ATRAS");
      break;
    case MAINTENANCE_MODE:
      break;
  }
}

void mover_izquierda() {
  switch (current_mode) {
    case NORMAL_MODE:
      analogWrite(motorPin1, 180);
      analogWrite(motorPin2, 0);
      analogWrite(motorPin3, 0);
      analogWrite(motorPin4, 180);
      break;
    case TEST_MODE:
      escribir_pantalla("IZQUIERDA");
      break;
    case MAINTENANCE_MODE:
      break;
  }
}

void mover_derecha() {
  switch (current_mode) {
    case NORMAL_MODE:
      analogWrite(motorPin1, 0);
      analogWrite(motorPin2, 180);
      analogWrite(motorPin3, 180);
      analogWrite(motorPin4, 0);
      break;
    case TEST_MODE:
      escribir_pantalla("DERECHA");
      break;
    case MAINTENANCE_MODE:
      break;
  }
}

void encendido() {
  switch (current_mode) {
    case NORMAL_MODE:
      break;
    case TEST_MODE:
      escribir_pantalla("ENCENDIDO");
      break;
    case MAINTENANCE_MODE:
      break;
  }
}

void apagado() {
  switch (current_mode) {
    case NORMAL_MODE:
      analogWrite(motorPin1, 0);
      analogWrite(motorPin2, 0);
      analogWrite(motorPin3, 0);
      analogWrite(motorPin4, 0);
      break;
    case TEST_MODE:
      escribir_pantalla("APAGADO");
      break;
    case MAINTENANCE_MODE:
      break;
  }
}

void opcion() {
  change_mode();
  String mensaje = modos[current_mode];
  escribir_pantalla(mensaje);
}

int leer_sonar_frontal() {
  int distancia = sonar_frontal.ping_cm();

  if (distancia == 0)
    distancia = MAX_DISTANCE;

  return distancia;
}

int leer_sonar_trasero() {
  int distancia = sonar_trasero.ping_cm();

  if (distancia == 0)
    distancia = MAX_DISTANCE;

  return distancia;
}

void test_pantalla() {
  int wait = 1000;
  lcd.clear();
  lcd.print("0123456789ABCDEF");
  delay(wait);
  lcd.clear();
  lcd.setCursor(0, 1);
  lcd.print("0123456789ABCDEF");
  delay(500);
  lcd.scrollDisplayRight();
  delay(500);
  lcd.scrollDisplayRight();
  delay(500);
  lcd.scrollDisplayLeft();
  delay(500);
  lcd.scrollDisplayLeft();
  delay(wait);
  lcd.clear();
}

void mantenimiento() {
  if (current_mode == MAINTENANCE_MODE) {
    int wait = 1000;
    int velocidad = 180;
    //prueba motores
    analogWrite(motorPin2, velocidad);
    analogWrite(motorPin4, velocidad);
    delay(wait);
    analogWrite(motorPin2, 0);
    analogWrite(motorPin4, 0);

    analogWrite(motorPin1, velocidad);
    analogWrite(motorPin3, velocidad);
    delay(wait);
    analogWrite(motorPin1, 0);
    analogWrite(motorPin3, 0);

    //prueba bluetooth
    escribir_pantalla((String)leer_sonar_frontal());
    delay(wait);
    escribir_pantalla((String)leer_sonar_trasero());
    delay(wait);

    test_pantalla();

    escribir_pantalla("funciona");
    delay(wait);

    opcion();
  }
}

void setup() {
  BT.begin(9600);
  lcd.begin(16, 2);

  pinMode(motorPin1, OUTPUT);
  pinMode(motorPin2, OUTPUT);
  pinMode(motorPin3, OUTPUT);
  pinMode(motorPin4, OUTPUT);
  //myservo.attach(9);
  //myservo.write(115);
}

void loop() {
  int accion = leer_bluetooth();

  if (accion > 0) {
    switch (accion) {
      case ADELANTE:
        mover_adelante();
        break;
      case ATRAS:
        mover_atras();
        break;
      case IZQUIERDA:
        mover_izquierda();
        break;
      case DERECHA:
        mover_derecha();
        break;
      case ENCENDIDO:
        encendido();
        break;
      case APAGADO:
        apagado();
        break;
      case OPCION:
        opcion();
        break;
    }
  }


  if (leer_sonar_frontal() <= 10) {
    if (current_mode == NORMAL_MODE) {
      analogWrite(motorPin2, 0);
      analogWrite(motorPin4, 0);
    }

    if (current_mode == TEST_MODE) {
      escribir_pantalla("FRENO FRONTAL");
      delay(500);
      lcd.clear();
    }
  }

  if (leer_sonar_trasero() <= 10) {
    if (current_mode == NORMAL_MODE) {
      analogWrite(motorPin1, 0);
      analogWrite(motorPin3, 0);
    }

    if (current_mode == TEST_MODE) {
      escribir_pantalla("FRENO TRASERO");
      delay(500);
      lcd.clear();
    }
  }

  mantenimiento();
}