/**************************************************************************\
* Title:  Apio Library                                                     *
* Author: Alesssandro Chelli                                               *
* https://github.com/ApioLab/library                                       *
* Copyright (c) 2014-2015, Apio srl. All rights reserved.                  *
* ------------------------------------------------------------------------ *
*  This program is free software; you can redistribute it and/or modify it *
*  under the terms of the BSD License as described in license.txt.         *
\**************************************************************************/

#ifndef LIB_APIO_H_
#define LIB_APIO_H_

//#define Apio_DEBUG
#ifdef APIO_DEBUG
#  define D(x) x
#else
#  define D(x)
#endif

#include <Arduino.h>

#include "lwm/sys/sysConfig.h"
#include "lwm/phy/phy.h"
#include "lwm/hal/hal.h"
#include "lwm/sys/sys.h"
#include "lwm/nwk/nwk.h"
#include "lwm/sys/sysTimer.h"
#include "avr/sleep.h"

class ApioClass {

  public:
    ApioClass();
    ~ApioClass();

    void setup();
    void loop();

    void goToSleep(uint32_t sleepForMs);

    const char* getLastResetCause();
    void loadSettingsFromEeprom();

    uint32_t getHwSerial();
    uint16_t getHwFamily();
    uint8_t getHwVersion();
    uint8_t getEEPROMVersion();

    void sendStateToHQ();

    void nwkSetRadio(const uint16_t theAddress, const uint16_t thePanId=0x4567, const uint8_t theChannel=20);
    void nwkSetPower(const uint8_t theTxPower);
    void nwkSetDataRate(const uint8_t theRate);
    void nwkSetSecurityKey(const uint8_t *key);
    void nwkGetSecurityKey(char *key);
    void nwkResetSecurityKey(void);
    void nwkListen(uint8_t endpoint, bool (*handler)(NWK_DataInd_t *ind));

    void nwkJoinGroup(uint16_t groupAddress);
    void nwkLeaveGroup(uint16_t groupAddress);
    bool nwkIsInGroup(uint16_t groupAddress);

    uint16_t getAddress();
    uint16_t getPanId();
    uint8_t getChannel();
    uint8_t getTxPower();
    const char* getTxPowerDb();
    uint8_t getDataRate();
    const char* getDataRatekbps();


  protected:
    void convertLongToBytes(byte *convBytes, uint32_t target);
    uint32_t convertBytesToLong(byte *convBytes);

    uint8_t lastResetCause;
    uint16_t address;
    uint16_t panId;
    uint8_t channel;
    uint8_t txPower;
    uint8_t dataRate;
};

#endif
