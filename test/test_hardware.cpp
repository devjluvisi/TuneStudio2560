/*

This file is for unit testing the physical hardware connected to the Mega 2560 microcontroller.
This taste case runs to check the RGB Led, Various LEDs, Speakers, Potentiometer, LCD, and buttons.

*/

#include <unity.h>
#include <studio-libs/tune_studio.h>
#include <debug/test_hardware.h>


// The amount of times a loop should test the blinking of the RGB led.
#define BLINK_TEST_AMOUNT 500
// The threshold time that the user should respond with when they have to interact.
#define USER_INTERACT_THERSHOLD 8000

void test_rgb_led()
{
    for (int i = 0; i < BLINK_TEST_AMOUNT; i++)
    {
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

void test_buttons()
{
    TEST_ASSERT_EQUAL(digitalRead(BTN_TONE_1), HIGH);
    TEST_ASSERT_EQUAL(digitalRead(BTN_TONE_2), HIGH);
    TEST_ASSERT_EQUAL(digitalRead(BTN_TONE_3), HIGH);
    TEST_ASSERT_EQUAL(digitalRead(BTN_TONE_4), HIGH);
    TEST_ASSERT_EQUAL(digitalRead(BTN_TONE_5), HIGH);
    TEST_ASSERT_EQUAL(digitalRead(BTN_ADD_SELECT), HIGH);
    TEST_ASSERT_EQUAL(digitalRead(BTN_DEL_CANCEL), HIGH);
    TEST_ASSERT_EQUAL(digitalRead(BTN_OPTION), HIGH);
}

void test_speaker1()
{
    TEST_ASSERT_EQUAL(analogRead(SPEAKER_1), 0);
    Serial.println("[SPEAKER #1] Playing tone 1...");
    tone(SPEAKER_1, 30, 200);
    delay(250);
    Serial.println("[SPEAKER #1] Playing tone 2...");
    tone(SPEAKER_1, 30, 200);
    delay(250);
    Serial.println("[SPEAKER #1] Playing tone 3...");
    tone(SPEAKER_1, 30, 200);
    delay(250);
    Serial.println("[SPEAKER #1] Press button SELECT if you heard the speaker play a sound. Press button DEL if you did not hear it.");
    bool speakerHeard = false;
    const int initalMilis = millis();
    while (true)
    {
        if (digitalRead(BTN_ADD_SELECT) == LOW)
        {
            speakerHeard = true;
            break;
        }
        if (digitalRead(BTN_DEL_CANCEL) == LOW)
        {
            break;
        }
        // Check if the time has run out.
        if (millis() - initalMilis > USER_INTERACT_THERSHOLD)
        {
            break;
        }
    }
    if (!speakerHeard)
    {
        TEST_FAIL_MESSAGE("Speaker #1 could not be heard.");
    }
}

void test_potentiometer() {
    Serial.println("Turn the Potentiometer.");
    Serial.println("Press Delete Button if there is no response on turning.");
    const int initalMilis = millis();
    const int initalValue = analogRead(TONE_FREQ);
    bool pTurned = false;

    while (millis() - initalMilis < USER_INTERACT_THERSHOLD) {
        if (analogRead(TONE_FREQ) > initalValue + 10 || analogRead(TONE_FREQ) < initalValue - 10) {
            pTurned = true;
            break;
        }
        if (digitalRead(BTN_DEL_CANCEL) == LOW) {
            break;
        }
    }

    if (!pTurned) {
        TEST_FAIL_MESSAGE("Could not read value from potentiometer.");
    }
}

void test_lcd() {
    LiquidCrystal_I2C lcd(0x27, 20, 4);
    lcd.init();
    lcd.backlight();
    lcd.setCursor(0, 0);
    lcd.print("Testing...");
    lcd.setCursor(0, 1);
    lcd.print("Second Line");
    Serial.println("Press the SELECT button if you see text on the lcd.\nPress the DELETE button if you do not see anything.");
    const int initalMillis = millis();
    bool lcdRead = false;
    while (millis() - initalMillis < USER_INTERACT_THERSHOLD) {
        if (digitalRead(BTN_ADD_SELECT) == LOW)
        {
            lcdRead = true;
            break;
        }
        if (digitalRead(BTN_DEL_CANCEL) == LOW)
        {
            break;
        }
    }
    if (!lcdRead) {
        TEST_FAIL_MESSAGE("The LCD display could not be read.");
    }
}

void setup()
{
    // Wait two seconds until the testing begins.
    delay(2000);
    Serial.begin(9600);
    delay(500);

    // Initalize Outputs
    pinMode(RGB_RED, OUTPUT);
    pinMode(RGB_BLUE, OUTPUT);
    pinMode(RGB_GREEN, OUTPUT);
    pinMode(BTN_TONE_1, INPUT_PULLUP);
    pinMode(BTN_TONE_2, INPUT_PULLUP);
    pinMode(BTN_TONE_3, INPUT_PULLUP);
    pinMode(BTN_TONE_4, INPUT_PULLUP);
    pinMode(BTN_TONE_5, INPUT_PULLUP);
    pinMode(BTN_ADD_SELECT, INPUT_PULLUP);
    pinMode(BTN_DEL_CANCEL, INPUT_PULLUP);
    pinMode(BTN_OPTION, INPUT_PULLUP);
    pinMode(SPEAKER_1, OUTPUT);
    pinMode(TONE_FREQ, INPUT);

    // Begin
    UNITY_BEGIN();
    RUN_TEST(test_rgb_led);
    RUN_TEST(test_buttons);
    RUN_TEST(test_speaker1);
    RUN_TEST(test_potentiometer);
    RUN_TEST(test_lcd);
    UNITY_END();
}

void loop()
{
}