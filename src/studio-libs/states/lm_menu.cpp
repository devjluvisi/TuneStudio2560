/**
 * @file lm_menu.cpp
 * @author Jacob LuVisi
 * @brief The class for managing the menu in listening mode.
 * @version 0.1
 * @date 2021-07-26
 *
 * @copyright Copyright (c) 2021
 *
 */

#include <studio-libs/states/states.h>

ListeningModeMenu::ListeningModeMenu() : ProgramState::ProgramState(LM_MENU) {}
ListeningModeMenu::~ListeningModeMenu() {}

void ListeningModeMenu::loop() {

    lcd.setCursor(2, 0);
    lcd.print(F("[Listening Mode]"));
    // If the SD card is enabled then also print the name of the song.
    lcd.setCursor(0, 1);

    const char* name = sd_get_file(get_selected_song() - 1);
    if (previousSong != get_selected_song()) {
        lcd_clear_row(1);
        lcd.print(F(">> Name: "));
        //const char* name = "LOL";
        if (strlen(name) != 0) {
            lcd.setCursor(8, 1);
            lcd.print(name);
        }
        else {
            lcd.setCursor(8, 1);
            lcd.print(F("NONE"));
        }
        lcd_clear_row(2);
        lcd.print(F(">> Song #"));
        lcd.print(get_selected_song());
        lcd.print(F("/("));
        lcd.print(get_selected_page());
        lcd.print(F(")"));

        previousSong = get_selected_song();
    }

    if (is_pressed(BTN_TONE_1)) {
        set_selected_song(((get_selected_page() - 1) * 5) + 1);
        // Page 1 = (1-1)*5+1 = 0*5+1= Song #1
        // Page 2 = (2-1)*5+1 = 1*5+1= Song #6
        // Page 3 = (3-1)*5+1 = 2*5+1= Song #11
        // etc.
        return;
    }
    else
        if (is_pressed(BTN_TONE_2)) {
            set_selected_song(((get_selected_page() - 1) * 5) + 2);
            // Page 1 = (1-1)*5+2 = 0*5+2= Song #2
            // Page 2 = (2-1)*5+2 = 1*5+2= Song #7
            // Page 3 = (3-1)*5+2 = 2*5+2= Song #12
            // etc.
            return;
        }
        else
            if (is_pressed(BTN_TONE_3)) {
                set_selected_song(((get_selected_page() - 1) * 5) + 3);
                // Page 1 = (1-1)*5+3 = 0*5+3= Song #3
                // Page 2 = (2-1)*5+3 = 1*5+3= Song #8
                // Page 3 = (3-1)*5+3 = 2*5+3= Song #13
                // etc.
                return;
            }
            else
                if (is_pressed(BTN_TONE_4)) {
                    set_selected_song(((get_selected_page() - 1) * 5) + 4);
                    // Page 1 = (1-1)*5+4 = 0*5+4= Song #4
                    // Page 2 = (2-1)*5+4 = 1*5+4= Song #9
                    // Page 3 = (3-1)*5+4 = 2*5+4= Song #14
                    // etc.
                    return;
                }
                else
                    if (is_pressed(BTN_TONE_5)) {
                        set_selected_song(((get_selected_page() - 1) * 5) + 5);
                        // Page 1 = (1-1)*5+5 = 0*5+5= Song #5
                        // Page 2 = (2-1)*5+5 = 1*5+5= Song #10
                        // Page 3 = (3-1)*5+5 = 2*5+5= Song #15
                        // etc.
                        return;
                    }
                    else
                        if (is_pressed(BTN_OPTION)) {
                            // Go to the next page and set the song to the first led.
                            set_selected_page(get_selected_page() + 1 > (MAX_SONG_AMOUNT / 5) ? 0 : get_selected_page() + 1);
                            set_selected_song(((get_selected_page() - 1) * 5) + 1);
                            previousSong = -1;
                            return;
                        }
}

void ListeningModeMenu::init() {
#if DEBUG == true
    Serial.print(get_active_time());
    Serial.println(F(" Entered listening mode."));
#endif
    previousSong = 0;
    delay_ms(500);
    lcd.setCursor(2, 1);
    lcd.print(F("[Listening Mode]"));
    lcd.setCursor(1, 2);
    lcd.print(F(">> INSTRUCTIONS <<"));
    delay_ms(1250);
    lcd.clear();
    print_lcd(F("Press select button to skip instructions."), 50);
    delay_ms(500);
    print_lcd(F("Select 1 of the 5 tune buttons to play a song saved in memory."));
    print_lcd(F("When using microSD, press the \"OPTION\" button to cycle to the next page of songs. Each page is 5 different songs."));
    print_lcd(F("Press the \"DEL/CANCEL\" button to go back to main menu."));
    print_lcd(F("While listening, press \"SELECT\" to pause song."));
    print_lcd(F("While paused, press Green Tone to go back, Blue Tone to go forward, and SELECT to restart after a song is finished."));
    print_lcd(F("While listening, press \"OPTION+DEL\" to delete song."));
    //TODO: Possibly add instruction for OPTION+SELECT to edit a saved song.
    delay_ms(1500);
    lcd.setCursor(0, 3);
    lcd.print(F("Press SELECT to play"));
}