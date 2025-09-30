#ifndef _UART_DRIVER_HPP_
#define _UART_DRIVER_HPP_ 

#include "usart.h"
#include "dma.h"
#include "../User/LowLayer/Equipment/remote/Dr16.hpp"

extern Clicker::DR16 dr16;
extern uint8_t pData[18];

#ifdef __cplusplus
extern "C" {
#endif
	
void RxEventCallback(void);

#ifdef __cplusplus
}
#endif

#endif
