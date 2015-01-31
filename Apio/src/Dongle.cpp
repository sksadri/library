/**************************************************************************\
* Title:  Apio Dongle Library                                              *
* Author: Alesssandro Chelli                                               *
* https://github.com/ApioLab/library                                       *
* Copyright (c) 2014-2015, Apio srl. All rights reserved.                  *
* ------------------------------------------------------------------------ *
*  This program is free software; you can redistribute it and/or modify it *
*  under the terms of the BSD License as described in license.txt.         *
\**************************************************************************/

#include <Arduino.h>
#include <Wire.h>
#include <Dongle.h>
#include "SleepHandler.h"
#include <math.h>
#include <avr/eeprom.h>
#include <avr/wdt.h>

#include "XBee.h"

static bool nwkDataReqBusy= false;
static void appDataConf(NWK_DataReq_t *req);
static bool apioReceive(NWK_DataInd_t *ind);

ApioDongle Dongle;


ApioDongle::ApioDongle() {
  contatore=0;
  content="";
  deviceAddr="";
  comunication_protocol=NULL;
  
}

ApioDongle::~ApioDongle() { }

void ApioDongle::setup() {
  wdt_disable();
  Serial.begin(115200);
  Serial1.begin(9600);//for comunicate with  xbee
  xbee.setSerial(Serial1);
  SYS_Init();
  //Figure out how to use EEPROM
  //ApioClass::setup();
  ApioClass::nwkSetRadio(0,0x01,0x1a);
  ApioClass::nwkListen(1, apioReceive);
  wdt_enable(WDTO_8S);
}

void ApioDongle::loop() {
  SYS_TaskHandler(); //task management LWM
  lengthIncoming=Serial.available();
  if(lengthIncoming)
  {
    ApioDongle::readFromWebServer();
    //Serial.println(Serial.available());  
    //Serial.println(content);
    comunication_protocol=content[0];       
    switch(comunication_protocol)
    {
      case('z'):
        ApioDongle::xbeeSend(deviceAddr, content); 
        break;
        
      case('l'):
        ApioDongle::apioSend(deviceAddr,content); 
        break;
    }
    Serial.println(content);        
    content="";
    deviceAddr="";
    comunication_protocol=NULL;
    contatore++;
   } 
   if(contatore>1000)
   {
     wdt_enable(WDTO_30MS);
     while(true)
     {
     }
   }
     
 /*--------------------------------------------------------------------*/ 
  wdt_reset(); //watchdog reset

}

void ApioDongle::xbeeSend(String devAddr, String toSend)
{   
  //Serial.println(toSend);
  char toSend_char[100]; //beacause the ZBTxRequest needs a uint8_t*
  toSend.toCharArray(toSend_char, 100);
  
  uint32_t indirizzo = stringHEX_To_uint32_t(devAddr);
  
  XBeeAddress64 addr64 = XBeeAddress64(0x0013A200, indirizzo);
  
  zbTx = ZBTxRequest(addr64, (uint8_t*)(toSend_char), strlen(toSend_char)); 
  
  xbee.send(zbTx);
}


void ApioDongle::apioSend(String devAddr,String toSend) 
{

  int len = toSend.length();

  char sendThis[len];
  for(int g=0; g<len ;g++) 
  {
      sendThis[g]=toSend.charAt(g);
  }
  nwkDataReqBusy = true; 
  NWK_DataReq_t *message = (NWK_DataReq_t*)malloc(sizeof(NWK_DataReq_t));
  message->dstAddr = devAddr.toInt(); //object address
  message->dstEndpoint = 1; 
  message->srcEndpoint = 1;
  message->options = NWK_OPT_ACK_REQUEST; //request an ack
  message->size = len;
  message->data = (uint8_t*)(sendThis);

  message->confirm = appDataConf; //callback for the management of the confirmation (option field)
                                  //and verification of ack required above
  NWK_DataReq(message); //send message

}

static bool apioReceive(NWK_DataInd_t *ind) 
{ 
  
  int message_size=ind->size;
  char bufferL[62];
  for(int i=0; i<message_size; i++)
  {
    //Buffer[i] = ind->data[i];
    bufferL[i] = ind->data[i];
  }
  Serial.println(String(bufferL)); 
  return true;
}

uint32_t ApioDongle::getCpuTime() {
  return millis();
}


static void appDataConf(NWK_DataReq_t *req)
{
  //Serial.print("ACK: "); //debug
  switch(req->status)
  {
    case NWK_SUCCESS_STATUS:
      Serial.print(1,DEC);
      break;
    case NWK_ERROR_STATUS:
      Serial.print(2,DEC);
      break;
    case NWK_OUT_OF_MEMORY_STATUS:
      Serial.print(3,DEC);
      break;
    case NWK_NO_ACK_STATUS:
      Serial.print(4,DEC);
      break;
    case NWK_NO_ROUTE_STATUS:
      Serial.print(5,DEC);
      break;
    case NWK_PHY_CHANNEL_ACCESS_FAILURE_STATUS:
      Serial.print(6,DEC);
      break;
    case NWK_PHY_NO_ACK_STATUS:
      Serial.print(7,DEC);
      break;
//    default:
//      Serial1.print("no correspondence in ack");
//      break;
     

  }
  nwkDataReqBusy = false;
  Serial.println();
}

void ApioDongle::readFromWebServer()
{
  int flagAddr=0;
  char buf;
  //delayMicroseconds(DEL);
  //comunication_protocol = Serial.read();
  //delayMicroseconds(DEL);
  for(int i=0; i<lengthIncoming; i++)
  {
    buf = Serial.read();
    content += buf;
    if(buf==':'){
      flagAddr=1;
    }
    if(flagAddr==0)
    {
      deviceAddr+=buf;
    }
    //delayMicroseconds(DEL);
    
  }
  //delayMicroseconds(DEL);
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
//does the power of two integers:I need because the 'pow' function had trouble rounding
//(pow(10,3)=999,838...) if I make a cast to int truncates the decimal

uint32_t ApioDongle::power(int base, int esp)
{
  int i;
  uint32_t result = base;
  if(esp == 0)
  {
    return 1;
  }
  for(i=1; i<esp; i++)
  {
    result *= base;
  }
  return result;
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
//convert a string made up of only numbers (representing a number in hexadecimal) in a data type uint32_t-----------
uint32_t ApioDongle::stringHEX_To_uint32_t(String string)
{
  uint32_t num = 0;
  char buf[12];
  int len = string.length();
  string.toCharArray(buf, 12);
  int i;
  
  for(i=len-1; i>=0; i--)
  {
    if(buf[i]>='0'  && buf[i]<='9') //if the character is a number(ascii value between 48('0') and 57('9') ) then subtract '0'
    {
    num += ( (buf[i] - '0')*power(16,len-i-1) ); 
    
    }
    
    
    if(buf[i]>='A'  && buf[i]<='F') //if the character is an uppercase letter invalid (ASCII value between 65('A') and 70('F') - than subtract 'A'
    {                               //then I add 10 because A (in hex)  is equal to 10 (alternatively I can subtract '7' = 'A'-10)
    num += ( (buf[i] - 'A' + 10)*power(16,len-i-1) ); 
    
    }
    
    
    if(buf[i]>='a'  && buf[i]<='f') //if the character is an lowercase letter invalid (ASCII value between 97('a') and 102('f') - than subtract 'a'
    {                               //then I add 10 because a (in hex) is equal to 10 (alternatively I can subtract 'W' = 'a'-10)
    num += ( (buf[i] - 'a' + 10)*power(16,len-i-1) ); 
    
    }
    
  }
  
  return num;
  
}

