---
layout: schede
title: Let's Punch for Sherlock & IoT
description: Analytic Machine and Punched Cards for an immersive storytelling experience
img:
---

<iframe src="https://docs.google.com/presentation/d/1I_HqbeIkkM0IzyYXSrYuDQwzTVmJsympivM3NPsiQmg/embed?start=false&loop=false&delayms=3000" frameborder="0" width="800" height="480" allowfullscreen="true" mozallowfullscreen="true" webkitallowfullscreen="true"></iframe>


### Analytic Machine and Punched Cards

#### MEMBERS

BREAN FEDERICO, CONTESSA GIULIA, NARESE FRANCESCO, SAVASTA MARCO

#### INTRODUCTION:

Sherlock Holmes & the Internet of Things is a project of Digital Storytelling Lab by Columbia University that experiments new scenarios in narration shared through interaction with Virtual Reality and Internet of Things.
The story is set in a Steampunk Turin in 1834 and it revolves around a secret society and a series of advanced machineries which have been occulted away from the rest of the humanity.
These machines hide a very powerful secret within them...

#### OBJECTIVE:

In collaboration with Sherlock Holmes & the IoT, we decided to build an analytic machine to give a unique experience to the players. By inserting punched cards in a specific machine the teams will receive hints to use for solving the mystery.
Each player will live a first person experience through immediate and immersive interactions with the game system aided by interactive and interconnected objects.

#### DESCRIPTION:

The analytic machine will read the punched cards. On the top of the box there’s a led that constantly blinks and represent an emergency situation which alarms the players to hurry and complete certain tasks to avoid negative repercussions on the main story.
There are a total of 11 punched cards, 10 will trigger the thermal printer and activate 10 hints which will be printed for the players to use.
The last one will be found by the players during the game and it’s needed to turn off the blinking led on the top of the machine and will give the final hint.

#### HARDWARE:

A wooden box compose the main structure of the machine which works with an Arduino MKR1000, inside 5 leds and photoresistors will “read” the inserted punched cards, a led on the top (which shows the status of the machine) and one on the front (for the inserted cards feedback), and finally the thermal printer.
The punched cards are made of wood with 25 holes in a 5x5 pattern of which only the first row is useful for the readings signals.

#### SOFTWARE:

The MKR1000 inside the machine works with the Adafruit Thermal Printer libraries to implement the functions used to print the hints for each team playing.
The software recognize the code of each card by receiving the light signals which go through the holes of the card itself.
APPLICATIONS:

#### What can you do with the analytic machine?

- Print informations reserved for each owner of a punched card.
- Assign actions or tasks to do for each owner of a card
- Educational purpose, to get kids a closer look to the world of IT
