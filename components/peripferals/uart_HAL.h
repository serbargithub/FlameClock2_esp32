#ifndef _UART_HAL_H_
#define _UART_HAL_H_

#ifdef	__cplusplus
extern "C" {
#endif

typedef enum {
    UART_SPEED_2400	= 0,
    UART_SPEED_4800,
    UART_SPEED_9600,
    UART_SPEED_19200,
    UART_SPEED_38400,
    UART_SPEED_57600,
    UART_SPEED_115200,
}UART_Speed_t;

typedef enum {
    UART_CH1 = 0,
    UART_CH2,
    UART_CH3,
    UART_CH4,
}UART_Channel_t;

typedef char (*GETCHAR_CALLBACK)(void);

void HAL_UART__SetExternGetch(GETCHAR_CALLBACK getChar);
void HAL_UART__SerialSetup(UART_Speed_t serialSpeed, UART_Channel_t uartChannel);
bool HAL_UART__CheckAndResetErrors(UART_Channel_t uartChannel);
void HAL_UART__TurnOff(UART_Channel_t uartChannel);
void putch(char c);
char getch(void);



#ifdef	__cplusplus
}
#endif

#endif //_UART_HAL_H_


