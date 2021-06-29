/*

TuneStudio2560 main head file.
This header file contains basic definitions that are used in TuneStudio2560.

*/

// LEDs

#ifndef RGB_BRIGHTNESS
    /*
    The brightness of the RGB LED which serves as a status indicator.
    Numbers should be between 0-255
    */
    #define RGB_BRIGHTNESS 200
#endif

#ifndef RGB_RED
    /*The pin of the RED led inside of the RGB status led.*/
    #define RGB_RED 9
#endif

#ifndef RGB_GREEN
    /*The pin of the GREEN led inside of the RGB status led.*/
    #define RGB_GREEN 10
#endif

#ifndef RGB_BLUE
    /*The pin of the BLUE led inside of the RGB status led.*/
    #define RGB_BLUE 11
#endif

// Buttons

#ifndef BTN_TONE_1
    /*Represents the pin of the button with the green led.*/
    #define BTN_TONE_1 27
#endif

#ifndef BTN_TONE_2
    /*Represents the pin of the button with the blue led.*/
    #define BTN_TONE_2 25 
#endif

#ifndef BTN_TONE_3
    /*Represents the pin of the button with the red led.*/
    #define BTN_TONE_3 28 
#endif

#ifndef BTN_TONE_4
    /*Represents the pin of the button with the yellow led.*/
    #define BTN_TONE_4 26
#endif

#ifndef BTN_TONE_5
    /*Represents the pin of the button with the white led.*/
    #define BTN_TONE_5 24 
#endif

#ifndef BTN_ADD_SELECT
    /*The pin of the select/confirm button which is also used to add tones.*/
    #define BTN_ADD_SELECT 2
#endif

#ifndef BTN_DEL_CANCEL
    /*The pin of the delete button which can be used to delete tones.*/
    #define BTN_DEL_CANCEL 3
#endif

#ifndef BTN_RST
    /*The pin of the reset button which can be used to delete songs and flush EEPROM.*/
    #define BTN_RST 4
#endif

// Potentiometer (Frequency Changer)

#ifndef TONE_FREQ
    #define TONE_FREQ A7
#endif

// Passive Speaker

#ifndef SPEAKER_1
    #define SPEAKER_1 A2
#endif
#ifndef SPEAKER_2
    #define SPEAKER_2 A1
#endif

// I2C 16x2 Sunflounder LCD

#define LCD1602_SDA 20
#define LCD1602_SCL 21

// Extra Declarations
#define ANALOG_MAX 255
#define ANALOG_MIN 0

/*
UNIT TESTING:
Test the RGB led on the breadboard.
*/

void test_rgb_led();
void test_buttons();
void test_speaker1();
void test_speaker2();
void test_potentiometer();
void test_lcd();