/*
 *  NVClockX.cpp
 *  HWSensors
 *
 *  Created by mozo on 15/10/10.
 *  Copyright 2010 mozodojo. All rights reserved.
 *
 */

#include "NVClockX.h"
#include "FakeSMC.h"

#include <stdarg.h>
#include <string.h>
#include "backend.h"

#define Debug FALSE

#define LogPrefix "NVClockX: "
#define DebugLog(string, args...)	do { if (Debug) { IOLog (LogPrefix "[Debug] " string "\n", ## args); } } while(0)
#define WarningLog(string, args...) do { IOLog (LogPrefix "[Warning] " string "\n", ## args); } while(0)
#define InfoLog(string, args...)	do { IOLog (LogPrefix string "\n", ## args); } while(0)

#define super IOService
OSDefineMetaClassAndStructors(NVClockX, IOService)

NVClock	nvclock;
NVCard*	nv_card;

bool is_digit(char c)
{
	if (((c>='0')&&(c<='9'))||((c>='a')&&(c<='f'))||((c>='A')&&(c<='F')))
		return true;
	
	return false;
}

int NVClockX::probeDevices()
{
	nvclock.num_cards=0;
	
	if (OSDictionary * dictionary = serviceMatching(kGenericPCIDevice)) {
		if (OSIterator * iterator = getMatchingServices(dictionary)) {
			
			IOPCIDevice* device = 0;
			
			while (device = OSDynamicCast(IOPCIDevice, iterator->getNextObject())) {
				UInt16 vendor_id=0;
				
#if __LP64__
				mach_vm_address_t addr;
				//mach_vm_size_t size;
#else
				vm_address_t addr;
				//vm_size_t size;
#endif
				
				OSString *string = OSDynamicCast(OSString, device->getProperty("IOName"));
				OSData *data = OSDynamicCast(OSData, device->getProperty("vendor-id"));
				
				if (data)
					vendor_id = *(UInt32*)data->getBytesNoCopy();
				
				if (string && string->isEqualTo("display") && vendor_id==0x10de){
					device->setMemoryEnable(true);
					nvio = device->mapDeviceMemoryWithIndex(0);
#if __LP64__
					addr = (mach_vm_address_t)nvio->getVirtualAddress();
#else
					addr = (vm_address_t)nvio->getVirtualAddress();
#endif
					if (data = OSDynamicCast(OSData, device->getProperty("device-id"))) {
						nvclock.card[nvclock.num_cards].device_id=*(UInt32*)data->getBytesNoCopy();
					
						InfoLog("Vendor ID: %x, Device ID: %x", vendor_id, nvclock.card[nvclock.num_cards].device_id);
						
						nvclock.card[nvclock.num_cards].arch = get_gpu_arch(nvclock.card[nvclock.num_cards].device_id);
						
						InfoLog("Architecture: %x", nvclock.card[nvclock.num_cards].arch);
						
						nvclock.card[nvclock.num_cards].number = nvclock.num_cards;
						nvclock.card[nvclock.num_cards].card_name = (char*)get_card_name(nvclock.card[nvclock.num_cards].device_id, &nvclock.card[nvclock.num_cards].gpu);
						
						InfoLog("%s", nvclock.card[nvclock.num_cards].card_name);
						
						nvclock.card[nvclock.num_cards].state = 0;
						nvclock.card[nvclock.num_cards].reg_address = addr;
						
						//map_mem_card(&nvclock.card[nvclock.num_cards], addr);
						// Map the registers of the nVidia chip 
						// normally pmc is till 0x2000 but extended it for nv40 
						nvclock.card[nvclock.num_cards].PEXTDEV = (volatile unsigned int*)addr + 0x101000;
						nvclock.card[nvclock.num_cards].PFB     = (volatile unsigned int*)addr + 0x100000;
						nvclock.card[nvclock.num_cards].PMC     = (volatile unsigned int*)addr + 0x000000;
						nvclock.card[nvclock.num_cards].PCIO    = (volatile unsigned char*)addr + 0x601000;
						nvclock.card[nvclock.num_cards].PDISPLAY= (volatile unsigned int*)addr + NV_PDISPLAY_OFFSET;
						nvclock.card[nvclock.num_cards].PRAMDAC = (volatile unsigned int*)addr + 0x680000;
						nvclock.card[nvclock.num_cards].PRAMIN  = (volatile unsigned int*)addr + NV_PRAMIN_OFFSET;
						nvclock.card[nvclock.num_cards].PROM    = (volatile unsigned char*)addr + 0x300000;
						
						// On Geforce 8xxx cards it appears that the pci config header has been moved 
						if(nvclock.card[nvclock.num_cards].arch & NV5X)
							nvclock.card[nvclock.num_cards].PBUS = (volatile unsigned int*)addr + 0x88000;
						else
							nvclock.card[nvclock.num_cards].PBUS = nvclock.card[nvclock.num_cards].PMC + 0x1800/4;
						
						nvclock.card[nvclock.num_cards].mem_mapped = 1;
						
						InfoLog("Memory mapped successfully");
						
						nvclock.num_cards++;
					}
				}
			}
		}
	}
	
	if (nvclock.num_cards == 0)
		WarningLog("no nVidia graphics adapters found");
	
	return nvclock.num_cards;
}

bool NVClockX::addSensor(const char* key, const char* type, unsigned char size, int index)
{
	if (kIOReturnSuccess == fakeSMC->callPlatformFunction(kFakeSMCAddKeyHandler, false, (void *)key, (void *)type, (void *)size, (void *)this))
		return sensors->setObject(key, OSNumber::withNumber(index, 32));
	
	return 0;
}

int NVClockX::addTachometer(int index)
{
	for (int i = 0; i < 0x10; i++) {
		
		char name[5];
		
		snprintf(name, 5, KEY_FORMAT_FAN_SPEED, i); 
		
		if (addSensor(name, TYPE_FPE2, 2, index)) {
			
			UInt8 length = 0;
			void * data = 0;
			
			IOReturn result = fakeSMC->callPlatformFunction(kFakeSMCGetKeyValue, true, (void *)KEY_FAN_NUMBER, (void *)&length, (void *)&data, 0);
			
			if (kIOReturnError == result) {
				length = 1;
				
				if (kIOReturnSuccess != fakeSMC->callPlatformFunction(kFakeSMCAddKeyValue, true, (void *)KEY_FAN_NUMBER, (void *)TYPE_UI8, (void *)1, (void *)&length))
					WarningLog("error adding FNum value");
			}
			else if (kIOReturnSuccess == result) {
				length = 0;
				
				bcopy(data, &length, 1);
				
				length++;
				
				if (kIOReturnSuccess != fakeSMC->callPlatformFunction(kFakeSMCSetKeyValue, true, (void *)KEY_FAN_NUMBER, (void *)1, (void *)&length, 0))
					WarningLog("error updating FNum value");
			}
			else WarningLog("error reading FNum value");
			
			return i;
		}
	}
	
	return -1;
}

bool NVClockX::init(OSDictionary *properties)
{
	DebugLog("Initialising...");
	
    if (!super::init(properties))
		return false;
	
	if (!(sensors = OSDictionary::withCapacity(0)))
		return false;
}

IOService* NVClockX::probe(IOService *provider, SInt32 *score)
{
	DebugLog("Probing...");
	
	if (super::probe(provider, score) != this) return 0;
	
	return this;
}

bool NVClockX::start(IOService * provider)
{
	DebugLog("Starting...");
	
	if (!super::start(provider)) return false;
	
    if (!(fakeSMC = waitForService(serviceMatching(kFakeSMCService)))) {
		WarningLog("can't locate fake SMC device, kext will not load");
		return false;
	}
	
	char key[7];
	
	nvclock.dpy = NULL;
	
	if(!probeDevices()) {
		char buf[80];
		InfoLog("[Error] %s", get_error(buf, 80));
		return false;
	}
	
	for (int index = 0; index < nvclock.num_cards; index++) {
		/* set the card object to the requested card */
		if(!set_card(index)){
			char buf[80];
			InfoLog("[Error] %s", get_error(buf, 80));
			return 0;
		}
		
		nvbios* bios=read_bios("");
		nvclock.card[index].bios=bios;
		
		/* Check if the card is supported, if not print a message. */
		if(nvclock.card[index].gpu == UNKNOWN){
			InfoLog("it seems your card isn't officialy supported in FakeSMCnVclockPort yet.");
			InfoLog("please tell the author the pci_id of the card for further investigation.");
			InfoLog("continuing anyway");
		}
		
		if(nv_card->caps & (GPU_TEMP_MONITORING)) {
			snprintf(key, 5, KEY_FORMAT_GPU_DIODE_TEMPERATURE, index);
			addSensor(key, TYPE_SP78, 2, index);
			
			if(nv_card->caps & BOARD_TEMP_MONITORING) {
				snprintf(key, 5, KEY_FORMAT_GPU_BOARD_TEMPERATURE, index);
				addSensor(key, TYPE_SP78, 2, index);
			}
		}
		
		if(nv_card->caps & (I2C_FANSPEED_MONITORING | GPU_FANSPEED_MONITORING)){
			
			int fanIndex = addTachometer(index);
			
			if (fanIndex > -1) {
				snprintf(key, 5, KEY_FORMAT_FAN_ID, fanIndex);
				
				char name[6]; 
				
				snprintf (name, 6, "GPU %X", index);
				
				fakeSMC->callPlatformFunction(kFakeSMCAddKeyValue, false, (void *)key, (void *)TYPE_CH8, (void *)strlen(name), (void *)name);
			}
		}
		
		//snprintf(key, 5, "FGC%d", index);
		//gpuFreqSensor[index] = new FrequencySensor(key, "freq", 2);
	}
	
	return true;
}

void NVClockX::stop (IOService* provider)
{
	DebugLog("Stoping...");
	
	sensors->flushCollection();
	
	super::stop(provider);
}

void NVClockX::free ()
{
	DebugLog("Freeing...");
	
	sensors->release();
	
	super::free();
}

IOReturn NVClockX::callPlatformFunction(const OSSymbol *functionName, bool waitForFunction, void *param1, void *param2, void *param3, void *param4 )
{
	if (functionName->isEqualTo(kFakeSMCGetValueCallback)) {
		const char* key = (const char*)param1;
		void * data = param2;
		//UInt32 size = (UInt64)param3;
		
		if (key && data) {
			if (OSNumber *number = OSDynamicCast(OSNumber, sensors->getObject(key))) {
				
				UInt32 index = number->unsigned16BitValue();
				
				if (index >= 0 && index < nvclock.num_cards) {
					
					if (!set_card(index)){
						char buf[80];
						InfoLog("Error: %s", get_error(buf, 80));
						return kIOReturnSuccess;
					}
					
					UInt16 value = 0;
					
					if (key[0] == 'T') {
						switch (key[3]) {
							case 'D':
								if (nv_card->caps & GPU_TEMP_MONITORING)
									value = nv_card->get_gpu_temp(nv_card->sensor);
								break;
							case 'H':
								if (nv_card->caps & BOARD_TEMP_MONITORING)
									value = nv_card->get_board_temp(nv_card->sensor);
								break;
						}
					}
					else if (key[0] == 'F') {
						if (nv_card->caps & I2C_FANSPEED_MONITORING)
							value = nv_card->get_i2c_fanspeed_rpm(nv_card->sensor) << 2;
						else if(nv_card->caps & GPU_FANSPEED_MONITORING)
							value = (UInt16)nv_card->get_fanspeed() << 2;
					}
					
					bcopy(&value, data, 2);
					
					return kIOReturnSuccess;					
				}
			}
			
			return kIOReturnBadArgument;
		}
		
		return kIOReturnBadArgument;
	}
	
	return super::callPlatformFunction(functionName, waitForFunction, param1, param2, param3, param4);
}