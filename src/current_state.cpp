/**
 * @file current_state.cpp
 * @author Jacob LuVisi
 * @brief The logic behind the current application states.
 * @version 0.1
 * @date 2021-07-18
 *
 * @copyright Copyright (c) 2021
 *
 */

#include <current_state.h>

CurrentState::CurrentState(PossibleStates currentState) {
    _currentState = currentState;
}

void CurrentState::set_state(PossibleStates state) {
    Serial.print(F("CHANGED STATE TO: "));
    Serial.print(state);
    Serial.print(F(" from "));
    Serial.println(_currentState);
    _currentState = state;
    _hasInitalized = false; // Disable any previous initalizations.
}

PossibleStates CurrentState::get_state() {
    return _currentState;
}

void CurrentState::init() {
    // If the method has already ran then do not run.
    if (_hasInitalized) {
        return;
    }
    lcd.clear(); // Remove all information from the previous state.
    segDisplay.blank(); // Clear the 7 segment display.
    segDisplay.refreshDisplay(); // Refresh it to clear.

    switch (_currentState) {
    case MAIN_MENU:
        break; // Break immediatly because there is no initalization we have to do for the main menu.
    case CM_MENU:
        break;
    case LM_MENU:
        Serial.println(F("Entered listening mode."));
        delay(500);
        lcd.setCursor(3, 1);
        lcd.print(F("[Listen Mode]"));
        lcd.setCursor(1, 2);
        lcd.print(F(">> INSTRUCTIONS <<"));
        delay(1250);
        lcd.clear();
        print_lcd(F("Press select button to skip instructions."), 50);
        delay(500);
        print_lcd(F("Select 1 of the 5 tune buttons to play a song saved in memory."));
        print_lcd(F("Press the \"DEL/CANCEL\" button to go back to main menu."));
        print_lcd(F("Press \"SELECT\" to pause song."));
        print_lcd(F("Press \"OPTION+DEL\" to delete song."));
        delay(1500);
        break;
    case LM_PLAYING_SONG:
        break;
    case CM_CREATE_NEW:
        break;
    default:
        Serial.println(F("STATE ERROR."));
    }
    _hasInitalized = true;
}

void CurrentState::load() {

    switch (_currentState) {
    case MAIN_MENU:
    {
        lcd.setCursor(1, 0);
        lcd.print(F("Welcome to..."));
        lcd.setCursor(2, 1);
        lcd.print(F("TuneStudio2560"));
        delay(4000);
        lcd.clear();
        lcd.setCursor(5, 1);
        lcd.print(F("By"));
        lcd.setCursor(2, 2);
        lcd.print(F("Jacob LuVisi"));
        delay(2000);
        print_lcd(F("To enter creator mode press the select button. To enter listening mode press the delete button. To view more information please check out my github."));
        delay(1000);
        lcd.clear();
        lcd.setCursor(0, 1);
        lcd.print(F("GitHub: "));
        print_scrolling(F("github.com/devjluvisi/TuneStudio2560"), 2, 235);
        // Information message.
        delay(2000);
        lcd.clear(); // Clear the screen incase the method needs to run again.
        delay(1000);
    }
    break;
    case CM_MENU:
    {
        delay(500);
        lcd.setCursor(3, 1);
        lcd.print(F("[Creator Mode]"));
        lcd.setCursor(1, 2);
        lcd.print(F(">> INSTRUCTIONS <<"));
        delay(1250);
        print_lcd(F("To start, press the select button."));
        print_lcd(F("To exit, press the DEL/CANCEL button."));
        print_lcd(F("Create a song using the 5 tune buttons."));
        print_lcd(F("To add a tune, press the tune button and then press SELECT."));
        print_lcd(F("To add a delay in the song press SELECT without pressing a tune button before."));
        print_lcd(F("To just listen to a note without adding press a tune button without select."));
        print_lcd(F("Adjust the frequency of the tune using the potentiometer."));
        print_lcd(F("Delete notes using the DEL/CANCEL button."));
        print_lcd(F("Save the song by pressing OPTION+SELECT button."));
        print_lcd(F("Delete the current song (exit) by pressing OPTION+DEL."));
        print_lcd(F("Play current track by pressing OPTION twice."));
    }
    break;
    case LM_MENU:
    {
        lcd.setCursor(2, 1);
        lcd.print(F("[Listening Mode]"));
        lcd.setCursor(0, 2);
        lcd.print(F(">> AWAITING INPUT <<"));
    }
    break;
    case LM_PLAYING_SONG:
        break;
    case CM_CREATE_NEW:
        break;
    default:
        Serial.println(F("STATE ERROR."));
    }
}

