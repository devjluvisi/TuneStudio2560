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


 // Initalize libraries.
#include <Arduino.h>
#include <tune_studio.h>
#include <EEPROM.h>
#include <pitches.h>
#include <song.h>
#include <current_state.h>

// measure memory usage.
#ifdef __arm__
// should use uinstd.h to define sbrk but Due causes a conflict
extern "C" char* sbrk(int incr);
#else  // __ARM__
extern char* __brkval;
#endif  // __arm__

int freeMemory() {
  char top;
#ifdef __arm__
  return &top - reinterpret_cast<char*>(sbrk(0));
#elif defined(CORE_TEENSY) || (ARDUINO > 103 && ARDUINO != 151)
  return &top - __brkval;
#else  // __arm__
  return __brkval ? &top - __brkval : &top - __malloc_heap_start;
#endif  // __arm__
}


/**
Indicates whether or not an immediate interrupt should be called.
Almost all loops in TuneStudio2560 have another condition to check for this interrupt.
If this flag is set to true then the current execution loop halts and attempts to return back
to the loop() function as fast as possible.

The delay(ms) function in this program also makes use of this variable.
*/
volatile bool immediateInterrupt = false;
/**
 * Indicates the last time that an interrupt was fired.
 */
volatile unsigned long lastInterruptFire = 0;
volatile unsigned long lastButtonPress = 0;

//////////////////////////////
//// INTERRUPTS & DELAYS ////
////////////////////////////

bool isInterrupt() {
  segDisplay.refreshDisplay();
  if (millis() % 500 == 0) {

    Serial.println("FREE MEMORY: " + String(freeMemory()) + "/8192");
  }
  return immediateInterrupt;
}

void delay(int milliseconds) {
  for (int i = 0; i < milliseconds; i++) {
    if (isInterrupt()) return;

    delayMicroseconds(1000);
  }
}


/*
The current time that TuneStudio2560 has been active.
*/
int aliveMillis = millis();

/*
Represents the current state of the application.
View current_state.h for more information on states.
*/
PossibleStates stateEnum = MAIN_MENU;

CurrentState currentState(stateEnum);

void setup()
{
  // Create serial monitor.
  Serial.begin(9600);
  while (!Serial);

  // Print welcome message to Serial monitor.
  Serial.println(F("--------------------------------------\n> TuneStudio2560 has initalized\n> Have fun!\n--------------------------------------"));

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
  /*
  lcd.createChar(MUSIC_NOTE_SYMBOL, MUSIC_NOTE);
  lcd.createChar(PLAYING_SONG_SYMBOL, PLAYING_SONG);
  lcd.createChar(PAUSE_SONG_SYMBOL, PAUSE_SONG);
  lcd.createChar(PROGRESS_BLOCK_SYMBOL, PROGRESS_BLOCK);
*/
// Setup 4-wide 7 segment display.
// More information: https://github.com/bridystone/SevSegShift

  byte numDigits = 4;
  byte digitPins[] = { 8 + 2, 8 + 5, 8 + 6, 2 }; // of ShiftRegister(s) | 8+x (2nd Register)
  byte segmentPins[] = { 8 + 3, 8 + 7, 4, 6, 7, 8 + 4, 3,  5 }; // of Shiftregister(s) | 8+x (2nd Register)
  /* configuration without ShiftRegister - Direct arduino connection
  byte digitPins[] = {2, 3, 4, 5}; // PINs of Arduino
  byte segmentPins[] = {6, 7, 8, 9, 10, 11, 12, 13}; // PINs of Arduino
  */
  bool resistorsOnSegments = false; // 'false' means resistors are on digit pins
  byte hardwareConfig = COMMON_CATHODE; // See README.md for options
  bool updateWithDelays = false; // Default 'false' is Recommended
  bool leadingZeros = false; // Use 'true' if you'd like to keep the leading zeros
  bool disableDecPoint = false; // Use 'true' if your decimal point doesn't exist or isn't connected. Then, you only need to specify 7 segmentPins[]

  segDisplay.begin(hardwareConfig, numDigits, digitPins, segmentPins, resistorsOnSegments,
    updateWithDelays, leadingZeros, disableDecPoint);
}


void loop()
{
  currentState.load();
  aliveMillis = millis();

}

////////////////////////
//// LCD FUNCTIONS ////
//////////////////////

void print_large_text(uint8_t numOfMessages, char* messages[]) {
  lcd.clear();
  uint8_t row = 0;
  for (uint8_t i = 0; i < numOfMessages; i++) {
    for (uint8_t j = 0; j < strlen(messages[i]); j++) {
      if (messages[i][j] == '\0') {
        break;
      }

      delay(135);
      if (j != 0 && (j) % LCD_COLS == 0) {
        row++;
        lcd.setCursor(0, row);
      }

      if (row == LCD_ROWS) {
        delay(500);
        lcd.clear();
        row = 0;
        lcd.setCursor(0, row);
      }

      if (!(j % LCD_COLS == 0 && messages[i][j] == ' ')) {
        lcd.write(messages[i][j]); // Print the character.
      }


    }
    if (messages[i][0] == '\0') {
      break;
    }
    row = 0;
    delay(500);
    lcd.clear();
    delay(500);
  }
}

void print_large_text(char* message) {
  lcd.clear();
  uint8_t row = 0;
  for (uint8_t j = 0; j < strlen(message); j++) {
    if (message[j] == '\0') {
      break;
    }

    delay(135);
    if (j != 0 && (j) % LCD_COLS == 0) {
      row++;
      lcd.setCursor(0, row);
    }

    if (row == LCD_ROWS) {
      delay(500);
      lcd.clear();
      row = 0;
      lcd.setCursor(0, row);
    }

    if (!(j % LCD_COLS == 0 && message[j] == ' ')) {
      lcd.write(message[j]); // Print the character.
    }
  }
}

void print_scrolling_text(uint8_t numOfMessages, char* messages[]) {
  lcd.clear();
  uint8_t cursor = 0;
  for (uint8_t i = 0; i < numOfMessages; i++) {
    for (uint8_t j = 0; j < strlen(messages[i]); j++) {
      lcd.setCursor(j, cursor);
      if (j < LCD_COLS) {
        lcd.write(messages[i][j]);
        delay(150);
        continue;
      }
      if (j == LCD_COLS) {
        delay(250); // Delay when the scrolling first starts,
      }
      lcd.setCursor(0, cursor);
      for (uint8_t k = 0; k < LCD_COLS; k++) {
        lcd.print(F(" "));
      }
      lcd.setCursor(0, cursor);
      for (uint8_t k = 0; k < LCD_COLS; k++) {
        if (messages[i][k + j - LCD_COLS + 1] == '\0') {
          break;
        }
        lcd.write(messages[i][k + j - LCD_COLS + 1]);
      }
      delay(200);
    }
    if (messages[i][0] == '\0') {
      break;
    }
    cursor++;
    delay(500);
  }

}

////////////////////////////
//// BUTTONS & ACTIONS ////
//////////////////////////

void select_btn_click() {
  if (millis() - lastButtonPress < DEBOUNCE_RATE) {
    return;
  }
  lastButtonPress = millis();
}

void cancel_btn_click() {
  if (millis() - lastButtonPress < DEBOUNCE_RATE) {
    return;
  }
  lastButtonPress = millis();
}

bool is_pressed(uint8_t buttonPin) {
  if (millis() - lastButtonPress < DEBOUNCE_RATE) {
    return false;
  }
  if (digitalRead(buttonPin) == LOW) {
    lastButtonPress = millis();
    return true;
  }
  return false;
}

bool is_pressed(uint8_t buttonPin1, uint8_t buttonPin2) {
  if (millis() - lastButtonPress < DEBOUNCE_RATE) {
    return false;
  }
  if (digitalRead(buttonPin1) == LOW && digitalRead(buttonPin2) == LOW) {
    lastButtonPress = millis();
    return true;
  }
  return false;
}

/////////////////////////////////
//// EEPROM LOADING & SAVING ////
////////////////////////////////

/*
void save_song_data() {
  if (sizeof(savedSongs) > EEPROM_SIZE) {
    // May give compiler error however the below println works fine.
    Serial.println("Size of saved songs (" + ((String)sizeof(savedSongs)) + ") is greater then the maximum capable size of the EEPROM (" + String(EEPROM_SIZE) + "). Please reduce the size of the songs or the maximum allowed number of songs to save this data.");
    return;
  }
  EEPROM.put(EEPROM_SONG_SAVE_ADDR, savedSongs);
  Serial.println("Saved songs to EEPROM.");
}

void delete_song(uint32_t index) {
  if (index < 1 || index >(sizeof(savedSongs) / sizeof(savedSongs[0]))) {
    Serial.println("ERR: Could not delete the song. Index out of bounds.");
    return;
  }
  savedSongs[index].clear();
}

void load_songs_from_eeprom() {
  // Load data to global variable.
  EEPROM.get(EEPROM_SONG_SAVE_ADDR, savedSongs);
}
*/

