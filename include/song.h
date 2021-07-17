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

#include <tune_studio.h>
#include <Arduino.h>

 // The delay that should be added to the song when the user adds a pause.
#ifndef PAUSE_DELAY
#define PAUSE_DELAY 500
#endif

 // Defines what number should be used to indicate that the song should wait while pausing.
#ifndef PAUSE_NOTE
#define PAUSE_NOTE 0x01
#endif

// Represents a part of the song array that has not yet been filled. (Empty)
#ifndef EMPTY_NOTE
#define EMPTY_NOTE 0x00
#endif

// Where in the EEPROM should the songs be saved?
#ifndef EEPROM_SONG_SAVE_ADDR
#define EEPROM_SONG_SAVE_ADDR 0x00
#endif

class song {
private:
    uint8_t _pin; // The pin to send the frequencies to.
    uint8_t _noteDelay; // The delay between playing each note of the song.
    uint8_t _noteLength; // The length that the note should be played for.
    uint32_t _maxLength = MAX_SONG_LENGTH; // Quick access to the size of the array.
    uint16_t _songData[MAX_SONG_LENGTH] = { 0 }; // A array of all of the different tones.
public:
    /**
     * @brief Construct a new song object.
     *
     * @param pin The pin to use for speaker output.
     * @param noteLength The length that the note should be played.
     * @param noteDelay The delay between each note of the song.
     * @param init If the constructor should initalize the pin as an output via pinMode.
     */
    song(uint8_t pin, uint8_t noteLength, uint16_t noteDelay, bool init);
    /**
     * @brief Prints some information about the current song to
     * the Serial monitor.
     *
     * Requires serial to be enabled.
     *
     */
    void print_info();
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

};
#endif