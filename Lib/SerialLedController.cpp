/*
  SerialLedController.cpp
 */

#include "SerialLedController.h"

#define ACK                  (0x06)         /* Return value of success. */
#define UART_HI_SPEED        (57600)        /* 57600bps */
#define UART_LO_SPEED        (19200)        /* 19200bps */
#define UART_RES_TOUT        (1000)         /* 1000ms time out */

/* Run. ACK command, returned to play after completion. */
const uint8_t play_cmd[SERIALCOMMAND_FIXEDLEN] = {
    0x94, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

/* Stop. */
const uint8_t stop_cmd[SERIALCOMMAND_FIXEDLEN] = {
    0x94, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

// constructor
SerialLedController::SerialLedController(const PinName _pinreset, const PinName _pinbr)
    : pin_reset(_pinreset), pin_baudrate(_pinbr)
{
    port_reset = new DigitalOut(pin_reset);
    port_baudrate = new DigitalOut(pin_baudrate);
}


void SerialLedController::begin(Serial* _pSerial, const bool _hispeed)
{
    pSerial = _pSerial;
    is_hi_speed = _hispeed;

    /* Initialization process of software serial */
    if(is_hi_speed) {
        pSerial->baud(UART_HI_SPEED);
    } else {
        pSerial->baud(UART_LO_SPEED);
    }

    /* initialize */
    init();
}

void SerialLedController::reset(void)
{
    init();
}

void SerialLedController::init(void)
{
    /* select the communication speed. */
    select_baudrate();

    /* reset the controller. */
    shield_reset();

    /* enable the trigger port. */
    enable_trigger();
}

void SerialLedController::select_baudrate(void)
{
    if(pin_baudrate != PIN_NOTUSE) {
        if(is_hi_speed) {
            *port_baudrate = 0;
        } else {
            *port_baudrate = 1;
        }
    }
}

void SerialLedController::shield_reset(void)
{
    if(pin_reset != PIN_NOTUSE) {
        /* LED Contorl Reset. 50ms Low */
        *port_reset = 0;
        wait_ms(200);
        *port_reset = 1;

        /* It is necessary to wait for 200ms or more. */
        /* waits for the completion of the initialization process. */
        wait_ms(200);
    }
}

void SerialLedController::enable_trigger(void)
{
    if(pin_baudrate != PIN_NOTUSE) {
        *port_baudrate = 1;
    }
}

char SerialLedController::write_and_rsp(const uint8_t* pcmd)
{
    char rsp = NULL;

    if(pSerial) {
        /* dammy Read */
        while(pSerial->readable() != 0) {
            pSerial->getc();
        }

        /* send 8byte command. */
        serial_write(pcmd, SERIALCOMMAND_FIXEDLEN);

        /* wait return command. */
        rsp = serial_read(UART_RES_TOUT);
    }

    return rsp;
}

bool SerialLedController::write(const uint8_t tbl[][SERIALCOMMAND_FIXEDLEN], int size)
{
    int i;
    char rsp = 0;
    bool ret = false;
    int lines = size / SERIALCOMMAND_FIXEDLEN;

    /* play stop. */
    write_and_rsp(stop_cmd);

    /* Batch Transfer the data */
    for(i = 0 ; i < lines ; i++) {
        /* Send 8byte command and wait response. */
        rsp = write_and_rsp(&tbl[i][0]);

        /* check ack command. */
        if(rsp != ACK) {
            break;
        }
    }

    if(rsp == ACK) {
        ret = true;
    }

    return ret;
}

void SerialLedController::start(void)
{
    serial_write(play_cmd, sizeof(play_cmd));
}

void SerialLedController::stop(void)
{
    write_and_rsp(stop_cmd);
}

void SerialLedController::serial_write(const uint8_t* pcmd, int len)
{
    if(pSerial) {
        /* send 8byte command. */
        for(int i = 0 ; i < len ; i++) {
            /* writeable wait */
            while(pSerial->writeable() == 0);

            pSerial->putc(*(pcmd+i));
        }
    }
}

char SerialLedController::serial_read(int tout)
{
    char ret = NULL;
    int count = 0;
    
    if(pSerial) {
        
        while(count < tout)
        {
            /* readable wait */
            if(pSerial->readable() == 0){
                wait_ms(1);
                count++;
            }else{
                ret = pSerial->getc();
            }
        }
    }

    return ret;
}
