/**
 * @file song.cpp
 * @author Jacob LuVisi
 * @brief The main class which defines a song object and its utility methods.
 * @version 0.1
 * @date 2021-07-16
 *
 * @copyright Copyright (c) 2021
 *
 * NOTE: Every song object created is exactly 517 bytes in size [for max length 255] regardless of the number of notes and
 * empty spaces in the song. Increasing the maximum number of notes may increase the size of the objects.
 *
 * TODO: Optimize functions for song searching to be faster. Instead of iterating through the entire
 * loop each time to find the next avaliable space to add a pause/note, iterate only once and then
 * cache that value in memory. Next time we need to search for the next avaliable space to add/remove note
 * we can just read the cached memory value and add it instead of looping through the song each time.
 *
 * OR -> Add a variable that tracks how many notes we have added so far.
 *
 */

#include <song.h>

Song::Song(uint8_t pin, uint8_t noteLength, uint16_t noteDelay, bool init) {
    Serial.println(F("song.cpp: Initalized a new song."));
    Serial.println("Pin: " + String(pin));
    Serial.println("Note Length: " + String(noteLength));
    Serial.println("Note Delay: " + String(noteDelay));

    if (init) {
        Serial.println(F("Initalized the song on a pin."));
        pinMode(pin, OUTPUT);
    }
    _pin = pin;
    _noteDelay = noteDelay;
    _noteLength = noteLength;
}

void Song::print_info() {
    Serial.println("------------------------------------");
    Serial.println("song.cpp: Information on current song.");
    Serial.println("Pin: " + String(_pin));
    Serial.println("Note Length: " + String(_noteLength));
    Serial.println("Note Delay: " + String(_noteDelay));
    Serial.println("Max Song Size: " + String(this->_maxLength));
    Serial.println("------------------------------------");
}

void Song::play_note(uint16_t note) {
    tone(_pin, note);
}

bool Song::is_song_full() {
    // Check if the final value in the array is zero. If so we know it is not full since 0's represent empty values in
    // an integer array.
    return _songData[_maxLength - 1] != EMPTY_NOTE;
}

void Song::add_note(uint16_t note) {
    if (Song::is_song_full()) return;
    for (uint32_t i = 0; i < this->_maxLength; i++) {
        if (_songData[i] == EMPTY_NOTE) {
            _songData[i] = note;
            return;
        }
    }
}

void Song::add_pause() {
    if (Song::is_song_full()) return;
    for (uint32_t i = 0; i < this->_maxLength; i++) {
        if (_songData[i] == EMPTY_NOTE) {
            _songData[i] = PAUSE_NOTE;
            return;
        }
    }
}

void Song::remove_note() {
    // Loop through song and find first non-empty number.
    for (uint32_t i = this->_maxLength; i > 0; i--) {
        if (_songData[i] != EMPTY_NOTE) {
            _songData[i] = EMPTY_NOTE;
            return;
        }
    }
}

void Song::play_song() {
    uint32_t songIndex = 0;
    // While the song index is not empty and does not equal the maximum length allowed.
    while (_songData[songIndex] != EMPTY_NOTE && songIndex != this->_maxLength) {
        if (_songData[songIndex] == PAUSE_NOTE) {
            delay(PAUSE_DELAY); // Delay the song from continuing for a certain amount of time.
            songIndex++; // Go to the next index of the song.
            continue; // Go to the next iteration of the loop.
        }

        play_note(_songData[songIndex]);
        delay(_noteLength);
        noTone(_pin);
        delay(_noteDelay);
        songIndex++;
    }
}

void Song::clear() {
    for (uint32_t i = this->_maxLength; i > 0; i--) {
        if (_songData[i] != EMPTY_NOTE) {
            _songData[i] = EMPTY_NOTE;
        }
    }
}

uint16_t Song::get_note(uint32_t index) {
    return _songData[index];
}

bool Song::is_empty() {
    return _songData[0] == EMPTY_NOTE;
}

