//Layout of signals of UCLA board

#include <Layout.hpp>
#define N(x) (sizeof(x)/sizeof(*x))
#define GPIODIR_IN  0
#define GPIODIR_OUT 1

//Configure GPIO Input/Output Direction
//0 is OUTPUT
//1 is INPUT
//-1 not a gpio
//DO NOT reorder this list

Layout::Layout()  {}; 
Layout::~Layout() {}; 

int Layout::gpioConfiguration(unsigned igpio) {
    //-1 == DO not Configure
    //1 == Output
    //0 == Input
    igpio -= 1; //count from 0 
    static const int gpioconf[] = {
        //1     2     3     4     5      6     7     8     9     10
        -1,    -1,   -1,   -1,   -1,    -1,   -1,   -1,   -1,     0, 
        //11    12    13    14    15     16    17    18    19    20
        -1,    -1,   -1,   -1,   -1,    -1,   -1,   -1,   -1,    -1, 
        //21    22    23    24    25     26    27    28    29    30
        -1,    -1,   -1,   -1,   -1,    -1,   -1,   -1,   -1,    -1, 
        //31    32    33    34    35     36    37    38    39    40
        -1,    -1,   -1,   -1,   -1,    -1,   -1,   -1,   -1,    -1, 
        //41    42    43    44    45     46    47    48    49    50
        -1,    -1,   -1,   -1,   -1,    -1,   -1,   -1,   -1,    -1, 
        //51    52    53    54    55     56    57    58    59    60
        -1,    -1,   -1,   -1,   -1,    -1,   -1,   -1,   -1,    -1, 
        //61    62    63    64    65     66    67    68    69    70
        -1,    -1,   -1,   -1,   -1,    -1,   -1,   -1,   -1,     1, 
        //71    72    73    74    75     76    77    78    79    80
        1,      1,    1,   -1,    1,    -1,   -1,   -1,   -1,    -1, 
        //81    82    83    84    85     86    87    88    89    90
        -1,    -1,   -1,   -1,   -1,    -1,   -1,   -1,   -1,    -1, 
        //91    92    93    94    95     96    97    98    99    100
        -1,    -1,   -1,   -1,   -1,    -1,   -1,   -1,   -1,    -1, 
        //101   102   103   104   105    106   107   108   109   110
        -1,    -1,   -1,   -1,   -1,    -1,   -1,   -1,   -1,    -1, 
        //111   112   113   114   115    116   117   118   119   120
        -1,    -1,   -1,   -1,   -1,    -1,   -1,   -1,   -1,    -1, 
        //121   122   123   124   125    126   127   128   129   130
        -1,    -1,   -1,   -1,   -1,    -1,   -1,   -1,   -1,    -1, 
        //131   132   133   134   135    136   137   138   139   140
        -1,    -1,   -1,   -1,   -1,    -1,   -1,   -1,   -1,    -1, 
        //141   142   143   144   145    146   147   148   149   150
        -1,    -1,   -1,   -1,   -1,    -1,   -1,   -1,   -1,    -1, 
        //151   152   153   154   155    156   157   158   159   160
        -1,    -1,   -1,   -1,   -1,    -1,   -1,   -1,   -1,    -1, 
        //161   162   163   164   165    166   167   168   169   170
        -1,    -1,   -1,   -1,   -1,    -1,   -1,   -1,   -1,    -1, 
        //171   172   173   174   175    176   177   178   179   180
        -1,    -1,   -1,   -1,   -1,    -1,   -1,   -1,   -1,    -1, 
        //181   182   183   184   185    186   187   188   189   190
        -1,    -1,   -1,   -1,   -1,    -1,   -1,   -1,   -1,    -1, 
        //191   192 
        -1,    -1  
    };
    if ((igpio>0) && ((igpio) < 192))
        return gpioconf[igpio]; 
    else
        return -1; 
}

//Maps Overo output pins (J1 1-70, J 71-140) to GPIO Pins
//-1 is returned for a pin140 that is not a GPIO
int Layout::pin140ToGPIO(unsigned ipin140) {
    static const int igpio[] =  {
        -1,  //N_MANUAL_RESET      1 
        71,  //GPIO71_L_DD01       2
        70,  //GPIO70_L_DD00       3
        73,  //GPIO73_L_DD03       4
        75,  //GPIO75_L_DD05       5
        72,  //GPIO72_L_DD02       6
        74,  //GPIO74_L_DD04       7
        10,  //GPIO_10             8
        -1,  //GPIO0_WAKEUP        9
        185, //GPIO185_I2C3_SDA    10
        80,  //GPIO80_L_DD10       11
        81,  //GPIO81_L_DD11       12
        184, //GPIO184_I2C3_SCL    13
        186, //GPIO_186            14
        92,  //GPIO92_L_DD22       15
        147, //GPIO147_GPT8_PWM    16
        83,  //GPIO83_L_DD13       17
        144, //GPIO144_GPT9_PWM    18
        84,  //GPIO84_L_DD14       19
        85,  //GPIO85_L_DD15       20
        146, //GPIO146_GPT11_PWM   21
        163, //GPIO163_IR_CTS3     22
        91,  //GPIO91_L_DD21       23
        87,  //GPIO87_L_DD17       24
        88,  //GPIO88_L_DD18       25
        166, //GPIO166_IR_TXD3     26
        89,  //GPIO89_L_DD19       27
        79,  //GPIO79_L_DD09       28
        77,  //GPIO77_L_DD07       29
        78,  //GPIO78_L_DD08       30
        165, //GPIO165_IR_RXD3     31
        66,  //GPIO66_L_PCLK       32
        76,  //GPIO76_L_DD06       33
        68,  //GPIO68_L_FCLK       34
        67,  //GPIO67_L_LCLK       35
        -1,  //USBOTG_DP           36  
        -1,  //USBOTG_DM           37  
        -1,  //AUXLF               38  
        -1,  //MIC_SUB_MF          39  
        -1,  //ADCIN4              40  
        -1,  //AUXRF               41  
        -1,  //PWM0                42  
        69,  //GPIO69_L_BIAS       43  
        86,  //GPIO86_L_DD16       44  
        90,  //GPIO90_L_DD20       45  
        -1,  //USBOTG_ID           46  
        170, //GPIO170_HDQ_1WIRE   47  
        -1,  //ADCIN3              48  
        -1,  //PWM1                49  
        -1,  //AGND                50  
        -1,  //ADCIN5              51  
        -1,  //VBACKUP             52  
        -1,  //ADCIN6              53  
        -1,  //USBOTG_VBUS         54  
        145, //GPIO145_GPT10_PWM   55  
        -1,  //GND                 56  
        -1,  //MIC_MAIN_MF         57  
        -1,  //ADCIN2              58  
        -1,  //SYSEN               59  
        82,  //GPIO82_L_DD12       60  
        93,  //GPIO93_L_DD23       61  
        -1,  //TV_OUT2             62  
        -1,  //TV_OUT1             63  
        -1,  //ADCIN7              64  
        -1,  //POWERON             65  
        -1,  //VSYSTEM             66  
        -1,  //VSYSTEM             67  
        -1,  //HSOLF               68  
        -1,  //HSORF               69  
        -1,  //GND                 70  

        ////Connector J4 (70-pin): Extended Memory Bus & MMC Signals
        -1,  //VSYSTEM             71 
        -1,  //VSYSTEM             72 
        -1,  //GND                 73 
        -1,  //EM_NCS5_ETH0        74 
        -1,  //EM_NCS4             75 
        -1,  //EM_NWE              76 
        -1,  //EM_NADV_ALE         77 
        -1,  //EM_NOE              78 
        65,  //GPIO65_ETH1_IRQ1    79 
        64,  //GPIO64_ETH0_NRESET  80 
        -1,  //EM_A2               81 
        -1,  //EM_A8               82 
        -1,  //EM_A5               83 
        -1,  //EM_A7               84 
        -1,  //EM_D2               85 
        -1,  //EM_D10              86 
        -1,  //EM_D3               87 
        -1,  //EM_D11              88 
        -1,  //EM_D4               89 
        -1,  //EM_D12              90 
        -1,  //EM_D5               91 
        -1,  //EM_D15              92 
        13,  //GPIO13_MMC3_CMD     93 
        148, //GPIO148_TXD1        94 
        176, //GPIO176_ETH0_IRQ    95 
        18,  //GPIO18_MMC3_D0      96 
        174, //GPIO174_SPI1_CS0    97 
        168, //GPIO168_USBH_CPEN   98 
        14,  //GPIO14_MMC3_DAT4    99 
        21,  //GPIO21_MMC3_DAT7    100
        17,  //GPIO17_MMC3_D3      101
        -1,  //USBH_VBUS           102
        -1,  //GND                 103
        -1,  //USBH_DP             104
        -1,  //USBH_DM             105
        19,  //GPIO19_MMC3_D1      106
        22,  //GPIO22_MMC3_DAT6    107
        23,  //GPIO23_MMC3_DAT5    108
        20,  //GPIO20_MMC3_D2      109
        12,  //GPIO12_MMC3_CLK     110
        114, //GPIO114_SPI1_NIRQ   111
        175, //GPIO175_SPI1_CS1    112
        171, //GPIO171_SPI1_CLK    113
        172, //GPIO172_SPI1_MOSI   114
        173, //GPIO173_SPI1_MISO   115
        -1,  //4030GP2_N_MMC3_CD   116
        150, //GPIO150_MMC3_WP     117
        151, //GPIO151_RXD1        118
        -1,  //EM_D7               119
        -1,  //EM_D14              120
        -1,  //EM_D6               121
        -1,  //EM_D13              122
        -1,  //EM_D1               123
        -1,  //EM_D8               124
        -1,  //EM_D9               125
        -1,  //EM_D0               126
        -1,  //EM_A6               127
        -1,  //EM_A1               128
        -1,  //EM_A3               129
        -1,  //EM_A10              130
        -1,  //EM_A4               131
        -1,  //EM_A9               132
        -1,  //EM_NWP              133
        -1,  //EM_NCS1             134
        -1,  //EM_NBE0             135
        -1,  //EM_NCS0             136
        -1,  //EM_NCS6             137
        -1,  //EM_WAIT0            138
        -1,  //EM_NBE1             139
        -1,  //EM_CLK              140
    };
    if  ((ipin140>0) && ((--ipin140) < N(igpio)))
        return igpio[ipin140];
    else 
        return -1;
} 

unsigned Layout::igpioN_M_RESET()     { return pin140ToGPIO(N_MANUAL_RESET)     ;} 
unsigned Layout::igpioEN_IO()         { return pin140ToGPIO(GPIO72_L_DD02)      ;}

unsigned Layout::igpioPowerADC()      { return pin140ToGPIO(GPIO150_MMC3_WP)    ;} //PowerADC
unsigned Layout::igpioADCSel1()       { return pin140ToGPIO(GPIO83_L_DD13)      ;} //ADCSel1
unsigned Layout::igpioADCSel2()       { return pin140ToGPIO(GPIO77_L_DD07)      ;} //ADCSel2

unsigned Layout::igpioEncoderEnable() { return pin140ToGPIO(GPIO71_L_DD01)      ;} //EncoderEnable

unsigned Layout::igpioConsoleRXD()    { return pin140ToGPIO(GPIO165_IR_RXD3)    ;} //ConsoleRXD
unsigned Layout::igpioConsoleTXD()    { return pin140ToGPIO(GPIO166_IR_TXD3)    ;} //ConsoleTXD

unsigned Layout::igpioTP1()           { return pin140ToGPIO(GPIO69_L_BIAS)      ;}
unsigned Layout::igpioTP2()           { return pin140ToGPIO(GPIO86_L_DD16)      ;}
unsigned Layout::igpioTP3()           { return pin140ToGPIO(GPIO90_L_DD20)      ;}
unsigned Layout::igpioTP4()           { return pin140ToGPIO(GPIO170_HDQ_1WIRE)  ;}
unsigned Layout::igpioTP5()           { return pin140ToGPIO(GPIO145_GPT10_PWM)  ;}
unsigned Layout::igpioTP6()           { return pin140ToGPIO(GPIO82_L_DD12)      ;}
unsigned Layout::igpioTP7()           { return pin140ToGPIO(GPIO93_L_DD23)      ;}


unsigned Layout::igpioUSBReset()      { return pin140ToGPIO(GPIO74_L_DD04)      ;} //USBReset
unsigned Layout::igpioUSBOff1()       { return pin140ToGPIO(GPIO14_MMC3_DAT4)   ;} //USB1EnableBar
unsigned Layout::igpioUSBOff2()       { return pin140ToGPIO(GPIO18_MMC3_D0)     ;} //USB2EnableBar
unsigned Layout::igpioUSBOff3()       { return pin140ToGPIO(GPIO80_L_DD10)      ;} //USB3EnableBar
unsigned Layout::igpioUSBOff4()       { return pin140ToGPIO(GPIO_10)            ;} //USB4EnableBar
unsigned Layout::igpioUSBOff5()       { return pin140ToGPIO(GPIO78_L_DD08)      ;} //USB5EnableBar
unsigned Layout::igpioUSBOff6()       { return pin140ToGPIO(GPIO12_MMC3_CLK)    ;} //USB6EnableBar
unsigned Layout::igpioUSBOff7()       { return pin140ToGPIO(GPIO17_MMC3_D3)     ;} //USB7EnableBar

unsigned Layout::igpioMS1()           { return pin140ToGPIO(GPIO146_GPT11_PWM)  ;} //DRMS1
unsigned Layout::igpioMS2()           { return pin140ToGPIO(GPIO85_L_DD15)      ;} //DRMS2
unsigned Layout::igpioPwrIncBar()     { return pin140ToGPIO(GPIO175_SPI1_CS1)   ;} //DRPowerIncreaseBar
unsigned Layout::igpioSR()            { return pin140ToGPIO(GPIO91_L_DD21)      ;} //DRSRBar
unsigned Layout::igpioReset()         { return pin140ToGPIO(GPIO147_GPT8_PWM)   ;} //DRResetBar
unsigned Layout::igpioSleep()         { return pin140ToGPIO(GPIO84_L_DD14)      ;} //DRSleepBar

unsigned Layout::igpioStep1()         { return pin140ToGPIO(GPIO67_L_LCLK)      ;} //DR1Step
unsigned Layout::igpioStep2()         { return pin140ToGPIO(GPIO66_L_PCLK)      ;} //DR2Step
unsigned Layout::igpioStep3()         { return pin140ToGPIO(GPIO88_L_DD18)      ;} //DR3Step
unsigned Layout::igpioStep4()         { return pin140ToGPIO(GPIO_186)           ;} //DR4Step
unsigned Layout::igpioStep5()         { return pin140ToGPIO(GPIO145_GPT10_PWM)  ;} //DR5Step
unsigned Layout::igpioStep6()         { return pin140ToGPIO(GPIO23_MMC3_DAT5)   ;} //DR6Step

unsigned Layout::igpioDir1()          { return pin140ToGPIO(GPIO76_L_DD06)      ;} //DR1Dir
unsigned Layout::igpioDir2()          { return pin140ToGPIO(GPIO79_L_DD09)      ;} //DR2Dir
unsigned Layout::igpioDir3()          { return pin140ToGPIO(GPIO87_L_DD17)      ;} //DR3Dir
unsigned Layout::igpioDir4()          { return pin140ToGPIO(GPIO144_GPT9_PWM)   ;} //DR4Dir
unsigned Layout::igpioDir5()          { return pin140ToGPIO(GPIO70_L_DD00)      ;} //DR5Dir
unsigned Layout::igpioDir6()          { return pin140ToGPIO(GPIO20_MMC3_D2)     ;} //DR6Dir

unsigned Layout::igpioEnable1()       { return pin140ToGPIO(GPIO68_L_FCLK)      ;} //DR1EnableBar
unsigned Layout::igpioEnable2()       { return pin140ToGPIO(GPIO73_L_DD03)      ;} //DR2EnableBar
unsigned Layout::igpioEnable3()       { return pin140ToGPIO(GPIO89_L_DD19)      ;} //DR3EnableBar
unsigned Layout::igpioEnable4()       { return pin140ToGPIO(GPIO81_L_DD11)      ;} //DR4EnableBar
unsigned Layout::igpioEnable5()       { return pin140ToGPIO(GPIO92_L_DD22)      ;} //DR5EnableBar
unsigned Layout::igpioEnable6()       { return pin140ToGPIO(GPIO75_L_DD05)      ;} //DR6EnableBar

unsigned Layout::igpioSPI_Tx()        { return pin140ToGPIO(GPIO172_SPI1_MOSI)  ;} 
unsigned Layout::igpioSPI_Rx()        { return pin140ToGPIO(GPIO173_SPI1_MISO)  ;}
unsigned Layout::igpioSPI_Sclk()      { return pin140ToGPIO(GPIO171_SPI1_CLK)   ;}
unsigned Layout::igpioSPI_SFRM_bar()  { return pin140ToGPIO(GPIO174_SPI1_CS0)   ;}

// Returns Motor Direction Control Pin GPIO Number for a given idrive
unsigned Layout::igpioDir(const unsigned idrive) {
    static const unsigned igpio[] = { 
        igpioDir1(),
        igpioDir2(),
        igpioDir3(),
        igpioDir4(),
        igpioDir5(),
        igpioDir6()
    };
    return igpio[idrive];
}

// Returns Motor Step Pin GPIO Number for a given idrive
unsigned Layout::igpioStep(const unsigned idrive) {
    static const unsigned igpio[] = { 
        igpioStep1(),
        igpioStep2(),
        igpioStep3(),
        igpioStep4(),
        igpioStep5(),
        igpioStep6()
    };
    return igpio[idrive];
}


// Returns Motor Enable Pin GPIO Number for a given idrive
unsigned Layout::igpioEnable(const unsigned idrive) {
    static const unsigned igpio[] = { 
        igpioEnable1(),
        igpioEnable2(),
        igpioEnable3(),
        igpioEnable4(),
        igpioEnable5(),
        igpioEnable6()
    };
    return igpio[idrive];
}

// Returns USB Power Enable Pin GPIO Number for a given iusb
unsigned Layout::igpioUSBOff(const unsigned iusb) {
    static const unsigned igpio[] = { 
        igpioUSBOff1(),
        igpioUSBOff2(),
        igpioUSBOff3(),
        igpioUSBOff4(),
        igpioUSBOff5(),
        igpioUSBOff6(),
        igpioUSBOff7()
    };
    return igpio[iusb];
}