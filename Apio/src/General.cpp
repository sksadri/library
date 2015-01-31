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
}

ApioGeneral::~ApioGeneral() { }

void ApioGeneral::setup() {
  ApioClass::setup();
}

void ApioGeneral::loop() {
}


