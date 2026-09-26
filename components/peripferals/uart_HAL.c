#include <p24Fxxxx.h>
#include<stdbool.h>
#include "uart_24F_definitions.h"
#include "uart_HAL.h"

GETCHAR_CALLBACK g_GetchCallBack;

const unsigned int CONST_MODE = UART_IDLE_STOP & UART_IrDA_DISABLE & UART_MODE_FLOW & UART_UEN_00 & UART_DIS_WAKE & UART_DIS_LOOPBACK & UART_DIS_ABAUD & UART_UXRX_IDLE_ONE & UART_NO_PAR_8BIT&UART_1STOPBIT;
const unsigned int CONS_STA = UART_INT_TX_BUF_EMPTY & UART_IrDA_POL_INV_ZERO & UART_SYNC_BREAK_DISABLED & UART_TX_ENABLE & UART_INT_RX_CHAR & UART_ADR_DETECT_DIS&UART_RX_OVERRUN_CLEAR;

void HAL_UART__SetExternGetch(GETCHAR_CALLBACK getChar) {
    g_GetchCallBack = getChar;
}

void HAL_UART__TurnOff(UART_Channel_t uartChannel) {
    switch (uartChannel) {
        case UART_CH1:
            U1MODEbits.UARTEN = 0;
            break;
        case UART_CH2:
            U2MODEbits.UARTEN = 0;
            break;
        case UART_CH3:
            U3MODEbits.UARTEN = 0;
            break;
        case UART_CH4:
            U4MODEbits.UARTEN = 0;
            break;
        default:
            break;
    }

}

void HAL_UART__SerialSetup(UART_Speed_t serialSpeed, UART_Channel_t uartChannel) {

    unsigned int UART_value, BRG_value;
// calculated for 64mHz
    switch (serialSpeed) {
        case UART_SPEED_2400:
            UART_value = UART_BRGH_SIXTEEN;
            BRG_value = 207;
            break;
        case UART_SPEED_4800:
            UART_value = UART_BRGH_SIXTEEN;
            BRG_value = 103;
            break;
        case UART_SPEED_9600:
            UART_value = UART_BRGH_FOUR;
            BRG_value = 207;
            break;
        case UART_SPEED_38400:
            UART_value = UART_BRGH_FOUR;
            BRG_value = 51;
            break;
        case UART_SPEED_115200:
            UART_value = UART_BRGH_FOUR;
            BRG_value = 16;
            break;
        default:
            break;
    }
    switch (uartChannel) {
        case UART_CH1:
            U1MODE = UART_value&CONST_MODE;
            U1BRG = BRG_value;
            U1STA = CONS_STA;
            while (U1STAbits.URXDA == 1) /* Clear Buffer*/ {
                char Temp;
                Temp = U1RXREG;
            }
            IFS0bits.U1RXIF = 0; /*clear interrupt flag*/
            break;
        case UART_CH2:
            U2MODE = UART_value&CONST_MODE;
            U2BRG = BRG_value;
            U2STA = CONS_STA;
            while (U2STAbits.URXDA == 1) /* Clear Buffer*/ {
                char Temp;
                Temp = U2RXREG;
            }
            IFS1bits.U2RXIF = 0; /*clear interrupt flag*/
            break;
        case UART_CH3:
            U3MODE = UART_value&CONST_MODE;
            U3BRG = BRG_value;
            U3STA = CONS_STA;
            while (U3STAbits.URXDA == 1) /* Clear Buffer*/ {
                char Temp;
                Temp = U3RXREG;
            }
            IFS5bits.U3RXIF = 0; /*clear interrupt flag*/
            break;
        case UART_CH4:
            U4MODE = UART_value&CONST_MODE;
            U4BRG = BRG_value;
            U4STA = CONS_STA;
            while (U4STAbits.URXDA == 1) /* Clear Buffer*/ {
                char Temp;
                Temp = U4RXREG;
            }
            IFS5bits.U4RXIF = 0; /*clear interrupt flag*/
            break;
        default:
            break;
    }
}

bool HAL_UART__CheckAndResetErrors(UART_Channel_t uartChannel) {

    unsigned char temp;
    bool errorDetected = false;
    switch (uartChannel) {
        case UART_CH1:
            if (U1STAbits.FERR) {
                temp = U1RXREG;
                U1STAbits.UTXEN = 0;
                U1STAbits.UTXEN = 1;
                U1STAbits.FERR = 0;
                temp = U1RXREG;
                errorDetected = true;
                U1STAbits.FERR = 0;
            }
            if (!U1STAbits.OERR) {
                return errorDetected;
            }
            U1STAbits.UTXEN = 0;
            U1STAbits.UTXEN = 1;
            temp = U1RXREG;
            temp = U1RXREG;
            temp = U1RXREG;
            U1STAbits.OERR = 0;
            errorDetected = true;
            break;
        case UART_CH2:
            if (U2STAbits.FERR) {
                temp = U2RXREG;
                U2STAbits.UTXEN = 0;
                U2STAbits.UTXEN = 1;
                U2STAbits.FERR = 0;
                temp = U2RXREG;
                errorDetected = true;
                U2STAbits.FERR = 0;
            }
            if (!U2STAbits.OERR) {
                return errorDetected;
            }

            U2STAbits.UTXEN = 0;
            U2STAbits.UTXEN = 1;
            temp = U2RXREG;
            temp = U2RXREG;
            temp = U2RXREG;
            U2STAbits.OERR = 0;
            errorDetected = true;
            break;
        case UART_CH3:
            if (U3STAbits.FERR) {
                temp = U3RXREG;
                U3STAbits.UTXEN = 0;
                U3STAbits.UTXEN = 1;
                U3STAbits.FERR = 0;
                temp = U3RXREG;
                errorDetected = true;
                U3STAbits.FERR = 0;
            }
            if (!U3STAbits.OERR) {
                return errorDetected;
            }

            U3STAbits.UTXEN = 0;
            U3STAbits.UTXEN = 1;
            temp = U3RXREG;
            temp = U3RXREG;
            temp = U3RXREG;
            U3STAbits.OERR = 0;
            errorDetected = true;
            break;
        case UART_CH4:
            if (U4STAbits.FERR) {
                temp = U4RXREG;
                U4STAbits.UTXEN = 0;
                U4STAbits.UTXEN = 1;
                U4STAbits.FERR = 0;
                temp = U4RXREG;
                errorDetected = true;
                U4STAbits.FERR = 0;
            }
            if (!U4STAbits.OERR) {
                return errorDetected;
            }

            U4STAbits.UTXEN = 0;
            U4STAbits.UTXEN = 1;
            temp = U4RXREG;
            temp = U4RXREG;
            temp = U4RXREG;
            U4STAbits.OERR = 0;
            errorDetected = true;
            break;
        default:
            break;
    }
    return errorDetected;
}

void putch(char c) {

    while (U1STAbits.UTXBF) {
        //        ClrWdt();
    }
    U1TXREG = c;
}

char getch(void) {

    //    if (g_GetchCallBack == NULL){
    //      return 0; 
    //    }
    return g_GetchCallBack();
}