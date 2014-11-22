/*
 *  MirrorControlBoard.cpp - Class which implements useful mid-level
 *  functionality for the mirror control board.
 */

#include <cassert>
#include <MirrorControlBoard.hpp>
#include <GPIOInterface.hpp>
#include <unistd.h>
#include <time.h>
#include <math.h>
#include <cstdlib>

MirrorControlBoard::MirrorControlBoard() { }
MirrorControlBoard::~MirrorControlBoard() { }

void MirrorControlBoard::adcSleep (int iadc) {
    // Set on-board ADC into sleep mode
    selectADC(iadc);
    spi.Configure();
    spi.WriteRead(ADC.codeSWPowerDown());

    selectADC(iadc);
    spi.Configure();
    spi.WriteRead(ADC.codeSWPowerDown());
}

void MirrorControlBoard::powerDownUSB(unsigned iusb)
{
    gpio.WriteLevel(layout.igpioUSBOff(iusb),1);
}

void MirrorControlBoard::powerUpUSB(unsigned iusb)
{
    gpio.WriteLevel(layout.igpioUSBOff(iusb),0);
}

bool MirrorControlBoard::isUSBPoweredUp(unsigned iusb)
{
    return gpio.ReadLevel(layout.igpioUSBOff(iusb))?false:true;
}

void MirrorControlBoard::powerDownDriveControllers()
{
    gpio.WriteLevel(layout.igpioSleep,0);
}

void MirrorControlBoard::powerUpDriveControllers()
{
    gpio.WriteLevel(layout.igpioSleep,1);
}

bool MirrorControlBoard::isDriveControllersPoweredUp()
{
    return gpio.ReadLevel(layout.igpioSleep)?true:false;
}

void MirrorControlBoard::powerDownEncoders()
{
    gpio.WriteLevel(layout.igpioEncoderEnable,0);
}

void MirrorControlBoard::powerUpEncoders()
{
    gpio.WriteLevel(layout.igpioEncoderEnable,1);
}

bool MirrorControlBoard::isEncodersPoweredUp()
{
    return gpio.ReadLevel(layout.igpioEncoderEnable)?true:false;
}

void MirrorControlBoard::powerUpSensors()
{
    gpio.WriteLevel(layout.igpioPowerADC,1);
}

void MirrorControlBoard::powerDownSensors()
{
    gpio.WriteLevel(layout.igpioPowerADC,0);
}

bool MirrorControlBoard::isSensorsPoweredUp()
{
    return gpio.ReadLevel(layout.igpioPowerADC)?true:false;
}

void MirrorControlBoard::enableDriveSR(bool enable)
{
    gpio.WriteLevel(layout.igpioSR, enable?0:1);
}


void MirrorControlBoard::disableDriveSR()
{
    enableDriveSR(false);
}

bool MirrorControlBoard::isDriveSREnabled()
{
    return gpio.ReadLevel(layout.igpioSR)?false:true;
}

void MirrorControlBoard::setUStep(UStep ustep)
{
    unsigned mslog2 = 0;
    switch(ustep)
    {
        case USTEP_1:
            mslog2 = 0x0;
            break;
        case USTEP_2:
            mslog2 = 0x1;
            break;
        case USTEP_4:
            mslog2 = 0x2;
            break;
        case USTEP_8:
            mslog2 = 0x3;
            break;
    }
    gpio.WriteLevel(layout.igpioMS1, mslog2 & 0x1);
    gpio.WriteLevel(layout.igpioMS2, mslog2 & 0x2);
}

MirrorControlBoard::UStep MirrorControlBoard::getUStep()
{
    if(gpio.ReadLevel(layout.igpioMS2))
        return gpio.ReadLevel(layout.igpioMS1)?USTEP_8:USTEP_4;
    else
        return gpio.ReadLevel(layout.igpioMS1)?USTEP_2:USTEP_1;
}

void MirrorControlBoard:: stepOneDrive(unsigned idrive, Dir dir, unsigned frequency)
{
    // Write Direction to the DIR pin
    gpio.WriteLevel(layout.igpioDir(idrive),(dir==DIR_RETRACT)?1:0);

    // Writes one step to STEP pin
    unsigned igpio = layout.igpioStep(idrive);
    gpio.WriteLevel(igpio,(dir==DIR_NONE)?0:1);

    // a delay
    waitHalfPeriod(frequency);

    // Toggle pin back to low
    gpio.WriteLevel(igpio,0);
}

void MirrorControlBoard::stepAllDrives(Dir dr1_dir, Dir dr2_dir, Dir dr3_dir, Dir dr4_dir, Dir dr5_dir, Dir dr6_dir, unsigned frequency)
{
    gpio.WriteLevel(layout.igpioDir1,(dr1_dir==DIR_RETRACT)?1:0);
    gpio.WriteLevel(layout.igpioDir2,(dr2_dir==DIR_RETRACT)?1:0);
    gpio.WriteLevel(layout.igpioDir3,(dr3_dir==DIR_RETRACT)?1:0);
    gpio.WriteLevel(layout.igpioDir4,(dr4_dir==DIR_RETRACT)?1:0);
    gpio.WriteLevel(layout.igpioDir5,(dr5_dir==DIR_RETRACT)?1:0);
    gpio.WriteLevel(layout.igpioDir6,(dr6_dir==DIR_RETRACT)?1:0);

    gpio.WriteLevel(layout.igpioStep1,(dr1_dir==DIR_NONE)?0:1);
    gpio.WriteLevel(layout.igpioStep2,(dr2_dir==DIR_NONE)?0:1);
    gpio.WriteLevel(layout.igpioStep3,(dr3_dir==DIR_NONE)?0:1);
    gpio.WriteLevel(layout.igpioStep4,(dr4_dir==DIR_NONE)?0:1);
    gpio.WriteLevel(layout.igpioStep5,(dr5_dir==DIR_NONE)?0:1);
    gpio.WriteLevel(layout.igpioStep6,(dr6_dir==DIR_NONE)?0:1);

    waitHalfPeriod(frequency);

    gpio.WriteLevel(layout.igpioStep1,0);
    gpio.WriteLevel(layout.igpioStep2,0);
    gpio.WriteLevel(layout.igpioStep3,0);
    gpio.WriteLevel(layout.igpioStep4,0);
    gpio.WriteLevel(layout.igpioStep5,0);
    gpio.WriteLevel(layout.igpioStep6,0);
}

void MirrorControlBoard::setPhaseZeroOnAllDrives()
{
    gpio.WriteLevel(layout.igpioReset,0);
    waitHalfPeriod(400);
    gpio.WriteLevel(layout.igpioReset,1);
}

void MirrorControlBoard::enableDrive(unsigned idrive, bool enable)
{
    gpio.WriteLevel(layout.igpioEnable(idrive), enable?0:1);
}

void MirrorControlBoard::disableDrive(unsigned idrive)
{
    enableDrive(idrive, false);
}

bool MirrorControlBoard::isDriveEnabled(unsigned idrive)
{
    return gpio.ReadLevel(layout.igpioEnable(idrive))?false:true;
}

void MirrorControlBoard::enableDriveHiCurrent(bool enable)
{
    gpio.WriteLevel(layout.igpioPwrIncBar, enable?0:1);
}

void MirrorControlBoard::disableDriveHiCurrent()
{
    enableDriveHiCurrent(false);
}


bool MirrorControlBoard::isDriveHiCurrentEnabled()
{
    return gpio.ReadLevel(layout.igpioPwrIncBar)?false:true;
}

//------------------------------------------------------------------------------
// ADCs
//------------------------------------------------------------------------------

void MirrorControlBoard::initializeADC(unsigned iadc)
{
    spi.Configure();
    selectADC(iadc);                                        // Assert Chip Select for ADC in question
    spi.WriteRead(ADC.codeInitialize());
    spi.WriteRead(ADC.codeConfig());
}

void MirrorControlBoard::selectADC(unsigned iadc)
{
    gpio.WriteLevel(layout.igpioADCSel1, iadc==0?1:0);
    gpio.WriteLevel(layout.igpioADCSel2, iadc==1?1:0);
}

uint32_t MirrorControlBoard::measureADC(unsigned iadc, unsigned ichan)
{
    spi.Configure();

    initializeADC(iadc);

    // Assert Chip Select
    selectADC(iadc);

    // ADC Channel Select
    uint32_t code = ADC.codeSelect(ichan);
    spi.WriteRead(code);

    // Read ADC
    uint32_t datum = spi.WriteRead(ADC.codeReadFIFO());

    return ADC.decodeUSB(datum);
}

void MirrorControlBoard::measureADCStat(unsigned iadc, unsigned ichan, unsigned nmeas, uint32_t& mean, uint32_t& stddev)
{
    spi.Configure();
    initializeADC(iadc);
    selectADC(iadc);
    uint32_t code   = ADC.codeSelect(ichan);
    uint32_t sum             = 0;
    uint32_t sumsq           = 0;

    uint16_t *measurement = (uint16_t *) malloc(nmeas * sizeof(uint16_t));

    // Loop over number of measurements
    for(unsigned iloop=0; iloop<nmeas; iloop++) {
        // Read data
        uint32_t datum = spi.WriteRead(code);
        // Decode data
        measurement[iloop] = ADC.decodeUSB(datum);
        sum+=datum;
    }

    // Read last FIFO
    uint32_t datum = spi.WriteRead(ADC.codeReadFIFO());
    //decode data
    measurement[nmeas-1] = ADC.decodeUSB(datum);
    sum+=datum;

    mean = sum / nmeas;

    for(unsigned iloop=0; iloop<nmeas; iloop++) {
        sumsq += (measurement[iloop] - mean);
    }

    uint64_t variance = sumsq*sumsq/(nmeas-1);
    stddev = sqrt(variance);

    free(measurement);
}

//------------------------------------------------------------------------------
// General Purpose Utilities
//------------------------------------------------------------------------------

void MirrorControlBoard::waitHalfPeriod(unsigned frequency)
{
     /* This is a MACHINE DEPENDANT calibration constant */
     int calibrationConstant = (10*100000000/24);
     frequency = (calibrationConstant)/frequency;

     /* adjust for the number of microsteps */
     frequency = frequency * getUStep();
     for(volatile unsigned iloop=0;iloop<frequency;iloop++);
}
