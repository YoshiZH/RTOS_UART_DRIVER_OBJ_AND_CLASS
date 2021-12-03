 // FileName:        UART.Cpp
 // Dependencies:    UART.h
 // Processor:       MSP432
 // Board:           MSP432P401R
 // Program version: CCS V10.4 TI
 // Company:         Texas Instruments
 // Description:     Definición de las funciones del UART
 // Authors:         Jesus Joshua Muñoz Pacheco & Daniel Fernando Gonzales Lara
 // Updated:         11/2021


#include "HVAC.h"

/**************************************************************************
* Constructor: UART
* Preconditions: Haber creado un puntero del tipo UART
* Overview: Este constructor es para el usuario si es que este no quiere inicializar
*           el UART de la manera predeterminada por los desarrolladores
* Input: _uart_1,_puerto, _pines_puerto, _clk, _transmision, _bits, _paridad
*        _baudios, _comunicacion, _sobremuestreo, _longitud, _interrupcion1 y
*        _interrupcion2
* Output: Ninguno.
*
*****************************************************************************/

UART::UART(uint32_t _uart_1, uint32_t _puerto, uint32_t _pines_puerto, uint32_t _clk, bool _transmision,bool _bits, bool _paridad, uint8_t _baudios, bool _comunicacion, bool _sobremuestreo,bool _longitud, bool _interrupcion1, bool _interrupcion2)
{

    uart_1 = _uart_1;
    puerto = _puerto;
    pines_puerto = _pines_puerto;
    clk = _clk;
    transmision = _transmision;
    bits = _bits;
    paridad = _paridad;
    baudios = _baudios;
    comunicacion = _comunicacion;
    sobremuestreo = _sobremuestreo;
    longitud = _longitud;
    interrupcion1 = _interrupcion1;
    interrupcion2 = _interrupcion2;

}
/**************************************************************************
* Constructor: UART
* Preconditions: Haber creado un puntero del tipo UART
* Overview: Constructor de inicializacion con configuraciones predeterminadas.
* Input:  Ninguno.
* Output: Ninguno.
*
*****************************************************************************/
UART::UART(){

    uart_1 = UART_1;
    puerto = PUERTO_1;
    pines_puerto = PINES_PUERTO_1;
    clk = SMCLK;
    transmision = LSB;
    bits = UN_BIT;
    paridad = SIN_PARIDAD;
    baudios = (uint8_t)BR_115200;
    comunicacion = ASINCRONO;
    sobremuestreo = SOBREMUESTREO;
    longitud = BIT_8;
    interrupcion1 = SIN_INTERRUPCION;
    interrupcion2 = SIN_INTERRUPCION;
}

/**************************************************************************
* Function: UART_enviarByte
* Preconditions: Haber creado un puntero del tipo UART o un objeto del mismo.
* Overview: Permite enviar un caracter al buffer de transmision del UART.
* Input:  uart -> Modulo que se utilizara, byte -> caracter a trasmitir.
* Output: Ninguno.
*
*****************************************************************************/
void UART::UART_enviarByte(uint32_t uart, char byte){

    if (!BITBAND_PERI(EUSCI_A(uart) -> IE, EUSCI_A_IE_TXIE_OFS))
         while (!BITBAND_PERI(EUSCI_A(uart) -> IFG, EUSCI_A_IFG_TXIFG_OFS));

     EUSCI_A(uart)->TXBUF = byte;
}

/**************************************************************************
* Function: UART_sPrintf
* Preconditions: Haber creado un puntero del tipo UART o un objeto del mismo.
* Overview: Permite enviar una serie de caracteres concatenados al buffer de
*           transmision del UART.
*
* Input:  uart -> Modulo que se utilizara, c -> cadena de carateres.
* Output: Ninguno.
*
*
* Nota.- Estas funciones, UART_enviarByte & UART_sPrintf pueden
* servir para el entendimiento y control del modulo UART pero es
* recomendable utilizar, si es posible, las funciones printf() y
* sprintf().
*****************************************************************************/
void UART::UART_sPrintf(uint32_t uart, char *c){
    char x;

    while(x = *c++){

        UART_enviarByte(uart, x);
    }
}
/**************************************************************************
* Function: UART_Limpiar
* Preconditions: Haber creado un puntero del tipo UART o un objeto del mismo.
* Overview: Permite limpiar los registros del modulo UART
* Input:  Ninguno.
* Output: Ninguno.
*
*****************************************************************************/
void UART::UART_Limpiar(){
    EUSCI_A(EUSCI_A0)-> CTLW0 = (EUSCI_A(EUSCI_A0)-> CTLW0 & ~( UCRXEIE | UCBRKIE | UCDORM | UCTXADDR | UCTXBRK)) | EUSCI_A_CTLW0_MODE_0;
}
/**************************************************************************
* Function: UART_desactivacion
* Preconditions: Haber creado un puntero del tipo UART o un objeto del mismo.
* Overview: Permite desactivar el modulo UART, manteniendolo en RESET.
* Input:  uart -> Modulo que se utilizara
* Output: Ninguno.
*
*****************************************************************************/
void UART::UART_desactivacion(uint32_t uart){
    BITBAND_PERI(EUSCI_A(uart) -> CTLW0, EUSCI_A_CTLW0_SWRST_OFS) = 1;
}

/**************************************************************************
* Function: UART_activacion
* Preconditions: Haber creado un puntero del tipo UART o un objeto del mismo.
* Overview: Permite activar el modulo UART.
* Input:  uart -> Modulo que se utilizara
* Output: Ninguno.
*
*****************************************************************************/
void UART::UART_activacion (uint32_t uart){
    BITBAND_PERI(EUSCI_A(uart) -> CTLW0, EUSCI_A_CTLW0_SWRST_OFS) = 0;
}
/**************************************************************************
* Function: UART_puertos_pines
* Preconditions: Haber creado un puntero del tipo UART o un objeto del mismo.
* Overview: Permite habilitar la configuracion de UART para los pines especificos
*           de los puertos correspondientes, solo existen 4 puertos que tienen pines
*           que permiten comunicacion serial.
* Input:  Puerto -> puerto que se usara, pines -> pines del puerto.
* Output: Ninguno.
*
*
** Nota.- Por defecto, y para comodidad del usuario, ya estan hechas las definiciones
** para los puertos y pines de esos puertos, pueden enontrarse en el archivo UART.h.
*****************************************************************************/
void UART::UART_puertos_pines(uint32_t puerto, uint32_t pines){
    HWREG16(puerto + OFS_PADIR) &= ~(pines);
    HWREG16(puerto + OFS_PASEL0) |= (pines);
    HWREG16(puerto + OFS_PASEL1) &= ~(pines);
}
/**************************************************************************
* Function: UART_inicializacion
* Preconditions: Haber creado un puntero del tipo UART o un objeto del mismo.
* Overview: Permite inicializar el uart de una manera especifica, para esto se
*           requiere de la utilizacion de uno de los dos constructores que se
*           definen y explican en la parte superior de este archivo.
* Input:  Ninguno.
* Output: Ninguno.
*****************************************************************************/
void UART::UART_inicializacion(){

    UART_desactivacion(uart_1);

    UART_puertos_pines (puerto, pines_puerto);

    UART_seleccion_clock (clk);

    UART_transmision (transmision);

    UART_bits_de_paro (bits);

    UART_paridad (paridad);

    UART_baudios ((uint8_t) baudios);

    UART_longitud_caracteres (longitud);

    UART_comunicacion (comunicacion);

    UART_sobremuestreo (sobremuestreo);

    UART_interrupcion_PB(interrupcion1);

    UART_interrupcion_PCE(interrupcion2);

    UART_activacion(uart_1);
}
/**************************************************************************
* Function: UART_seleccion_clock
* Preconditions: Haber creado un puntero del tipo UART o un objeto del mismo.
* Overview: Permite seleccionar el clock que se utilizara para el modulo UART.
* Input:  clock -> Clock seleccionado.
* Output: Ninguno.
*
** Nota.- Por defecto, y para comodidad del usuario, ya estan hechas las definiciones
** para los distintos tipos de clock, es preciso utilizar dichas definiciones, ademas
** tome en cuenta que dependiendo del clock seleccionado se vera afectada la manera
** en que el baudaje se toma en cuenta, por defecto el clock seleccionado es el SMCLK.
*****************************************************************************/
void UART::UART_seleccion_clock(uint32_t clock){
    switch(clock)
    {
        case UCLK:  EUSCI_A(EUSCI_A0) -> CTLW0 = (EUSCI_A(EUSCI_A0) -> CTLW0 & ~UCSSEL_3) | EUSCI_A_CTLW0_SSEL__UCLK;  break;
        case ACLK:  EUSCI_A(EUSCI_A0) -> CTLW0 = (EUSCI_A(EUSCI_A0) -> CTLW0 & ~UCSSEL_3) | EUSCI_A_CTLW0_SSEL__ACLK;  break;
        case SMCLK: EUSCI_A(EUSCI_A0) -> CTLW0 = (EUSCI_A(EUSCI_A0) -> CTLW0 & ~UCSSEL_3) | EUSCI_A_CTLW0_SSEL__SMCLK; break;
        default: break;
    }
}
/**************************************************************************
* Function: UART_transmision
* Preconditions: Haber creado un puntero del tipo UART o un objeto del mismo.
* Overview: Permite seleccionar si se tomara en cuenta primero el bit mas
*           significativo o el menos significativo.
* Input:  orden -> LSB o MSB.
* Output: Ninguno.
*****************************************************************************/
void UART:: UART_transmision (bool orden){
    BITBAND_PERI(EUSCI_A(EUSCI_A0)-> CTLW0, UCMSB_OFS) = orden;
}
/**************************************************************************
* Function: UART_bits_de_paro
* Preconditions: Haber creado un puntero del tipo UART o un objeto del mismo.
* Overview: Permite seleccionar cuantos bits de paro se requeriran.
* Input:  bits -> UN_BIT o DOS_BIT.
* Output: Ninguno.
*****************************************************************************/
void UART:: UART_bits_de_paro(bool bits){
    BITBAND_PERI(EUSCI_A(EUSCI_A0)-> CTLW0, UCSPB_OFS) = bits;
}
/**************************************************************************
* Function: UART_longitud_caracteres
* Preconditions: Haber creado un puntero del tipo UART o un objeto del mismo.
* Overview: Permite seleccionar la longitud de los datos para el modulo UART.
* Input:  lg -> BIT_8 o BIT_7.
* Output: Ninguno.
*****************************************************************************/
void UART:: UART_longitud_caracteres(bool lg){
    BITBAND_PERI(EUSCI_A(EUSCI_A0)-> CTLW0, UC7BIT_OFS ) = lg;
}
/**************************************************************************
* Function: UART_baudios
* Preconditions: Haber creado un puntero del tipo UART o un objeto del mismo.
* Overview: Permite seleccionar a cuantos baudios trabajara la comunicacion del
*           modulo UART
* Input:  baudios.
* Output: Ninguno.
** Nota.- Vea la funcion UART_seleccion_clock para mas informacion.
*****************************************************************************/
void UART::UART_baudios(uint8_t baudios){
    if(baudios<MAX_BAUD_RATE_CHANNELS){
        EUSCI_A(EUSCI_A0)-> BRW = BRX[baudios];
        EUSCI_A(EUSCI_A0)-> MCTLW = ((BRS[baudios] << 8)  + (BRFX[baudios] << 4) +  EUSCI_A_MCTLW_OS16);
    }
}
/**************************************************************************
* Function: UART_sobremuestreo
* Preconditions: Haber creado un puntero del tipo UART o un objeto del mismo.
* Overview: Permite seleccionar habra sobremuestreo o no.
* Input:  tipo -> SOBREMUESTREO o SIN_SOBREMUESTREO.
* Output: Ninguno.
*****************************************************************************/
void UART::UART_sobremuestreo(bool tipo){
    BITBAND_PERI(EUSCI_A(EUSCI_A0) -> MCTLW, EUSCI_A_MCTLW_OS16_OFS) = tipo;
}
/**************************************************************************
* Function: UART_comunicacion
* Preconditions: Haber creado un puntero del tipo UART o un objeto del mismo.
* Overview: Permite seleccionar la longitud de los datos para el modulo UART.
* Input:  lg -> BIT_8 o BIT_7.
* Output: Ninguno.
*****************************************************************************/
void UART::UART_comunicacion(bool sync){
    BITBAND_PERI(EUSCI_A(EUSCI_A0) -> CTLW0, EUSCI_A_CTLW0_SYNC) = sync;
}
/**************************************************************************
* Function: UART_paridad
* Preconditions: Haber creado un puntero del tipo UART o un objeto del mismo.
* Overview: Permite seleccionar si habra paridad y el tipo de paridad.
* Input:  paridad -> SIN_PARIDAD o PAR
* Output: Ninguno.
*****************************************************************************/
void UART:: UART_paridad(bool paridad){
    if(paridad == 1)
    {
        BITBAND_PERI(EUSCI_A(EUSCI_A0)-> CTLW0, UCPEN_OFS) = 1; // Paridad se habilita
        BITBAND_PERI(EUSCI_A(EUSCI_A0)-> CTLW0, UCPAR_OFS) = 0; // Si la paridad se habilita se debe deshabilitar el UCPAR
    }
    else
        BITBAND_PERI(EUSCI_A(EUSCI_A0)-> CTLW0, UCPEN_OFS) = 0; // Paridad deshabilitada
}
/**************************************************************************
* Function: UART_interrupcion_PB
* Preconditions: Haber creado un puntero del tipo UART o un objeto del mismo.
* Overview: Permite habilitar las interrupciones por caracteres erroneos.
* Input:  interrupcion -> SIN_INTERRUPCION o CON_INTERRUPCION
* Output: Ninguno.
*****************************************************************************/
void UART::UART_interrupcion_PB(bool interrupcion){
    BITBAND_PERI(EUSCI_A(EUSCI_A0) -> CTLW0, EUSCI_A_CTLW0_RXEIE_OFS) = interrupcion;
}
/**************************************************************************
* Function: UART_interrupcion_PCE
* Preconditions: Haber creado un puntero del tipo UART o un objeto del mismo.
* Overview: Permite habilitar las interrupciones por caracteres Break.
* Input:  interrupcion -> SIN_INTERRUPCION o CON_INTERRUPCION
* Output: Ninguno.
*****************************************************************************/
void UART::UART_interrupcion_PCE(bool interrupcion){
    BITBAND_PERI(EUSCI_A(EUSCI_A0) -> CTLW0, EUSCI_A_CTLW0_BRKIE_OFS) = interrupcion;
}

