/**************************************************************************\
* Title:  Apio Dongle Library                                              *
* Author: Alesssandro Chelli                                               *
* https://github.com/ApioLab/library                                       *
* Copyright (c) 2014-2015, Apio srl. All rights reserved.                  *
* ------------------------------------------------------------------------ *
*  This program is free software; you can redistribute it and/or modify it *
*  under the terms of the BSD License as described in license.txt.         *
\**************************************************************************/

#ifndef LIB_APIO_Dongle_H_
#define LIB_APIO_Dongle_H_

#define DEL 1

#include <Apio.h>
#include <Wire.h>

#include "lwm/phy/phy.h"
#include "lwm/hal/hal.h"
#include "lwm/sys/sys.h"
#include "lwm/nwk/nwk.h"

#include "XBee.h"


// This is a temporary hack to check the result of snprintf and print an
// error
/*
#define snprintf(buf, size, ...) do { \
  if (size <= snprintf(buf, size, __VA_ARGS__)) { \
    Serial.print("snprintf overflow "); \
    Serial.print(__FILE__); \
    Serial.print(":"); \
    Serial.println(__LINE__); \
    Serial.flush(); \
  } \
} while(0)
*/


class ApioDongle : public ApioClass {

  public:
    ApioDongle();
    ~ApioDongle();

    XBee xbee = XBee();
    ZBRxResponse zbRx = ZBRxResponse(); //for the package in reception
    ZBTxRequest zbTx = ZBTxRequest(); 
    ZBTxStatusResponse txStatus = ZBTxStatusResponse(); // for the response of the xbee



    void setup();
    void loop();


    uint32_t getCpuTime();

  private:

    int contatore;
    String content;
    String deviceAddr="";
    char comunication_protocol=NULL;
    int lengthIncoming;

    void readFromWebServer();
    uint32_t stringHEX_To_uint32_t(String string);
    uint32_t power(int base, int esp);

    void xbeeSend(String deviceAddr, String content);
    void apioSend(String deviceAddr, String content);
};

extern ApioDongle Dongle;

#endif
