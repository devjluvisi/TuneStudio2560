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
 * <b>[OUTDATED] R1 & R2:</b>
 * X // The array which stores the frequencies for the song is dynamically allocated initally depending on the _maxLength value but it cannot
 * X // be reallocated once it is set in the constructor. When a song pointer has finished its use it should be removed via delete.
 * 
 * <b>R3 & NEWER</b>:
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

/** @brief A variable type which can be either uint8_t or uint16_t depending on song size and PRGM_MODE. */
typedef uint8_t song_size_t;

/** @brief The global delay that should be used when a PAUSE_NOTE is encountered. Default: 500ms */
constexpr uint16_t PAUSE_DELAY = 500;

/**
 * @brief A Song object.
 * Initalized with a <MAX_SONG_SIZE> template to describe how large the songs are allowed to be in the program.
 * 
 * @tparam MAX_SONG_SIZE 
 */
template < song_size_t MAX_SONG_SIZE >
class Song {
  private:
  /** @brief The current hardware pin of a speaker. */
  uint8_t _pin; 
  /** @brief The note delay of the song. */
  uint8_t _noteDelay; 
  /** @brief The note length of the song. */
  uint8_t _noteLength; 
  /** @brief The current size of the song. Is changed every time a note is added or remove from the _songData. */
  song_size_t _currSize;
  /**
   * @brief An array of MAX_SONG_SIZE which holds the values of all of the frequencies the song should play.
   * @remark It is important to know that the Song class does not directly deal with the note_t struct at all. Rather, notes are stored as frequencies only and pitches are retrieved on the fly.
   * @see note_t get_note_from_freq(const uint16_t frequency)
   * @see note_t get_note_from_pitch(const char * const pitch)
   * 
   */
  uint16_t _songData[MAX_SONG_SIZE];
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
   * @remark Uses a blocking delay method.
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
   * @brief Gets the size of the song (number of individual frequencies).
   * @return Size of the song.
   */
  song_size_t get_size();

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