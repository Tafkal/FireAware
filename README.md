# FireAware
NASA Space Apps Challenge 2018 Belgrade, Serbia. Team **.A.L.S.M.** code and hardware schematics

## Hardware
The hardware section is split into two parts, one of them being the code used, and the other the schematic of the hardware with other relevant information.

The code is written on the mbed online compiler, and for this git is exported to be build with the cmake-gcc-arm and a STM32 Nucleo-L476RG board.

## Radio
We implemented the GFSK demodulation method (using the RTL-SDR dongle as a receiver) using a sinc filter with a blackman window and finally the information was extracted through the delay-and-multiply method, the sample rate is 30 times larger than the Tx bit rate to provide a simple yet efficient method of error corection

## Server
This project consists of two parts - the server and the frontend.

The server is a node application that receives instructions from gateways regarding the state of sensors.
The frontend is a single-page application that renders a map with all active sensors.

Installation:
1. Install node
2. Install typescript
3. Install dependencies
    `npm run install`
4. Install http-server
    `npm install http-server -g`

To build:
1. Run in root directory:
    `npm run build`

To run:
1. Open two console windows and navigate both to root.
2. In one console, host /src/ on a web server (I used http-server from npm and served on port 8080).
    `cd src && http-server`
3. In the other, run the compiled node server. From root run
    `node dist/worker.js`
4. Open the hosted location (eg. localhost:8080) in a browser with caching disabled.

The server will listen on port 6969 for data from the gateways. Upon receiving data from the gateway it will write it into /src/data.json, from which the frontend (loaded into index.html as bundle.js) will read every 5 seconds to scan for changes, and add markers to the map accordingly.

_Thank you for reading,
Team .A.L.S.M._
