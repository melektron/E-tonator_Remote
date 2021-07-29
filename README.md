# E-tonator_Remote

This is a repository containing all the code for my E-tonator project.

The E-tonator was originaly intended as a safe way and open source project to egnite fireworks from a large distance, as professional firework command stations are really expensive.
Since then it has evolved into a general purpose remote that can do much more than that. There is a menue with multiple different functions and modes to choose from and it can even play music. (Tetris game comming soon :) )

The first portotype version of the hardware is based on the ATmega328p microcontroler on an Arduino ProMini clone board.
As this is supposed to be a remote controll, it currently uses a simple 433 MHz UART sender/receiver board for communication with other devices such as a base station.

However, this way of communication is not very reliable and not good for large distances. Eventually I want to upgrade it to use a more suitable way of communication.dddddddddddddddddddddddddddddddddd
A more powerfull microcontroller such as an esp8266 / esp32 / stm32 would also be helpfull, altough I enjoy trying to write my code as efficientyl as possible to get it to fit into the 2k RAM of the ATmega328p.

The software side of the project is based on the Arduino Framework and library for ease of use, altough I am using VSCode+PlatformIO as my IDE.
The entire project folder can thus be imported as a PlatromIO project and should automatically download any library and framework dependencies needed.dd
