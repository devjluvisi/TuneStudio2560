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


#include <Arduino.h>
#include <tune_studio.h>
#include <LiquidCrystal_I2C.h>
#include <EEPROM.h>

LiquidCrystal_I2C lcd(0x27, 20, 4);  // set the LCD address to 0x27 for a 16 chars and 2 line display

void setup()
{
  // Create serial monitor.
  Serial.begin(9600);
  while (!Serial);
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

  attachInterrupt(digitalPinToInterrupt(BTN_ADD_SELECT), select_btn_click, CHANGE);

  // Initalize the LCD.
  lcd.init();
  lcd.backlight();
}

/*
A list of different actions that can be performed
on TuneStudio2560.
*/
enum CurrentAction {
  HOME_SCREEN,
  MODE_SELECT_SCREEN,
  LISTEN_MODE_SONG_SELECT,
  LISTEN_MODE_SONG_PLAY,
  LISTEN_MODE_DELETE_SONG,
  LISTEN_MODE_FLUSH_EEPROM,
  CREATOR_MODE_CREATE_NEW,
  CREATER_MODE_SAVE_CURRENT,
  CREATER_MODE_DELETE_SONG
};
// The current screen the user is on.
CurrentAction action = HOME_SCREEN;
// Indicates whether or not the current action has already been drawn to the LCD or not.
bool hasDrawn = false;
/*
For keeping track of the current time the user has been on a specific action.
Can be updated depending on what the action wants to do.
*/
int actionMillis = 0;

/*
The current time that TuneStudio2560 has been active.
*/
int aliveMillis = millis();

void home_screen() {
  if (!hasDrawn) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("TuneStudio2560");
    lcd.setCursor(0, 1);
    lcd.print("Click SELECT to enter creator mode.");
    delay(2000);
    hasDrawn = true;
  }
  lcd.scrollDisplayLeft();
}

void mode_select_screen() {
  if (!hasDrawn) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Select a mode.");
    hasDrawn = true;
  }
}

void update_action(CurrentAction newAction) {
  action = newAction;
  Serial.println("Changed current action to: " + newAction);
  hasDrawn = false;
}

void select_btn_click() {
  switch (action) {
  case HOME_SCREEN:
    update_action(MODE_SELECT_SCREEN);
    break;
  default:
    Serial.println(F("Error. Could not find specified action enum."));
  }
}

void loop()
{
  switch (action) {
  case HOME_SCREEN:
    home_screen();
    break;
  case MODE_SELECT_SCREEN:
    mode_select_screen();
    break;
  default:
    Serial.println(F("Unknown Mode."));
  }
  delay(500);
  aliveMillis = millis();
}