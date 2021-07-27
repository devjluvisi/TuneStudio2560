/**
 * @file state.h
 * @author Jacob LuVisi
 * @brief Represents the "state" that TuneStudio is currently in.
 *
 * Each different section of TuneStudio2560 is split off into different states.
 * Each states have their own "routes" which dictate where they go when the user presses a specific button. (Select, Delete, etc)
 * Each state handles user input differently, displays to the lcd using different text, and handles code differently.
 * An example of states would be the home screen or a screen to create a new song.
 * Each state has two primary methods:
 *  - init(): A method to be run once the state is loaded but only when it is first loaded.
 *  - loop(): A method to be constantly run.
 * Note that the init() method for a single state can be run multiple times throughout the programs life cycle but only once after the state has
 * first been switched.
 *
 * Each individual state is defined in the ./states folder. This header file represents the parent class to every state.
 * The main.cpp class holds one global "state" variable which is overriden when a new state is loaded.
 *
 * @version 0.1
 * @date 2021-07-26
 *
 * @copyright Copyright (c) 2021
 *
 */
#ifndef state_h
#define state_h

 /**
  * @brief Allows the identification of individual states.
  *
  */
enum StateID {
    MAIN_MENU, CM_MENU, LM_MENU, LM_PLAYING_SONG, CM_CREATE_NEW
};

class ProgramState {
private:
    bool _hasInitalized = false; // If the state has initalized.
    StateID _stateId;
    virtual void init() = 0;
    virtual void loop() = 0;
public:
    ProgramState(StateID stateId);

    void execute();

    StateID get_state();

    bool has_initalized();

    virtual ~ProgramState();
};

#endif