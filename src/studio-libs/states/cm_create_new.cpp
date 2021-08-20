/**
 * @file cm_create_new.cpp
 * @author Jacob LuVisi
 * @brief The class for creating a new song. All controls on how this state is used are found on the GitHub as well as described
 * in the program instructions.
 * @version 0.1
 * @date 2021-07-26
 *
 * @copyright Copyright (c) 2021
 *
 */

#include <studio-libs/states/states.h>
#if PERF_METRICS == true
#include <debug/debug.h>
#endif

CreatorModeCreateNew::CreatorModeCreateNew() : ProgramState::ProgramState(CM_CREATE_NEW) {}
CreatorModeCreateNew::~CreatorModeCreateNew() {}
void CreatorModeCreateNew::loop() {

    // Go through all of the tune buttons and check if they are being pressed.
    if (is_pressed(BTN_TONE_1)) {
        // Play the song back.
        if (optionWaiting) {
            delay_ms(150);
            newSong->play_song();
            delay_ms(150);
            optionWaiting = false;
            return;
        }
        playSound = true;
        lastButtonPress = BTN_TONE_1;
    }
    else if (is_pressed(BTN_TONE_2)) {
        if (!optionWaiting) {
            playSound = true;
        }

        lastButtonPress = BTN_TONE_2;
    }
    else if (is_pressed(BTN_TONE_3)) {
        playSound = true;
        lastButtonPress = BTN_TONE_3;
    }
    else if (is_pressed(BTN_TONE_4)) {
        playSound = true;
        lastButtonPress = BTN_TONE_4;
    }
    else if (is_pressed(BTN_TONE_5)) {
        playSound = true;
        lastButtonPress = BTN_TONE_5;
    }

    // If the option button has been pressed then light the LED orange.
    if (is_pressed(BTN_OPTION)) {
        optionWaiting = !optionWaiting;
        // Scroll the LCD.
        if (!optionWaiting) {
            scrolledLines += 1;
            if (scrolledLines > get_lcd_required_rows()) {
                scrolledLines = 0;
            }
            this->print_song_lcd();
        }
    }
    // Create a note from the last tune button which was pressed.
    const note_t currentNote = optionWaiting && lastButtonPress == BTN_TONE_2 ? PAUSE_NOTE : get_current_tone(lastButtonPress);
    // If the previous note was the pause note then disable the option waiting as it has been used.

    segDisplay.refreshDisplay();

    if (millis() - previousUpdate > 100) {
        previousUpdate = millis();
        segDisplay.setChars(currentNote.pitch);
        segDisplay.refreshDisplay();
        if (optionWaiting) {
            analogWrite(RGB_BLUE, RGB_BRIGHTNESS);
        }
        else {
            analogWrite(RGB_BLUE, 0);
        }
    }
    // Add a tune if the button to add/select is pressed.
    if (is_pressed(BTN_ADD_SELECT)) {
        if (currentNote.frequency == PAUSE_NOTE.frequency) {
            optionWaiting = false;
        }
        if (optionWaiting) {
            optionWaiting = false;
            // SAVE SONG.
            if (newSong->get_size() < MIN_SONG_LENGTH) {
                lcd.clear();
                lcd.setCursor(0, 1);
                lcd.print(F("[ERROR]"));
                print_scrolling(F("Please make your song at least eight or more notes to save."), 2, 150);
                delay_ms(500);
                print_song_lcd();
                return;
            }

            // Update the fileName variable.
            set_save_name();

#if DEBUG == true
            Serial.print(get_active_time());
            Serial.print(F(" Saving Song file: "));
            Serial.println(fileName);
#endif
            // Exit the program.
            if (fileName[0] == '\0') {
                print_song_lcd();
                return;
            }

            // Create a buffer to store both the file name as well as the new file extension.
            char buffer[strlen(fileName) + strlen(FILE_TXT_EXTENSION)];
            // Copy the file name to the buffer.
            strncpy(buffer, fileName, strlen(fileName));
            // Copy the file extension to the buffer starting at the next memory address past file name + 1 because the ending null character.
            strncpy(buffer + strlen(fileName), FILE_TXT_EXTENSION, strlen(FILE_TXT_EXTENSION) + 1);
            // Save the song.
            sd_save_song(buffer, newSong);
#if DEBUG == true
            Serial.print(get_active_time());
            Serial.println(F(" Saved song from creator mode."));
#endif
            lcd.clear();
            lcd.setCursor(0, 1);
            lcd.print(F("Song Saved."));
            lcd.setCursor(0, 2);
            lcd.print(F("Returning to Song."));
            delay_ms(1500);
            print_song_lcd();
            return;
        }
        newSong->add_note(currentNote.frequency);
        this->print_song_lcd();
#if DEBUG == true
        newSong->play_song();
#endif
    }
    else if (is_pressed(BTN_DEL_CANCEL)) {
        // Exit the state.
        if (optionWaiting) {
            analogWrite(RGB_BLUE, 0);
            delete newSong;
            update_state(MAIN_MENU);
            return;
        }
        // If the song still has notes remaining.
        if (newSong->get_size() != 0)
        {
            newSong->remove_note();
            this->print_song_lcd();
        }

#if DEBUG == true
        Serial.print(get_active_time());
        Serial.println(F(" Playing Song."));
        newSong->play_song();
#endif

    }

    if (playSound) {
        newSong->play_note(currentNote.frequency);
        delay_ms(200);
        noTone(SPEAKER_1);
        // Eliminates static noise
        pinModeFast(SPEAKER_1, INPUT);
        playSound = false;

#if PERF_METRICS == true
        Serial.print(get_active_time());
        Serial.print(F(" Free Memory: "));
        Serial.println(freeMemory());
#endif

    }
}

void CreatorModeCreateNew::init() {
    lcd.print(F("[SONG]"));
    segDisplay.setBrightness(-100);

    newSong = new Song(SPEAKER_1, DEFAULT_NOTE_LENGTH, DEFAULT_NOTE_DELAY);

    previousUpdate = 0;
    lastButtonPress = 0;
    scrolledLines = 0;
    optionWaiting = false;
    playSound = false;
    // Eliminates static noise
    pinModeFast(SPEAKER_1, INPUT);
    return;
}

/**
 * @brief Prints the current song data onto the LCD display.
 *
 */
void CreatorModeCreateNew::print_song_lcd() {
    const song_size_t songSize = newSong->get_size();

    // Setup the top row of the display.

/*
// Alternative "clean" way of doing it. (Takes more SRAM and Flash)
    char buffer[20];
    lcd.clear();
    sprintf(buffer, "[SONG] ([%d] %d/%d)", scrolledLines+1, songSize, MAX_SONG_LENGTH);
*/

    lcd.clear();
    char buffer[3 + sizeof(char)];
    lcd.print(F("[SONG] (["));
    itoa(scrolledLines + 1, buffer, 10);
    lcd.print(buffer);
    lcd.print(F("] "));
    itoa(songSize, buffer, 10);
    lcd.print(buffer);
    lcd.print(F("/"));
    itoa(MAX_SONG_LENGTH, buffer, 10);
    lcd.print(buffer);
    lcd.print(F(")"));

    uint8_t lcdCursor = 1;
    uint8_t columnCount = 0;

    // Print each of the notes from the song onto the LCD.
    uint8_t scrolledLineCounter = scrolledLines;
    for (song_size_t i = 0; i < songSize; i++) {
        const char* pitch = get_note_from_freq(newSong->get_note(i)).pitch;
        const uint8_t pitchSize = strlen(pitch);

        // Get if adding the pitch to the current column would overflow. If so then reset the column.
        if (columnCount + pitchSize > LCD_COLS) {
            columnCount = 0;
            if (scrolledLineCounter == 0) {
                ++lcdCursor;
            }
            else {
                --scrolledLineCounter;
            }
        }
        // -1 for the top [Song] header.
        // Stop printing if we are off the screen.
        if (lcdCursor > (LCD_ROWS - 1)) {
            return;
        }


        if (scrolledLineCounter == 0) {
            lcd.setCursor(columnCount, lcdCursor);
            lcd.print(pitch);
        }

        columnCount += pitchSize;
    }
}

/**
 * @brief Get the number of rows the LCD can have.
 *
 * @return A number of rows.
 */
uint8_t CreatorModeCreateNew::get_lcd_required_rows() {
    uint8_t columnCount = 0;
    uint8_t rowCounter = 0;
    for (song_size_t i = 0; i < newSong->get_size(); i++) {
        const uint8_t pitchSize = strlen(get_note_from_freq(newSong->get_note(i)).pitch);
        if (columnCount + pitchSize > LCD_COLS) {
            columnCount = 0;
            rowCounter++;
        }
        columnCount += pitchSize;
    }
    return rowCounter;
    
}


// NOTE: Methods like the ones below would not be included on ports to the UNO because the UNO does not
// support microSD. Therefore we can be more liberal about how we use our SRAM.

/**
 * @brief Sets the name of which the song should be saved into permanent storage.
 * Modifies the "fileName" global variable to hold the new value.
 *
 */
void CreatorModeCreateNew::set_save_name() {

    // Keep track of the index to add to name.
    uint8_t index = 0;
    // The name the user is choosing.
    char name[9];

    lcd.clear();
    lcd.print(F("[Saving Song]"));
    lcd.setCursor(1, 1);
    lcd.print(F("~ Name The Song"));
    // The display shows some information on the bottom row.
    // This variable tracks which piece of text to show.
    uint8_t slideInfo = 0;
    unsigned long lastUpdate = 0;
    const uint16_t updateInterval = 5000; // 5 seconds.
    segDisplay.blank();


    char analogChar = get_character_from_analog();
    while (true) {

        // same as millis() % 32 == 0
        if (millis() & ((2 ^ 5) - 1)) {
            analogChar = get_character_from_analog();
            segDisplay.refreshDisplay();
            if (optionWaiting) {
                analogWrite(RGB_BLUE, RGB_BRIGHTNESS);
            }
            else {
                analogWrite(RGB_BLUE, 0);
            }
        }

        lcd.setCursor(0, 2);
        lcd.print(F("Name > "));


        // Go through the current name array and print it out.
        for (int i = 0; i < index; i++) {
            lcd.print(name[i]);
        }
        // If the user string is not at its maximum characters
        if (index != 8) {
            lcd.print(analogChar);
        }

        lcd.print(F(".txt "));

        // Toggle option button.
        if (is_pressed(BTN_OPTION)) {
            optionWaiting = !optionWaiting;
        }

        if (is_pressed(BTN_ADD_SELECT)) {
            // Return an array of each character.
            if (optionWaiting) {
                if (index == 0) {
                    lcd_clear_row(2);
                    delay_ms(500);
                    lcd.print(F("PLEASE SET A NAME."));
                    delay_ms(2500);
                    lcd_clear_row(2);
                    delay_ms(500);
                    continue;
                }

                // Terminate the array and copy it to the fileName global var.
                name[index] = '\0';
                strcpy(fileName, name);
                return;
            }
            // If we are at 8 characters then don't update the name.
            if (index == 8) {
                continue;
            }
            name[index] = analogChar;
            index++;
        }

        if (is_pressed(BTN_DEL_CANCEL)) {
            if (optionWaiting) {
                // Return an empty string.
                strcpy(fileName, "\0");
                optionWaiting = false;
                return;
            }
            if (index == 0) {
                continue;
            }
            // Remove a character and go back.
            name[index] = ' ';
            index--;
        }


        // Show information text on bottom row.
        if (millis() - lastUpdate > updateInterval) {
            lcd_clear_row(3);
            switch (slideInfo) {
            case 0:
                lcd.print(F("SELECT: Add Symbol."));
                break;
            case 1:
                lcd.print(F("DEL: Remove Symbol."));
                break;
            case 2:
                lcd.print(F("OPTION+SEL: Save."));
                break;
            case 3:
                lcd.print(F("OPTION+DEL: Exit."));
                break;
            case 4:
                lcd.print(F("8 Characters Limit"));
                break;
            }
            lastUpdate = millis();
            if (slideInfo == 4) {
                slideInfo = 0;
                return;
            }
            slideInfo++;
        }

    }
}

/**
 * @brief Converts the analog read signal from the potentiometer into one character A-Z, 0-9 and underscore.
 *
 * @return A character matching the analog signal.
 */
const char CreatorModeCreateNew::get_character_from_analog() {
    // An array of every possible character that can be included in the name.
    // Note that because Arduino SD uses file format 8.3 every character will be a capital letter (no lowercase)
    // Characters are A-Z, 0-9, and underscores.
    // 26 letters + 10 numbers
    // The array containing all of the possible values is stored in tune_studio.h in PROGMEM.
    return pgm_read_byte_near(&OPTIONAL_NAMING_CHARACTERS[((get_current_freq() + 1) / (uint8_t)28)]);
}