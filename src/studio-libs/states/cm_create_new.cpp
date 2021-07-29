/**
 * @file cm_create_new.cpp
 * @author Jacob LuVisi
 * @brief The class for creating a new song.
 * @version 0.1
 * @date 2021-07-26
 *
 * @copyright Copyright (c) 2021
 *
 */

#include <studio-libs/states/states.h>

CreatorModeCreateNew::CreatorModeCreateNew() : ProgramState::ProgramState(CM_CREATE_NEW) {}
CreatorModeCreateNew::~CreatorModeCreateNew() {}
void CreatorModeCreateNew::loop() {
    // Go through all of the tune buttons and check if they are being pressed.
    if (is_pressed(BTN_TONE_1)) {
        // Play the song back.
        if (optionWaiting) {
            delay(150);
            newSong->play_song();
            delay(150);
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
    const note currentNote = optionWaiting && lastButtonPress == BTN_TONE_2 ? PAUSE_NOTE : get_current_tone(lastButtonPress);
    // If the previous note was the pause note then disable the option waiting as it has been used.

    segDisplay.refreshDisplay();

    if (millis() - previousUpdate > 100) {
        previousUpdate = millis();
        segDisplay.setChars(currentNote.pitch);
        segDisplay.refreshDisplay();
        if (optionWaiting) {
            analogWrite(RGB_BLUE, 35);
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
        newSong->remove_note();
        this->print_song_lcd();
#if DEBUG == true
        newSong->play_song();
#endif

    }

    if (playSound) {
        pinMode(SPEAKER_1, OUTPUT);
        newSong->play_note(currentNote.frequency);
        delay(200);
        playSound = false;
        pinMode(SPEAKER_1, INPUT);
#if DEBUG == true
        Serial.println(freeMemory());
#endif

    }
}

void CreatorModeCreateNew::init() {
    pinMode(SPEAKER_1, INPUT);

    lcd.setCursor(0, 0);
    lcd.print(F("[SONG]"));

    segDisplay.setBrightness(-100);
    lcd.setCursor(0, 1);

    newSong = new Song(SPEAKER_1, 50, 80, MAX_SONG_LENGTH, false);

    previousUpdate = 0;
    lastButtonPress = 0;
    scrolledLines = 0;
    optionWaiting = false;
    playSound = false;
    return;
}

void CreatorModeCreateNew::print_song_lcd() {
    lcd.clear();
    //lcd.print(F("[SONG]"));
    lcd.print("[SONG] ([" + String(scrolledLines + 1) + "] " + String(newSong->get_size()) + "/" + String(MAX_SONG_LENGTH) + ")");

    uint8_t lcdCursor = 1;
    uint8_t columnCount = 0;

    uint8_t scrolledLineCounter = scrolledLines;
    for (uint32_t i = 0; i < newSong->get_size(); i++) {

        const char* pitch = get_note_from_freq(newSong->get_note(i)).pitch;

        if (columnCount + strlen(pitch) > LCD_COLS) {
            columnCount = 0;
            if (scrolledLineCounter != 0) {
                --scrolledLineCounter;
            }
            else {
                ++lcdCursor;
            }

        }
        // -1 for the top [Song] header.
        // Stop printing if we are off the screen.
        if (lcdCursor > LCD_ROWS - 1) {
            return;
        }

        if (scrolledLineCounter == 0) {
            lcd.setCursor(columnCount, lcdCursor);
            lcd.print(pitch);
        }

        columnCount += strlen(pitch);
    }
}

uint8_t CreatorModeCreateNew::get_lcd_required_rows() {
    uint8_t columnCount = 0;
    uint8_t rowCounter = 0;
    for (uint32_t i = 0; i < newSong->get_size(); i++) {
        const char* pitch = get_note_from_freq(newSong->get_note(i)).pitch;

        if (columnCount + strlen(pitch) > LCD_COLS) {
            columnCount = 0;
            rowCounter++;
        }
        columnCount += strlen(pitch);
    }
    return rowCounter;
}


