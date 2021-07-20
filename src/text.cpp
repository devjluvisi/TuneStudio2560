/**
 * @file text.cpp
 * @author Jacob LuVisi
 * @brief Logic for text.h class.
 * @version 0.1
 * @date 2021-07-18
 *
 * @copyright Copyright (c) 2021
 *
 */

#include <text.h>

Text::Text() {
    _charDelay = DEFAULT_CHAR_DELAY;
    _newLineDelay = DEFAULT_NEW_LINE_DELAY;
    _refreshDelay = DEFAULT_REFRESH_DELAY;
}

Text::Text(uint8_t charDelay, uint8_t newLineDelay, uint16_t refreshDelay) {
    _charDelay = charDelay;
    _newLineDelay = newLineDelay;
    _refreshDelay = refreshDelay;
}

void Text::setLine(uint8_t line, String text) {
    if (line > MAX_TEXT_OBJECT_LINES) {
        Serial.println(F("Error: Encountered error trying to add string to line. Exceeded max length!"));
        return;
    }
    //_text[line].reserve(text.length());
    _text[line] = text;
}

void Text::print() {
    lcd.clear();

    int cursorIndex = 0; // Tracks the index of the vertical cursor.
    int longTextIndex = 0; // Tracks the index of text when it is to long.

    for (uint8_t i = 0; i < sizeof(_text) / sizeof(_text[0]); i++) {
        for (uint8_t j = 0; j < _text[i].length(); j++) {
            // If the text is longer then the cols can handle, begin scrolling.
            if (j > LCD_COLS) {
                if (longTextIndex % LCD_COLS == 0) {
                    lcd.setCursor(0, cursorIndex);
                    // Clear a specific line of the LCD.
                    for (int k = 0; k < LCD_COLS; k++) {
                        lcd.print(F(" "));
                    }
                    longTextIndex = 0;
                }
                lcd.setCursor(longTextIndex, cursorIndex);
                longTextIndex++;
            }
            lcd.write(_text[i][j]);
            delay(_charDelay);
        }

        longTextIndex = 0;
        ++cursorIndex;

        // We have reached the end of the LCD. Refresh everything.
        if (cursorIndex == LCD_ROWS) {
            cursorIndex = 0;
            delay((int)_refreshDelay);
            lcd.clear();
        }

        lcd.setCursor(0, cursorIndex);

        if (_text[i] == NULL) {
            delay((int)_refreshDelay);
            return;
        }
        delay(_newLineDelay);
    }
}

void Text::print_large() {
    lcd.clear();
    //char text[MAX_TEXT_OBJECT_LINES * 255];
    uint16_t indexCounter = 0;
    uint8_t cursor = 0;
    for (uint8_t i = 0; i < sizeof(_text) / sizeof(_text[0]); i++) {
        for (uint8_t k = 0; k < _text[i].length(); k++) {
            delay(_charDelay);
            if (!(k % LCD_COLS == 0 && _text[i][k] == ' ')) {
                lcd.write(_text[i][k]);
            }
            indexCounter++;
            if (indexCounter % LCD_COLS == 0) {
                cursor++;
                lcd.setCursor(0, cursor);
                delay(_newLineDelay);
            }
            if (indexCounter % (LCD_COLS * LCD_ROWS) == 0) {
                delay((int)_refreshDelay);
                cursor = 0;
                lcd.clear();
            }
        }
    }

}