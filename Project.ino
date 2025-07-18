#include <Servo.h>

Servo myServo;

int triggerPin = 6;
int echoPin = 10;
int timeInMicroSec, distance, centimeters;

int redLED = 13;
int yellowLED = 8;
int blueLED = 2;
int buzzer = 7;

int relayPin = 3;

int angle = -90;              // Start angle
int step = 1;                 // +1 or -1 step
unsigned long lastMove = 0;   // Time of last step
const int interval = 15;      // Time between moves (ms)


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

}

// ------ FUNCTION ------

// Relay
void shoot(){
  digitalWrite(relayPin, LOW);
  delay(500);

  digitalWrite(relayPin, HIGH);
  delay(1000);
}

// ------ LOOP ------
void loop() {

// Ultrasonic Distance Measurement
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2);
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);

  timeInMicroSec = pulseIn(echoPin, HIGH);
  centimeters = timeInMicroSec / 29 / 2;
  distance = centimeters / 2.54; // convert to inches

  // Reset LEDs
  digitalWrite(yellowLED, LOW);
  digitalWrite(redLED, LOW);
  digitalWrite(blueLED, LOW);

  // Distancing
  if (distance <= 12) {
    digitalWrite(redLED, HIGH);
    digitalWrite(buzzer, HIGH);
    shoot();

  } 
  else if (distance <= 20) {
    digitalWrite(buzzer, LOW);
    digitalWrite(yellowLED, HIGH);

    } 
    else {
      digitalWrite(blueLED, HIGH);
      digitalWrite(buzzer, LOW);
    }

  delay(30);  // Small delay to avoid flooding serial

  // Servo Logic
// Servo sweep logic using millis()
if (distance > 12) {
  // true && 
  unsigned long currentTime = millis();

  if (currentTime - lastMove >= interval) {
    lastMove = currentTime;

    myServo.write(angle);  // Move servo
    angle += step;

    // Reverse direction at sweep limits
    if (angle >= 90 || angle <= -90) {
      step = -step;
    }
  }
}

}






