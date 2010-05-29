#include <IOKit/pci/IOPCIDevice.h>
#include <IOKit/IOTimerEventSource.h>

class PTKawainVi : public IOService {
    OSDeclareDefaultStructors(PTKawainVi)    
public:
	virtual int			update();
	virtual IOService*	probe(IOService *provider, SInt32 *score);
    virtual bool		start	(IOService *provider);
	virtual bool		init	(OSDictionary *properties=0);
	virtual void		free (void);
	virtual void		stop (IOService *provider);
private:
	volatile UInt8* nvio_base;
	UInt16 device_id;
	int arch;
	void get_gpu_arch ();
	float offset;
	float slope;
	IOPCIDevice * NVCard;
	IOMemoryMap * nvio;
};