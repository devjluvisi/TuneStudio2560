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
}

PossibleStates CurrentState::get_state() {
    return _currentState;
}

void CurrentState::init() {
    lcd.clear(); // Remove all information from the previous state.
    segDisplay.blank(); // Clear the 7 segment display.
    segDisplay.refreshDisplay(); // Refresh it to clear.

    switch (_currentState) {
    case MAIN_MENU:
        break; // Break immediatly because there is no initalization we have to do for the main menu.
    case CM_MENU:
        break;
    case LM_MENU:
        break;
    case LM_PLAYING_SONG:
        break;
    case CM_CREATE_NEW:
        break;
    default:
        Serial.println(F("STATE ERROR."));
    }

}

void CurrentState::load() {
    if (!_hasInitalized) {
        init();
    }

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
        //print_large_text("To enter creator mode press the select button. To enter listening mode press the delete button. To view more information please check out my github.");
        char* messages[] = {
            "Hello I am currently testing the scrolling functionality!",
            "is it working?"
        };
        print_scrolling_text(2, messages);
        // Information message.
        delay(2000);
        lcd.clear(); // Clear the screen incase the method needs to run again.
        delay(1000);
    }
    break;
    case CM_MENU:
        break;
    case LM_MENU:
        break;
    case LM_PLAYING_SONG:
        break;
    case CM_CREATE_NEW:
        break;
    default:
        Serial.println(F("STATE ERROR."));
    }
}

