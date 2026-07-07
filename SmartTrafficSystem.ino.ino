/*
=========================================================
AI SMART TRAFFIC MANAGEMENT SYSTEM
Version 3
---------------------------------------------------------
Python (TomTom API)
        │
USB Serial
        │
Arduino Nano
        │
Traffic Lights
        │
IR Sensors
        │
SIM900A GSM
        │
Emergency Override
=========================================================
*/

#include <SoftwareSerial.h>

//======================================================
// GSM MODULE
//======================================================

SoftwareSerial gsm(10,11);      // RX,TX

//======================================================
// TRAFFIC LIGHT PINS
//======================================================

// Lane 1
#define L1_RED      2
#define L1_GREEN    3

// Lane 2
#define L2_RED      4
#define L2_GREEN    5

// Lane 3
#define L3_RED      6
#define L3_GREEN    7

// Lane 4
#define L4_RED      8
#define L4_GREEN    9

//======================================================
// IR SENSOR PINS
//======================================================

const int irPins[4] =
{
  A0,
  A1,
  A2,
  A3
};

//======================================================
// EMERGENCY BUTTON
//======================================================

#define EMERGENCY_PIN 12

//======================================================
// SETTINGS
//======================================================

#define THRESHOLD 150

const unsigned long smsDelay = 5000;

//======================================================
// VARIABLES
//======================================================

// Traffic congestion received from Python
int congestion[4] =
{
  3,
  3,
  3,
  3
};

// Current green lane
bool laneGreen[4] =
{
  false,
  false,
  false,
  false
};

// SMS Lock
bool smsLock[4] =
{
  false,
  false,
  false,
  false
};

// SMS timer
unsigned long smsTimer[4] =
{
  0,
  0,
  0,
  0
};

// SMS Queue
bool smsPending = false;
int smsLane = 0;

// GSM Busy
bool gsmBusy = false;

// Emergency Mode
bool emergencyMode = false;

//======================================================
// SETUP
//======================================================

void setup()
{

  Serial.begin(9600);

  gsm.begin(9600);

  // Lane 1
  pinMode(L1_RED,OUTPUT);
  pinMode(L1_GREEN,OUTPUT);

  // Lane 2
  pinMode(L2_RED,OUTPUT);
  pinMode(L2_GREEN,OUTPUT);

  // Lane 3
  pinMode(L3_RED,OUTPUT);
  pinMode(L3_GREEN,OUTPUT);

  // Lane 4
  pinMode(L4_RED,OUTPUT);
  pinMode(L4_GREEN,OUTPUT);

  // IR Sensors
  for(int i=0;i<4;i++)
  {
    pinMode(irPins[i],INPUT);
  }

  // Emergency Button
  pinMode(EMERGENCY_PIN,INPUT_PULLUP);

  allRed();

  Serial.println("======================================");
  Serial.println("AI SMART TRAFFIC MANAGEMENT SYSTEM");
  Serial.println("Version 3");
  Serial.println("======================================");
}
//======================================================
// MAIN LOOP
//======================================================

void loop()
{
    // Check emergency button first
    checkEmergency();

    if(emergencyMode)
    {
        allRed();
        delay(100);
        return;
    }

    // Receive congestion values from Python
    receiveSerial();

    // Run all lanes
    runLane(1, greenTime(congestion[0]));
    runLane(2, greenTime(congestion[1]));
    runLane(3, greenTime(congestion[2]));
    runLane(4, greenTime(congestion[3]));
}

//======================================================
// RECEIVE SERIAL DATA
// Python sends:
// 2,5,7,4
//======================================================

void receiveSerial()
{
    if(Serial.available())
    {
        String data = Serial.readStringUntil('\n');

        if(sscanf(data.c_str(),
                  "%d,%d,%d,%d",
                  &congestion[0],
                  &congestion[1],
                  &congestion[2],
                  &congestion[3]) == 4)
        {
            Serial.println("--------------------------------");

            Serial.println("Traffic Data Received");

            Serial.print("Lane 1 : ");
            Serial.println(congestion[0]);

            Serial.print("Lane 2 : ");
            Serial.println(congestion[1]);

            Serial.print("Lane 3 : ");
            Serial.println(congestion[2]);

            Serial.print("Lane 4 : ");
            Serial.println(congestion[3]);
        }
    }
}

//======================================================
// CONGESTION TO GREEN TIME
// Level 1 → 3 sec
// Level 9 → 10 sec
//======================================================

int greenTime(int level)
{
    level = constrain(level,1,9);

    return map(level,1,9,3,10);
}

//======================================================
// ALL RED SIGNAL
//======================================================

void allRed()
{
    digitalWrite(L1_RED,HIGH);
    digitalWrite(L1_GREEN,LOW);

    digitalWrite(L2_RED,HIGH);
    digitalWrite(L2_GREEN,LOW);

    digitalWrite(L3_RED,HIGH);
    digitalWrite(L3_GREEN,LOW);

    digitalWrite(L4_RED,HIGH);
    digitalWrite(L4_GREEN,LOW);

    laneGreen[0]=false;
    laneGreen[1]=false;
    laneGreen[2]=false;
    laneGreen[3]=false;
}
//======================================================
// RUN SELECTED LANE
//======================================================

void runLane(int lane, int seconds)
{
    allRed();

    switch (lane)
    {
        case 1:
            digitalWrite(L1_RED, LOW);
            digitalWrite(L1_GREEN, HIGH);
            laneGreen[0] = true;
            break;

        case 2:
            digitalWrite(L2_RED, LOW);
            digitalWrite(L2_GREEN, HIGH);
            laneGreen[1] = true;
            break;

        case 3:
            digitalWrite(L3_RED, LOW);
            digitalWrite(L3_GREEN, HIGH);
            laneGreen[2] = true;
            break;

        case 4:
            digitalWrite(L4_RED, LOW);
            digitalWrite(L4_GREEN, HIGH);
            laneGreen[3] = true;
            break;
    }

    Serial.print("Lane ");
    Serial.print(lane);
    Serial.println(" GREEN");

    unsigned long startTime = millis();

    while (millis() - startTime < (unsigned long)seconds * 1000)
    {
        // Check emergency during green time
        checkEmergency();

        if (emergencyMode)
        {
            allRed();
            return;
        }

        // Check IR sensors
        checkViolations();

        delay(50);
    }

    // Yellow Signal
    yellowSignal(lane);

    // Send pending SMS after lane completes
    if (smsPending && !gsmBusy)
    {
        gsmBusy = true;

        sendSMS(smsLane);

        smsPending = false;
        smsLane = 0;

        gsmBusy = false;
    }
}

//======================================================
// YELLOW SIGNAL
//======================================================

void yellowSignal(int lane)
{
    switch (lane)
    {
        case 1:
            digitalWrite(L1_RED, HIGH);
            digitalWrite(L1_GREEN, HIGH);
            break;

        case 2:
            digitalWrite(L2_RED, HIGH);
            digitalWrite(L2_GREEN, HIGH);
            break;

        case 3:
            digitalWrite(L3_RED, HIGH);
            digitalWrite(L3_GREEN, HIGH);
            break;

        case 4:
            digitalWrite(L4_RED, HIGH);
            digitalWrite(L4_GREEN, HIGH);
            break;
    }

    Serial.print("Lane ");
    Serial.print(lane);
    Serial.println(" YELLOW");

    delay(2000);

    allRed();
}
//======================================================
// CHECK RED SIGNAL VIOLATIONS
//======================================================

void checkViolations()
{
    int sensorValue;

    for (int i = 0; i < 4; i++)
    {
        // Ignore the current GREEN lane
        if (laneGreen[i])
            continue;

        sensorValue = analogRead(irPins[i]);

        Serial.print("Lane ");
        Serial.print(i + 1);
        Serial.print(" IR : ");
        Serial.println(sensorValue);

        // Vehicle detected on RED signal
        if (sensorValue > THRESHOLD)
        {
            if (!smsLock[i] && !smsPending)
            {
                Serial.print("Violation at Lane ");
                Serial.println(i + 1);

                // Inform Python
                Serial.print("VIOLATION,");
                Serial.println(i + 1);

                // Queue SMS
                smsPending = true;
                smsLane = i + 1;

                smsLock[i] = true;
                smsTimer[i] = millis();
            }
        }
        else
        {
            // Vehicle moved away
            smsLock[i] = false;
        }

        // Safety unlock after timeout
        if (smsLock[i] && millis() - smsTimer[i] > smsDelay)
        {
            smsLock[i] = false;
        }
    }
}
//======================================================
// SEND SMS
//======================================================

void sendSMS(int lane)
{
    while (gsm.available()) gsm.read();

    gsm.println("AT");
    delay(1000);

    gsm.println("AT+CMGF=1");
    delay(1000);

    gsm.println("AT+CMGS=\"+918523032446\"");
    delay(1000);

    // Wait for '>' prompt
    while (gsm.available())
    {
        char c = gsm.read();
        Serial.write(c);
    }

    // Send message immediately
    gsm.print("AI SMART TRAFFIC SYSTEM\r\n");
    gsm.print("RED SIGNAL VIOLATION\r\n");
    gsm.print("Lane: ");
    gsm.println(lane);
    gsm.print("Location: Shilparamam Junction\r\n");
    gsm.print("Vehicle crossed RED signal.\r\n");

    delay(500);

    // Ctrl+Z
    gsm.write(26);

    // Wait for modem response
    unsigned long start = millis();
    while (millis() - start < 15000)
    {
        while (gsm.available())
        {
            Serial.write(gsm.read());
        }
    }
}
//======================================================
// CHECK EMERGENCY BUTTON
//======================================================

void checkEmergency()
{
    if (digitalRead(EMERGENCY_PIN) == LOW)
    {
        if (!emergencyMode)
        {
            emergencyMode = true;

            allRed();

            Serial.println("================================");
            Serial.println("EMERGENCY MODE ACTIVATED");
            Serial.println("================================");

            sendEmergencySMS();
        }
    }
    else
    {
        emergencyMode = false;
    }
}
//======================================================
// EMERGENCY SMS
//======================================================

//======================================================
// SEND EMERGENCY SMS
//======================================================

void sendEmergencySMS()
{
    Serial.println("Sending Emergency SMS...");

    // Clear GSM buffer
    while (gsm.available())
        gsm.read();

    // Check GSM
    gsm.println("AT");
    delay(1000);

    while (gsm.available())
        Serial.write(gsm.read());

    // Text mode
    gsm.println("AT+CMGF=1");
    delay(1000);

    while (gsm.available())
        Serial.write(gsm.read());

    // Recipient
    gsm.println("AT+CMGS=\"+918523032446\"");
    delay(1000);

    // Wait for '>' prompt
    while (gsm.available())
    {
        char c = gsm.read();
        Serial.write(c);
    }

    // Message
    gsm.println("AI SMART TRAFFIC SYSTEM");
    gsm.println("EMERGENCY ALERT");
    gsm.println("All Traffic Signals RED");
    gsm.println("Location: Shilparamam Junction");
    gsm.println("Emergency Override Activated");

    delay(500);

    // Send SMS (Ctrl+Z)
    gsm.write(26);

    Serial.println("Waiting for GSM Response...");

    unsigned long start = millis();

    while (millis() - start < 15000)
    {
        while (gsm.available())
        {
            Serial.write(gsm.read());
        }
    }

    Serial.println("EMERGENCY_SMS_SENT");
}