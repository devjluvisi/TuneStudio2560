/**
 * @file lm_playing_song.cpp
 * @author Jacob LuVisi
 * @brief The class for managing the current song being played. This state also uses the bottom row of the LCD to display different
 * controls on a varying interval.
 *
 * Note: The way that songs are played back is different then usual. This class does NOT use the song.cpp way of playing back songs
 * and instead it relies on a global index and the general loop() function instead of a seperate while loop.
 *
 * @version 0.1
 * @date 2021-07-26
 *
 * @copyright Copyright (c) 2021
 *
 */

#include <studio-libs/states/states.h>

ListeningModePlayingSong::ListeningModePlayingSong() : ProgramState::ProgramState(LM_PLAYING_SONG) {}
ListeningModePlayingSong::~ListeningModePlayingSong() {
    delete currentSong;
}

void ListeningModePlayingSong::loop() {

#if PERF_METRICS == true
    // FOR DEBUG USE ONLY
    unsigned long start = micros(); // Measure the starting time of the loop.
#endif

    // Update the bottom row of the LCD to display different controls.
    // Timed on an interval.
    if (millis() - lastTextUpdate > bottomTextDelayInterval) {
        lcd_clear_row(3);
        switch (bottomTextMode) {
        case 0:
            lcd.print(F("DEL/CANCEL: Exit"));
            break;
        case 1:
            lcd.print(F("SELECT: Pause Song"));
            break;
        case 2:
            lcd.print(F("PAUSE+GREEN: Rewind"));
            break;
        case 3:
            lcd.print(F("PAUSE+BLUE: Forward"));
            break;
        case 4:
            lcd.print(F("HOLD OP+DEL: Delete"));
            break;
        case 5:
            lcd.print(F("SELECT: Replay"));
            break;
        }
        // Move to the next text mode.
        bottomTextMode++;
        if (bottomTextMode == 6) {
            bottomTextMode = 0;
        }
        lastTextUpdate = millis();
    }


    // When the user presses the add/select button.
    if (digitalRead(BTN_ADD_SELECT) == LOW) {
        // If the song is being played then pause it.
        if (currentSongNote < currentSongSize) {
            isPaused = !isPaused;
            // Update the paused indicator.
            lcd_clear_row(1);
            lcd.setCursor(1, 1);
            if (isPaused) {
                lcd.write(byte(PAUSE_SONG_SYMBOL));
                lcd.print(F(" NOW PAUSED "));
            }
            else {
                lcd.write(byte(PLAYING_SONG_SYMBOL));
                lcd.print(F(" NOW PLAYING "));
            }
            lcd.write(byte(MUSIC_NOTE_SYMBOL));
            delay(200);
        }
        else {
            // If the song has finished then restart it.
            delay(500);
            delete currentSong;
            lcd.clear();
            lcd.setCursor(0, 1);
            lcd.print(F("Restarting!"));
            delay(1200);
            lcd.clear();
            init();
        }
    }

    // Forward/Backwards
    if (isPaused && currentSongNote < currentSongSize) {
        if (digitalRead(BTN_TONE_1) == LOW) {
            // Forward
            currentSongNote += 1;

            delay(100);
        }
        else if (digitalRead(BTN_TONE_2) == LOW) {
            // Backwards
            if (currentSongNote == 0) {
                return;
            }
            currentSongNote -= 1;
            delay(100);
            // Reset the PROGRESS block. (+2 because space)
            // NOTE: The progress bar needs to be reset because the instructions to update the progress bar usually do not 
            // account for a reduction in the block size. Therefore we need to regenerate the block size from zero.
            lcd_clear_row(2);
            lcd.setCursor(strlen(PROGRESS_LABEL) + 1, 2);
            blockSize = blockRequirement;
            for (uint8_t i = 0; i < 8; i++) {
                if (currentSongNote >= blockSize) {
                    // Set the new requirement.
                    blockSize += blockRequirement;
                    // Add a block to the progress.
                    lcd.write(byte(PROGRESS_BLOCK_SYMBOL)); 
                }
            }
        }
    }
    // End

    // Deleting the song
    if (digitalRead(BTN_DEL_CANCEL) == LOW && digitalRead(BTN_OPTION) == LOW) {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(F("DELETING:"));
        lcd.setCursor(0, 1);
        lcd.print(F(">> "));
        lcd.print(sd_get_file(get_selected_song() - 1));
        lcd.setCursor(0, 2);
        lcd.print(F("ARE YOU SURE? (Y/N)"));
        lcd.setCursor(0, 3);
        lcd.print(F("SELECT=Y, CANCEL=N"));
        delay(1000);

        while (true) {
            if (digitalRead(BTN_ADD_SELECT) == LOW) {
                // Remove the file from SD.
                sd_rem(sd_get_file(get_selected_song() - 1));
                lcd.clear();
                lcd.setCursor(0, 1);
                lcd.print(F("Deleted."));
                delay(2000);
                delete currentSong;
                update_state(MAIN_MENU);
                delay(500);

                return;
            }
            if (digitalRead(BTN_DEL_CANCEL) == LOW) {
                delay(850);
                break;
            }
        }
        delete currentSong;
        lcd.clear();
        delay(850);

        init();
    }
    // Return to main menu.
    else if (digitalRead(BTN_DEL_CANCEL) == LOW) {
        delay(1000);
        update_state(MAIN_MENU);
        return;
    }
    // End

    /*
    [NOTE]
    This class uses a custom song playing method in order to play the individual notes from the song.
    This is required in order to make the pause, rewind/forward, and progress bar work properly.

    The playing of songs in this class relies entirely on the looping method of this class.
    Therefore, the performance of song playback is dependent on the performance of the code in this looping function.
    Code in this function must be optimized to a high level or else extra delays between notes can occur.

    Using DEBUG and PERF_METRIC modes can cause the notes to be delayed for a longer period of time.
    */
    if (!isPaused && currentSongNote < currentSongSize) {
        if (currentSong->get_note(currentSongNote) == PAUSE_NOTE.frequency) {
            delay(PAUSE_DELAY); // Delay the song from continuing for a certain amount of time.
            currentSongNote++; // Go to the next index of the song.
        }
        else {
            currentSong->play_note(currentSong->get_note(currentSongNote));
            delay(50);
            noTone(SPEAKER_1);
            delay(80);
            currentSongNote++;
        }

    }
    else if (currentSongNote >= currentSongSize) {
        lcd.setCursor(1, 1);
        lcd.write(byte(FINISHED_SONG_SYMBOL));
        lcd.print(F(" FINISHED SONG "));
        lcd.write(byte(MUSIC_NOTE_SYMBOL));
    }

    // Set the cursor to a point on the LCD where the next block is to be inserted.
    lcd.setCursor(strlen(PROGRESS_LABEL) + (blockSize / blockRequirement), 2);
    // If the current song note is past the requirement for the next block.
    if (currentSongNote >= blockSize) {
        // Set the new requirement.
        blockSize += blockRequirement;
        // Add a block to the progress.
        lcd.write(byte(PROGRESS_BLOCK_SYMBOL));
    }

#if PERF_METRICS == true
    unsigned long end = micros();
    unsigned long delta = end - start;
    Serial.print(F("In Microseconds: "));
    Serial.println(delta);
    Serial.print(F("In Milliseconds: "));
    Serial.println(((long)(delta / 1000)));
#endif
    return;
}

void ListeningModePlayingSong::init() {
    currentSongNote = 0;
    bottomTextMode = 0;
    lastTextUpdate = 0;
    isPaused = false;
    confirmDelete = false;
    const char* name = sd_get_file(get_selected_song() - 1);
    currentSong = new Song(SPEAKER_1, 50, 80, MAX_SONG_LENGTH, false);

    lcd.print(F("[#"));
    lcd.print(get_selected_song());
    lcd.print(F("] "));
    char buffer[9];
    // Copy the song name to the buffer and remove the file extension.
    strncpy(buffer, name, strlen(name) - 4);
    buffer[strlen(name) - 4] = '\0'; // Terminate "garbage" unneeded data.
    lcd.print(buffer);

    lcd.setCursor(1, 1);
    lcd.write('#');
    lcd.print(F(" INITALIZING "));
    lcd.write(byte(MUSIC_NOTE_SYMBOL));

    lcd.setCursor(0, 2);
    lcd.print(F("PROGRESS: "));
    sd_songcpy(currentSong, name);

    currentSongSize = currentSong->get_size();
    // Seperate the progress bar into 8 different blocks.
    blockRequirement = currentSongSize / 8;
    blockSize = blockRequirement;

    delay(750);

    lcd.setCursor(1, 1);
    lcd.write(byte(PLAYING_SONG_SYMBOL));
    lcd.print(F(" NOW PLAYING "));
    lcd.write(byte(MUSIC_NOTE_SYMBOL));
}