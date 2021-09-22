/**
 * @file lm_playing_song.cpp
 * @author Jacob LuVisi
 * @brief The class for managing the current song being played. This state also uses the bottom row of the LCD to display different
 * controls on a varying interval.
 *
 * Note: The way that songs are played back is different then usual. This class does NOT use the song.cpp way of playing back songs
 * and instead it relies on a global index and the general loop() function instead of a seperate while loop.
 *
 * @version 0.1
 * @date 2021-07-26
 *
 * @copyright Copyright (c) 2021
 *
 */

#include <studio-libs/states/states.h>

ListeningModePlayingSong::ListeningModePlayingSong(): ProgramState::ProgramState(LM_PLAYING_SONG) {}
ListeningModePlayingSong::~ListeningModePlayingSong() {
  prgmSong.clear();
}

void ListeningModePlayingSong::loop() {
  // If an invalid song was selected then return back.
  if (invalidSong) {
    delay_ms(1000);
    update_state(LM_MENU);
    set_selected_page(1);
    set_selected_song(1);
    return;
  }

  // Check if the user is trying to delete their song.
  if(requestedDelete) {
    if(digitalReadFast(BTN_ADD_SELECT) == LOW) {
      // User has confirmed. Delete the file from the SD card.
      lcd.clear();
      lcd.setCursor(0, 1);
      lcd.print(F("Deleted."));
      lcd.setCursor(0, 2);
      lcd.print(sd_get_file(get_selected_song() - 1));
      delay_ms(2000);
      sd_rem(sd_get_file(get_selected_song() - 1));
      update_state(MAIN_MENU);
      set_selected_page(1);
      set_selected_song(1);
      delay_ms(500);
      return;
    }else if(digitalReadFast(BTN_DEL_CANCEL) == LOW) {
      delay_ms(850);
      lcd.clear();
      delay_ms(850);
      // User cancelled so we must run init() to regenerate the information on the display.
      init();
      return;
    }
    return;
  }

  // Update the bottom row of the LCD to display different controls.
  // Timed on an interval.
  if (millis() - lastTextUpdate > LM_BOTTOM_TEXT_DELAY_INTERVAL) {
    lcd_clear_row(3);
    switch (bottomTextMode) {
    case 0:
      lcd.print(F("Song Size: "));
      lcd.print(currentSongSize);
      break;
    case 1:
      lcd.print(F("Note Delay: "));
      lcd.print(prgmSong.get_note_delay());
      break;
    case 2:
      lcd.print(F("Note Length: "));
      lcd.print(prgmSong.get_note_length());
      break;
    case 3:
      lcd.print(F("Song #: "));
      lcd.print(get_selected_song());
      break;
    case 4:
      lcd.print(F("Page #: "));
      lcd.print(get_selected_page());
      break;
    }
    // Move to the next text mode.
    bottomTextMode++;
    if (bottomTextMode == 5) {
      bottomTextMode = 0;
    }
    lastTextUpdate = millis();
  }

  // When the user presses the add/select button.
  if (digitalReadFast(BTN_ADD_SELECT) == LOW) {
    // If the song is being played then pause it.
    if (currentSongNote < currentSongSize) {
      isPaused = !isPaused;
      // Update the paused indicator.
      lcd_clear_row(1);
      lcd.setCursor(1, 1);
      if (isPaused) {
        lcd.write(byte(PAUSE_SONG_SYMBOL));
        lcd.print(F(" NOW PAUSED "));
      } else {
        lcd.write(byte(PLAYING_SONG_SYMBOL));
        lcd.print(F(" NOW PLAYING "));
      }
      lcd.write(byte(MUSIC_NOTE_SYMBOL));
      delay_ms(200);
    } else {
      // If the song has finished then restart it.
      delay_ms(500);
      lcd.clear();
      lcd.setCursor(0, 1);
      lcd.print(F("Restarting!"));
      delay_ms(900);
      lcd.clear();
      init();
    }
  }

  // Forward/Backwards
  if (currentSongNote < currentSongSize) {
    if (isPaused) {
      if (digitalReadFast(BTN_TONE_1) == LOW) {
        // Forward
        currentSongNote++;
        delay_ms(100);
      } else if (digitalReadFast(BTN_TONE_2) == LOW) {
        // Backwards
        if (currentSongNote == 0) {
          return;
        }
        currentSongNote--;
        delay_ms(100);
        // NOTE: The progress bar needs to be reset because the instructions to update the progress bar usually do not 
        // account for a reduction in the block size. Therefore we need to regenerate the block size from zero.
        lcd.setCursor(strlen_P(PROGRESS_LABEL) + 1, 2);
        blockSize = blockRequirement;
        for (uint8_t i = 0; i < 8; i++) {
          if (currentSongNote >= blockSize) {
            // Set the new requirement.
            blockSize += blockRequirement;
            // Add a block to the progress.
            lcd.write(byte(PROGRESS_BLOCK_SYMBOL));
          } else {
            lcd.write(byte(PROGRESS_BLOCK_UNFILLED_SYMBOL));
          }
        }
      }
    }
    

  }
  // End

  // Deleting the song
  if (digitalReadFast(BTN_DEL_CANCEL) == LOW && digitalReadFast(BTN_OPTION) == LOW) {
    if(!requestedDelete) {
      #if DEBUG == true
      Serial.print(get_active_time());
      Serial.println(F(" User has requested to delete the current song."));
      #endif
      lcd.clear();
      lcd.print(F("DELETING:"));
      lcd.setCursor(0, 1);
      lcd.print(F(">> "));
      lcd.print(sd_get_file(get_selected_song() - 1));
      lcd.setCursor(0, 2);
      lcd.print(F("ARE YOU SURE? (Y/N)"));
      lcd.setCursor(0, 3);
      lcd.print(F("SELECT=Y, CANCEL=N"));
      delay_ms(1000);
      // Now that the LCD has text set to it, we set requestedDelete to true so when we return to the 
      // first part of the loop we know that we should be awaiting user input on whether or not to delete the song.
      requestedDelete = true;
      return;
    }
  }
  // User pressed Cancel but not option; Return to listening mode menu.
  else if (digitalReadFast(BTN_DEL_CANCEL) == LOW) {
    delay_ms(1000);
    update_state(LM_MENU);
    return;
  }
  // End

  /*
  [NOTE]
  This class uses a custom song playing method in order to play the individual notes from the song.
  This is required in order to make the pause, rewind/forward, and progress bar work properly.

  The playing of songs in this class relies entirely on the looping method of this class.
  Therefore, the performance of song playback is dependent on the performance of the code in this looping function.
  Code in this function must be optimized to a high level or else extra delays between notes can occur.

  Using DEBUG and PERF_METRIC modes can cause the notes to be delayed for a longer period of time.
  */
  if (!isPaused && currentSongNote < currentSongSize && millis() - lastTonePlay > prgmSong.get_note_delay()) {
    if (prgmSong.get_note(currentSongNote) != PAUSE_NOTE.frequency) {
      prgmSong.play_note(prgmSong.get_note(currentSongNote));
      delay_ms(prgmSong.get_note_length());
      noNewTone(SPEAKER_1);
      lastTonePlay = millis();
      currentSongNote++;
    } else {
      delay_ms(PAUSE_DELAY); // Delay the song from continuing for a certain amount of time.
      lastTonePlay = millis();
      currentSongNote++; // Go to the next index of the song.
    }

  } else if (currentSongNote >= currentSongSize) {
    lcd.setCursor(1, 1);
    lcd.write(byte(FINISHED_SONG_SYMBOL));
    lcd.print(F(" FINISHED SONG "));
    lcd.write(byte(MUSIC_NOTE_SYMBOL));
  }
  
    // If the current song note is past the requirement for the next block.
    if (currentSongNote >= blockSize) {
      // Set the cursor to a point on the LCD where the next block is to be inserted.
      #if PRGM_MODE == 0
      lcd.setCursor(strlen_P(PROGRESS_LABEL) + (blockSize / blockRequirement < MIN_SONG_LENGTH ? blockSize / blockRequirement : MIN_SONG_LENGTH), 2);
      #else
      lcd.setCursor(((float)strlen_P(PROGRESS_LABEL) + (blockSize / blockRequirement < MIN_SONG_LENGTH ? blockSize / blockRequirement : MIN_SONG_LENGTH)), 2);
      #endif
      // Set the new requirement.
      blockSize += blockRequirement;
      // Add a block to the progress.
      lcd.write(byte(PROGRESS_BLOCK_SYMBOL));
    }
  return;
}

void ListeningModePlayingSong::init() {
  #if DEBUG == true
  Serial.print(get_active_time());
  Serial.println(F(" Initalizing song player"));
  #endif
  currentSongNote = 0;
  bottomTextMode = 0;
  lastTextUpdate = 0;
  isPaused = false;
  invalidSong = false;
  lastTonePlay = 0;
  requestedDelete = false;

  const char * name = sd_get_file(get_selected_song() - 1);

  #if DEBUG == true
  Serial.print(F("Attempting to load song: \""));
  Serial.print(name);
  Serial.print(F("\" in song player."));
  #endif

  // If the name is invalid "" (Song file does not exist)
  if (!strlen(name)) {
    invalidSong = true;
  }

  lcd.print(F("[#"));
  lcd.print(get_selected_song());
  lcd.print(F("] "));

  if (!invalidSong) {
    char buffer[9];
    // Copy the song name to the buffer and remove the file extension.
    strncpy(buffer, name, strlen(name) - strlen(FILE_TXT_EXTENSION));
    buffer[strlen(name) - strlen(FILE_TXT_EXTENSION)] = '\0'; // Terminate "garbage" unneeded data.
    lcd.print(buffer);
  }

  lcd.setCursor(1, 1);
  lcd.write('#');
  lcd.print(F(" INITALIZING "));
  lcd.write(byte(MUSIC_NOTE_SYMBOL));

  lcd.setCursor(0, 2);
  lcd.print(F("PROGRESS: "));
  // Put unfilled progress blocks.
  for (uint8_t i = 0; i < 8; i++) {
    lcd.write(byte(PROGRESS_BLOCK_UNFILLED_SYMBOL));
  }

  if (invalidSong || sd_songcpy(name) == false) {
    lcd.clear();
    lcd.print(F("Invalid Song"));
    #if PRGM_MODE == 0
    digitalWriteFast(RGB_RED, HIGH);
    #else
    analogWrite(RGB_RED, RGB_BRIGHTNESS);
    #endif
    delay_ms(1000);
    #if PRGM_MODE == 0
    digitalWriteFast(RGB_RED, LOW);
    #else
    analogWrite(RGB_RED, 0);
    #endif
    invalidSong = true;
  }

  currentSongSize = prgmSong.get_size();
  // Seperate the progress bar into 8 different blocks.
  #if PRGM_MODE == 0
  blockRequirement = currentSongSize / 8;
  #else
  blockRequirement = (float)((float)currentSongSize / 8.0F);
  #endif

  blockSize = blockRequirement;
  

  delay_ms(750);

  if (!invalidSong) {
    lcd.setCursor(1, 1);
    lcd.write(byte(PLAYING_SONG_SYMBOL));
    lcd.print(F(" NOW PLAYING "));
    lcd.write(byte(MUSIC_NOTE_SYMBOL));
  }

}