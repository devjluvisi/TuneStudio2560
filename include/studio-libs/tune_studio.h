/**
 * @file tune_studio.h
 * @author Jacob LuVisi
 * @brief The main header file for the application. Includes important objects and methods that are used inside of the main class.
 * @version 0.1
 * @date 2021-07-26
 *
 * @copyright Copyright (c) 2021
 *
 */

 // Header Guard
#ifndef tune_studio_h
#define tune_studio_h

// Include all the libraries for the program.
#include <LiquidCrystal_I2C.h>
#include <SevSegShift.h>
#include <Arduino.h>
#include <studio-libs/song.h>
#include <debug/debug.h>
#include <hardware/sd_card.h>
// Different states.
#include <studio-libs/state.h>


// Debug Variables (True for Serial prints)
#define DEBUG true

// Program Mode Selection
// Change the performance, SRAM, and flash usage by adjusting.
// 0 = Performance Favor (Reduced Variables & Song Sizes, Cutdown code)
// 1 = Standard (How T2560 was intended.)
// 2 = Feature (Increased song sizes, extra features)
#define PRGM_MODE 1
//////////////////////////////
//// COMPILER DEFINITIONS ////
//////////////////////////////
// Below are various different constant definitions for the compiler to use.

// Shift pins for the 4-wide 7 segment display.
#ifndef SHIFT_PIN_DS
#define SHIFT_PIN_DS (uint8_t)5
#endif

#ifndef SHIFT_PIN_STCP
#define SHIFT_PIN_STCP (uint8_t)6
#endif

#ifndef SHIFT_PIN_SHCP
#define SHIFT_PIN_SHCP (uint8_t)7
#endif

// Pin for the "CS" pin on the SD card module.
#ifndef SD_CS_PIN
#define SD_CS_PIN 53
#endif

// The specifications of the I2C LCD.
#ifndef LCD_COLS
#define LCD_COLS (uint8_t)20
#endif

#ifndef LCD_ROWS
#define LCD_ROWS (uint8_t)4
#endif

#ifndef RGB_BRIGHTNESS
    /*
    The brightness of the RGB LED which serves as a status indicator.
    Numbers should be between 0-255
    */
#define RGB_BRIGHTNESS (uint8_t)200
#endif

#ifndef RGB_RED
    /*The pin of the RED led inside of the RGB status led.*/
#define RGB_RED (uint8_t)9
#endif

#ifndef RGB_GREEN
    /*The pin of the GREEN led inside of the RGB status led.*/
#define RGB_GREEN (uint8_t)10
#endif

#ifndef RGB_BLUE
    /*The pin of the BLUE led inside of the RGB status led.*/
#define RGB_BLUE (uint8_t)11
#endif

// Buttons
// REMEMBER: TuneButtons go from lowest pitch (Green) to highest pitch (White)

#ifndef BTN_TONE_1
    /* Represents the pin of the button with the green led. */
#define BTN_TONE_1 (uint8_t)27
#endif

#ifndef BTN_TONE_2
    /* Represents the pin of the button with the blue led.*/
#define BTN_TONE_2 (uint8_t)25 
#endif

#ifndef BTN_TONE_3
    /*Represents the pin of the button with the red led.*/
#define BTN_TONE_3 (uint8_t)28 
#endif

#ifndef BTN_TONE_4
    /*Represents the pin of the button with the yellow led.*/
#define BTN_TONE_4 (uint8_t)26
#endif

#ifndef BTN_TONE_5
    /*Represents the pin of the button with the white led.*/
#define BTN_TONE_5 (uint8_t)24 
#endif

#ifndef BTN_ADD_SELECT
    /*The pin of the select/confirm button which is also used to add tones.*/
#define BTN_ADD_SELECT (uint8_t)2
#endif

#ifndef BTN_DEL_CANCEL
    /*The pin of the delete button which can be used to delete tones.*/
#define BTN_DEL_CANCEL (uint8_t)3
#endif

#ifndef BTN_OPTION
    /*The pin of the reset button which can be used to delete songs and flush EEPROM.*/
#define BTN_OPTION (uint8_t)4
#endif

#ifndef DEBOUNCE_RATE
/*The rate at which additional button presses should be ignored.*/
#define DEBOUNCE_RATE (uint16_t)1000
#endif

// Potentiometer (Frequency Changer)

#ifndef TONE_FREQ
/* The pin of the potentiometer which is used for frequency adjusting. */
#define TONE_FREQ A7
#endif

// Speakers

#ifndef SPEAKER_1
/* The pin of the passive speaker connected to the Arduino. */
#define SPEAKER_1 (uint8_t)34
#endif

// Audio

/* The maximum length of a song. Increasing this variable may require a change of PRGM_MODE. */
#ifndef MAX_SONG_LENGTH
#define MAX_SONG_LENGTH (uint8_t)255
#endif

/*
The maximum allowed number of songs that can be saved. (microSD only, EEPROM is limited to 5)
NOTE: Changing this value above 255 may cause the program to break due to a limitation using the uint8_t datatype.
Expanding will require the change of this datatype.
*/
#ifndef MAX_SONG_AMOUNT
#define MAX_SONG_AMOUNT (uint8_t)255
#endif


// CUSTOM LCD SYMBOLS use lcd.write(hex num) to write the characters.

/*Custom Char symbol for music note.*/
#define MUSIC_NOTE_SYMBOL 0x00
const byte MUSIC_NOTE[8] PROGMEM = {
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
const byte PLAYING_SONG[8] PROGMEM = {
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
const byte PAUSE_SONG[8] PROGMEM = {
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
const byte PROGRESS_BLOCK[8] PROGMEM = {
  0x1F,
  0x1F,
  0x1F,
  0x1F,
  0x1F,
  0x1F,
  0x1F,
  0x1F
};

///////////////////////////////////////
//// TONES, PITCHES, AND FREQUENCY ////
///////////////////////////////////////
// There are 85 different frequencies which can be used for making songs in TuneStudio2560.
// Each of these is called a "note" which contains the frequency to play the buzzer and a human readable "pitch".
// The pitch is as defined on the musical chromatic scale.
#define TONE_BUTTON_AMOUNT 5
#define TONES_PER_BUTTON 17

/**
 * @brief Represents an individual "note" in TuneStudio which contains
 * both a human readable "pitch" and a 16-bit integer frequency which is played
 * by the buzzer.
 */
typedef struct note {
    const char* pitch;
    const uint16_t frequency;
} note;

/**
 * @brief Represents a tune button as well as all of the different notes that it can play.
 * Each tune button can play 17 different notes in their respective range.
 */
typedef struct buttonFrequencies {
    const uint8_t pin;
    const note notes[TONES_PER_BUTTON];
} buttonFrequencies;


/**
 * @brief An array of all possible tones which can be played.
 * 85 total tones, 17 per button, 5 buttons.
 */
const buttonFrequencies toneButtons[TONE_BUTTON_AMOUNT] PROGMEM{
    {BTN_TONE_1, {{"B0", 31}, {"C1", 33}, {"CS1", 35}, {"D1", 37}, {"DS1", 39}, {"E1", 41}, {"F1", 44}, {"FS1", 46}, {"G1", 49}, {"GS1", 52}, {"A1", 55}, {"AS1", 58}, {"B1", 62}, {"C2", 65}, {"CS2", 69}, {"D2", 73}, {"DS2", 78}}},
    {BTN_TONE_2, {{"E2", 82}, {"F2", 87}, {"FS2", 93}, {"G2", 98}, {"GS2", 104}, {"A2", 110}, {"AS2", 117}, {"B2", 123}, {"C3", 131}, {"CS3", 139}, {"D3", 147}, {"DS3", 156}, {"E3", 165}, {"F3", 175}, {"FS3", 185}, {"G3", 196}, {"GS3", 208}}},
    {BTN_TONE_3, {{"A3", 220}, {"AS3", 233}, {"B3", 247}, {"C4", 262}, {"CS4", 277}, {"D4", 294}, {"DS4", 311}, {"E4", 330}, {"F4", 349}, {"FS4", 370}, {"G4", 392}, {"GS4", 415}, {"A4", 440}, {"AS4", 466}, {"B4", 494}, {"C5", 523}, {"CS5", 554}}},
    {BTN_TONE_4, {{"D5", 587}, {"DS5", 622}, {"E5", 659}, {"F5", 698}, {"FS5", 740}, {"G5", 784}, {"GS5", 831}, {"A5", 880}, {"AS5", 932}, {"B5", 988}, {"C6", 1047}, {"CS6", 1109}, {"D6", 1175}, {"DS6", 1245}, {"E6", 1319}, {"F6", 1397}, {"FS6", 1480}}},
    {BTN_TONE_5, {{"G6", 1568}, {"GS6", 1661}, {"A6", 1760}, {"AS6", 1865}, {"B6", 1976}, {"C7", 2093}, {"CS7", 2217}, {"D7", 2349}, {"DS7", 2489}, {"E7", 2637}, {"F7", 2794}, {"FS7", 2960}, {"G7", 3136}, {"GS7", 3322}, {"A7", 3520}, {"AS7", 3729}, {"B7", 3951}}}
};
/**
 * @brief A note which defines a pause.
 *
 */
#define PAUSE_NOTE note { (const char*)"PS", (const uint16_t)1 }

 ///////////////////////////////
 //// MAIN PROGRAM METHODS ////
 /////////////////////////////
 // The different methods which are used in the main class.
 //TODO: Make LCD an "extern" function once the lcd class is made. Make a getter for segDisplay?

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
 * @return If the interrupt flag is true.
 */
bool is_interrupt();

/**
 * @brief A custom delay function which checks if an immediate interrupt is occuring.
 *
 * Works the same as the normal arduino delay(ms) function just with a custom handler.
 *
 * This function is always inlined and will override the default delay function.
 *
 * @param milliseconds The time to delay for.
 */
inline void delay(unsigned long milliseconds) __attribute__((always_inline));
void delay(unsigned long milliseconds) {
    const unsigned long waitTime = milliseconds + millis();
    while (waitTime > millis() && !is_interrupt()) { // Continue looping forever.
        continue;
    }
}

/**
 * @brief An ISR to handle whenever the select button is pressed throughout the program.
 * Reroutes the state to the respective new state for the button press.
 *
 */
void select_btn_click();

/**
 * @brief An ISR to handle whenever the delete/cancel button is pressed throughout the program.
 * Reroutes the state to the respective new state for the button press.
 *
 */
void cancel_btn_click();

/**
 * @brief Forcefully update the current state.
 *
 * @param newState
 */
void update_state(StateID state);

/**
 * @brief Checks for button presses & updates Debounce rate.
 *
 * @param buttonPin The pin of the button to check for.
 * @return If the button is being pressed.
 */
bool is_pressed(uint8_t buttonPin);

/**
 * @brief Checks for button presses & updates Debounce rate.
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
void print_lcd(const __FlashStringHelper* text, uint8_t charDelay = 150);

/**
 * @brief Prints a single line of scrolling text on the lcd.
 *
 * Note that this method does not clear the screen before executing.
 *
 * @param text The text to print.
 * @param cursorY Where the cursor should start printing.
 * @param charDelay Delay between the scroll.
 */
void print_scrolling(const __FlashStringHelper* text, uint8_t cursorY, uint8_t charDelay);


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
 * @param toneButton The button which was pressed.
 * @return A note struct from the toneButton that was pressed. Finds the note by copying information from PROGMEM.
 */
note get_current_tone(uint8_t toneButton);

/**
 * @brief Get the length of a string stored in flash storage.
 *
 * @param FSHinput The string to check the length of.
 * @return unsigned int
 */
uint16_t FSHlength(const __FlashStringHelper* FSHinput);

/**
 * @brief Retrieve a note object that matches a specified frequency.
 *
 * @param frequency The frequency to search for.
 * @return note The note which has the frequency.
 */
note get_note_from_freq(const uint16_t frequency);

/**
 * @brief Retrieve a note object from a specified pitch.
 *
 * @param pitch The pitch string to search for.
 * @return note The note which matches the frequency.
 */
note get_note_from_pitch(const char* pitch);
#endif