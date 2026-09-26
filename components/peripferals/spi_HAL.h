#ifndef _SPI_HAL_H_
#define _SPI_HAL_H_

#ifdef	__cplusplus
extern "C" {
#endif

    void HAL_SPI__TurnOff(void);
    void HAL_SPI__Init(void);
    uint8_t HAL_SPI__SendByte(uint8_t);
    uint8_t HAL_SPI__GetByte(void);

#ifdef	__cplusplus
}
#endif

#endif //_SPI_HAL_H_
