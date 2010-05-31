#include <IOKit/IOService.h>
#include <IOKit/pci/IOPCIDevice.h>
#include <IOKit/IOTimerEventSource.h>
#include <IOKit/acpi/IOACPIPlatformDevice.h>
#include <IOACPIPlatformDeviceCh.h>

void FakeSMCAddKey (const char*, uint8_t, char*);
void FakeSMCAddKey (const char*, const char*, uint8_t, char*);
void FakeSMCAddKeyCallback (const char*, uint8_t, char*, OnKeyReadCallback);
void FakeSMCAddKeyCallback (const char*, const char*, uint8_t, char*, OnKeyReadCallback);
char* FakeSMCGetKey (const char*);
void FakeSMCRemoveKeyCallback(const char*);

class FakeSMC : public IOService
{
	OSDeclareDefaultStructors(FakeSMC)
private:
	IOACPIPlatformDevice *	SMCDevice;
	void * smc_array;

protected:
	IODeviceMemory					*bar[ 2 ];
public:
    virtual bool init(OSDictionary *dictionary = 0);
    virtual void free(void);
    virtual IOService *probe(IOService *provider, SInt32 *score);
    virtual bool start(IOService *provider);
    virtual void stop(IOService *provider);
};