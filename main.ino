#include <Servo.h>

// Definición de pines
const int sensorAguaPin = A0;
const int ledPin = 8;
const int buzzerPin = 7;
const int servoPin = 9;

// Umbral de detección (puedes ajustarlo si el sensor es muy sensible)
const int umbralAgua = 300; 

Servo miServo;

void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  miServo.attach(servoPin);
  
  // Estado inicial (sin agua)
  digitalWrite(ledPin, LOW);
  digitalWrite(buzzerPin, LOW);
  miServo.write(0); // Servo en posición inicial (0 grados)
  
  Serial.begin(9600);
}

void loop() {
  int valorSensor = analogRead(sensorAguaPin);
  Serial.print("Lectura de agua: ");
  Serial.println(valorSensor);

  if (valorSensor > umbralAgua) {
    // Si detecta agua: se activa la alerta
    digitalWrite(ledPin, HIGH);
    digitalWrite(buzzerPin, HIGH);
    miServo.write(180); // Gira a 180 grados
  } else {
    // Si no detecta agua: todo en reposo
    digitalWrite(ledPin, LOW);
    digitalWrite(buzzerPin, LOW);
    miServo.write(0); // Regresa a 0 grados
  }
  
  delay(500);
}