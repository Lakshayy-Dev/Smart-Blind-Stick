#include <Wire.h>
#include <Adafruit_LIS3DH.h>
#include <Adafruit_Sensor.h>

#define TRIG_PIN     18
#define ECHO_PIN     19
#define VIB_PIN      25
#define BUZZER_PIN   26
#define LED_PIN      33
#define LDR_PIN      34

#define SDA_PIN      21
#define SCL_PIN      22

#define BUZZ_FREQ_OBS 2000   // obstacle buzzer pitch
#define BUZZ_FREQ_1   1800   // fall PII
#define BUZZ_FREQ_2   2600   // fall POO

Adafruit_LIS3DH lis = Adafruit_LIS3DH();

long duration;
float distance;
int ldrValue;

/* ---------- vibration helper ---------- */
void vibrate(int onTime, int offTime) {
  digitalWrite(VIB_PIN, HIGH);
  delay(onTime);
  digitalWrite(VIB_PIN, LOW);
  delay(offTime);
}

/* ---------- obstacle buzzer patterns ---------- */
void obstacleBeep(int onTime, int offTime) {
  tone(BUZZER_PIN, BUZZ_FREQ_OBS);
  delay(onTime);
  noTone(BUZZER_PIN);
  delay(offTime);
}

/* ---------- fall alert (BUZZER ONLY) ---------- */
void fallAlert() {
  digitalWrite(VIB_PIN, LOW);   // ensure no vibration

  for (int i = 0; i < 8; i++) {
    tone(BUZZER_PIN, BUZZ_FREQ_1);  // PII
    delay(250);
    tone(BUZZER_PIN, BUZZ_FREQ_2);  // POO
    delay(250);
  }

  noTone(BUZZER_PIN);
}

void setup() {
  Serial.begin(115200);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(VIB_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);

  digitalWrite(VIB_PIN, LOW);
  noTone(BUZZER_PIN);

  Wire.begin(SDA_PIN, SCL_PIN);

  if (!lis.begin(0x18)) {
    Serial.println("LIS3DH not detected");
    while (1);
  }

  lis.setRange(LIS3DH_RANGE_4_G);

  Serial.println("FINAL SYSTEM STARTED");
}

void loop() {
  /* ---------- FALL DETECTION ---------- */
  lis.read();

  float ax = lis.x / 16384.0;
  float ay = lis.y / 16384.0;
  float az = lis.z / 16384.0;
  float magnitude = sqrt(ax*ax + ay*ay + az*az);

  if (magnitude < 0.35 || magnitude > 2.6) {
    Serial.println("FALL DETECTED");
    fallAlert();
    delay(1500);   // lockout
    return;        // skip obstacle logic
  }

  /* ---------- ULTRASONIC ---------- */
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  duration = pulseIn(ECHO_PIN, HIGH, 30000);

  if (duration != 0) {
    distance = duration * 0.034 / 2;

    if (distance <= 10) {
      // VERY NEAR → continuous sound + strong vibration
      digitalWrite(VIB_PIN, HIGH);
      tone(BUZZER_PIN, BUZZ_FREQ_OBS);
    }
    else if (distance <= 20) {
      vibrate(200, 30);
      obstacleBeep(120, 40);   // fast beep
    }
    else if (distance <= 30) {
      vibrate(150, 80);
      obstacleBeep(120, 80);
    }
    else if (distance <= 40) {
      vibrate(100, 140);
      obstacleBeep(120, 140);
    }
    else if (distance <= 55) {
      vibrate(60, 220);
      obstacleBeep(120, 220);  // slow beep
    }
    else {
      digitalWrite(VIB_PIN, LOW);
      noTone(BUZZER_PIN);
    }
  } else {
    digitalWrite(VIB_PIN, LOW);
    noTone(BUZZER_PIN);
  }

  /* ---------- LDR + LED ---------- */
  ldrValue = analogRead(LDR_PIN);
  digitalWrite(LED_PIN, (ldrValue > 1500) ? HIGH : LOW);

  delay(30);
}
