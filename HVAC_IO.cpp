#include "hvac.h"

uint32_t y,z ;
bool x=0,cool=0, modo=0;
void UART_initPD(){
    prueba = new UART;
    prueba->UART_inicializacion();
}

void UART_initUSR(uint32_t uart_1, uint32_t puerto, uint32_t pines_puerto, uint32_t clk, bool transmision,
                  bool bits, bool paridad, uint8_t baudios, bool comunicacion, bool sobremuestreo,
                  bool longitud, bool interrupcion1, bool interrupcion2){

    prueba = new UART(uart_1, puerto, pines_puerto,  clk, transmision,
                  bits,  paridad,  baudios,  comunicacion,  sobremuestreo,
                   longitud,  interrupcion1,  interrupcion2);
    prueba->UART_inicializacion();
}

void INT_OnOff(void){
    GPIO_clear_interrupt_flag(P2,B4);
 x=!x;
}

void INT_MODO(void){
    modo =! modo;
}

void INT_COOL(void){
    cool =! cool;
}


void S_OUT(void){
    z=1;
}

void S_IN(void){
    y=1;
}

uint32_t getVarOnOff(void){
    return x;
}

uint32_t getVarS_IN(void){
    return y;
}

uint32_t getVarS_OUT(void){
    return z;
}

uint32_t getVarMODO(void){
    return modo;
}

uint32_t getVarCOOL(void){
    return cool;
}


void setVarS_IN(void){
    y=0;
}

void setVarS_OUT(void){
    z=0;
}

void setVarMODO(void){
    modo=0;
}

void setVarCOOL(void){
    cool=0;
}

void INT_SWI(void)
{
    GPIO_clear_interrupt_flag(P1,B6); // Limpia la bandera de la interrupción.
    GPIO_clear_interrupt_flag(P1,B7); // Limpia la bandera de la interrupción.
    GPIO_clear_interrupt_flag(P1,B1); // Limpia la bandera de la interrupción.
    GPIO_clear_interrupt_flag(P1,B4); // Limpia la bandera de la interrupción.

    if(!GPIO_getInputPinValue(1,BIT(B6)))        // Si se trata del botón para aumentar setpoint (SW1).
        S_IN();
    else if(!GPIO_getInputPinValue(1,BIT(B7))) // Si se trata del botón para disminuir setpoint (SW2).
        S_OUT();
    else if(!GPIO_getInputPinValue(1,BIT(B1))) // Si se trata del botón para disminuir setpoint (SW2).
        INT_MODO();
    else if(!GPIO_getInputPinValue(1,BIT(B4))) // Si se trata del botón para disminuir setpoint (SW2).
        INT_COOL();
    return;
}
/*FUNCTION******************************************************************************
*
* Function Name    : HVAC_InicialiceIO
* Returned Value   : None.
* Comments         :
*    Controla los preparativos para poder usar las entradas y salidas GPIO.
*
*END***********************************************************************************/
void Inicializar_GPIO(void)
{
    // Para entradas y salidas ya definidas en la tarjeta.
    GPIO_init_board();

    // Modo de interrupción de los botones principales.
    GPIO_interruptEdgeSelect(1,BIT(B6),   GPIO_HIGH_TO_LOW_TRANSITION);
    GPIO_interruptEdgeSelect(1,BIT(B7), GPIO_HIGH_TO_LOW_TRANSITION);
    GPIO_interruptEdgeSelect(1,BIT(B1),   GPIO_HIGH_TO_LOW_TRANSITION);
    GPIO_interruptEdgeSelect(1,BIT(B4), GPIO_HIGH_TO_LOW_TRANSITION);
    GPIO_interruptEdgeSelect(2,BIT4, GPIO_HIGH_TO_LOW_TRANSITION);


    // Preparativos de interrupción.
    GPIO_clear_interrupt_flag(P1,B6);
    GPIO_clear_interrupt_flag(P1,B7);
    GPIO_clear_interrupt_flag(P2,B4);
    GPIO_clear_interrupt_flag(P1,B1);
    GPIO_clear_interrupt_flag(P1,B4);


    GPIO_enable_bit_interrupt(P1,B6);
    GPIO_enable_bit_interrupt(P1,B7);
    GPIO_enable_bit_interrupt(P2,B4);
    GPIO_enable_bit_interrupt(P1,B1);
    GPIO_enable_bit_interrupt(P1,B4);

    // Se necesitan más entradas, se usarán las siguientes:
    GPIO_setBitIO(2, 3, ENTRADA);
    GPIO_setBitIO(2, 4, ENTRADA);
    GPIO_setBitIO(2, 5, ENTRADA);
    GPIO_setBitIO(2, 6, ENTRADA);
    GPIO_setBitIO(2, 7, ENTRADA);
    GPIO_setBitIO(1, 1, ENTRADA);
    GPIO_setBitIO(1, 4, ENTRADA);


    /* Uso del módulo Interrupt para generar la interrupción general y registro de esta en una función
    *  que se llame cuando la interrupción se active.      */

    Int_registerInterrupt(INT_PORT1, INT_SWI); //Interrupcion puerto 1
    Int_enableInterrupt(INT_PORT1);

    Int_registerInterrupt(INT_PORT2, INT_OnOff);  //Interrupcion puerto 2
    Int_enableInterrupt(INT_PORT2);

    //Int_registerInterrupt(INT_PORT3, INT_P3);  //Interrupcion puerto 3
    //Int_enableInterrupt(INT_PORT3);

    //Int_registerInterrupt(INT_PORT4, INT_P4);  //Interrupcion puerto 4
    //Int_enableInterrupt(INT_PORT4);
}



