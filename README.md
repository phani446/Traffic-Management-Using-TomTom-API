# 🚦 AI Smart Traffic Management System using TomTom API

![Python](https://img.shields.io/badge/Python-3.x-blue?logo=python)
![Arduino](https://img.shields.io/badge/Arduino-Nano-00979D?logo=arduino)
![TomTom API](https://img.shields.io/badge/API-TomTom-orange)
![IoT](https://img.shields.io/badge/IoT-Embedded-success)
![License](https://img.shields.io/badge/License-MIT-yellow)

An AI-powered Smart Traffic Management System that dynamically controls traffic signals using **live traffic data from the TomTom Traffic API**. The project integrates **Python, Arduino Nano, IR Sensors, and SIM900A GSM** to optimize traffic flow, detect red-light violations, and provide emergency traffic control.

---

# 📌 Project Overview

Traditional traffic signals operate using fixed timing regardless of traffic conditions, leading to congestion and delays.

This project overcomes these limitations by using **live traffic congestion data from the TomTom Traffic API** to dynamically adjust traffic signal timings. The system also detects **red-light violations** using IR sensors and sends **SMS alerts** using a SIM900A GSM module. An **Emergency Override Mode** allows all traffic signals to turn RED instantly during emergency situations.

---

# 🎯 Objectives

- Develop an AI-based smart traffic management system.
- Dynamically adjust signal timing using live traffic data.
- Detect red-light violations automatically.
- Send SMS alerts using SIM900A GSM.
- Implement an Emergency Override mode.
- Demonstrate hardware and software integration.

---

# ✨ Features

- 🚦 Dynamic Traffic Signal Control
- 🌐 Live TomTom Traffic API Integration
- 🧠 AI-Based Congestion Analysis
- 🚗 Red-Light Violation Detection
- 📩 SMS Alerts using SIM900A GSM
- 🚨 Emergency Override Mode
- 🔄 Python-Arduino Serial Communication
- 📊 Real-Time Traffic Monitoring

---

# 🛠 Hardware Used

- Arduino Nano
- SIM900A GSM Module
- 4 IR Sensors
- 4 Red LEDs
- 4 Green LEDs
- Push Button (Emergency)
- Breadboard
- Jumper Wires
- USB Cable
- Laptop

---

# 💻 Software Used

- Python 3.x
- Arduino IDE
- Visual Studio Code
- TomTom Traffic API
- PySerial
- Requests Library

---

# 🔌 Pin Connections

| Device | Arduino Pin |
|---------|-------------|
| Lane 1 Red LED | D2 |
| Lane 1 Green LED | D3 |
| Lane 2 Red LED | D4 |
| Lane 2 Green LED | D5 |
| Lane 3 Red LED | D6 |
| Lane 3 Green LED | D7 |
| Lane 4 Red LED | D8 |
| Lane 4 Green LED | D9 |
| SIM900A TX | D10 |
| SIM900A RX | D11 |
| Emergency Button | D12 |
| IR Sensor Lane 1 | A0 |
| IR Sensor Lane 2 | A1 |
| IR Sensor Lane 3 | A2 |
| IR Sensor Lane 4 | A3 |

---

# 🏗 System Architecture

```text
                 TomTom Traffic API
                        │
                        ▼
                 Python AI Program
                        │
                 USB Serial (COM)
                        ▼
                  Arduino Nano
                        │
      ┌─────────────────┼──────────────────┐
      │                 │                  │
      ▼                 ▼                  ▼
 Traffic Lights     IR Sensors      Emergency Button
      │                 │                  │
      │      Vehicle crosses RED           │
      │                 │                  │
      │        Threshold > 150             │
      │                 │                  │
      └─────────────────┼──────────────────┘
                        ▼
                   SIM900A GSM
                        │
         ┌──────────────┴──────────────┐
         ▼                             ▼
  Violation SMS Alert          Emergency SMS Alert
```

---

# ⚙️ Working Principle

1. Python fetches live traffic data from the TomTom Traffic API.
2. Traffic congestion is converted into congestion levels (1–9).
3. Python sends congestion values to the Arduino Nano through USB Serial.
4. Arduino dynamically adjusts the green signal duration for each lane.
5. IR sensors monitor all RED signal lanes.
6. If a vehicle crosses a RED signal, the Arduino detects a violation.
7. SIM900A GSM sends an SMS alert containing the violation information.
8. Pressing the Emergency Button immediately turns all traffic signals RED and sends an Emergency SMS.

---

# 📷 Project Images

## Working Model

![Working Model](Images/WORKING%20MODEL%20IMAGE.jpeg)

## SMS Alert

![SMS Alert](Images/SMS_OUTPUT.jpeg)

## Python Program

![Python](Images/Screenshot%20(6).png)

## Arduino IDE

![Arduino](Images/Screenshot%20(7).png)

---

# 🎥 Project Demonstration

A demonstration video of the complete working system is available in the **Videos** folder of this repository.

---

# 📦 Installation

Clone the repository

```bash
git clone https://github.com/phani446/Traffic-Management-Using-TomTom-API.git
```

Install Python libraries

```bash
pip install requests pyserial
```

Open Arduino IDE

- Upload **SmartTrafficSystem.ino**

Open Python

```bash
python version_1.py
```

---

# 🚀 Future Scope

- Camera-based Vehicle Detection
- Automatic Number Plate Recognition (ANPR)
- Cloud Database Integration
- Mobile Application
- AI-Based Traffic Prediction
- Smart City Integration
- IoT Dashboard

---

# 📂 Repository Structure

```
Traffic-Management-Using-TomTom-API
│
├── Arduino/
│   └── SmartTrafficSystem.ino
│
├── Python/
│   ├── version_1.py
│   └── requirements.txt
│
├── Images/
│   ├── Working_Model.jpeg
│   ├── SMS_Output.jpeg
│   ├── Screenshot (6).png
│   └── Screenshot (7).png
│
├── Videos/
│   └── Demo.mp4
│
├── README.md
└── LICENSE
```

---

# 👨‍💻 Author

**Panniru Phanidhar**

B.Tech – Electronics and Communication Engineering

JNTUH University College of Engineering Rajanna Sircilla

GitHub: https://github.com/phani446

---

# ⭐ Support

If you found this project useful, please consider giving this repository a **⭐ Star**.

---

## 📜 License

This project is licensed under the **MIT License**.
