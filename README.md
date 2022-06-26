# WeMos Hand Grip Game

## Get started with Arduino [D1/D1 mini series]

### Requirements

- [CH340 Driver](https://www.wemos.cc/en/latest/ch340_driver.html)
- [Python](https://www.python.org/downloads/)
- [Arduino IDE](https://www.arduino.cc/)

### Installing with Boards Manager

- Start Arduino and open the Preferences window.
- Enter ```https://arduino.esp8266.com/stable/package_esp8266com_index.json``` into the *File > Preferences > Additional Boards Manager URLs* field of the Arduino IDE. You can add multiple URLs, separating them with commas.
- Open Boards Manager from *Tools > Board* menu and install *esp8266* platform (and don't forget to select your ESP8266 board from Tools > Board menu after installation).

### Configure Board

After install hardware package, you will see LOLIN(WEMOS) D1 R2 & mini boards in the Tools→Board:xxx Choose your right board.

## Get started with TFT LCD 1.8

### Installing Essential Libraries

- Adafruit ST7735 and ST7789 Library

Enter essential libraries into the *Tools > Mange Libraries...* and install them

## Flowchart

```mermaid
    flowchart TD;
        Start([Start]) --> A1[Score = 0] --> A2[/Print Home Page/]
        A2 --> Loop{Is looping ?}
        ToLoop(( )) --> Loop
        Loop --> |True| GetAll1[/Get all input state/]
        GetAll1 --> WhichPage[Which page?]
        WhichPage --> HomePage{Case Home Page}
        HomePage --> |True| HomeP1{Is button input 1?}
        HomeP1 --> |True| HomeP1Act[Change page to Level 1 Page]
        HomeP1 --> |False| HomeP2{Is button input 2?}
        HomeP2 --> |True| HomeP2Act[Change page to Level 2 Page]
        HomeP2 --> |False| HomeP3{Is button input 3?}
        HomeP3 --> |True| HomeP3Act[Change page to Level 3 Page]
        HomeP1Act & HomeP2Act & HomeP3Act --> HomeP01[Score = 0] --> ToLoop
        HomePage --> |False| LevelPage{Case Level 1 or 2 or 3 Page}
        LevelPage --> |True| LevelLoop{Is Looped for 10 times?}
        LevelLoop --> |True| ToLoop
        LevelLoop --> |False| LevelLoop1[Wait depend on level] --> LevelLoop2[random = random number 1 to 4] --> LevelLoop3[Wait for random button press] --> LevelLoop
        LevelPage --> |False| ScorePage{Case Score Page}
        ScorePage --> |true| ScoreP1[/Print Score Page/] --> ScoreP2[/Wait for button 1 press/] --> ScoreP3[/Change page to Home Page/] --> ToLoop
        Loop --> |False| End([End])
```
