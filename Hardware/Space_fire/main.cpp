/*Includes*/
#include "mbed.h"
#include "DHT.h"
#include "WS2812.h"
#include "PixelArray.h"

/*Defines*/
/*4x4 RGB*/
#define WS2812_BUF 16 // W2812_BUFF >/= NUM_COLORS * NUM_LEDS
#define NUM_COLORS 6
#define NUM_LEDS 16

/*Pins*/
#define tRF_TX (PA_0)
#define tRF_RX (PA_1)

#define smoke_pin (PB_0)
#define humidity_pin (PA_4)
/*Smoke detector*/
#define smoke_thresh (0.50f)

/*Where to do a print for debug?*/
Serial pc(SERIAL_TX, SERIAL_RX, 115200);

/*Init tRF*/
void init_tRF(Serial* trf){
    //tRF go to config mode
    trf->printf("+++");
    wait(0.05);
    trf->printf("ATR\r");
    // Freq
    trf->printf("ATS206=12\r");
    // Channel
    trf->printf("ATS200=3\r");
    // Speed transmit uart
    trf->printf("ATS210=5\r");
    // Radio baud
    trf->printf("ATS201=3\r");
    // Go to send mode
    trf->printf("ATO\r");
}
/*End tRF init*/

/*Scream*/
void tRFScream(Serial* trf,WS2812* ws, PixelArray* px) {
    for (int z=0; z<WS2812_BUF; z++) // Color updated from Top to Bottom
//        for (z=WS2812_BUF; z>=0; z--) // Color updated from Bottom to Top        
    {
        ws->write_offsets(px->getBuf(),z,z,z);
    }
    while(1){
        trf->printf("UUUUUUUU");
    }
}
/*End Scream*/

 /*Get some humi data*/
void doDHT(DHT* sensor){
    int err;
    err = sensor->readData();
    if (err == 0) {
        pc.printf("Temperature is %4.2f C \r\n",sensor->ReadTemperature(CELCIUS));
        pc.printf("Humidity is %4.2f \r\n",sensor->ReadHumidity());
        pc.printf("Dew point is %4.2f  \r\n",sensor->CalcdewPoint(sensor->ReadTemperature(CELCIUS), sensor->ReadHumidity()));
        pc.printf("Dew point (fast) is %4.2f  \r\n",sensor->CalcdewPointFast(sensor->ReadTemperature(CELCIUS), sensor->ReadHumidity()));
    } else {
        pc.printf("\r\nErr %i \n",err);
    }
}
/*Got em humi datas*/

/*Do me*/
int main()
{
    float smoke_val;
    /*tRF Serial definition*/
    Serial tRF(tRF_TX, tRF_RX, 115200);
    /*RGB*/
    WS2812 ws(PB_5, WS2812_BUF, 3, 12, 9, 12); // LED Driver
    PixelArray px(WS2812_BUF);
    ws.useII(WS2812::GLOBAL); // Pixel dimming disabled
        int colorbuf[NUM_COLORS] = {0x2f0000,0x2f2f00,0x002f00,0x002f2f,0x00002f,0x2f002f};
    
    // for each of the colours (j) write out 16 of them the pixels are written at the colour*16, plus the colour position all modulus 60 so it wraps around
    for (int i = 0; i < WS2812_BUF; i++) {
        px.Set(i, colorbuf[(i / NUM_LEDS) % NUM_COLORS]);
    }

    // now all the colours are computed, add a fade effect using intensity scaling compute and write the II value for each pixel
    for (int j=0; j<WS2812_BUF; j++) 
    {
        // px.SetI(pixel position, II value)
        px.SetI(j%WS2812_BUF, 0xf+(0xf*(j%NUM_LEDS)));
    }
    /*Smoke detector*/
    AnalogIn smoke_in(smoke_pin);
    /*Humidity and temperature*/
    DHT HumSensor(humidity_pin,SEN11301P);
    /*Init all the things*/
    init_tRF(&tRF);
    pc.printf("Alright chums, let's do this!\r\n"); //Debug line
    
    /*Some infinite action*/
    while (1) {
        pc.printf("Loop start.\r\n"); //Debug line, I'm using the mbed online compiler, understand me pls.
        /*Do smoke*/
        smoke_val = smoke_in.read();
        if(smoke_val >= smoke_thresh) {
            pc.printf("NOK Smoke: %f\r\n", smoke_val);   
            pc.printf("SMOKE!");
            tRFScream(&tRF, &ws, &px);
        }
        else {
            pc.printf("OK Smoke: %f\r\n", smoke_val); 
        }
        /*Do humi sensor*/
        doDHT(&HumSensor);
        if (HumSensor.ReadTemperature(CELCIUS) > 35.0f) {
            pc.printf("HUMI!");
            tRFScream(&tRF, &ws, &px);
        }
        wait(1);
    }
}
/*End me*/