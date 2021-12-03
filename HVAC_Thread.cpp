
#include "HVAC.h"
#define NUM_MAX_PERSONAS 5
#define SETPOINT 30
#define CONTROL PLUS

void apagado(void);
void encendido(void);
int i;
uint32_t P_Dentro;
float lectura;
char buffer[50];
////////////////////////////Dispara el ADC y espera la conversion/////////////////////////
void Dispara_ADC(void){
     ADC_trigger();
     while(ADC_is_busy());
}
//////////////////////////Devuelve el valor del canal que ocurrio por la conversion/////////////
int ReadADC(uint8_t channel)
{ return ADC_result(channel); }

void ConvToNum(void){
    lectura=(lectura*100)/16000;
}

void alarma(void){
    uint32_t conteo=0;
    P2->OUT &= ~0x05;
    while(conteo<=1000){
        P2->OUT |= 0x01;
        for(i=0;i<10000;i++);
        P2->OUT &= ~0x05;
        P2->OUT |= 0x04;
        for(i=0;i<10000;i++);
        P2->OUT &= ~0x05;
        conteo++;
    }

}
UART *prueba = new UART;

void *HVAC_Thread(void *arg0)
{
    /*INICIALIZANDO COMPONENTES*/
     SystemInit();
     Inicializar_GPIO();
     prueba->UART_Limpiar();
     prueba->UART_inicializacion();
     Inicializar_ADC();
     bool x;
     while(1){
         x=getVarOnOff();
         if(x==0){
             apagado();
         }
         else if(x==1){
             encendido();
         }
     }
}

void apagado(void){
    bool z=0;
    P1->OUT &= ~0x01;

    sprintf(buffer,"\r                                             ");
    prueba->UART_sPrintf(UART_1,buffer);
    sprintf(buffer,"\rSystem: OFF");
    prueba->UART_sPrintf(UART_1,buffer);

    while(z==0){
        z=getVarOnOff();
    }
}

#if CONTROL == PLUS


//MODO: AUTO -> 0
//MODO: ON -> 1

//COOL -> 0
//HEAT -> 1


void encendido(void){
    bool modo,cool, control1=1, control2=1;
    bool z,anterior=0,ahora,before=0,now;
    z=1;
    P1->OUT |= 0x01;
    while(z==1){
        //OBTENCION DE VALORES PARA EL ENCENDIDO / APAGADO, ENTRADAS Y SALIDAS DE LA TIENDA
        z=getVarOnOff();
        modo=getVarMODO();
        cool=getVarCOOL();
        //LOGICA DE EVENTOS
        ahora=modo;
        if(anterior!=ahora){
            control1=1;
        }
        anterior = modo;

        now=cool;
        if(before!=now){
            control2=1;
        }
        before = cool;
        //ACCIONES DEL MODO PLUS
         if(modo == 0 && control1 == 1){
            sprintf(buffer,"\rMODO: AUTO.");
            prueba->UART_sPrintf(UART_1,buffer);
            for(i=0;i<1000000;i++);
            sprintf(buffer,"\r                                             ");
            control1 = 0;
        }

        else if(modo == 1 && control1 == 1){
            sprintf(buffer,"\rMODO: ON.");
            prueba->UART_sPrintf(UART_1,buffer);
            for(i=0;i<1000000;i++);
            sprintf(buffer,"\r                                             ");
            control1=0;
        }

        else if(cool == 0 && control2 == 1){
            sprintf(buffer,"\rCOOL");
            prueba->UART_sPrintf(UART_1,buffer);
            for(i=0;i<1000000;i++);
            sprintf(buffer,"\r                                             ");
            control1 = 0;
        }

        else if(cool == 1 && control2 == 1){
            sprintf(buffer,"\rHEAT.");
            prueba->UART_sPrintf(UART_1,buffer);
            for(i=0;i<1000000;i++);
            sprintf(buffer,"\r                                             ");
            control1=0;
        }
        //MODO AUTO Y COOL
        else if(modo == 0 && cool == 0){

            sprintf(buffer,"\r                                                ");
            prueba->UART_sPrintf(UART_1,buffer);
            sprintf(buffer,"\rDetectando temperatura del edificio");
            prueba->UART_sPrintf(UART_1,buffer);
            for(i=0;i<10000000;i++);
            Dispara_ADC();
            lectura=ReadADC(CH0);
            ConvToNum();
            sprintf(buffer,"\r                                                 ");
            prueba->UART_sPrintf(UART_1,buffer);
            sprintf(buffer,"\rLa temperatura es de: %f",lectura);
            prueba->UART_sPrintf(UART_1,buffer);
            for(i=0;i<10000000;i++);
            sprintf(buffer,"\r                                                 ");
            prueba->UART_sPrintf(UART_1,buffer);

            if(lectura>SETPOINT){
                sprintf(buffer,"\rFAN: ON.");
                prueba->UART_sPrintf(UART_1,buffer);
                for(i=0;i<1000000;i++);
                sprintf(buffer,"\r                                             ");
            }
            else{
                sprintf(buffer,"\rFAN: OFF.");
                prueba->UART_sPrintf(UART_1,buffer);
                for(i=0;i<1000000;i++);
                sprintf(buffer,"\r                                             ");
            }

        }

        else if(modo == 0 && cool == 1){

            sprintf(buffer,"\r                                                ");
            prueba->UART_sPrintf(UART_1,buffer);
            sprintf(buffer,"\rDetectando temperatura del edificio");
            prueba->UART_sPrintf(UART_1,buffer);
            for(i=0;i<10000000;i++);
            Dispara_ADC();
            lectura=ReadADC(CH0);
            ConvToNum();
            sprintf(buffer,"\r                                                 ");
            prueba->UART_sPrintf(UART_1,buffer);
            sprintf(buffer,"\rLa temperatura es de: %f",lectura);
            prueba->UART_sPrintf(UART_1,buffer);
            for(i=0;i<10000000;i++);
            sprintf(buffer,"\r                                                 ");
            prueba->UART_sPrintf(UART_1,buffer);

            if(lectura<SETPOINT){
                sprintf(buffer,"\rFAN: ON.");
                prueba->UART_sPrintf(UART_1,buffer);
                for(i=0;i<1000000;i++);
                sprintf(buffer,"\r                                             ");
            }
            else{
                sprintf(buffer,"\rFAN: OFF.");
                prueba->UART_sPrintf(UART_1,buffer);
                for(i=0;i<1000000;i++);
                sprintf(buffer,"\r                                             ");
            }

        }
        else if(modo == 1 && cool == 0){

                sprintf(buffer,"\rFAN: ON.");
                prueba->UART_sPrintf(UART_1,buffer);
                for(i=0;i<1000000;i++);
                sprintf(buffer,"\r                                             ");
        }

        else{
            sprintf(buffer,"\rFAN: OFF.");
            prueba->UART_sPrintf(UART_1,buffer);
            for(i=0;i<1000000;i++);
            sprintf(buffer,"\r                                             ");
        }
    }
}

#else

void encendido(void){
    uint32_t y,x,conteo=0;
    bool aux=1,z;
    z=1;
    P1->OUT |= 0x01;
    while(z==1){
        z=getVarOnOff();
        y=getVarS_IN();
        x=getVarS_OUT();

        if(aux==1){
            sprintf(buffer,"\r                                             ");
            prueba->UART_sPrintf(UART_1,buffer);
            sprintf(buffer,"\rSystem: ON");
            prueba->UART_sPrintf(UART_1,buffer);
            for(i=0;i<10000000;i++);
            sprintf(buffer,"\r                                             ");
            prueba->UART_sPrintf(UART_1,buffer);
            aux=0;
        }

        if(y==1){
            conteo++;
            if(conteo>=NUM_MAX_PERSONAS+1){
                sprintf(buffer,"\r                                             ");
                prueba->UART_sPrintf(UART_1,buffer);
                sprintf(buffer,"\rWe are Full, wait.");
                prueba->UART_sPrintf(UART_1,buffer);
                for(i=0;i<1000000;i++);
                sprintf(buffer,"\r                                             ");
                prueba->UART_sPrintf(UART_1,buffer);
                conteo--;
            }
            else{
                sprintf(buffer,"\r                                                ");
                prueba->UART_sPrintf(UART_1,buffer);
                sprintf(buffer,"\rAcerque su cabeza al termometro");
                prueba->UART_sPrintf(UART_1,buffer);
                for(i=0;i<10000000;i++);
                Dispara_ADC();
                lectura=ReadADC(CH0);
                ConvToNum();
                sprintf(buffer,"\r                                                 ");
                prueba->UART_sPrintf(UART_1,buffer);
                sprintf(buffer,"\rSu temperatura es de: %f",lectura);
                prueba->UART_sPrintf(UART_1,buffer);
                for(i=0;i<10000000;i++);
                sprintf(buffer,"\r                                                 ");
                prueba->UART_sPrintf(UART_1,buffer);
                if(lectura<=37.5){+
                    sprintf(buffer,"\r                                             ");
                    prueba->UART_sPrintf(UART_1,buffer);
                    sprintf(buffer,"\rDOOR: OPEN");
                    prueba->UART_sPrintf(UART_1,buffer);
                    for(i=0;i<10000000;i++);
                    sprintf(buffer,"\r                                             ");
                    prueba->UART_sPrintf(UART_1,buffer);
                }
                else{
                    sprintf(buffer,"\r                                             ");
                    prueba->UART_sPrintf(UART_1,buffer);
                    sprintf(buffer,"\rTemp out of range");
                    prueba->UART_sPrintf(UART_1,buffer);
                    for(i=0;i<1000000;i++);
                    alarma();
                    sprintf(buffer,"\r                                             ");
                    prueba->UART_sPrintf(UART_1,buffer);
                    conteo--;
                }
            }
            setVarS_IN();
        }
        else if(x==1){
            if(conteo==0){
                conteo=0;
            }
            else{
                conteo--;
            }


            setVarS_OUT();
        }

        else if(y==0&&x==0){

            sprintf(buffer,"\rDOOR: CLOSE");
            prueba->UART_sPrintf(UART_1,buffer);
        }
    }
}
#endif


