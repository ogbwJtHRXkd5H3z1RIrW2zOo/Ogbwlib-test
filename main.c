#include <stdio.h>
#include <stdlib.h>
#include "Ogbwlib.h"
#include "modules/UART1.h"

_FBS(BWRP_WRPROTECT_OFF & RBS_NO_BOOT_RAM & BSS_NO_BOOT_CODE)
_FSS(SWRP_WRPROTECT_OFF & RSS_NO_SEC_RAM & SSS_NO_SEC_CODE)
_FGS(GWRP_OFF & GSS_OFF)
_FOSCSEL(FNOSC_FRCPLL & IESO_OFF)
_FOSC(FCKSM_CSDCMD & IOL1WAY_OFF & OSCIOFNC_ON & POSCMD_NONE)
_FWDT(FWDTEN_OFF)
_FPOR(PWMPIN_ON & HPOL_ON & LPOL_ON & FPWRT_PWR1 & ALTI2C_OFF)
_FICD(JTAGEN_OFF & ICS_PGD1)

void UART1_test(void);
//void LinkedList_test(void);
//void ByteTabbedFIFO_test(void);
//void ByteFIFO_test(void);
//void ByteLIFO_test(void);
//void ObjectLIFO_test(void);

void wait(U32 delay, U16 z) {
    U32 i,j;
    for(j=0;j<5;j++) {
        for (i = 0; i < delay; i++);
        LATA ^= z;
    }
    SetRAOutput(0);
}



int main(void) {
    oscConfig();
    SetRAAsOutput();
    wait(2000000,31);
    UART1_init(0,1500,7,1,500,6,38400,7);
    wait(2000000,20);
    UART1_sendStr("c'est parti\r\n");

    UART1_test();
    //LinkedList_test();
    //ByteTabbedFIFO_test();
    //ByteFIFO_test();
    //ByteLIFO_test();
    //ObjectLIFO_test();
    

    while(1);
}
