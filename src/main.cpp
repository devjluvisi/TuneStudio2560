/**
 * @file main.cpp
 * @author Jacob LuVisi
 * @brief The main class for TuneStudio2560.
 * @version 1.3.0 Release 5
 * @date 2021-06-27 <-> 2021-9-22 (+)
 *
 * @copyright Copyright (c) 2021
 *
 * <h1>TuneStudio2560</h1>
 * <h3>Open source song playback and creation tool made for the Arduino Mega 2560 and compatible.</h3>
 *
 * ████████ ██    ██ ███    ██ ███████ ███████ ████████ ██    ██ ██████  ██  ██████  ██████  ███████  ██████   ██████
 *    ██    ██    ██ ████   ██ ██      ██         ██    ██    ██ ██   ██ ██ ██    ██      ██ ██      ██       ██  ████
 *    ██    ██    ██ ██ ██  ██ █████   ███████    ██    ██    ██ ██   ██ ██ ██    ██  █████  ███████ ███████  ██ ██ ██
 *    ██    ██    ██ ██  ██ ██ ██           ██    ██    ██    ██ ██   ██ ██ ██    ██ ██           ██ ██    ██ ████  ██
 *    ██     ██████  ██   ████ ███████ ███████    ██     ██████  ██████  ██  ██████  ███████ ███████  ██████   ██████
 *
 *
 * TuneStudio2560 was developed using PlatformIO on Visual Studio Code and it is highly recommended to modify
 * this project using those tools.
 *
 * Useful Developer Documentation & References:
 * ~ PRODUCT BRIEF -> https://devjluvisi.github.io/TuneStudio2560/content/TuneStudio2560%20Product%20Brief.pdf
 * ~ C++ DOXYGEN WEB DOC -> https://devjluvisi.github.io/TuneStudio2560/doxy/html/index.html
 * ~ GITHUB WIKI DEV DOC -> https://github.com/devjluvisi/TuneStudio2560/wiki/For-Developers
 * 
 * To view release changelogs, MakerStudio2560, future todo, and doxygen documentation view
 * ~ https://devjluvisi.github.io/TuneStudio2560/
 * 
 * All documentation to method functionality is in header files.
 * Hover over methods to view information.
 * It is highly recommended to use the online Doxygen documentation to view information about methods and variables. 
 * 
 * LICENSE - MIT
 */

#include <studio-libs/tune_studio.h>
#include <studio-libs/states/states.h>
#include <SPI.h>
#include <SdFat.h>

#if PERF_METRICS == true
#include <debug/debug.h>
#endif

/**
Indicates whether or not an immediate interrupt should be called.
Almost all loops in TuneStudio2560 main class have another condition to check for this interrupt.
If this flag is set to true then the current execution loop halts and attempts to return back
to the main loop() function as fast as possible.

The delay_ms(ms) function in this program also makes use of this variable.
*/
static volatile bool immediateInterrupt = false;
/** @brief The last time a button was pressed that the main class registered through is_pressed. */
static volatile unsigned long lastButtonPress = 0;
/** @brief The current index of the selected. */
static volatile uint8_t selectedSong = 1;
/** @brief The current selected page of the program. @remark Each page contains 5 different songs on the SD card. */
static volatile uint8_t selectedPage = 1;

/**
 * @brief Represents a global instance of the Liquid Crystal display used for TuneStudio2560.
 * @brief
 * The Liquid Crystal display uses the I2C protocol and is responsible for direct user feedback on using the program.<br />
 * The display should be the recommended <b>20 columns and 4 rows</b>.<br />
 * <br />
 * Editing TuneStudio2560 to accomidate smaller displays is possible by adjusting the LCD_COLS and LCD_ROWS but some changes to setCursor(x, x) methods would
 * need to be done.
 */
LiquidCrystal_I2C lcd(0x27, LCD_COLS, LCD_ROWS);

/**
 * @brief Represents the 4-digit-wide 7-segment display used in TuneStudio2560.
 * @brief
 * The segment display is reponsible for showing the current note in Creator Mode. <br />
 * The display is wired using the SevSegShift library by being connected to <b>2x 74HC595N Shift Registers</b>.
 */
SevSegShift segDisplay(SHIFT_PIN_DS, SHIFT_PIN_SHCP, SHIFT_PIN_STCP);

/**
 * @brief Represents the global song object to be used in the program when managing songs.
 * 
 * This song object is used between program states and can be accessed at any time.
 * When a new song wants to be used by the application, the song is cleared using .clear() and notes are re-added.
 * 
 * @since v1.2.0-R3
 * Songs are no longer pointers and are no longer dynamically allocated.
 * Any song the program is going to use is now accessible by the "prgmSong" global variable.
 */
Song<MAX_SONG_LENGTH> prgmSong(SPEAKER_1, DEFAULT_NOTE_LENGTH, DEFAULT_NOTE_DELAY);

/**
 * @brief A main-class-scoped global variable that represents the SD card on the Arduino.
 * @since v1.2.2-R4
 */
static SdFat SD;

//////////////////////////////
//// INTERRUPTS & DELAYS ////
////////////////////////////

bool is_interrupt() {

  #if PERF_METRICS == true
  if (millis() % 1250 == 0) {
    Serial.println();
    Serial.print(get_active_time());
    Serial.println(" FREE MEMORY: " + String(freeMemory()) + +"B/8192B");
    Serial.print(get_active_time());
    Serial.println(" STACK SIZE: " + String((RAMEND - SP)) + "B");
    Serial.print(get_active_time());
    Serial.println(" HEAP FRAG: " + String(getFragmentation()) + "%");
    Serial.println();
  }
  #endif

  return immediateInterrupt;
}


void delay_ms(const unsigned long milliseconds) {
  // Set a constant "waitTime" so we can track the time the delay function was first called.
  const unsigned long waitTime = milliseconds + millis();
  while (waitTime > millis() && !is_interrupt()) { // Continue looping forever.
    asm("nop");
  }
}

////////////////////////////////
//// SETUP & LOOP FUNCTIONS ////
////////////////////////////////

/**
 * @brief A global main-class-scoped variable which is a pointer to a ProgramState instance.
 * 
 * @paragraph A Brief Description
 * The prgmState variable represents the current ProgramState the program is in on a global scale.
 * <br />
 * The prgmState is a pointer to a larger ProgramState object which was allocated on the heap.<br />
 * Whenever we need to interact with the current user state we access the prgmState variable which is only scoped to the main class.<br />
 * To change the prgmState from another class you either need to use an interrupt from a supporting ProgramState or use the update_state method.<br />
 * The prgmState variable <b>SHOULD NEVER</b> be allocated with new() outside of update_state and the setup() method. <br />
 * To learn more about program states view the state.h file.
 * 
 * @see state.h
 * @see update_state(StateID state)
 */
static ProgramState * prgmState = new MainMenu();

/**
 * @brief Sets up TuneStudio2560 to be used on an infinite loop by initalizing hardware and checking for errors.
 * 
 * @par In Order:
 * - [DEBUG ONLY] Initalize the Serial Monitor
 * - Setup pinMode
 * - Attach interrupts to select/cancel buttons.
 * - Initalize the Liquid Crystal Display.
 * - Add custom characters to the LCD.
 * - Initalize and setup the 4-digit 7-Segment display.
 * - Setup the SD card and check for errors.
 * - Make the README.TXT file.
 * - [PRGM_MODE==2] Enable Fast Analog Read
 * - Set the prgmState variable to the Main Menu.
 */
void setup() {
  #if DEBUG == true
  // Create serial monitor.
  Serial.begin(9600);
  while (!Serial) {
    ;
  }
 
  // Print welcome message to Serial monitor.
  Serial.println(F("--------------------------------------"));
  Serial.println(F("> TuneStudio2560 has been initialized."));
  Serial.println(F("> Have Fun!"));
  Serial.print(F("> Version: "));
  Serial.println((__FlashStringHelper*)VERSION_ID);
  Serial.println(F("> GitHub: https://github.com/devjluvisi/TuneStudio2560"));
  Serial.println(F("--------------------------------------"));

  Serial.println(F("[!!!] WARNING: DEBUG mode is enabled. When DEBUG is enabled TuneStudio2560 may not run at full speed due to performance reduction outputting to Serial Monitor. Using performance metrics will also largely reduce speed."));
  delay_ms(1000);
  #endif
  #if DEBUG == false && PERF_METRICS == true
  Serial.begin(9600);
  while (!Serial) {
    ;
  }
  while (true) {
    Serial.println(F("ERROR: YOU MUST ENABLE DEBUG IN ORDER TO USE PERFORMANCE METRICS."));
    delay_ms(500);
  }
  #endif
  #if DEBUG == true
  Serial.print(get_active_time());
  Serial.print(F(" PROGRAM MODE: "));
  if(PRGM_MODE==0) {
    Serial.println(F("Performance/Low Size (0)"));
  }else if(PRGM_MODE == 1) {
    Serial.println(F("Standard (1)"));
  }else if(PRGM_MODE == 2) {
  Serial.println(F("Feature (2)"));
  }else{
  Serial.println(F("Unknown"));
  Serial.println(F("Set Performance Mode to Standard (1)."));
  }
  
  #endif

  // Setup all of the pins.
  pinModeFast(RGB_BLUE, OUTPUT);
  pinModeFast(RGB_GREEN, OUTPUT);
  pinModeFast(RGB_RED, OUTPUT);
  pinModeFast(BTN_TONE_1, INPUT);
  pinModeFast(BTN_TONE_2, INPUT);
  pinModeFast(BTN_TONE_3, INPUT);
  pinModeFast(BTN_TONE_4, INPUT);
  pinModeFast(BTN_TONE_5, INPUT);
  pinModeFast(BTN_ADD_SELECT, INPUT);
  pinModeFast(BTN_DEL_CANCEL, INPUT);
  pinModeFast(BTN_OPTION, INPUT);
  pinModeFast(TONE_FREQ, INPUT);
  pinModeFast(SPEAKER_1, OUTPUT);
  pinModeFast(SD_CS_PIN, OUTPUT);

  // Make all button pins INPUT_PULLUP
  // For Arduino Mega 2560 ONLY
  {
    PORTA = PORTA |
      (
        (1 << PORTA2) |
        (1 << PORTA3) |
        (1 << PORTA4) |
        (1 << PORTA5)
      );
    PORTE = PORTE |
      (
        (1 << PORTE4) |
        (1 << PORTE5)
      );
    PORTG = PORTG |
      (
        (1 << PORTG5)
      );
  }

  #if DEBUG == true
  Serial.print(get_active_time());
  Serial.println(F(" pins have been initalized."));
  #endif

  // Interrupt to handle when the select button is pressed.
  attachInterrupt(digitalPinToInterrupt(BTN_ADD_SELECT), isr_btn_handle, FALLING);
  attachInterrupt(digitalPinToInterrupt(BTN_DEL_CANCEL), isr_btn_handle, FALLING);

  // Initalize the LCD.
  lcd.init();
  lcd.backlight();

  // Add custom characters to the LCD.
  byte buffer[8];
  for (uint8_t i = 0; i < CUSTOM_CHAR_AMOUNT; i++) {
    memcpy_P(buffer, & (CUSTOM_LCD_CHARS[i]), 8);
    lcd.createChar(i, buffer);
  }

  #if DEBUG == true
  Serial.print(get_active_time());
  Serial.println(F(" lcd has been initalized."));
  #endif
  // Setup 4-wide 7 segment display.
  // More information: https://github.com/bridystone/SevSegShift

  const byte numDigits = 4;
  constexpr byte digitPins[] = {
    8 + 2,
    8 + 5,
    8 + 6,
    2
  }; // of ShiftRegister(s) | 8+x (2nd Register)
  constexpr byte segmentPins[] = {
    8 + 3,
    8 + 7,
    4,
    6,
    7,
    8 + 4,
    3,
    5
  }; // of Shiftregister(s) | 8+x (2nd Register)

  const bool resistorsOnSegments = false; // 'false' means resistors are on digit pins
  const byte hardwareConfig = COMMON_CATHODE; // See README.md for options
  const bool updateWithDelays = false; // Default 'false' is Recommended
  const bool leadingZeros = false; // Use 'true' if you'd like to keep the leading zeros
  bool disableDecPoint = false; // Use 'true' if your decimal point doesn't exist or isn't connected. Then, you only need to specify 7 segmentPins[]

  segDisplay.begin(hardwareConfig, numDigits, (byte * ) digitPins, (byte * ) segmentPins, resistorsOnSegments,
    updateWithDelays, leadingZeros, disableDecPoint);
  segDisplay.blank();
  segDisplay.refreshDisplay();
  #if DEBUG == true
  Serial.print(get_active_time());
  Serial.println(F(" segment display has been initalized."));
  #endif


  // Setup SD Card
  if (!SD.begin(SD_CS_PIN)) {
    #if DEBUG == true
    Serial.println(F("[CRITICAL] SD module cannot be initalized due to one or more problems."));
    Serial.println(F("* Is the card properly inserted?"));
    Serial.println(F("* Is the card correctly wired to the board?"));
    Serial.println(F("* Did you change the CS pin to match your board?"));
    Serial.println(F("* Have you verified the card is working on a PC?"));
    Serial.println(F("* Is the format for the microSD either FAT16 or FAT32?"));
    Serial.println(F("View https://github.com/devjluvisi/TuneStudio2560/wiki/For-Developers for more information."));
    #endif
    lcd.clear();
    lcd.setCursor(0, 1);
    lcd.print(F("SD Card Error"));
    lcd.setCursor(0, 2);
    lcd.print(F("Check Serial."));
    #if PRGM_MODE == 0
    digitalWriteFast(RGB_RED, HIGH);
    #else
    analogWrite(RGB_RED, RGB_BRIGHTNESS);
    #endif
    while (true) {
      ;
    }
  }
  #if DEBUG == true
  Serial.print(get_active_time());
  Serial.println(F(" SD card has been initalized."));
  #endif

  sd_make_readme();
  #if DEBUG == true
  Serial.print(get_active_time());
  Serial.println(F(" README file has been generated."));
  #endif

  #if FAST_ADC == 1
  // set prescale to 16
  sbi(ADCSRA, ADPS2);
  cbi(ADCSRA, ADPS1);
  cbi(ADCSRA, ADPS0);
  #endif

  // Blink LED according to Program Mode.
  #if PRGM_MODE==0
    digitalWriteFast(RGB_GREEN, HIGH);
    delay(700);
    digitalWriteFast(RGB_GREEN, LOW);
  #elif PRGM_MODE==2
  analogWrite(RGB_GREEN, RGB_BRIGHTNESS);
  delay(700);
  analogWrite(RGB_GREEN, LOW);
  delay(700);
  analogWrite(RGB_GREEN, RGB_BRIGHTNESS);
  delay(700);
  analogWrite(RGB_GREEN, LOW);
  delay(700);
  analogWrite(RGB_GREEN, RGB_BRIGHTNESS);
  delay(700);
  analogWrite(RGB_GREEN, LOW);
  #else
  analogWrite(RGB_GREEN, RGB_BRIGHTNESS);
  delay(700);
  analogWrite(RGB_GREEN, LOW);
  delay(700);
  analogWrite(RGB_GREEN, RGB_BRIGHTNESS);
  delay(700);
  analogWrite(RGB_GREEN, LOW);
  #endif
}

/**
 * @brief The primary loop() method of the Arduino is responsible for managing the programState and resetting interrupts.
 * @paragraph What it does
 * The loop() method will first call the execute() function for the prgmState variable.
 * The execute() function will handle the initalization and looping for the Program State.
 * <br />
 * After, the immediateInterrupt variable is set to "false" just in case the loop was reset due to an interrupt.
 * <br /><br />
 * <i>For PERF_METRICS & DEBUG Only:</i><br />
 * Prints out performance statistics to the Serial monitor.
 * The performance statistics which are displayed are:
 * - Time the loop took to complete (in microseconds)
 * - The RAM usage of the program.
 * - The current (%) of RAM the program is utilizing.
 * - The number of clock cycles which have passed since the previous iteration.
 * - The "IPS" or "Iterations Per Second" of the loop which describes how many times the loop can run PER SECOND if the number of clock cycles is counted accurately.
 * 
 */
void loop() {
  #if PERF_METRICS // For testing the iteration speed of the programState loop() function. Results will be off for functions who use blocking delays.
  unsigned long startingMicros = micros();
  #define RAM_SIZE_BYTES 8192
  #endif
  prgmState -> execute();
  immediateInterrupt = false;
  #if PERF_METRICS
  const unsigned long finishTime = micros() - startingMicros;
  const unsigned int memoryUtilBytes = RAM_SIZE_BYTES - freeMemory();
  const float ramUtilPercent = ((float) memoryUtilBytes / (float) RAM_SIZE_BYTES) * (float) 100;
  const unsigned long clockCycles = microsecondsToClockCycles(finishTime);
  const uint32_t iterationsPerSecond = F_CPU / microsecondsToClockCycles(finishTime);

  Serial.print(F("Loop Complete in: "));
  Serial.print(finishTime); // Amount of microseconds it takes to complete an iteration of the program state.
  Serial.print(F("μs | RAM USAGE: "));
  Serial.print(memoryUtilBytes); // How much SRAM the current application is using.
  Serial.print(F("B ("));
  Serial.print(ramUtilPercent); // The percent of ram being utilized.
  Serial.print(F("%)"));
  Serial.print(F(" | "));
  Serial.print(F("Cycles: "));
  Serial.print(clockCycles); // How many clock cycles the loop took.
  Serial.print(F(" > IPS: "));
  // How many times the loop executes per second judging on the last known amount of clock cycles the loop took to execute.
  // IPS means Iterations Per Second (how many times loop() can run per second)
  Serial.println(iterationsPerSecond);

  #endif
}

////////////////////////
//// AUDIO METHODS ////
//////////////////////

void set_selected_page(uint8_t page) {
  selectedPage = page;
  #if DEBUG == true
  Serial.print(get_active_time());
  Serial.print(F(" Changed the page to "));
  Serial.print(page);
  Serial.println(F("."));
  #endif
}

uint8_t get_selected_page() {
  return selectedPage;
}

void set_selected_song(uint8_t song) {
  selectedSong = song;
  #if DEBUG == true
  Serial.print(get_active_time());
  Serial.print(F(" Changed the current song to "));
  Serial.print(song);
  Serial.println(F("."));
  #endif

}

uint8_t get_selected_song() {
  return selectedSong;
}
const char* pgm_pcpyr(uint8_t btnIndex, uint8_t noteIndex) {
  // Create a static buffer to hold the value of any pitch the program wants to use.
  static char pitchBuff[4];
  // Copy the PROGMEM pointer data to pitchBuff.
  strcpy_P(pitchBuff, (PGM_P)pgm_read_word(&(PROGRAM_NOTES[btnIndex].notes[noteIndex].pitch)));
  return pitchBuff;
}


note_t get_note_from_freq(const uint16_t frequency) {
  if (frequency == PAUSE_NOTE.frequency) {
    return PAUSE_NOTE;
  }

  // 5 tune buttons, 17 pitches each
  for (uint8_t i = 0; i < TONE_BUTTON_AMOUNT; i++) {
    for (uint8_t j = 0; j < TONES_PER_BUTTON; j++) {
      if (pgm_read_word( & PROGRAM_NOTES[i].notes[j].frequency) == frequency) {

        // Get a pointer to the current note we want.
        const char * pitch = pgm_pcpyr(i, j);

        return note_t {
          pitch,
          frequency
        }; // Dont need to find the frequency because the parameter already passed it.
      }
    }
  }
  #if DEBUG == true
  Serial.print(get_active_time());
  Serial.print(F(" WARNING: Encountered unknown frequency "));
  Serial.print(frequency);
  Serial.println(F("."));
  #endif
  return EMPTY_NOTE;
}

note_t get_note_from_pitch(const char *
  const pitch) {
  // If the note is a pause then return it instantly.
  if (strcmp(PAUSE_NOTE.pitch, pitch) == 0) {
    return PAUSE_NOTE;
  }

  // Go through every tune button and find a match.
  for (uint8_t i = 0; i < TONE_BUTTON_AMOUNT; i++) {
    for (uint8_t j = 0; j < TONES_PER_BUTTON; j++) {
      if (strcmp(pgm_pcpyr(i, j), pitch) == 0) {

        uint16_t freq = (uint16_t) pgm_read_word( & PROGRAM_NOTES[i].notes[j].frequency);

        return note_t {
          pitch,
          freq
        }; // Dont need to find the pitch because the parameter already passed it.
      }
    }
  }
  #if DEBUG == true
  Serial.print(get_active_time());
  Serial.print(F(" WARNING: Encountered unknown pitch "));
  Serial.print(pitch);
  Serial.println(F("."));
  #endif
  return EMPTY_NOTE;
}

note_t get_current_tone(uint8_t toneButton) {
  // Split the potentiometer value into 17 different sections because each tune button represents 17 different tones.
  // Note that the subTone value is not evenly split and the final subTone (17) has slightly less potential values.
  const uint8_t subTone = (get_current_freq() + 1) / 61;
  toneButton = BTN_TO_INDEX(toneButton);
  return toneButton != UINT8_MAX ? note_t {
    (pgm_pcpyr(toneButton, subTone)), ((uint16_t) pgm_read_word( & PROGRAM_NOTES[toneButton].notes[subTone].frequency))
  } : EMPTY_NOTE;
}

uint16_t get_current_freq() {
  return analogRead(TONE_FREQ);
}

////////////////////////
//// LCD FUNCTIONS ////
//////////////////////

void print_lcd(const __FlashStringHelper * text, uint8_t charDelay) {
  lcd.clear();

  uint8_t cursorX = 0; // Track cursor on X position.
  uint8_t cursorY = 0; // Track cursor on Y position.

  // Track a pointer to each character in the flash string.
  const char * p = (const char PROGMEM * ) text;

  // As long as a terminating character is not encountered, continue.
  while (pgm_read_byte_near(p) != '\0') {
    if (is_interrupt()) return;

    // Read character from PROGMEM and store it in SRAM.
    const char letter = pgm_read_byte_near(p++);

    lcd.setCursor(cursorX, cursorY);

    // Check if the beginning character is a space.
    if (!(cursorX == 0 && letter == ' ') && letter != '\n') {
      lcd.write(letter); // Print letter to LCD.
      delay_ms(charDelay);
      cursorX++; // Go up by one cursor.
    }

    // Reached end of the row.
    if (cursorX != 0 && cursorX % LCD_COLS == 0) {
      cursorY++;
      cursorX = 0;
    }

    // Reached end of the LCD.
    if ((cursorY != 0 && cursorY % LCD_ROWS == 0) || letter == '\n') {
      delay_ms(600);
      cursorX = 0;
      cursorY = 0;
      lcd.clear();
    }
  }
}

void print_scrolling(const __FlashStringHelper * text, uint8_t cursorY, uint8_t charDelay) {

  // Track a pointer to each character in the flash string.
  const char * p = (const char PROGMEM * ) text;
  // If the inital char delay should be waited.
  bool flag = false;

  // Increment the address of the pointer and get the next character for as long as the pointer address is not a string terminating character.
  while (pgm_read_byte_near(p + (LCD_COLS - 1)) != '\0') {
    if (is_interrupt()) return;
    lcd.setCursor(0, cursorY); {
      // Create a substring by going through 20 characters on the progmem string.
      for (uint8_t i = 0; i < LCD_COLS; i++) {
        lcd.write(pgm_read_byte_near(p++));
      }
      // Move the pointer back 19 spaces to prepare for next character in substring.
      p -= (LCD_COLS - 1);
    }
    if (!flag) {
      delay_ms(600);
      flag = true;
    }
    delay_ms(charDelay);
  }
}

void lcd_clear_row(uint8_t row) {
  lcd.setCursor(0, row);
  for (uint8_t i = 0; i < LCD_COLS; i++) {
    lcd.print(F(" "));
  }
  lcd.setCursor(0, row);
}

////////////////////////////
//// BUTTONS & ACTIONS ////
//////////////////////////

void update_state(StateID state) {
  if (state == prgmState -> get_state()) {
    return;
  }
  
  // Free the memory that the previous program state.
  delete prgmState;
  prgmSong.clear();
  // Reset the songs attributes in case they were changed.
  prgmSong.set_attributes(DEFAULT_NOTE_LENGTH, DEFAULT_NOTE_DELAY);

  #if DEBUG == true
  Serial.print(get_active_time());
  Serial.print(F(" changed current program state to "));
  Serial.println(state);
  #endif

  switch (state) {
  case MAIN_MENU:
    prgmState = new MainMenu();
    return;
  case CM_MENU:
    prgmState = new CreatorModeMenu();
    return;
  case LM_MENU:
    prgmState = new ListeningModeMenu();
    return;
  case LM_PLAYING_SONG:
    prgmState = new ListeningModePlayingSong();
    return;
  case CM_CREATE_NEW:
    prgmState = new CreatorModeCreateNew();
    return;
  default:
    #if DEBUG == true
    Serial.print(get_active_time());
    Serial.println(F(" Unknown State Requested."));
    #endif
    return;
  }
}

void isr_btn_handle() {
  // First, we must check if the current state wants to handle an interrupt in the first place.
  if(millis() - lastButtonPress < DEBOUNCE_RATE || digitalReadFast(BTN_OPTION) == LOW) {
    return; // Terminate ISR
  }
  #define SELECT 0 // Select button signified as false (LOW)
  #define CANCEL 1 // Cancel button signified as true (HIGH)

  bool buttonType = digitalReadFast(BTN_ADD_SELECT); // Check if the select button was the button pressed.

  immediateInterrupt = true;
  switch(prgmState->get_state()) {
    case MAIN_MENU:
      if(buttonType==SELECT) update_state(CM_MENU);
      if(buttonType==CANCEL) update_state(LM_MENU);
      break;
    case LM_MENU:
      if(buttonType==SELECT && prgmState->has_initalized()) { update_state(LM_PLAYING_SONG); immediateInterrupt = false;}
      if(buttonType==CANCEL) { update_state(MAIN_MENU); set_selected_page(1); set_selected_song(1); }
      break;
    case CM_MENU:
      if(buttonType==SELECT) update_state(CM_CREATE_NEW);
      if(buttonType==CANCEL) update_state(MAIN_MENU);
      break;
    default:
      immediateInterrupt = false;
      break;
  }
  lastButtonPress = millis();
}

bool is_pressed(const uint8_t buttonPin) {
  if (!(millis() - lastButtonPress < DEBOUNCE_RATE) && digitalRead(buttonPin) == LOW) {
    lastButtonPress = millis();
    return true;
  }
  return false;
}

////////////////////////////
//// SD CARD FUNCTIONS ////
//////////////////////////

void sd_save_song(const char * const fileName) {
  // Delete the previous song if the name already exists.
  sd_rem(fileName);
  // Create a song object to be saved
  File songFile = SD.open(fileName, FILE_WRITE);

  songFile.print(F(
    "# Welcome to a song file!\n"
    "# To view more information, check out https://github.com/devjluvisi/TuneStudio2560/wiki/For-Users\n"
    "\n# The delay between each different tone (ms). (Must be 9999 or less and greater than 0)\n"
    "TONE_DELAY="
  ));
  songFile.println(DEFAULT_NOTE_DELAY);
  songFile.print(F("\n# The length that each tone should play for (ms). (Must be 255 or less and greater than 0)\nTONE_LENGTH="));
  songFile.println(DEFAULT_NOTE_LENGTH);
  songFile.println(F("\nData:"));

  // Convert each frequency in the song to a pitch and save it on the SD.
  for (song_size_t i = 0; i < prgmSong.get_size(); i++) {
    songFile.print(F("  - "));
    songFile.println(get_note_from_freq(prgmSong.get_note(i)).pitch);
  }
  songFile.println(F("\n# END"));
  songFile.close();
  #if DEBUG == true
  Serial.print(get_active_time());
  Serial.println(F(" Finished writing with SD Card."));
  #endif
  return;
}

void sd_rem(const char * const fileName) {
  if (SD.exists(fileName)) {
    #if DEBUG == true
    Serial.print(get_active_time());
    Serial.print(F(" "));
    Serial.print(fileName);
    Serial.println(F(" HAS BEEN DELETED FROM SD CARD."));
    #endif
    // Delete the old file.
    SD.remove(fileName);
  }
  return;
}

const char * sd_get_file(uint8_t index) {
  File baseDir = SD.open(ROOT_DIR);
  baseDir.rewindDirectory();
  // The current amount of song files we have opened.
  uint8_t count = 0;
  static char name[14];
  while (true) {
    File entry = baseDir.openNextFile();

    // Copies the name of the SD file onto the static name buffer.
    entry.getName(name, sizeof(name));

    const bool isValid = entry && !entry.isDirectory() && strcasestr(name, FILE_TXT_EXTENSION) && strcmp(name, README_FILE) != 0;
    entry.close();

    if(!isValid) {
      // Check to see if the file we are trying to open does not even exist.
      if(name[0] == '\0') {
        break;
      }
      // File is not a valid song but it does exist.
      continue;
    }

    if(count == index) {
      break; // We have gotten to our number. Now we break out and return.
    }
    count++;
  }
  // Close the root directory we were editing.
  baseDir.close();
  return name;
}

bool sd_songcpy(const char * const fileName) {

  // If the file does not exist.
  if (!SD.exists(fileName)) {
    return false;
  }

  // Open a new file to read from.
  File entry = SD.open(fileName);
  // Track if the current '=' sign being read is for the tone delay or for the tone length.
  bool isToneDelay = true;
  // Remove all data from song
  prgmSong.clear();

  // Store these variables to update later if the user has input custom delays.
  uint8_t noteDelay = DEFAULT_NOTE_DELAY;
  uint16_t noteLength = DEFAULT_NOTE_LENGTH;

  unsigned int songSize = 0;

  // As long as their are avaliable characters in the file.
  while (entry.available()) {

    // Store the current character.
    char letter = entry.read();

    // Ignore the lines with hashtags.
    if (letter == '#') {
      while (letter != '\n' && entry.available()) {
        letter = entry.read();
      }
    }

    // Attempt to read the tone delay and tone length values.
    if (letter == '=') {
      char buffer[5];
      uint8_t index = 0;
      while (entry.peek() != '\n') {
        letter = entry.read();
        // Ignore the spaces.|| letter == '\r' || letter == '\b' || letter = '\t'
        if (letter == ' ' || letter == '\r' || letter == '\b' || letter == '\t' || letter == '=') {
          continue;
        }
        // Add the character to the buffer.
        buffer[index] = letter;
        index++;
      }
      buffer[index] = '\0';
      const uint16_t textToNum = atoi(buffer);

      if (isToneDelay) {
        isToneDelay = false;
        noteDelay = textToNum;
        #if DEBUG == true
        Serial.print(get_active_time());
        Serial.print(F(" Read a TONE DELAY of: "));
        Serial.println(textToNum);
        #endif
      } else {

        noteLength = textToNum;
        #if DEBUG == true
        Serial.print(get_active_time());
        Serial.print(F(" Read a TONE LENGTH of: "));
        Serial.println(textToNum);
        #endif
      }
    }

    // Find the lines with a '-' which indicates that they are notes.
    if (letter == '-') {
      char buffer[4];
      uint8_t index = 0;
      // Note: we are on a line with a '-' which means it has a note on it.
      // Go through each character of the line until we find a line break.
      while (entry.peek() != '\n') {
        if (index == 4) {
          entry.close();
          #if DEBUG == true
          Serial.print(get_active_time());
          Serial.print(F("Song failed due to note size. Count: "));
          Serial.print(index);
          #endif
          return false;
        }
        letter = entry.read();
        // Ignore the spaces.
        if (letter == ' ' || letter == '\r' || letter == '\b' || letter == '\t' || letter == '=') {
          continue;
        }
        // Add the character to the buffer.
        buffer[index] = letter;
        index++;
      }
buffer[index] = '\0';
      // Add the note.
      note_t foundNote = get_note_from_pitch(buffer);
      if (foundNote.frequency == EMPTY_NOTE.frequency) {
        entry.close();
        #if DEBUG == true
        Serial.print(get_active_time());
        Serial.println(F("Song copy finished, found END note."));
        #endif
        return false;
      }
      prgmSong.add_note(foundNote.frequency);
      songSize++;
    }
  }
  if (songSize < MIN_SONG_LENGTH || songSize > MAX_SONG_LENGTH) {
    #if DEBUG == true
    Serial.print(get_active_time());
    Serial.println(F("Song failed due to size."));
    #endif
    entry.close();
    return false;
  }

  // Update the note length and note delay with what the file has read.
  prgmSong.set_attributes(noteLength, noteDelay);

  // Close the file
  entry.close();
  
  #if DEBUG == true
  Serial.print(get_active_time());
  Serial.print(F(" Copied a song ("));
  Serial.print(fileName);
  Serial.println(F(") from SD card to memory."));
  #endif
  return true;
}

// FOR DEBUG USE ONLY
#if DEBUG == true
const char * get_active_time() {
  unsigned long currMillis = millis();
  // 15000
  unsigned long seconds = currMillis / 1000;
  unsigned long minutes = seconds / 60;
  unsigned long hours = minutes / 60;
  seconds %= 60;
  minutes %= 60;
  hours %= 24;
  static char buffer[16];
  strcpy(buffer, "[");
  if (hours < 10) strcat(buffer, "0");
  itoa(hours, buffer + strlen(buffer), 10);
  strcat(buffer, ":");
  if (minutes < 10) strcat(buffer, "0");
  itoa(minutes, buffer + strlen(buffer), 10);
  strcat(buffer, ":");
  if (seconds < 10) strcat(buffer, "0");
  itoa(seconds, buffer + strlen(buffer), 10);
  strcat(buffer, "]");
  return buffer;
}
#endif

void sd_make_readme() {

  if (SD.exists(README_FILE)) {
    return;
  }

  #if PRGM_MODE != 0
  static const char README_TEXT[] PROGMEM =
    "---------> || TuneStudio2560 || <---------\n"
  "Welcome to the TuneStudio2560 SD Card!\n"
  "The SD card allows users to seemlessly edit, create, and remove songs without having to interact with TuneStudio at all!\n"
  "If you have already created songs then you will find them here.\n"
  "You can edit the songs and create your own but you must follow the standard file format. Two spaces and one hyphen followed by a space and then the tone.\n"
  "Each SD card also has \"#\" which indicate comments. These comments cannot be read by the device so feel free to put your own \"#\" followed by text to put notes!\n"
  "\n"
  "Remember: \n"
  " - Song must follow correct format.\n"
  " - Song must have between 8-255 tones.\n"
  " - Ensure that tones added are valid and exist.\n"
  " - Follow number paremeters for customizing TONE_DELAY and TONE_LENGTH.\n"
  "\n"
  "The program will try to alert when there is a problem with the song but not all errors are caught.\n"
  "When an error is encountered you will be redirected back to the listening mode menu.\n"
  "\n"
  "You can view more information about SD cards here: https://github.com/devjluvisi/TuneStudio2560/wiki/For-Users\n"
  "To view the main Repository go to: https://github.com/devjluvisi/TuneStudio2560\n"
  "\n"
  "I hope you enjoy!";
  #else
  static const char README_TEXT[] PROGMEM =
    "---------> || TuneStudio2560 || <---------\n"
  "Welcome to the TuneStudio2560 SD Card!\n"
  "Due to low performance requirements, the full README could not be generated.\n"
  "You can view more information about SD cards here: https://github.com/devjluvisi/TuneStudio2560/wiki/For-Users\n"
  "To view the main Repository go to: https://github.com/devjluvisi/TuneStudio2560\n"
  "\n"
  "I hope you enjoy!";
  #endif

  File readMe = SD.open(README_FILE, FILE_WRITE);
  readMe.print((__FlashStringHelper*)README_TEXT);
  readMe.close();

  #if DEBUG == true
  Serial.print(get_active_time());
  Serial.println(F(" Generated README file."));
  #endif

}