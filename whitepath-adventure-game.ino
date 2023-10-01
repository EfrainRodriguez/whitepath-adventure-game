////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////// WHITEPATH ADVENTURE GAME ///////////////////////////////////////

/*
  Este programa ha sido desarrollado para un juego didáctico de cooperación donde tres o más
  jugadores deben guiar el dispositivo a través de un camino/sendero blanco delimitado por
  borde negro. El dispositivo cuenta los desbordes cada vez que los sensores detectan linea
  negra y avisa del desborde activando un buzzer y leds intermitentes. Pierde el equipo que
  acumule más desbordes detectados.
  
  Autor: Efrain R.
*/

#include <LiquidCrystal.h>

// Pines de la LCD
const int rs = 12;
const int en = 11;
const int d4 = 5;
const int d5 = 4;
const int d6 = 3;
const int d7 = 2;

LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// Pines de los sensores de linea negra infrarrojos
const int sensorPinA0 = A0;
const int sensorPinA1 = A1;
const int sensorPinA2 = A2;
const int sensorPinA3 = A3;

// Pin del buzzer
const int buzzerPin = A8;

// Pin de los leds
const int greenLed = A9;
const int redLed = A10;

// Pin del botón de stop
const int stopButton = A11;

// Declaración de variables
unsigned int count = 0;
unsigned long startTime = 0;
unsigned long elapsedTime = 0;
bool isStopped = false;

// Declaración de funciones
bool isOnBlackLine();
void showElapsedTime();
void showOverflows(int c);

void setup() {
  lcd.begin(16, 2);

  // Pins setup
  pinMode(sensorPinA0, INPUT);
  pinMode(sensorPinA1, INPUT);
  pinMode(sensorPinA2, INPUT);
  pinMode(sensorPinA3, INPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(greenLed, OUTPUT);
  pinMode(redLed, OUTPUT);
  pinMode(stopButton, INPUT_PULLUP);

  // Mostrar el nombre del juego
  lcd.setCursor(0, 0);
  lcd.print("   WhitePath    ");
  lcd.setCursor(0, 1);
  lcd.print(" Adventure Game ");
  delay(2000);  // Mostrar por 2 segundos

  showOverflows(count);

  digitalWrite(greenLed, HIGH);

  startTime = millis();  // Guarda el tiempo de inicio
}

void loop() {
  if (digitalRead(stopButton) == LOW) {
    isStopped = true;
  }

  if (!isStopped) {
    showElapsedTime();

    bool failed = isOnBlackLine();

    if (failed) {
      count++;
      showOverflows(count);

      digitalWrite(greenLed, LOW);

      // Mientras permanece sobre la zona negra
      while (isOnBlackLine()) {
        showElapsedTime();

        digitalWrite(redLed, HIGH);
        digitalWrite(buzzerPin, HIGH);
        delay(100);
        digitalWrite(redLed, LOW);
        digitalWrite(buzzerPin, LOW);
        delay(100);
      }

    } else {
      // Si no se detecta la línea, apaga el buzzer y enciende el led verde
      digitalWrite(buzzerPin, LOW);
      digitalWrite(greenLed, HIGH);
    }

  } else {
    unsigned long seconds = elapsedTime / 1000;
    unsigned long minutes = seconds / 60;

    // Al tiempo base se suma la cantidad de errors multiplicado por 1 minuto (60000 milisegundos)
    unsigned long extendedTime = elapsedTime + (count * 60000);
    unsigned long extendedSeconds = extendedTime / 1000;
    unsigned long extendedMinutes = extendedSeconds / 60;

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("D: ");
    lcd.print(count);
    lcd.print(" T: ");
    lcd.print(minutes);
    lcd.print("m ");
    lcd.print(seconds % 60);
    lcd.print("s ");
    lcd.setCursor(0, 1);
    lcd.print("TT: ");
    lcd.print(extendedMinutes);
    lcd.print("m ");
    lcd.print(extendedSeconds % 60);
    lcd.print("s ");
    delay(10000);
  }
}

bool isOnBlackLine() {
  int sensorValueA0 = digitalRead(sensorPinA0);
  int sensorValueA1 = digitalRead(sensorPinA1);
  int sensorValueA2 = digitalRead(sensorPinA2);
  int sensorValueA3 = digitalRead(sensorPinA3);

  return sensorValueA0 == 1 || sensorValueA1 == 1 || sensorValueA2 == 1 || sensorValueA3 == 1;
}

void showElapsedTime() {
  unsigned long currentTime = millis();
  elapsedTime = currentTime - startTime;
  unsigned long seconds = elapsedTime / 1000;
  unsigned long minutes = seconds / 60;

  // Muestra el tiempo transcurrido
  lcd.setCursor(0, 1);
  lcd.print("Tiempo: ");
  lcd.print(minutes);
  lcd.print("m ");
  lcd.print(seconds % 60);
  lcd.print("s ");
}

void showOverflows(int c) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Desbordes: ");
  lcd.print(c);
}
