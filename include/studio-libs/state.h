/**
 * @file state.h
 * @author Jacob LuVisi
 * @brief Represents the "state" that TuneStudio is currently in. This header file represents the parent class that all states
 * in TuneStudio inherit from.
 *
 * [!] Important information about States [!]
 * ------------------------------------------
 * Each different section of TuneStudio2560 is split off into different states.
 * Each states have their own "routes" which dictate where they go when the user presses a specific button. (Select, Delete, etc)
 * Each state handles user input differently, displays to the lcd using different text, and handles code differently.
 * An example of states would be the home screen or a screen to create a new song.
 * Each state has two primary methods:
 *  - init(): A method to be run once the state is loaded but only when it is first loaded.
 *  - loop(): A method to be constantly run.
 * Note that the init() method for a single state can be run multiple times throughout the programs life cycle but only once after the state has
 * first been switched.
 * Every state is allowed to have its seperate methods from init() and loop() as well as personal private (global) variables with that state.
 * However, variables allocated dynamically during a states lifecycle must be destroyed in the deconstructor.
 *
 * Each individual state is defined in the "states.h" header file. This header file represents the parent class to every state.
 * The main.cpp class holds one global "state" variable called "prgmState" which is overriden when a new state is loaded.
 *
 * "prgmState" is a pointer to this class (parent class) and can be reallocated to any of the child states.
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
  * Useful as it prevents having to compare state objects to each other.
  *
  */
enum StateID {
    MAIN_MENU, CM_MENU, LM_MENU, LM_PLAYING_SONG, CM_CREATE_NEW
};

/**
 * @brief A class which controls how to handle the controls, what to loop, and what to display on the lcd.
 * The ProgramState can be thought of as a "container" where different parts of the program work seperately but use
 * the same global data.
 * 
 * @see state.h
 */
class ProgramState {
private:
    /** @brief If the state has already initalized (ran the init() method) */
    bool _hasInitalized = false;
    /** @brief The corresponding StateID which goes with each state object. */
    StateID _stateId; 
    /** @brief The method to be run when the program state first loads. */
    virtual void init() = 0;
    /** @brief A method to be run on an infinite loop after the init() method is run. */
    virtual void loop() = 0;
public:
    explicit ProgramState(const StateID stateId);

    /**
     * @brief Executes the states functionality.
     * Runs the init() function once and then runs the loop() until the program is terminated
     * or the class is deleted.
     *
     */
    void execute();

    /**
     * @return StateID enum for the current program.
     */
    StateID get_state();

    /**
     * @return If the init() method has been run for the current instance of ProgramState.
     */
    bool has_initalized();

    /** @brief Deconstructor. */
    virtual ~ProgramState();
};

#endif