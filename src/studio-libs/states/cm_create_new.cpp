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
    pinMode(SPEAKER_1, INPUT);
    Song newSong(SPEAKER_1, 20, 40, MAX_SONG_LENGTH, false);
    // SPLIT INTO SEPERATE METHOD.
    //Song newSong(SPEAKER_1, 20, 40, false);
    lcd.setCursor(0, 0);
    lcd.print(F("[SONG]"));
    // Track some variables for adding/removing

    uint8_t lastButtonPress = 0;
    // If the user has pressed the option button.
    bool optionWaiting = false;
    bool playSound = false;

    // Create an infinite loop while creating a song.
    segDisplay.setBrightness(-100);
    lcd.setCursor(0, 1);
    while (true) {
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

        // Update the segment display to reflect the current note.
        segDisplay.refreshDisplay();
        if (millis() % 100 == 0) {
            // OPTION BUTTON
            //noTone(SPEAKER_1); // Fixes bug where the LED sometimes will not light up.
            analogWrite(RGB_GREEN, optionWaiting * RGB_BRIGHTNESS / 3);
            analogWrite(RGB_RED, optionWaiting * RGB_BRIGHTNESS);
            // 7 SEGMENT DISPLAY
            segDisplay.setChars(currentNote.pitch);
        }
        segDisplay.refreshDisplay();

        // Add a tune if the button to add/select is pressed.
        if (is_pressed(BTN_ADD_SELECT)) {
            lcd.print(currentNote.pitch);
        }

        if (playSound) {
            pinMode(SPEAKER_1, OUTPUT);
            newSong.play_note(currentNote.frequency);
            delay(200);
            playSound = false;
            pinMode(SPEAKER_1, INPUT);
        }
    }

}

void CreatorModeCreateNew::init() {
    return;
}