---
layout: schede
title: DomusBot
description: Control the status of your home with your smartphone
img: 
---

<iframe src="https://docs.google.com/presentation/d/1rR92ojpLLrXuvdnftXga4MrEzMEllcMQl8Jj6vqTAtY/embed?start=false&loop=false&delayms=3000" frameborder="0" width="800" height="480" allowfullscreen="true" mozallowfullscreen="true" webkitallowfullscreen="true"></iframe>


#### MEMBERS:
Luca Paschetta, Marco Delmastro, Daniele Lo Rillo, Alessandro Zeppegno, Christian Nkomp Temangou

#### OBJECT:
Create a system that checks the status of the fixtures and monitoring the temperature/humidity of your home, through interaction with a Bot Telegram.

#### ABSTRACT:
We want to allow anyone to know the dynamics that happens in their own home.
We thought to use a MKR1000 connected to the home WiFi, which interacts with the users through a bot telegram.
The main functions we have decided to implement are the temperature and humidity monitoring in your home and control the opening and closing windows.

#### THE IDEA:
We thought it would be useful to know by a simple Telegram Bot  the state of the home. Because in addition to being a convenient and effective tool, it can also increases the safety of the house and of ourselves, because we can be aware, when you are out  if there are open windows “or if perhaps we have forgotten the gas on.”

#### WHAT WE USED:
1x MKR1000
1x DHT22 (temperature / humidity sensor)
1x buttons
x2 Resistance (10k)
x1 LCD Hitachi

#### BOT
The Bot was developed on Telegram with @BotFather.
Its main functions are two:
Communicate user request the actual temperature of the house.
Notify the opening or closing of windows.
The Bot is connected to MKR1000 through the implementation of the Telegram library for Arduino, that allows you to receive requests, process them, and send data to the Bot.

#### TEMPERATURE/HUMIDITY
To monitor the temperature we decided to notify only on user request via BOT Telegram.
This way you can know at any time the temperature in your home.

#### WINDOWS
For the window control we thought of using a magnetic sensor placed on the two windows ashes.
In this way it will be possible to know the time when the two sensors are in contact or not, and the opening and closing of the window.

#### WORK PROCESS
We started from the hardware part, we have putted on the breadboard the window-button, and the DHT22 (Humidity/Temperature sensor). We have tried only with the Arduino software if they both functioned.
After that we create the Telegram Bot, with Bot Father, we putted in 2 buttons: Humididty and Temperature.
Then we have connected the Arduino Code with the Bot, using the Telegram library for Arduino. We maked a cicle that controls if there are request from the user, when he note a request it responds with the actual temperature. For the window-button instead we decide to notify everytime the button is pushed, so everytime the window is opened or closed.

#### DESIGN
We decided to make a box for our system, made with wood and cutted by a laser cut machine.
We also thought that sometimes the users want to see his system or change something in it, so we have putted a top cover that you can open.

#### WHY OUR PROJECT IS IMPORTANT?
Our project is very important, just because having used a MKR1000 can transmit data not only on the PC screen, but it can notify you everytime and anywhere you are, as long as there is an active internet connection.
