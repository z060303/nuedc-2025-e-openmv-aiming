#ifndef EMM_V5_H
#define EMM_V5_H

#include <stdbool.h>
#include <stdint.h>

/* Minimal velocity command used by this project for a compatible Emm V5 driver. */
void Emm_V5_Vel_Control(uint8_t address, uint8_t direction,
                        uint16_t rpm, uint8_t acceleration, bool synchronized);

#endif
