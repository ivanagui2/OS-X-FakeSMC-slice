/*
 *  Binding.h
 *  TheOnlyWorkingITESMCController
 *
 *  Created by Mozodojo on 13/06/10.
 *  Copyright 2010 mozodojo. All rights reserved.
 *
 */

#ifndef Binding_h
#define Binding_h

#include "FakeSMCBinding.h"

class Binding : public FakeSMCBinding
{
protected:
	char*			m_Key;
public:
	Binding*		Next;
	
	Binding(){};
	
	Binding(const char* key, const char* type, UInt8 size){
		//InfoLog("Binding key %s", key);
		
		m_Key = (char*)IOMalloc(5);
		bcopy(key, m_Key, 5);
		
		char* value = (char*)IOMalloc(size);
		FakeSMCAddKey(key, type, size, value, this);
		IOFree(value, size);
	};
	
	~Binding() {
		if (m_Key) {
			//InfoLog("Removing key %s binding", m_Key);
			FakeSMCRemoveKeyBinding(m_Key);
			IOFree(m_Key, 5);
		}
	};
	
	const char* GetKey() {return m_Key;};
	
	virtual IOReturn OnKeyRead(__unused const char* key, __unused char* data){return kIOReturnInvalid;};
	virtual IOReturn OnKeyWrite(__unused const char* key, __unused char* data){return kIOReturnInvalid;};
};

#endif