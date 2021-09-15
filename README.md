# TuneStudio2560
*For ATMega2560 microcontroller.*

![v1.2.1-R3](https://github.com/ntkme/github-buttons/workflows/build/badge.svg)
[watch this repo](https://github.com/devjluvisi/TuneStudio2560/subscription)

TuneStudio2560 is a song creation and playback device made for the Arduino Mega 2560 microcontroller. The program and hardware allow users to add tunes in succession in order to make a simple song. Once created, users can playback their songs via microSD as well as edit, create, and delete songs right on their personal computer!

The type of the tune is determined by both which "tune button" is pressed and how much a potentiometer is turned. Once a song has been saved, users can playback their songs in **Listening Mode** which incorporates pausing, fast forwarding, and rewinding functionalities.

[User Guide](https://github.com/devjluvisi/TuneStudio2560/wiki/For-Users)  
[Developer Guide](https://github.com/devjluvisi/TuneStudio2560/wiki/For-Developers)  
[Reference Website](https://devjluvisi.github.io/TuneStudio2560/)

**[Check out the real life image library!](https://drive.google.com/drive/folders/1CkPubvIi6yJfRg2Uyx3KxVqNjCzE9Wjo)**

#### *For best compatibility it is recommended to open and upload the project using the [PlatformIO](https://platformio.org/) extension for [Visual Studio Code](https://code.visualstudio.com/).*  

**Program Resource Utilization History**  
*Percentages are based off of the Arduino Mega's 8KB of SRAM and 256KB of flash memory using PRGM_MODE=0*  
|      BUILD     |       SRAM          |        FLASH        |
| -------------- | ------------------- | ------------------- |
| **V1.0.0-R1**  | 1705 Bytes (20.8%)  | 37436 Bytes (14.7%) |
|   V1.0.1-R1    | 1705 Bytes (20.8%)  | 37272 Bytes (14.7%) |
|   V1.0.2-R1    | 1689 Bytes (20.6%)  | 35446 Bytes (14.0%) |
|   V1.0.3-R1    | 1689 Bytes (20.6%)  | 35046 Bytes (13.8%) |
|   V1.0.4-R1    | 1689 Bytes (20.6%)  | 34468 Bytes (13.6%) |
|   V1.0.5-R1    | 1689 Bytes (20.6%)  | 34346 Bytes (13.5%) |
| **V1.1.0-R2**  | 1664 Bytes (20.3%)  | 32386 Bytes (12.8%) |
| **V1.2.0-R3**  | 1511 Bytes (18.4%)  | 29906 Bytes (11.8%) |
|   V1.2.1-R3    | 1300 Bytes (15.9%)  | 29974 Bytes (11.8%) |

## Features
- Ability to create different tunes using one of the five defined buttons and adjust their frequency using the potentiometer.
- Uses the standard chromatic scale to identify different tones/pitches for ease of use.
- Song saving persistence using Micro SD Card.
- Play back previously saved songs which are saved in persistent storage.
- Ability to add songs, remove songs, add tunes in songs and remove tunes from songs.
- Delete all saved songs and start new.
- Play each tune separately in Creator Mode without having to add it.
- Edit saved music tune files on a computer with a MicroSD card adapter.
- Text feed back on an LCD.
- RGB Led status indicator.
- Custom file parsing with microSD.
- Edit songs on your own PC using a text editor.
- Edit the tone delays and length of the tones on your PC.
- 4 Digit 7 Segment display tells which tune is currently being selected.

**Song Storage & Editing w/ MicroSD**
TuneStudio2560 uses a microSD card module which can be added to the breadboard to allow seamless saving of songs and data. The microSD card interfaces with the SD library to write songs and data to files as well as to read files. Of course, the microSD is completely compatible with modern machines using FAT32 meaning that you can plug it right into your computer and edit the data of your songs. The library is aided by custom file parsing functions which convert the text in each of the song files to program code. The custom microSD interface methods contain error checking and error correcting to ensure ease of use when modifying your own songs.

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
However, five tones is not nearly enough to make a highly customized song. For this, there is a potentiometer known as the **frequency adjuster** which allows the fine tuning of each of the tone buttons. Each tune button can play 17 distinct tones depending on the value of the frequency adjuster.  

### Displays, MicroSD and Buzzer
**Liquid Crystal 20x4 Display**
A 20x4 Liquid Crystal display allows TuneStudio2560 to display messages and a simple text GUI to the user. The display tells the user messages on how to operate the device as well as lists of songs and other GUI functionality. The display communicates to the Arduino using the I2C bus.

**4-Wide 7-Segment Display**
A four digit wide segment display allows the Arduino to display the value of the potentiometer to the Arduino. This is useful as someone may want to replicate a song but they may not know the exact values of the potentiometer. This display allows the precise adjustment of the potentiometer to achieve the desired frequency. The display is connected to the microcontroller by using two SN74HC595N shift registers. As a result, only three pins are needed to control the segment display. The library [SevSegShift](https://github.com/bridystone/SevSegShift) is used for easy communication between the display and the Arduino.

**MicroSD Card Reader Module**
> **Important**:  *The microSD card in use must be formatted using FAT32 in order to function properly. Custom names for songs must not exceed 8 characters long.*
>
A microSD card reader which uses the SPI interface can be added to the project to allow the convenient storage of persistent data like songs. A microSD card also allows a much larger storage space for songs then an EEPROM would. MicroSD also allows users to add custom names to songs, as well as the ability to edit the songs on your personal computer!
> TuneStudio2560 utilizes the microSD card for storing songs as well as indexing them. Songs are stored in the root directory and are stored using a ".txt" file extension. Custom names cannot be over 8 characters long due to limitations with the Arduino SD library. 
> The ".txt" songs can be opened up using a text editor like Notepad++. When inside, they are user-readable which allows editing on a PC. This also means that you can share your songs with others or even create songs in the microSD card without even having to touch the **Creator Mode** on the Arduino!  Make sure to view the **README.TXT** file about important information. This file is automatically generated on your SD card at launch.

**Piezo Active Buzzer**
An active piezo buzzer is used on the Arduino to playback songs and tunes. The speaker can only play one tune at a time but is quite good at 8 bit music. The speaker is fed frequency values stored in a lookup table in TuneStudio2560's program code. Each frequency value the speaker plays is assigned to a specified human readable "pitch" (like FS2/F#2).

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
- 1x MicroSD Card Adapter w/ SPI Interface
- 2x SN74HC595N *(or compatible)* Shift Registers
- 1x 7-Segment Display (4 Wide) (Common-Cathode)
- A lot of wires.
> Schematics
- Electrical Schematic [Still WIP]
![Schematic](https://i.imgur.com/tzxPLCZ.png)  

- Fritizing Diagram
![Diagram](https://i.imgur.com/xtUNJ9G.png)

**[Have these parts? Click here to learn how to build your own TuneStudio2560!](https://github.com/devjluvisi/TuneStudio2560/wiki/Build-It)**
## Quick Start & Usage Tutorial
For a more comprehensive guide with all tips & tricks check out [this wiki page](https://github.com/devjluvisi/TuneStudio2560/wiki/For-Users) which contains literally everything you would want to know as a user!  

1. Attach your Arduino Mega 2560 to the pins in the [build guide](https://github.com/devjluvisi/TuneStudio2560/wiki/Build-It).
2. Plug a FAT32 formatted microSD card into the SD Card module. It is fine to have extra files on the SD Card. However, .txt files should be in a seperate directory to prevent TuneStudio from attempting to read them.
3. Clone this repository and open it using PlatformIO.
4. Upload the sketch to the Arduino. Ensure that the #DEBUG and #PERF_METRICS macros are "false" (find them in tune_studio.h).
5. Follow the instructions on screen to view controls or check the wiki page.
6. If the RGB LED flashes RED then check the Serial output (change #DEBUG to true) and troubleshoot your problems.
7. (OPTIONAL) Run the test case to make sure your hardware is connected properly.

## Installation & Modification using VS Code

TuneStudio2560 uses the [PlatformIO](https://platformio.org/) with VS Code as its build environment. As such any editing and requests must be made using PlatformIO and not other IDEs.  
For a complete tutorial and breakdown of the code as well as the design philosophy of TuneStudio2560, check out [this wiki page](https://github.com/devjluvisi/TuneStudio2560/wiki/For-Developers) which goes into detail on the concepts of TuneStudio2560 and will help developers understand how to work with the code.  
Libraries that TuneStudio2560 uses are automatically downloaded by PlatformIO. In case they are not, libraries can also be viewed in the platform.ini file.

## Issues & Limitations
TuneStudio2560 is a very comprehensive program allowing users to create, listen, delete, and even edit songs. However there are still some limitations with TuneStudio2560. 
- SD card file formatting limited to FAT16/FAT32 only.
- SD library used is archived. Library should be ported to SdFat for more modern features and performance.
- Code is currently not compact and efficient enough to operate on the Arduino Uno.
- Looping functions which require high performance to operate smoothly currently lag when using debugging features such as the Serial monitor.  
- Users cannot edit their songs in TuneStudio2560, editing songs requires a PC to plug the microSD card into.
- The pause time is hardcoded and currently not configurable.
- Users cannot configure the Tone Delay or Tone Length variables in Songs while using TuneStudio, adjusting these variables requires an external PC to edit the song file on the microSD.

As of currently time there are no known major bugs in TuneStudio2560. Any bugs can be posted on the GitHub or through direct contact to me.
## Future Plans
None of the future plans for TuneStudio2560 have been finalized as the product is not finished and is still in development going under major changes in both hardware and software.

However, there are potential ports to two microcontrollers. 

> **Arduino Uno R3**  
If possible, a potential port to the ATMega328P microcontroller is being looked into when the project is finished for the current Mega2560. Due to the ATMega328P's limited SRAM, EEPROM, and lack of IO pins it may be impossible to port a final version of TuneStudio2560 but a potential "lite" version could be ported with less features eventually.

> **Raspberry Pi Pico**  
Since I personally have a Raspberry Pi Pico on hand, a potential port to the RP2040 is possible but ARM compatible libraries will have to be found and used. Due to the RP2040's larger SRAM and flash memory it will have the ability to store larger songs in memory but likely will not have any major additional features as it is just a port. However, due to a lack of EEPROM, any potential port to the RP2040 would be required to utilize the microSD card module which is optional for the Mega/Uno.

## Contributing & Suggestions
Extending TuneStudio2560 to other microcontrollers is still in consideration.  
For any software requests then feel free to request a pull. For any hardware requests you can either fork your own or message me but further expansion of hardware is unlikely.  
*Possible future additions:*  
- Integrate potentiometer with pause button to allow varying lengths of pause time instead of a hard coded one.
- Allow users to edit the tone delays for their own songs.
- Port TuneStudio2560 to other microcontrollers.
- Attempt to fit the SD card library on the Arduino Uno's limited SRAM and Flash. Currently, the SD library takes up far too much space
to port the SD functionallity of TuneStudio2560.
## Other Notes & Credit
Check out [Arduino](https://www.arduino.cc/) to build your own fun prototyping projects.  
Check out [PlatformIO](https://platformio.org/) which provides a convenient way to create new Arduino projects with VS Code.  

TuneStudio2560 would not be possible without the open source libraries that have been made publicly available for users of Arduino.

> [**Liquid Crystal I2C**](https://github.com/johnrickman/LiquidCrystal_I2C?utm_source=platformio&utm_medium=piohome)
Used for communication directly with the 20x4 LCD.  

> ~~[**SD** by Adafruit](https://github.com/adafruit/SD?utm_source=platformio&utm_medium=piohome)~~ (DEPRECATED)
Allows easy direct communication with the SD.

> [**SdFat** by greiman](https://github.com/greiman/SdFat)
Allows a more modern and fast interaction with a microSD card module.

> [**SevSegShift** by Dean Reading & Jens Breidenstien](https://github.com/bridystone/SevSegShift?utm_source=platformio&utm_medium=piohome)
A well documented useful library for communicating with Common Cathode or Common Anode 7 segment displays using Shift Registers.  

> [**Unity** by Throw the Switch ](https://github.com/ThrowTheSwitch/Unity)
A very comprehensive test case library made for C.

## License
[MIT](https://choosealicense.com/licenses/mit/)
*Yay for free open source software!*
### Contact
*Feel free to contact me with any questions. Email communication is preferred.*  
Email: jluvisi2021@gmail.com  
Phone: (+1) 310-465-8872  
