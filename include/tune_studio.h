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

/*
The maximum allowed number of songs that can be saved. (microSD only, EEPROM is limited to 5)
NOTE: Changing this value above 255 may cause the program to break due to a limitation using the uint8_t datatype.
Expanding will require the change of this datatype.
*/
#ifndef MAX_SONG_AMOUNT
#define MAX_SONG_AMOUNT 255
#endif


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
inline uint8_t MUSIC_NOTE[] = {
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

/*
Tone & Potentiometer Look Up Dictonary.
Used for looking up the different tones and getting their respective value on the chromatic scale.
*/
typedef struct {
    const char* pitch;
    const uint16_t frequency;
} note;

// Represents a single tone button and the frequencies that are possible.
typedef struct {
    const uint8_t pin;
    const note notes[17];
} buttonFrequencies;

/**
 * @brief An array of all tone buttons and their respective frequencies.
 *
 */
const buttonFrequencies toneButtons[]{
    {BTN_TONE_1, {{"B0", 31}, {"C1", 33}, {"CS1", 35}, {"D1", 37}, {"DS1", 39}, {"E1", 41}, {"F1", 44}, {"FS1", 46}, {"G1", 49}, {"GS1", 52}, {"A1", 55}, {"AS1", 58}, {"B1", 62}, {"C2", 65}, {"CS2", 69}, {"D2", 73}, {"DS2", 78}}},
    {BTN_TONE_2, {{"E2", 82}, {"F2", 87}, {"FS2", 93}, {"G2", 98}, {"GS2", 104}, {"A2", 110}, {"AS2", 117}, {"B2", 123}, {"C3", 131}, {"CS3", 139}, {"D3", 147}, {"DS3", 156}, {"E3", 165}, {"F3", 175}, {"FS3", 185}, {"G3", 196}, {"GS3", 208}}},
    {BTN_TONE_3, {{"A3", 220}, {"AS3", 233}, {"B3", 247}, {"C4", 262}, {"CS4", 277}, {"D4", 294}, {"DS4", 311}, {"E4", 330}, {"F4", 349}, {"FS4", 370}, {"G4", 392}, {"GS4", 415}, {"A4", 440}, {"AS4", 466}, {"B4", 494}, {"C5", 523}, {"CS5", 554}}},
    {BTN_TONE_4, {{"D5", 587}, {"DS5", 622}, {"E5", 659}, {"F5", 698}, {"FS5", 740}, {"G5", 784}, {"GS5", 831}, {"A5", 880}, {"AS5", 932}, {"B5", 988}, {"C6", 1047}, {"CS6", 1109}, {"D6", 1175}, {"DS6", 1245}, {"E6", 1319}, {"F6", 1397}, {"FS6", 1480}}},
    {BTN_TONE_5, {{"G6", 1568}, {"GS6", 1661}, {"A6", 1760}, {"AS6", 1865}, {"B6", 1976}, {"C7", 2093}, {"CS7", 2217}, {"D7", 2349}, {"DS7", 2489}, {"E7", 2637}, {"F7", 2794}, {"FS7", 2960}, {"G7", 3136}, {"GS7", 3322}, {"A7", 3520}, {"AS7", 3729}, {"B7", 3951}}}
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
 * @return If the microSD card is enabled.
 */
bool sd_enabled();

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
void delay(uint16_t& milliseconds);

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
 * @brief Set the selected page for selecting a song.
 * Each page selection represents a jump of 5.
 * EX: Page 1 (Songs 1-5)
 *     Page 2 (Songs 6-10)
 *
 * There can only be a absolute maximum of 51 pages because 255/5 is 51 and there are 255 maximum allowed songs.
 *
 * @param page The new page to set.
 */
void set_selected_page(uint8_t page);

/**
 * @brief Get the current selected page of songs.
 *
 * @return uint8_t
 */
uint8_t get_selected_page();

/**
 * @brief Set the selected song object
 *
 * @param song The ID of the song to set.
 */
void set_selected_song(uint8_t song);

/**
 * @brief Get the selected song object
 *
 * @return uint8_t The current selected song in the main class.
 */
uint8_t get_selected_song();

/**
 * @brief Get the current frequency from the potentiometer.
 * Whatever tune button was pressed previously is not accounted for.
 *
 * @return The current frequency read by the potentiometer. (0 and 1023)
 */
uint16_t get_current_freq();

/**
 * @brief Gets the current tone that is in queue. Takes into account BOTH the tune button and the
 * potentiometer in order to generate a note. This method is the final method used to add notes to
 * a song.
 * @return Returns the frequency of the note.
 */
note get_current_tone(uint8_t toneButton);
#endif