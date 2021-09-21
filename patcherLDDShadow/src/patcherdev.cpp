// f32test\Patcherldd\src\dPatcherdev.cpp
//
// Copyright (c) 2004-2004 Symbian Ltd.  All rights reserved.
//

#include "patcherdev.h"
#include "patcherc.h"



DECLARE_STANDARD_LDD()
    { 
	return( new DPatcherDevice() );
	}

  
// global Dfc Que
//TDynamicDfcQue* gDfcQ;

//
// DLL export number 1: Create a new LDD factory
//
// Chiamata quando si effettua la User::LoadLogicalDevice();


DPatcherDevice::DPatcherDevice()
	{	
	//static _LIT_SECURITY_POLICY_PASS(KAllowAllPolicy);
	iVersion = TVersion(KPatcherMajorVersion, KPatcherMinorVersion, KPatcherBuildVersion);
	iUnitsMask = 0xffffffff;
	}


TBool DPatcherDevice::IsAvailable(TInt aUnit, const TDesC *aPhysicalDevice, const TDesC8 *anInfo) const
	{
	return EFalse;
	}


DPatcherDevice::~DPatcherDevice()
	{
/*	if (gDfcQ)
		gDfcQ->Destroy();*/
	}
	

TInt DPatcherDevice::Install()
	{
/*	return KErrBadHandle;
	// Allocate a kernel thread to run the DFC 
	TInt r = Kern::DynamicDfcQCreate(gDfcQ, KDSPatcherThreadPriority, KDPatcherThread);

	if (r != KErrNone)
		return r; 	*/
	return SetName(&KDevName);
	}


/*TBool DPatcherDevice::QueryVersionSupported(const TVersion& aVer) const
	{
	return ETrue;
	}	*/


void DPatcherDevice::GetCaps(TDes8& aDes) const
	{
	RDevPatcherClient::TCaps caps;
    caps.iVersion = iVersion;

    // Write it back to user memory
    Kern::InfoCopy(aDes,(TUint8*)&caps,sizeof(caps));
	}
	
TInt DPatcherDevice::Create(DLogicalChannelBase*& aChannel)
	{
	aChannel = new DPatcherChannel();
	return aChannel ? KErrNone : KErrNoMemory;
	}


