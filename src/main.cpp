/*
-------------------------------
TUNE STUDIO 2560
Open source song creation and playback using the ATMega2560 microchip
on AVR 8-Bit architecture.

CURRENT PROJECT STATUS: NOT FUNCTIONAL
-------------------------------
Official GitHub Repo:
  - (TBD)

Hardware Compatability:
  - TuneStudio2560 was created specifically for 8-bit AVR microarchitecture although
  it may be possible to run TuneStudio2560 on compatible AVR boards which can supply
  the neccessarily Analog/Digital & I2C interfaces.
  - A port to the RP2040 microchip is currently under consideration.

Unit Testing Information:
  - (TBD)

C++ Library Information:
  - (TBD)

TuneStudio2560 was created entirely in PlatformIO a visual studio extension
which allows the fast and complete creation of programs for various microcontrollers.
For the best compatiblity it is reccomended that this GitHub repo is edited and run
using PlatformIO.
--------------------------------
By Jacob LuVisi (2021)
6/27/2021 - ???

*/

/*
Check tune_studio.h for variables which are not defined here.
*/

// Initalize libraries.
#include <Arduino.h>
#include <tune_studio.h>
#include <EEPROM.h>

//TODO: Implement the adding of songs.

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
  return immediateInterrupt;
}

void delay(int milliseconds) {
  for (int i = 0; i < milliseconds; i++) {
    if (isInterrupt()) return;
    delayMicroseconds(1000);
  }
}

///////////////////////////
//// CLASS DEFINITIONS ////
///////////////////////////

/**
 * @brief A abstract class which manages methods and variables for all
 * of the different types of action screens.
 */
class ActionScreen {

#define INITAL_DISPLAY_SLEEP 4000

protected:
  bool hasDrawn;
public:
  virtual void load() = 0;
  /**
   * @brief Set the Drawn object
   *
   * @param val What to set the value of hasDrawn to.
   */
  void setDrawn(bool val) {
    hasDrawn = val;
  }

};

/**
 * @brief The Home Screen which is first seen when
 * the Arduino starts up.
 */
class HomeScreen : public ActionScreen {
private:
  const String message = "To enter creator mode press the select button. To enter listening mode press the delete button. To view more information please check out my github.";
public:
  void load() {
    if (!hasDrawn) {
      lcd.clear();
      lcd.home();
      lcd.setCursor(0, 0);
      lcd.print("Welcome to...");
      lcd.setCursor(1, 1);
      lcd.print("TuneStudio2560");
      delay(INITAL_DISPLAY_SLEEP);
      lcd.clear();
      lcd.setCursor(0, 0);
      hasDrawn = true;
    }

    // Print out the message.
    print_large_text(message);
    delay(1000);

    // Scroll a link to my github.
    const String githubLink = "github.com/devjluvisi/TuneStudio2560";
    delay(500);
    lcd.print(githubLink);
    int count = githubLink.length() - 16;

    delay(2500);
    while (count != 0) {
      if (isInterrupt()) return;
      lcd.scrollDisplayLeft();
      count--;
      delay(550);
    }
    delay(600); // Short delay after message end.
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("<3");
    delay(600);
    // GITHUB SCROLL END

    // finally clear the lcd.
    lcd.clear();
    lcd.setCursor(0, 0);
    delay(1000);

    // Go back to the title screen (redraw everything)
    this->hasDrawn = false;

  }

};

/**
 * @brief A screen that displays what creator mode is about.
 */
class CreatorModeHome : public ActionScreen {
public:
  void load() {
    if (!hasDrawn) {
      lcd.clear();
      lcd.home();
      lcd.setCursor(0, 0);
      hasDrawn = true;
    }
    print_bottom_scrolling_text_v2("[Creator Mode]", "To start, press the SELECT button.");
    print_bottom_scrolling_text_v2("[Creator Mode]", "To exit, press the DEL/CANCEL button.");
    print_bottom_scrolling_text_v2("[Creator Mode]", "Create a song using the 5 tune buttons.");
    print_bottom_scrolling_text_v2("[Creator Mode]", "Adjust the frequency using the potentiometer.");
    print_bottom_scrolling_text_v2("[Creator Mode]", "Delete notes using the DEL/CANCEL button.");
    print_bottom_scrolling_text_v2("[Creator Mode]", "Save the song by pressing SELECT button.");
    print_bottom_scrolling_text_v2("[Creator Mode]", "To just listen to a note without adding press OPTION+tune button.");
    print_bottom_scrolling_text_v2("[Creator Mode]", "Delete the current song (exit) by pressing OPTION+DEL.");
    print_bottom_scrolling_text_v2("[Creator Mode]", "Play current track by pressing OPTION+ADD.");
  }
};

class CreatorModeCreateNew : public ActionScreen {
private:
  // Tracks the top text of the LCD to see if it has changed.
  String lcdTopText = "";
  // Tracks the bottom text of the LCD to see if it has changed.
  String lcdBottomText = "";
public:
  //TODO: NEXT VERSION - IMPLEMENT
  void load() {
    if (!hasDrawn) {
      lcd.clear();
      lcd.home();
      lcd.setCursor(0, 0);
      lcd.print("-END");
      hasDrawn = true;
    }

  }
};

class ListenModeSongSelect : public ActionScreen {
private:
  int selectedSong = -1;
public:
  void load() {
    if (!hasDrawn) {
      lcd.clear();
      delay(500);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("[Listen Mode]");
      lcd.setCursor(0, 1);
      lcd.print("> INSTRUCTIONS <");
      delay(2000);
      print_bottom_scrolling_text_v2("[Listen Mode]", "Select 1 of the 5 tune buttons to play a song saved in memory.");
      delay(500);
      print_bottom_scrolling_text_v2("[Listen Mode]", "Press the \"DEL/CANCEL\" button to go back to main menu.");
      delay(500);
      print_bottom_scrolling_text_v2("[Listen Mode]", "Press \"SELECT\" to pause song.");
      delay(500);
      print_bottom_scrolling_text_v2("[Listen Mode]", "Press \"OPTION+DEL\" to delete song.");
      delay(1500);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("[Listen Mode]");
      lcd.setCursor(0, 1);
      lcd.print("Awaiting Input.");
      hasDrawn = true;
    }
    if (selectedSong == -1) {
      if (isPressed(BTN_TONE_1)) {
        Serial.println("PRESSED TONE 1");
        selectedSong = 1;
        delay(1000);
      }
      if (isPressed(BTN_TONE_2)) {
        Serial.println("PRESSED TONE 2");
        selectedSong = 2;
        delay(1000);
      }
      if (isPressed(BTN_TONE_3)) {
        Serial.println("PRESSED TONE 3");
        selectedSong = 3;
        delay(1000);
      }
      if (isPressed(BTN_TONE_4)) {
        Serial.println("PRESSED TONE 4");
        selectedSong = 4;
        delay(1000);
      }
      if (isPressed(BTN_TONE_5)) {
        Serial.println("PRESSED TONE 5");
        selectedSong = 5;
        delay(1000);
      }
    }
    else {
      // Player has selected a song.
      lcd.clear();
      lcd.home();
      lcd.setCursor(0, 0);
      lcd.write(byte(PLAYING_SONG_SYMBOL));
      lcd.print(" ");
      lcd.write(byte(MUSIC_NOTE_SYMBOL));
      lcd.print(" Song: #" + String(selectedSong));
      lcd.setCursor(0, 1);
      lcd.print("PROGRESS: ");
      for (int i = 0; i < 5; i++) {
        lcd.write(byte(PROGRESS_BLOCK_SYMBOL));
      }
      delay(5000);
    }
  }
};

//////////////////////
//// ARDUINO CODE ////
//////////////////////

HomeScreen homeScreen;
CreatorModeHome creatorModeHome;
CreatorModeCreateNew createNewSong;
ListenModeSongSelect selectSongScreen;


// The current screen the user is on.
CurrentAction userAction = HOME_SCREEN;

/*
For keeping track of the current time the user has been on a specific action.
Can be updated depending on what the action wants to do.
*/
int actionMillis = 0;

/*
The current time that TuneStudio2560 has been active.
*/
int aliveMillis = millis();

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
  pinMode(SPEAKER_2, OUTPUT);

  // Interrupt to handle when the select button is pressed.
  attachInterrupt(digitalPinToInterrupt(BTN_ADD_SELECT), select_btn_click, CHANGE);
  attachInterrupt(digitalPinToInterrupt(BTN_DEL_CANCEL), cancel_btn_click, CHANGE);
  // Initalize the LCD.
  lcd.init();
  lcd.backlight();
  lcd.createChar(MUSIC_NOTE_SYMBOL, MUSIC_NOTE);
  lcd.createChar(PLAYING_SONG_SYMBOL, PLAYING_SONG);
  lcd.createChar(PAUSE_SONG_SYMBOL, PAUSE_SONG);
  lcd.createChar(PROGRESS_BLOCK_SYMBOL, PROGRESS_BLOCK);
}

void loop()
{
  switch (userAction) {
  case HOME_SCREEN:
    homeScreen.load();
    break;
  case LISTEN_MODE_SONG_SELECT:
    selectSongScreen.load();
    break;
  case CREATOR_MODE_HOME:
    creatorModeHome.load();
    break;
  case CREATOR_MODE_CREATE_NEW:
    createNewSong.load();
  default:
    Serial.println(F("Unknown Mode."));
  }
  immediateInterrupt = false;
  aliveMillis = millis();
}

////////////////////////
//// LCD FUNCTIONS ////
//////////////////////

void print_large_text(String message) {
  // Go through evert 16 characters.
  for (uint16_t i = 0; i < message.length(); i += 16) {
    if (isInterrupt()) return;
    String sub;
    // Go through every letter.
    for (uint16_t j = i; j < (i + 16); j++) {
      if (isInterrupt()) return;
      if (j > message.length() - 1) {
        break;
      }
      sub.concat(message[j]);
    }
    sub.trim(); // Remove white spaces.

    if (i % 32 == 0) {
      delay(650);
      lcd.clear();
      lcd.home();
      lcd.setCursor(0, 0);
      delay(300);

      for (char c : sub) {
        if (isInterrupt()) return;
        lcd.print(c);
        delay(150);
      }
    }
    else {
      lcd.setCursor(0, 1);
      for (char c : sub) {
        if (isInterrupt()) return;
        lcd.print(c);
        delay(150);
      }
    }
  }
  lcd.clear();
  lcd.home();
  lcd.setCursor(0, 0);
}

void print_bottom_scrolling_text(String top, String bottom, bool forward = true) {
  lcd.clear();
  lcd.home();
  lcd.setCursor(0, 0);
  lcd.print(top);
  if (forward) {
    for (uint8_t i = 0; i < bottom.length(); i++) {
      if (isInterrupt()) return;
      lcd.setCursor(0, 1);
      // Create a substring of the primary string that could fit on a row of the display.
      lcd.print(bottom.substring(i, (i + 16) > bottom.length() ? bottom.length() : i + 16));
      delay(450);
      if (i == 0) {
        delay(800);
      }
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(top);
    }
  }
  else {

    for (uint8_t i = bottom.length(); i > 0; i--) {
      if (isInterrupt()) return;
      lcd.setCursor(0, 1);
      lcd.print(bottom.substring(i, (i + 16) > bottom.length() ? bottom.length() : i + 16));
      delay(450);
      if (i == 0) {
        delay(800);
      }
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(top);
    }

  }
}


void print_bottom_scrolling_text_v2(String top, String bottom) {
  lcd.clear();
  lcd.home();
  lcd.setCursor(0, 0);
  lcd.print(top);

  uint8_t cursor = 0;
  for (uint8_t i = 0; i < bottom.length(); i++) {
    if (isInterrupt()) return;
    if (i % 16 == 0) {
      cursor = 0;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(top);
    }
    lcd.setCursor(cursor, 1);
    // Ignore spaces on new lines
    if (i % 16 == 0 && bottom[i] == ' ') {
      continue;
    }
    lcd.print(bottom[i]);
    cursor++;
    delay(TEXT_SCROLL_SPEED);
  }
}

////////////////////////////
//// BUTTONS & ACTIONS ////
//////////////////////////

void update_action(CurrentAction newAction) {
  if (millis() - lastInterruptFire < DEBOUNCE_RATE) {
    return;
  }
  userAction = newAction;
  immediateInterrupt = true;
  lastInterruptFire = millis();
  Serial.println("Changed current action to: " + String(newAction));
}

void select_btn_click() {
  if (millis() - lastButtonPress < DEBOUNCE_RATE) {
    return;
  }
  switch (userAction) {
  case HOME_SCREEN:
    update_action(CREATOR_MODE_HOME);
    break;
  case LISTEN_MODE_SONG_SELECT: // Used for speeding up the text.
    immediateInterrupt = true;
    lastInterruptFire = millis();
    break;
  case CREATOR_MODE_HOME:
    update_action(CREATOR_MODE_CREATE_NEW);
    break;
  default:
    Serial.println(F("No routing from current action."));
    return;
  }
  lastButtonPress = millis();
}

void cancel_btn_click() {
  if (millis() - lastButtonPress < DEBOUNCE_RATE) {
    return;
  }
  switch (userAction) {
  case HOME_SCREEN:
    selectSongScreen.setDrawn(false);
    update_action(LISTEN_MODE_SONG_SELECT);
    break;
  case LISTEN_MODE_SONG_SELECT:
    // Redraw the home screen.
    homeScreen.setDrawn(false);
    update_action(HOME_SCREEN);
    break;
  case CREATOR_MODE_HOME:
    // Redraw the home screen.
    homeScreen.setDrawn(false);
    update_action(HOME_SCREEN);
    break;
  default:
    Serial.println(F("No routing from current action."));
    return;
  }
  lastButtonPress = millis();
}

bool isPressed(uint8_t buttonPin) {
  if (millis() - lastButtonPress < DEBOUNCE_RATE) {
    return false;
  }
  if (digitalRead(buttonPin) == LOW) {
    lastButtonPress = millis();
    return true;
  }
}

bool isPressed(uint8_t buttonPin1, uint8_t buttonPin2) {
  if (millis() - lastButtonPress < DEBOUNCE_RATE) {
    return false;
  }
  if (digitalRead(buttonPin1) == LOW && digitalRead(buttonPin2) == LOW) {
    lastButtonPress = millis();
    return true;
  }
}


