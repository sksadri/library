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

    void setup();
    void loop();

    






    uint32_t getWallTime();
    uint32_t getCpuTime();
    uint32_t getSleepTime();

    // Schedule a sleep that lasts until now + ms. The optional bitlash
    // command is executed after the sleep and then free()'d. A previous
    // sleep can be canceled by passing 0, NULL.
    void scheduleSleep(uint32_t ms, char *cmd);

  protected:
    void checkStateChange();

    void doSleep(int32_t ms);

    bool sleepPending;
    uint32_t sleepUntil;
    char * postSleepCommand;
};

extern ApioGeneral General;

#endif
