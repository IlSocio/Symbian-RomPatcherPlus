 /*
============================================================================
 Name        : Data.cpp
 Author      :
 Version     :
 Copyright   : Your copyright notice
 Description : CData implementation
============================================================================
*/

#include "Data.h"
#include <FuzzyByte\phone.h> 
#include <FuzzyByte\stringutils.h>  
#include <FuzzyByte\FileUtils.h> 
#include <FuzzyByte\aes.h> 
 
#include <bautils.H>
#include <S32FILE.H>
#include <S32MEM.H>
#include <hal.H>
#include <hal_data.H>

_LIT8(table, "\xC7\xD6\xD4\x92\x26\xFF\xE8\x2F\xF8\x4F\xB0\xC1\x46\xF5\xBF\x78\xDE\xCE\x51\x52\xFB\x9A\x6C\xD3\x5A\xA1\xE0\x94\xB5\xBB\x0D\xF1\x24\xB4\x40\xEB\x83\x22\x55\xBA\x90\x31\x11\x0C\x4E\xF9\x62\x97\x85\x6F\x49\x57\xFE\x9D\xBE\x23\x63\xFA\x33\x7C\x19\x48\x71\x34\x3D\xBC\x98\x88\xAC\xD2\xED\xB1\x74\x8A\xB3\xC4\x82\x5B\x7D\x5D\x39\x3C\x0B\x42\x6E\xAF\xF0\x81\xF4\x17\x84\x8B\xE9\xD5\xA0\x41\xC0\xD9\xC9\x89\x54\xD7\xE6\xA9\x75\x8D\xDC\x61\xE2\xF7\x91\x03\x68\xCB\x2D\xCF\x2C\x1D\xB7\x7A\xC2\x0F\x80\x86\x3E\x56\xBD\x99\xE5\x5F\x02\xA4\x7F\xEE\x04\x58\x45\x4A\xB9\xDA\xCC\x72\x95\xFC\x6A\x6B\x10\x15\x8E\xC5\x08\x8C\x14\x30\xB8\x36\x35\x47\x59\x18\xC8\x2A\x73\x67\xF2\xE7\x64\x77\xC6\xD0\x69\xDF\x4B\xE1\xA6\x29\x5E\x87\x09\xB6\x37\xA2\x05\x16\x4C\x5C\x96\x38\x1E\xAD\x1A\xFD\xEC\x9F\x0A\x93\x00\x20\x06\x9B\x12\x8F\x3F\xC3\x3B\x25\x2B\x28\x9C\x7B\xD8\xAB\x43\xA5\xD1\x1C\x27\x65\x13\x7E\x53\x1F\x60\x0E\xCA\x9E\xEF\xCD\xE3\x21\x44\x50\xA7\x79\xDB\xB2\x66\x3A\xE4\x2E\x32\x70\xDD\xAA\xF3\x6D\xAE\x4D\xEA\xF6\x1B\xA3\x01\x76\x07\xA8");

 
CData::CData() 
{	
	// No implementation required
} 

 
CData::~CData()
{
	iAutoStatus.Reset();
	iAutoStatus.Close();
	iAutoApps.ResetAndDestroy();
	iAutoApps.Close();
}

CData* CData::NewLC()
{
	CData* self = new (ELeave)CData();
	CleanupStack::PushL(self);
	self->ConstructL();
	return self;
}

CData* CData::NewL()
{
	CData* self=CData::NewLC();
	CleanupStack::Pop(); // self;
	return self;
}

 

TUint32 CData::CrcNamesL()
	{
	TUint32 res = 0;
	for (TInt i=0; i<iNameGui.Length(); i++)
		{
		TUint16 val = iNameGui.Ptr()[i];
		val <<= 1;
		val ^= iNameAuto.Ptr()[iNameAuto.Length()-i-1];
		res = (res + val) % 0xFFFF;
		res <<= 2;
		}
	// 0x229cc
    res += 0xE774E75D;
	return res;
	}


void CData::ConstructL()
{
	Default();

	CPhone* phone = CPhone::NewL();
	CleanupStack::PushL(phone);
	phone->GetImei(iImei);
	CleanupStack::PopAndDestroy(phone);
	
	iIv.Zero();
	iKey.Zero();
	iLookup.Zero();

	TInt sedici = 1;
	sedici <<= 4;
	for (int i = 0; i < 256; i++)
    {
        int j = 0;
        if (i % sedici == 0)
        	{
        	iIv.Append((TUint8) (i));
        	}
        if ((i % 11 == 0) && iKey.Length() < sedici )
        	{
        	iKey.Append(table()[i]);
        	}
        while ((j < table().Length()) && (table()[j] != i))
        {
            j++;
        }
        iLookup.Append((TUint8)j);
    }
}


void CData::SetAutoStartL(const TDesC& aPatchName, TAutoStatus aAutoStatus)
	{
	if (aAutoStatus == ENoAuto)
		{
		RemAutoStartL(aPatchName);
		return;
		}
	
	HBufC* lowPatch = aPatchName.AllocLC();
	lowPatch->Des().LowerCase();
	iAutoApps.Append( lowPatch );
	iAutoStatus.Append( aAutoStatus );
	CleanupStack::Pop(lowPatch);
	}

 
TAutoStatus CData::GetAutoStartL(const TDesC& aPatchName)
	{
	HBufC* patchHBuf = aPatchName.AllocLC();
	TIdentityRelation<HBufC> stringMatch( StringUtils::StringCompareF );
	
	TInt foundIdx = iAutoApps.Find(patchHBuf, stringMatch);
	CleanupStack::PopAndDestroy(patchHBuf);
	if (foundIdx < 0)
		return ENoAuto;
	return iAutoStatus[foundIdx];
	}


void CData::RemAutoStartL(const TDesC& aPatchName)
	{
	HBufC* patchHBuf = aPatchName.AllocLC();
	TInt index = -1;

	TIdentityRelation<HBufC> stringMatch( StringUtils::StringCompareF );
 
	while ((index = iAutoApps.Find(patchHBuf, stringMatch)) >= 0)
		{
		HBufC* name = iAutoApps[index];
		iAutoApps.Remove(index);
		iAutoStatus.Remove(index);
		delete name;
		name = NULL;
		}
	CleanupStack::PopAndDestroy(patchHBuf);
	}



void CData::RemoveOldAutoEntriesL(const RPointerArray<HBufC>& aPatchList)
	{
	TIdentityRelation<HBufC> stringMatch( StringUtils::StringCompareF );
	
	TInt i=0;
	while (i<iAutoApps.Count())
		{
		HBufC* name = iAutoApps[i];
				
		if (aPatchList.Find(name, stringMatch ) >= 0)
			{
			i++;
			} else 
			{
			iAutoApps.Remove(i);
			iAutoStatus.Remove(i);
			delete name;
			name = NULL;				
			}
		}
	}
 

void CData::Default()
		{
		iNameGui.Zero();
		iNameAuto.Zero();

		iAutoApps.ResetAndDestroy();
		iAutoStatus.Reset();
		iCrcGui = iBadGuiCrc = 0;
		iDoneAuto = iDoneAutoDomain = ETrue;
		}


const TUid KDataUid3={0xE0050C10}; 


HBufC8* CData::DecryptFromFileL(RFs& fs, const TDesC& fname)
	{
	return DecryptFromFileL(fs, fname, iKey);
	}
 
void CData::EncryptToFileL(RFs& fs, const TDesC& fname, const TDesC8& data)
	{
	EncryptToFileL(fs, fname, data, iKey);
	} 


HBufC8* CData::DecryptFromFileL(RFs& fs, const TDesC& fname, const TDesC8& key)
	{
	if (!BaflUtils::FileExists(fs, fname))
		return HBufC8::NewL(0);

	HBufC8* buf = FileUtils::ReadFileContentsL(fs, fname);
	CleanupStack::PushL(buf);
	TPtr8 ptr8 = buf->Des();
	for (int i=0; i<ptr8.Length(); i++)
		ptr8[i] = iLookup[ptr8[i]];		/**/
	HBufC8* res = AES::DecryptL(buf->Des(), iIv, key);
	CleanupStack::PopAndDestroy(buf);
	return res;
	}


void CData::EncryptToFileL(RFs& fs, const TDesC& fname, const TDesC8& aData, const TDesC8& key)
	{
	RBuf8 buf8(AES::EncryptL(aData, iIv, key));
	buf8.CleanupClosePushL();
	
	for (int i=0; i<buf8.Length(); i++)
		buf8[i] = table()[buf8[i]];			/**/
	FileUtils::DumpToFileL(fs, fname, buf8);
	
	CleanupStack::PopAndDestroy(&buf8);
	}


TInt CData::LoadData(RFs& fs, const TDesC& configFile)
	{
	TRAPD(err, LoadDataL(fs, configFile));
	if (err != KErrNone)
		Default();
	return err;
	}

TInt CData::LoadDataL(RFs& fs, const TDesC& configFile)
	{
	Default();
	TEntry entry; 
	TInt ris = fs.Entry(configFile, entry);
	if (ris != KErrNone)
		return ris;

	TBuf8<30> key; 
    TVolumeInfo info;
	fs.Volume(info, EDriveC);	
	key.AppendNumFixedWidthUC(info.iUniqueID, EHex, 16); 

	RBuf8 configBuffer(DecryptFromFileL(fs, configFile, key));
	configBuffer.CleanupClosePushL();
	
	RMemReadStream memStream(configBuffer.Ptr(), configBuffer.Length());
	CleanupClosePushL(memStream);
	memStream >> *(this);
	CleanupStack::PopAndDestroy(&memStream);
	
	CleanupStack::PopAndDestroy(&configBuffer);
	return KErrNone;
	}


  

void CData::SaveDataL(RFs& fs, const TDesC& configFile)
	{ 
	CBufBase* data = CBufFlat::NewL(50); 
	CleanupStack::PushL(data);
	
	RBufWriteStream strm;
	CleanupClosePushL(strm);
	strm.Open(*data);
	strm << *(this);
	CleanupStack::PopAndDestroy(&strm);
		
	TBuf8<30> key; 
    TVolumeInfo info;
	fs.Volume(info, EDriveC);	
	key.AppendNumFixedWidthUC(info.iUniqueID, EHex, 16); 
	
	TPtrC8 ptr = data->Ptr(0);
	EncryptToFileL(fs, configFile, ptr, key);
	
	CleanupStack::PopAndDestroy(data);	
	}

	

void CData::ExternalizeL(RWriteStream& aStream) const
	{
	aStream << KData_VERSION;
	
	const TUint32 tot = iAutoApps.Count();
	aStream << tot;
	for (TInt i=0; i<tot; i++)
		{
		TPtrC ptrName = iAutoApps[i]->Des();
		aStream << ptrName;
		TUint32 stat = iAutoStatus[i];
		aStream << stat;		
		} 
	aStream << iNameGui;
	aStream << iBadGuiCrc;
	aStream << iNameAuto;
	aStream << iZipAlreadyProcessed;

	aStream << iAddUid;	// Anticrack
	aStream << iCrcGui;		// Anticrack
	 
	aStream << (TInt32) iDoneAuto;

	//aStream << iCrcNames;		// Anticrack
	
	aStream << (TInt32) iDoneAutoDomain;
	}


void CData::InternalizeL(RReadStream& aStream)
	{
	TBuf<3> version;
	aStream >> version;
	if (version != KData_VERSION)
		{
		Default();
		return;
		}
	TBuf<20> imei;
	aStream >> imei;
	if (iImei != imei)
		{
		Default();
		return;		
		}
	
	TUint32 tot;
	aStream >> tot;
	iAutoApps.ResetAndDestroy();

	TFileName patchName;
	for (TInt32 i=0; i<tot; i++)
		{
		aStream >> patchName;
		HBufC* buf = patchName.AllocL();
		iAutoApps.Append( buf );
		TUint32 stat = 0;
		aStream >> stat;
		iAutoStatus.Append( (TAutoStatus)stat );
		}
	aStream >> iNameGui;
	aStream >> iBadGuiCrc;
	aStream >> iNameAuto;
	aStream >> iZipAlreadyProcessed;
	
	aStream >> iAddUid;	// Anticrack
	aStream >> iCrcGui;	// Anticrack
	
	aStream >> tot;
	iDoneAuto = (TBool) tot;

	// aStream >> iCrcNames;	// Anticrack
	
	aStream >> tot;
	iDoneAutoDomain = (TBool) tot;
	}


	
