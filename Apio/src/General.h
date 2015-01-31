/**************************************************************************\
* Title:  Apio Library                                                     *
* Author: Alesssandro Chelli                                               *
* https://github.com/ApioLab/library                                       *
* Copyright (c) 2014-2015, Apio srl. All rights reserved.                  *
* ------------------------------------------------------------------------ *
*  This program is free software; you can redistribute it and/or modify it *
*  under the terms of the BSD License as described in license.txt.         *
\**************************************************************************/

#ifndef LIB_APIO_GENERAL_H_
#define LIB_APIO_GENERAL_H_

#include <Apio.h>
#include <Wire.h>

#include "lwm/phy/phy.h"
#include "lwm/hal/hal.h"
#include "lwm/sys/sys.h"
#include "lwm/nwk/nwk.h"

#define ARRAY_LENGTH 50
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

class ApioGeneral : public ApioClass {

  public:
    ApioGeneral();
    ~ApioGeneral();

    String deviceAddr;
    String property; // variables that are to be processed in the running loop
    String value;  // variables that are to be processed in the running loop
    String propertyArray[ARRAY_LENGTH];
    String valueArray[ARRAY_LENGTH];
    int numberkey=0;
    int j=0;

    int flag; //flag which manages the logic of the select
    int x=0;//is used to keep track the running property:value in the loop

    void setup(int address);
    void loop();
    void apioSend(String toSend);
    void decodeIncomingString(String incoming);
  private:
    
    void select();
};

extern ApioGeneral General;

#endif
