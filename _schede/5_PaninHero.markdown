---
layout: schede
title: PaninHero
description: order a sandwich through a chat bot system
img: 
---

presentazione in fase di update


#### MEMBERS:
Barbero Giada, Quartero Roberto, Quatrale Andrea, Sacchetto Dario, Spano Costantino.

#### Problemi:	 
- Riportare su carta le comande della ordinazioni.
- Ordinazioni possibili solo fino alle 11:30.
- Perdita di tempo per rispondere ai clienti quando manca qualche informazione.

#### Soluzioni:
- Stampa automatica delle comande.
- Ordinazioni no-stop.
- Bot che gestisce il processo di ordinazione.

#### Obiettivi:
- Collegare una stampante termica alla MKR1000 che stampi automaticamente le comande delle ordinazioni prese tramite bot.
- Creazione bot per ordinazioni no-stop e automatiche, attivo negli orari di apertura del negozio fisico.

#### Materiale:
- Mini Thermal Receipt Printer
- MKR1000
- Alimentatore 5 volt
- Connettori vari
- Breadbord
- 1x led
- Case originale tagliato a laser


### L’IDEA

#### Abstract

_"Le ordinazioni vi fanno perdere tempo? Manca sempre qualcosa?
  Il bot delle ordinazioni é quello che fa per voi! “
  Un modo semplice e veloce per gestire le comande e far gustare un ottimo
 panino ai vostri clienti."_

#### Progetto

L’idea di progetto consiste nel creare un bot Telegram che permetta a Marco di gestire automaticamente le ordinazione, senza doversi mettere a riscrivere ogni messaggio e sopratutto senza interrompere il servizio dopo una certa ora.
Il bot interagirà con una scheda Arduino MKR1000 che a sua volta sarà collegata ad una piccola stampante termica, in grado di stampare la comanda dell’ordinazione ogni volta che qualcuno confermerà l’ordine su Telegram.
La stampante verrà inserita all’interno di una scatola tagliata a laser, unica e personalizzabile con le informazioni principali del locale, nonché il logo del Pan.da’s, creando un vero e proprio oggetto d’arredo. In aggiunta, verrà montato un led che permetterà a Marco,  di capire quando sta arrivando un nuovo ordine.

#### MVP (Minimum Viable Product)

Il nostro MVP è un bot Telegram che gestisce l’ordinazione di un panino e la stampi attraverso la stampante termica, con un’identazione corretta.
La stampante è inserita in una scatola di compensato da 4mm tagliata a laser con testata richiamante il logo e un piccolo led verde di notifica arrivo nuovo ordine.
