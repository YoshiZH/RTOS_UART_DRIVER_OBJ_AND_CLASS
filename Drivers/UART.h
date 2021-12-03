 // FileName:        UART.h
 // Dependencies:    msp432p401r.h
 // Processor:       MSP432
 // Board:           MSP432P401R
 // Program version: CCS V10.4 TI
 // Company:         Texas Instruments
 // Description:     Librerias, definiciones y clase para el modulo UART
 // Authors:         Jesus Joshua Muñoz Pacheco & Daniel Fernando Gonzales Lara
 // Updated:         11/2021

//#include <msp432p401r.h>

#ifndef DRIVERS_UART_H_
#define DRIVERS_UART_H_


///////////////////////////////////////////////////////
///////////////////////DEFINICIONES///////////////////
//////////////////////////////////////////////////////

/*Tipos de valores utilizados para el driver, si ya los tiene definidos
 * puede eliminarlos.*/

/*Longitud de los caracteres 8 o 7 bits*/
#define BIT_8 0
#define BIT_7 1

/*Interupciones*/
#define SIN_INTERRUPCION 0
#define CON_INTERRUPCION 1

/*Direccion de transmision*/
#define MSB 1
#define LSB 0

/*Bits de paro*/
#define UN_BIT 0
#define DOS_BITS 1

/*Sobremuestreo de datos*/
#define SOBREMUESTREO 1
#define SIN_SOBREMUESTREO 0

/*Relojes*/
#define UCLK 0
#define SMCLK 1
#define ACLK 2

/*Paridad*/
#define SIN_PARIDAD 0
#define PAR 1

/*Sinconizacion*/
#define ASINCRONO 0
#define SINCRONO 1

/*Definicion de modulo*/
#define UART_1 (uint32_t)(EUSCI_A0)

/*Puertos y pines*/
#define PUERTO_1 (0x40004C00)
#define PINES_PUERTO_1 ( 0x0001 << 2 | 0x0001 << 3) //PINES 1.2 Y 1.3
#define PUERTO_2 (0x40004C01)
#define PINES_PUERTO_2 (0xC) //PINES 2.2 Y 2.3
#define PUERTO_3 (0x40004C20)
#define PINES_PUERTO_3 (0xC) //PINES 3.2 Y 3.3
#define PUERTO_9 (0x40004C80)
#define PINES_PUERTO_9 (0xC0)//PINES 9.2 Y 9.3



/*Macrofunciones*/

#define EUSCI_A(x) ((EUSCI_A_Type *) x)
#define EUSCI_B(x) ((EUSCI_B_Type *) x)

/*Maximas definiciones de baudaje*/
#define MAX_BAUD_RATE_CHANNELS 5

/*Tipos de baudaje*/
#define BR_9600 0
#define BR_19200 1
#define BR_38400 2
#define BR_57600 3
#define BR_115200 4


/*Constantes para la determinacion del baudaje*/
const uint32_t BRX[]  = {78, 39, 19, 13, 6};
const uint32_t BRFX[] = {2, 1, 8, 0, 8};
const uint32_t BRS[]  = {0x0000, 0x0000, 0x0065, 0x0025, 0x0020};

/**************************************************************************
* Clase: UART
* Preconditions: Utilizar C++
* Overview: Esta clase define y engloba los aspectos de inicializacion y control
*           del modulo UART
* Input:  Ninguno.
* Output: Ninguno.
*
*****************************************************************************/
class UART{
    private:
        bool transmision,bits,paridad,comunicacion,sobremuestreo,longitud,interrupcion1,interrupcion2;
        uint32_t puerto, pines_puerto,clk,uart_1;
        uint8_t baudios;

    public:
        UART(uint32_t uart_1, uint32_t puerto, uint32_t pines_puerto, uint32_t clk, bool transmision,
                    bool bits, bool paridad, uint8_t baudios, bool comunicacion, bool sobremuestreo,
                    bool longitud, bool interrupcion1, bool interrupcion2);/*<- Constructor con caracteristicas
                                                                                definidas por el usuario.*/

        UART(); //<- Constructor con caracteristicas predeterminadas

        void UART_inicializacion (void); // <- Funcion De inicializacion de uart.

        void UART_seleccion_clock (uint32_t clock); // <- Elige el tipo de clock que se utilizara.

        void UART_comunicacion (bool sync); // <- Cambia el modo de comunicacion.

        void UART_longitud_caracteres (bool lg); // <- Tamaño de bits de datos 8 o 7 bits.

        void UART_paridad(bool paridad); // <- Tipo de paridad de los datos.

        void UART_baudios (uint8_t baudios); // <- Seleccion de baudios.

        void UART_sobremuestreo(bool tipo); //<- Tipo de sobremuestreo.

        void UART_bits_de_paro (bool bits); // <- Selecciona cuantos bits de paro se necesitan.

        void UART_puertos_pines(uint32_t puerto, uint32_t pines); // Seleccion del puerto del UART, Puerto 1,2,3 o 9.

        void UART_desactivacion(uint32_t uart); //<- Desactivacion del modulo UART.

        void UART_activacion(uint32_t uart); //<- Activacion del modulo UART.

        void UART_transmision(bool orden); // <- MSB o LSB.

        void UART_interrupcion_PCE(bool interrupcion); //<- Habilitacion de interrupcion por caracteres erroneos.

        void UART_interrupcion_PB(bool interrupcion); //<- Habilitacion de interrupcion por caracteres break.

        void UART_enviarByte(uint32_t uart, char byte); //<- Enviar un caracter al buffer de transmision.

        void UART_sPrintf(uint32_t uart, char *c); //<- Enviar una serie de caracteres al buffer de transmision.

        void UART_Limpiar(void); //<- Limpia todas las interupciones banderas, etc.
};

#endif
