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
/*  Servo myservo;

  boolean goesForward = false;
  int distance = 100;
  int speedSet = 0;*/
/*
  const int motorPin1  = 11;
  const int motorPin2  = 10;
  //Motor B
  const int motorPin3  = 6;
  const int motorPin4  = 5;
*/
// Retorna el modo de operacion actual.

byte change_mode() {
  current_mode = ++current_mode % 3;
  return current_mode;
}

int leer_bluetooth() {
  if (BT.available())   // Si llega un dato por el puerto BT se envÃ­a al monitor serial
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
  int distancia = leer_sonar_frontal();
  switch (current_mode) {
    case NORMAL_MODE:
      if(distancia > 10) {
        /*analogWrite(motorPin1, 180);
        analogWrite(motorPin2, 0);
        analogWrite(motorPin3, 180);
        analogWrite(motorPin4, 0);*/
      }
      break;
    case TEST_MODE:
      escribir_pantalla("ADELANTE");
      break;
    case MAINTENANCE_MODE:
      break;
  }
}

void mover_atras() {
  int distancia = leer_sonar_trasero();
  switch (current_mode) {
    case NORMAL_MODE:
      if(distancia > 10) {
        /*
        analogWrite(motorPin1, 0);
        analogWrite(motorPin2, 180);
        analogWrite(motorPin3, 0);
        analogWrite(motorPin4, 180);
        */
      }
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
  String mensaje = "MODO " + modos[current_mode];
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

void setup() {
  BT.begin(9600);
  lcd.begin(16, 2);
  /*
    myservo.attach(9);
    myservo.write(115);
  */
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
}