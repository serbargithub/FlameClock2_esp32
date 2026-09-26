#ifndef _PERIFERALS_HAL_H_
#define _PERIFERALS_HAL_H_

#ifdef	__cplusplus
extern "C" {
#endif

    typedef enum {
        PIN_OFF = 0,
        PIN_ON = 1,
    } PinValue_t;

    typedef enum {
        BUTTON1 = 0,
        BUTTON2,
        BUTTON3,
        BUTTON4,
    } ButtonsName_t;

    void HAL_ConrolReg__HighPerfomance(void);
    void HAL_ConrolReg__LowPerfomance(void);
    void HAL_PIO__Init_IOPorts(void);
    void HAL_PIO__TurnOff_IOPorts(void);
    bool HAL_PIO__GetButtonState(ButtonsName_t button);
    bool HAL_ADC__GetPowerState();
    void HAL_PIO__SetInformLed(PinValue_t value);
    void HAL_PIO__SetBuzzerOut(PinValue_t value);
    void HAL_PIO__BuckUp1Out(PinValue_t value);
    void HAL_PIO__BuckUp2Out(PinValue_t value);
    void HAL_PIO__DisplayLatch(PinValue_t value);

    void HAL_ADC__InitADC(void);
    void HAL_MAP__GeneralPeripheralsMapping(void);

#ifdef	__cplusplus
}
#endif

#endif //_PERIFERALS_HAL_H_

