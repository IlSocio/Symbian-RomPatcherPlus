/*
 ============================================================================
 Name		: RomPatcherEngine.h
 Author	  : Marco Bellino
 Copyright   : Guardian-Mobile.com
 Description : RomPatcherEngine.h - CRomPatcherEngine class header
 ============================================================================
 */

// This file defines the API for RomPatcherEngine.dll

#ifndef __ROMPATCHERENGINE_H__
#define __ROMPATCHERENGINE_H__

//  Include Files


#include <FuzzyByte\RP\RP_Types.h>
#include <e32base.h>	// CBase
#include <e32std.h>	 // TBuf
#include <f32file.h>
#include <FuzzyByte\RP\Data.h>

//  Constants
_LIT(KAUTOFILE, "c:\\System\\Data\\RPPlus.dat");
_LIT(KZIPFILE, "\\System\\Data\\RP_Patches.zip");
_LIT(KPATCHEXT, ".rmp");
_LIT(KPATCHEXTRPP, ".rpp");


class CAbstractLine;



//  Class Definitions

class RomPatcherEngine
	{
public:
	static TBool ApplyPatchL(RFs& aFs, const TDesC& aPatchNameAndExt);
	static void RemovePatchL(RFs& aFs, const TDesC& aPatchNameAndExt);
	static void SetAutoStartL(CData* aData, const TDesC& aPatchNameAndExt, TAutoStatus aAutoValue);
	//IMPORT_C static void GetPatchesStatusL(RFs& aFs, RPointerArray<HBufC>& aPatchList, RArray<TPatchStatus>& aPatchStatus, RArray<TAutoStatus>& aAutoStatus);
	static HBufC* GetPatchInfoLC(RFs& aFs, const TDesC& aPatchNameAndExt);
	static void GetSWVersionL(TDes& versionBuf);

	//...//
	static void GetPatchesStatusL(CData* aData, RFs& aFs, RPointerArray<HBufC>& aPatchList, RArray<TPatchStatus>& aPatchStatus,  RArray<TAutoStatus>& aAutoStatus);
	//static TBool GetPatchesDriveAndPath(RFs& aFs, TDes& driveAndPath);
	//static TBool GetPatchFileName(RFs& aFs, const TDesC& aPatchName, TDes& fname);
	//static void GetUnpatchFileName(RFs& aFs, const TDesC& aPatchName, TDes& fname);

	static TInt LoadDevice();
	static void FreeDevice();
	
private:
	static HBufC* ReadAndPreProcessFileL(RFs& aFs, const TDesC& aFileName, TInt& lineError);
	static CAbstractLine* ParseLineL(const TDesC& line);
	static TLinAddr SuperPageL();
	static TInt LoadPatchValuesL(RFs& fs, const TDesC& filename, RPointerArray<CAbstractLine>& array);
	static TInt ApplyPatchValuesL(RPointerArray<CAbstractLine>& array, TBool& hasWarnings);
	// static TInt CheckPatchValuesL(RPointerArray<CAbstractLine>& array, TBool& hasWarnings);
	static TInt RemovePatchValuesL(const RPointerArray<CAbstractLine>& array);
	static void CreateUnpatchFileL(RFs& aFs, const TDesC& aPatchName, RPointerArray<CAbstractLine> array);

private:
	// data
	};

#endif  // __ROMPATCHERENGINE_H__

