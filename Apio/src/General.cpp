/**************************************************************************\
* Title:  Apio Library                                                     *
* Author: Alesssandro Chelli                                               *
* https://github.com/ApioLab/library                                       *
* Copyright (c) 2014-2015, Apio srl. All rights reserved.                  *
* ------------------------------------------------------------------------ *
*  This program is free software; you can redistribute it and/or modify it *
*  under the terms of the BSD License as described in license.txt.         *
\**************************************************************************/

#include <Arduino.h>
#include <Wire.h>
#include <General.h>
#include <math.h>
#include <avr/eeprom.h>
#include <avr/wdt.h>

static bool nwkDataReqBusy= false;
static void appDataConf(NWK_DataReq_t *req);
static bool apioReceive(NWK_DataInd_t *ind);

ApioGeneral General;


ApioGeneral::ApioGeneral() {
}

ApioGeneral::~ApioGeneral() { }

void ApioGeneral::setup(int address) {
  wdt_disable();
  SYS_Init();
  //Figure out how to use EEPROM
  //ApioClass::setup();
  ApioClass::nwkSetRadio(address,0x01,0x1a);
  ApioClass::nwkListen(1, apioReceive);
  wdt_enable(WDTO_8S);
}

void ApioGeneral::loop() {
  SYS_TaskHandler();
  ApioGeneral::select();
}

void ApioGeneral::decodeIncomingString(String incoming) {
  
  int strlen=incoming.length();
  //Serial1.println(incoming); //debug
  int i; //counter
  deviceAddr=""; 
  for(i=0; i<strlen ; i++)
  {
    if(incoming.charAt(i)=='-')
      numberkey++;
  }
  //Serial1.println(numberkey);
  //-----------deviceAddr----------------  
  
  for(i=0; incoming.charAt(i)!=':' && i<strlen ;i++)
  {
    deviceAddr += String(incoming.charAt(i));
  }

  for(j; j<numberkey ;j++)
  {
    //-----------property----------------

    for(i++; incoming.charAt(i)!=':' && i<strlen ;i++)
    {
      propertyArray[j] += String(incoming.charAt(i));
    }

    
    //-----------value----------------  
    
    for(i++; incoming.charAt(i)!='-' && i<strlen ;i++)
    {
      valueArray[j] += String(incoming.charAt(i)); 
    }
    
  }
}

static bool apioReceive(NWK_DataInd_t *ind) 
{ 
  int message_size=ind->size;
  int i;
  char Buffer[110];
  String receivedL="";
  for(i=0; i<message_size; i++)
  {
    Buffer[i] = ind->data[i];
    //delay(10);
    //Serial1.write(ind->data[i]);
   
  }

  General.decodeIncomingString(String(Buffer)); 
  
  for(int i=0; i<100; i++)
  {
    Buffer[i]=NULL;
    
  }
  return true; 
}

void ApioGeneral::select()
{

  if(x==numberkey && flag==1)
  {
    x=0;
    for(int k=0; k<numberkey; k++)
    {
      propertyArray[k]="";
      valueArray[k]="";
    }
    numberkey=0;
    j=0;
    flag=0;
    
  }
  if(numberkey!=0)
  {
    property=propertyArray[x];
    value=valueArray[x];
    x++;
    flag=1;
    //Serial1.println(property+":"+value);
  }
}

static void appDataConf(NWK_DataReq_t *req)
{
  //Serial1.print("ACK: "); //debug
  switch(req->status)
  {
    case NWK_SUCCESS_STATUS:
      //Serial1.print(1,DEC);
      break;
    case NWK_ERROR_STATUS:
      //Serial1.print(2,DEC);
      break;
    case NWK_OUT_OF_MEMORY_STATUS:
      //Serial1.print(3,DEC);
      break;
    case NWK_NO_ACK_STATUS:
      //Serial1.print(4,DEC);
      break;
    case NWK_NO_ROUTE_STATUS:
      //Serial1.print(5,DEC);
      break;
    case NWK_PHY_CHANNEL_ACCESS_FAILURE_STATUS:
      //Serial1.print(6,DEC);
      break;
    case NWK_PHY_NO_ACK_STATUS:
      //Serial1.print(7,DEC);
      break;
//    default:
//      Serial1.print("nessuna corrispondenza nell ack");
//      break;
     

  }
  nwkDataReqBusy = false;

  Serial1.println("");
  
}




