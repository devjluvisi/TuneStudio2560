/**
 * @file tune_studio.h
 * @author Jacob LuVisi
 * @brief The main header file for the application. Includes important variables and methods that are shared between all TuneStudio files.
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
#include <LiquidCrystal_I2C.h> // Manages basic LCD functionality.
#include <SevSegShift.h> // Manages connecting the Shift Registers to the Segment Display.
#include <Arduino.h> // The basic Arduino library.
#include <studio-libs/song.h> // Manage songs.
#include <SD.h> // Manage the SD Card.
#include <SPI.h> // For the SD card library.
#include <studio-libs/state.h> // Manages the variety of differnet states the program can be running in.

/**
 * Enable/Disable the DEBUG functionallity of TuneStudio2560.
 * Enabling this will: Enable Serial Monitor, prints debugging messages indicating when sections of the code are reached.
 */
#define DEBUG true
 /**
  * Enable/Disable performance metrics for TuneStudio2560.
  * Enabling this will: Enable serial console prints on the current Ram Usage, Stack Size, and Heap Fragmentation, as well as provide
  * metrics on how fast certain functions are executing in micro and milliseconds as well as the change since last execute.
  *
  * NOTE: Enabling performance metrics REQUIRES debug to be true.
  */
#define PERF_METRICS true

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

// Define a song class so the header file knows it exists.
class Song;

const char VERSION_ID[] PROGMEM = "1.0.0-R1";

/*
************************
*** Application Pins ***
************************
*/

// Shift pins for the 4-wide 7 segment display.
constexpr uint8_t SHIFT_PIN_DS = 5;
constexpr uint8_t SHIFT_PIN_STCP = 6;
constexpr uint8_t SHIFT_PIN_SHCP = 7;

// Pin for the "CS" pin on the SD card module.
constexpr uint8_t SD_CS_PIN = 53;

/*
The brightness of the RGB LED which serves as a status indicator.
Numbers should be between 0-255
*/
constexpr uint8_t RGB_BRIGHTNESS = 200;

// Pin of the "RED" led on the RGB status led.
constexpr uint8_t RGB_RED = 9;
// Pin of the "GREEN" led on the RGB status led.
constexpr uint8_t RGB_GREEN = 10;
// Pin of the "BLUE" led on the RGB status led.
constexpr uint8_t RGB_BLUE = 11;

/*
Tone Buttons
Tone Buttons go from deepest pitch (green) to highest pitch (white).
 */

 // Green LED
constexpr uint8_t BTN_TONE_1 = 27;
// Blue LED
constexpr uint8_t BTN_TONE_2 = 25;
// Red LED
constexpr uint8_t BTN_TONE_3 = 28;
// Yellow LED
constexpr uint8_t BTN_TONE_4 = 26;
// White LED
constexpr uint8_t BTN_TONE_5 = 24;

// Interaction Buttons

constexpr uint8_t BTN_ADD_SELECT = 2;
constexpr uint8_t BTN_DEL_CANCEL = 3;
constexpr uint8_t BTN_OPTION = 4;

// Potentiometer
constexpr uint8_t TONE_FREQ = PIN_A7;
// Speaker
constexpr uint8_t SPEAKER_1 = 34;

/* General Definitions */

// How many columns the LCD has.
constexpr uint8_t LCD_COLS = 20;
// How many rows the LCD has.
constexpr uint8_t LCD_ROWS = 4;

// The delay between reading each button press.
constexpr uint16_t DEBOUNCE_RATE = 500;

// Maximum length of songs.
constexpr uint8_t MAX_SONG_LENGTH = 255;
// Maximum allowed number of songs.
constexpr uint8_t MAX_SONG_AMOUNT = 255;

/*
*************************
*** Custom Characters ***
*************************
*/

/*Custom Char symbol for music note.*/
constexpr uint8_t MUSIC_NOTE_SYMBOL = 0x00;
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
constexpr uint8_t PLAYING_SONG_SYMBOL = 0x01;
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
constexpr uint8_t PAUSE_SONG_SYMBOL = 0x02;
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
constexpr uint8_t PROGRESS_BLOCK_SYMBOL = 0x03;
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
/*Custom char symbol for when a song is finished*/
constexpr uint8_t FINISHED_SONG_SYMBOL = 0x04;
const byte FINISHED_SONG[8] PROGMEM = {
  0x1F,
  0x1F,
  0x1F,
  0x10,
  0x10,
  0x10,
  0x10,
  0x10
};

/* Custom char symbol for an unfilled progress block */
constexpr uint8_t PROGRESS_BLOCK_UNFILLED_SYMBOL = 0x05;
const byte PROGRESS_BLOCK_UNFILLED[8] PROGMEM = {
  0x1F,
  0x11,
  0x11,
  0x11,
  0x11,
  0x11,
  0x11,
  0x1F
};
/*
*************************************
*** Tones, Pitches, and Frequency ***
*************************************
*/

// There are 85 different frequencies which can be used for making songs in TuneStudio2560.
// Each of these is called a "note" which contains the frequency to play the buzzer and a human readable "pitch".
// The pitch is as defined on the musical chromatic scale.

constexpr uint8_t TONE_BUTTON_AMOUNT = 5;
constexpr uint8_t TONES_PER_BUTTON = 17;

constexpr uint8_t DEFAULT_NOTE_DELAY = 80;
constexpr uint8_t DEFAULT_NOTE_LENGTH = 50;

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
 * TODO: Move to PROGMEM
 */
const note PAUSE_NOTE = { (const char*)"PS", (const uint16_t)1 };
const note EMPTY_NOTE = { (const char*)"0000", (const uint16_t)0 };
/*
*********************************
*** Program Methods & Globals ***
*********************************
*/

const char ROOT_DIR[] = "/";
const char PROGRESS_LABEL[] = "PROGRESS:";
const char FILE_TXT_EXTENSION[] = ".txt";
const char optionalCharacters[] PROGMEM = { 'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z', '0','1','2','3','4','5','6','7','8','9', '_' };

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
inline void delay(const unsigned long milliseconds) __attribute__((always_inline));
void delay(const unsigned long milliseconds) {
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

/**
 * @brief Clears a row on the LCD.
 *
 * @param row The row to clear.
 */
void lcd_clear_row(uint8_t row);

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

/*
SD CARD FUNCTIONS
*/

/**
 * @brief Saves a song class to the SD card by using the SD.h library and writing all of
 * the tones from the song to the SD card. Allows the saving with a specified file name. File name cannot
 * be longer then 8 characters and should include a .TXT extension.
 *
 * @param fileName The name to save the song as.
 * @param song The song to save.
 */
void sd_save_song(char* fileName, Song* song);

/**
 * @brief Gets a file from the SD card in descending order. For example index "0" would be the file at the top of the SD card.
 *
 * @param index The index of the file to get.
 * @return The name of the file (includes the file extension)
 */
const char* sd_get_file(uint8_t index);

/**
 * @brief Copies song data from a .txt file onto a song pointer. Note that this only reads the file and copies its frequencies.
 * The file must be in the correct format to work properly.
 *
 * @param song The song to copy to.
 * @param fileName The path in the SD card.
 *
 * @return If the song was able to be successfully copied.
 */
bool sd_songcpy(Song* song, const char* fileName);

/**
 * @brief Delete a file from the microSD.
 *
 * @param fileName The file to delete.
 */
void sd_rem(const char* fileName);

/**
 * @brief Generates a README file in the SD card.
 *
 */
void sd_make_readme();

#if DEBUG == true
/**
 * @brief Get the current time that the arduino has been running in
 * hours, minutes, and seconds.
 */
const char* get_active_time();

#endif

#endif