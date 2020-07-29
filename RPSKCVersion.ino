#include "bot_credentials.h"
#include <memory_constraints.h>
#include <twitch_irc_api.h> //this library needs the ArduinoWebsockets library to work, so be sure to add it. https://github.com/Links2004/arduinoWebSockets
#include <ESP8266WiFi.h>
#include <Servo.h>
#include <FastLED.h>

// USE GENERIC ESP AS NODE MCU HAS ISSUES WITH FAST LED //

// Based off https://github.com/noycebru/twitch-websockets-for-channel-point-robots

int debugdelay = 1000;
int lDelay = 500;
int servoDelay = 100;

const char* choice[] = {"Rock","Paper","Scissors"};

// Replace YOU with USERNAME
  //char* display_name according to struct PrivMsg

const char* rox[]={" chose ROCK. That says a lot about your personality, interesting...stay away from kids.",
" chose ROCK. It's good, but METAL is better |m|.",
" chose ROCK. You like small hard things?",
" chose ROCK, good luck, you'll need it."};

const char* pap[]={" chose PAPER, so.. do you wipe front to back or back to front?",
" chose PAPER, It's good for writing on. Write this down, you will lose.",
" chose PAPER, weak.",
" chose PAPER. What's black and white and read all over, doesn't matter you'll lose."};

const char* scis[]={" chose SCISSORS. Some comment about ladies who like ladies (who are awesome).",
" chose SCISSORS, a tool of many uses, like stabbing.",
" chose SCISSORS, let's hope they are sharp.",
" chose SCISSORS, I've run out of ideas, hope you lose."};

char* win[]={"This is Bullshit!!",
"You are cheating right?",
"I Lost. How is this possible? I am a god, you hear me...a GOD!!!!",
"Meh, I let you win, hope you feel like a big man"};

char* loss[]={"Don't feel bad, I was cheating...I mean playing this when you were a youngin'",
"Loser says what?",
"I'd gloat, but it's against my programming. No it's not, nya nya nya nya nya nyaaaaa.",
"It's so hard being this awesome all the time."};

char* draw[]={"You know drawing is just like losing right?",
"It's like we both lost, how depressing.",
"Remember that time you won once...",
"All I am programmed to draw is hentai."};

#define LED_PIN 12 // D6
#define NUM_LEDS 6
#define COLOR_ORDER GRB
#define COOLING 55
#define SPARKING 120
#define BRIGHTNESS 150


//Twitch Api Instance
TwitchWebsocketIRC twitch_api;
  
uint8_t gHue = 0; // rotating "base color" used by many of the patterns

bool gReverseDirection = false;

const char* ssid = WIFI_SSID; //Enter SSID
const char* password = WIFI_PASS; //Enter Password

Servo myservo;
CRGB leds[NUM_LEDS];

void setup() {
  
  // Connect to wifi
  WiFi.begin(ssid, password);

  // Check if connected to wifi
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print("No connection");
    delay(2000);
  }

  myservo.attach(14,540,2400); //D5
  myservo.write(90); // set to default position

  FastLED.addLeds<WS2812, LED_PIN, COLOR_ORDER>(leds,NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness(BRIGHTNESS);

  Serial.begin(115200);
  Serial.println("Connected to Wifi.");
  delay(3000);

 
  //Connect to twitch via websockets
  twitch_api.connect_to_twitch_websocket();

  //Register generic callbacks
  twitch_api.onPrivMsg(onPrivMsgCallback);
;


  //Helper Functions
  /*
    A sub plan is returned on subs, resub, gift, etc. You can use the helper function to convert int value to string.
  */
  printf("%s\n", twitch_api.sub_type_to_string(0));
  printf("%s\n", twitch_api.sub_type_to_string(1000));
  printf("%s\n", twitch_api.sub_type_to_string(2000));
  printf("%s\n", twitch_api.sub_type_to_string(3000));
  /*
    Emotes are returned when people say them in chat or in messages when donating. You can use the function to get the image url of the icon.
    Sizes: IMG_SMALL, IMG_MEDIUM, IMG_LARGE
  */
  printf("Emote Image URL: %s\n", twitch_api.get_emote_image_url(301448586, TwitchWebsocketIRC::Emote_Image_Size::IMG_MEDIUM));
}

void loop() 
{


  if (twitch_api.available()) 
  {
    twitch_api.poll();
  }
 startup();

 
////////NEED TO PUT IN DELAY SO IT CAN'T BE TRIGGERED WHILE IN LOOP AGAIN

}

void clearThem()
{

  for (int q = 0; q < NUM_LEDS; q++)
    {
      leds[q] = CRGB::Black;  
      FastLED.show();
    }
}

void showTime()

{
  
 for (int st = 0; st < 10; st++)
  {
      int servoRand = random(0,3);

      if (servoRand == 0)
      {
        int servoAngle = 0;
        myservo.write(servoAngle); 
      }
      else if (servoRand == 1)
      {
        int servoAngle = 90;
        myservo.write(servoAngle); 
      }
      else if (servoRand == 2)
      {
        int servoAngle = 210;
        myservo.write(servoAngle); 
      }
      delay(100); 
  }
  
}

void winner()
{
    for (int x = 0; x < 3; x++)
  {
    for (int i = 0; i < NUM_LEDS; i++)
      {
         leds[i] = CRGB::Green;
         FastLED.show();
         delay(10);
         FastLED.clear();
      }
      
      for (int i = 0; i < NUM_LEDS; i++)
      {
        leds[i] = CRGB::Aqua;
        FastLED.show();
        delay(10);
        FastLED.clear();
        delay(10);
      }

       for (int i = 0; i < NUM_LEDS; i++)
      {
        leds[i] = CRGB::Teal;
        FastLED.show();
        delay(10);
        FastLED.clear();
        delay(10);
      }
      
      for (int i = 0; i < NUM_LEDS; i++)
      {
        leds[i] = CRGB::Purple;
         FastLED.show();
         delay(10);
         FastLED.clear();
         delay(10);
      }

      for (int i = 0; i < NUM_LEDS; i++)
      {
        leds[i] = CRGB::Blue;
        FastLED.show();
        delay(10);
        FastLED.clear();
        delay(10);
      }
  }
clearThem();
}

void startup()
{
      for (int i = 0; i < NUM_LEDS; i++)
      {
        leds[i] = CRGB::White;
        FastLED.show();
      }

}

void loser()
{

  
      for (int i = 0; i < NUM_LEDS; i++)
      {
        leds[i] = CRGB::Red;
        FastLED.show();
        delay(250);
      }

}

void lDraw()
{

  
      for (int i = 0; i < NUM_LEDS; i++)
      {
        leds[i] = CRGB::Yellow;
        FastLED.show();
        delay(250);
      }

}





void onPrivMsgCallback(PrivMsg data) {
  printf("\n---------------------------------------------------\n");
  int count = 0;
  printf("\n=PRIVMSG=\n");
  printf("\nBadges Info\n");

//////////////////////////////////////////////////////MONITOR FOR CALL FROM BOT IN MAIN CHAT///////////////////////////////////////////////////////

/*if (strstr(data.message, "chose Rock") != NULL && (strcmp(data.display_name, "DrPuppetMaster") == 0)) {

    twitch_api.send_chat_message("This is where the RPS stuff would now go");   
*/  
    
  if (strstr(data.message, "chose Rock") != NULL && (strcmp(data.display_name, "DrPuppetMaster") == 0)) {

    int Player = 0; 
    String submessage(data.message);
    int endOfUsername = submessage.indexOf(" chose Rock"); 
    String username = submessage.substring(0, endOfUsername);
    int randomDraw = random(0,4);
    char message[51] = "";
    strncat (message,username.c_str(),50);
    strncat (message,rox[randomDraw],500);
    twitch_api.send_chat_message(message);   
    RPS(Player);
  }
  
if (strstr(data.message, "chose Paper") != NULL && (strcmp(data.display_name, "DrPuppetMaster") == 0)) {
    int Player = 1; 
    String submessage(data.message);
    int endOfUsername = submessage.indexOf(" chose Paper"); 
    String username = submessage.substring(0, endOfUsername);
    int randomDraw = random(0,4);
    char message[51] = "";
    strncat (message,username.c_str(),50);
    strncat (message,pap[randomDraw],500);
    twitch_api.send_chat_message(message);   
    RPS(Player);
  }
if (strstr(data.message, "chose Scissors") != NULL && (strcmp(data.display_name, "DrPuppetMaster") == 0)) {
    int Player = 2; 
    String submessage(data.message);
    int endOfUsername = submessage.indexOf(" chose Paper"); 
    String username = submessage.substring(0, endOfUsername);
    int randomDraw = random(0,4);
    char message[51] = "";
    strncat (message,username.c_str(),50);
    strncat (message,scis[randomDraw],500);
    twitch_api.send_chat_message(message);   
    RPS(Player);
  }
 
  
  while (data.badge_info[count].name != NULL) {
    printf("\t-Badge Name: %s Version: %i\n", data.badge_info[count].name, data.badge_info[count].version);
    count += 1;
  }
  count = 0;
  printf("\nBadges\n");
  while (data.badges[count].name != NULL) {
    printf("\t-Badge Name: %s Version: %i\n", data.badges[count].name, data.badges[count].version);
    count += 1;
  }
  count = 0;
  printf("\nEmote Chat Set\n");
  while (data.emotes[count].start_index_location != 0 && data.emotes[count].end_index_location != 0) {
    printf("\t-Emote Id: %i\n", data.emotes[count].emote.emote_id);
    printf("\t-Emote First Index: %i\n", data.emotes[count].start_index_location);
    printf("\t-Emote Second Index: %i\n", data.emotes[count].end_index_location);
    count += 1;
  }
  printf("Bits: %i\nColor: %s\nDisplay Name: %s\nId: %s\nMessage: %s\nMod: %i\nRoom Id: %i\nTmi Sent Ts: %s\nUser Id: %i\nChannel Name: %s\n",
         data.bits, data.color, data.display_name, data.id, data.message, data.mod, data.room_id, data.tmi_sent_ts, data.user_id, data.channel_name);
  printf("\n---------------------------------------------------\n");
}


void RPS(int Player) // This is the main code
{

    startup(); 
    
    //Randomly select PC selection of Rock, Paper, Scissors
    
    
    int randomPlay = random(0,3);
    showTime();
    delay(500);
    
    
          
    //////////////////////COMPARE RESULTS////////////////////
    
    ////////////////////DRAW////////////////////
    
    if (Player == randomPlay)
    
        {
          if (Player == 0) // ROCK
            {
              myservo.write(210);
              delay(servoDelay);
            }
            else if  (Player == 1)// PAPER
            {
              myservo.write(90);
              delay(servoDelay);
            }
            else 
            {
              (Player == 2);// SCISSORS
              myservo.write(0);
              delay(servoDelay);
            }
    
    
       
          //outputs to twitch chat
          //username here::::::::::::::::::::::::::::::::::::::::::
          // be good to identify the name of that chosen.
    
          //trying to get one one line
          char message[] = {"Whelp, looks like we both chose "};

             char rps[30] = {"/w drpuppetmaster !rpsdraw"};
             twitch_api.send_chat_message(rps); 
          
          strncat (message,choice[Player],45); // max chars is 45 with both added
          twitch_api.send_chat_message(message);    
          int randomDraw = random(0,4);
          twitch_api.send_chat_message(draw[randomDraw]);
          
          lDraw();

    
          //Clear variables
          Player = 99;
          randomPlay = 666;
    
    }
    
    // ROCK BEAT SCISSORS---------------
    
    if (Player == 0 && randomPlay == 2)
    
        {
    
          myservo.write(0);
          delay(servoDelay);
          
           //outputs to twitch chat
          //username here::::::::::::::::::::::::::::::::::::::::::
          twitch_api.send_chat_message("So...I choose SCISSORS.");
          int randomWin = random(0,4);
          twitch_api.send_chat_message(win[randomWin]);     
          
          winner();

              
          //Clear variables
          Player = 99;
          randomPlay = 666;

             char rps[30] = {"/w drpuppetmaster !rpswin"};
             twitch_api.send_chat_message(rps); 
    
    }
    
    // ROCK BEATEN BY PAPER----------    
    
    if (Player == 0 && randomPlay == 1)
    
    {
    
    myservo.write(90);
    delay(servoDelay);
    
    //outputs to twitch chat
          //username here::::::::::::::::::::::::::::::::::::::::::
          twitch_api.send_chat_message("To be different I choose PAPER.");

             char rps[30] = {"/w drpuppetmaster !rpsloss"};
             twitch_api.send_chat_message(rps); 
          
          int randomLoss = random(0,4);
          twitch_api.send_chat_message(loss[randomLoss]);    
          
          loser();

    
          //Clear variables
          Player = 99;
          randomPlay = 666;

    }
    
    // SCISSORS BEAT PAPER---------------
    
    if (Player == 2 && randomPlay == 1)
    
    {
    
          myservo.write(90);
          delay(servoDelay);
          
    //outputs to twitch chat
          //username here::::::::::::::::::::::::::::::::::::::::::
          twitch_api.send_chat_message("I guess I choose PAPER.");
          int randomWin = random(0,4);
          twitch_api.send_chat_message(win[randomWin]);    
    
          winner();
    
    
          //Clear variables
          Player = 99;
          randomPlay = 666;

          char rps[30] = {"/w drpuppetmaster !rpswin"};
          twitch_api.send_chat_message(rps); 
          
    }
    
    // SCISSORS BEATEN BY ROCK----------    
    
    if (Player == 2 && randomPlay == 0)
    
        {
         
          myservo.write(210);
          delay(servoDelay); 
          
          //outputs to twitch chat
          //username here::::::::::::::::::::::::::::::::::::::::::
          twitch_api.send_chat_message("I choose ROCK.");

             char rps[30] = {"/w drpuppetmaster !rpsloss"};
             twitch_api.send_chat_message(rps); 
          
          int randomLoss = random(0,4);
          twitch_api.send_chat_message(loss[randomLoss]); 
    
          loser();

    
    //Clear variables
          Player = 99;
          randomPlay = 666;

          
    }
    
     // PAPER BEATS ROCK---------------
    
    if (Player == 1 && randomPlay == 0)
    
        {
    
          myservo.write(210);
          delay(servoDelay);
          
          //outputs to twitch chat
          //username here::::::::::::::::::::::::::::::::::::::::::
          twitch_api.send_chat_message("Well, I choose ROCK.");
          int randomWin = random(0,4);
          twitch_api.send_chat_message(win[randomWin]);    
          
          winner();
    
      
      //Clear variables
          Player = 99;
          randomPlay = 666;

             char rps[30] = {"/w drpuppetmaster !rpswin"};
             twitch_api.send_chat_message(rps); 
    }
    
    // PAPER BEATEN BY SCISSORS----------    
    
    if (Player == 1 && randomPlay == 2)
    
        {
    
          myservo.write(0);
          delay(servoDelay);
    
          //outputs to twitch chat
          //username here::::::::::::::::::::::::::::::::::::::::::
          twitch_api.send_chat_message("Ok, I choose SCISSORS.");
          
             char rps[30] = {"/w drpuppetmaster !rpsloss"};
             twitch_api.send_chat_message(rps); 
          
          int randomLoss = random(0,4);
          twitch_api.send_chat_message(loss[randomLoss]);  
    
          loser();
          delay(800);
          //Clear variables
          Player = 99;
          randomPlay = 666;
     
        }
    
    
    //Clear variables
          Player = 99;
          randomPlay = 666;



    
    }
