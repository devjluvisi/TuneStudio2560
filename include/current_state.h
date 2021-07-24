/**
 * @file current_state.h
 * @author Jacob LuVisi
 * @brief An object which represents one of various "states" that the application can be running in.
 * In TuneStudio2560 software, each different state represents a different part of the software in which
 * a user can do different things (Example: The main menu would be one state while listening to a song would be
 * another state). States manage what data should be shown on the LCD, Segment Display, and which button presses we
 * should be expecting or waiting for. States are important as they compartmentalize the code into different areas
 * which are easier to manage.
 * States can be changed at any time while the application is running.
 * Every possible state that can be represented in TuneStudio2560 is listed below.
 *
 * There are three primary dividers or "modes" of TuneStudio2560. Each mode may have prefixes to their respective states.
 *
 * Main Menu -> Mode 1 (No Prefix)
 * Creator Mode -> Mode 2 (CM_ Prefix)
 * Listen Mode -> Mod 3 (LM_ Prefix)
 *
 * @version 0.1
 * @date 2021-07-18
 *
 * @copyright Copyright (c) 2021
 *
 */
#ifndef current_state_h
#define current_state_h

#include <tune_studio.h>
#include <Arduino.h>

 /**
  * @brief The different types of states that the user can be in.
  *
  */
enum PossibleStates {
    MAIN_MENU, CM_MENU, LM_MENU, LM_PLAYING_SONG, CM_CREATE_NEW
};

/**
 * @brief Represents the current state the user is in.
 * Allows the managing of such state.
 */
class CurrentState {
private:
    PossibleStates _currentState; // The current state the application is in.
    bool _hasInitalized = false; // If the current state has already been initalized or not.
public:
    /**
     * @brief Construct a new state application object.
     *
     * @param currentState The current state that should be run when the constructor initalizes.
     */
    CurrentState(PossibleStates currentState);
    /**
     * @brief Set the current state of the application.
     *
     * @param state The state to set the application to.
     */
    void set_state(PossibleStates state);
    /**
     * @return The current state the application is in.
     */
    PossibleStates get_state();
    /**
     * @brief A method which is ran whenever the state is first loaded.
     * This method can be run multiple times throughout the lifespan of TuneStudio2560 but should
     * only be run on the first iteration after the state has loaded and not again until the state
     * is switched to.
     */
    void init();
    /**
     * @brief A method to be ran on repeat everytime the main loop() function repeats.
     * Runs all of the logic for that specific state.
     */
    void load();
};
// /**
//  * @brief Represents a generic state.
//  * Each state in the "PossibleStates" enum has a respecting State class.
//  */
// class State {
// private:
//     PossibleStates stateName;
//     bool hasInitalized = false; // If the init() method has been ran.
// public:
//     /**
//      * @brief A method which is only ran once when the state is first loaded.
//      * The init() can be run multiple times throughout TuneStudio2560's lifespan but
//      * it only should be run once when the state is first rendered.
//      */
//     void init();
//     /**
//      * @brief A method which is ran everytime the main loop checks the state.
//      */
//     void load();
// };


#endif