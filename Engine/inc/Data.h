//#define _LOGGING


/*
============================================================================
 Name        : Data.h
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : CData declaration
============================================================================
*/

#ifndef Data_H
#define Data_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>
#include <s32strm.h>
#include <f32file.h>

#include <FuzzyByte\RP\RP_Types.h>

_LIT(KData_VERSION, "2.7");



class CData : public CBase
{
public: // Constructors and destructor

	~CData();

	static CData* NewL();
	static CData* NewLC();

	HBufC8* DecryptFromFileL(RFs& fs, const TDesC& fname);
	void EncryptToFileL(RFs& fs, const TDesC& fname, const TDesC8& data);
	TInt LoadData(RFs& fs, const TDesC& configFile);
	void SaveDataL(RFs& fs, const TDesC& configFile);
	void Default();
	TUint32 CrcNamesL();
	
	void SetAutoStartL(const TDesC& aPatchName, TAutoStatus aAutoStatus);
	TAutoStatus GetAutoStartL(const TDesC& aPatchName);
	void RemoveOldAutoEntriesL(const RPointerArray<HBufC>& aPatchList);

	void ExternalizeL(RWriteStream& aStream) const;
	void InternalizeL(RReadStream& aStream); 

private:
	HBufC8* DecryptFromFileL(RFs& fs, const TDesC& fname, const TDesC8& key);
	void EncryptToFileL(RFs& fs, const TDesC& fname, const TDesC8& data, const TDesC8& key);
	TInt LoadDataL(RFs& fs, const TDesC& configFile);
	void RemAutoStartL(const TDesC& aPatchName);
	CData();
	void ConstructL();

private:
	RPointerArray<HBufC> iAutoApps;
	RArray<TAutoStatus> iAutoStatus;
	
public:
	TInt32 iZipAlreadyProcessed;
	TUint32	 iAddUid;
	TUint16	 iCrcGui;
	TBool iDoneAutoDomain;
	TUint16  iBadGuiCrc;
	TBool iDoneAuto;
	TBuf<20>  iImei;
	TBuf<50> iNameGui;
	TBuf<50> iNameAuto;
	TBuf8<16> iIv;
	TBuf8<16> iKey;
	TBuf8<256> iLookup;
};

#endif // Data_H





