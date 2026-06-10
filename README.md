# Smart-Blind-Stick
A smart assistive device designed for visually impaired individuals using ESP32.

## Features

- Obstacle Detection using HC-SR04
- Distance-based Vibration Feedback
- Distance-based Audio Alerts
- Fall Detection using LIS3DH
- Automatic LED Activation using LDR
- Low-cost and Portable Design

## Hardware Used

- ESP32 Development Board
- HC-SR04 Ultrasonic Sensor
- LIS3DH Accelerometer
- LDR Module
- Coin Vibration Motor
- Passive Buzzer
- 2N2222 NPN Transistor
- 1N4148 Diode

## Working

The ultrasonic sensor detects nearby obstacles and provides graded vibration and buzzer alerts depending on distance.

The LIS3DH accelerometer continuously monitors acceleration and triggers a special emergency alert when a fall is detected.

The LDR automatically turns on an LED in low-light environments.

## Results

- Obstacle Detection Accuracy: >95%
- Fall Detection Accuracy: ~93–100%
- Total Prototype Cost: INR 1200

## Project Report

See the full report in the report folder.

## License

MIT License
