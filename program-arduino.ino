// Smart Bin dengan Servo, 2 Sensor Ultrasonik, dan 2 Modul LED Indikator
#include <Servo.h>

// --- Servo Setup ---
Servo servo1;
Servo servo2;
const int pinServo1 = 13;
const int pinServo2 = 12;

// --- Sensor Plastik ---
#define TRIG_PLASTIK 9
#define ECHO_PLASTIK 10

// --- Sensor Logam ---
#define TRIG_LOGAM 7
#define ECHO_LOGAM 6

// --- LED Plastik ---
#define LED_P_HIJAU 2
#define LED_P_KUNING 3
#define LED_P_MERAH 4

// --- LED Logam ---
#define LED_L_HIJAU 5
#define LED_L_KUNING 8
#define LED_L_MERAH 11

const float binDepth = 25.0; // cm

unsigned long penuhStartPlastik = 0;
bool sedangPenuhPlastik = false;

unsigned long penuhStartLogam = 0;
bool sedangPenuhLogam = false;

void setup() {
  // Servo setup
  servo1.attach(pinServo1);
  servo2.attach(pinServo2);
  servo1.write(90);
  servo2.write(90);

  // Sensor
  pinMode(TRIG_PLASTIK, OUTPUT);
  pinMode(ECHO_PLASTIK, INPUT);
  pinMode(TRIG_LOGAM, OUTPUT);
  pinMode(ECHO_LOGAM, INPUT);

  // LED Plastik
  pinMode(LED_P_HIJAU, OUTPUT);
  pinMode(LED_P_KUNING, OUTPUT);
  pinMode(LED_P_MERAH, OUTPUT);

  // LED Logam
  pinMode(LED_L_HIJAU, OUTPUT);
  pinMode(LED_L_KUNING, OUTPUT);
  pinMode(LED_L_MERAH, OUTPUT);

  Serial.begin(9600);
}

// Fungsi menggerakkan servo perlahan
void moveServosSmooth(int startAngle1, int endAngle1, int startAngle2, int endAngle2, int delayMs) {
  int step1 = (startAngle1 < endAngle1) ? 1 : -1;
  int step2 = (startAngle2 < endAngle2) ? 1 : -1;
  int steps = max(abs(endAngle1 - startAngle1), abs(endAngle2 - startAngle2));
  for (int i = 0; i <= steps; i++) {
    int angle1 = startAngle1 + (i * step1);
    int angle2 = startAngle2 + (i * step2);
    if ((step1 > 0 && angle1 > endAngle1) || (step1 < 0 && angle1 < endAngle1)) angle1 = endAngle1;
    if ((step2 > 0 && angle2 > endAngle2) || (step2 < 0 && angle2 < endAngle2)) angle2 = endAngle2;
    servo1.write(angle1);
    servo2.write(angle2);
    delay(delayMs);
  }
}

void moveToPlastic() {
  moveServosSmooth(90, 0, 90, 180, 10);
  delay(1000);
  resetServos();
}

void moveToMetal() {
  moveServosSmooth(90, 180, 90, 0, 10);
  delay(1000);
  resetServos();
}

void resetServos() {
  moveServosSmooth(servo1.read(), 90, servo2.read(), 90, 10);
}

float bacaJarak(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  long duration = pulseIn(echoPin, HIGH);
  return duration * 0.034 / 2;
}

void tampilkanStatus(float distance, unsigned long &startTime, bool &sedangPenuh,
                     int ledH, int ledK, int ledM, const char* label) {
  digitalWrite(ledH, LOW);
  digitalWrite(ledK, LOW);
  digitalWrite(ledM, LOW);

  Serial.print(label);
  Serial.print(" - Jarak: ");
  Serial.print(distance);
  Serial.println(" cm");

  if (distance >= 20) {
    digitalWrite(ledH, HIGH);
    sedangPenuh = false;
  } else if (distance >= 10 && distance < 20) {
    digitalWrite(ledK, HIGH);
    sedangPenuh = false;
  } else {
    if (!sedangPenuh) {
      startTime = millis();
      sedangPenuh = true;
    } else if (millis() - startTime >= 3000) {
      digitalWrite(ledM, HIGH);
    }
  }
}

void loop() {
  // Input dari serial
  if (Serial.available() > 0) {
    String data = Serial.readStringUntil('\n');
    if (data == "plastic") {
      Serial.println("Moving to PLASTIC bin...");
      moveToPlastic();
    } else if (data == "metal") {
      Serial.println("Moving to METAL bin...");
      moveToMetal();
    }
  }

  float jarakPlastik = bacaJarak(TRIG_PLASTIK, ECHO_PLASTIK);
  float jarakLogam = bacaJarak(TRIG_LOGAM, ECHO_LOGAM);

  tampilkanStatus(jarakPlastik, penuhStartPlastik, sedangPenuhPlastik, LED_P_HIJAU, LED_P_KUNING, LED_P_MERAH, "PLASTIK");
  tampilkanStatus(jarakLogam, penuhStartLogam, sedangPenuhLogam, LED_L_HIJAU, LED_L_KUNING, LED_L_MERAH, "LOGAM");

  Serial.println("-------------------------------");
  delay(1000);
}
