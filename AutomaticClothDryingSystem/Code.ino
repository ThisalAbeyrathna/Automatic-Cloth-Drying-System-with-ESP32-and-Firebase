#include <A4988.h>
#include <WiFi.h>
#include <FirebaseESP32.h>

// Pin definitions
#define DIR_PIN 18           // Direction pin connected to GPIO 18
#define STEP_PIN 19          // Step pin connected to GPIO 19
#define ENABLE_PIN 23        // Enable pin connected to GPIO 23
#define LDR_PIN 34           // LDR connected to GPIO 34 (ADC pin)
#define RAIN_SENSOR_PIN 35   // Rain sensor connected to GPIO 35 (ADC pin)
#define CONTROL_PIN 14       // GPIO pin to control (set it as output for fan control)

// Define the number of steps for one complete forward or backward movement
#define FORWARD_STEPS 400
#define BACKWARD_STEPS 400

// Wi-Fi credentials
#define WIFI_SSID "Uok_Students"
#define WIFI_PASSWORD "Student@UOK123"

// Firebase credentials
#define FIREBASE_HOST "https://dryer-eda50-default-rtdb.firebaseio.com/"
#define FIREBASE_AUTH "y8hNZczYHWBiBn029WOWksXlqCfwovJG7TRDqtv8"

// Firebase Data object
FirebaseData firebaseData;

// Firebase configuration and authentication objects
FirebaseConfig config;
FirebaseAuth auth;

// Variables to hold sensor values
bool sun_light = false;
bool rain_status = false;
bool isMotorAtForwardPosition = false;
bool fan_status = false;  // State for GPIO 14 (fan control)

void setup() {
  // Initialize serial communication
  Serial.begin(115200);

  // Set pin modes for motor control, sensors, and GPIO 14 control
  pinMode(DIR_PIN, OUTPUT);
  pinMode(STEP_PIN, OUTPUT);
  pinMode(ENABLE_PIN, OUTPUT);
  pinMode(LDR_PIN, INPUT);
  pinMode(RAIN_SENSOR_PIN, INPUT);
  pinMode(CONTROL_PIN, OUTPUT);  // Set GPIO 14 as output (for fan control)

  // Enable the motor
  digitalWrite(ENABLE_PIN, LOW);

  // Connect to Wi-Fi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println(" connected!");

  // Firebase configuration
  config.host = FIREBASE_HOST;
  config.signer.tokens.legacy_token = FIREBASE_AUTH;

  // Initialize Firebase
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);

  // Check Firebase connection
  if (Firebase.ready()) {
    Serial.println("Connected to Firebase.");
  } else {
    Serial.println("Failed to connect to Firebase.");
  }
}

void loop() {
  // Read sensor values
  sun_light = digitalRead(LDR_PIN) == LOW;  // Sunlight detected when LOW (send true)
  rain_status = digitalRead(RAIN_SENSOR_PIN) == LOW;  // Rain detected when LOW (send true)

  // Determine motor movement based on sensor values
  if (rain_status) {
    // If rain is detected, move the motor backward
    if (isMotorAtForwardPosition) {
      moveMotorBackward();
      isMotorAtForwardPosition = false;
    }
  } else if (sun_light) {
    // If no rain and sunlight is detected, move the motor forward
    if (!isMotorAtForwardPosition) {
      moveMotorForward();
      isMotorAtForwardPosition = true;
    }
  } else {
    // If no sunlight and no rain, move the motor backward
    if (isMotorAtForwardPosition) {
      moveMotorBackward();
      isMotorAtForwardPosition = false;
    }
  }

  // Read `fan_status` value from Firebase and control GPIO 14
  if (Firebase.ready()) {
    if (Firebase.getBool(firebaseData, "fan_status")) {  // Get the boolean value for `fan_status`
      fan_status = firebaseData.boolData();  // Store the state of `fan_status`
      digitalWrite(CONTROL_PIN, fan_status ? HIGH : LOW);  // Set GPIO 14 HIGH or LOW based on Firebase data
      Serial.println("Fan status updated from Firebase.");
    } else {
      Serial.println("Failed to read fan status from Firebase.");
      Serial.println(firebaseData.errorReason());
    }

    // Send sunlight status to Firebase
    if (Firebase.setBool(firebaseData, "sun_light", sun_light)) {
      Serial.println("Sunlight status sent successfully.");
    } else {
      Serial.println("Failed to send sunlight status.");
      Serial.println(firebaseData.errorReason());
    }

    // Send rain status to Firebase
    if (Firebase.setBool(firebaseData, "rain_status", rain_status)) {
      Serial.println("Rain status sent successfully.");
    } else {
      Serial.println("Failed to send rain status.");
      Serial.println(firebaseData.errorReason());
    }
  } else {
    Serial.println("Firebase not ready.");
  }

  // Print motor and sensor status
  Serial.print("Sunlight: ");
  Serial.print(sun_light ? "Detected" : "Not Detected");
  Serial.print(" | Rain: ");
  Serial.print(rain_status ? "Detected" : "Not Detected");
  Serial.print(" | Motor Position: ");
  Serial.println(isMotorAtForwardPosition ? "Forward" : "Backward");
  Serial.print(" | Fan: ");
  Serial.println(fan_status ? "ON" : "OFF");

  // Wait before the next update
  delay(5000);  // Send sensor data every 5 seconds
}

// Function to move the motor forward
void moveMotorForward() {
  digitalWrite(DIR_PIN, HIGH);  // Set direction to forward
  for (int i = 0; i < FORWARD_STEPS; i++) {
    digitalWrite(STEP_PIN, HIGH);
    delayMicroseconds(1000);  // Adjust speed here
    digitalWrite(STEP_PIN, LOW);
    delayMicroseconds(3000);  // Adjust speed here
  }
}

// Function to move the motor backward
void moveMotorBackward() {
  digitalWrite(DIR_PIN, LOW);  // Set direction to backward
  for (int i = 0; i < BACKWARD_STEPS; i++) {
    digitalWrite(STEP_PIN, HIGH);
    delayMicroseconds(1000);  // Adjust speed here
    digitalWrite(STEP_PIN, LOW);
    delayMicroseconds(3000);  // Adjust speed here
  }
}
