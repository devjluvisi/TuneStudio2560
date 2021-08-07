/**
 * @file sd_card.h
 * @author Jacob LuVisi
 * @brief Manages the SD card controller in TuneStudio2560. Allows the creation, deletion and accessing of various files as
 * well as useful utility methods.
 * @version 0.1
 * @date 2021-07-17
 *
 * @copyright Copyright (c) 2021
 *
 */

#ifndef sd_card_h
#define sd_card_h

#include <Arduino.h>
#include <studio-libs/tune_studio.h>
#include <SD.h>
#include <SPI.h>

class Song;

class SDModule {
private:
    File _currentFile;
    uint8_t _csPin;
    uint16_t _filePointer;
    SdVolume _volume;
    Sd2Card _card;
    SdFile _root;
public:
    /**
     * @brief Initalize a new SD card object.
     *
     * @param csPin Which pin the "CS" pin on the SD module connects to the Arduino.
     */
    SDModule(uint8_t csPin);
    /**
     * @brief Print to the serial monitor a large portion of information about the
     * SD card. Only works if DEBUG is enabled.
     *
     */
    void print_info();
    /**
     * @brief Saves a song to the SD card. If the file already exists, then it is overriden.
     *
     * @param fileName The name of the song (name of the file) to be stored.
     * @param song The song object.
     */
    void save_song(const char* fileName, Song* song);
    /**
     * @brief Delete a file from the SD card.
     *
     * @param fileName The file to delete.
     */
    void delete_file(const char* fileName);
    /**
     * @brief Gets a song from the SD card.
     *
     * @param fileName The name of the song to locate.
     * @return A song to play.
     */
    Song get_song(const char* fileName);

    /**
     * @brief Get the nth file in the sd card.
     *
     * @param index The number to search for.
     * @return The name of the file at the specified index.
     */
    const char* get_file(uint8_t index);

};
#endif