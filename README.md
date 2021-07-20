# TuneStudio2560
#### ⚠  As of this moment TuneStudio2560 is still in pre-development and this README *DOES NOT* represent the complete version of this project and is likely missing information.
TuneStudio2560 is a song creator and playback interface made for the Arduino Mega 2560 microcontroller. The program and hardware allow users to create their own "tunes" in order to make a simple song by playing the noises from a passive buzzer connected to the microcontroller.

The type of the tune is determined by both which "tune button" is pressed and how much a potentiometer is turned. Users can also save their songs on a persistent form of data storage and access those songs later and play them back! 

#### *For best compatibility it is recommended to open and upload the project using the [PlatformIO](https://platformio.org/) extension for [Visual Studio Code](https://code.visualstudio.com/).*

## Features
- Ability to create different tunes using one of the five defined buttons and adjust their frequency using the potentiometer.
- Song saving persistence using Micro SD Card or EEPROM.
- Play back previously saved songs which are saved in persistent storage.
- Ability to add songs, remove songs, add tunes in songs and remove tunes from songs.
- Delete all saved songs and start new.
- Play each tune separately.
- Edit saved music tune files on a computer with a MicroSD card adapter.
- Text feed back on an LCD.
- RGB Led status indicator.

**Song Storage & Editing w/ MicroSD & EEPROM**  
TuneStudio2560 adds an optional microSD card module which can be added to the breadboard to allow seamless saving of songs and data. Although not required, the MicroSD card is heavily recommended in comparison to EEPROM due to larger storage spaces especially for any potential future Arduino Uno ports.

*A microSD card module is also required for any potential port to the RP2040 microcontroller due to its lack of EEPROM.*

| Feature        | EEPROM               | MicroSD                               |
| -----------    | -------------------- | ------------------------------------- |
| Persistent        |   :white_check_mark:              | :white_check_mark:    |
| Save/Delete Songs| :white_check_mark:                 | :white_check_mark:    |
| Play Saved Songs        | :white_check_mark:          | :white_check_mark:    |
| Less Pins | :white_check_mark:                        | :no_entry: |
| Utilizes SPI Bus        | :no_entry:                | :white_check_mark: |
| Edit Saved Songs| :no_entry:                 | :white_check_mark:|
| Upgradeable Storage      | :no_entry:                | :white_check_mark: |
| Long Endurance| :no_entry:                 | :white_check_mark: |
| Replaceable| :no_entry:                 | :white_check_mark: |
| Name Saved Songs        | :no_entry:                | :white_check_mark: |
| Can Upload Songs | :no_entry:                 | :white_check_mark: |

## How it works
>A hardware and software overview.
>
### Controls & Buttons
> **Note:** *Sometimes buttons may be used in together to perform certain actions. Ex: Hold Select+Option to save the song.*  

**Tune Buttons**: Five buttons used for adding different ranges of tunes to the song.  
**Frequency Adjust (Potentiometer)**: Used for adjusting the specific frequency of each of the five tune buttons.  
**Add/Select Button**: Used for confirming prompts or adding tunes.  
**Delete/Cancel Button**: Used for denying prompts, removing notes, or exiting the current menu.  
**Option Button**: A button which is used with other buttons to do certain tasks.  
**Status RGB**:  Indicates the current status of the Arduino. Each color represents a different action (See color lookup table).  

### Listening & Creator Mode
There are two different modes in TuneStudio2560 which can be accessed from the "Home" screen. Different modes allow users to either create songs or listen to saved songs. Modes can be switched between at any time by pressing the DEL/CANCEL button.  

**Listening Mode** accesses the persistent storage and allows the user to listen to saved songs as well as delete old songs to make room for new ones. Also allows the user to pause and play through the song and has a song progress bar.  
**Creator Mode** allows the user to create their own songs using the tune buttons and frequency adjuster. Creator mode allows the adding/removing of tunes, adding pauses to a song, listening to a song without saving, as well as saving the song.  

### Tune Buttons & Potentiometer
Each of the five tune buttons have their own LED's which indicate if they are being pressed. The different tune buttons represent different ranges of frequency. A simple rule to remember is that the left-most tune button (Green) represents the lowest pitched tones and the right-most tune button (White) represents the highest pitched tones.  
However, five tones is not nearly enough to make a highly customized song. For this, there is a potentiometer known as the **frequency adjuster** which allows the fine tuning of each of the tone buttons.

### Displays & MicroSD
**Liquid Crystal 20x4 Display**
A 20x4 Liquid Crystal display allows TuneStudio2560 to display messages and a simple text GUI to the user. The display tells the user messages on how to operate the device as well as lists of songs and other GUI functionality. The display communicates to the Arduino using the $I^2C$ bus.

**4-Wide 7-Segment Display**
A four digit wide segment display allows the Arduino to display the value of the potentiometer to the Arduino. This is useful as someone may want to replicate a song but they may not know the exact values of the potentiometer. This display allows the precise adjustment of the potentiometer to achieve the desired frequency. The display is connected to the microcontroller by using two SN74HC595N shift registers. As a result, only three pins are needed to control the segment display.

**MicroSD Card Reader Module**
> **Note**:  *The microSD card in use must be formatted using FAT32 in order to function properly. Custom names for songs must not exceed 8 characters long.*
>
A optional* microSD card reader which uses the SPI interface can be added to the project to allow the convenient storage of persistent data like songs. If no microSD card is used then TuneStudio2560 defaults to using the built-in EEPROM. A microSD interface is **highly recommended** due to the fact that the EEPROM can wear out overtime on the Arduino (~100,000 write cycles). A microSD card also allows a much larger storage space for songs, adding custom names to songs, as well as the ability to edit the songs on your personal computer!
> TuneStudio2560 utilizes the microSD card for storing songs as well as indexing them. Songs are stored in a /music directory and are stored using a ".bin" file extension. Custom names cannot be over 8 characters long due to limitations with the Arduino SD library. 
> The ".bin" songs can be opened up using a text editor like Notepad++. When inside, they are user-readable which allows editing on a PC. This also means that you can share your songs with others or even create songs in the microSD card without even having to touch the **Creator Mode** on the Arduino!

## Hardware & Schematics
*Check out the wiki for a more in depth list of software libraries and hardware components.*
> Parts
- 1x Arduino Mega 2560 *(or compatible)* Microcontroller
- 1-2x Full Size Breadboard
- 1x Passive Buzzer
- 5x LEDS
  - *Green, Blue, Red, Yellow, and White.*
- 1x RGB LED
- 8x 220Ω Resistors
- 4x 330Ω Resistors
- 1x 10K Potentiometer
- 8x Standard Switch Buttons
- 1x 20x4 Liquid Crystal display *(I2C Protocol)*
- 1x MicroSD Card Adapter w/ SPI Interface *(optional*)*
- 2x SN74HC595N Shift Register
- 1x 7-Segment Display (4 Wide) (Common-Cathode)
- A lot of wires.
> Schematics
- (In Progress)

## Future Plans
None of the future plans for TuneStudio2560 have been finalized as the product is not finished and is still in development going under major changes in both hardware and software.

However, there are potential ports to two microcontrollers. 

> **Arduino Uno R3**  
If possible, a potential port to the ATMega328P microcontroller is being looked into when the project is finished for the current Mega2560. Due to the ATMega328P's limited SRAM, EEPROM, and lack of IO pins it may be impossible to port a final version of TuneStudio2560 but a potential "lite" version could be ported with less features eventually.

> **Raspberry Pi Pico**  
Since I personally have a Raspberry Pi Pico on hand, a potential port to the RP2040 is possible but ARM compatible libraries will have to be found and used. Due to the RP2040's larger SRAM and flash memory it will have the ability to store larger songs in memory but likely will not have any major additional features as it is just a port. However, due to a lack of EEPROM, any potential port to the RP2040 would be required to utilize the microSD card module which is optional for the Mega/Uno.

## Usage Tutorial

(In Progress)

## Installation & Modification using VS Code

(In Progress)

## Contributing & Suggestions
Extending TuneStudio2560 to other microcontrollers is still in consideration.  
For any software requests then feel free to request a pull. For any hardware requests you can either fork your own or message me but further expansion of hardware is unlikely.
## Other Notes
Check out [Arduino](https://www.arduino.cc/) to build your own fun prototyping projects.  
Check out [PlatformIO](https://platformio.org/) which provides a convenient way to create new Arduino projects with VS Code.  

## License
[MIT](https://choosealicense.com/licenses/mit/)

*Yay for free open source software!*