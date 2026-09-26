#include <p24Fxxxx.h>
//#include <PPS.h>

void PeripheralsMap__GeneralMapping(void) {
    //Uart1
    /*
    OUT_PIN_PPS_RP25	=OUT_FN_PPS_U1TX;		// TX1  at RP25
    IN_FN_PPS_U1RX		=IN_PIN_PPS_RP20;		// RX1  at RP20
    //SPI1
    OUT_PIN_PPS_RP4     =OUT_FN_PPS_SDO1;		// SDO1 at RP19
    OUT_PIN_PPS_RP3     = 0x08;	// SCK1OUT at RP3
    IN_FN_PPS_SDI1		=IN_PIN_PPS_RP2;		// SDI  at RP2
     */
    _RP25R = _RPOUT_U1TX; // TX1  at RP25
    _U1RXR = 20; // RX1  at RP20
    //SPI1
    _RP4R = _RPOUT_SDO1; // SDO1 at RP19
    _RP3R = _RPOUT_SCK1OUT; // SCK1OUT at RP3
    _SDI1R = 2; // SDI  at RP2
}



