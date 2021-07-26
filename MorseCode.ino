#include "Queue.h"

#define TEST_MODE

#define SPEAKER_OUT 40

#define PERIOD 250

#define SHORT_BEEP '.'
#define LONG_BEEP '-'
#define NO_BEEP ' '

#define QUEUE_OVERFLOW 2
#define INVALID_CHAR 1
#define SUCCESS 0


bool morseFilter(String);


Queue* morseQueue = new Queue(1024);


String morseLetterMapping[] = 
{
  ". - ", //a
  "- . . . ", //b
  "- . - . ", //c 
  "- . . ", // d
  ". ", //e
  ". . - . ", //f 
  ". . . . ", //g 
  ". . ", //h
  ". - - - ", //i
  ". - - - ", //j
  "- . - ", //k
  ". - . . ", //l
  "- - ", // m
  "- . ", // n
  "- - - ", // o
  ". - - . ", // p
  "-  - . - ", // q
  ". - . ", // r
  ". . . ", // s
  "- ", // t
  ". . -", //u 
  ". . . -", //v
  ". . - ", // w
  "- . . - ", // x
  "- . - - ", // y
  "- - . . " //z
  };
  
String morseDigitMapping[] = 
{
  "- - - - - ",//0
  ". - - - - ",//1
  ". . - - - ",//2
  ". . . - - ",//3
  ". . . . - ",//4
  ". . . . . ",//5
  "- . . . . ", //6
  "- - . . .", //7
  "- - - . . ", //8
  "- - - - . " // 9
};

void setup() 
{
  Serial.begin(9600);
  pinMode(RED_LED, OUTPUT);
  pinMode(SPEAKER_OUT,OUTPUT);
  Serial.flush();
  #ifdef TEST_MODE
  sendMessage("lemon12");
  #endif
}

int sendChar(char message)
{
  if(!morseFilter(message))
  {
    return INVALID_CHAR;
  }
  
  if(morseQueue->getSize()>=morseQueue->getCapacity())
  {
    return QUEUE_OVERFLOW;
  }
  String morse = translate2Morse(message);
  for(int i=0; i<morse.length();i++)
  {
    morseQueue->push(morse.charAt(i));
  }
  return SUCCESS;
}


int sendMessage(String message)
{
  message.replace(" ","");
  int returnCode = 0;
 for(int i=0;i<message.length();i++)
 {
  int charStatus = sendChar(message.charAt(i));
  if(returnCode<charStatus)
  {
    returnCode = charStatus;
    if(returnCode==QUEUE_OVERFLOW)
    {
      break;
    }
  }
 }
 return returnCode;
}

void queueInput()
{
  while(Serial.available())
  {
    if(sendChar((char)Serial.read())==QUEUE_OVERFLOW)
    {
      Serial.println("OVERFLOW");
      Serial.flush();
      break;
    }
  }
}

void displayMorse()
{
  static unsigned long beepStarted=0, beepDuration=0;
  if(beepDuration+beepStarted>=millis())
  {
    return;
  }
  //pop queue
  char currentBeep = morseQueue->pop();
  //adjust duration beep
  beepDuration = getBeepDuration(currentBeep);
  int outputFreq = currentBeep==NO_BEEP|currentBeep=='\0'?0:128;
  bool ledOn = outputFreq == 128;
  digitalWrite(GREEN_LED,ledOn);
  analogWrite(SPEAKER_OUT, outputFreq);
  beepStarted = millis();
}


void loop() 
{ 

  queueInput();
  displayMorse();
  delay(10);
}

//returns true if the character can be represented
bool morseFilter(char c)
{
  return translate2Morse(c)!="";
}

String translate2Morse(char c)
{
  c = toUpperCase(c);
  if(c>='A'&&c<='Z')
  {
    return morseLetterMapping[c-'A'];
  }
  else if(c>='0' || c<='9')
  {
    return morseDigitMapping[c-'0'];
  }
  else
  {
    return "";
  }
}

long getBeepDuration(char type)
{
  switch(type)
  {
    case LONG_BEEP:
      return PERIOD*3;
    case SHORT_BEEP:
    default:
     return PERIOD;
    }
}
