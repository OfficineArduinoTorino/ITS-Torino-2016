---
layout: schede
title: openPoll
description: monitor in real time the opinion of the web during the election campaigns
img:
---

<iframe src="https://docs.google.com/presentation/d/1TKPp3Ydo8pAHRVkV2EAdUaBg65J1RMyN_OTEkfZQ9K4/embed?start=false&loop=false&delayms=3000" frameborder="0" width="800" height="479" allowfullscreen="true" mozallowfullscreen="true" webkitallowfullscreen="true"></iframe>


<iframe width="560" height="315" src="https://www.youtube.com/embed/aIOBzP_tBms?rel=0" frameborder="0" allowfullscreen></iframe>

### OpenPoll

All the Materials of the project (code, circuits and laser files) can be found [here](https://github.com/OfficineArduinoTorino/ITS-Torino-2016/tree/master/OpenPoll).

#### Members:

Fabrizio Bianco, Francesco Brandozzi, Nicole Castagno, Patrizia D’Onofrio, Riccardo Benotto, Veronica Torresin

#### Objective:

This project intends to monitor in real time the web popularity of the six main city mayoral candidates in Turin before the elections. The results of the monitoring exercise are projected into a physical and dynamic installation.

#### Starting phase:

In order to understand which data we needed to collect, we interviewed social media experts, journalists, academics, politics and other professionals involved in this field. Twitter resulted to be one of the main sources. So we worked to identify the most used hashtags. Finally, based on the suggestions collected, we studied how to represent web data regarding the polls into a physical structure.

#### Initial idea and main challenges:

Initially we considered different solutions. For the physical part, we planned to use glass balls to be colored through color jets when the candidate has an increase of attention on the web. Another option was to use waterworks features inside a cylinder. However both solutions appeared difficult to create due to technical reasons.
Regarding the conceptual side of the project (web monitoring), we wanted to carry out a sentiment analysis of the data collected on Twitter. However we discovered that, with the current technologies, Italian contents are almost impossible to analyze.
In order to collect the data, we decided to use import.io, a platform allowing to easily take information from selected websites. However, we encountered a malfunctioning on the API callings and we had to look for a new method.

#### Actual project and development:

Based on the main challenges encountered during the preparation phase, the physical component of the final project consists of a wooden box containing a fan connected to electronical parts. On the top of it, a parallelepiped structure made of plexiglass is positioned. This is filled with small balls of polystyrene, which get mixed up by the air conveyed by the fan. The entire structure is closed by a metal net.
The data gathering from Twitter is carried through an Arduino library acting directly on the Twitter API. An Arduino device collects, process and transform the data into analog values. The values are then sent to another Arduino device whose job is to manage the physical part of the installation.

#### What we achieved:

A physical installation that collects in real time the inputs deriving from the polls monitoring on the web, showing them physically in a visual and engaging way.

#### ABSTRACT

We live in the era of Web 2.0, where social networks and online news have become key actors in our lives. At the same time, the tools used to assess politics are changing. This is the reason why the OpenPoll team wants to create an innovative way to monitor in real time the opinion of the web during the election campaigns and show it to the public through a physical computing installation.
