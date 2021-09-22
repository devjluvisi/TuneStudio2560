/**
 * @file states.h
 * @author Jacob LuVisi
 * @brief Represents every different type of state in TuneStudio2560. Read more about states in the state.h class.
 * 
 * Note that created variables in each of the states are global to that state only and are deleted upon changing states.
 * Individual states are allowed to have their own methods.
 * 
 * @version 0.1
 * @date 2021-07-26
 *
 * @copyright Copyright (c) 2021
 *
 */

#ifndef states_h
#define states_h

#include <studio-libs/tune_studio.h>

class MainMenu: public ProgramState {
  private: 
  void init() override;
  void loop() override;
  public: MainMenu();~MainMenu();
};

class ListeningModePlayingSong: public ProgramState {
  #define LM_BOTTOM_TEXT_DELAY_INTERVAL 5000
  private: 
  void init() override;
  void loop() override;
  /** @brief Tracks the last time that the information text was updated on the bottom of the lcd. */
  unsigned long lastTextUpdate;
  /** @brief Tracks a number which represents what text should be displayed on the bottom row of the lcd (information text). */
  uint8_t bottomTextMode;
  /** @brief If the song which has been copied is invalid and cannot be played. */
  bool invalidSong;
  /** @brief If the song is paused. */
  bool isPaused;
  /** @brief If the user has requested to delete their song. */
  bool requestedDelete;
  /** @brief The last time a tone was played. */
  unsigned long lastTonePlay;
  /** @brief The current note of the song that we are on. */
  song_size_t currentSongNote;
  /** @brief The size of the current song. */
  song_size_t currentSongSize;
  #if PRGM_MODE == 0
  /** @brief Tracks how many notes need to pass before a progress block is filled in. */
  song_size_t blockRequirement; 
  /** @brief Tracks the total amount of notes that need to be played before the next progress block gets filled in. */
  song_size_t blockSize;
  #else
  /** @brief Tracks how many notes need to pass before a progress block is filled in. */
  float blockRequirement; 
  /** @brief Tracks the total amount of notes that need to be played before the next progress block gets filled in. */
  float blockSize;
  #endif
  public: ListeningModePlayingSong();~ListeningModePlayingSong();

};

class ListeningModeMenu: public ProgramState {
  private: 
  void init() override;
  void loop() override;
  /** @brief The previous song that the method has read the user selected. Used for knowing when to update the lcd with new information. */
  uint8_t previousSong;

  public: ListeningModeMenu();~ListeningModeMenu();

};

class CreatorModeMenu: public ProgramState {
  private: 
  void init() override;
  void loop() override;
  public: CreatorModeMenu();~CreatorModeMenu();
};

class CreatorModeCreateNew: public ProgramState {

  private: 
  void init() override;
  void loop() override;
  /** @brief The last time the information text was updated. */
  unsigned long previousUpdate; 
  /** @brief The last time a button was pressed. */
  uint8_t lastButtonPress;
  /** @brief If the user has pressed the option button and is waiting for an additonal command. */
  bool optionWaiting;
  /** @brief If the loop should play a sound on the next iteration (user has pressed a button). */
  bool playSound;
  /** @brief The amount of lines the user has scrolled on the lcd. Needed for determining what notes to show. */
  uint8_t scrolledLines;
  
  /**
   * @brief Prints the current song to the LCD and accounts for scrolling.
   */
  void print_song_lcd();

  /**
   * @brief Get the amount of pages that the song should have.
   *
   * @return The number of rows required to store the current amount of notes the user has added to the song.
   *
   */
  uint8_t get_lcd_required_rows();

  /**
   * @brief Allows the user to create their own name for the song. Uses an infinite loop which briefly stops the program
   * while the user chooses a name.
   *
   * @return The name the user has decided on.
   */
  void set_save_name(char fileName[9]);

  /**
   * @brief Gets a character from the alphabet depending on the current reading
   * by an analog input (the potentiometer).
   *
   * This is used when deciding a name for a new song to save.
   *
   * @return The character that matches the analog signal.
   *
   */
  const char get_character_from_analog();

  public: CreatorModeCreateNew();~CreatorModeCreateNew();

};

#endif