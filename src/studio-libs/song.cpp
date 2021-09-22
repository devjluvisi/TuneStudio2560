/**
 * @file song.cpp
 * @author Jacob LuVisi
 * @brief The main class which defines a song object and its utility methods.
 * @version 0.1
 * @date 2021-07-16
 *
 * @copyright Copyright (c) 2021
 *
 * NOTE: Every song object created takes the same amount of SRAM [for max length 255] regardless of the number of notes and
 * empty spaces in the song. Increasing the maximum number of notes may increase the size of the objects.
 *
 */

#include <studio-libs/song.h>
#include <studio-libs/tune_studio.h>


template <> Song<MAX_SONG_LENGTH>::Song(uint8_t pin, uint8_t noteLength, uint16_t noteDelay) {
#if DEBUG == true
    Serial.print(get_active_time());
    Serial.println(F(" song.cpp >> Initalized a new song."));
#endif
    _pin = pin;
    _noteDelay = noteDelay;
    _noteLength = noteLength;
    _currSize = 0;
    _songData[MAX_SONG_LENGTH] = {EMPTY_NOTE.frequency};
}

template<> song_size_t Song<MAX_SONG_LENGTH>::get_size() {
    return _currSize;
}

template <> void Song<MAX_SONG_LENGTH>::play_note(uint16_t note) {
#if DEBUG == true
    Serial.print(get_active_time());
    Serial.print(F(" song.cpp >> Playing note: "));
    Serial.println(note);
#endif
    // Since v1.1.0-R2
    // Check NewTone lib for details: https://bitbucket.org/teckel12/arduino-new-tone/src/master/
    NewTone(_pin, note);
}

template<> bool Song<MAX_SONG_LENGTH>::is_song_full() {
    // Check if the final value in the array is zero. If so we know it is not full since 0's represent empty values in
    // an integer array.
    return _songData[MAX_SONG_LENGTH - 1] != EMPTY_NOTE.frequency;
}
template<> void Song<MAX_SONG_LENGTH>::add_note(uint16_t note) {
    if (is_song_full() || note == EMPTY_NOTE.frequency) return;
    _songData[get_size()] = note;
    _currSize++;
}
template<> void Song<MAX_SONG_LENGTH>::add_pause() {
    add_note(PAUSE_NOTE.frequency);
}
template<> void Song<MAX_SONG_LENGTH>::remove_note() {
    _songData[get_size() - 1] = EMPTY_NOTE.frequency;
    _currSize--;
}
template<> void Song<MAX_SONG_LENGTH>::play_song() {
    
    song_size_t songIndex = 0;
    // While the song index is not empty and does not equal the maximum length allowed.
    while (songIndex != get_size()) {
        if (_songData[songIndex] == PAUSE_NOTE.frequency) {
            delay_ms(PAUSE_DELAY); // Delay the song from continuing for a certain amount of time.
            songIndex++; // Go to the next index of the song.
            continue; // Go to the next iteration of the loop.
        }
        play_note(_songData[songIndex]);
        delay_ms(_noteLength);
        noNewTone(_pin);
        delay_ms(_noteDelay);
        songIndex++;
    }
}
template<> void Song<MAX_SONG_LENGTH>::clear() {
    memset(_songData, EMPTY_NOTE.frequency, sizeof(_songData));
    _currSize = 0;
    #if DEBUG == true
    Serial.print(get_active_time());
    Serial.println(F(" song.cpp >> Song object has been cleared."));
    #endif
}

template<> uint16_t Song<MAX_SONG_LENGTH>::get_note(song_size_t index) {
    return _songData[index];
}

template<> bool Song<MAX_SONG_LENGTH>::is_empty() {
    return _songData[0] == EMPTY_NOTE.frequency;
}

template<> void Song<MAX_SONG_LENGTH>::set_attributes(uint8_t noteLength, uint16_t noteDelay) {
    _noteLength = noteLength;
    _noteDelay = noteDelay;
#if DEBUG == true
    Serial.print(get_active_time());
    Serial.println(F(" song.cpp >> Updated attributes noteLength and noteDelay for a song class."));
#endif
}

template<> uint8_t Song<MAX_SONG_LENGTH>::get_note_length() {
    return _noteLength;
}

template<> uint16_t Song<MAX_SONG_LENGTH>::get_note_delay() {
    return _noteDelay;
}