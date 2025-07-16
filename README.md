# 🤖 Robot NRF Communication System

This repository documents the **robot-side firmware** that communicates with a custom-built NRF controller. The robot receives movement and weapon commands, and returns telemetry data such as **gyro orientation status**, and **battery levels for Li-ion and LiPo power systems**.

---

## 🧠 Robot System Overview

### Hardware Components

- **NRF24L01+ Radio Module**  
  For bidirectional wireless communication with the controller

- **Motor & Weapon Drivers**  
  Controlled based on input speed values

- **Gyroscope/IMU**  
  Detects orientation (e.g., if flipped upside down)

- **Battery Monitoring Circuits**  
  Tracks Li-ion and LiPo battery levels

---

## 📡 Communication Protocol

### 📥 Robot ← Controller (RX)

Receives commands periodically:

| Field           | Type   | Description                          |
|-----------------|--------|--------------------------------------|
| `move_speed`    | `int8` | -100 to +100                         |
| `weapon_speed`  | `int8` | -100 to +100                         |

### 📤 Robot → Controller (TX)

Sends back status telemetry:

| Field             | Type   | Description                         |
|------------------|--------|-------------------------------------|
| `is_upside_down` | `bool` | Indicates if robot is flipped       |
| `battery_liion`  | `uint8`| 0–100% battery level                |
| `battery_lipo`   | `uint8`| 0–100% battery level                |
| `gyro_status`    | `int16`| Orientation/angle data from IMU     |
| `signal_strength`| `uint8`| (Optional) signal/RSSI level        |

---

## ⚙️ Motion & Weapon Control

- **Motor Driver Interface**
  - Reads `move_speed` value
  - Maps to PWM or motor controller input
- **Weapon Driver Interface**
  - Reads `weapon_speed` value
  - Optional ramp-up/down logic for safety

---

## 🎯 Orientation Detection

- **IMU (e.g., MPU6050 or similar)**
  - Used to detect whether the robot is upside down
  - Optionally outputs orientation angle (`gyro_status`)
- **Flip Detection Logic**
  - If pitch or roll exceeds defined threshold, `is_upside_down = true`

---

## 🔋 Battery Monitoring

| Battery | Functionality             | Notes                                 |
|---------|---------------------------|----------------------------------------|
| Li-ion  | Powers logic/electronics  | Uses voltage divider or fuel gauge IC |
| LiPo    | Powers motors/weapons     | Monitored via analog read             |

Both voltages are converted to percentage and included in the telemetry packet sent to the controller.

---

## 🔐 Safety Features

- **Failsafe Timeout**
  - Stop motors if no command received in X ms
- **Battery Cutoff**
  - Optional: disable motors when LiPo below safe voltage
- **Orientation Lock**
  - Prevent action when upside down (optional)

---
