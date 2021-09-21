// f32test\Patcherldd\inc\dtestusblogdev.h
//
// Copyright (c) 2004-2004 Symbian Ltd.  All rights reserved.
//

/** @file
	Test USB logical channel classes.

	@internalTechnology
*/

#ifndef __PATCHERDEVICE_H_
#define __PATCHERDEVICE_H_

#include "kerndefs.h"



class DPatcherDevice : public DLogicalDevice
	{
public:
	DPatcherDevice();
	~DPatcherDevice();
	TInt Install();
	void GetCaps(TDes8& aDes) const;
	TInt Create(DLogicalChannelBase*& aChannel);
	DLogicalChannel* CreateL();
	TBool IsAvailable(TInt aUnit, const TDesC *aPhysicalDevice, const TDesC8 *anInfo) const;
	};



	
class DPatcherChannel : public DLogicalChannelBase
	{
public:
	DPatcherChannel();	
	TInt Request(TInt aReqNo, TAny* a1, TAny* a2);
	TInt DoCreate(TInt aUnit, const TDesC8* anInfo, const TVersion& aVer);
	~DPatcherChannel();
private:
	TInt AllocShadowPage(TLinAddr ptrStartAddr, TUint32 len) const;
	TInt FreeShadowPage(TLinAddr ptrStartAddr, TUint32 len) const;
	TInt AllocShadowAndCopy(TLinAddr ptrDest, TLinAddr ptrSrc, TInt len) const;
	TInt MemCopy(TLinAddr ptrDest, TLinAddr ptrSrc, TInt len) const;
	TInt CopyFromTo(TLinAddr ptrDest, TLinAddr ptrSrc, TInt len) const;

private:
	DThread* iClient;
	};
			
#endif // __DPatcherDEVICE_H_
