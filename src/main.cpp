#include <Arduino.h>

#include "uSevenSegmentLib.h"

int LDRDigitalPin = 25;
int MoistureDigitalPin = 33;
int LDRAnalogPin = 27;
int MoistureAnalogPin = 26;
int motorPin = 32;

const int waterSec = 2000;

int pins[8] = {17, 18, 5, 2, 4, 19, 16, 15};
int muxes[2] = {22, 23};

uSevenSegmentLib sevenSegments(2, pins, muxes, 40, false);
bool led_status = true;

int sec = 0;
int minute = 0;
int hour = 0;
int detectSec = 0;
int displaySec = 0;

void timer() {
    delay(1000);
    sec = sec + 1;
    if (sec >= 60) {
        minute = minute + 1;
        sec = 0;
    }
    if (minute >= 60) {
        hour = hour + 1;
        minute = 0;
    }
    sevenSegments.set(sec);
}

void setup() {
    // put your setup code here, to run once:
    Serial.begin(115200);

    pinMode(MoistureDigitalPin, INPUT);
    pinMode(MoistureAnalogPin, INPUT);

    pinMode(LDRDigitalPin, INPUT);
    pinMode(LDRAnalogPin, INPUT);

    pinMode(motorPin, OUTPUT);

    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, led_status);

    sevenSegments.attachInterrupt();
}

void loop() {
    // put your main code here, to run repeatedly:
    timer();
    detectSec = sec;

    int MoistureAnalogStatus = analogRead(MoistureAnalogPin);
    int MoistureDigitalStatus = digitalRead(MoistureDigitalPin);
    int ldrAnalogStatus = analogRead(LDRAnalogPin);
    int ldrdigitalStatus = digitalRead(LDRDigitalPin);

    if (displaySec % 5 == 0) {
        Serial.print("LDR Analog: ");
        Serial.println(ldrAnalogStatus);
        Serial.print("LDR Digital: ");
        Serial.println(ldrdigitalStatus);

        Serial.print("Moisture Analog: ");
        Serial.println(MoistureAnalogStatus);
        Serial.print("Moisture Digital: ");
        Serial.println(MoistureDigitalStatus);

        Serial.println("----------------------------------------------");
    }


    if (detectSec % 5 == 0) {
        if (MoistureDigitalStatus == 1 && ldrdigitalStatus == 0) {
            digitalWrite(motorPin, HIGH);
            delay(waterSec);
            digitalWrite(motorPin, LOW);
            sec = 0;
            minute = 0;
            hour = 0;
            Serial.println("---------- Motor Activate ----------");
        } else {
            digitalWrite(motorPin, LOW);
            Serial.println("---------- Motor Deactivate ----------");
        }
    }
    detectSec = 0;
}