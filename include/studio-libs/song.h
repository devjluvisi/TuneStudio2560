/**
 * @file song.h
 * @author Jacob LuVisi
 * @brief The song header file used for defining the methods for a song object. Songs can be added to, removed from, played, and saved
 * to permanent storage like an SD Card. There are important notes about using the song class which should be read below.
 *
 * Note that songs do not use the "note" struct defined in tune_studio.h to store notes. Instead a basic 16-bit unsigned integer
 * array stores the frequencies of each of the individual notes. In order to get the respective "pitch" of the current note the
 * frequency must be passed into the get_note_from_freq(const uint16_t frequency) method in the main class.
 *
 * The array which stores the frequencies for the song is dynamically allocated initally depending on the _maxLength value but it cannot
 * be reallocated once it is set in the constructor. When a song pointer has finished its use it should be removed via delete.
 *
 * The size of the song is dependent on the PRGM_MODE ("Program Mode"). This allows any user who edits TuneStudio2560 to change the max length
 * of the song to their own desire. A special song_size_t datatype is used to keep track of which unsigned integer datatype to use depending on the Program Mode for
 * maximum efficiency.
 *
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

#if PRGM_MODE == 0
typedef uint8_t song_size_t;
#elif PRGM_MODE == 2
typedef uint32_t song_size_t;
#else
typedef uint8_t song_size_t;
#endif

// The time to delay the song when the user adds a pause.
constexpr uint16_t PAUSE_DELAY = 500;

class Song {
private:
    uint8_t _pin; // The pin to send the frequencies to.
    uint8_t _noteDelay; // The delay between playing each note of the song.
    uint8_t _noteLength; // The length that the note should be played for.
    song_size_t _maxLength; // Quick access to the size of the array.
    uint16_t* _songData; // A array of all of the different tones.
    uint16_t EMPTY_FREQ;
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
    Song(uint8_t pin, uint8_t noteLength, uint16_t noteDelay, song_size_t maxLength, bool init);
    /**
     * @brief Destroy the Song object and
     * Eliminates the _songData[] array by flushing it from the heap.
     */
    ~Song();

    /**
     * @brief Play a note at a specified pin. This method is not neccessarily tied to any specific song object and works the same
     * no matter how the song is built.
     *
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
    uint16_t get_note(song_size_t index);

    /**
     * @return If a song is empty as in it has no notes in it.
     */
    bool is_empty();

    /**
     * @brief Gets the size of the song (number of individual frequencies). Note that this uses a regular iterative loop to find the size
     * so its return value should be cached when possible for maximum performance.
     *
     * @return Size of the song.
     */
    song_size_t get_size();

    /**
     * @brief Set the attributes of the song.
     *
     * @param noteLength The note length to set.
     * @param noteDelay The delay between each note to set.
     */
    void set_attributes(uint8_t noteLength, uint16_t noteDelay);

    /**
     * @return The length that each note is played for.
     */
    uint8_t get_note_length();

    /**
     * @return The delay between each note.
     */
    uint16_t get_note_delay();
};
#endif