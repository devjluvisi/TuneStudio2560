/**
 * @file main.cpp
 * @author Jacob LuVisi
 * @brief The main class for TuneStudio2560.
 * @version 1.0.0-Revision 1
 * @date 2021-06-27 <-> 2021-08-12 (+)
 *
 * @copyright Copyright (c) 2021
 *
 * TuneStudio2560
 * Open source song playback and creation tool made for the Arduino Mega 2560 and compatible.
 *
 * ████████ ██    ██ ███    ██ ███████ ███████ ████████ ██    ██ ██████  ██  ██████  ██████  ███████  ██████   ██████
 *    ██    ██    ██ ████   ██ ██      ██         ██    ██    ██ ██   ██ ██ ██    ██      ██ ██      ██       ██  ████
 *    ██    ██    ██ ██ ██  ██ █████   ███████    ██    ██    ██ ██   ██ ██ ██    ██  █████  ███████ ███████  ██ ██ ██
 *    ██    ██    ██ ██  ██ ██ ██           ██    ██    ██    ██ ██   ██ ██ ██    ██ ██           ██ ██    ██ ████  ██
 *    ██     ██████  ██   ████ ███████ ███████    ██     ██████  ██████  ██  ██████  ███████ ███████  ██████   ██████
 *
 * TuneStudio2560 was developed using PlatformIO on Visual Studio Code and it is highly reccomended to modify
 * this project using that framework.
 *
 * To view documentation on how the software for TuneStudio2560 operates please check out
 * ~ https://github.com/devjluvisi/TuneStudio2560/wiki/For-Developers
 *
 * To view general information on how to use TuneStudio2560 see
 * ~ https://github.com/devjluvisi/TuneStudio2560/wiki/For-Users
 *
 * To view how to create TuneStudio2560 along with the schematics, parts, and wiring view
 * ~ https://github.com/devjluvisi/TuneStudio2560/wiki/Build-It
 *
 * LICENSE - MIT
 */


/*
TODO LIST:
 - Move char strings in toneButtons PROGMEM array to PROGMEM as they are currently stored in SRAM.
*/

#include <studio-libs/tune_studio.h>
#include <studio-libs/states/states.h>
#if PERF_METRICS == true
#include <debug/debug.h>
#endif
 /**
 Indicates whether or not an immediate interrupt should be called.
 Almost all loops in TuneStudio2560 have another condition to check for this interrupt.
 If this flag is set to true then the current execution loop halts and attempts to return back
 to the main loop() function as fast as possible.

 The delay_ms(ms) function in this program also makes use of this variable.
 */
static volatile bool immediateInterrupt = false;
static volatile unsigned long lastButtonPress = 0; // The last time a button was pressed in millis().
static volatile uint8_t selectedSong = 1; // The current song which is selected in any circumstance. (STARTS AT 1)
static volatile uint8_t selectedPage = 1; // The current page of songs which is selected. Each page is a group of 5 songs. (microSD only) (STARTS AT 1)

// The LCD of TuneStudio2560.
LiquidCrystal_I2C lcd(0x27, LCD_COLS, LCD_ROWS);
// The 4 Digit 7 Segment Display.
SevSegShift segDisplay(SHIFT_PIN_DS, SHIFT_PIN_SHCP, SHIFT_PIN_STCP);


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
  const unsigned long waitTime = milliseconds + millis();
  while (waitTime > millis() && !is_interrupt()) { // Continue looping forever.
    continue;
  }
}

////////////////////////////////
//// SETUP & LOOP FUNCTIONS ////
////////////////////////////////

/**
 * @brief Keeps track of the current state of the program.
 * Read more about program states in the state.h class.
 */
static ProgramState* prgmState;

void setup()
{

#if DEBUG == true
  // Create serial monitor.
  Serial.begin(9600);
  while (!Serial) {
    ;
  }

  // Print welcome message to Serial monitor.
  Serial.println(F("--------------------------------------\n> TuneStudio2560 has initalized\n> Have fun!\n--------------------------------------"));
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

  // Setup all of the pins.
  pinModeFast(RGB_BLUE, OUTPUT);
  pinModeFast(RGB_GREEN, OUTPUT);
  pinModeFast(RGB_RED, OUTPUT);
  pinMode(BTN_TONE_1, INPUT_PULLUP);
  pinMode(BTN_TONE_2, INPUT_PULLUP);
  pinMode(BTN_TONE_3, INPUT_PULLUP);
  pinMode(BTN_TONE_4, INPUT_PULLUP);
  pinMode(BTN_TONE_5, INPUT_PULLUP);
  pinMode(BTN_ADD_SELECT, INPUT_PULLUP);
  pinMode(BTN_DEL_CANCEL, INPUT_PULLUP);
  pinMode(BTN_OPTION, INPUT_PULLUP);
  pinModeFast(TONE_FREQ, INPUT);
  pinModeFast(SPEAKER_1, OUTPUT);
  pinModeFast(SD_CS_PIN, OUTPUT);

#if DEBUG == true
  Serial.print(get_active_time());
  Serial.println(F(" pins have been initalized."));
#endif

  // Interrupt to handle when the select button is pressed.
  attachInterrupt(digitalPinToInterrupt(BTN_ADD_SELECT), select_btn_click, CHANGE);
  attachInterrupt(digitalPinToInterrupt(BTN_DEL_CANCEL), cancel_btn_click, CHANGE);

  // Initalize the LCD.
  lcd.init();
  lcd.backlight();

  // Add custom characters to the LCD.
  byte buffer[8];
  for(uint8_t i = 0; i < CUSTOM_CHAR_AMOUNT; i++) {
    memcpy_P(buffer, &(CUSTOM_LCD_CHARS[i]), 8);
    lcd.createChar(i, buffer);
  }

#if DEBUG == true
  Serial.print(get_active_time());
  Serial.println(F(" lcd has been initalized."));
#endif
  // Setup 4-wide 7 segment display.
  // More information: https://github.com/bridystone/SevSegShift

  const byte numDigits = 4;
  byte digitPins[] = { 8 + 2, 8 + 5, 8 + 6, 2 }; // of ShiftRegister(s) | 8+x (2nd Register)
  byte segmentPins[] = { 8 + 3, 8 + 7, 4, 6, 7, 8 + 4, 3,  5 }; // of Shiftregister(s) | 8+x (2nd Register)
  /* configuration without ShiftRegister - Direct arduino connection
  byte digitPins[] = {2, 3, 4, 5}; // PINs of Arduino
  byte segmentPins[] = {6, 7, 8, 9, 10, 11, 12, 13}; // PINs of Arduino
  */
  const bool resistorsOnSegments = false; // 'false' means resistors are on digit pins
  const byte hardwareConfig = COMMON_CATHODE; // See README.md for options
  const bool updateWithDelays = false; // Default 'false' is Recommended
  const bool leadingZeros = false; // Use 'true' if you'd like to keep the leading zeros
  bool disableDecPoint = false; // Use 'true' if your decimal point doesn't exist or isn't connected. Then, you only need to specify 7 segmentPins[]

  segDisplay.begin(hardwareConfig, numDigits, digitPins, segmentPins, resistorsOnSegments,
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
    analogWrite(RGB_RED, RGB_BRIGHTNESS);
    while (true) { ; }
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

  // Set the user to the main menu.
  prgmState = new MainMenu();

}

void loop()
{
  prgmState->execute();
  immediateInterrupt = false;
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

note_t get_note_from_freq(const uint16_t frequency) {
  if (frequency == PAUSE_NOTE.frequency) {
    return PAUSE_NOTE;
  }
  //TODO: Make searching algorithm more efficient
  // 5 tune buttons, 17 pitches each
  for (uint8_t i = 0; i < TONE_BUTTON_AMOUNT; i++) {
    for (uint8_t j = 0; j < TONES_PER_BUTTON; j++) {
      if (pgm_read_word(&PROGRAM_NOTES[i].notes[j].frequency) == frequency) {
        char* pitch = (char*)pgm_read_word(&PROGRAM_NOTES[i].notes[j].pitch);
        //uint16_t freq = (uint16_t)pgm_read_word(&toneButtons[i].notes[j].frequency);
        return note_t{ pitch, frequency }; // Dont need to find the frequency because the parameter already passed it.
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

note_t get_note_from_pitch(const char* pitch) {
  // If the note is a pause then return it instantly.
  if (strcmp(PAUSE_NOTE.pitch, pitch) == 0) {
    return PAUSE_NOTE;
  }
  //TODO: Make searching algorithm more efficient
  // 5 tune buttons, 17 pitches each
  for (uint8_t i = 0; i < TONE_BUTTON_AMOUNT; i++) {
    for (uint8_t j = 0; j < TONES_PER_BUTTON; j++) {
      if (strcmp((char*)pgm_read_word(&PROGRAM_NOTES[i].notes[j].pitch), pitch) == 0) {
        //char* pitch = (char*)pgm_read_word(&toneButtons[i].notes[j].pitch);
        uint16_t freq = (uint16_t)pgm_read_word(&PROGRAM_NOTES[i].notes[j].frequency);
        return note_t{ pitch, freq }; // Dont need to find the pitch because the parameter already passed it.
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
  const uint16_t subTone = (uint16_t)(get_current_freq() + 1) / (uint8_t)61;
  uint8_t buttonIndex;
  switch(toneButton) {
    case BTN_TONE_1:
      buttonIndex = 0;
      break;
    case BTN_TONE_2:
      buttonIndex = 1;
      break;
    case BTN_TONE_3:
      buttonIndex = 2;
      break;
    case BTN_TONE_4:
      buttonIndex = 3;
      break;
    case BTN_TONE_5:
      buttonIndex = 4;
      break;
    default:
      buttonIndex = 255; 
#if DEBUG == true
      Serial.print(get_active_time());
      Serial.println(F(" Error in get_current_tone(uint8_t toneButton). Cannot find specified tone button."));
#endif
      break;
  }
  return buttonIndex != 255 ? note_t{ ((char*)pgm_read_word(&PROGRAM_NOTES[buttonIndex].notes[subTone].pitch)), ((uint16_t)pgm_read_word(&PROGRAM_NOTES[buttonIndex].notes[subTone].frequency)) } : EMPTY_NOTE;
}

uint16_t get_current_freq() {
  return analogRead(TONE_FREQ);
}

////////////////////////
//// LCD FUNCTIONS ////
//////////////////////


void print_lcd(const __FlashStringHelper* text, uint8_t charDelay) {
  lcd.clear();
  uint8_t cursorX = 0; // Track cursor on X position.
  uint8_t cursorY = 0; // Track cursor on Y position.

  // Track a pointer to each character in the flash string.
  const char *p = (const char PROGMEM *)text;

  // As long as a terminating character is not encountered, continue.
  while(pgm_read_byte_near(p) != '\0') {
    if (is_interrupt()) return;

    // Read character from PROGMEM and store it in SRAM.
    char letter = pgm_read_byte_near(p++);

    lcd.setCursor(cursorX, cursorY);

    // Check if the beginning character is a space.
    if (!(cursorX == 0 && letter == ' ')) {
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
    if (cursorY != 0 && cursorY % LCD_ROWS == 0) {
      delay_ms(600);
      cursorX = 0;
      cursorY = 0;
      lcd.clear();
    }
  }
}

void print_scrolling(const __FlashStringHelper* text, uint8_t cursorY, uint8_t charDelay) {
  const uint16_t initalCharDelay = charDelay * 2;

  // Track a pointer to each character in the flash string.
  const char *p = (const char PROGMEM *)text;
  // If the inital char delay should be waited.
  bool flag = false;

  while(pgm_read_byte_near(p+(LCD_COLS-1)) != '\0') {
    if (is_interrupt()) return;
    lcd.setCursor(0, cursorY);
    {
      // Create a substring by going through 20 characters on the progmem string.
      for(uint8_t i = 0; i < LCD_COLS; i++) {
        lcd.write(pgm_read_byte_near(p++));
      }
      // Move the pointer back 19 spaces to prepare for next character in substring.
      p-=(LCD_COLS-1);
    }
    if(!flag) {
      delay_ms(initalCharDelay);
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
  if (state == prgmState->get_state()) {
    return;
  }
  delete prgmState;

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

void select_btn_click() {

  // Ignore interrupt if not enough time has passed or the option button is being pressed.
  if ((millis() - lastButtonPress < DEBOUNCE_RATE) || digitalReadFast(BTN_OPTION) == LOW) {
    return;
  }
  lastButtonPress = millis();

  switch (prgmState->get_state()) {
  case MAIN_MENU:
  {
    immediateInterrupt = true;
    update_state(CM_MENU);
    return;
  }
  case LM_MENU:
  {
    // For skipping the instructions.
    if (!prgmState->has_initalized()) {
      immediateInterrupt = true;
      return;
    }
    // For actually playing the song.
    update_state(LM_PLAYING_SONG);
    return;
  }
  case LM_PLAYING_SONG:
  {
    return;
  }
  case CM_MENU:
  {
    immediateInterrupt = true;
    update_state(CM_CREATE_NEW);
    return;
  }
  default:
    return;
  }
}

void cancel_btn_click() {

  // Ignore interrupt if not enough time has passed or the option button is being pressed.
  if ((millis() - lastButtonPress < DEBOUNCE_RATE) || digitalReadFast(BTN_OPTION) == LOW) {
    return;
  }

  lastButtonPress = millis();

  switch (prgmState->get_state()) {
  case MAIN_MENU: {
    immediateInterrupt = true;
    update_state(LM_MENU);
    return;
  }
  case CM_MENU: {
    immediateInterrupt = true;
    update_state(MAIN_MENU);
    return;
  }
  case LM_MENU:
  {
    immediateInterrupt = true;
    update_state(MAIN_MENU);
    // If the user is leaving the listening menu then reset the song and page.
    set_selected_page(1);
    set_selected_song(1);
    return;
  }
  case LM_PLAYING_SONG:
  {
    return;
  }
  case CM_CREATE_NEW:
  {
    return;
  }
  }
}

bool is_pressed(uint8_t buttonPin) {
  if (!(millis() - lastButtonPress < DEBOUNCE_RATE) && digitalReadFast(buttonPin) == LOW) {
    lastButtonPress = millis();
    return true;
  }
  return false;
}

////////////////////////////
//// SD CARD FUNCTIONS ////
//////////////////////////

void sd_save_song(char* fileName, Song* song) {
  // Ensure the song being saved is correctly formatted.
  if(strlen(fileName) > 8 || strlen(fileName) < 1 || song->get_size() < MIN_SONG_LENGTH) {
    analogWrite(RGB_RED, RGB_BRIGHTNESS);
    #if DEBUG == true
    Serial.print(get_active_time());
    Serial.println(F(" INVALID FILE NAME/SONG SAVE."));
    Serial.print(get_active_time());
    Serial.println(F(" - File name must be between 1-8 characters."));
    Serial.print(get_active_time());
    Serial.println(F(" - Song must be at least 8 notes in length."));
    #endif
    while(true) { 
      ;
    }
  }
  File songFile;
  if (SD.exists(fileName)) {
#if DEBUG == true
    Serial.print(get_active_time());
    Serial.print(F(" "));
    Serial.print(fileName);
    Serial.println(F(" already exists. Overwriting..."));
#endif
    // Delete the old file.
    SD.remove(fileName);
  }
  else {
#if DEBUG == true
    Serial.print(get_active_time());
    Serial.print(F(" Creating "));
    Serial.println(fileName);
#endif
  }
  songFile = SD.open(fileName, FILE_WRITE);

  songFile.println(F("# Welcome to a song file!"));
  songFile.println(F("# To view more information, check out https://github.com/devjluvisi/TuneStudio2560/wiki/For-Users"));
  songFile.println(F("\n# The delay between each different tone (ms). (Must be 9999 or less and greater than 0)"));
  songFile.print(F("TONE_DELAY="));
  songFile.println(DEFAULT_NOTE_DELAY);
  songFile.println(F("\n# The length that each tone should play for (ms). (Must be 255 or less and greater than 0)"));
  songFile.print(F("TONE_LENGTH="));
  songFile.println(DEFAULT_NOTE_LENGTH);
  songFile.println(F("\nData:"));

  // Convert each frequency in the song to a pitch and save it on the SD.
  for (song_size_t i = 0; i < song->get_size(); i++) {
    songFile.print(F("  - "));
    songFile.println(get_note_from_freq(song->get_note(i)).pitch);
  }
  songFile.println(F("\n# END"));
  songFile.close();
#if DEBUG == true
  Serial.print(get_active_time());
  Serial.println(F(" Finished writing with SD Card."));
#endif
}

void sd_rem(const char* fileName) {
  char* name = (char*)fileName;
  if (SD.exists(name)) {
#if DEBUG == true
    Serial.print(get_active_time());
    Serial.print(F(" "));
    Serial.print(fileName);
    Serial.println(F(" HAS BEEN DELETED FROM SD CARD."));
#endif
    // Delete the old file.
    SD.remove(name);
  }
}

const char* sd_get_file(uint8_t index) {
  // "static" Fixes Heap Frag Errors.
  static File entry;
  File baseDir = SD.open(ROOT_DIR);
  baseDir.rewindDirectory();
  uint8_t count = 0;
  while (true) {
    entry = baseDir.openNextFile();
    // If the entry does not exist return an empty string.
    if (!entry) {
      baseDir.close();
      entry.close();
      return "";
    }

    // Skip if directory, README file, or if the file does not have a .txt extension.
    if (entry.isDirectory() || !strcasestr(entry.name(), FILE_TXT_EXTENSION) || strcmp(entry.name(), README_FILE) == 0) {
      entry.close();
      continue;
    }

    // Entry matches.
    if (count == index) {
      baseDir.close();
      entry.close();
      return entry.name();
    }
    entry.close();
    count++;
  }
  return "";
}

bool sd_songcpy(Song* song, const char* fileName) {

  File entry = SD.open(fileName);
  bool isToneDelay = true;

  // If the file does not exist.
  if (!entry) {
    entry.close();
    return false;
  }

  // Store these variables to update later if the user has input custom delays.
  uint8_t noteDelay = DEFAULT_NOTE_DELAY;
  uint16_t noteLength = DEFAULT_NOTE_LENGTH;

  uint16_t songSize = 0;

  // As long as their are avaliable characters in the file.
  while (entry.available()) {

    // Store the current character.
    char letter = entry.read();

    // Ignore the lines with hashtags.
    if (letter == '#') {
      while (letter != '\n') {
        letter = entry.read();
      }
    }

    // Attempt to read the tone delay and tone length values.
    if (letter == '=') {
      char buffer[5];
      uint8_t index = 0;
      while (entry.peek() != '\n') {
        letter = entry.read();
        // Ignore the spaces.
        if (letter == ' ') {
          continue;
        }
        // Add the character to the buffer.
        buffer[index] = letter;
        index++;
      }
      // Set a terminating character.
      buffer[index - 1] = '\0';

      if (isToneDelay) {
        isToneDelay = false;
        noteDelay = atoi(buffer);
#if DEBUG == true
        Serial.print(get_active_time());
        Serial.print(F(" Read a TONE DELAY of: "));
        Serial.println(buffer);
#endif
      }
      else {

        noteLength = atoi(buffer);
#if DEBUG == true
        Serial.print(get_active_time());
        Serial.print(F(" Read a TONE LENGTH of: "));
        Serial.println(buffer);
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
          return false;
        }
        letter = entry.read();
        // Ignore the spaces.
        if (letter == ' ') {
          continue;
        }
        // Add the character to the buffer.
        buffer[index] = letter;
        index++;
      }
      // Set a terminating character.
      buffer[index - 1] = '\0';
      // Add the note.
      note_t foundNote = get_note_from_pitch(buffer);
      if (foundNote.frequency == EMPTY_NOTE.frequency) {
        entry.close();
        return false;
      }
      song->add_note(foundNote.frequency);
      songSize++;
    }
  }
  if (songSize < MIN_SONG_LENGTH || songSize > MAX_SONG_LENGTH) {
    entry.close();
    return false;
  }

  // Update the note length and note delay with what the file has read.
  song->set_attributes(noteLength, noteDelay);

  // Close the file
  entry.close();
  return true;
#if DEBUG == true
  Serial.print(get_active_time());
  Serial.print(F(" Copied a song ("));
  Serial.print(fileName);
  Serial.println(F(") from SD card to memory."));
#endif
}

// FOR DEBUG USE ONLY
#if DEBUG == true
const char* get_active_time() {
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

  File readMe;

  if (SD.exists((char*)README_FILE)) {
    readMe.close();
    return;
  }
  
  // BIG STRING WOW
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
  static PROGMEM const char * const README_PTR = README_TEXT;

  readMe = SD.open(README_FILE, FILE_WRITE);
  for(uint16_t i = 0; i < strlen_P(README_TEXT); i++) {
    readMe.print((char)pgm_read_byte_near(&README_PTR[i]));
  }

  readMe.close();

}
