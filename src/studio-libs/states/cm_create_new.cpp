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
        playSound = true;
        lastButtonPress = BTN_TONE_1;
    }
    else if (is_pressed(BTN_TONE_2)) {
        playSound = true;
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
    }

    // Create a note from the last tune button which was pressed.
    const note currentNote = get_current_tone(lastButtonPress);
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
        lcd.print(currentNote.pitch);
    }

    if (playSound) {
        pinMode(SPEAKER_1, OUTPUT);
        newSong->play_note(currentNote.frequency);
        delay(200);
        playSound = false;
        pinMode(SPEAKER_1, INPUT);
        Serial.println(freeMemory());
    }

    
}

void CreatorModeCreateNew::init() {
    pinMode(SPEAKER_1, INPUT);

    lcd.setCursor(0, 0);
    lcd.print(F("[SONG]"));

    segDisplay.setBrightness(-100);
    lcd.setCursor(0, 1);

    newSong = new Song(SPEAKER_1, 20, 40, MAX_SONG_LENGTH, false);

    previousUpdate = 0;
    lastButtonPress = 0;
    optionWaiting = false;
    playSound = false;


    return;
}