/**
 * @file song.h
 * @author Jacob LuVisi
 * @brief The song header file used for defining the methods for a song object. Songs can be added to, removed from, played, and saved
 * to permanent storage like an EEPROM.
 * @version 0.1
 * @date 2021-07-16
 *
 * @copyright Copyright (c) 2021
 *
 */
#ifndef song_h
#define song_h

#include <studio-libs/tune_studio.h>
#include <Arduino.h>

 // The delay that should be added to the song when the user adds a pause.
#ifndef PAUSE_DELAY
#define PAUSE_DELAY (uint16_t)500
#endif

 // Defines what number should be used to indicate that the song should wait while pausing.
#ifndef PAUSE_FREQ
#define PAUSE_FREQ 0x01
#endif

// Represents a part of the song array that has not yet been filled. (Empty)
#ifndef EMPTY_FREQ
#define EMPTY_FREQ 0x00
#endif

// Where in the EEPROM should the songs be saved?
#ifndef EEPROM_SONG_SAVE_ADDR
#define EEPROM_SONG_SAVE_ADDR 0x00
#endif
/*
#if PRGM_MODE == 0
using prgm_song_size = uint8_t;
#elif PRGM_MODE == 2
using prgm_song_size = uint32_t;
#else
using prgm_song_size = uint16_t;
#endif
*/
class Song {
private:
    uint8_t _pin; // The pin to send the frequencies to.
    uint8_t _noteDelay; // The delay between playing each note of the song.
    uint8_t _noteLength; // The length that the note should be played for.
    uint32_t _maxLength; // Quick access to the size of the array.
    uint16_t* _songData; // A array of all of the different tones.
public:
    /**
     * @brief Construct a new song object.
     *
     * @param pin The pin to use for speaker output.
     * @param noteLength The length that the note should be played.
     * @param noteDelay The delay between each note of the song.
     * @param maxLength The maximum length of a song (# of notes).
     * @param init If the constructor should initalize the pin as an output via pinMode.
     */
    Song(uint8_t pin, uint8_t noteLength, uint16_t noteDelay, uint32_t maxLength, bool init);
    /**
     * @brief Destroy the Song object and
     * Eliminates the _songData[] array by flushing it from the heap.
     */
    ~Song();
    /**
     * @brief Play a note at a specified pin.
     * @param note The note to play.
     */
    void play_note(uint16_t note);
    /**
     * @return If the song has reached its max length and nothing more can be added to it.
     */
    bool is_song_full();
    /**
     * @brief Adds a note to the end of the song. If the song is full then the method completes without executing.
     *
     * @param note The note to add.
     */
    void add_note(uint16_t note);
    /**
     * @brief Adds a pause to the song. The amount of time the song is paused by is determined by a unchangeable variable. If the song is full then the method completes without executing.
     *
     */
    void add_pause();
    /**
     * @brief Remove a note from the end of the song.
     */
    void remove_note();
    /**
     * @brief Plays the current song.
     */
    void play_song();
    /**
     * @brief Flushes all data from the array.
     */
    void clear();
    /**
     * @brief Get a note at a specified part of the song.
     *
     * @param index The index of the note to retrieve.
     * @return The frequency of the note.
     */
    uint16_t get_note(uint32_t index);
    /**
     * @return If a song is empty as in it has no notes in it.
     */
    bool is_empty();

    uint32_t get_size();
    /*
        char* get_notes();
    */

};
#endif