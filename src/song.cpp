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

#include <Arduino.h>
#include <song.h>
#include <tune_studio.h>

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

void song::print_info() {
    Serial.println("------------------------------------");
    Serial.println("song.cpp: Information on current song.");
    Serial.println("Pin: " + String(_pin));
    Serial.println("Note Length: " + String(_noteLength));
    Serial.println("Note Delay: " + String(_noteDelay));
    Serial.println("Max Song Size: " + this->_maxLength);
    Serial.println("------------------------------------");
}

void song::play_note(uint16_t note) {
    tone(_pin, note);
}

bool song::is_song_full() {
    // Check if the final value in the array is zero. If so we know it is not full since 0's represent empty values in
    // an integer array.
    return _songData[_maxLength - 1] != EMPTY_NOTE;
}

void song::add_note(uint16_t note) {
    if (song::is_song_full()) return;
    for (uint32_t i = 0; i < this->_maxLength; i++) {
        if (_songData[i] == EMPTY_NOTE) {
            _songData[i] = note;
            Serial.println("song.cpp: Added note (" + String(note) + ") to INDEX: " + String(i));
            return;
        }
    }
    Serial.println("song.cpp: Could not find anywhere to add new note. Song full?");
}

void song::add_pause() {
    if (song::is_song_full()) return;
    for (uint32_t i = 0; i < this->_maxLength; i++) {
        if (_songData[i] == EMPTY_NOTE) {
            _songData[i] = PAUSE_NOTE;
            Serial.println("song.cpp: Added pause to INDEX: " + String(i));
            return;
        }
    }
    Serial.println("song.cpp: Could not find anywhere to add pause. Song full?");
}

void song::remove_note() {
    // Loop through song and find first non-empty number.
    for (uint32_t i = this->_maxLength; i > 0; i--) {
        if (_songData[i] != EMPTY_NOTE) {
            _songData[i] = EMPTY_NOTE;
            return;
        }
    }
    Serial.println("song.cpp: Could not find a note to remove.");
}

void song::play_song() {
    Serial.println("song.cpp: Playing a song.");
    uint32_t songIndex = 0;
    // While the song index is not empty and does not equal the maximum length allowed.
    while (_songData[songIndex] != EMPTY_NOTE && songIndex != this->_maxLength) {
        Serial.println("song.cpp: Playing song note: " + String(_songData[songIndex]));
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
    Serial.println("song.cpp: Song has finished playing.");
}

void song::clear() {
    // Similar to remove method but does not return after a single index is removed.
    Serial.println("song.cpp: Deleting current song data.");
    for (uint32_t i = this->_maxLength; i > 0; i--) {
        if (_songData[i] != EMPTY_NOTE) {
            _songData[i] = EMPTY_NOTE;
        }
    }
}

uint16_t song::get_note(uint32_t index) {
    return _songData[index];
}

bool song::is_empty() {
    return _songData[0] == EMPTY_NOTE;
}

