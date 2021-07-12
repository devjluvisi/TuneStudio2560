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
unsigned long lastInterruptFire = 0;


void delay(int milliseconds) {
  for (int i = 0; i < milliseconds; i++) {
    if (immediateInterrupt) return;
    delayMicroseconds(1000);
  }
}


///////////////////////////
//// CLASS DEFINITIONS ////
///////////////////////////

void print_large_text(String message) {
  unsigned int counter = 0;
  // Print out each character.
  while (counter != message.length()) {
    if (immediateInterrupt) return;
    // Ignore spaces on new lines. (Dont draw it)
    if (message[counter] == ' ') {
      if (counter % 16 != 0) {
        lcd.print(message[counter]);
      }
    }
    else {
      lcd.print(message[counter]);
    }

    counter++;

    // Every 16 characters go to a new line.
    if (counter % 16 == 0) {
      lcd.setCursor(0, 1);
    }

    // Every 32 characters clear the screen.
    if (counter % 32 == 0) {
      delay(500);
      lcd.clear();
      lcd.home();
      lcd.setCursor(0, 0);
    }
    delay(150);
  }
  // A temporary delay after the message has completely finished.
  delay(1000);
  // Clear the screen for future use.
  lcd.clear();
  lcd.setCursor(0, 0);
}

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

};

/**
 * @brief The Home Screen which is first seen when
 * the Arduino starts up.
 */
class HomeScreen : public ActionScreen {
private:
  const String message = "To enter creator mode press the select button. To enter listening mode press the delete button. To view more please check out my github.";
public:
  void load() {
    if (!hasDrawn) {
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

    // Scroll a link to my github.
    const String githubLink = "github.com/devjluvisi/TuneStudio2560";
    delay(500);
    lcd.print(githubLink);
    int count = githubLink.length() - 16;

    delay(2500);
    while (count != 0) {
      if (immediateInterrupt) return;
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
 * @brief A screen where the user selects to create a song or listen to a song.
 */
class ModeSelectScreen : public ActionScreen {
public:
  void load() {
    if (!hasDrawn) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Select a mode.");
      hasDrawn = true;
    }


  }
};

//////////////////////
//// ARDUINO CODE ////
//////////////////////

HomeScreen homeScreen;
ModeSelectScreen modeSelectScreen;

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
  pinMode(BTN_RST, INPUT_PULLUP);
  pinMode(TONE_FREQ, INPUT);
  pinMode(SPEAKER_1, OUTPUT);
  pinMode(SPEAKER_2, OUTPUT);

  // Interrupt to handle when the select button is pressed.
  attachInterrupt(digitalPinToInterrupt(BTN_ADD_SELECT), select_btn_click, CHANGE);

  // Initalize the LCD.
  lcd.init();
  lcd.backlight();
}

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

void update_action(CurrentAction newAction) {
  userAction = newAction;
  immediateInterrupt = true;
  Serial.println("Changed current action to: " + newAction);
}

void select_btn_click() {
  switch (userAction) {
  case HOME_SCREEN:
    update_action(MODE_SELECT_SCREEN);
    break;
  default:
    Serial.println(F("Error. Could not find specified action enum."));
  }
}

void loop()
{
  switch (userAction) {
  case HOME_SCREEN:
    homeScreen.load();
    break;
  case MODE_SELECT_SCREEN:
    modeSelectScreen.load();
    break;
  default:
    Serial.println(F("Unknown Mode."));
  }
  immediateInterrupt = false;
  aliveMillis = millis();

}