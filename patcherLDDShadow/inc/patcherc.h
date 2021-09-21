 
#ifndef __PATCHERC_H__
#define __PATCHERC_H__

#include <e32ver.h>
#include <e32cmn.h>


_LIT(KDevName, "RomPatcherPlus.Driver");


const TInt KPatcherMajorVersion = 0;
const TInt KPatcherMinorVersion = 2;
const TInt KPatcherBuildVersion = 1;
 

class RDevPatcherClient : public RBusLogicalChannel
	{
	public:
		
	class TCaps
		{
	public:
		TVersion iVersion;
		};
	    
	enum TControl
		{
		EControlFreePage = 100,
		EControlRead,
		EControlWriteInt,
		EControlGetSuperPageAddr,
		EControlReadInt,
		EControlWrite
		};
		
	inline TInt Open()
		{
#ifdef __WINSCW__
		return KErrNone;
#endif
		return (DoCreate(KDevName, TVersion(KPatcherMajorVersion, KPatcherMinorVersion, KPatcherBuildVersion), KNullUnit, NULL, NULL, EOwnerThread));
		}
	
	inline TInt Read(TLinAddr ptr, TDes8& data)
		{
#ifdef __WINSCW__
		return KErrNone;
#endif
		return DoControl(EControlRead, &ptr, &data);
		}		
	
	inline TInt Read(TLinAddr ptr, TUint32& data)
		{
#ifdef __WINSCW__
		return KErrNone;
#endif
		return DoControl(EControlReadInt, &ptr, &data);
		}		
	
	inline TInt Write(TLinAddr ptr, const TDesC8& data)
		{
#ifdef __WINSCW__
		return KErrNone;
#endif
		return DoControl(EControlWrite, &ptr, (TAny *)&data);
		}		
	
	inline TInt Write(TLinAddr ptr, TUint32 data)
		{
#ifdef __WINSCW__
		return KErrNone;
#endif
		return DoControl(EControlWriteInt, &ptr, &data);
		}		
	
	inline TInt FreePage(TLinAddr ptr, TInt length)
		{
#ifdef __WINSCW__
		return KErrNone;
#endif
		return DoControl(EControlFreePage, &ptr, &length);
		}
	
	inline TInt GetSuperPage(TLinAddr& ptr)
		{
#ifdef __WINSCW__
		return KErrNone;
#endif
		return DoControl(EControlGetSuperPageAddr, &ptr);
		}		
	};
	
#endif //__Patcher_H__
