/**
 * @file lm_playing_song.cpp
 * @author Jacob LuVisi
 * @brief The class for managing the current song being played.
 * @version 0.1
 * @date 2021-07-26
 *
 * @copyright Copyright (c) 2021
 *
 */

#include <studio-libs/states/states.h>

ListeningModePlayingSong::ListeningModePlayingSong() : ProgramState::ProgramState(LM_PLAYING_SONG), sdCard(SD_CS_PIN) {}
ListeningModePlayingSong::~ListeningModePlayingSong() {}

void ListeningModePlayingSong::loop() {
    return;
}

void ListeningModePlayingSong::init() {
    lcd.print(F(">> Song #"));
    sdCard.print_info();
    lcd.print(get_selected_song());
    lcd.setCursor(0, 1);
    lcd.write(byte(PLAYING_SONG_SYMBOL));
    lcd.print(F(" NOW PLAYING "));
    lcd.write(byte(MUSIC_NOTE_SYMBOL));
    lcd.setCursor(0, 2);
    lcd.print(F("PROGRESS: "));
    for (uint8_t i = 0; i < 5; i++) {
        lcd.write(byte(PROGRESS_BLOCK_SYMBOL));
    }
    delay(500);
}