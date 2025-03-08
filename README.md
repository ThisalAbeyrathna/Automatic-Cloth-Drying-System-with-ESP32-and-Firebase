# Automatic-Cloth-Drying-System-with-ESP32-and-Firebase
Here’s a description you can use for your GitHub repository:

---

# Automatic Cloth Drying System with ESP32 and Firebase

This project is an **automatic cloth drying system** controlled by an **ESP32 microcontroller**. The system detects sunlight and rain using an **LDR sensor** and a **rain sensor**, respectively. Based on the sensor readings, it moves a **stepper motor** forward or backward to extend or retract the drying rack. Additionally, the system integrates **Firebase** to store and retrieve real-time data, including:

- **Sunlight detection** (`sun_light`)
- **Rain detection** (`rain_status`)
- **Fan control** (`fan_status`)

## Features
- **Wi-Fi Connectivity**: Connects to a Wi-Fi network to sync data with Firebase.
- **Motorized Rack Movement**: Moves forward when sunlight is detected and retracts when it rains.
- **Firebase Integration**: Sends sensor data to Firebase and receives commands to control a fan.
- **Remote Fan Control**: Turns the fan ON/OFF based on a Firebase boolean variable.
- **Real-Time Status Logging**: Prints sensor values and motor position in the serial monitor.

## Components Used
- **ESP32** (for control and Wi-Fi communication)
- **A4988 Stepper Motor Driver** (to control the stepper motor)
- **Stepper Motor** (to move the drying rack)
- **LDR Sensor** (to detect sunlight)
- **Rain Sensor** (to detect rain)
- **Wi-Fi & Firebase** (to store and fetch data)
- **GPIO Control** (for fan operation)

## Setup Instructions
1. Connect the **ESP32** to your **Wi-Fi network** by updating the `WIFI_SSID` and `WIFI_PASSWORD`.
2. Configure **Firebase** by replacing `FIREBASE_HOST` and `FIREBASE_AUTH` with your credentials.
3. Upload the code to your ESP32 using the **Arduino IDE**.
4. Monitor the serial output to check system status.
5. Control the fan remotely using Firebase.

## Usage
- When **sunlight is detected**, the motor moves forward to extend the drying rack.
- When **rain is detected**, the motor moves backward to retract the rack.
- The **fan can be controlled** remotely via Firebase.
- Sensor values and motor position are updated every **5 seconds**.

This project is ideal for **smart home automation** and **IoT-based drying solutions**.💡

---

Would you like me to add more details, such as wiring instructions or a circuit diagram?
