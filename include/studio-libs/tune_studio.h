/**
 * @file tune_studio.h
 * @author Jacob LuVisi
 * @brief The main header file for the application. Includes important variables and methods that are shared between all TuneStudio files.
 * Note that method definitions described in this file are declerations for main.cpp methods only.
 * @version 1.2.1-R3
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
#include <studio-libs/song.h> // Manage songs.
#include <studio-libs/state.h> // Manages the variety of differnet states the program can be running in.
#include <lib/digitalWriteFast.h>
#include <studio-libs/pitches.h>
#include <studio-libs/song.h>


/**
 * Enable/Disable the DEBUG functionallity of TuneStudio2560.
 * Enabling this will: Enable Serial Monitor, prints debugging messages indicating when sections of the code are reached.
 */

#define DEBUG false
 /**
  * Enable/Disable performance metrics for TuneStudio2560.
  * Enabling this will: Enable serial console prints on the current Ram Usage, Stack Size, and Heap Fragmentation, as well as provide
  * metrics on how fast certain functions are executing in micro and milliseconds as well as the change since last execute.
  *
  * NOTE: Enabling performance metrics REQUIRES debug to be true.
  */
#define PERF_METRICS false

  // Program Mode Selection
  // Change the performance, SRAM, and flash usage by adjusting.
  // 0 = Performance Favor (Reduced Variables & Song Sizes, Cutdown code)
  // 1 = Standard (How T2560 was intended.)
  // 2 = Feature (Increased song sizes, extra features)
#define PRGM_MODE 0



// Set FAST_ADC to 1 for faster analogRead(). 
// 5-6x faster analogRead but takes 30 more bytes of flash.
// Increases creator mode iterations per second by around 300-400.
#define FAST_ADC (PRGM_MODE == 2 ? true : false)
#if FAST_ADC
#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif
#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif
#endif


//////////////////////////////
//// COMPILER DEFINITIONS ////
//////////////////////////////
// Below are various different constant definitions for the compiler to use.

/**
 * The current version of the Program.
 * How the versions work:
 * (Major Version Release).(Minor Code Updates since last release).(Minor Updates)-R(Release Number)
 * Minor Updates refer to updates which fix bugs or do optimizations. Sometimes minor updates may add function as well.
 * Minor Code Updates occur when enough minor updates have been made to justify a new release.
 */
const char VERSION_ID[] PROGMEM = "1.2.1-R3";

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

/**
 * @brief Converts a button pin to a pin index.
 *  Ex: BTN_TONE_1 is pin 27 (by default) if it is passed then the macro will return 0.
 *  Unknown values are given 255.
 * 
 */
constexpr uint8_t BTN_TO_INDEX(uint8_t x) { return x == BTN_TONE_1 ? 0 : x == BTN_TONE_2 ? 1 : x == BTN_TONE_3 ? 2 : x == BTN_TONE_4 ? 3 : x == BTN_TONE_5 ? 4 : UINT8_MAX; }

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

// Maximum number of notes in a song.
// Note! Adjusting requires the editing of the song_size_t typedef in "song.h"
#if PRGM_MODE == 0
constexpr song_size_t MAX_SONG_LENGTH = 64;
#elif PRGM_MODE == 2
constexpr song_size_t MAX_SONG_LENGTH = 512;
#else
constexpr song_size_t MAX_SONG_LENGTH = 255;
#endif

// Minimum number of notes in a song.
constexpr uint8_t MIN_SONG_LENGTH = 8;
// Maximum allowed number of songs.
constexpr uint8_t MAX_SONG_AMOUNT = 255;

/*
*************************
*** Custom Characters ***
*************************
*/

/**
 * [v1.1.0-R2]
 * A PROGMEM list of bytes that represent custom characters which should be loaded into the LCD.
 * 
 */
const byte CUSTOM_LCD_CHARS[6][8] PROGMEM = {
{0x04, 0x06, 0x05, 0x04, 0x04, 0x0C, 0x1C, 0x0C},
{0x10, 0x18, 0x1C, 0x1E, 0x1E, 0x1C, 0x18, 0x10},
{0x00, 0x1B, 0x1B, 0x1B, 0x1B, 0x1B, 0x1B, 0x00},
{0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F},
{0x1F, 0x1F, 0x1F, 0x10, 0x10, 0x10, 0x10, 0x10},
{0x1F, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x1F}
};

// Amount of custom characters we want to load.
constexpr uint8_t CUSTOM_CHAR_AMOUNT = sizeof(CUSTOM_LCD_CHARS)/sizeof(CUSTOM_LCD_CHARS[0]);

/*Custom Char symbol for music note.*/
constexpr uint8_t MUSIC_NOTE_SYMBOL = 0x00;

/*Custom Char symbol for a playing song symbol.*/
constexpr uint8_t PLAYING_SONG_SYMBOL = 0x01;

/*Custom Char symbol for a paused song symbol.*/
constexpr uint8_t PAUSE_SONG_SYMBOL = 0x02;

/*Custom Char symbol for a progress block.*/
constexpr uint8_t PROGRESS_BLOCK_SYMBOL = 0x03;

/*Custom char symbol for when a song is finished*/
constexpr uint8_t FINISHED_SONG_SYMBOL = 0x04;

/* Custom char symbol for an unfilled progress block */
constexpr uint8_t PROGRESS_BLOCK_UNFILLED_SYMBOL = 0x05;
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
  const char * const pitch;
  const uint16_t frequency;
} note_t;

/**
 * @brief Represents a tune button as well as all of the different notes that it can play.
 * Each tune button can play 17 different notes in their respective range.
 */
typedef struct buttonFrequencies {
  const uint8_t pin;
  const note_t notes[TONES_PER_BUTTON];
} buttonFrequencies_t;


/**
 * @brief An array of all possible tones which can be played.
 * 85 total tones, 17 per button, 5 buttons.
 * 
 * TODO: Make method in main.cpp which takes a constant char[5] from memory and converts it to a const char pointer like the program can read.
 * Copy the length 5 char array from Program Memory and convert it to a pointer and pass it.
 */
const buttonFrequencies_t PROGRAM_NOTES[TONE_BUTTON_AMOUNT] PROGMEM{
    {BTN_TONE_1, {{pitch_b0, 31}, {pitch_c1, 33}, {pitch_cs1, 35}, {pitch_d1, 37}, {pitch_ds1, 39}, {pitch_e1, 41}, {pitch_f1, 44}, {pitch_fs1, 46}, {pitch_g1, 49}, {pitch_gs1, 52}, {pitch_a1, 55}, {pitch_as1, 58}, {pitch_b1, 62}, {pitch_c2, 65}, {pitch_cs2, 69}, {pitch_d2, 73}, {pitch_ds2, 78}}},
    {BTN_TONE_2, {{pitch_e2, 82}, {pitch_f2, 87}, {pitch_fs2, 93}, {pitch_g2, 98}, {pitch_gs2, 104}, {pitch_a2, 110}, {pitch_as2, 117}, {pitch_b2, 123}, {pitch_c3, 131}, {pitch_cs3, 139}, {pitch_d3, 147}, {pitch_ds3, 156}, {pitch_e3, 165}, {pitch_f3, 175}, {pitch_fs3, 185}, {pitch_g3, 196}, {pitch_gs3, 208}}},
    {BTN_TONE_3, {{pitch_a3, 220}, {pitch_as3, 233}, {pitch_b3, 247}, {pitch_c4, 262}, {pitch_cs4, 277}, {pitch_d4, 294}, {pitch_ds4, 311}, {pitch_e4, 330}, {pitch_f4, 349}, {pitch_fs4, 370}, {pitch_g4, 392}, {pitch_gs4, 415}, {pitch_a4, 440}, {pitch_as4, 466}, {pitch_b4, 494}, {pitch_c5, 523}, {pitch_cs5, 554}}},
    {BTN_TONE_4, {{pitch_d5, 587}, {pitch_ds5, 622}, {pitch_e5, 659}, {pitch_f5, 698}, {pitch_fs5, 740}, {pitch_g5, 784}, {pitch_gs5, 831}, {pitch_a5, 880}, {pitch_as5, 932}, {pitch_b5, 988}, {pitch_c6, 1047}, {pitch_cs6, 1109}, {pitch_d6, 1175}, {pitch_ds6, 1245}, {pitch_e6, 1319}, {pitch_f6, 1397}, {pitch_fs6, 1480}}},
    {BTN_TONE_5, {{pitch_g6, 1568}, {pitch_gs6, 1661}, {pitch_a6, 1760}, {pitch_as6, 1865}, {pitch_b6, 1976}, {pitch_c7, 2093}, {pitch_cs7, 2217}, {pitch_d7, 2349}, {pitch_ds7, 2489}, {pitch_e7, 2637}, {pitch_f7, 2794}, {pitch_fs7, 2960}, {pitch_g7, 3136}, {pitch_gs7, 3322}, {pitch_a7, 3520}, {pitch_as7, 3729}, {pitch_b7, 3951}}}
};



/**
 * @brief A note which defines a pause.
 * TODO: Move to PROGMEM
 */
const note_t PAUSE_NOTE = { "PS", (const uint16_t)1 };
const note_t EMPTY_NOTE = { "0000", (const uint16_t)0 };
/*
*********************************
*** Program Methods & Globals ***
*********************************
*/

const char ROOT_DIR[] = "/";
const char PROGRESS_LABEL[] PROGMEM = "PROGRESS:";
const char FILE_TXT_EXTENSION[] = ".txt";
const char README_FILE[] = "README.TXT";
const char OPTIONAL_NAMING_CHARACTERS[] PROGMEM = { 'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z', '0','1','2','3','4','5','6','7','8','9', '_' };

/**
 * @brief The primary LCD object which controls the main LCD.
 *
 */
extern LiquidCrystal_I2C lcd;

/**
 * @brief Represents the 4-wide 7 segment display which is connected to 2x SN74HC595N shift registers.
 */
extern SevSegShift segDisplay;

/**
 * @since v1.2.0-R3
 * Songs are no longer pointers and are no longer dynamically allocated.
 * Any song the program is going to use is now accessible by the "prgmSong" global variable.
 * 
 * @brief Represents a song object to be used in the program.
 * This song object is used between program states and can be accessed at any time.
 * When a new song wants to be used by the application, the song is cleared using .clear() and notes are re-added.
 * 
 */
extern Song<MAX_SONG_LENGTH> prgmSong;

// Try max SPI clock for an SD. Reduce SPI_CLOCK if errors occur.
#define SPI_CLOCK SD_SCK_MHZ(1)

// Try to select the best SD card configuration.
#if HAS_SDIO_CLASS
#define SD_CONFIG SdioConfig(FIFO_SDIO)
#elif  ENABLE_DEDICATED_SPI
#define SD_CONFIG SdSpiConfig(SD_CS_PIN, DEDICATED_SPI, SPI_CLOCK)
#else  // HAS_SDIO_CLASS
#define SD_CONFIG SdSpiConfig(SD_CS_PIN, SHARED_SPI, SPI_CLOCK)
#endif  // HAS_SDIO_CLASS

/**
 * @return If the interrupt flag is true.
 */
bool is_interrupt();

/**
 * @brief A custom (blocking) delay function which checks if an immediate interrupt is occuring.
 * Works the same as the normal arduino delay(ms) function just with a custom handler.
 *
 * @param milliseconds The time to delay for.
 */
void delay_ms(const unsigned long milliseconds);

/**
 * @brief Handles any interrupts which could be generated by the add/select or del/cancel buttons.
 * Whether or not the immediateInterrupt variable should be toggled on depends on the current 
 * ProgramState and how it wants to handle interrupts.
 */
void isr_btn_handle();

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
bool is_pressed(const uint8_t buttonPin);

/**
 * @brief Prints text to the lcd and wraps text automatically.
 *
 *  Will clear the screen on a \\n character encounter.
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
note_t get_current_tone(uint8_t toneButton);

/**
 * @brief Retrieve a note object that matches a specified frequency.
 *
 * @param frequency The frequency to search for.
 * @return The note which has the frequency.
 */
note_t get_note_from_freq(const uint16_t frequency);

/**
 * @brief Retrieve a note object from a specified pitch.
 *
 * @param pitch The pitch string to search for.
 * @return The note which matches the frequency.
 */
note_t get_note_from_pitch(const char* const pitch);

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
void sd_save_song(const char * const fileName, Song<MAX_SONG_LENGTH> song);

/**
 * @brief Gets a file name from the SD card in descending order according to a specified index. For example index "0" would be the file at the top of the SD card.
 *
 * @param index The index of the file to get.
 * @return The name of the file (includes the file extension)
 */
const char* sd_get_file(uint8_t index);

/**
 * @brief Copies and parses song data from a .txt file onto the global song object.
 * The file must be in the correct format to work properly.
 *
 * @param song The song to copy to.
 * @param fileName The path in the SD card.
 *
 * @return If the song was able to be successfully copied.
 */
bool sd_songcpy(Song<MAX_SONG_LENGTH> &song, const char * const fileName);

/**
 * @brief Delete a file from the microSD.
 *
 * @param fileName The file to delete.
 */
void sd_rem(const char* const fileName);


/**
 * @brief Generates a README file in the SD card.
 *
 */
void sd_make_readme();


/**
 * Method Naming Logic:
 * pgm -> Has to do with PROGMEM space.
 * p -> Uses a pitch.
 * cpy -> Copies data.
 * r -> Returns copied data.
 * 
 * @brief [v1.2.0-R3] Copies the string data from a pointer to a pitch cstring from PROGMEM and then
 * returns the string data for that pointer.
 * 
 * After the pointer is retrieved from PROGMEM the value is stored in a private static buffer array which can be used
 * by other methods.
 * 
 * Ex: pgm_pcpyr(0, 0) -> "B0"
 * 
 * @param btnIndex The button to get in the PROGRAM_NOTES array.
 * @param noteIndex The note to get for that button.
 * @return The data which was copied from the pointer.
 */
const char* pgm_pcpyr(uint8_t btnIndex, uint8_t noteIndex);

#if DEBUG == true
/**
 * @brief Get the current time that the arduino has been running in
 * hours, minutes, and seconds.
 */
const char* get_active_time();

#endif

#endif