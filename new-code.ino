#include <ESP32Servo.h>
#include <BlynkSimpleEsp32.h>

// Define pin connections for LEDs, Buzzer, Servo, and Ultrasonic Sensor
#define LED1 13
#define LED2 12
#define LED3 14
#define LED4 27
#define BUZZER 25
#define SERVO_PIN 26
#define TRIG_PIN 33
#define ECHO_PIN 32

// Blynk IoT Template Configuration
#define BLYNK_TEMPLATE_ID "TMPL6JiusC0L7"  // Your Template ID
#define BLYNK_TEMPLATE_NAME "medisina"  // Your Template Name
#define BLYNK_AUTH_TOKEN "klGTIvlY84XIlgNkpKlEEEOiBJt3RXGI"  // Your Blynk IoT Auth Token

// Blynk credentials
const char* ssid = "Solax_27D1CC8";  // Replace with your Wi-Fi SSID
const char* password = "BahalaKa1";  // Replace with your Wi-Fi password

// Timer settings
unsigned long ledOnDuration = 120000;  // 2 minutes (in milliseconds)
unsigned long startTime;
unsigned long ledOffTime[4];  // Store the time when each LED should be turned off
int ledControl[4] = {0, 0, 0, 0};  // Track if a specific LED is on

BlynkTimer timer;

// Create a Servo object
Servo myServo;  // Declare the Servo object

// Switch control for LEDs (these are mapped to Blynk buttons)
BLYNK_WRITE(V1) {
  ledControl[0] = param.asInt();  // Control LED1 (Medicine 1)
  if (ledControl[0] == 1) {
    digitalWrite(LED1, HIGH);
    ledOffTime[0] = millis() + ledOnDuration;  // Set when to turn off LED1
    digitalWrite(BUZZER, HIGH);  // Turn on buzzer
    myServo.write(90);  // Move servo to 90 degrees
  }
}

BLYNK_WRITE(V2) {
  ledControl[1] = param.asInt();  // Control LED2 (Medicine 2)
  if (ledControl[1] == 1) {
    digitalWrite(LED2, HIGH);
    ledOffTime[1] = millis() + ledOnDuration;  // Set when to turn off LED2
    digitalWrite(BUZZER, HIGH);  // Turn on buzzer
    myServo.write(90);  // Move servo to 90 degrees
  }
}

BLYNK_WRITE(V3) {
  ledControl[2] = param.asInt();  // Control LED3 (Medicine 3)
  if (ledControl[2] == 1) {
    digitalWrite(LED3, HIGH);
    ledOffTime[2] = millis() + ledOnDuration;  // Set when to turn off LED3
    digitalWrite(BUZZER, HIGH);  // Turn on buzzer
    myServo.write(90);  // Move servo to 90 degrees
  }
}

BLYNK_WRITE(V4) {
  ledControl[3] = param.asInt();  // Control LED4 (Medicine 4)
  if (ledControl[3] == 1) {
    digitalWrite(LED4, HIGH);
    ledOffTime[3] = millis() + ledOnDuration;  // Set when to turn off LED4
    digitalWrite(BUZZER, HIGH);  // Turn on buzzer
    myServo.write(90);  // Move servo to 90 degrees
  }
}

// Set the timer values from the Input Number Widgets (These are the scheduled times)
BLYNK_WRITE(V5) {
  unsigned long scheduleTime = param.asLong();  // Set the scheduled time for Medicine 1
  if (millis() >= scheduleTime && ledControl[0] == 0) {
    // Turn on the LED when the scheduled time arrives
    digitalWrite(LED1, HIGH);
    ledOffTime[0] = millis() + ledOnDuration;  // Set when to turn off LED1
    digitalWrite(BUZZER, HIGH);  // Turn on buzzer
    myServo.write(90);  // Move servo to 90 degrees
    ledControl[0] = 1;  // Mark LED1 as active
  }
}

BLYNK_WRITE(V6) {
  unsigned long scheduleTime = param.asLong();  // Set the scheduled time for Medicine 2
  if (millis() >= scheduleTime && ledControl[1] == 0) {
    // Turn on the LED when the scheduled time arrives
    digitalWrite(LED2, HIGH);
    ledOffTime[1] = millis() + ledOnDuration;  // Set when to turn off LED2
    digitalWrite(BUZZER, HIGH);  // Turn on buzzer
    myServo.write(90);  // Move servo to 90 degrees
    ledControl[1] = 1;  // Mark LED2 as active
  }
}

BLYNK_WRITE(V7) {
  unsigned long scheduleTime = param.asLong();  // Set the scheduled time for Medicine 3
  if (millis() >= scheduleTime && ledControl[2] == 0) {
    // Turn on the LED when the scheduled time arrives
    digitalWrite(LED3, HIGH);
    ledOffTime[2] = millis() + ledOnDuration;  // Set when to turn off LED3
    digitalWrite(BUZZER, HIGH);  // Turn on buzzer
    myServo.write(90);  // Move servo to 90 degrees
    ledControl[2] = 1;  // Mark LED3 as active
  }
}

BLYNK_WRITE(V8) {
  unsigned long scheduleTime = param.asLong();  // Set the scheduled time for Medicine 4
  if (millis() >= scheduleTime && ledControl[3] == 0) {
    // Turn on the LED when the scheduled time arrives
    digitalWrite(LED4, HIGH);
    ledOffTime[3] = millis() + ledOnDuration;  // Set when to turn off LED4
    digitalWrite(BUZZER, HIGH);  // Turn on buzzer
    myServo.write(90);  // Move servo to 90 degrees
    ledControl[3] = 1;  // Mark LED4 as active
  }
}

void setup() {
  Serial.begin(115200);
  delay(1000);

  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(LED4, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  pinMode(SERVO_PIN, OUTPUT);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  // Set initial states for LEDs and buzzer
  digitalWrite(LED1, LOW);
  digitalWrite(LED2, LOW);
  digitalWrite(LED3, LOW);
  digitalWrite(LED4, LOW);
  digitalWrite(BUZZER, LOW);

  myServo.attach(SERVO_PIN);  // Attach the servo to the pin
  myServo.write(0);  // Initialize servo to the 0-degree position

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, password);  // Connect to Blynk
  Serial.println("Setup complete.");
}

void loop() {
  Blynk.run();  // Keep Blynk running

  unsigned long currentTime = millis();

  // Check if any LEDs need to be turned off after the duration (2 minutes)
  for (int i = 0; i < 4; i++) {
    if (ledControl[i] == 1 && currentTime >= ledOffTime[i]) {
      // Turn off the LED after 2 minutes
      digitalWrite(i == 0 ? LED1 : (i == 1 ? LED2 : (i == 2 ? LED3 : LED4)), LOW);
      digitalWrite(BUZZER, LOW);  // Turn off buzzer
      myServo.write(0);  // Move servo back to 0-degree position
      ledControl[i] = 0;  // Mark LED as inactive
      Serial.print("Turning off LED for Medicine ");
      Serial.println(i + 1);
    }
  }
}
