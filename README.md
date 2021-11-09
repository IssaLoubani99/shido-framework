![SHIDO Framework Poster](https://raw.githubusercontent.com/IssaLoubani99/shido-framework/main/images/title.jpg)
# SHIDO
The SHIDO project is dedicated to implement **shell** (terminal) on Arduino as well as a control environment capable of automated tasks offering a dynamic way to control Arduinos with a code that does not need to flashed each time or changing it.

## Features
The SHIDO project offers the ability to access arduino via a shell that is implemented using UART protocol. As well as offering a network of **controller-target** (master-slave) of arduinos that give an arduino controller the ability to controll and send commands to multiple arduinos targets.

## Development Process
This project was developped using arduino IDE for coding and proteus for testing.

## DEMO
A demonstration using proteus software.
![Demo](https://raw.githubusercontent.com/IssaLoubani99/shido-framework/main/images/embeded.jpg)

## Usage
The controller code is to be used on the controller arduino, the commands are sended via **i2c** protocol which requires that each target arduino have their own unique *ID*. Two codes are used for demonstration.

## Compiling
The code must be compiled using arduino IDE. The code uses the following libaries :

 - [FreeRTOS](https://www.arduino.cc/reference/en/libraries/freertos/) library
 - [Wire](https://www.arduino.cc/en/reference/wire) library for I2C communication
 - [Queue](https://www.arduino.cc/reference/en/libraries/queue/) library
 - [Arduino Timer](https://github.com/contrem/arduino-timer) by [Michael Contreras](https://github.com/contrem)
