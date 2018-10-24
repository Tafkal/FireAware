# FireAware
NASA Space Apps Challenge 2018 Belgrade, Serbia. Team **.A.L.S.M.** code and hardware schematics

## Hardware
The hardware section is split into two parts, one of them being the code used, and the other the schematic of the hardware with other relevant information.

The code is written on the mbed online compiler, and for this git is exported to be build with the cmake-gcc-arm and a STM32 Nucleo-L476RG board.

## Radio
We implemented the GFSK demodulation method (using the RTL-SDR dongle as a receiver) using a sinc filter with a blackman window and finally the information was extracted through the delay-and-multiply method, the sample rate is 30 times larger than the Tx bit rate to provide a simple yet efficient method of error corection

## Server


_Thank you for reading,
Team .A.L.S.M._
