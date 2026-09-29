#include "Emm_v5.h"
#include "usart.h"

/* This small packet encoder replaces the third-party demo driver. The project
 * uses velocity mode only; other Emm V5 commands are intentionally absent. */
void Emm_V5_Vel_Control(uint8_t address, uint8_t direction,
                        uint16_t rpm, uint8_t acceleration, bool synchronized)
{
    uint8_t packet[8] = {
        address,
        0xF6,
        direction,
        (uint8_t)(rpm >> 8),
        (uint8_t)rpm,
        acceleration,
        (uint8_t)synchronized,
        0x6B
    };
    (void)HAL_UART_Transmit(&huart1, packet, sizeof(packet), 100);
}
