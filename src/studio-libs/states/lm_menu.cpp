/**
 * @file lm_menu.cpp
 * @author Jacob LuVisi
 * @brief The class for managing the menu in listening mode.
 * @version 0.1
 * @date 2021-07-26
 *
 * @copyright Copyright (c) 2021
 *
 */

#include <studio-libs/states/states.h>

ListeningModeMenu::ListeningModeMenu(): ProgramState::ProgramState(LM_MENU) {}
ListeningModeMenu::~ListeningModeMenu() {}

void ListeningModeMenu::loop() {
  
  if (previousSong != get_selected_song()) {
    const char * name = sd_get_file(get_selected_song() - 1);
    lcd_clear_row(1);
    lcd.print(F(">> Name: "));
    lcd.setCursor(8, 1);
    if(strlen(name)) {
      lcd.print(name);
    }else{
      lcd.print(F("NONE"));
    }
    lcd_clear_row(2);
    lcd.print(F(">> Song #"));
    lcd.print(get_selected_song());
    lcd.print(F("/("));
    lcd.print(get_selected_page());
    lcd.print(F(")"));
    previousSong = get_selected_song();
  }
  const uint8_t indexer = !digitalReadFast(BTN_TONE_1) ? 1 : !digitalReadFast(BTN_TONE_2) ? 2 : !digitalReadFast(BTN_TONE_3) ? 3 : !digitalReadFast(BTN_TONE_4) ? 4 : !digitalReadFast(BTN_TONE_5) ? 5 : 0;
  set_selected_song(indexer == 0 ? previousSong : ((get_selected_page() - 1) * 5) + indexer);
  if(is_pressed(BTN_OPTION)) {
    set_selected_page(get_selected_page() + 1 > (MAX_SONG_AMOUNT / 5) ? 0 : get_selected_page() + 1);
    set_selected_song(((get_selected_page() - 1) * 5) + 1);
    previousSong = -1;
  }
}

void ListeningModeMenu::init() {
  #if DEBUG == true
  Serial.print(get_active_time());
  Serial.println(F(" Entered listening mode."));
  #endif
  previousSong = 0;
  delay_ms(500);
  lcd.setCursor(2, 1);
  lcd.print(F("[Listening Mode]"));
  lcd.setCursor(1, 2);
  lcd.print(F(">> INSTRUCTIONS <<"));
  delay_ms(1250);
  lcd.clear();
  print_lcd(F("Press select button to skip instructions."), 50);
  delay_ms(500);

  print_lcd(F(
    "Select 1 of the 5 tune buttons to play a song saved in memory.\n"
    "When using microSD, press the \"OPTION\" button to cycle to the next page of songs. Each page is 5 different songs.\n"
    "Press the \"DEL/CANCEL\" button to go back to main menu.\n"
    "While listening, press \"SELECT\" to pause song.\n"
    "While paused, press Green Tone to go back, Blue Tone to go forward, and SELECT to restart after a song is finished.\n"
    "While listening, press \"OPTION+DEL\" to delete song."));

  //TODO: Possibly add instruction for OPTION+SELECT to edit a saved song.
  delay_ms(1500);
  lcd.clear();
  lcd.setCursor(0, 3);
  lcd.print(F("Press SELECT to play"));
  lcd.setCursor(2, 0);
  lcd.print(F("[Listening Mode]"));
}