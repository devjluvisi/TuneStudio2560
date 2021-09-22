/**
 * @file tune_studio.h
 * @author Jacob LuVisi
 * @brief The main header file for the application. Includes important variables and methods that are shared between all TuneStudio files. <br />
 * Note that method definitions described in this file are declerations for main.cpp methods only.
 * @version 1.3.0-R5
 * @date 2021-07-26
 *
 * @copyright Copyright (c) 2021
 *
 */

 // Header Guard
#ifndef tune_studio_h
#define tune_studio_h

#include <LiquidCrystal_I2C.h> 
#include <SevSegShift.h>
#include <studio-libs/song.h>
#include <studio-libs/state.h> 
#include <lib/digitalWriteFast.h>
#include <studio-libs/pitches.h>
#include <studio-libs/song.h>

/**
 * @brief Enable/Disable the DEBUG functionallity of TuneStudio2560.<br/>
 * Enabling this will: Enable Serial Monitor, prints debugging messages indicating when sections of the code are reached.
 */
#define DEBUG false

 /**
  * @brief Enable/Disable performance metrics for TuneStudio2560.<br/>
  * Enabling this will: Enable serial console prints on the current Ram Usage, Stack Size, and Heap Fragmentation, as well as provide
  * metrics on how fast certain functions are executing in micro and milliseconds as well as the change since last execute.<br/><br/>
  *
  * <b>NOTE:</b> Enabling performance metrics REQUIRES debug to be true.
  */
#define PERF_METRICS false

/**
 * @brief Select a mode for the program to run in.
 * <br />
 * TuneStudio2560 allows the program to run in different "performance modes". <br />
 * Mode 0 = Small Code Size <br />
 * Mode 1 = Balanced <br />
 * Mode 2 = High Features, Larger Songs <br />
 * 
 */
#define PRGM_MODE 1


/**
 * @brief A macro for defining if fast analog read should be used.
 * <br />
 * Faster Analog Read takes up around 30-40 more bytes of flash but allows much faster analog read. <br />
 * Increases the iterations per second (IPS) of creator mode by around 300-400.
 * <br /><br />
 * <i>Automatically enabled in PRGM_MODE == 2</i>
 */
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
 * Represents the current version of the program. <br />
 * (Major Version Release).(Minor Code Updates since last release).(Minor Updates)-R(Release Number) <br />
 * Minor Updates refer to updates which fix bugs or do optimizations. Sometimes minor updates may add function as well. <br />
 * Minor Code Updates occur when enough minor updates have been made to justify a new release. <br />
 */
const char VERSION_ID[] PROGMEM = "1.3.0-R5";

/** @brief DS pin for the Shift Register connected to the 4 Digit 7-Segment Display.*/
constexpr uint8_t SHIFT_PIN_DS = 5;
/** @brief STCP pin for the Shift Register connected to the 4 Digit 7-Segment Display.*/
constexpr uint8_t SHIFT_PIN_STCP = 6;
/** @brief SHCP pin for the Shift Register connected to the 4 Digit 7-Segment Display.*/
constexpr uint8_t SHIFT_PIN_SHCP = 7;

/** @brief Pin connected to the "CS" pin on the SD Card Module */
constexpr uint8_t SD_CS_PIN = 53;

/** @brief A brightness indicator for how bright the RGB Led should light up. <br />Irrelevant for PRGM_MODE == 0 because digitalWrite is used instead of analog. */
constexpr uint8_t RGB_BRIGHTNESS = 200;

/** @brief Pin for the "RED" pin on the RGB Led. */
constexpr uint8_t RGB_RED = 9;
/** @brief Pin for the "GREEN" pin on the RGB Led. */
constexpr uint8_t RGB_GREEN = 10;
/** @brief Pin for the "BLUE" pin on the RGB Led. */
constexpr uint8_t RGB_BLUE = 11;


/** @brief The pin which is connected to the Green LED (First Tone Button) (Lowest Pitch) */
constexpr uint8_t BTN_TONE_1 = 27;
/** @brief The pin which is connected to the Blue LED (Second Tone Button) */
constexpr uint8_t BTN_TONE_2 = 25;
/** @brief The pin which is connected to the Red LED (Third Tone Button) */
constexpr uint8_t BTN_TONE_3 = 28;
/** @brief The pin which is connected to the Yellow LED (Fourth Tone Button) */
constexpr uint8_t BTN_TONE_4 = 26;
/** @brief The pin which is connected to the White LED (Fifth Tone Button) (Highest Pitch) */
constexpr uint8_t BTN_TONE_5 = 24;

/**
 * Converts a button pin to a pin index. 
 * <br />
 *  Ex: BTN_TONE_1 is pin 27 (by default) if it is passed then the macro will return 0. 
 * <br />
 *  Unknown values are given 255.
 */
constexpr uint8_t BTN_TO_INDEX(uint8_t x) { return x == BTN_TONE_1 ? 0 : x == BTN_TONE_2 ? 1 : x == BTN_TONE_3 ? 2 : x == BTN_TONE_4 ? 3 : x == BTN_TONE_5 ? 4 : UINT8_MAX; }

/** @brief The pin which is connected to the Add/Select button. */
constexpr uint8_t BTN_ADD_SELECT = 2;
/** @brief The pin which is connected to the Del/Cancel button. */
constexpr uint8_t BTN_DEL_CANCEL = 3;
/** @brief The pin which is connected to the Option button. */
constexpr uint8_t BTN_OPTION = 4;

/** @brief The pin which is connected to the potentiometer / tone frequency adjuster. */
constexpr uint8_t TONE_FREQ = PIN_A7;

/** @brief The pin which is connected to the primary passive speaker. */
constexpr uint8_t SPEAKER_1 = 34;

/** @brief How many columns the connected I2C LCD has. */
constexpr uint8_t LCD_COLS = 20;
/** @brief How many rows the connected I2C LCD has. */
constexpr uint8_t LCD_ROWS = 4;

/** @brief The delay between when button presses should be read by the program. @see is_pressed **/
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

/** @brief Minimum allowed number of notes in a song for playback and saving */
constexpr uint8_t MIN_SONG_LENGTH = 8;

/** @brief Maximum allowed number of songs on the SD card. */
constexpr uint8_t MAX_SONG_AMOUNT = 255;

/**
 * @since [v1.1.0-R2]
 * @brief A PROGMEM list of bytes that represent custom characters which should be loaded into the LCD.
 * @see setup()
 */
const PROGMEM byte CUSTOM_LCD_CHARS[6][8] = {
{0x04, 0x06, 0x05, 0x04, 0x04, 0x0C, 0x1C, 0x0C},
{0x10, 0x18, 0x1C, 0x1E, 0x1E, 0x1C, 0x18, 0x10},
{0x00, 0x1B, 0x1B, 0x1B, 0x1B, 0x1B, 0x1B, 0x00},
{0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F},
{0x1F, 0x1F, 0x1F, 0x10, 0x10, 0x10, 0x10, 0x10},
{0x1F, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x1F}
};

/** @brief The amount of custom characters that we want to load onto the LCD. */
constexpr uint8_t CUSTOM_CHAR_AMOUNT = sizeof(CUSTOM_LCD_CHARS)/sizeof(CUSTOM_LCD_CHARS[0]);

/** @brief The hex integer which represents the music note symbol on the lcd. */
constexpr uint8_t MUSIC_NOTE_SYMBOL = 0x00;

/** @brief The hex integer which represents the playing song symbol on the lcd. */
constexpr uint8_t PLAYING_SONG_SYMBOL = 0x01;

/** @brief The hex integer which represents the pause song symbol on the lcd. */
constexpr uint8_t PAUSE_SONG_SYMBOL = 0x02;

/** @brief The hex integer which represents the progress block symbol on the lcd. */
constexpr uint8_t PROGRESS_BLOCK_SYMBOL = 0x03;

/** @brief The hex integer which represents the finished song symbol on the lcd. */
constexpr uint8_t FINISHED_SONG_SYMBOL = 0x04;

/** @brief The hex integer which represents the unfilled progress block symbol on the lcd. */
constexpr uint8_t PROGRESS_BLOCK_UNFILLED_SYMBOL = 0x05;

//////////////////////////////////////////
//// TONES, PITHCHES, AND FREQUENCIES ////
//////////////////////////////////////////

// There are 85 different frequencies which can be used for making songs in TuneStudio2560.
// Each of these is called a "note" which contains the frequency to play the buzzer and a human readable "pitch".
// The pitch is as defined on the musical chromatic scale.

/** @brief The amount of tone buttons the program has. */
constexpr uint8_t TONE_BUTTON_AMOUNT = 5;
/** @brief The amount of tones that each button has. */
constexpr uint8_t TONES_PER_BUTTON = 17;

/** @brief The default note delay in the program. */
constexpr uint8_t DEFAULT_NOTE_DELAY = 80;
/** @brief The default note length in the program. */
constexpr uint8_t DEFAULT_NOTE_LENGTH = 50;

/**
 * @brief Represents a structure of a "note" in the program. <i>(Used as note_t)</i>
 * @brief
 * Notes are defined as having both a human readable (char string pointer) pitch and a 16-bit integer which represents the frequency of the
 * note to be played by the passive buzzer.
 * 
 * @see buttonFrequencies
 */
typedef struct note {
  /** @brief A pointer to a char array representing a human readable pitch (Like GS2) */
  const char * const pitch;
  /** @brief A 16-Bit integer which corresponds with the pitch. (Ex. 104) */
  const uint16_t frequency;
} note_t;

/**
 * @brief Represents a structure for each different tune button in the program.
 * @brief Each buttonFrequency has a "pin" describing the physical connection to the arduino,
 * and an array of notes which stores all of the possible notes for that tune button.
 * 
 */
typedef struct buttonFrequencies {
  /** @brief Physical connection to the arduino for a tune button. */
  const uint8_t pin;
  /** @brief The array of notes which the tune button represents. */
  const note_t notes[TONES_PER_BUTTON];
} buttonFrequencies_t;

/**
 * @brief An array of all possible tones which can be played.
 * @brief 85 total tones, 17 per button, 5 buttons.
 * 
 * @see pitches.h
 */
const buttonFrequencies_t PROGMEM PROGRAM_NOTES[TONE_BUTTON_AMOUNT] {
    {BTN_TONE_1, {{pitch_b0, 31}, {pitch_c1, 33}, {pitch_cs1, 35}, {pitch_d1, 37}, {pitch_ds1, 39}, {pitch_e1, 41}, {pitch_f1, 44}, {pitch_fs1, 46}, {pitch_g1, 49}, {pitch_gs1, 52}, {pitch_a1, 55}, {pitch_as1, 58}, {pitch_b1, 62}, {pitch_c2, 65}, {pitch_cs2, 69}, {pitch_d2, 73}, {pitch_ds2, 78}}},
    {BTN_TONE_2, {{pitch_e2, 82}, {pitch_f2, 87}, {pitch_fs2, 93}, {pitch_g2, 98}, {pitch_gs2, 104}, {pitch_a2, 110}, {pitch_as2, 117}, {pitch_b2, 123}, {pitch_c3, 131}, {pitch_cs3, 139}, {pitch_d3, 147}, {pitch_ds3, 156}, {pitch_e3, 165}, {pitch_f3, 175}, {pitch_fs3, 185}, {pitch_g3, 196}, {pitch_gs3, 208}}},
    {BTN_TONE_3, {{pitch_a3, 220}, {pitch_as3, 233}, {pitch_b3, 247}, {pitch_c4, 262}, {pitch_cs4, 277}, {pitch_d4, 294}, {pitch_ds4, 311}, {pitch_e4, 330}, {pitch_f4, 349}, {pitch_fs4, 370}, {pitch_g4, 392}, {pitch_gs4, 415}, {pitch_a4, 440}, {pitch_as4, 466}, {pitch_b4, 494}, {pitch_c5, 523}, {pitch_cs5, 554}}},
    {BTN_TONE_4, {{pitch_d5, 587}, {pitch_ds5, 622}, {pitch_e5, 659}, {pitch_f5, 698}, {pitch_fs5, 740}, {pitch_g5, 784}, {pitch_gs5, 831}, {pitch_a5, 880}, {pitch_as5, 932}, {pitch_b5, 988}, {pitch_c6, 1047}, {pitch_cs6, 1109}, {pitch_d6, 1175}, {pitch_ds6, 1245}, {pitch_e6, 1319}, {pitch_f6, 1397}, {pitch_fs6, 1480}}},
    {BTN_TONE_5, {{pitch_g6, 1568}, {pitch_gs6, 1661}, {pitch_a6, 1760}, {pitch_as6, 1865}, {pitch_b6, 1976}, {pitch_c7, 2093}, {pitch_cs7, 2217}, {pitch_d7, 2349}, {pitch_ds7, 2489}, {pitch_e7, 2637}, {pitch_f7, 2794}, {pitch_fs7, 2960}, {pitch_g7, 3136}, {pitch_gs7, 3322}, {pitch_a7, 3520}, {pitch_as7, 3729}, {pitch_b7, 3951}}}
};

/**
 * @brief A note which defines a pause.
 * @brief
 * Pitch: "PS"
 * <br />
 * Frequency: 1hz (Arbitrary as the frequency does not matter as long as it does not play a sound)
 */
const note_t PAUSE_NOTE = { "PS", (const uint16_t)1 };

/**
 * @brief A note which defines an empty space, usually representing the end of the song.
 * @brief
 * Pitch: "0000"
 * <br />
 * Frequency: 0hz (Arbitrary as the frequency does not matter as long as it does not play a sound)
 */
const note_t EMPTY_NOTE = { "0000", (const uint16_t)0 };

////////////////////////////////////
//// PROGRAM METHODS & GLOBALS ////
//////////////////////////////////

/** @brief Root Directory of the SD Card (Non-PROGMEM) */
const char ROOT_DIR[] = "/";
/** @brief A string representing "PROGRESS:". Used for listening mode. */
const PROGMEM char PROGRESS_LABEL[] = "PROGRESS:";
/** @brief A string representing the .txt file extension, the primary extension used for storing songs (Non-PROGMEM). */
const char FILE_TXT_EXTENSION[] = ".txt";
/** @brief A string representing the README file (Non-PROGMEM). */
const char README_FILE[] = "README.TXT";
/** 
 * @brief A char array of all possible characters which can be used when naming a song.
 * @brief <b>Valid Characters are:</b>
 * - A-Z<br/>
 * - 0-9<br />
 * - Underscores (_)
 */
const PROGMEM char OPTIONAL_NAMING_CHARACTERS[] = { 'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z', '0','1','2','3','4','5','6','7','8','9', '_' };

/**
 * @brief Represents a global instance of the Liquid Crystal display used for TuneStudio2560.
 * @brief
 * The Liquid Crystal display uses the I2C protocol and is responsible for direct user feedback on using the program.<br />
 * The display should be the recommended <b>20 columns and 4 rows</b>.<br />
 * <br />
 * Editing TuneStudio2560 to accomidate smaller displays is possible by adjusting the LCD_COLS and LCD_ROWS but some changes to setCursor(x, x) methods would
 * need to be done.
 */
extern LiquidCrystal_I2C lcd;

/**
 * @brief Represents the 4-digit-wide 7-segment display used in TuneStudio2560.
 * @brief
 * The segment display is reponsible for showing the current note in Creator Mode. <br />
 * The display is wired using the SevSegShift library by being connected to <b>2x 74HC595N Shift Registers</b>.
 */
extern SevSegShift segDisplay;

/**
 * @brief Represents the global song object to be used in the program when managing songs.
 * @brief 
 * This song object is used between program states and can be accessed at any time. <br />
 * When a new song wants to be used by the application, the song is cleared using .clear() and notes are re-added.
 * 
 * @since v1.2.0-R3
 * Songs are no longer pointers and are no longer dynamically allocated.
 * Any song the program is going to use is now accessible by the "prgmSong" global variable.
 */
extern Song<MAX_SONG_LENGTH> prgmSong;

/** @brief The SPI_CLOCK as defined by the SdFat library. Set to 1MHz for maximum compatiblity (can go up to 20MHz for good modules and sd cards). */
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
 * This method call should be the last one during a states loop() iteration and no code should come after this function call.
 *
 * @param newState The new state to set the application to.
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
 * @brief Will clear the screen on a \\n character encounter.
 * 
 * @param text The text to print.
 * @param charDelay The delay before each character. (150 default)
 */
void print_lcd(const __FlashStringHelper* text, uint8_t charDelay = 150);

/**
 * @brief Prints a single line of scrolling text on the lcd.
 *
 * @remark This method does not clear the screen before executing.
 *
 * @param text The text to print.
 * @param cursorY Where the cursor should start printing on the Y-Axis (row).
 * @param charDelay Delay between the scroll.
 */
void print_scrolling(const __FlashStringHelper* text, uint8_t cursorY, uint8_t charDelay);

/**
 * @brief Clears a row on the LCD.
 *
 * @param row The row to clear.
 */
void lcd_clear_row(uint8_t row);

/**
 * @brief Set the selected page for selecting a song.
 * 
 * @brief 
 * Each page selection represents a jump of 5. <br />
 * EX: Page 1 (Songs 1-5) <br />
 *     Page 2 (Songs 6-10) <br />
 * <br />
 * There can only be a absolute maximum of 51 pages because 255/5 is 51 and there are 255 maximum allowed songs.
 *
 * @param page The new page to set.
 */
void set_selected_page(uint8_t page);

/**
 * @return The value of the selectedPage variable in the main method.
 */
uint8_t get_selected_page();

/**
 * @brief Set a new song to be selected.
 *
 * @param song The numerical index of the song to set.
 */
void set_selected_song(uint8_t song);

/**
 * @brief Get the current index of the selected song.
 *
 * @return The current selected song in the main class.
 */
uint8_t get_selected_song();

/**
 * @brief Get the current frequency from the potentiometer.
 * Whatever tune button was pressed previously is not accounted for.
 * 
 * @remark This method does not deal with any tune buttons, rather it simply returns the current value as measured by the arduino.
 * @remark This method's speed increases drastically when PRGM_MODE==2 or fast analog read is enabled.
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

/**
 * @brief Saves a song class to the SD card by using the SD.h library and writing all of
 * the tones from the song to the SD card. Allows the saving with a specified file name. File name cannot
 * be longer then 8 characters and should include a .TXT extension.
 * 
 * @since v1.3.0-5: Now directly references global song object, no longer takes an object in.
 *
 * @param fileName The name to save the song as. Must be between 1 and 8 characters, A-Z, 0-9 and underscores only.
 * @param song The song instance to save.
 */
void sd_save_song(const char * const fileName);

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
 * @since v1.3.0-R5: Function no longer takes in a parameter for a reference to a song "&song". Instead it directly references the global song variable.
 *
 * @param fileName The path in the SD card.
 *
 * @return If the song was able to be successfully copied.
 */
bool sd_songcpy(const char * const fileName);

/**
 * @brief Delete a file from the microSD.
 *
 * @param fileName The file to delete.
 */
void sd_rem(const char* const fileName);

/**
 * @brief Generates a README file in the SD card.
 * @remark Will not generate a README if a README.TXT file exists on the SD card root.
 */
void sd_make_readme();


/**
 * @brief Copies the string data from a index on PROGRAM_NOTES PROGMEM and then
 * returns the string data for that pointer.
 * 
 * @brief After the pointer is retrieved from PROGMEM the value is stored in a private static buffer array which can be used
 * by other methods.
 * <br />
 * Ex: pgm_pcpyr(0, 0) -> "B0"
 * 
 * @since v1.2.0-R3
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