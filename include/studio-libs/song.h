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
 * [OUTDATED] R1 & R2:
 * X // The array which stores the frequencies for the song is dynamically allocated initally depending on the _maxLength value but it cannot
 * X // be reallocated once it is set in the constructor. When a song pointer has finished its use it should be removed via delete.
 * 
 * R3 & NEWER:
 * The array for the song class is set automatically by PRGM_MODE and the MAX_SONG_LENGTH constant in tune_studio.h
 * Only one song object in TuneStudio is created and that is the global "prgmSong" object which can be used by different states
 * at any time during the programming cycle. (Songs are no longer created/freed on the heap as of v1.2.0-R3)
 * 
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

#include <NewTone.h>

typedef uint8_t song_size_t;

// The time to delay the song when the user adds a pause.
constexpr uint16_t PAUSE_DELAY = 500;
template < song_size_t MAX_SONG_SIZE >
class Song {
  private:
  uint8_t _pin; // The pin to send the frequencies to.
  uint8_t _noteDelay; // The delay between playing each note of the song.
  uint8_t _noteLength; // The length that the note should be played for.
  song_size_t _currSize; // Current size of the song.
  uint16_t _songData[MAX_SONG_SIZE]; // A array of all of the different tones.
  public:
    /**
     * @brief Construct a new song object.
     *
     * @param pin The pin to use for speaker output.
     * @param noteLength The length that the note should be played.
     * @param noteDelay The delay between each note of the song.
     */
    Song(uint8_t pin, uint8_t noteLength, uint16_t noteDelay);
    
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
   * @brief Get a frequency at a specified index of the song.
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
  uint8_t get_size();

  /**
   * @brief Set the attributes of the song.
   * Only updates the note length and note delay. Leaves the rest of the song untouched.
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