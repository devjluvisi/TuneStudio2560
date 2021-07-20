/**
 * @file text.h
 * @author Jacob LuVisi
 * @brief A header to create a text object. A text object is used for displaying text on the LCD with some fancy utility methods.
 * In a way, it could be thought of as an extension of the I2C LiquidCrystal class.
 * @version 0.1
 * @date 2021-07-18
 *
 * @copyright Copyright (c) 2021
 *
 */

#ifndef text_h
#define text_h

#include <Arduino.h>
#include <tune_studio.h>

#define MAX_TEXT_OBJECT_LINES 16
#define DEFAULT_CHAR_DELAY 150
#define DEFAULT_NEW_LINE_DELAY 0
#define DEFAULT_REFRESH_DELAY 650

class Text {
private:
    String _text[MAX_TEXT_OBJECT_LINES];
    uint8_t _charDelay;
    uint8_t _newLineDelay;
    uint16_t _refreshDelay;
public:

    /**
     * @brief Construct a new Text object. Uses the default values defined in this class.
     * Keep each text line below 255 characters.
     * Keep the amount of lines in a text object below 16.
     * You cannot reassign a line of text with a string longer then its original size.
     */
    Text();
    /**
     * @brief Construct a new Text object
     *
     * Keep each text line below 255 characters.
     * Keep the amount of lines in a text object below 16.
     * You cannot reassign a line of text with a string longer then its original size.
     *
     * @param charDelay The delay between printing each character.
     * @param newLineDelay The delay when moving to the next line.
     * @param refreshDelay The delay before clearing the screen and continuing to print.
     */
    Text(uint8_t charDelay, uint8_t newLineDelay, uint16_t refreshDelay);
    /**
     * @brief Set a line in the text.
     *
     *
     * @param line The line to set. Lines can go past the max rows of the LCD. It just means that the LCD will clear before printing.
     * @param text The text to set the line to.
     */
    void setLine(uint8_t line, String text);
    /**
     * @brief Print the text onto the lcd object.
     *
     */
    void print();
    /**
     * @brief Prints the text but instead of scrolling it continues to print each line until it terminates.
     *
     */
    void print_large();
};

#endif
