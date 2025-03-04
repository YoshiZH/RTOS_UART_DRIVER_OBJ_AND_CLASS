 // FileName:        HVAC.h
 // Dependencies:    None.
 // Processor:       MSP432
 // Board:           MSP432P401R
 // Program version: CCS V8.3 TI
 // Company:         Texas Instruments
 // Description:     Incluye librer�as, define ciertas macros y significados as� como llevar un control de versiones.
 // Authors:         Jos� Luis Chac�n M. y Jes�s Alejandro Navarro Acosta.
 // Updated:         11/2018

#ifndef _hvac_h_
#define _hvac_h_


#pragma once

#define __MSP432P401R__
#define  __SYSTEM_CLOCK    48000000 // Frecuencias funcionales recomendadas: 12, 24 y 48 Mhz.

/* Archivos de cabecera importantes. */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/* Archivos de cabecera POSIX. */
#include <pthread.h>
#include <semaphore.h>
#include <ti/posix/tirtos/_pthread.h>
#include <ti/sysbios/knl/Task.h>

/* Archivos de cabecera para RTOS. */
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Event.h>

#include "Drivers/BSP.h"

// Definiciones B�sicas.
#define ENTRADA 1
#define SALIDA 0

// Re-definici�n de los bits y puertos de entrada a utilizar.
#define FAN_ON      B3
#define FAN_AUTO    B4
#define SYSTEM_COOL B5
#define SYSTEM_OFF  B6
#define SYSTEM_HEAT B7
#define SP_UP       B1
#define SP_DOWN     B4

#define FAN_PORT        2
#define SYSTEM_PORT     2
#define SETPOINT_PORT   1
#define FAN_PORTT        P2
#define SYSTEM_PORTT     P2
#define SETPOINT_PORTT   P1

#define TEMP_CH         CH0
#define HEARTBEAT_CH    CH1
#define POT_PIN         AN1

// Re-definici�n de los bits y puertos de salida a utilizar.
#define FAN_LED     BSP_LED1
#define HEAT_LED    BSP_LED2
#define HBeatLED    BSP_LED3
#define COOL_LED    BSP_LED4

#define FAN_LED_PORT    BSP_LED1_PORT
#define HEAT_LED_PORT   BSP_LED2_PORT
#define HB_LED_PORT     BSP_LED3_PORT
#define COOL_LED_PORT   BSP_LED4_PORT

// Definiciones del estado 'normal' de los botones externos a la tarjeta (solo hay dos botones).
#define GND 0
#define VCC 1
#define NORMAL_STATE_EXTRA_BUTTONS GND  // Aqui se coloca GND o VCC.

// Definiciones del sistema.
#define MAX_MSG_SIZE 64
#define MAX_ADC_VALUE 16383             // (2 ^14 bits) es la resoluci�n default.
#define MAIN_UART (uint32_t)(EUSCI_A0)
#define DELAY 20000
#define ITERATIONS_TO_PRINT 49
// Definici�n para el RTOS.
#define THREADSTACKSIZE1 1500
/* Funci�n de interrupci�n para botones de setpoint. */
extern void INT_SWI(void);
extern void INT_OnOff(void);
extern void INT_MODO(void);
extern void INT_COOL(void);
extern void INT_P3(void);
extern void INT_P4(void);

extern void Int_registerInterrupt(uint32_t interruptNumber,
        void (*intHandler)(void));

extern uint32_t getVarOnOff(void);
extern uint32_t getVarS_IN(void);
extern uint32_t getVarS_OUT(void);
extern uint32_t getVarCOOL(void);
extern uint32_t getVarMODO(void);

/*Funciones de inicializacion del UART*/
extern void UART_initPD(void);
extern void UART_initUSR(uint32_t uart_1, uint32_t puerto, uint32_t pines_puerto, uint32_t clk, bool transmision,
                         bool bits, bool paridad, uint8_t baudios, bool comunicacion, bool sobremuestreo,
                         bool longitud, bool interrupcion1, bool interrupcion2);

extern void setVarS_IN(void);
extern void setVarS_OUT(void);
extern void setVarCOOL(void);
extern void setVarMODO(void);

extern void S_IN(void);
extern void S_OUT(void);


#endif
