/*

This file is for unit testing the physical hardware connected to the Mega 2560 microcontroller.
This taste case runs to check the RGB Led, Various LEDs, Speakers, Potentiometer, LCD, and buttons.

*/

#include <Arduino.h>
#include <unity.h>
#include <tune_studio.h>

// The amount of times a loop should test the blinking of the RGB led.
#define BLINK_TEST_AMOUNT 500


/*
Test the RGB led using a digital signal.
Tests RED, GREEN, BLUE pins.
*/
void test_rgb_led() {
    for(int i = 0; i < BLINK_TEST_AMOUNT; i++) {
        digitalWrite(RGB_RED, HIGH);
        TEST_ASSERT_EQUAL(digitalRead(RGB_RED), HIGH);
        digitalWrite(RGB_RED, LOW);

        digitalWrite(RGB_BLUE, HIGH);
        TEST_ASSERT_EQUAL(digitalRead(RGB_BLUE), HIGH);
        digitalWrite(RGB_BLUE, LOW);

        digitalWrite(RGB_GREEN, HIGH);
        TEST_ASSERT_EQUAL(digitalRead(RGB_GREEN), HIGH);
        digitalWrite(RGB_GREEN, LOW);

        // Test Low Signals

        digitalWrite(RGB_RED, LOW);
        TEST_ASSERT_EQUAL(digitalRead(RGB_RED), LOW);

        digitalWrite(RGB_BLUE, LOW);
        TEST_ASSERT_EQUAL(digitalRead(RGB_BLUE), LOW);

        digitalWrite(RGB_GREEN, LOW);
        TEST_ASSERT_EQUAL(digitalRead(RGB_GREEN), LOW);
    }
}

void setup() {
    // Wait two seconds until the testing begins.
    delay(2000);
    Serial.begin(9600);
    delay(500);

    // Initalize Outputs
    pinMode(RGB_RED, OUTPUT);
    pinMode(RGB_BLUE, OUTPUT);
    pinMode(RGB_GREEN, OUTPUT);
    
    // Begin
    UNITY_BEGIN();
    RUN_TEST(test_rgb_led);
    UNITY_END();
}

void loop() {

}