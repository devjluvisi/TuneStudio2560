/*

TuneStudio2560 main head file.
This header file contains basic definitions that are used in TuneStudio2560.

*/

// LEDs
#ifndef tune_studio_h
#define tune_studio_h

#ifndef LiquidCrystal_I2C_h
#include <LiquidCrystal_I2C.h>
#endif

#ifndef SevSegShift_h
#include <SevSegShift.h>
#endif

// The size of the EEPROM of the board.
#ifndef EEPROM_SIZE
#define EEPROM_SIZE 4096
#endif

// Shift pins for the 4-wide 7 segment display.

#ifndef SHIFT_PIN_DS
#define SHIFT_PIN_DS 5
#endif

#ifndef SHIFT_PIN_STCP
#define SHIFT_PIN_STCP 6
#endif

#ifndef SHIFT_PIN_SHCP
#define SHIFT_PIN_SHCP 7
#endif

// The specifications of the LCD.

#ifndef LCD_COLS
#define LCD_COLS 20
#endif

#ifndef LCD_ROWS
#define LCD_ROWS 4
#endif

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

// The rate that each tone in the song should be seperated by.

#ifndef TONE_PAUSE
#define TONE_PAUSE 100
#endif

// The maximum allowed length for a song.

#ifndef MAX_SONG_LENGTH
#define MAX_SONG_LENGTH 255
#endif

// I2C 16x2 Sunflounder LCD

#define LCD1602_SDA 20
#define LCD1602_SCL 21

// Extra Declarations
#define ANALOG_MAX 255
#define ANALOG_MIN 0

// The speed at new characters appear on a scrolling LCD.
#define TEXT_SCROLL_SPEED 135

/**
 * @brief The maximum allowed size (characters) for a string when using functions
 * to manipulate the LCD.
 */
#define MAX_LCD_STRING_LENGTH 140
 /**
  * @brief The maximum allowed number of strings in an LCD function.
  */
#define MAX_LCD_STRING_AMOUNT 8


  /**
   * @brief A list of all of the tone pins.
   */
inline uint_fast8_t TONE_PINS[] = {
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
inline byte MUSIC_NOTE[] = {
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
inline byte PLAYING_SONG[] = {
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
inline byte PAUSE_SONG[] = {
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
inline byte PROGRESS_BLOCK[] = {
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
inline LiquidCrystal_I2C lcd(0x27, LCD_COLS, LCD_ROWS);
/**
 * @brief Represents the 4-wide 7 segment display which is connected to 2x SN74HC595N shift registers.
 */
inline SevSegShift segDisplay(SHIFT_PIN_DS, SHIFT_PIN_SHCP, SHIFT_PIN_STCP);


/**
 * @brief Returns if there is currently an interrupt waiting in progress.
 *
 * @return If the interrupt flag is true.
 */
bool is_interrupt();

/**
 * @brief A custom delay function which checks if an immediate interrupt is occuring.
 * Works the same as the normal arduino delay(ms) function just with a custom handler.
 * @param milliseconds The time to delay for.
 */
void delay(int milliseconds);

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
 * @brief Checks for & updates Debounce rate.
 *
 * @param buttonPin The pin of the button to check for.
 * @return If the button is being pressed.
 */
bool is_pressed(uint8_t buttonPin);

/**
 * @brief Checks for & updates Debounce rate.
 *
 * @param buttonPin The pin of the button to check for.
 * @param buttonPin2 The pin of the second button to check for.
 * @return If both of the buttons are being pressed.
 */
bool is_pressed(uint8_t buttonPin1, uint8_t buttonPin2);

/**
 * @brief Prints text to the lcd and wraps text automatically.
 *
 * @param text The text to print.
 * @param charDelay The delay before each character.
 */
void print_lcd(String text, uint8_t charDelay = 150);

/**
 * @brief Prints a single line of scrolling text on the lcd.
 *
 * Note that this method does not clear the screen before executing.
 *
 * @param text The text to print.
 * @param cursorY Where the cursor should start printing.
 * @param charDelay Delay between the scroll.
 */
void print_scrolling(String text, uint8_t cursorY, uint8_t charDelay = 150);


// Audio Methods

/**
 * @brief Get the current frequency from the potentiometer.
 * Whatever tune button was pressed previously is not accounted for.
 *
 * @return The current frequency read by the potentiometer. (0 and 1023)
 */
uint16_t get_current_freq();

/**
 * @brief Gets the current NOTE that is in queue. Takes into account BOTH the tune button and the
 * potentiometer in order to generate a note. This method is the final method used to add notes to
 * a song.
 * @return Returns the frequency of the note.
 */
uint16_t get_current_note();

// Song-Related Methods

/**
 * @brief Saves the data from the array of songs to EEPROM.
 *
 */
void save_song_data();

/**
 * @brief Deletes a song from the array of songs. Does not save to EEPROM.
 *
 * @param index The song to delete.
 *
 */
void delete_song(uint32_t index);

/**
 * @brief Loads the saved songs from EEPROM to a variable in SRAM.
 *
 */
void load_songs_from_eeprom();

#endif