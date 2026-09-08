/*
  Automated Fertilizer Spraying Vehicle
  ---------------------------------------
  Project: Development of an Automated Fertilizer Spraying Vehicle for Crop Management
  Author : Omkar Dhekane

  Description:
  Bluetooth-controlled rover that drives through crop rows and sprays fertilizer
  through a pump + nozzle. Movement and spray are controlled remotely via an
  Android Bluetooth terminal app (HC-05 module), and a float sensor protects the
  pump from running dry.

  Hardware:
  - Arduino Uno / Nano
  - L298N Motor Driver (2 DC motors for left/right wheels)
  - HC-05 Bluetooth module (RX/TX)
  - 12V DC water/fertilizer pump + relay module
  - Float sensor (tank level) on pin FLOAT_PIN
  - Buzzer for low-tank alert

  Bluetooth command set (sent from phone app):
  F = forward   B = backward   L = left   R = right   S = stop
  P = pump ON   O = pump OFF
*/

#include <SoftwareSerial.h>

// Bluetooth module pins
SoftwareSerial BT(10, 11); // RX, TX

// Motor driver pins
const int leftMotorFwd  = 2;
const int leftMotorBack = 3;
const int rightMotorFwd = 4;
const int rightMotorBack = 5;
const int enableLeft    = 6;  // PWM speed control
const int enableRight   = 9;  // PWM speed control

// Pump + sensor pins
const int pumpRelay  = 7;
const int floatPin   = 8;   // HIGH = tank has fertilizer, LOW = empty
const int buzzerPin  = 12;

int motorSpeed = 180; // 0-255
char command;

void setup() {
  Serial.begin(9600);
  BT.begin(9600);

  pinMode(leftMotorFwd, OUTPUT);
  pinMode(leftMotorBack, OUTPUT);
  pinMode(rightMotorFwd, OUTPUT);
  pinMode(rightMotorBack, OUTPUT);
  pinMode(enableLeft, OUTPUT);
  pinMode(enableRight, OUTPUT);

  pinMode(pumpRelay, OUTPUT);
  pinMode(floatPin, INPUT_PULLUP);
  pinMode(buzzerPin, OUTPUT);

  digitalWrite(pumpRelay, LOW);
  stopMotors();

  Serial.println("Fertilizer Spraying Vehicle ready. Waiting for Bluetooth commands...");
}

void loop() {
  checkTankLevel();

  if (BT.available() > 0) {
    command = BT.read();
    Serial.print("Command received: ");
    Serial.println(command);

    switch (command) {
      case 'F': moveForward();  break;
      case 'B': moveBackward(); break;
      case 'L': turnLeft();     break;
      case 'R': turnRight();    break;
      case 'S': stopMotors();   break;
      case 'P': pumpOn();       break;
      case 'O': pumpOff();      break;
      default:  break;
    }
  }
}

void moveForward() {
  digitalWrite(leftMotorFwd, HIGH);
  digitalWrite(leftMotorBack, LOW);
  digitalWrite(rightMotorFwd, HIGH);
  digitalWrite(rightMotorBack, LOW);
  analogWrite(enableLeft, motorSpeed);
  analogWrite(enableRight, motorSpeed);
}

void moveBackward() {
  digitalWrite(leftMotorFwd, LOW);
  digitalWrite(leftMotorBack, HIGH);
  digitalWrite(rightMotorFwd, LOW);
  digitalWrite(rightMotorBack, HIGH);
  analogWrite(enableLeft, motorSpeed);
  analogWrite(enableRight, motorSpeed);
}

void turnLeft() {
  digitalWrite(leftMotorFwd, LOW);
  digitalWrite(leftMotorBack, HIGH);
  digitalWrite(rightMotorFwd, HIGH);
  digitalWrite(rightMotorBack, LOW);
  analogWrite(enableLeft, motorSpeed);
  analogWrite(enableRight, motorSpeed);
}

void turnRight() {
  digitalWrite(leftMotorFwd, HIGH);
  digitalWrite(leftMotorBack, LOW);
  digitalWrite(rightMotorFwd, LOW);
  digitalWrite(rightMotorBack, HIGH);
  analogWrite(enableLeft, motorSpeed);
  analogWrite(enableRight, motorSpeed);
}

void stopMotors() {
  digitalWrite(leftMotorFwd, LOW);
  digitalWrite(leftMotorBack, LOW);
  digitalWrite(rightMotorFwd, LOW);
  digitalWrite(rightMotorBack, LOW);
  analogWrite(enableLeft, 0);
  analogWrite(enableRight, 0);
}

void pumpOn() {
  if (digitalRead(floatPin) == HIGH) {
    digitalWrite(pumpRelay, HIGH);
    Serial.println("Pump ON - spraying fertilizer");
  } else {
    Serial.println("Tank empty - pump not started");
    alertLowTank();
  }
}

void pumpOff() {
  digitalWrite(pumpRelay, LOW);
  Serial.println("Pump OFF");
}

void checkTankLevel() {
  if (digitalRead(floatPin) == LOW) {
    digitalWrite(pumpRelay, LOW); // safety cut-off
    alertLowTank();
  }
}

void alertLowTank() {
  digitalWrite(buzzerPin, HIGH);
  delay(150);
  digitalWrite(buzzerPin, LOW);
}
