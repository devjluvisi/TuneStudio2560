# TuneStudio2560
*An 8-Bit song creation and playback tool for ATMega2560 microcontroller.*

[Watch this Repo](https://github.com/devjluvisi/TuneStudio2560/subscription)  
Current Build: <span style="color: green;">**Passing**</span>

##### Summary
TuneStudio2560 is a song creation and playback device made for the Arduino Mega 2560 microcontroller. The program and hardware allow users to add "notes" in a linear fashion in order to create a basic 8-bit song. Once created, users can playback their songs via microSD as well as edit, create, and delete songs right on their personal computer using a microSD card!

Along with the hardware and Arduino code, TuneStudio2560 also provides a JavaScript client-side website called **MakerStudio2560** hosted on GitHub pages! This website allows users to create and playback songs in a more user-friendly way using a mouse and keyboard. When finished, songs can be downloaded and uploaded to the Arduino and vice versa.

TuneStudio2560 is a simple but heavily thought out project and the repository provides large portions of documentation for regular users and for interested developers.  
*Check out the list below for links to read and learn about TuneStudio2560*
> GitHub Wiki  
<a href="https://github.com/devjluvisi/TuneStudio2560/wiki/For-Users" target="_blank">**User Guide**</a>
<small>> User Guide for installing, controlling, making songs, playing songs back, and much more!</small>  
<a href="https://github.com/devjluvisi/TuneStudio2560/wiki/For-Developers" target="_blank">**Developer Guide**</a>
<small>> A brief code and design explanation for developers who are interested functionality.</small>  
<a href="https://github.com/devjluvisi/TuneStudio2560/wiki/Build-It" target="_blank">**Build Guide**</a>
<small>> Build TuneStudio2560 yourself!</small>

> Main Website  
<a href="https://devjluvisi.github.io/TuneStudio2560/" target="_blank">**Reference Website**</a>
<small>> The main website; information on changelogs and everything else you could need.</small>  
<a href="https://devjluvisi.github.io/TuneStudio2560/MakerStudio2560.html" target="_blank">**MakerStudio2560**</a>
<small>> A cross-compatible song creation tool for TuneStudio2560 made in JavaScript.</small>  
<a href="https://devjluvisi.github.io/TuneStudio2560/content/TuneStudio2560%20Product%20Brief.pdf" target="_blank">**Product Brief**</a>
<small>> An online PDF white paper containing a full explanation of TuneStudio2560 in both a high and low level.</small>  
<a href="https://devjluvisi.github.io/TuneStudio2560/doxy/html/index.html" target="_blank">**Doxygen Documentation**</a>
<small>> Direct doxygen-generated website code documentation from the source.</small>  

**[Check out the real life image library!](https://drive.google.com/drive/folders/1CkPubvIi6yJfRg2Uyx3KxVqNjCzE9Wjo)**  
<small>*See TuneStudio2560 fully assembled!*</small>

Current Program Space & RAM Requirements for **v1.3.0-R5 (latest)**
|      BUILD     |       SRAM          |        FLASH        |
| -------------- | ------------------- | ------------------- |
| **v1.3.0-R5**  | 1300 Bytes (15.9%)  | 29914 Bytes (11.8%) |
  
<small>Percentages based off Arduino Mega 256K Program Space & 8K RAM.</small>  

*View GitHub WIKI for a full table covering all releases*
## Features
- Ability to create different tunes using one of the five defined buttons and adjust their frequency utilizing the potentiometer.
- Uses the standard chromatic scale to identify different tones/pitches for ease of use.
- Song saving <u>persistence</u> using Micro SD Card.
- Playback previously saved songs that are saved in persistent storage.
- Ability to add songs and remove songs from SD.
- Create songs in **Creator Mode** and play each tune back one by one.
- Edit saved music tune files on a computer with a MicroSD card adapter.
- Text feedback on an LCD.
- RGB Led status indicator.
- Custom file parsing with microSD.
- Edit the tone delays and length of the song on your PC.
- 4 Digit 7 Segment display tells which tune is currently being selected.

**Song Storage & Editing w/ MicroSD**
TuneStudio2560 uses a microSD card module with an SPI interface to write and read song files. Of course, the microSD is completely compatible with modern machines using FAT32, meaning that you can plug it right into your computer and edit the data of your songs. The library is aided by custom file parsing functions which convert the text in each of the song files to program code. The custom microSD interface methods contain error checking and error-correcting to ensure ease of use when modifying your own songs.

## How it works
>A very brief overview on the hardware and user controls.   
> See: <a href="https://github.com/devjluvisi/TuneStudio2560/wiki/For-Users" target="_blank">User Guide</a>, <a href="https://github.com/devjluvisi/TuneStudio2560/wiki/For-Developers" target="_blank">Developer Guide</a> for more information.
>
### Controls & Buttons
> **Note:** *Sometimes, buttons may be used together to perform certain actions. Ex: Hold Select+Option to save the song.*  

**Tune Buttons**: Five buttons are used for adding different ranges of tunes to the song.  
**Frequency Adjust (Potentiometer)**: Used for adjusting the specific frequency of each of the five tune buttons.  
**Add/Select Button**: Used for navigation and adding tunes/confirming prompts.  
**Delete/Cancel Button**: Used for navigation and removing tunes/canceling prompts.  
**Option Button**: A button that is used with other buttons to perform certain tasks.  
**Status RGB**:  Indicates the current status of the Arduino. Each color represents a different action (See color lookup table).  

### Listening & Creator Mode
There are two different modes in TuneStudio2560 that can be accessed from the "Home" screen. Different modes allow users to either create songs or listen to saved songs.  

**Listening Mode** accesses the persistent storage and allows the user to listen to saved songs as well as delete old songs to make room for new ones. It also allows the user to pause and play through the song and has a song progress bar.  
**Creator Mode** allows the user to create their own songs using the tune buttons and frequency adjuster. Creator mode allows the adding/removing of tunes, adding pauses to a song, listening to a song without saving, as well as saving the song.  

### Tune Buttons & Potentiometer
Each of the five tune buttons has their own LED's which indicate if they are being pressed. The different tune buttons represent different ranges of frequency. A simple rule to remember is that the left-most tune button (Green) represents the lowest-pitched tones and the right-most tune button (White) represents the highest pitched tones.  
However, five tones are not nearly enough to make a highly customized song. For this, there is a potentiometer known as the **frequency adjuster**, which allows the fine-tuning of each of the tone buttons. Each tune button can play 17 distinct tones depending on the value of the frequency adjuster. In Creator Mode, the currently selected tune, chosen by the last tune button pressed and the value of the potentiometer, is displayed on the 4-digit 7-segment display.

### Displays, MicroSD, and Buzzer
**Liquid Crystal 20x4 Display**
A 20x4 Liquid Crystal display allows TuneStudio2560 to display messages and a simple text GUI to the user. The display tells the user messages on how to operate the device as well as lists of songs and other GUI functionality. The display communicates to the Arduino using the I2C protocol.

**4-Wide 7-Segment Display**
A four digit wide segment display allows the Arduino to display the value of the potentiometer to the Arduino. This is useful as someone may want to replicate a song but they may not know the exact values of the potentiometer. This display allows the precise adjustment of the potentiometer to achieve the desired frequency. The display is connected to the microcontroller by using two SN74HC595N shift registers. As a result, only three pins are needed to control the segment display. The library [SevSegShift](https://github.com/bridystone/SevSegShift) is used for easy communication between the display and the Arduino.

**MicroSD Card Reader Module**
> **Important**:  *The microSD card in use must be formatted using FAT32 in order to function properly. Custom names for songs must not exceed 8 characters long.*
>
A microSD card reader which uses the SPI interface allows for the convenient saving and retrieval of persistent data like songs.  
The microSD card saves one distinct file on the root directory for each song the user creates. The name of the file (8 characters max) specifies the name of the song.
> TuneStudio2560 utilizes the microSD card for storing songs as well as indexing them. Songs are stored in the root directory and are stored using a ".txt" file extension. Custom names cannot be over 8 characters long due to limitations with the Arduino SD library. 
> The ".txt" songs can be opened up using a text editor like Notepad++. When inside, they are user-readable, which allows editing on a PC. This also means that you can share your songs with others or even create songs on the microSD card without even having to touch the **Creator Mode** on the Arduino!  Make sure to view the **README.TXT** file about important information. This file is automatically generated on your SD card at launch.

**Piezo Active Buzzer**
An active piezo buzzer is used on the Arduino to playback songs and tunes. The speaker can only play one tune at a time but is quite good at 8-bit music. The speaker is fed frequency (integer) values stored in a lookup table in TuneStudio2560's program code. Each frequency value also corresponds with a human-readable pitch (like FS2/F#2).

## Hardware & Schematics
*Check out the wiki for a more in depth list of software libraries and hardware components.*
> Parts
- 1x Arduino Mega 2560 *(or compatible)* Microcontroller
- 1-2x Full-Size Breadboard
- 1x Passive Buzzer
- 5x LEDs
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

- Fritzing Diagram
![Diagram](https://i.imgur.com/xtUNJ9G.png)

**[Have these parts? Click here to learn how to build your own TuneStudio2560!](https://github.com/devjluvisi/TuneStudio2560/wiki/Build-It)**
## Quick Start & Usage Tutorial
<span style="color: #585ded;">**[ ! ]** The <u>[Reference Website](https://devjluvisi.github.io/TuneStudio2560/)</u> contains both images and video tutorials of using the TuneStudio2560. It is recommended to check it out if you are having trouble.</span>

For a more comprehensive guide with all tips & tricks check out [this wiki page](https://github.com/devjluvisi/TuneStudio2560/wiki/For-Users), which contains literally everything you would want to know as a user!  

1. Attach your Arduino Mega 2560 to the pins in the [build guide](https://github.com/devjluvisi/TuneStudio2560/wiki/Build-It).
2. Plug a FAT32 formatted microSD card into the SD Card module. It is fine to have extra files on the SD Card. However, ".txt" files should be in a separate directory to prevent TuneStudio from attempting to read them.
3. Clone this repository and open it using PlatformIO.
4. Upload the sketch to the Arduino. Ensure that the #DEBUG and #PERF_METRICS macros are "false" (find them in tune_studio.h).
5. Follow the instructions on the screen to view controls or check the wiki page.
6. If the RGB LED flashes RED, then check the Serial output (change #DEBUG to true) and troubleshoot your problems.
7. (OPTIONAL) Run the test case to make sure your hardware is connected properly.

## Installation & Modification using VS Code

TuneStudio2560 uses the [PlatformIO](https://platformio.org/) with Visual Studio Code as its build environment. As such any editing and requests must be made using PlatformIO and not other IDEs.  
For a complete tutorial and breakdown of the code as well as the design philosophy of TuneStudio2560, check out [this wiki page](https://github.com/devjluvisi/TuneStudio2560/wiki/For-Developers) or [this product brief](https://devjluvisi.github.io/TuneStudio2560/content/TuneStudio2560%20Product%20Brief.pdf) which goes into detail on the concepts of TuneStudio2560 and will help developers understand how to work with the code.  
Libraries that TuneStudio2560 uses are automatically downloaded by PlatformIO. In case they are not, libraries can also be viewed in the platform.ini file.

## Issues & Limitations
TuneStudio2560 is a very comprehensive program allowing users to create, listen, delete, and even edit songs. However, there are still some limitations with TuneStudio2560. 
- SD card file formatting is limited to FAT16/FAT32 only.
- Looping functions which require high performance to operate smoothly currently lag when using debugging features such as the Serial monitor.  
- The pause time is hardcoded and currently not configurable.
- Song playback is linear, and only one speaker can be connected to the Arduino (two notes can't play together at the same time).
- Users cannot configure the Tone Delay or Tone Length variables in Songs while using TuneStudio; adjusting these variables requires an external PC to edit the song file on the microSD.

As of current time, there are no known major bugs in TuneStudio2560. Any bugs can be posted on the GitHub or through direct contact with me.
## Future Plans
TuneStudio2560 has officially been fully released and finished (without bugs) as of v1.3.0-R5. There is a list of potential "TODO" items on the reference website.

In the future, there are two potential ports to other microcontrollers which can (theoretically) be done. 

> **Arduino Uno R3**  
If possible, a potential port to the ATMega328P microcontroller is being looked into when the project is finished for the current Mega2560. As of now, setting the TuneStudio2560 program to PRGM_MODE=0 and turning off the debug macro allows the program to fit both within the 2KB SRAM and 32K program space limitations of the Uno. An official port and build guide has not been completed yet.

> **Raspberry Pi Pico**  
Since I personally have a Raspberry Pi Pico on hand, a potential port to the RP2040 is possible, but ARM-compatible libraries will have to be found and used. Due to the RP2040's large SRAM space (264K) the entire program of TuneStudio2560 could fit into the RAM alone. A port of TuneStudio2560 to the pico has not been started yet but it should be possible provided the proper libraries can be used.

## Contributing & Suggestions
Extending TuneStudio2560 to other microcontrollers is still in consideration.  
For any software requests, then feel free to request a pull or email me. For any hardware requests, you can either fork your own or message me but further expansion of hardware is unlikely.  
*Possible future additions:*  
- Integrate potentiometer with a pause button to allow varying lengths of pause time instead of a hard-coded one.
- Allow users to edit the tone delays for their songs.
- Port TuneStudio2560 to other microcontrollers.

## Other Notes & Credit
Check out [Arduino](https://www.arduino.cc/) to build your own fun prototyping projects.  
Check out [PlatformIO](https://platformio.org/), which provides a convenient way to create new Arduino projects with VS Code.  

Make sure to view the <a href="https://devjluvisi.github.io/TuneStudio2560/" target="_blank">**Reference Website**</a> for more information regarding the project and for video tutorials + images!

TuneStudio2560 would not be possible without the open-source libraries that have been made publicly available for users of Arduino.

> [**Liquid Crystal I2C**](https://github.com/johnrickman/LiquidCrystal_I2C?utm_source=platformio&utm_medium=piohome)
Used for communication directly with the 20x4 LCD.  

> ~~[**SD** by Adafruit](https://github.com/adafruit/SD?utm_source=platformio&utm_medium=piohome)~~ (DEPRECATED)
Allows easy direct communication with the SD.

> [**SdFat** by greiman](https://github.com/greiman/SdFat)
Allows a more modern and fast interaction with a microSD card module.

> [**SevSegShift** by Dean Reading & Jens Breidenstien](https://github.com/bridystone/SevSegShift?utm_source=platformio&utm_medium=piohome)
A well-documented useful library for communicating with Common Cathode or Common Anode 7 segment displays using Shift Registers.  

> [**Unity** by Throw the Switch ](https://github.com/ThrowTheSwitch/Unity)
A very comprehensive test case library made for C.

## License
[TuneStudio2560 is licensed compltelty under the open source MIT License.](https://choosealicense.com/licenses/mit/)  
<small>
:white_check_mark:	Use TuneStudio2560 in Commercial Use.  
:white_check_mark:	Modify and distribute TuneStudio2560.  
:white_check_mark:	Modify and use TuneStudio2560 for private reasons.  
:o: Must include all copyright  headers.
</small>

*Yay for free open source software!*
## Contact
*Feel free to contact me with any questions. Email communication is preferred.*  
Email: <a href="mailto:webmaster@example.com">jacobluvisi@pm.me</a>  
LinkedIn: <a href="www.linkedin.com/in/jacob-luvisi-478a64215">www.linkedin.com/in/jacob-luvisi-478a64215</a>  
Hand Shake: <a href="https://sjsu.joinhandshake.com/users/33061777">https://sjsu.joinhandshake.com/users/33061777</a>
