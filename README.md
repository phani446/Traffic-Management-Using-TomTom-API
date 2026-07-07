# 🚦 AI Smart Traffic Management System using TomTom API

An AI-based Smart Traffic Management System that dynamically controls traffic signals using **live traffic data from the TomTom Traffic API**. The system is developed using **Python, Arduino Nano, IR Sensors, and SIM900A GSM**.

---

## 📌 Project Overview

Traditional traffic signals use fixed timing regardless of traffic conditions. This project uses live traffic information from the **TomTom Traffic API** to adjust signal timing dynamically.

The system also detects **red-light violations** using IR sensors and sends an **SMS alert** using the SIM900A GSM module. An **emergency override** feature immediately turns all traffic lights red and sends an emergency SMS.

---

## ✨ Features

- 🚦 Dynamic traffic signal timing
- 🌐 Live TomTom Traffic API integration
- 🧠 AI-based congestion calculation
- 🚗 Red-light violation detection
- 📩 SMS alerts using SIM900A GSM
- 🚨 Emergency override mode
- 🔄 Python–Arduino serial communication
- 💻 Real-time monitoring

---

## 🛠 Hardware Used

- Arduino Nano
- SIM900A GSM Module
- 4 IR Sensors
- Red & Green LEDs
- Push Button (Emergency)
- Breadboard
- Jumper Wires
- Laptop

---

## 💻 Software Used

- Python
- Arduino IDE
- Visual Studio Code
- TomTom Traffic API

---

## 🔌 Pin Connections

| Device | Arduino Pin |
|---------|-------------|
| Lane 1 Red | D2 |
| Lane 1 Green | D3 |
| Lane 2 Red | D4 |
| Lane 2 Green | D5 |
| Lane 3 Red | D6 |
| Lane 3 Green | D7 |
| Lane 4 Red | D8 |
| Lane 4 Green | D9 |
| SIM900A TX | D10 |
| SIM900A RX | D11 |
| Emergency Button | D12 |
| IR Sensor 1 | A0 |
| IR Sensor 2 | A1 |
| IR Sensor 3 | A2 |
| IR Sensor 4 | A3 |

---

## ⚙️ Working Principle

1. Python fetches live traffic data from the TomTom API.
2. Congestion is calculated for each lane.
3. Python sends congestion values to the Arduino.
4. Arduino adjusts green signal timing dynamically.
5. IR sensors detect vehicles crossing on a red signal.
6. SIM900A sends an SMS alert when a violation occurs.
7. Pressing the emergency button turns all lights red and sends an emergency SMS.

---

## 📷 Project Images

### Working Model

![Working Model](Images/WORKING_MODEL_IMAGE.jpeg)

### SMS Alert

![SMS Output](Images/SMS_OUTPUT.jpeg)

---

## ▶️ Demo Video

See the demo in the **Videos** folder of this repository.

---

## 📦 Python Libraries

Install the required packages:

```bash
pip install requests pyserial
```

---

## ▶️ Run the Python Program

```bash
python version_1.py
```

---

## 🚀 Future Improvements

- Camera-based vehicle detection
- Automatic number plate recognition (ANPR)
- Cloud database integration
- Mobile application
- AI-based traffic prediction

---

## 👨‍💻 Author

**Panniru Phanidhar**

B.Tech – Electronics & Communication Engineering

JNTUH University College of Engineering Rajanna Sircilla
