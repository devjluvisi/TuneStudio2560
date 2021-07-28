/**
 * @file state.cpp
 * @author Jacob LuVisi
 * @brief Represents the shared logic between each different state.
 * @version 0.1
 * @date 2021-07-26
 *
 * @copyright Copyright (c) 2021
 *
 */

#include <studio-libs/state.h>
#include <Arduino.h>
#include <studio-libs/tune_studio.h>

ProgramState::ProgramState(StateID stateId) {
    this->_stateId = stateId;
}

StateID ProgramState::get_state() {
    return this->_stateId;
}

bool ProgramState::has_initalized() {
    return this->_hasInitalized;
}

// Destructor
ProgramState::~ProgramState() {}

void ProgramState::execute() {
    if (!this->_hasInitalized) {

        // Execute these instructions before every initalization

        lcd.clear(); // Remove all information from the previous state.
        segDisplay.blank(); // Clear the 7 segment display.
        segDisplay.refreshDisplay(); // Update the 7 segment display.

        this->init();
        this->_hasInitalized = true;
    }
    this->loop();
}