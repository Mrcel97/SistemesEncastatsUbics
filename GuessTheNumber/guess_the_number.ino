/**********************************************
Random Guess With LCD
Guillem Orellana, Jordi Onrubia, Marcel Ortiz
LCD Random Guess - Activity
4-10-2019
**********************************************/

#include <stdlib.h>
#include <time.h>
#include <LCD.h>

int n;
int seed = 1;
int out = 0;
int attempts = 0;
int found = -1;
int first = 1;
int initial = 0;
char msg[20];
char odd[] = "The number is odd";
char even[] = "The number is even";

LCD screenManager;
SoftwareSerial serial(3, 5);

void setup()
{
  randomSeed(analogRead(A0));
  Serial.begin(9600);
  screenManager.begin(&serial);
}

void loop()
{
  char c;
  int guess = -1;
  String guessy = "";

  while (out == 0)
  {
    randomSeed(analogRead(A0));
    if (first)
    {
      screenManager.clearScreen();
      seed = seed + 1;
      n = random(1, 1000);
      welcomeMessage();
      first = 0;
    }

    if (Serial.available())
    {
      screenManager.clearScreen();
      guessy = Serial.readString();
      if (isDigit(guessy[0]))
      {
        guess = guessy.toInt();
      }
    }

    if (guess != -1 && found == -1)
    {
      increaseAttempts();
      if (guess == n)
      {
        found = numberFound();
      }
      else if (guess > n)
      {
        char small[] = "The number is smaller";
        screenManager.print(1, 63, small);
      }
      else
      {
        char large[] = "The number is larger";
        screenManager.print(1, 63, large);
      }
    }
    else if (guessy[0] == 'h')
    {
      delay(100);
      char help[] = "Help:";
      screenManager.print(1, 63, help);
      seed = seed + 1;
      if (n % 2 == 0)
      {
        if ((seed % 2) == 0)
        {
          screenManager.print(1, 53, even);
        }
        else
        {
          printFunnyHelpMessages(n);
        }
      }
      else if (n % 2 == 1)
      {
        if ((seed % 2) == 0)
        {
          screenManager.print(1, 53, odd);
        }
        else
        {
          printFunnyHelpMessages(n);
        }
      }
    }
    else if (guess != -1 && found == 1)
    {
      if (guess == 1)
      {
        ressetGame();
      }
      else if (guess == 0)
      {
        endGame();
      }
    }
    guessy = "-1";
    guess = -1;
    delay(3000);
  }

  exit(0);
}

void welcomeMessage()
{
  char welcome[] = "Welcome";
  screenManager.rectangle(5, 40, 60, 60);
  screenManager.print(12, 53, welcome);
  delay(3000);
  screenManager.ressetXY();
}

void endGame()
{
  out = 1;
  char bye[] = "Bye Bye";
  screenManager.print(1, 63, bye);
}

void ressetGame()
{
  first = 1;
  found = -1;
  attempts = 0;
}

void increaseAttempts()
{
  attempts = attempts + 1;
}

int numberFound()
{
  char genius[] = "Genius";
  screenManager.clearScreen();
  fireworks();
  screenManager.print(1, 63, genius);
  sprintf(msg, "Attempts %d", attempts);
  screenManager.print(1, 53, msg);
  char again[] = "Wanna try again? Yes(1)/No(0)";
  screenManager.print(1, 46, again);
  return 1;
}

void fireworks()
{
  for (int i = 0; i < 30; i++)
  {
    screenManager.circle(30, 60, i);
  }
  delay(2000);
  screenManager.clearScreen();
  for (int i = 0; i < 30; i++)
  {
    screenManager.circle(70, 20, i);
  }
  delay(2000);
  screenManager.clearScreen();
  for (int i = 0; i < 30; i++)
  {
    screenManager.circle(50, 30, i);
  }
  delay(2000);
  screenManager.clearScreen();
  for (int i = 0; i < 30; i++)
  {
    screenManager.circle(100, 60, i);
  }
  delay(4000);
  screenManager.clearScreen();
}

void printFunnyHelpMessages(int n)
{
  char funny[] = "The number rimes with, por el culo te la hinco";
  char ohNoNo[] = "You do not want to have this mark";
  char cheap[] = "It is cheaper than an Airpods";
  char heaven[] = "This number is in heaven";
  char hell[] = "This number is in hell";
  if (n % 5 == 0)
  {
    screenManager.print(1, 53, funny);
  }
  else if (n <= 4)
  {
    screenManager.print(1, 53, ohNoNo);
  }
  else if (n < 230)
  {
    screenManager.print(1, 53, cheap);
  }
  else if (n > 666)
  {
    screenManager.print(1, 53, heaven);
  }
  else if (n <= 666)
  {
    screenManager.print(1, 53, hell);
  }
}
