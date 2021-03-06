/*
 *  NCT677x.h
 *  HWSensors
 *
 *
 *  Based on code from Open Hardware Monitor project by Michael Möller (C) 2011
 * 
 *  Copyright (C) 2011-2012 THe KiNG and mozodojo. All Rights Reserved (R)
 *
 *
 */

/*
Version: MPL 1.1/GPL 2.0/LGPL 2.1

The contents of this file are subject to the Mozilla Public License Version
1.1 (the "License"); you may not use this file except in compliance with
the License. You may obtain a copy of the License at

http://www.mozilla.org/MPL/

Software distributed under the License is distributed on an "AS IS" basis,
WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
for the specific language governing rights and limitations under the License.

The Original Code is the Open Hardware Monitor code.

The Initial Developer of the Original Code is 
Michael Möller <m.moeller@gmx.ch>.
Portions created by the Initial Developer are Copyright (C) 2010-2011
the Initial Developer. All Rights Reserved.

Contributor(s):

Alternatively, the contents of this file may be used under the terms of
either the GNU General Public License Version 2 or later (the "GPL"), or
the GNU Lesser General Public License Version 2.1 or later (the "LGPL"),
in which case the provisions of the GPL or the LGPL are applicable instead
of those above. If you wish to allow use of your version of this file only
under the terms of either the GPL or the LGPL, and not to allow others to
use your version of this file under the terms of the MPL, indicate your
decision by deleting the provisions above and replace them with the notice
and other provisions required by the GPL or the LGPL. If you do not delete
the provisions above, a recipient may use your version of this file under
the terms of any one of the MPL, the GPL or the LGPL.

*/

#include <IOKit/IOService.h>
#include <IOKit/IORegistryEntry.h>
#include <IOKit/IOPlatformExpert.h>
#include <IOKit/IODeviceTreeSupport.h>
#include <IOKit/IOKitKeys.h>

#include "SuperIOFamily.h"

const UInt8 NUVOTON_HARDWARE_MONITOR_LDN        = 0x0B;

const UInt8 NUVOTON_ADDRESS_REGISTER_OFFSET     = 0x05;
const UInt8 NUVOTON_DATA_REGISTER_OFFSET        = 0x06;
const UInt8 NUVOTON_BANK_SELECT_REGISTER        = 0x4E;
const UInt16 NUVOTON_VENDOR_ID                  = 0x5CA3;

// Hardware Monitor Registers
const UInt16 NUVOTON_VENDOR_ID_HIGH_REGISTER    = 0x804F;
const UInt16 NUVOTON_VENDOR_ID_LOW_REGISTER     = 0x004F;
const UInt16 NUVOTON_VOLTAGE_VBAT_REG           = 0x0551;

// Temperatures                                    SYSTIN  CPUTIN
const UInt16 NUVOTON_TEMPERATURE_REG[]          = { 0x27,  0x73};

// Voltages                                         VCORE VIN0  AVCC  3VCC  VIN1  VIN2  VIN3  3VSB   VBAT
const UInt16 NUVOTON_VOLTAGE_REG[]              = { 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x550, 0x551 };
const float  NUVOTON_VOLTAGE_SCALE[]            = { 8,    8,    16,   16,   8,    8,    8,    16,    16 };

// Fans                                             SYSFAN  CPUFAN  AUXFAN  AUXFAN1  AUXFAN2
const UInt16 NUVOTON_FAN_RPM_REG[]              = { 0x656,  0x658,  0x65A,  0x65C,  0x65E };
const UInt16 NUVOTON_FAN_PWM_OUT_REG[]          = { 0x001,  0x003,  0x011 };
const UInt16 NUVOTON_FAN_PWM_COMMAND_REG[]      = { 0x109,  0x209,  0x309 };
const UInt16 NUVOTON_FAN_CONTROL_MODE_REG[]     = { 0x102,  0x202,  0x302 };

// NCT677x Voltage Label
// TODO: Make and implement(how?) the math for voltages...
// UNKN0-3: Seems they are vendor implementations, eg ASUS use VIN0 for +12V, not the same on ASRock

// VCORE = "CPU VCore",
// VIN0  = "UNKN0", // +12V?
// AVCC  = "AVCC", // Analog +3.3 V power input. Internally supply power to all analog circuits
// 3VCC  = "3VCC", // +3.3 V power supply for driving 3 V on host interface.
// VIN1  = "UNKN1", // +5V?
// VIN2  = "UNKN2", // -5V?
// VIN3  = "UNKN3", // Disabled if AUXTIN is used, and if not?
// 3VSB  = "3VSB", // +3.3 V stand-by power supply for the digital circuits
// VBAT  = "VBAT" // +3 V on-board battery for the digital circuits(EN_VBAT_MNT MUST BE SET!)

enum NCT677xModel {
  NCT6771F = 0xB470,
  NCT6776F = 0xC330,
  NCT6779D = 0xC560
};

class NCT677x : public SuperIOMonitor
{
  OSDeclareDefaultStructors(NCT677x)
  
public:
  virtual bool			  init(OSDictionary *properties=0);
	virtual IOService*	probe(IOService *provider, SInt32 *score);
  virtual bool			  start(IOService *provider);
	virtual void			  stop(IOService *provider);
	virtual void			  free(void);
  

private:
  char              vendor[40];
  char              product[40];
  
  
  int                 temperature[2];
  bool                temperatureIsObsolete[2];

  int                 minFanRPM;

  UInt8               readByte(UInt16 reg);
  void                writeByte(UInt16 reg, UInt8 value);

  virtual bool        probePort();
//  virtual bool        startPlugin();
  virtual void        enter();
  virtual void        exit();

  virtual long        readTemperature(unsigned long index);
  virtual long        readVoltage(unsigned long index);
  virtual long        readTachometer(unsigned long index);

  virtual const char  *getModelName();

};
