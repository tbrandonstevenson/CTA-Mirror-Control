/*
 * cbc.cpp Console board control
 */

#include <iomanip>
#include <cstdlib>
#include <cassert>
#include <cstring>
#include <unistd.h>
#include <time.h>
#include <cbc.hpp>
#include <math.h>


#include <stdio.h>
#include <iostream>


// DESTROYER
CBC::~CBC() {};

// Constructor..
CBC::CBC (
        int  microsteps,
        int  steppingFrequency,
        bool highCurrentMode,
        bool driveSR,
        int  adcReadDelay,
        int  defaultADCSamples,
        int  usbEnable,
        int  driveEnable
        )
{
    gpio.ConfigureAll();

    powerUp();

    /* Microsteps */
    driver.setMicrosteps(microsteps);

    /* Set Stepping Frequency */
    driver.setSteppingFrequency(steppingFrequency);

    /* High Current Mode */
    if (highCurrentMode)
        driver.enableHighCurrent();
    else
        driver.disableHighCurrent();

    /* Drive SR */
    if (driveSR)
        driver.enableSR();
    else
        driver.disableSR();

    /* ADC Read Delay */
    adc.setReadDelay(adcReadDelay);

    /* ADC Number of Samples */
    adc.setDefaultSamples(defaultADCSamples);

    /* Turn on Ethernet Dongle */
    usb.enableEthernet();

    /* Configure USBs */
    for (int i=0; i<6; i++) {
        if ((usbEnable >> i) & 0x1)
            usb.enable(i+1);
        else
            usb.disable(i+1);
    }

    /* Configure Drives */
    for (int i=0; i<6; i++) {
        if ((driveEnable >> i) & 0x1)
            driver.enable(i+1);
        else
            driver.disable(i+1);
    }
}

void CBC::powerUp()
{
    // turn on level shifters
    mcb.enableIO();

    driver.wakeup();
    driver.reset();
    encoder.enable();
    auxSensor.enable();

    mcb.initializeADC(0);
    mcb.initializeADC(1);
    //spi.Configure();
    //spi.WriteRead(0x0000);

}

void CBC::powerDown() {
    driver.disableAll();
    driver.sleep();
    encoder.disable();
    auxSensor.disable();
    usb.disableAll();
}

//===========================================================================
//=USB Control===============================================================
//===========================================================================

void CBC::USB::enable(int iusb)
{
    if((iusb<1)||(iusb>6))
        return;
    mcb.powerUpUSB(iusb);
}

void CBC::USB::disable(int iusb)
{
    if((iusb<1)||(iusb>6))
        return;
    mcb.powerDownUSB(iusb);
}

void CBC::USB::enableAll()
{
    for (int i=1; i<7; i++)
        enable(i);
}


void CBC::USB::disableAll()
{
    for (int i=1; i<7; i++)
        disable(i);
}

bool CBC::USB::isEnabled (int iusb)
{
    if ((iusb<1) | (iusb>6))
        return(-1);
    else
        return (mcb.isUSBPoweredUp(iusb));
}

void CBC::USB::enableEthernet()
{
    mcb.powerUpUSB(0);
}

void CBC::USB::disableEthernet()
{
    mcb.powerDownUSB(0);
}

void CBC::USB::resetEthernet()
{
    disableEthernet();
    sleep(1);
    enableEthernet();
}

//===========================================================================
//=Motor Driver Control======================================================
//===========================================================================

void CBC::Driver::setMicrosteps(int usint)
{
    MirrorControlBoard::UStep us=MirrorControlBoard::USTEP_1;
    switch(usint) {
        case 1:
            us = MirrorControlBoard::USTEP_1;
            break;
        case 2:
            us = MirrorControlBoard::USTEP_2;
            break;
        case 4:
            us = MirrorControlBoard::USTEP_4;
            break;
        case 8:
            us = MirrorControlBoard::USTEP_8;
            break;
        default:
            return;
    }
    mcb.setUStep(us);
}

int CBC::Driver::getMicrosteps()
{
    unsigned us = mcb.getUStep();
    int steps = 0;
    switch(us) {
        case 0:
            steps = 1;
            break;
        case 1:
            steps = 2;
            break;
        case 2:
            steps = 4;
            break;
        case 3:
            steps = 8;
            break;
        default:
            return(0);
    }
    return (steps);
}

void CBC::Driver::enable(int drive)
{
    // whine if drive is out of bounds
    if ((drive<1)||(drive>6))
        return;

    //enable drive
    mcb.enableDrive(drive-1); //MCB counts from zero
}

void CBC::Driver::disable(int drive)
{
    // whine if drive is out of bounds
    if ((drive<1)||(drive>6))
        return;

    //disable drive
    mcb.disableDrive(drive-1); //MCB counts from zero
}

void CBC::Driver::enableAll()
{
    for (int i=1; i<7; i++)
        enable(i);
}

void CBC::Driver::disableAll()
{
    for (int i=1; i<7; i++)
        disable(i);
}

bool CBC::Driver::isEnabled(int drive)
{
    return (mcb.isDriveEnabled(drive));
}

void CBC::Driver::sleep()
{
    mcb.powerDownDriveControllers();
}

void CBC::Driver::wakeup()
{
    mcb.powerUpDriveControllers();
}

bool CBC::Driver::isAwake()
{
    MirrorControlBoard mcb;
    return(mcb.isDriveControllersPoweredUp());
}

bool CBC::Driver::isHighCurrentEnabled()
{
    MirrorControlBoard mcb;
    return(mcb.isDriveHiCurrentEnabled());
}

void CBC::Driver::enableHighCurrent ()
{
    mcb.enableDriveHiCurrent();
}

void CBC::Driver::disableHighCurrent ()
{
    mcb.disableDriveHiCurrent();
}

bool CBC::Driver::isSREnabled()
{
    return(mcb.isDriveSREnabled());
}

void CBC::Driver::enableSR()
{
    mcb.enableDriveSR();
}

void CBC::Driver::disableSR()
{
    mcb.disableDriveSR();
}

void CBC::Driver::setSteppingFrequency (int frequency)
{
        steppingFrequency = frequency;
}

void CBC::Driver::reset()
{
    mcb.setPhaseZeroOnAllDrives();
}

void CBC::Driver::step(int drive, int nsteps)
{
    step(drive, nsteps, steppingFrequency);
}

void CBC::Driver::step(int drive, int nsteps, int frequency)
{
    /* Check frequency limits */
    if (frequency > maximumSteppingFrequency)
        frequency = maximumSteppingFrequency;
    else if (frequency < minimumSteppingFrequency)
        frequency = minimumSteppingFrequency;

    /* whine if invalid actuator number is used */
    if ((drive<1)||(drive>6))
        return;

    if (isEnabled(drive)) {
        /* MCB counts from 0 */
        drive = drive - 1;

        /* if nstep > 0, extend. If nstep < 0, retract */
        MirrorControlBoard::Dir dir = MirrorControlBoard::DIR_EXTEND;
        if (nsteps<0)
            dir = MirrorControlBoard::DIR_RETRACT, nsteps=-nsteps;

        /* Convert from microsteps to macrosteps */
        unsigned microsteps = nsteps * getMicrosteps();

        /* loop over number of micro Steps */
        for (unsigned istep=0; istep<microsteps; istep++) {
            /* Give this thread higher priority to improve timing stability */
            pthread_t this_thread = pthread_self();
            struct sched_param params;
            params.sched_priority = sched_get_priority_max(SCHED_FIFO);
            pthread_setschedparam(this_thread, SCHED_FIFO, &params);

            /* Step the drive */
            mcb.stepOneDrive(drive, dir, frequency * getMicrosteps());
        }
        sched_yield();
        return;
    }
}

//===========================================================================
//=Encoder Control===========================================================
//===========================================================================

void CBC::Encoder::enable()
{
    mcb.powerUpEncoders();
}

void CBC::Encoder::disable()
{
    mcb.powerDownEncoders();
}

bool CBC::Encoder::isEnabled()
{
    return (mcb.isEncodersPoweredUp());
}

//===========================================================================
//=ADC Control/Readout=======================================================
//===========================================================================

CBC::ADC::adcData CBC::ADC::measure(int adc, int channel, int nsamples)
{
    uint32_t sum;
    uint64_t sumsq;
    uint32_t min;
    uint32_t max;

    /* initialize to zero */
    adcData data;
    memset(&data, 0, sizeof(adcData));

    /* Make sure we are doing something sensible */
    if ((adc > 1) | (adc < 0 ))
        return(data);
    if ((channel > 10) | (channel < 0 ))
        return(data);
    if (nsamples < 0)
        return(data);

    mcb.measureADCStat(adc, channel, nsamples, sum, sumsq, min, max, readDelay);

    uint32_t mean   = sum/nsamples;
    float    var    = (sumsq - (1.0*sum*sum)/nsamples)/nsamples;
    float    stddev = sqrt(var);

    data.voltage      = tlcadc.voltData(mean,                  volt_full);
    data.stddev       = tlcadc.voltData(stddev,                volt_full);
    data.voltageMin   = tlcadc.voltData(min,                   volt_full);
    data.voltageMax   = tlcadc.voltData(max,                   volt_full);
    data.voltageError = tlcadc.voltData(stddev/sqrt(nsamples), volt_full);

    return (data);
}

/*
 * Encoders
 */

CBC::ADC::adcData CBC::ADC::readEncoder (int encoder)
{
    return(readEncoder(encoder,defaultSamples));
}

CBC::ADC::adcData CBC::ADC::readEncoder (int encoder, int nsamples)
{
    /* Return all zeros if you read something wrong */
    if ( (encoder < 1) | (encoder > 6) ) {
        adcData data;
        memset(&data, 0, sizeof(adcData));
        return (data);
    }
    else {
        /* we count from zero in MCB */
        encoder = encoder-1;
        return(measure(0,encoder,nsamples));
    }
}

/*
 * Temperature Sensors
 */
CBC::ADC::adcData CBC::ADC::readOnboardTemp ()
{
    return(readOnboardTemp(defaultSamples));
}

CBC::ADC::adcData CBC::ADC::readOnboardTemp (int nsamples)
{
    return(measure(0,6,nsamples));
}

CBC::ADC::adcData CBC::ADC::readExternalTemp ()
{
    return(readExternalTemp(defaultSamples));
}

CBC::ADC::adcData CBC::ADC::readExternalTemp (int nsamples)
{
    return(measure(0,7,nsamples));
}

/*
 * Voltage References
 */

CBC::ADC::adcData CBC::ADC::readRefLow(int adc)
{
    return (readRefLow(adc, defaultSamples));
}

CBC::ADC::adcData CBC::ADC::readRefLow(int adc, int nsamples)
{
    return(measure(adc,10,nsamples));
}

CBC::ADC::adcData CBC::ADC::readRefMid(int adc)
{
    return (readRefMid(adc, defaultSamples));
}

CBC::ADC::adcData CBC::ADC::readRefMid(int adc, int nsamples)
{
    return(measure(adc,9,nsamples));
}

CBC::ADC::adcData CBC::ADC::readRefHigh(int adc)
{
    return (readRefHigh(adc, defaultSamples));
}

CBC::ADC::adcData CBC::ADC::readRefHigh(int adc, int nsamples) {
    return(measure(adc,8,nsamples));
}

/*
 * Voltage References
 */

int  CBC::ADC::getReadDelay()
{
    return (readDelay);
}

void CBC::ADC::setReadDelay(int delay)
{
    if (delay >= 0)
        readDelay = delay;
}

void CBC::ADC::setDefaultSamples(int nsamples) {
    if (nsamples > 0)
        defaultSamples = nsamples;
}

//===========================================================================
//=Sensor Control============================================================
//===========================================================================

void CBC::AUXsensor::enable()
{
    mcb.powerUpSensors();
}

void CBC::AUXsensor::disable()
{
    mcb.powerDownSensors();
}

bool CBC::AUXsensor::isEnabled()
{
    return(mcb.isSensorsPoweredUp());
}
