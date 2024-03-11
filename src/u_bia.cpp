#include "u_bia.h"

extern bool debug_print;

uint32_t AppBuff[APPBUFF_SIZE];
float AD5940MeasureResult;

volatile static uint32_t ucInterrupted = 0; // flag to indicate interrupt from AD5940

/** AD5940 Library_Interface Definition Begin **/

void AD5940_ReadWriteNBytes(unsigned char *pSendBuffer, unsigned char *pRecvBuff, unsigned long length) {
    // set SPI settings for the following transaction
    // speedMaximum: 12MHz found to be max for Adafruit Feather M0, AD5940 rated for max 16MHz clock frequency
    // dataOrder: MSB first
    // dataMode: SCLK idles low/ data clocked on SCLK falling edge --> mode 0
    SPI.beginTransaction(SPISettings(1200000, MSBFIRST, SPI_MODE0));

    for (unsigned int i = 0; i < length; i++) {
        *pRecvBuff++ = SPI.transfer(*pSendBuffer++);  //do a transfer
    }

    SPI.endTransaction(); //transaction over
}

void AD5940_CsClr(void) { digitalWrite(SPI_CS_AD5940_Pin, LOW); }

void AD5940_CsSet(void) { digitalWrite(SPI_CS_AD5940_Pin, HIGH); }

void AD5940_RstSet(void) { digitalWrite(AD5940_ResetPin, HIGH); }

void AD5940_RstClr(void) { digitalWrite(AD5940_ResetPin, LOW); }

void AD5940_Delay10us(uint32_t time) {
    // Warning: micros() only has 4us (for 16MHz boards) or 8us (for 8MHz boards) resolution - use a timer instead?
    unsigned long time_last = micros();
    while (micros() - time_last < time * 10);
}

uint32_t AD5940_GetMCUIntFlag(void) { return ucInterrupted; }

uint32_t AD5940_ClrMCUIntFlag(void) {
    ucInterrupted = 0;
    return 1;
}

/* Functions that used to initialize MCU platform */
void Ext_Int0_Handler() { ucInterrupted = 1; }

uint32_t AD5940_MCUResourceInit(void *pCfg) {
    /* Step1, initialize SPI peripheral and its GPIOs for CS/RST */
    // start the SPI library (setup SCK, MOSI, and MISO pins)
    SPI.begin();
    // initialize SPI chip select pin
    pinMode(SPI_CS_AD5940_Pin, OUTPUT);
    // initialize Reset pin
    pinMode(AD5940_ResetPin, OUTPUT);

    /* Step2: initialize GPIO interrupt that connects to AD5940's interrupt output pin(Gp0, Gp3, Gp4, Gp6 or Gp7 ) */
    // init AD5940 interrupt pin
    pinMode(AD5940_IntPin, INPUT_PULLUP);
    // attach ISR for falling edge
    attachInterrupt(digitalPinToInterrupt(AD5940_IntPin), Ext_Int0_Handler, FALLING);

    // chip select high to de-select AD5940 initially
    AD5940_CsSet();
    AD5940_RstSet();

    return 0;
}

/** AD5940 Library_Interface Definition End **/

static int32_t BIAShowResult(uint32_t *pData, uint32_t DataCount) {
    auto *pImp = (fImpPol_Type *) pData;

    /*Process data*/
    if (debug_print) {
        float freq;
        AppBIACtrl(BIACTRL_GETFREQ, &freq);
        for (uint32_t i = 0; i < DataCount; i++) {
            Serial.print("Freq: ");
            Serial.print(freq);
            Serial.print(" RzMag: ");
            Serial.print(pImp[i].Magnitude);
            Serial.print(" Ohm , RzPhase: ");
            Serial.println(pImp[i].Phase * 180 / MATH_PI);
        }
    } else {
        AD5940MeasureResult = pImp[0].Magnitude; //yw
    }

    return 0;
}

/* Initialize AD5940 basic blocks like clock */
static int32_t AD5940PlatformCfg() {
    CLKCfg_Type clk_cfg;
    FIFOCfg_Type fifo_cfg;
    AGPIOCfg_Type gpio_cfg;

    /* Use hardware reset */
    AD5940_HWReset();
    /* Platform configuration */
    AD5940_Initialize();

    /* Step1. Configure clock */
    clk_cfg.ADCClkDiv = ADCCLKDIV_1;
    clk_cfg.ADCCLkSrc = ADCCLKSRC_HFOSC;
    clk_cfg.SysClkDiv = SYSCLKDIV_1;
    clk_cfg.SysClkSrc = SYSCLKSRC_HFOSC;
    clk_cfg.HfOSC32MHzMode = bFALSE;
    clk_cfg.HFOSCEn = bTRUE;
    clk_cfg.HFXTALEn = bFALSE;
    clk_cfg.LFOSCEn = bTRUE;
    AD5940_CLKCfg(&clk_cfg);
    /* Step2. Configure FIFO and Sequencer*/
    fifo_cfg.FIFOEn = bFALSE;
    fifo_cfg.FIFOMode = FIFOMODE_FIFO;
    fifo_cfg.FIFOSize = FIFOSIZE_4KB;                       /* 4kB for FIFO, The reset 2kB for sequencer */
    fifo_cfg.FIFOSrc = FIFOSRC_DFT;
    fifo_cfg.FIFOThresh = 4;//AppBIACfg.FifoThresh;        /* DFT result. One pair for RCAL, another for Rz. One DFT result have real part and imaginary part */
    AD5940_FIFOCfg(&fifo_cfg);                             /* Disable to reset FIFO. */
    fifo_cfg.FIFOEn = bTRUE;
    AD5940_FIFOCfg(&fifo_cfg);                             /* Enable FIFO here */

    /* Step3. Interrupt controller */
    /* Enable all interrupt in Interrupt Controller 1, so we can check INTC flags */
    AD5940_INTCCfg(AFEINTC_1, AFEINTSRC_ALLINT, bTRUE);
    /* Interrupt Controller 0 will control GP0 to generate interrupt to MCU */
    AD5940_INTCCfg(AFEINTC_0, AFEINTSRC_DATAFIFOTHRESH, bTRUE);
    AD5940_INTCClrFlag(AFEINTSRC_ALLINT);

    /* Step4: Reconfigure GPIO */
    gpio_cfg.FuncSet = GP6_SYNC | GP5_SYNC | GP4_SYNC | GP2_TRIG | GP1_SYNC | GP0_INT;
    gpio_cfg.InputEnSet = AGPIO_Pin2;
    gpio_cfg.OutputEnSet = AGPIO_Pin0 | AGPIO_Pin1 | AGPIO_Pin4 | AGPIO_Pin5 | AGPIO_Pin6;
    gpio_cfg.OutVal = 0;
    gpio_cfg.PullEnSet = 0;

    AD5940_AGPIOCfg(&gpio_cfg);
    AD5940_SleepKeyCtrlS(SLPKEY_UNLOCK);  /* Allow AFE to enter sleep mode. */

    return 0;
}

/* !!Change the application parameters here if you want to change it to none-default value */
static void AD5940BIAStructInit() {
    AppBIACfg_Type *pBIACfg;

    AppBIAGetCfg(&pBIACfg);

    pBIACfg->SeqStartAddr = 0;
    pBIACfg->MaxSeqLen = 512; /** @todo add checker in function */

    pBIACfg->RcalVal = 10000.0;
    pBIACfg->DftNum = DFTNUM_256;

    pBIACfg->NumOfData = -1;       /* Never stop until you stop it manually by AppBIACtrl() function */
    //pBIACfg->NumOfData = 1;
    pBIACfg->BiaODR = 4095;        /* ODR(Sample Rate) 20Hz */
    //pBIACfg->BiaODR = 2000;
    pBIACfg->FifoThresh = 4;      /* 4 */
    pBIACfg->ADCSinc3Osr = ADCSINC3OSR_2;
}


void AD5940_BIA_Setup() {
    // init GPIOs (SPI, AD5940 Reset and Interrupt)
    AD5940_MCUResourceInit(nullptr);

    AD5940PlatformCfg();
    AD5940BIAStructInit();

    /* Initialize BIA application. Provide a buffer, which is used to store sequencer commands */
    AppBIAInit(AppBuff, APPBUFF_SIZE);
}

void AD5940_BIA_UpdateReading() {
    /* Wait for interrupt happens */
    while (AD5940_GetMCUIntFlag() == 0);

    AD5940_ClrMCUIntFlag();
    uint32_t temp = APPBUFF_SIZE;
    AppBIAISR(AppBuff, &temp); /* Deal with it and provide a buffer to store data we got */
    BIAShowResult(AppBuff, temp); /* Show the results */
}
