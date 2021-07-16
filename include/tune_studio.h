/*

TuneStudio2560 main head file.
This header file contains basic definitions that are used in TuneStudio2560.

*/

// LEDs
#ifndef tune_studio_h
#define tune_studio_h

#include <LiquidCrystal_I2C.h>

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

#ifndef BTN_OPTION
    /*The pin of the reset button which can be used to delete songs and flush EEPROM.*/
#define BTN_OPTION 4
#endif

// Potentiometer (Frequency Changer)

#ifndef TONE_FREQ
#define TONE_FREQ A7
#endif

// Passive Speaker

#ifndef SPEAKER_1
#define SPEAKER_1 A2
#endif

// The rate at which additional button presses & interrupts should be ignored.

#ifndef DEBOUNCE_RATE
#define DEBOUNCE_RATE 1000
#endif

// I2C 16x2 Sunflounder LCD

#define LCD1602_SDA 20
#define LCD1602_SCL 21

// Extra Declarations
#define ANALOG_MAX 255
#define ANALOG_MIN 0

#define TEXT_SCROLL_SPEED 135

/**
 * @brief A list of all of the tone pins.
 */
uint_fast8_t TONE_PINS[] = {
    BTN_TONE_1, BTN_TONE_2, BTN_TONE_3, BTN_TONE_4, BTN_TONE_5
};

/*
UNIT TESTING:
Test the RGB led on the breadboard.
*/

/**
 * @brief Tests if the RGB led is working by lighting it up multiple times and reading the values
 * from each of the pins.
 */
void test_rgb_led();

/**
 * @brief Tests if the buttons are working by reading the value of each button
 * and checking if it is HIGH. Buttons are HIGH if they are not being pressed and are LOW
 * if they are being pressed.
 */
void test_buttons();

/**
 * @brief Tests the first speaker. Plays 3 tones and then asks the user to input if the
 * speaker is working or not.
 */
void test_speaker1();

/**
 * @brief Asks the user to adjust the value of the potentiometer to test
 * its functionality.
 */
void test_potentiometer();

/**
 * @brief Displays text on the LCD and asks the user to confirm if the
 * text on the LCD exists.
 */
void test_lcd();

// CUSTOM LCD SYMBOLS use lcd.write(hex num) to write the characters.

/*Custom Char symbol for music note.*/
#define MUSIC_NOTE_SYMBOL 0x00
byte MUSIC_NOTE[] = {
  0x04,
  0x06,
  0x05,
  0x04,
  0x04,
  0x0C,
  0x1C,
  0x0C
};

/*Custom Char symbol for a playing song symbol.*/
#define PLAYING_SONG_SYMBOL 0x01
byte PLAYING_SONG[] = {
  0x10,
  0x18,
  0x1C,
  0x1E,
  0x1E,
  0x1C,
  0x18,
  0x10
};

/*Custom Char symbol for a paused song symbol.*/
#define PAUSE_SONG_SYMBOL 0x02
byte PAUSE_SONG[] = {
  0x00,
  0x1B,
  0x1B,
  0x1B,
  0x1B,
  0x1B,
  0x1B,
  0x00
};
/*Custom Char symbol for a progress block.*/
#define PROGRESS_BLOCK_SYMBOL 0x03
byte PROGRESS_BLOCK[] = {
  0x1F,
  0x1F,
  0x1F,
  0x1F,
  0x1F,
  0x1F,
  0x1F,
  0x1F
};

// METHODS FOR TUNESTUDIO2560 MAIN PROGRAM

/**
 * @brief The primary LCD object which controls the main LCD.
 *
 */
LiquidCrystal_I2C lcd(0x27, 20, 4);

/**
 * @brief A enum of all different types of screens the user can be on.
 * The type of screen the user is on depends what is loaded onto the screen.
 *
 */
enum CurrentAction {
    HOME_SCREEN,
    LISTEN_MODE_SONG_SELECT,
    LISTEN_MODE_SONG_PLAY,
    LISTEN_MODE_DELETE_SONG,
    LISTEN_MODE_FLUSH_EEPROM,
    CREATOR_MODE_HOME,
    CREATOR_MODE_CREATE_NEW,
    CREATER_MODE_SAVE_CURRENT,
    CREATER_MODE_DELETE_SONG
};

/**
 * @brief Returns if there is currently an interrupt waiting in progress.
 *
 * @return If the interrupt flag is true.
 */
bool isInterrupt();

/**
 * @brief A custom delay function which checks if an immediate interrupt is occuring.
 * Works the same as the normal arduino delay(ms) function just with a custom handler.
 * @param milliseconds The time to delay for.
 */
void delay(int milliseconds);

/**
 * @brief Print a large body of text onto the LCD. This method uses a custom way of printing it to look nicer
 * by printing each individual character and seperating lines when neccessary.
 *
 * @param message The message to print to the LCD.
 */
void print_large_text(String message);

/**
 * @brief Updates the current action. Usually used in interrupts.
 *
 * @param newAction The new action to set.
 */
void update_action(CurrentAction newAction);
/**
 * @brief A method ran when the select button is clicked. Is on an interrupt.
 */
void select_btn_click();
/**
 * @brief A method ran when the cancel/delete button is clicked. Is on an interrupt.
 *
 */
void cancel_btn_click();

/**
 * @brief Scrolls the bottom text on a screen while leaving the top part the same.
 *
 * @param top The text to be displayed on the first row.
 * @param bottom The text to be displayed on the second row (scrolling).
 * @param forward Direction of the scroll.
 */
void print_bottom_scrolling_text(String top, String bottom, bool forward = true);

/**
 * @brief Scrolls the bottom text on a screen while leaving the top part the same.
 * Version 2 uses a different sort of scrolling algorithm. Some may find it better
 * then v1.
 *
 * @param top The text to be displayed on the first row.
 * @param bottom The text to be displayed on the second row (scrolling).
 */
void print_bottom_scrolling_text_v2(String top, String bottom);

/**
 * @brief Checks for & updates Debounce rate.
 *
 * @param buttonPin The pin of the button to check for.
 * @return If the button is being pressed.
 */
bool isPressed(uint8_t buttonPin);

/**
 * @brief Checks for & updates Debounce rate.
 *
 * @param buttonPin The pin of the button to check for.
 * @param buttonPin2 The pin of the second button to check for.
 * @return If both of the buttons are being pressed.
 */
bool isPressed(uint8_t buttonPin1, uint8_t buttonPin2);
#endif