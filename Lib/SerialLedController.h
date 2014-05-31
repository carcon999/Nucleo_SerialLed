/*
  SerialLEDController.h
  
  Serial Led Controller Library For mbed v1.0  2014/05/31

  SerialLEDController was developed by @carcon999 in 2014.  This Library is
  originally distributed at carcon999's Homepage. 
  <http://blogs.yahoo.co.jp/carcon999/folder/1250867.html>

  SerialLEDController is free software: you can redistribute it and/or modify
  it under the terms of the GNU Lesser General Public License as published by
  the Free Software Foundation, either version 2.1 of the License, or
  (at your option) any later version.

  SerialLEDController is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public License
  along with SerialLEDController.  If not, see <http://www.gnu.org/licenses/>.

*/

#ifndef SERIALLEDCONTROLLER_H
#define SERIALLEDCONTROLLER_H

#include "mbed.h"
#include "Serial.h"

#define SERIALCOMMAND_FIXEDLEN  (8)   /* Serial Command Fixed Length */
#define PIN_RESET               (D4)  /* LED Control Reset Port */
#define PIN_BAUDRATE            (D5)  /* LED Control Baudrate Select Port */
#define PIN_NOTUSE              (NC)  /* Pin Not Used. */

class SerialLedController
{
private:
  const PinName pin_reset;    // reset pin
  const PinName pin_baudrate; // baudrate pin
  bool is_hi_speed;
  Serial* pSerial;
  
  DigitalOut* port_reset;
  DigitalOut* port_baudrate;

public:
  // constructor
  SerialLedController(const PinName _pinreset = PIN_RESET, const PinName _pinbr = PIN_BAUDRATE);

  // functions
  void begin(Serial* _pSerial, const bool _hispeed = true);
  bool write(const uint8_t tbl[][SERIALCOMMAND_FIXEDLEN], int size);
  void start(void);
  void stop(void);
  void reset(void);

private:
  void init(void);
  void select_baudrate(void);
  void shield_reset(void);
  void enable_trigger(void);
  char write_and_rsp(const uint8_t* pcmd);
  void serial_write(const uint8_t* pcmd, int len);
  char serial_read(int tout);
  
}; // SerialLedController

#endif // #ifndef SERIALLEDCONTROLLER_H