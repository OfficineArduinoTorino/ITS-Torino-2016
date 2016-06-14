#include <ArduinoJson.h>
#include <Adafruit_Thermal.h>
#include<WiFiSSLClient.h>
#include <WiFi101.h>
#include <SPI.h>
#include <TelegramBot.h>
#define TX_PIN 14;
#define RX_PIN 13;

Adafruit_Thermal printer(&Serial1);      // Or Serial2, Serial3, etc.

char ssid[] = "Hotspot Costa";             // your network SSID (name)
char pass[] = "costantino";           // your network key

//char ssid[] = "FASTWEB_AS_bis";             // your network SSID (name)
//char pass[] = "78B1920D6D";           // your network key

// Initialize Telegram BOT
const char BotToken[] = "225252437:AAHPTAnC4LYmFM6Swh78VHCScO7w6joVrTU"; //BOT Token
const char BotName[] = "Pan.da's Torino"; //BOT Name
const char BotUsername[] = "@pandaspaninibot"; //BOT Username
WiFiSSLClient client;
TelegramBot bot (BotToken, BotName, BotUsername, client);

//TELEGRAM KEYBOARDS CREATION
TelegramKeyboard scegliCibo;
TelegramKeyboard scegliPane;
TelegramKeyboard scegliTaglia;
TelegramKeyboard categorie;
TelegramKeyboard temperatura;
TelegramKeyboard nuovoOrdine;
TelegramKeyboard confermaAnnulla;
TelegramKeyboard fineRimuoviIngredienti;
TelegramKeyboard fineRimuoviIngredientiS;

//VARIABLES DECLARATION
bool ingredienti[66];
bool salse[11];
String comanda;
String listaCarniEUova;
String listaSalumi;
String listaFormaggi;
String listaInVasetto;
String listaVerduraCruda;
String listaVerduraCotta;
String listaPesce;
String listaSalse;

//SETUP
void setup() {
  pinMode(7, OUTPUT);
  digitalWrite(7, HIGH);
  Serial1.begin(9600); // Use this instead if using hardware serial
  Serial.begin(115200);
printer.begin(9600);        // Init printer (same regardless of serial type)

  // attempt to connect to Wifi network:
  delay(3000);
  Serial.print("Connecting Wifi: ");
  Serial.println(ssid);
  while (WiFi.begin(ssid, pass) != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("");
  Serial.println("WiFi connected");

  //KEYBOARD scegliCibo
  const char* rigascegliCibo[] = {"Panino", "Insalata"};
  scegliCibo.addRow(rigascegliCibo, 2);

  //KEYBOARD scegliPane

  const char* rigascegliPane[] = {"Morbido", "Croccante"};
  scegliPane.addRow(rigascegliPane, 2);

  //KEYBOARD scegliTaglia
  const char* rigascegliTaglia[] = {"Normale", "Grosso", "Gigante"};
  scegliTaglia.addRow(rigascegliTaglia, 3);

  //KEYBOARD temperatura
  const char* temp[] = {"Si", "No"};
  temperatura.addRow(temp, 2);

  //KEYBOARD categorie
  const char* categorie1[] = {"Carni e Uova", "Salumi"};
  const char* categorie2[] = {"Formaggi", "In vasetto" };
  const char* categorie3[] = {"Verdura cruda", "Verdura cotta"};
  const char* categorie4[] = {"Pesce"};
  categorie.addRow(categorie1, 2);
  categorie.addRow(categorie2, 2);
  categorie.addRow(categorie3, 2);
  categorie.addRow(categorie4, 1);

  //KEYBOARD fineIngredienti
  const char* pulsanteFineRimuoviIngredienti[] = {"Torna al menu", "Fine"};
  const char* pulsanteFineRimuoviIngredienti2[] = {"Rimuovi Ingrediente"};
  fineRimuoviIngredienti.addRow(pulsanteFineRimuoviIngredienti, 2);
  fineRimuoviIngredienti.addRow(pulsanteFineRimuoviIngredienti2, 1);

  //KEYBOARD confermaAnnulla (order)
  const char* pulsantiConfermaAnnulla[] = {"Conferma Ordine", "Annulla Ordine"};
  confermaAnnulla.addRow(pulsantiConfermaAnnulla, 2);

  //KEYBOARD fineIngredientiSalsa
  const char* pulsanteFineRimuoviIngredientiS[] = {"Fine"};
  const char* pulsanteFineRimuoviIngredientiS2[] = {"Rimuovi Salsa", "Nessuna salsa"};
  fineRimuoviIngredientiS.addRow(pulsanteFineRimuoviIngredientiS, 1);
  fineRimuoviIngredientiS.addRow(pulsanteFineRimuoviIngredientiS2, 2);
  
  for (int i = 1; i < 66; i++)
  {
    ingredienti[i] = false;
  }
  for (int i = 1; i < 11; i++)
  {
    salse[i] = false;
  }

  comanda = "";
  
  //INITIALIZING STRINGS OF INGREDIENTS
  listaCarniEUova = "-- CARNI E UOVA --\n";
  listaCarniEUova += "/1 Bacon\n";
  listaCarniEUova += "/2 Carne impanata\n";
  listaCarniEUova += "/3 Wurstel\n";
  listaCarniEUova += "/4 Salsiccia\n";
  listaCarniEUova += "/5 Uova sode\n";
  listaCarniEUova += "/6 Frittate\n";

  listaSalumi = "-- SALUMI --\n";
  listaSalumi += "/7 Prosciutto cotto\n";
  listaSalumi += "/8 Prosciutto crudo\n";
  listaSalumi += "/9 Salame pasta fine\n";
  listaSalumi += "/10 Salame pasta grossa\n";
  listaSalumi += "/11 Spianata calabra\n";
  listaSalumi += "/12 Mortadella\n";
  listaSalumi += "/13 Speck\n";
  listaSalumi += "/14 Arrosto di tacchino\n";
  listaSalumi += "/15 Lardo\n";
  listaSalumi += "/16 Pancetta arrotolata\n";
  listaSalumi += "/17 Porchetta\n";
  listaSalumi += "/18 Bresaola\n";

  listaFormaggi = "-- FORMAGGI --\n";
  listaFormaggi += "/19 Provola\n";
  listaFormaggi += "/20 Scamorza\n";
  listaFormaggi += "/21 Fontina\n";
  listaFormaggi += "/22 Gorgonzola\n";
  listaFormaggi += "/23 Mozzarella\n";
  listaFormaggi += "/24 Bufala\n";
  listaFormaggi += "/25 Brie\n";
  listaFormaggi += "/26 Stracchino\n";
  listaFormaggi += "/27 Parmigiano\n";
  listaFormaggi += "/28 Pecorino\n";
  listaFormaggi += "/29 Ricotta\n";
  listaFormaggi += "/30 Edamer\n";

  listaInVasetto = "-- IN VASETTO --\n";
  listaInVasetto += "/31 Friarielli\n";
  listaInVasetto += "/32 Carciofini\n";
  listaInVasetto += "/33 Cetriolini\n";
  listaInVasetto += "/34 Melanzane\n";
  listaInVasetto += "/35 Funghetti\n";
  listaInVasetto += "/36 Pomodori secchi\n";
  listaInVasetto += "/37 Peperoncini\n";
  listaInVasetto += "/38 Zucchine\n";
  listaInVasetto += "/39 Mais\n";

  listaVerduraCruda += "-- VERDURA CRUDA --\n";
  listaVerduraCruda += "/40 Insalata verde\n";
  listaVerduraCruda += "/41 Edamer\n";
  listaVerduraCruda += "/42 Rucola\n";
  listaVerduraCruda += "/43 Pomodoro\n";
  listaVerduraCruda += "/44 Carote\n";
  listaVerduraCruda += "/45 Finocchi\n";
  listaVerduraCruda += "/46 Funghi\n";
  listaVerduraCruda += "/47 Cipolla rossa (di tropea)\n";

  listaVerduraCotta += "-- VERDURA COTTA --\n";
  listaVerduraCotta += "/48 Peperoni grigliati\n";
  listaVerduraCotta += "/49 Peperoni saltati\n";
  listaVerduraCotta += "/50 Zucchine grigliate\n";
  listaVerduraCotta += "/51 Zucchine saltate\n";
  listaVerduraCotta += "/52 Melanzane grigliate\n";
  listaVerduraCotta += "/53 Patate bollite\n";
  listaVerduraCotta += "/54 Patate saltate\n";
  listaVerduraCotta += "/55 Patate fritte\n";
  listaVerduraCotta += "/56 Cipolle croccanti\n";
  listaVerduraCotta += "/57 Cipolle saltate\n";
  listaVerduraCotta += "/58 Crauti\n";
  listaVerduraCotta += "/59 Funghi trifolati\n";
  listaVerduraCotta += "/60 Spinaci al burro\n";
  listaVerduraCotta += "/61 Friggitelli\n";
  listaVerduraCotta += "/62 Fagioli\n";

  listaPesce = "-- PESCE --\n";
  listaPesce += "/63 Acciughe\n";
  listaPesce += "/64 Tonno\n";
  listaPesce += "/65 Pesce impanato\n";

  listaSalse = "-- SALSE --\n";
  listaSalse += "/1 Ketchup\n";
  listaSalse += "/2 Maionese\n";
  listaSalse += "/3 Senape\n";
  listaSalse += "/4 Salsa rosa\n";
  listaSalse += "/5 Barbecue\n";
  listaSalse += "/6 Tartara\n";
  listaSalse += "/7 Yogurt\n";
  listaSalse += "/8 Pan.da's piccante\n";
  listaSalse += "/9 Pan.da's dolce\n";
  listaSalse += "/10 Pan.da's saporita\n";


  bot.begin();


}




message m; // That variable read the message sent to the telegram bot
String scelti; //Chosen ingredients
String scelta = ""; 
boolean flag = false;
boolean flag2 = false;
boolean flag3 = false;
boolean flag4 = false;

int sceltaint;

void loop() {





  while (true)
  {
    m = bot.getUpdates();//GET UPDATES FROM THE LAST MESSAGE SENT TO THE BOT | IT READS A JSON STRING,THAT CONTAINS ALL THE FIELDS OF THE MESSAGE , FROM chat_id TO text.
    //Follow this link to read the Json file of your bot
    //https://api.telegram.org/bot<Insert your bot token here instead of this message , without quotes too>/getupdates


    
    scelta = (String)m.text;//GET THE STRING FROM THE text FIELD OF THE JSON MESSAGE.

    //IF THE BOT IS RUNNING , INITIALIZE ARRAYS OF INGREDIENTS
    if (scelta == "/start")
    {
      for (int i = 1; i < 66; i++)
      {
        ingredienti[i] = false;
      }
      for (int i = 1; i < 11; i++)
      {
        salse[i] = false;
      }

      comanda = "";

      //THE BOT ANSWER TO THE USER , RECOGNIZING HIM THROUGH THE chat_id FIELD FROM THE JSON
      bot.sendMessage(m.chat_id, "Ciao " +  m.sender + " sono PandaBot, il BOT per ordinare Panini.\nInizia l'ordine scrivendo il tuo nome");
      do {
        m = bot.getUpdates();
        scelta = (String)m.text;
      } while (scelta == "" || scelta == "/start");
      comanda += "NOME ORDINAZIONE: " + scelta + "\n";
      bot.sendMessage(m.chat_id, "A che ora lo vieni a prendere?\nformato hh:mm \nes. 12:40");

      do {
        m = bot.getUpdates();
        scelta = (String)m.text;
      } while (scelta == "" || scelta == "/start");

      comanda += "ORARIO:  " + scelta + "\n";
      bot.sendMessage(m.chat_id, comanda + "\nScegli il tipo di pane: MORBIDO o CROCCANTE", scegliPane, false);

    }

    else if (scelta == "Morbido" || scelta == "Croccante")
    {
      comanda += "TIPO DI PANE: " + scelta + "\n";
      bot.sendMessage(m.chat_id, comanda + "\nDi che taglia lo vuoi? \nNORMALE (€3)\nGROSSO (€4)\nGIGANTE (€5)\n ", scegliTaglia, false);


    }
    else if (scelta == "Normale" || scelta == "Grosso" || scelta == "Gigante")
    {
      comanda += "TAGLIA PANINO: " + scelta + "\n";

      bot.sendMessage(m.chat_id, comanda + "\nScegli la categoria degli ingredienti\nUna volta selezionata la categoria potrai scegliere l'ingrediente premendo sul numero blu o digitarlo preceduto da /", categorie, false);



      do {

        do {
          m = bot.getUpdates();
          scelta = (String)m.text;
        } while (scelta == "");

        if (scelta.length() <= 3)
        {

          scelta = scelta.substring(1);
          sceltaint = scelta.toInt();
        }


        if (scelta == "Torna al menu")
        {
          scelti="";
           for (int i = 1; i < 66; i++)
          {


            if (ingredienti[i] == true)
            {
              //if (flag == true) scelti += " - ";
              //flag = true;
              scelti += (String)i+"  ";
            }

          }
          
          bot.sendMessage(m.chat_id, "Ingredienti scelti fino ad ora:\n"+scelti+"\n\nScegli la categoria di ingredienti", categorie, false);
        
        }



        else if (scelta == "Carni e Uova")
        {
          bot.sendMessage(m.chat_id, listaCarniEUova, fineRimuoviIngredienti);

        } else if (scelta == "Salumi")
        {
          bot.sendMessage(m.chat_id, listaSalumi, fineRimuoviIngredienti);

        } else if (scelta == "Formaggi" )
        {
          bot.sendMessage(m.chat_id, listaFormaggi, fineRimuoviIngredienti);

        } else if (scelta == "In vasetto"  )
        {
          bot.sendMessage(m.chat_id, listaInVasetto, fineRimuoviIngredienti);

        } else if (scelta == "Verdura cruda"  )
        {
          bot.sendMessage(m.chat_id, listaVerduraCruda, fineRimuoviIngredienti);

        } else if (scelta == "Verdura cotta" )
        {
          bot.sendMessage(m.chat_id, listaVerduraCotta, fineRimuoviIngredienti);

        } else if (scelta == "Pesce"  )
        {
          bot.sendMessage(m.chat_id, listaPesce, fineRimuoviIngredienti);

        }


        Serial.println(scelta.length());
        if ((sceltaint > 0 && sceltaint < 66) && (scelta.length() > 0 && scelta.length() <= 3))
        {

          Serial.println(sceltaint);

          ingredienti[sceltaint] = true;
          //scelti = "Ingredienti selezionati: ";
          /*for (int i = 1; i < 66; i++)
          {


            if (ingredienti[i] == true)
            {
              if (flag == true) scelti += " - ";
              flag = true;
              scelti += (String)i;
            }

          }
          bot.sendMessage(m.chat_id, scelti);*/


          scelta == "";
        }



        if (scelta == "Rimuovi Ingrediente")
        {
          Serial.println("Entrato rimuovi ingrediente");
          scelti = "\n";
          for (int i = 1; i < 66; i++)
          {


            if (ingredienti[i] == true)  scelti += "/" + (String)i + "  ";



          }
          bot.sendMessage(m.chat_id, "Seleziona l'ingrediente da rimuovere" + scelti);
          do {
            m = bot.getUpdates();
            scelta = (String)m.text;
          } while (scelta == "");

          scelta = scelta.substring(1);
          sceltaint = scelta.toInt();
          if (sceltaint > 0 && sceltaint < 66)
          {

            ingredienti[sceltaint] = false;

            scelta = "Ingrediente " + (String)sceltaint + " rimosso";
            bot.sendMessage(m.chat_id, scelta);

          } else {
            bot.sendMessage(m.chat_id, "Ingrediente da rimuovere non valido");
          }

          scelti = "Ingredienti scelti fino ad ora :\n";
          for (int i = 1; i < 66; i++)
          {


            if (ingredienti[i] == true)
            {
              //if (flag2 == true) scelti += " - ";

              //flag2 = true;
              scelti += (String)i+"  ";
            }

          }
          bot.sendMessage(m.chat_id, scelti, fineRimuoviIngredienti, false);

          scelta == "";


        }


      } while (scelta != "Fine");

      comanda += "INGREDIENTI:\n";
      for (int i = 1; i < 66; i++)
      {
        if (ingredienti[i] == true)
          comanda += (String)i + "  ";
      }
      comanda += "\n";
      bot.sendMessage(m.chat_id, comanda);
      bot.sendMessage(m.chat_id, listaSalse, fineRimuoviIngredientiS, false);
      do {


        do {
          m = bot.getUpdates();
          scelta = (String)m.text;
        } while (scelta == "");


        if (scelta.length() <= 3)
        {
          scelta = scelta.substring(1);
          sceltaint = scelta.toInt();


          salse[sceltaint] = true;

          scelti = "Salse selezionate: ";
          /*for (int i = 1; i < 11; i++)
          {


            if (salse[i] == true)
            {
              if (flag3 == true) scelti += " - ";
              flag3 = true;
              scelti += (String)i+"  ";
            }

          }
          bot.sendMessage(m.chat_id, scelti);*/



          scelta = "";
        }

        if (scelta == "Rimuovi Salsa")
        {
          scelti = "";
          for (int i = 1; i < 11; i++)
          {


            if (salse[i] == true)
            {
              //if (flag4 == true) scelti += "  /";
              //flag4 = true;
              scelti +="/"+(String)i+"  " ;
            }

          }
          bot.sendMessage(m.chat_id, "Seleziona la salsa da rimuovere\n" + scelti);
          do {
            m = bot.getUpdates();
            scelta = (String)m.text;
          } while (scelta == "");

          scelta = scelta.substring(1);
          sceltaint = scelta.toInt();
          if (sceltaint > 0 && sceltaint < 11)
          {
            salse[sceltaint] = false;
            scelta = "Salsa " + (String)sceltaint + " rimossa";
            bot.sendMessage(m.chat_id, scelta);

          } else {
            bot.sendMessage(m.chat_id, "Salsa da rimuovere non valida");
          }  

          scelti = "Salse scelte fino ad ora:\n";
          for (int i = 1; i < 11; i++)
          {
            if (salse[i] == true) scelti += (String)i + "  ";
          }
          bot.sendMessage(m.chat_id, scelti, fineRimuoviIngredientiS);

          scelta = "";


        }
        Serial.println(scelta);
      } while (scelta != "Fine" && scelta != "Nessuna salsa");

      if (scelta == "Fine")
      {
        comanda += "SALSE:\n";
        for (int i = 1; i < 11; i++)
        {
          if (salse[i] == true)
          {
            comanda += (String)i + "  ";
          }
        }
        comanda += "\n";
      } else comanda += "Nessuna salsa";

      bot.sendMessage(m.chat_id, comanda + "\nLo vuoi caldo?", temperatura, false);
    } else if (scelta == "Si" || scelta == "No")
    {
      comanda += (scelta == "Si" ? "Panino scaldato\n" : "");



      bot.sendMessage(m.chat_id, comanda, confermaAnnulla, true);


    }





    else if (scelta == "Conferma Ordine")
    {

      //Print the order
      bot.sendMessage(m.chat_id, "Ordine confermato e inviato.\nClicca /start per un nuovo ordine.");
      for (int i = 0; i < 3; i++)
      {
        digitalWrite(7, LOW);
        delay(500);
        digitalWrite(7, HIGH);
        delay(500);

      }
      printer.println(comanda);
      printer.println("_______________________");
      printer.println();


      for (int i = 1; i < 66; i++)
      {
        ingredienti[i] = false;
      }
      for (int i = 1; i < 11; i++)
      {
        salse[i] = false;
      }

      comanda = "";


    }
    else if (scelta == "Annulla Ordine")
    {
      for (int i = 1; i < 66; i++)
      {
        ingredienti[i] = false;
      }
      for (int i = 1; i < 11; i++)
      {
        salse[i] = false;
      }

      comanda = "";
      bot.sendMessage(m.chat_id, "Ordine annullato.\nClicca /start per un nuovo ordine");
    }
  }


/*
TO DO:

- Aggiungere la possibilità di accettare o annullare l'ordine da parte del gestore.
- Aggiungere invio ordini anche in chat con gestore come copia nel caso in cui finisca la carta della stampante o altri possibili problemi con stampante.

*/



}
