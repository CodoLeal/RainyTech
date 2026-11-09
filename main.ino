#include <Servo.h>

const int sensorAguaPin = A0;
const int ledPin = 8;
const int buzzerPin = 7;
const int servoPin = 9;

const int umbralCierre = 300;
const int umbralApertura = 200;

const int numMuestras = 10;                     
const unsigned long intervaloLectura = 200;     
const int lecturasParaCerrar = 5;               
const unsigned long tiempoSecoParaAbrir = 5000; 

const int posAbierto = 0;
const int posCerrado = 180;

Servo miServo;

bool techoCerrado = false;
int contadorHumedo = 0;
bool contandoSeco = false;
unsigned long inicioSeco = 0;
unsigned long ultimaLectura = 0;
unsigned long ultimoBeep = 0;
bool buzzerEncendido = false;

int leerSensorFiltrado() {
  int minimo = 1023;
  int maximo = 0;
  long suma = 0;

  for (int i = 0; i < numMuestras; i++) {
    int v = analogRead(sensorAguaPin);
    suma += v;
    if (v < minimo) minimo = v;
    if (v > maximo) maximo = v;
    delay(2);
  }
  suma -= (minimo + maximo);
  return suma / (numMuestras - 2);
}

void cerrarTecho() {
  techoCerrado = true;
  miServo.write(posCerrado);
  digitalWrite(ledPin, HIGH);
  Serial.println(">> TECHO CERRADO");
}

void abrirTecho() {
  techoCerrado = false;
  miServo.write(posAbierto);
  digitalWrite(ledPin, LOW);
  digitalWrite(buzzerPin, LOW);
  buzzerEncendido = false;
  Serial.println(">> TECHO ABIERTO");
}

void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  miServo.attach(servoPin);

  digitalWrite(ledPin, LOW);
  digitalWrite(buzzerPin, LOW);
  miServo.write(posAbierto);

  Serial.begin(9600);
  Serial.println("RainyTech iniciado");
}

void loop() {
  unsigned long ahora = millis();

  if (ahora - ultimaLectura >= intervaloLectura) {
    ultimaLectura = ahora;
    int valor = leerSensorFiltrado();

    Serial.print("Lectura filtrada: ");
    Serial.println(valor);

    if (!techoCerrado) {
      if (valor > umbralCierre) {
        contadorHumedo++;
      } else {
        contadorHumedo = 0;
      }
      if (contadorHumedo >= lecturasParaCerrar) {
        contadorHumedo = 0;
        cerrarTecho();
      }
    } else {
      if (valor < umbralApertura) {
        if (!contandoSeco) {
          contandoSeco = true;
          inicioSeco = ahora;
        } else if (ahora - inicioSeco >= tiempoSecoParaAbrir) {
          contandoSeco = false;
          abrirTecho();
        }
      } else {
        contandoSeco = false;
      }
    }
  }
  if (techoCerrado && ahora - ultimoBeep >= 500) {
    ultimoBeep = ahora;
    buzzerEncendido = !buzzerEncendido;
    digitalWrite(buzzerPin, buzzerEncendido ? HIGH : LOW);
  }
}
