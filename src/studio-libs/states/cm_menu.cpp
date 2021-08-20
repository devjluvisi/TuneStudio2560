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

CreatorModeMenu::CreatorModeMenu() : ProgramState::ProgramState(CM_MENU) {}
CreatorModeMenu::~CreatorModeMenu() {}

void CreatorModeMenu::loop() {
    const uint16_t delayTime = 500;
    delay_ms(delayTime);
    lcd.setCursor(3, 1);
    lcd.print(F("[Creator Mode]"));
    lcd.setCursor(1, 2);
    lcd.print(F(">> INSTRUCTIONS <<"));
    delay_ms(delayTime * 2.5F);
    print_lcd(F("To start, press the select button."));
    print_lcd(F("To exit, press the DEL/CANCEL button."));
    print_lcd(F("Create a song using the 5 tune buttons."));
    print_lcd(F("To add a tune, press the tune button and then press SELECT."));
    print_lcd(F("To add a delay in the song press OPTION+BLUE TUNE."));
    print_lcd(F("To just listen to a note without adding press a tune button without select."));
    print_lcd(F("Adjust the frequency of the tune using the potentiometer."));
    print_lcd(F("Delete notes using the DEL/CANCEL button."));
    print_lcd(F("Save the song by pressing OPTION+SELECT button."));
    print_lcd(F("Delete the current song (exit) by pressing OPTION+DEL."));
    print_lcd(F("Play current track by pressing OPTION+GREEN TUNE."));
    print_lcd(F("Scroll through the track by pressing OPTION twice."));
    lcd.clear();
    delay_ms(delayTime);
    lcd.setCursor(3, 1);
    lcd.print(F("[Creator Mode]"));
    lcd.setCursor(5, 2);
    lcd.print(F(">> INFO <<"));
    delay_ms(1250);
    print_lcd(F("Each tune that is added will have a corresponding LETTER and NUMBER. TuneStudio2560 utilizes the standardized chromatic scale for notes."));
    print_lcd(F("Each tune button represents a frequency between 31-3951. A tune button along with the potentiometer create a note."));
    print_lcd(F("The type of note is displayed on the segment display. (Ex. GS6, A4, DS4)"));
    print_lcd(F("The individual tune buttons do not correspond with a letter or tone from the chromatic scale, just a frequency."));
    delay_ms(delayTime);
    lcd.setCursor(0, 0);
    lcd.print(F("The possible"));
    lcd.setCursor(0, 1);
    lcd.print(F("frequency ranges:"));
    delay_ms(delayTime);
    print_scrolling(F("GREEN: B0 (31hz) to DS2 (78hz), BLUE: E2 (82hz) to GS3 (208hz), RED: A3 (220hz) to CS5 (554hz), YELLOW: D5 (587hz) to FS6 (1480hz), WHITE: G6 (1568hz) to B7 (3951hz)"), 2, 150);
    delay_ms(delayTime * 2.5F);
    lcd.clear();
    delay_ms(delayTime);

}

void CreatorModeMenu::init() {
    return;
}