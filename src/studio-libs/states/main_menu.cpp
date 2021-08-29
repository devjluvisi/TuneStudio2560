/**
 * @file main_menu.cpp
 * @author Jacob LuVisi
 * @brief The state that represents the main menu. This state is run when the program first boots up.
 * @version 0.1
 * @date 2021-07-26
 *
 * @copyright Copyright (c) 2021
 *
 */
#include <studio-libs/states/states.h>

MainMenu::MainMenu() : ProgramState::ProgramState(MAIN_MENU) {}
MainMenu::~MainMenu() {}

void MainMenu::loop() {
    lcd.setCursor(1, 0);
    lcd.print(F("Welcome to..."));
    lcd.setCursor(2, 1);
    lcd.print(F("TuneStudio2560"));
    lcd.setCursor(0, 3);
    lcd.print(F("Ver: "));
    lcd.print((__FlashStringHelper*)VERSION_ID);
    delay_ms(4000);
    lcd.clear();
    lcd.setCursor(5, 1);
    lcd.print(F("By"));
    lcd.setCursor(2, 2);
    lcd.print(F("Jacob LuVisi"));
    delay_ms(2000);
    print_lcd(F("To enter creator mode press the select button. To enter listening mode press the delete button. To view more information please check out my github."));
    delay_ms(1000);
    lcd.clear();
    lcd.setCursor(0, 1);
    lcd.print(F("GitHub: "));
    print_scrolling(F("github.com/devjluvisi/TuneStudio2560"), 2, 235);
    delay_ms(2000);
    lcd.clear(); // Clear the screen incase the method needs to run again.
    delay_ms(1000);
}

void MainMenu::init() {
    return;
}