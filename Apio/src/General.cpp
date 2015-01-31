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
#include "SleepHandler.h"
#include <math.h>
#include <avr/eeprom.h>

ApioGeneral General;


ApioGeneral::ApioGeneral() {
  sleepPending = false;
  General.postSleepCommand = NULL;
}

ApioGeneral::~ApioGeneral() { }

void ApioGeneral::setup() {
  ApioClass::setup();
}

void ApioGeneral::loop() {
  bool canSleep = true;
  // TODO: Let other loop functions return some "cansleep" status as well

  ApioClass::loop();
  if (sleepPending) {
    canSleep = canSleep && !NWK_Busy();

    int32_t remaining = sleepUntil - millis();

    // if remaining <= 0, we won't actually sleep anymore, but still
    // call doSleep to run the callback and clean up
    if (canSleep || remaining <= 0)
      doSleep(remaining);
  }
}


void ApioGeneral::scheduleSleep(uint32_t ms, char *cmd) {
  General.sleepUntil = millis() + ms;
  General.sleepPending = (ms > 0);
  if (General.postSleepCommand)
    free(General.postSleepCommand);
  General.postSleepCommand = cmd;
}

void ApioGeneral::doSleep(int32_t ms) {
  // Copy the pointer, so the post command can set a new sleep
  // timeout again.
  char *cmd = postSleepCommand;
  postSleepCommand = NULL;
  sleepPending = false;

  if (ms > 0) {
    NWK_SleepReq();

    // TODO: suspend more stuff? Wait for UART byte completion?

    SleepHandler::doSleep(ms, true);
    NWK_WakeupReq();
  }

}

uint32_t ApioGeneral::getWallTime() {
  return getCpuTime() + getSleepTime();
}

uint32_t ApioGeneral::getCpuTime() {
  return millis();
}

uint32_t ApioGeneral::getSleepTime() {
  return SleepHandler::sleepMillis;
}
