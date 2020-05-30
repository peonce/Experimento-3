

/*****************************************************************************************
Autor: Jos� David Ponce
Carn�: 18187
Laboratorio 10 Tiva C
Algunas funciones se basaron en los ejemplos subidos al repositorio de la clase por
el Ing. Pablo Mazariegos.
 *****************************************************************************************/

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "inc/TM4C123GH6PM.h"
#include "inc/hw_gpio.h"
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "inc/hw_ints.h"
#include "driverlib/fpu.h"
#include "driverlib/debug.h"
#include "driverlib/sysctl.h"
#include "driverlib/pin_map.h"
#include "driverlib/gpio.h"
#include "driverlib/uart.h"


void enviarColorCorrespondiente(void);

bool pulsado = 0;
bool pulsado2 = 0;
uint8_t color = 0;
uint8_t leds = 0;

int main(void)
{
    SysCtlClockSet(SYSCTL_SYSDIV_5 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN);
    //Se configura el reloj para que tenga una frecuencia de 40MHz
    //Es necesario desbloquear el bot�n SW2
    GPIO_PORTF_LOCK_R = 0x4C4F434B;
    GPIO_PORTF_CR_R = 0X00000001;
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    //Se activa el perif�rico del puerto F
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOF)){
        //Este ciclo espera que se termine de iniciar el perif�rico
    }
    //Luego se activan como outputs los pines 1 2 y 3 (LEDS del RGB)
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);
    //Es necesario configurar el pin del bot�n SW1 que es el pin 4 del puerto F como input
    GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_4| GPIO_PIN_0);
    //Ac� se configura la resistencia weak pull up para el pin del bot�n
    GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_4 | GPIO_PIN_0, GPIO_STRENGTH_4MA, GPIO_PIN_TYPE_STD_WPU);


    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART1);
    GPIOPinConfigure(GPIO_PC4_U1RX);
    GPIOPinConfigure(GPIO_PC5_U1TX);
    GPIOPinTypeUART(GPIO_PORTC_BASE, GPIO_PIN_4 | GPIO_PIN_5);
    UARTClockSourceSet(UART1_BASE, UART_CLOCK_PIOSC);
    UARTConfigSetExpClk(UART1_BASE, 16000000, 115200, (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));
    UARTEnable(UART1_BASE);
    //comando AT
    //delay

	while(1){
	    if (!GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_0)){
	        pulsado2 = 1;
	    }
	    if (!GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_4)){
            pulsado = 1;
	    }

	    if (GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_4)){
	        SysCtlDelay(100);
	        if (pulsado == 1){
	            color++;
	            color = color%8;
	            pulsado = 0;
	        }
	    }
	    if (GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_0)){
	        SysCtlDelay(100);
	        if (pulsado2 == 1){
	            enviarColorCorrespopndiente();
	            pulsado2 = 0;
	        }

	    }
	    leds= color<<1;
	    GPIOPinWrite(GPIO_PORTF_BASE, 0x0E, leds);

	}
}

void enviarColorCorrespopndiente(void){
    switch(color){
    case 1:
        //Enviar
    }
}
