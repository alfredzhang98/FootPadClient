#ifndef Y3PROJ_LIB_BIA_H
#define Y3PROJ_LIB_BIA_H

#include <cmath>
#include <SPI.h>

extern "C" {
#include <ad5940.h>
#include <ad5940_bia.h>
}

#define APPBUFF_SIZE 512

#define SPI_CS_AD5940_Pin 10
#define AD5940_ResetPin A3
#define AD5940_IntPin 2

void AD5940_BIA_Setup();

void AD5940_BIA_UpdateReading();

#endif //Y3PROJ_LIB_BIA_H
