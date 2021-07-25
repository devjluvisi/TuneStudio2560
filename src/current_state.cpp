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

bool CurrentState::has_initalized() {
    return _hasInitalized;
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
    {
        Serial.println(F("Entered listening mode."));
        delay(500);
        lcd.setCursor(2, 1);
        lcd.print(F("[Listening Mode]"));
        lcd.setCursor(1, 2);
        lcd.print(F(">> INSTRUCTIONS <<"));
        delay(1250);
        lcd.clear();
        print_lcd(F("Press select button to skip instructions."), 50);
        delay(500);
        print_lcd(F("Select 1 of the 5 tune buttons to play a song saved in memory."));
        print_lcd(F("When using microSD, press the \"OPTION\" button to cycle to the next page of songs. Each page is 5 different songs."));
        print_lcd(F("Press the \"DEL/CANCEL\" button to go back to main menu."));
        print_lcd(F("While listening, press \"SELECT\" to pause song."));
        print_lcd(F("While listening, press \"OPTION+DEL\" to delete song."));
        delay(1500);
        break;
    }
    case LM_PLAYING_SONG:
    {
        lcd.print(F(">> Song #"));
        lcd.print(String(get_selected_song()));
        lcd.setCursor(0, 1);
        lcd.write(byte(PLAYING_SONG_SYMBOL));
        lcd.print(F(" NOW PLAYING "));
        lcd.write(byte(MUSIC_NOTE_SYMBOL));
        lcd.setCursor(0, 2);
        lcd.print(F("PROGRESS: "));
        for (uint8_t i = 0; i < 5; i++) {
            lcd.write(byte(PROGRESS_BLOCK_SYMBOL));
        }
        delay(5000);
        break;
    }
    // Player has selected a song.
    case CM_CREATE_NEW:
    {
        break;
    }

    default:
    {
        Serial.println(F("STATE ERROR."));
        return;
    }

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
        return;
    }
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
        lcd.clear();
        delay(500);
        lcd.setCursor(3, 1);
        lcd.print(F("[Creator Mode]"));
        lcd.setCursor(5, 2);
        lcd.print(F(">> INFO <<"));
        delay(1250);
        print_lcd(F("Each tune that is added will have a corresponding LETTER and NUMBER. TuneStudio2560 utilizes the standardized chromatic scale for notes."));
        print_lcd(F("Each tune button represents a frequency between 31-3951. A tune button along with the potentiometer create a note."));
        print_lcd(F("The type of note is displayed on the segment display. (Ex. GS6, A4, DS4)"));
        print_lcd(F("The individual tune buttons do not correspond with a letter or tone from the chromatic scale, just a frequency."));
        delay(500);
        lcd.setCursor(0, 0);
        lcd.print(F("The possible"));
        lcd.setCursor(0, 1);
        lcd.print(F("frequency ranges:"));
        delay(500);
        print_scrolling(F("GREEN: B0 (31hz) to DS2 (78hz), BLUE: E2 (82hz) to GS3 (208hz), RED: A3 (220hz) to CS5 (554hz), YELLOW: D5 (587hz) to FS6 (1480hz), WHITE: G6 (1568hz) to B7 (3951hz)"), 2);
        delay(1250);
        lcd.clear();
        delay(500);
        return;
    }
    case LM_MENU:
    {
        lcd.setCursor(2, 0);
        lcd.print(F("[Listening Mode]"));
        // If the SD card is enabled then also print the name of the song.
        if (sd_enabled()) {
            lcd.setCursor(0, 1);
            lcd.print(F(">> Name: "));
            lcd.print(F("NULL    "));
        }
        lcd.setCursor(0, 2);
        lcd.print(F(">> Song #"));
        lcd.print(String(get_selected_song()));
        lcd.print(F("  ")); // Add two extra spaces in order to prevent overlapping.
        lcd.setCursor(0, 3);
        lcd.print(F("Press SELECT to play"));

        if (is_pressed(BTN_TONE_1)) {
            set_selected_song(((get_selected_page() - 1) * 5) + 1);
            // Page 1 = (1-1)*5+1 = 0*5+1= Song #1
            // Page 2 = (2-1)*5+1 = 1*5+1= Song #6
            // Page 3 = (3-1)*5+1 = 2*5+1= Song #11
            // etc.
            return;
        }
        if (is_pressed(BTN_TONE_2)) {
            set_selected_song(((get_selected_page() - 1) * 5) + 2);
            // Page 1 = (1-1)*5+2 = 0*5+2= Song #2
            // Page 2 = (2-1)*5+2 = 1*5+2= Song #7
            // Page 3 = (3-1)*5+2 = 2*5+2= Song #12
            // etc.
            return;
        }
        if (is_pressed(BTN_TONE_3)) {
            set_selected_song(((get_selected_page() - 1) * 5) + 3);
            // Page 1 = (1-1)*5+3 = 0*5+3= Song #3
            // Page 2 = (2-1)*5+3 = 1*5+3= Song #8
            // Page 3 = (3-1)*5+3 = 2*5+3= Song #13
            // etc.
            return;
        }
        if (is_pressed(BTN_TONE_4)) {
            set_selected_song(((get_selected_page() - 1) * 5) + 4);
            // Page 1 = (1-1)*5+4 = 0*5+4= Song #4
            // Page 2 = (2-1)*5+4 = 1*5+4= Song #9
            // Page 3 = (3-1)*5+4 = 2*5+4= Song #14
            // etc.
            return;
        }
        if (is_pressed(BTN_TONE_5)) {
            set_selected_song(((get_selected_page() - 1) * 5) + 5);
            // Page 1 = (1-1)*5+5 = 0*5+5= Song #5
            // Page 2 = (2-1)*5+5 = 1*5+5= Song #10
            // Page 3 = (3-1)*5+5 = 2*5+5= Song #15
            // etc.
            return;
        }
        if (is_pressed(BTN_OPTION)) {
            set_selected_page(get_selected_page() + 1 > (MAX_SONG_AMOUNT / 5) ? 0 : get_selected_page() + 1);
            return;
        }
        return;
    }
    case LM_PLAYING_SONG:
    {
        return;
    }
    case CM_CREATE_NEW:
    {
        Song newSong(SPEAKER_1, 20, 40, false);
        // SPLIT INTO SEPERATE METHOD.
        //Song newSong(SPEAKER_1, 20, 40, false);
        lcd.setCursor(0, 0);
        lcd.print(F("[SONG]"));
        // Track some variables for adding/removing
        uint16_t previousTune; // A tune button that was last pressed.

        // Create an infinite loop while creating a song.
        while (true) {
            if (is_pressed(BTN_TONE_1)) {
                Serial.println(String(get_current_tone(BTN_TONE_1).pitch) + " / " + String(get_current_tone(BTN_TONE_1).frequency));
            }
            else if (is_pressed(BTN_TONE_2)) {
                Serial.println(String(get_current_tone(BTN_TONE_2).pitch) + " / " + String(get_current_tone(BTN_TONE_2).frequency));
            }
            else if (is_pressed(BTN_TONE_3)) {
                Serial.println(String(get_current_tone(BTN_TONE_3).pitch) + " / " + String(get_current_tone(BTN_TONE_3).frequency));
            }
            else if (is_pressed(BTN_TONE_4)) {
                Serial.println(String(get_current_tone(BTN_TONE_4).pitch) + " / " + String(get_current_tone(BTN_TONE_4).frequency));
            }
            else if (is_pressed(BTN_TONE_5)) {
                Serial.println(String(get_current_tone(BTN_TONE_5).pitch) + " / " + String(get_current_tone(BTN_TONE_5).frequency));
            }

            delay(500);
        }
        return;

    }
    default:
    {
        Serial.println(F("STATE ERROR."));
        return;
    }

    }
}