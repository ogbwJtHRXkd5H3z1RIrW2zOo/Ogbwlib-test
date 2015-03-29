#include <stdio.h>
#include "Ogbwlib.h"
#include "modules/UART1.h"

volatile U16 over=0;

void wait(U32 delay, U16 z);

/*
 * This function works as a test for both send and receive.
 * It should send the message "Send me some data : " then wait.
 * After a while, it dispalys how much data has been received, and how much have overflowed. The overflow number should always be 0 if the recieved number is not exactly equal to the size of the recieve buffer.
 * Then it prints the data you send, but with 1 added on each bit (therefore, 'a' becomes 'b', etc...). If the txBuffer is smaller than the rxBuffer+4, an overflow on txBuffer may occur, and no data is print
 * Warning : this function assumes that the rxBuffer is at most 996 bits wide. A bigger buffer may make the dsPIC crash. There is no assumption on the txBuffer size.
 * Warning : because it uses a no-test-classic U1RX_Interrupt, it can't be compiled if the uart.c file is compiled too. Use the 'exclude from build' option.
*/
void UART1_test(void) {

    wait(2000000,12);

    S8 uartRx[1000];
    U16 cnt, i;
    while(1) {
        UART1_sendStr("Send me some data : ");

        wait(3000000, 2);

        cnt = UART1_getRxBufferDataSize();
        U16 overtmp = over;
        over=0;
        UART1_printf("%u (%u)\r\n", cnt, overtmp);


        UART1_readTab(uartRx, cnt);
        for(i=0 ; i<cnt ; i++) {
            uartRx[i]++;
        }
        uartRx[cnt]='.';
        uartRx[cnt+1]='\r';
        uartRx[cnt+2]='\n';
        uartRx[cnt+3]=0;

        UART1_sendStr("Mirror+1 : ");

        //wait(1000000, 4);

        UART1_sendStr(uartRx);

        //wait(1000000, 1);

    }
}

void __attribute__((interrupt,no_auto_psv))_U1TXInterrupt(void)
{
    UART1_onU1TXInterrupt();
}

void __attribute__((interrupt,no_auto_psv))_U1RXInterrupt(void)
{
    if(UART1_onU1RXInterrupt() == UART_BUFFER_OVERFLOW) {
        over++;
    }
}