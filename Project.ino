#include <Servo.h>

Servo myServo;

int triggerPin = 5;
int echoPin = 4;
int timeInMicroSec, distance, centimeters;

int redLED = 13;
int yellowLED = 8;
int blueLED = 2;
int buzzer = 7;

int relayPin = 3;

int angle = 0;
int step = 1;
unsigned long lastUpdate = 0;
const int interval = 15;

bool pauseServo = false;  // Controls whether servo moves

// ------------------- FUNCTIONS -------------------

void beepBeep(int x) {
  digitalWrite(buzzer, HIGH);
  delay(100);
  digitalWrite(buzzer, LOW);
  delay(100);
  digitalWrite(buzzer, HIGH);
  delay(100);
  digitalWrite(buzzer, LOW);
  delay(x);
}

void shoot() {
  digitalWrite(relayPin, HIGH);
  delay(500);  // Adjust if needed
  digitalWrite(relayPin, LOW);
}

// -------------------- SETUP ---------------------

void setup() {
  myServo.attach(9);

  pinMode(triggerPin, OUTPUT);
  pinMode(echoPin, INPUT);

  pinMode(redLED, OUTPUT);
  pinMode(yellowLED, OUTPUT);
  pinMode(blueLED, OUTPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(relayPin, OUTPUT);

  Serial.begin(9600);

  // Startup beep
  digitalWrite(buzzer, HIGH);
  delay(500);
  digitalWrite(buzzer, LOW);
  delay(500);
}

// -------------------- LOOP ---------------------

void loop() {
  unsigned long currentTime = millis();

  // Radar sweep logic (non-blocking)
  if (!pauseServo && (currentTime - lastUpdate >= interval)) {
    lastUpdate = currentTime;

    myServo.write(angle);
    angle += step;

    if (angle >= 180 || angle <= 0) {
      step = -step;
    }
  }

  // --- Ultrasonic Distance Measurement ---
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2);
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);

  timeInMicroSec = pulseIn(echoPin, HIGH);
  centimeters = timeInMicroSec / 29 / 2;
  distance = centimeters / 2.54; // convert to inches

  Serial.print("\nDistance: ");
  Serial.print(distance);

  // Reset LEDs
  digitalWrite(yellowLED, LOW);
  digitalWrite(redLED, LOW);
  digitalWrite(blueLED, LOW);

  // --- Decision Making ---
  if (distance <= 12) {
    pauseServo = true;  // Pause radar sweep
    digitalWrite(redLED, HIGH);
    beepBeep(250);
    shoot();
  } else {
    pauseServo = false; // Resume sweep

    if (distance <= 30) {
      digitalWrite(yellowLED, HIGH);
      beepBeep(700);
    } else {
      digitalWrite(blueLED, HIGH);
    }
  }

  delay(50);  // Small delay to avoid flooding serial
}
