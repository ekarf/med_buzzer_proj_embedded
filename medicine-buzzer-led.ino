#include <ESP32Servo.h>

// Define pin connections for LEDs, Buzzer, Servo, and Ultrasonic Sensor
#define LED1 13
#define LED2 12
#define LED3 14
#define LED4 27
#define BUZZER 25
#define SERVO_PIN 26
#define TRIG_PIN 33
#define ECHO_PIN 32

Servo myServo;

// PID variables
float Kp = 1.0, Ki = 0.1, Kd = 0.05;
float prev_error = 0.0;
float integral = 0.0;
unsigned long lastTime = 0;
int targetAngle = 90;  // Open position
int currentAngle = 0;  // Starting position

// Timer settings
const unsigned long intervalDuration = 10000;
const unsigned long activeDuration = 5000;
unsigned long startTime;
bool isActive = false;
int currentLED = 1;

void setup() {
    Serial.begin(115200);
    delay(1000);

    pinMode(LED1, OUTPUT);
    pinMode(LED2, OUTPUT);
    pinMode(LED3, OUTPUT);
    pinMode(LED4, OUTPUT);
    pinMode(BUZZER, OUTPUT);
    pinMode(TRIG_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);

    digitalWrite(LED1, LOW);
    digitalWrite(LED2, LOW);
    digitalWrite(LED3, LOW);
    digitalWrite(LED4, LOW);
    digitalWrite(BUZZER, LOW);

    myServo.attach(SERVO_PIN);
    myServo.write(currentAngle);

    startTime = millis();
    lastTime = millis();

    Serial.println("Setup complete. Monitoring...");
}

void loop() {
    float distance = readUltrasonicDistance();

    if (distance > 0 && distance < 10) {
        Serial.println("* Object Detected! Opening box and sounding buzzer *");
        moveServoPID(targetAngle);
        digitalWrite(BUZZER, HIGH);
        delay(2000);  // Keep it open and buzzer on for 2 seconds
        digitalWrite(BUZZER, LOW);
        moveServoPID(0);  // Close box again
        return;  // Skip the regular timer cycle this loop
    }

    unsigned long currentTime = millis();
    unsigned long elapsedTime = currentTime - startTime;

    static unsigned long lastDebugTime = 0;
    if (currentTime - lastDebugTime >= 1000) {
        Serial.print("Elapsed Time: ");
        Serial.println(elapsedTime);
        Serial.print("Current LED: ");
        Serial.println(currentLED);
        Serial.print("isActive: ");
        Serial.println(isActive ? "YES" : "NO");
        lastDebugTime = currentTime;
    }

    if (!isActive && elapsedTime >= intervalDuration) {
        Serial.print("* Turning ON LED");
        Serial.print(currentLED);
        Serial.println(" and Buzzer *");

        switch(currentLED) {
            case 1: digitalWrite(LED1, HIGH); break;
            case 2: digitalWrite(LED2, HIGH); break;
            case 3: digitalWrite(LED3, HIGH); break;
            case 4: digitalWrite(LED4, HIGH); break;
        }
        digitalWrite(BUZZER, HIGH);
        isActive = true;
        startTime = currentTime;

        moveServoPID(targetAngle);
    }
    else if (isActive && elapsedTime >= activeDuration) {
        Serial.print("* Turning OFF LED");
        Serial.print(currentLED);
        Serial.println(" and Buzzer *");

        digitalWrite(LED1, LOW);
        digitalWrite(LED2, LOW);
        digitalWrite(LED3, LOW);
        digitalWrite(LED4, LOW);
        digitalWrite(BUZZER, LOW);

        isActive = false;
        currentLED++;
        if (currentLED > 4) currentLED = 1;
        startTime = currentTime;

        moveServoPID(0);
    }
}

// PID control to move servo smoothly
void moveServoPID(int target) {
    unsigned long now = millis();
    float dt = (now - lastTime) / 1000.0;
    lastTime = now;

    for (int i = 0; i < 50; i++) {
        float error = target - currentAngle;
        integral += error * dt;
        float derivative = (error - prev_error) / dt;

        float output = Kp * error + Ki * integral + Kd * derivative;
        currentAngle += output;

        if (currentAngle > 180) currentAngle = 180;
        if (currentAngle < 0) currentAngle = 0;

        myServo.write((int)currentAngle);
        prev_error = error;

        delay(20);
    }
}

// Function to measure distance
float readUltrasonicDistance() {
    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);

    long duration = pulseIn(ECHO_PIN, HIGH, 30000);
    float distance = duration * 0.0343 / 2.0;

    return distance;
}