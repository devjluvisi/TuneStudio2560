/**
 * @file states.h
 * @author Jacob LuVisi
 * @brief Represents every different type of state in TuneStudio2560. Read more about states in the state.h class.
 * @version 0.1
 * @date 2021-07-26
 *
 * @copyright Copyright (c) 2021
 *
 */

#ifndef states_h
#define states_h

#include <studio-libs/tune_studio.h>

class MainMenu : public ProgramState {
private:
    void init() override;
    void loop() override;
public:
    MainMenu();
    ~MainMenu();
};

class ListeningModePlayingSong : public ProgramState {
private:
    void init() override;
    void loop() override;
public:
    ListeningModePlayingSong();
    ~ListeningModePlayingSong();
};

class ListeningModeMenu : public ProgramState {
private:
    void init() override;
    void loop() override;
public:
    ListeningModeMenu();
    ~ListeningModeMenu();
};

class CreatorModeMenu : public ProgramState {
private:
    void init() override;
    void loop() override;
public:
    CreatorModeMenu();
    ~CreatorModeMenu();
};

class CreatorModeCreateNew : public ProgramState {

private:
    void init() override;
    void loop() override;
    unsigned long previousUpdate;
    uint8_t lastButtonPress;
    bool optionWaiting;
    bool playSound;
    Song* newSong;
    // LCD Management
    uint8_t scrolledLines; // The amount of lines scrolled on the lcd.
    char fileName[9];
    /**
     * @brief Prints the current song to the LCD and accounts for scrolling.
     */
    void print_song_lcd();
    /**
     * @brief Get the amount of pages that the song should habe.
     *
     * @return uint8_t
     */
    uint8_t get_lcd_required_rows();
    /**
     * @brief Allows the user to create their own name for the song. Uses an infinite loop which briefly stops the program
     * while the user chooses a name.
     *
     * @return The name the user has decided on.
     */
    void set_save_name();
    /**
     * @brief Gets a character from the alphabet depending on the current reading
     * by an analog input (the potentiometer).
     *
     * This is used when deciding a name for a new song to save.
     *
     * @return The character that matches the analog signal.
     */
    const char get_character_from_analog();

public:
    CreatorModeCreateNew();
    ~CreatorModeCreateNew();

};

#endif

