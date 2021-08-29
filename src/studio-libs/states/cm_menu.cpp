/**
 * @file cm_menu.cpp
 * @author Jacob LuVisi
 * @brief The class for managing the menu in creator mode.
 * @version 0.1
 * @date 2021-07-26
 *
 * @copyright Copyright (c) 2021
 *
 */
#include <studio-libs/states/states.h>

CreatorModeMenu::CreatorModeMenu(): ProgramState::ProgramState(CM_MENU) {}
CreatorModeMenu::~CreatorModeMenu() {}

void CreatorModeMenu::loop() {
  delay_ms(500);
  lcd.setCursor(3, 1);
  lcd.print(F("[Creator Mode]"));
  lcd.setCursor(1, 2);
  lcd.print(F(">> INSTRUCTIONS <<"));
  delay_ms(1250);

  print_lcd(F(
    "To start, press the select button.\n"
    "To exit, press the DEL/CANCEL button.\n"
    "Create a song using the 5 tune buttons.\n"
    "To add a tune, press the tune button and then press SELECT.\n"
    "To add a delay in the song press OPTION+BLUE TUNE.\n"
    "To just listen to a note without adding press a tune button without select.\n"
    "Adjust the frequency of the tune using the potentiometer.\n"
    "Delete notes using the DEL/CANCEL button.\n"
    "Save the song by pressing OPTION+SELECT button.\n"
    "Delete the current song (exit) by pressing OPTION+DEL.\n"
    "Play current track by pressing OPTION+GREEN TUNE.\n"
    #if PRGM_MODE != 0
    "Scroll through the track by pressing OPTION twice."
    #else
    "Scroll through the track by pressing OPTION twice.\n"
    "Check out my GitHub for detail on how notes and pitches work."
    #endif
    
    
  ));
#if PRGM_MODE != 0
  lcd.clear();
  delay_ms(500);
  lcd.setCursor(3, 1);
  lcd.print(F("[Creator Mode]"));
  lcd.setCursor(5, 2);
  lcd.print(F(">> INFO <<"));
  delay_ms(1250);

  print_lcd(F(
    "Each tune that is added will have a corresponding LETTER and NUMBER. TuneStudio2560 utilizes the standardized chromatic scale for notes.\n"
    "Each tune button represents a frequency between 31-3951. A tune button along with the potentiometer create a note.\n"
    "The type of note is displayed on the segment display. (Ex. GS6, A4, DS4)\n"
    "The individual tune buttons do not correspond with a letter or tone from the chromatic scale, just a frequency."
  ));

  delay_ms(500);
  lcd.clear();
  lcd.setCursor(0, 1);
  lcd.print(F("Freq. Ranges:"));
  delay_ms(500);
  print_scrolling(F("GREEN: B0 (31hz) to DS2 (78hz), BLUE: E2 (82hz) to GS3 (208hz), RED: A3 (220hz) to CS5 (554hz), YELLOW: D5 (587hz) to FS6 (1480hz), WHITE: G6 (1568hz) to B7 (3951hz)"), 2, 150);
  #endif
  delay_ms(1250);
  lcd.clear();
  delay_ms(500);
}

void CreatorModeMenu::init() {
  return;
}