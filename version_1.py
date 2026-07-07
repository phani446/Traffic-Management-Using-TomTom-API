"""
==============================================
AI SMART TRAFFIC MANAGEMENT SYSTEM
Version 2
----------------------------------------------
TomTom API -> Python -> USB Serial -> Arduino
==============================================
"""

import requests
import serial
import time

# =====================================================
# CONFIGURATION
# =====================================================

API_KEY = "CUAplFjD58a7RQ9USR8XB0ZNgl06jvKG"

COM_PORT = "COM8"
BAUD_RATE = 9600

UPDATE_INTERVAL = 5  # seconds

# =====================================================
# SHILPARAMAM JUNCTION (Example Coordinates)
# =====================================================

locations = [
    (17.451891, 78.381640),   # Lane 1
    (17.451225, 78.380477),   # Lane 2
    (17.451143, 78.381402),   # Lane 3
    (17.451995, 78.380744)    # Lane 4
]

# =====================================================
# CONNECT TO ARDUINO
# =====================================================

try:
    arduino = serial.Serial(COM_PORT, BAUD_RATE, timeout=1)
    time.sleep(2)
    print("Arduino Connected")
except Exception as e:
    print("Unable to open Serial Port")
    print(e)
    exit()

# =====================================================
# GET TOMTOM TRAFFIC DATA
# =====================================================

def getCongestion(lat, lon):

    url = "https://api.tomtom.com/traffic/services/4/flowSegmentData/absolute/10/json"

    try:

        response = requests.get(
            url,
            params={
                "key": API_KEY,
                "point": f"{lat},{lon}"
            },
            timeout=10
        )

    except Exception as e:

        print("Network Error :", e)
        return 1

    if response.status_code != 200:

        print("TomTom Error :", response.status_code)
        print(response.text)

        return 1

    data = response.json()["flowSegmentData"]

    currentSpeed = data["currentSpeed"]
    freeFlowSpeed = data["freeFlowSpeed"]

    density = max(0, freeFlowSpeed - currentSpeed)

    if density <= 5:
        congestion = 1
    elif density <= 10:
        congestion = 2
    elif density <= 15:
        congestion = 3
    elif density <= 20:
        congestion = 4
    elif density <= 25:
        congestion = 5
    elif density <= 30:
        congestion = 6
    elif density <= 35:
        congestion = 7
    elif density <= 40:
        congestion = 8
    else:
        congestion = 9

    return congestion, currentSpeed, freeFlowSpeed, density

# =====================================================
# MAIN LOOP
# =====================================================

print("\n========================================")
print("AI SMART TRAFFIC MANAGEMENT SYSTEM")
print("========================================")

while True:

    congestionValues = []

    print("\n----------------------------------------")

    for i, (lat, lon) in enumerate(locations):

        congestion, current, free, density = getCongestion(lat, lon)

        congestionValues.append(str(congestion))

        print(f"Lane {i+1}")
        print(f"Current Speed : {current} km/h")
        print(f"Free Speed    : {free} km/h")
        print(f"Density       : {density}")
        print(f"Congestion    : {congestion}")
        print()

    serialData = ",".join(congestionValues)

    print("Sending ->", serialData)

    try:
        arduino.write((serialData + "\n").encode())
    except Exception as e:
        print("Serial Error:", e)

    # Read any messages from Arduino
    start = time.time()
    while time.time() - start < UPDATE_INTERVAL:
        if arduino.in_waiting:
            msg = arduino.readline().decode(errors="ignore").strip()

            if msg:
                print("[Arduino] ", msg)

                # Emergency message
                if msg == "EMERGENCY MODE":
                    print(">>> EMERGENCY OVERRIDE ACTIVE <<<")

                # Violation message
                if "Violation" in msg:
                    print(">>> RED SIGNAL VIOLATION DETECTED <<<")

        time.sleep(0.05)