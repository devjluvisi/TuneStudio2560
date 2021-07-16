/**
 * @file song.cpp
 * @author Jacob LuVisi
 * @brief The main class which defines a song object and its utility methods.
 * @version 0.1
 * @date 2021-07-16
 *
 * @copyright Copyright (c) 2021
 *
 */

#include <Arduino.h>
#include <song.h>

song::song(uint8_t pin, uint8_t noteLength, uint16_t noteDelay, bool init) {
    Serial.println("song.cpp: Initalized a new song.");
    Serial.println("Pin: " + String(pin));
    Serial.println("Note Length: " + String(noteLength));
    Serial.println("Note Delay: " + String(noteDelay));
    if (init) {
        Serial.println("song.cpp: Initalized pin " + String(pin) + " for output.");
        pinMode(pin, OUTPUT);
    }
    _pin = pin;
    _noteDelay = noteDelay;
    _noteLength = noteLength;
}

void song::play_note(uint16_t note) {
    tone(_pin, note);
}

bool song::is_song_full() {
    // Check if the final value in the array is zero. If so we know it is full since 0's represent empty values in
    // an integer array.
    return _songData[_maxLength - 1] == 0;
}

void song::add_note(uint16_t note) {
    if (song::is_song_full) return;
    for (int i = 0; i < this->_maxLength; i++) {
        if (_songData[i] == 0) {
            _songData[i] = note;
            return;
        }
    }

    //TODO: KEEP ADDING METHOD FUNCTION

}