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
#include <studio-libs/state.h>
#include <Arduino.h>
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
public:
    CreatorModeCreateNew();
    ~CreatorModeCreateNew();
};

#endif