/*
 *
 *  Copyright © 2010 mozodojo. All rights reserved.
 *
 */

#ifndef FakeSMCLPCMonitor_h
#define FakeSMCLPCMonitor_h

#include <IOKit/IOService.h>
#include <IOKit/IOTimerEventSource.h>

#include "ITE.h"


class TheOnlyWorkingITESMCController : public IOService
{
    OSDeclareDefaultStructors(TheOnlyWorkingITESMCController)    
private:
	//One-time timer
	IOWorkLoop*			WorkLoop;
	IOTimerEventSource*	TimerEventSource;

protected:
	SuperIO*			superio;
public:	
	virtual bool		init	(OSDictionary *properties=0);
	virtual IOService*	probe	(IOService *provider, SInt32 *score);
    virtual bool		start	(IOService *provider);
	virtual void		stop	(IOService *provider);
	virtual void		free	();
	
	//One-time timer
	virtual IOReturn	LoopTimerEvent	();
	virtual IOWorkLoop*	getWorkLoop		() const;
};

#endif
