/******************************************************************************
TCC.h
Humberto Kramm
Novembro 27, 2022
******************************************************************************/

#include <stdio.h>

#include "adc.h"
#include "HT_P2P_app.h"
#include "RV_3032.h"

#ifndef TCC_H
#define TCC_H

//****************************************************************************//
//
//  Variables
//
//****************************************************************************//

//#define

//****************************************************************************//
//
//  Functions
//
//****************************************************************************//
void TCC_Radio();
void TCC_Transmit();

void _printbits(uint32_t v,uint8_t len);

void clearData();
void saveData(uint32_t payload);
void printMemory();

uint16_t readVoltage();
#endif
