/**
 * @file main.cpp
 * @author Jacob LuVisi
 * @brief The main class for TuneStudio2560.
 * @version 0.1
 * @date 2021-06-27 - ???
 *
 * @copyright Copyright (c) 2021
 *
 * TuneStudio2560
 * Open source song playback and creation tool made for the Arduino Mega 2560 and compatible.
 *
 * GitHub: https://github.com/devjluvisi/TuneStudio2560
 *
 * For library, unit testing, and code explanation please check out the wiki. (https://github.com/devjluvisi/TuneStudio2560/wiki)
 *
 * -------------------------------------------
 * HARDWARE:
 * A complete list of hardware needed can be found on the GitHub as well as the "For Users" wiki page.
 * TuneStudio2560 was created and designed for the 8-Bit AVR archetecture. It is possible to run this program
 * on AVR-compatible boards which can supply enough IO as well as at least 1x I2C bus and 1x SPI bus (optional).
 * A potential port to other microcontrollers like the Raspberry Pi Pico is under consideration.
 * -------------------------------------------
 * PINS: (A list of my pin layouts (for mega) are in tune_studio.h)
 *
 * - Note: Digital pins can be replaced with Analog Pins if you run out of Digital Pins or PWM.
 * These connections do not go over ground/5V pins. Please check each of your devices to figure out
 * where they go. These connections only describe the direct connections to the Arduino. More info on wiki.
 *
 * Connect RGB Led Red, Green, and Blue to any digital pins. (PWM if possible).
 * Connect Tune Buttons 1-5 on any digital pins.
 * Connect SELECT/ADD button to a digital pin that provides interrupt.
 * Connect DELETE/CANCEL button to a digital pin that provides interrupt.
 * Connect OPTION button to any digital pin.
 * Connect Potentiometer to any Analog pin.
 * Connect Speaker to any Analog pin.
 * Connect I2C LCD to SDA/SCL pins.
 * Connect 7-Segment display DIGIT pins to any Digital Pins (MEGA Only).
 * Connect MicroSD MOSI,MISO,SCK, and SS to the respective SPI pins on your board. If you do not have enough pins
 * then connect them to the ICSP pins (also supports SPI).
 *
 * Total Pins Required: 22
 * - 14 Digital/Analog Pins (4 PWM Reccomended, 1 Required)
 * - 2 Analog Pins
 * - (I2C) SDA/SCL Pins [2 Pins]
 * - (SPI) MOSI, MISO, SCK, SS Pins [4 Pins]
 * -------------------------------------------
 *
 * TuneStudio2560 was developed using PlatformIO on Visual Studio Code and it is highly reccomended to modify
 * this project using that framework.
 *
 * LICENSE - MIT
 */

 // Main Header File
#include <studio-libs/tune_studio.h>
#include <studio-libs/states/states.h>

 /**
 Indicates whether or not an immediate interrupt should be called.
 Almost all loops in TuneStudio2560 have another condition to check for this interrupt.
 If this flag is set to true then the current execution loop halts and attempts to return back
 to the main loop() function as fast as possible.

 The delay(ms) function in this program also makes use of this variable.
 */
volatile bool immediateInterrupt = false;
volatile unsigned long lastButtonPress = 0; // The last time a button was pressed in millis().
volatile uint8_t selectedSong = 1; // The current song which is selected in any circumstance. (STARTS AT 1)
volatile uint8_t selectedPage = 1; // The current page of songs which is selected. Each page is a group of 5 songs. (microSD only) (STARTS AT 1)
volatile bool SDEnabled = true; // If the microSD card is enabled.



//////////////////////////////
//// INTERRUPTS & DELAYS ////
////////////////////////////

bool is_interrupt() {
  segDisplay.refreshDisplay();
#if DEBUG == true
  if (millis() % 1250 == 0) {
    Serial.println();
    Serial.println("FREE MEMORY: " + String(freeMemory()) + +"KB/8192KB");
    Serial.println("STACK SIZE: " + String((RAMEND - SP)) + "KB");
    Serial.println("HEAP FRAG: " + String(getFragmentation()) + "%");
    Serial.println();
  }
#endif

  return immediateInterrupt;
}

ProgramState* prgmState;

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
#endif

  // Setup all of the pins.
  pinMode(RGB_BLUE, OUTPUT);
  pinMode(RGB_GREEN, OUTPUT);
  pinMode(RGB_RED, OUTPUT);
  pinMode(BTN_TONE_1, INPUT_PULLUP);
  pinMode(BTN_TONE_2, INPUT_PULLUP);
  pinMode(BTN_TONE_3, INPUT_PULLUP);
  pinMode(BTN_TONE_4, INPUT_PULLUP);
  pinMode(BTN_TONE_5, INPUT_PULLUP);
  pinMode(BTN_ADD_SELECT, INPUT_PULLUP);
  pinMode(BTN_DEL_CANCEL, INPUT_PULLUP);
  pinMode(BTN_OPTION, INPUT_PULLUP);
  pinMode(TONE_FREQ, INPUT);
  pinMode(SPEAKER_1, OUTPUT);

  // Interrupt to handle when the select button is pressed.
  attachInterrupt(digitalPinToInterrupt(BTN_ADD_SELECT), select_btn_click, CHANGE);
  attachInterrupt(digitalPinToInterrupt(BTN_DEL_CANCEL), cancel_btn_click, CHANGE);

  // Initalize the LCD.
  lcd.init();
  lcd.backlight();

  // Add custom characters to the LCD.
  byte buffer[8];
  memcpy_P(buffer, MUSIC_NOTE, 8);
  lcd.createChar(MUSIC_NOTE_SYMBOL, buffer);
  memcpy_P(buffer, PLAYING_SONG, 8);
  lcd.createChar(PLAYING_SONG_SYMBOL, buffer);
  memcpy_P(buffer, PAUSE_SONG, 8);
  lcd.createChar(PAUSE_SONG_SYMBOL, buffer);
  memcpy_P(buffer, PROGRESS_BLOCK, 8);
  lcd.createChar(PROGRESS_BLOCK_SYMBOL, buffer);

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

bool sd_enabled() {
  return SDEnabled;
}

void set_selected_page(uint8_t page) {
  selectedPage = page;
#if DEBUG == true
  Serial.print(F("Changed the page to "));
  Serial.print(String(page));
  Serial.println(F("."));
#endif
}

uint8_t get_selected_page() {
  return selectedPage;
}

void set_selected_song(uint8_t song) {
  selectedSong = song;
#if DEBUG == true
  Serial.print(F("Changed the current song to "));
  Serial.print(String(song));
  Serial.println(F("."));
#endif

}

uint8_t get_selected_song() {
  return selectedSong;
}

note get_note_from_freq(const uint16_t frequency) {
  if (frequency == PAUSE_NOTE.frequency) {
    return PAUSE_NOTE;
  }
  //TODO: Make searching algorithm more efficient
  // 5 tune buttons, 17 pitches each
  for (uint8_t i = 0; i < TONE_BUTTON_AMOUNT; i++) {
    for (uint8_t j = 0; j < TONES_PER_BUTTON; j++) {
      if (pgm_read_word(&toneButtons[i].notes[j].frequency) == frequency) {
        char* pitch = (char*)pgm_read_word(&toneButtons[i].notes[j].pitch);
        //uint16_t freq = (uint16_t)pgm_read_word(&toneButtons[i].notes[j].frequency);
        return note{ pitch, frequency }; // Dont need to find the frequency because the parameter already passed it.
      }
    }
  }
  return note{ "0000", 0 };
}

note get_note_from_pitch(const char* pitch) {
  if (pitch == PAUSE_NOTE.pitch) {
    return PAUSE_NOTE;
  }
  //TODO: Make searching algorithm more efficient
  // 5 tune buttons, 17 pitches each
  for (uint8_t i = 0; i < TONE_BUTTON_AMOUNT; i++) {
    for (uint8_t j = 0; j < TONES_PER_BUTTON; j++) {
      if (((char*)pgm_read_word(&toneButtons[i].notes[j].pitch)) == pitch) {
        //char* pitch = (char*)pgm_read_word(&toneButtons[i].notes[j].pitch);
        uint16_t freq = (uint16_t)pgm_read_word(&toneButtons[i].notes[j].frequency);
        return note{ pitch, freq }; // Dont need to find the pitch because the parameter already passed it.
      }
    }
  }
  return note{ "0000", 0 };
}

note get_current_tone(uint8_t toneButton) {

  // Split the potentiometer value into 17 different sections because each tune button represents 17 different tones.
  // Note that the subTone value is not evenly split and the final subTone (17) has slightly less potential values.
  const uint16_t subTone = (uint16_t)(get_current_freq() + 1) / (uint8_t)61;

  switch (toneButton) {
  case 0:
    return { "0000", 0 };
  case BTN_TONE_1:
    return note{ ((char*)pgm_read_word(&toneButtons[0].notes[subTone].pitch)), ((uint16_t)pgm_read_word(&toneButtons[0].notes[subTone].frequency)) };
  case BTN_TONE_2:
    return note{ ((char*)pgm_read_word(&toneButtons[1].notes[subTone].pitch)), ((uint16_t)pgm_read_word(&toneButtons[1].notes[subTone].frequency)) };
  case BTN_TONE_3:
    return note{ ((char*)pgm_read_word(&toneButtons[2].notes[subTone].pitch)), ((uint16_t)pgm_read_word(&toneButtons[2].notes[subTone].frequency)) };
  case BTN_TONE_4:
    return note{ ((char*)pgm_read_word(&toneButtons[3].notes[subTone].pitch)), ((uint16_t)pgm_read_word(&toneButtons[3].notes[subTone].frequency)) };
  case BTN_TONE_5:
    return note{ ((char*)pgm_read_word(&toneButtons[4].notes[subTone].pitch)), ((uint16_t)pgm_read_word(&toneButtons[4].notes[subTone].frequency)) };
  default:
#if DEBUG == true
    Serial.println(F("Error in get_current_tone(uint8_t toneButton). Cannot find specified tone button."));
#endif
    //memcpy_P(&nt, &toneButtons[0].notes[0], sizeof(note));
    return note{ "0000", 0 };
  }

}

uint16_t get_current_freq() {
  return analogRead(TONE_FREQ);
}

////////////////////////
//// LCD FUNCTIONS ////
//////////////////////

uint16_t FSHlength(const __FlashStringHelper* FSHinput) {
  PGM_P FSHinputPointer = reinterpret_cast<PGM_P>(FSHinput);
  uint16_t stringLength = 0;
  while (pgm_read_byte(FSHinputPointer++)) {
    stringLength++;
  }
  return stringLength;
}

void print_lcd(const __FlashStringHelper* text, uint8_t charDelay) {
  lcd.clear();
  uint8_t cursorX = 0; // Track cursor on X position.
  uint8_t cursorY = 0; // Track cursor on Y position.

  const uint8_t length = FSHlength(text);
  char buffer[length + 1];
  memcpy_P(buffer, text, length + 1);

  for (uint8_t i = 0; i < length; i++) {
    if (is_interrupt()) return;
    lcd.setCursor(cursorX, cursorY);
    // Check if the beginning character is a space.
    if (!(cursorX == 0 && buffer[i] == ' ')) {
      lcd.print(buffer[i]); // Print letter to LCD.
      delay(charDelay);
      cursorX++; // Go up by one cursor.
    }
    // Reached end of the row.
    if (cursorX != 0 && cursorX % LCD_COLS == 0) {
      cursorY++;
      cursorX = 0;
    }

    // Reached end of the LCD.
    if (cursorY != 0 && cursorY % LCD_ROWS == 0) {
      delay(600);
      cursorX = 0;
      cursorY = 0;
      lcd.clear();
    }
  }
}

void print_scrolling(const __FlashStringHelper* text, uint8_t cursorY, uint8_t charDelay) {

  const uint16_t initalCharDelay = charDelay * 2;
  // Copy the string from PROGMEM to the stack.
  const uint8_t length = FSHlength(text);
  char buffer[length + 1];
  memcpy_P(buffer, text, length + 1);

  for (uint8_t i = 0; i < length - LCD_COLS + 1; i++) {
    if (is_interrupt()) return;
    lcd.setCursor(0, cursorY);
    {
      char subStr[LCD_COLS];
      memcpy(subStr, buffer + i, LCD_COLS);

      for (uint8_t j = 0; j < LCD_COLS; j++) {
        lcd.write(subStr[j]);
      }
    }

    if (i == 0) {
      delay(initalCharDelay); // Delay an extra amount for the inital text..
    }
    delay(charDelay);
  }
}




////////////////////////////
//// BUTTONS & ACTIONS ////
//////////////////////////

void update_state(StateID state) {
  if (state == prgmState->get_state()) {
    return;
  }
  delete prgmState;

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
    Serial.println(F("Unknown State Requested."));
#endif
    return;
  }
}

void select_btn_click() {

  // Ignore interrupt if not enough time has passed or the option button is being pressed.
  if ((millis() - lastButtonPress < DEBOUNCE_RATE) || digitalRead(BTN_OPTION) == LOW) {
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
  if ((millis() - lastButtonPress < DEBOUNCE_RATE) || digitalRead(BTN_OPTION) == LOW) {
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
    update_state(LM_MENU);
    return;
  }
  case CM_CREATE_NEW:
  {
    return;
  }
  }
}

bool is_pressed(uint8_t buttonPin) {
  if (!(millis() - lastButtonPress < DEBOUNCE_RATE) && digitalRead(buttonPin) == LOW) {
    lastButtonPress = millis();
    return true;
  }
  return false;
}

bool is_pressed(uint8_t buttonPin1, uint8_t buttonPin2) {
  if (!(millis() - lastButtonPress < DEBOUNCE_RATE) && digitalRead(buttonPin1) == LOW && digitalRead(buttonPin2) == LOW) {
    lastButtonPress = millis();
    return true;
  }
  return false;
}