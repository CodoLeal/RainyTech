#include <Servo.h>

const int sensorAguaPin = A0;
const int ledPin = 8;
const int buzzerPin = 7;
const int servoPin = 9;

const int umbralAgua = 300; 

Servo miServo;

void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  miServo.attach(servoPin);
  
  digitalWrite(ledPin, LOW);
  digitalWrite(buzzerPin, LOW);
  miServo.write(0);
  Serial.begin(9600);
}

void loop() {
  int valorSensor = analogRead(sensorAguaPin);
  Serial.print("Lectura de agua: ");
  Serial.println(valorSensor);

  if (valorSensor > umbralAgua) {
    digitalWrite(ledPin, HIGH);
    digitalWrite(buzzerPin, HIGH);
    miServo.write(180); 
  } else {
    digitalWrite(ledPin, LOW);
    digitalWrite(buzzerPin, LOW);
    miServo.write(0); 
  }
  
  delay(500);
}
