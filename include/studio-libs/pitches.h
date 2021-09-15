/**
 * @file pitches.h
 * @author Jacob LuVisi
 * @brief A complete fill containing all of the human readable pitches available in TuneStudio2560.
 *  
 *  This header file was created to store constants for every char string (c-string) pitch which has a respective frequency.<br />
 *  This is preferable for microcontrollers with low RAM due to the fact that the previous method:<br />
    const buttonFrequencies_t PROGRAM_NOTES[TONE_BUTTON_AMOUNT] PROGMEM{<br />
        {BTN_TONE_1, {{"B0", 31}, ...}},<br />
    };<br />
    Stored the pointers for the pitches in PROGMEM but the pitch data was still stored in SRAM causing over 357 of SRAM bytes to be used. <br />
    This header file is dedicated to making sure the pitch data and the pitch pointers remain and are retrieved from PROGMEM. <br />
    <br />
    ** Used Since v1.2.0-R3 **
 *  
 * @version 1.2.0-R3
 * @date 2021-08-29
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#ifndef pitches_h
#define pitches_h
#include <avr/pgmspace.h>

// Green Tune Button
const char pitch_b0[] PROGMEM = "B0";
const char pitch_c1[] PROGMEM = "C1";
const char pitch_cs1[] PROGMEM = "CS1";
const char pitch_d1[] PROGMEM = "D1";
const char pitch_ds1[] PROGMEM = "DS1";
const char pitch_e1[] PROGMEM = "E1";
const char pitch_f1[] PROGMEM = "F1";
const char pitch_fs1[] PROGMEM = "FS1";
const char pitch_g1[] PROGMEM = "G1";
const char pitch_gs1[] PROGMEM = "GS1";
const char pitch_a1[] PROGMEM = "A1";
const char pitch_as1[] PROGMEM = "AS1";
const char pitch_b1[] PROGMEM = "B1";
const char pitch_c2[] PROGMEM = "C2";
const char pitch_cs2[] PROGMEM = "CS2";
const char pitch_d2[] PROGMEM = "D2";
const char pitch_ds2[] PROGMEM = "DS2";

// Blue Tune Button
const char pitch_e2[] PROGMEM = "E2";
const char pitch_f2[] PROGMEM = "F2";
const char pitch_fs2[] PROGMEM = "FS2";
const char pitch_g2[] PROGMEM = "G2";
const char pitch_gs2[] PROGMEM = "GS2";
const char pitch_a2[] PROGMEM = "A2";
const char pitch_as2[] PROGMEM = "AS2";
const char pitch_b2[] PROGMEM = "B2";
const char pitch_c3[] PROGMEM = "C3";
const char pitch_cs3[] PROGMEM = "CS3";
const char pitch_d3[] PROGMEM = "D3";
const char pitch_ds3[] PROGMEM = "DS3";
const char pitch_e3[] PROGMEM = "E3";
const char pitch_f3[] PROGMEM = "F3";
const char pitch_fs3[] PROGMEM = "FS3";
const char pitch_g3[] PROGMEM = "G3";
const char pitch_gs3[] PROGMEM = "GS3";

// Red Tune Button
const char pitch_a3[] PROGMEM = "A3";
const char pitch_as3[] PROGMEM = "AS3";
const char pitch_b3[] PROGMEM = "B3";
const char pitch_c4[] PROGMEM = "C4";
const char pitch_cs4[] PROGMEM = "CS4";
const char pitch_d4[] PROGMEM = "D4";
const char pitch_ds4[] PROGMEM = "DS4";
const char pitch_e4[] PROGMEM = "E4";
const char pitch_f4[] PROGMEM = "F4";
const char pitch_fs4[] PROGMEM = "FS4";
const char pitch_g4[] PROGMEM = "G4";
const char pitch_gs4[] PROGMEM = "GS4";
const char pitch_a4[] PROGMEM = "A4";
const char pitch_as4[] PROGMEM = "AS4";
const char pitch_b4[] PROGMEM = "B4";
const char pitch_c5[] PROGMEM = "C5";
const char pitch_cs5[] PROGMEM = "CS5";

// Yellow Tune Button
const char pitch_d5[] PROGMEM = "D5";
const char pitch_ds5[] PROGMEM = "DS5";
const char pitch_e5[] PROGMEM = "E5";
const char pitch_f5[] PROGMEM = "F5";
const char pitch_fs5[] PROGMEM = "FS5";
const char pitch_g5[] PROGMEM = "G5";
const char pitch_gs5[] PROGMEM = "GS5";
const char pitch_a5[] PROGMEM = "A5";
const char pitch_as5[] PROGMEM = "AS5";
const char pitch_b5[] PROGMEM = "B5";
const char pitch_c6[] PROGMEM = "C6";
const char pitch_cs6[] PROGMEM = "CS6";
const char pitch_d6[] PROGMEM = "D6";
const char pitch_ds6[] PROGMEM = "DS6";
const char pitch_e6[] PROGMEM = "E6";
const char pitch_f6[] PROGMEM = "F6";
const char pitch_fs6[] PROGMEM = "FS6";

// White Tune Button
const char pitch_g6[] PROGMEM = "G6";
const char pitch_gs6[] PROGMEM = "GS6";
const char pitch_a6[] PROGMEM = "A6";
const char pitch_as6[] PROGMEM = "AS6";
const char pitch_b6[] PROGMEM = "B6";
const char pitch_c7[] PROGMEM = "C7";
const char pitch_cs7[] PROGMEM = "CS7";
const char pitch_d7[] PROGMEM = "D7";
const char pitch_ds7[] PROGMEM = "DS7";
const char pitch_e7[] PROGMEM = "E7";
const char pitch_f7[] PROGMEM = "F7";
const char pitch_fs7[] PROGMEM = "FS7";
const char pitch_g7[] PROGMEM = "G7";
const char pitch_gs7[] PROGMEM = "GS7";
const char pitch_a7[] PROGMEM = "A7";
const char pitch_as7[] PROGMEM = "AS7";
const char pitch_b7[] PROGMEM = "B7";

#endif
