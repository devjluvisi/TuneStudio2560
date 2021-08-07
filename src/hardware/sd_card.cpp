/**
 * @file sd_card.cpp
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2021-07-31
 *
 * @copyright Copyright (c) 2021
 *
 */
#include <hardware/sd_card.h>

SDModule::SDModule(uint8_t csPin) {
    _csPin = csPin;
#if DEBUG == true
    Serial.println(F("Initalizing SD card module."));
#endif


    if (!_card.init(SPI_HALF_SPEED, _csPin)) {
#if DEBUG == true
        Serial.println();
        Serial.println(F("[CRITICAL] SD module cannot be initalized due to one or more problems."));
        Serial.println(F("* Is the card properly inserted?"));
        Serial.println(F("* Is the card correctly wired to the board?"));
        Serial.println(F("* Did you change the CS pin to match your board?"));
        Serial.println(F("* Have you verified the card is working on a PC?"));
        Serial.println(F("View https://github.com/devjluvisi/TuneStudio2560/wiki/For-Developers for more information."));
        Serial.println();
#endif
        // Stop the program forever.
        while (true) {
            analogWrite(RGB_BLUE, 0);
            analogWrite(RGB_GREEN, 0);
            analogWrite(RGB_RED, RGB_BRIGHTNESS);
            delay(500);
            analogWrite(RGB_RED, 0);
            delay(500);
        }
    }
    if (!_volume.init(_card)) {
#if DEBUG == true
        Serial.println();
        Serial.println(F("[CRITICAL] SD module cannot be initalized due to one or more problems."));
        Serial.println(F("* Incorrect formatting. The format for the microSD must be either FAT16 or FAT32."));
        Serial.println();
#endif
        while (true) {
            analogWrite(RGB_BLUE, 0);
            analogWrite(RGB_GREEN, 0);
            analogWrite(RGB_RED, RGB_BRIGHTNESS);
            delay(500);
            analogWrite(RGB_RED, 0);
            delay(500);
        }
    }
#if DEBUG == true
    Serial.println(F("SD Module has been initalized."));
#endif
    //print_info();
}

void SDModule::print_info() {
#if DEBUG == true
    Serial.print(F("SD Card Information connected to pin: "));
    Serial.println(_csPin);
    Serial.print(F("Card Type: "));
    switch (_card.type()) {
    case SD_CARD_TYPE_SD1:
        Serial.println(F("SD1"));
        break;
    case SD_CARD_TYPE_SD2:
        Serial.println(F("SD2"));
        break;
    case SD_CARD_TYPE_SDHC:
        Serial.println(F("SDHC"));
        break;
    default:
        Serial.println(F("Unknown."));
    }
    Serial.print(F("Volume Type: FAT"));
    Serial.println(_volume.fatType(), DEC);
    uint32_t volumesize;
    volumesize = _volume.blocksPerCluster();    // clusters are collections of blocks
    volumesize *= _volume.clusterCount();       // we'll have a lot of clusters
    volumesize /= 2;                           // SD card blocks are always 512 bytes (2 blocks are 1KB)

    Serial.print(F("Volume size (Kb):  "));
    Serial.println(volumesize);
    Serial.print(F("Volume size (Mb):  "));
    volumesize /= 1024;
    Serial.println(volumesize);
    Serial.print(F("Volume size (Gb):  "));
    Serial.println((float)volumesize / 1024.0);
    Serial.println();
    Serial.println(F("Files found on the card (name, date and size in bytes):"));
    _root.openRoot(_volume);
    // list all files in the card with date and size
    _root.ls(LS_R | LS_DATE | LS_SIZE);
    _root.close();
#endif
}

void SDModule::save_song(const char* fileName, Song* song) {
    if (!SD.begin(SD_CS_PIN)) {
#if DEBUG == true
        Serial.println(F("[CRITICAL] SD module cannot be initalized due to one or more problems."));
        Serial.println(F("* Is the card properly inserted?"));
        Serial.println(F("* Is the card correctly wired to the board?"));
        Serial.println(F("* Did you change the CS pin to match your board?"));
        Serial.println(F("* Have you verified the card is working on a PC?"));
        Serial.println(F("* Is the format for the microSD either FAT16 or FAT32?"));
        Serial.println(F("View https://github.com/devjluvisi/TuneStudio2560/wiki/For-Developers for more information."));
#endif
        while (true) {
            analogWrite(RGB_BLUE, 0);
            analogWrite(RGB_GREEN, 0);
            analogWrite(RGB_RED, RGB_BRIGHTNESS);
            delay(500);
            analogWrite(RGB_RED, 0);
            delay(500);
        }
    }
    File songFile;
    if (SD.exists(fileName)) {
#if DEBUG == true
        Serial.print(fileName);
        Serial.println(F(" already exists. Overwriting..."));
#endif
        // Delete the old file.
        SD.remove(fileName);
    }
    else {
#if DEBUG == true
        Serial.print(F("Creating "));
        Serial.println(fileName);
#endif
    }
    songFile = SD.open(fileName, FILE_WRITE);

    songFile.println(F("# Welcome to a song file!"));
    songFile.println(F("# To view more information, check out https://github.com/devjluvisi/TuneStudio2560/wiki/For-Users"));
    songFile.println(F(""));
    songFile.println(F("Data:"));

    for (song_size_t i = 0; i < song->get_size(); i++) {

        const char* pitch = get_note_from_freq(song->get_note(i)).pitch;

        songFile.print(F("  - "));
        songFile.println(pitch);

    }
    songFile.println(F(""));
    songFile.println(F("# END"));
    songFile.close();
#if DEBUG == true
    Serial.println(F("Finished writing with SD Card."));
#endif
}

const char* SDModule::get_file(uint8_t index) {
    File baseDir = SD.open("/");
    uint8_t count = 0;
    while (true) {
        File entry = baseDir.openNextFile();
        //SD.root();
        if (!entry) {
            baseDir.rewindDirectory();
            entry.close();
            Serial.println("Entry Terminated, not found. Count: " + String(count));
            return "";
        }
        // Ignore directories
        if (entry.isDirectory()) {
            Serial.println(F("Encountered Directory."));
            continue;
        }
        if (count == index) {
            entry.close();
            return entry.name();
        }
        count++;
    }
    Serial.println(F("End of method reached. Entry terminated."));
    //entry.close();
    return "";
}