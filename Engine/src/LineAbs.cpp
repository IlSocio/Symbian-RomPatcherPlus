/*
 ============================================================================
 Name		: LineAbs.cpp
 Author	  : Marco Bellino
 Version	 : 1.0
 Copyright   : Guardian-Mobile.com
 Description : CLineAbs implementation
 ============================================================================
 */

#include "LineAbs.h"
#include <e32rom.h>    
#include <FuzzyByte\RStringBuf.h>
  
#include <FuzzyByte\FileUtils.h>
#include <FuzzyByte\Notes.h>

CLineAbs::CLineAbs(TLinAddr addr) : CAbstractLine(EAbs)
	{
	iAddr = addr;
	}

CLineAbs::CLineAbs(TLinAddr addr, TLineType aType) : CAbstractLine(aType)
	{
	iAddr = addr;
	}


CLineAbs::~CLineAbs()
	{
	iWildNewPos.Close();
	iWildOriPos.Close();
	iOriBytes.Close();  
	iNewBytes.Close();   
	}

CLineAbs* CLineAbs::NewLC(CStringTokenizer& st)
	{
	CLineAbs* self = new (ELeave) CLineAbs(0);
	CleanupStack::PushL(self);
	self->ConstructL(st);
	return self;
	}

CLineAbs* CLineAbs::NewL(CStringTokenizer& st )
	{
	CLineAbs* self = CLineAbs::NewLC(st);
	CleanupStack::Pop(); // self;
	return self;
	}


void CLineAbs::ConstructL(CStringTokenizer& st)
	{
	TLex lex( st.NextToken() );
	lex.Val(iAddr, EHex);
	BaseConstructL(st);
	}


LOCAL_C void FillWildArray(RArray<TInt>& aWildArray, const TDesC& buffer)
	{
	// take in consideration the WildCard "??" and fills the iWildPos array
	TInt offset = 0;
	do 
		{
		TPtrC ptrOri( buffer.Mid(offset) );
		TInt posWild = ptrOri.Find(_L("??"));
		if (posWild >= 0)
			{
			TInt bytePos = (offset + posWild) / 2;
			aWildArray.Append( bytePos );
			offset += posWild + 2;
			}
		else
			{
			offset = buffer.Length();
			}	
		} 
	while (offset < buffer.Length());	
	}


LOCAL_C TBool CheckWildArraysAreValid(RArray<TInt>& aOri, RArray<TInt>& aNew)
	{
	if (aNew.Count() > aOri.Count())
		return EFalse;
	for (TInt i=0; i<aNew.Count(); i++)
		{
		TInt posNew = aNew[i];
		TInt j=0;
		while (j<aOri.Count() && aOri[j] != posNew)
			j++;
		if (j==aOri.Count())
			return EFalse;
		}
	return ETrue;
	}


HBufC8* CLineAbs::Ascii2BytesL(const TDesC& asciiBuf, RArray<TInt>& wildPos)
	{
	wildPos.Reset();
	FillWildArray( wildPos, asciiBuf);
	RBuf modAscii;
	modAscii.CleanupClosePushL();
	modAscii.Create(asciiBuf);
	StringUtils::ReplaceAll(modAscii, _L("??"), _L("AA"));
	HBufC8* hexBuf = StringUtils::ParseHexInBytesLC(modAscii);
	CleanupStack::Pop(hexBuf);
	CleanupStack::PopAndDestroy(&modAscii);
	return hexBuf;
	}


void CLineAbs::BaseConstructL(CStringTokenizer& st)
	{

	iOriBytes.Close();
	HBufC8* oriHexBytes = Ascii2BytesL(st.NextToken(), iWildOriPos);
	iOriBytes.Assign( oriHexBytes );

	iNewBytes.Close();
	HBufC8* newHexBytes = Ascii2BytesL(st.NextToken(), iWildNewPos);
	iNewBytes.Assign( newHexBytes );
	
	if (!CheckWildArraysAreValid( iWildOriPos, iWildNewPos ))
		{
		User::Leave(KErrCorrupt);		// Error
		}
	if (iOriBytes.Length() != iNewBytes.Length())
		{
		User::Leave(KErrCorrupt);		// Error
		}
/*	FillWildArray( iWildOriPos, oriHexBytes);
	FillWildArray( iWildNewPos, newHexBytes);
	
	if (!CheckWildArraysAreValid( iWildOriPos, iWildNewPos ))
		{
		User::Leave(KErrCorrupt);		// Error
		}		
	
	StringUtils::ReplaceAll(oriHexBytes, _L("??"), _L("BE"));
	StringUtils::ReplaceAll(newHexBytes, _L("??"), _L("EF"));
	
	HBufC8* oriBytes = StringUtils::ParseHexInBytesLC(oriHexBytes);
	HBufC8* newBytes = StringUtils::ParseHexInBytesLC(newHexBytes);
	
	if (oriBytes->Length() != newBytes->Length())
		{
		User::Leave(KErrCorrupt);		// Error
		}

	iOriBytes.Create( oriBytes->Des() );
	iNewBytes.Create( newBytes->Des() );
	
	CleanupStack::PopAndDestroy(newBytes);
	CleanupStack::PopAndDestroy(oriBytes);
	
	CleanupStack::PopAndDestroy(&newHexBytes);
	CleanupStack::PopAndDestroy(&oriHexBytes);*/
	}


TInt CLineAbs::FindWithWildcardsL(const TDes8& buf, const TDesC8& toFind, RArray<TInt>& wildPos)
	{
	// Search toSearch inside buf...
	if (toFind.Length() == 0)
		return -1;
	TInt offset = 0;
	
	/*RFs fs;
	fs.Connect();	
	FileUtils::DumpToFileL(fs, _L("c:\\toSearch.bin"), toSearch);
	FileUtils::DumpToFileL(fs, _L("c:\\buf.bin"), buf);	
	fs.Close();*/
	
	while  (buf.Length() - offset >= toFind.Length() )
		{
		// CheckMatch Using iWildPos...
/*		TBool log=EFalse;
		if (offset >= 0x8355)
			log=ETrue;
		if (offset >= 0x835A)
			log = EFalse;*/
		
		TInt wild_idx=0;
		TInt i=0;
		TBool noMatch = EFalse;
		while (i<toFind.Length() && noMatch == EFalse)
			{
/*			if (log)
				Notes::Debug_InfoL(_L("%x:%x"), buf[i], toSearch[i]);*/
			TBool isWildCardByte = (wild_idx < wildPos.Count() && i == wildPos[wild_idx]);
			if (isWildCardByte)
				{
				wild_idx++;
				} else
				{
				noMatch = (toFind[i] != buf[i + offset]);
				}
			i++;
			}
		if (i == toFind.Length())
			return offset;
		
		offset++;		
		}
	return -1;
	}

TBool CLineAbs::Ordinal2Addr(RDevPatcherClient& client, const TLinAddr& fileAddr, TInt ordinal, TLinAddr& addr)
	{
	TUint32 exportDirCount;
	TUint32 exportTableAddr;
	client.Read( fileAddr+0x3C, exportDirCount );
	client.Read( fileAddr+0x3C+4, exportTableAddr );

	if (ordinal == 0)
		return EFalse;
	if (ordinal > exportDirCount)
		return EFalse;
	
	TLinAddr addrApi=0;
	client.Read( exportTableAddr+(ordinal-1)*4, addrApi );
	
    // FIX Exports... Align to 16bit boundry to make IDA Happy
	TLinAddr alignedAddrApi = addrApi;
    TUint32 diff = (addrApi - fileAddr) % 2;
    if (diff > 0)
    {
        alignedAddrApi = addrApi - diff;
    }
    addr = alignedAddrApi;
	return ETrue;
	}

HBufC8* CLineAbs::ReadBufferL(RDevPatcherClient& client, TLinAddr& addr, TInt len)
	{
	HBufC8* res = HBufC8::NewLC(len);
	res->Des().SetLength(len);
	TPtr8 ptr = res->Des();	
	client.Read( addr, ptr );
	CleanupStack::Pop(res);
	return res;
	}

TInt CLineAbs::FindOriBytesPatternL(const TDes8& buf, TDes8& oriBytes, RArray<TInt>& oriWildPos, TDes8& newBytes, RArray<TInt>& newWildPos)
	{
	TBool hasWildcards = (oriWildPos.Count() > 0);
	if (!hasWildcards)
		return buf.Find( oriBytes );
	
	// Take in consideration the WildCard "??"
	TInt offset = FindWithWildcardsL(buf, oriBytes, oriWildPos);
	//Notes::Debug_InfoL(_L("FindWithWild:%x"), offset);
	// If exists the pattern replaces the "??" with matched values.
	if (offset < 0)
		return -1;
	
	for (TInt i=0; i<oriWildPos.Count(); i++)
		{
		TInt posByte = oriWildPos[i];
		oriBytes[posByte] = buf[offset + posByte];
		}
	oriWildPos.Reset();
	
	// Replaces the "??" with matched values in NewBytes...
	for (TInt i=0; i<newWildPos.Count(); i++)
		{
		TInt posByte = newWildPos[i];
		newBytes[posByte] = buf[offset + posByte];
		}
	newWildPos.Reset();
	return offset;
	}

/*
TInt CLineAbs::FindOriBytesPatternL(TLinAddr startAddr, TInt maxLen, RDevPatcherClient& client)
	{
	RBuf8 buf;
	buf.Create( maxLen );
	buf.CleanupClosePushL();
	buf.SetLength( maxLen );
	client.Read( startAddr, buf );
	TInt offset = FindOriBytesPatternL(buf, iOriBytes, iWildOriPos, iNewBytes, iWildNewPos);
	CleanupStack::PopAndDestroy( &buf );
	return offset;
	}
	*/
	
/*	RBuf8 buf;
	buf.Create( maxLen );
	buf.CleanupClosePushL();
	buf.SetLength( maxLen );
	client.Read( startAddr, buf );
	
	// Initialize the Wildcard flag
	TBool hasWildcards = (iWildOriPos.Count() > 0);
	TInt offset = -1;
	if (hasWildcards)
		{
		// Take in consideration the WildCard "??"
		offset = FindWithWildcardsL(buf, iOriBytes, iWildOriPos);
		//Notes::Debug_InfoL(_L("FindWithWild:%x"), offset);
		// If exists the pattern replaces the "??" with matched values.
		if (offset >= 0)
			{
			for (TInt i=0; i<iWildOriPos.Count(); i++)
				{
				TInt posByte = iWildOriPos[i];
				iOriBytes[posByte] = buf[offset + posByte];
				}
			iWildOriPos.Reset();
			
			// Replaces the "??" with matched values in NewBytes...
			for (TInt i=0; i<iWildNewPos.Count(); i++)
				{
				TInt posByte = iWildNewPos[i];
				iNewBytes[posByte] = buf[offset + posByte];
				}
			iWildNewPos.Reset();
			}
		}
	else
		{
		offset = buf.Find( iOriBytes );
		//Notes::Debug_InfoL(_L("Find:%x"), offset);
		}
	CleanupStack::PopAndDestroy( &buf );
	return offset;*/
	


TBool CLineAbs::IsValidL(RDevPatcherClient& client, TBool& aWarning)
	{
	aWarning = EFalse;
//	Notes::Debug_InfoL(_L("Abs_IsValid"));
	if (iAddr == 0)
		return EFalse; 
	
	if (iOriBytes.Length() != iNewBytes.Length())
		return EFalse;

	RBuf8 buf( ReadBufferL(client, iAddr, iOriBytes.Length()) );
	buf.CleanupClosePushL();
	TInt offset = FindOriBytesPatternL(buf, iOriBytes, iWildOriPos, iNewBytes, iWildNewPos);
	CleanupStack::PopAndDestroy(&buf);
//	TInt offset = FindOriBytesPatternL(iAddr, iOriBytes.Length(), client);
	return (offset == 0);
	}



TBool CLineAbs::ApplyL(RDevPatcherClient& client, TBool& aWarning)
	{
	if (!IsValidL(client, aWarning))
		return EFalse;
	
	if (iAddr == 0)
		{
		aWarning = ETrue;
		return ETrue;
		}
	TInt ris = client.Write(iAddr, iNewBytes);
	if (ris != KErrNone)
		return EFalse;
	
	TBool isRom = EFalse;
	User::IsRomAddress(isRom, (TAny *)iAddr);
	if (isRom)
		{
		}
	
	RBuf8 newBuf;
	newBuf.Create( iOriBytes );
	newBuf.CleanupClosePushL();
	client.Read(iAddr, newBuf);
	
	iMemChanged = (newBuf.Compare(iNewBytes) == 0);
/*	if (newBuf.Compare(iNewBytes) != 0)
		{
		//FileUtils::DumpToFileL(iEikonEnv->FsSession(), _L("e:\\mem.bin"), newBuf);
		//FileUtils::DumpToFileL(iEikonEnv->FsSession(), _L("e:\\new.bin"), line->iNewBytes);				
		res = EFalse;
		}*/
	CleanupStack::PopAndDestroy(&newBuf);
	
	return iMemChanged;
	}


TBool CLineAbs::RemoveL(RDevPatcherClient& client)
	{
	if (iAddr == 0)
   		return ETrue;
	
	// Non e' possibile fare questo check perche' viene chiamato RemoveL() anche per rimuovere le patch
	//if (!iMemChanged)
	//	return ETrue;
	//iMemChanged = EFalse;
	
	// restore Original Bytes...
	//if (client.Write(iAddr, iOriBytes) != KErrNone)
	//	return EFalse;
	// Ignora eventuali errori della write cosi' da poter procedere con il ripristino delle linee successive
	if (iWildOriPos.Count() == 0)
		{
		client.Write(iAddr, iOriBytes);		
		} else
		{
		// Wild non ha matchato oppure newbytes.length != oldbytes.length
		}
	
	// Le patch che non lavorano con la ROM danno errore altrimenti.
	TBool isRom = EFalse;
	User::IsRomAddress(isRom, (TAny *)iAddr);
	if (isRom)
		{
		// Non rimuove la Shadow Page perche' altrimenti 2 patch che usano la stessa pagina verrebbero disattivate entrambe
		// client.FreePage(iAddr, iNewBytes.Length());
		// Restituisce un errore se la pagina e' gia' stata rimossa da un'altra linea... 
		// Non bisogna segnalare errore
		return ETrue;
		}
		
	// CEikonEnv::InfoWinL(_L("NOT ROM"), _L(""));
	return ETrue;
	}


HBufC8* CLineAbs::GetUnpatchDataL()
	{
	TInt textSize = 3 + 1 + 8 + 1 + iOriBytes.Length() * 2 + 1 + iNewBytes.Length() * 2 ;
	textSize+=10; // just in case
	RStringBuf8 bufLine;
	bufLine.Create(textSize);
	bufLine.CleanupClosePushL();
	
	if (iAddr > 0)
		{
		// abs:00000000:ABDA2123:ABDA2123
    	TBuf8<10> hexAddr;
		hexAddr.AppendNum(iAddr, EHex);
		while (hexAddr.Length() < 8) hexAddr.Insert(0, _L8("0"));
		
		bufLine.AppendL( _L8("abs:") );
		bufLine.AppendL( hexAddr );
		
		bufLine.AppendL( _L8(":") );    		
		for (TInt i=0; i<iOriBytes.Length(); i++)
			{
			hexAddr.Zero();
			hexAddr.AppendNum(iOriBytes[i], EHex);
			if (hexAddr.Length() == 1)
				hexAddr.Insert(0, _L8("0"));
			bufLine.AppendL( hexAddr );
			}
		
		bufLine.AppendL( _L8(":") );
		for (TInt i=0; i<iNewBytes.Length(); i++)
			{
			bufLine.AppendL( _L8("00") );
			}
		}
	
	return bufLine.PopAndReturnAsHBufL();
	}





//
//Compares two filenames.  Folds ASCII characters to uppercase
//
LOCAL_C TInt Compare(const TDesC& aLeft, const TDesC& aRight)
	{
	TInt ll = aLeft.Length();
	TInt rl = aRight.Length();
	TInt len = Min(ll, rl);
	const TText* l = aLeft.Ptr();
	const TText* r = aRight.Ptr();
	while (len--)
		{
		TText lc = *l++;
		TText rc = *r++;
		if (lc >= 'A' && lc <= 'Z')
			lc += ('a' - 'A');
		if (rc >= 'A' && rc <= 'Z')
			rc += ('a' - 'A');
		TInt x = lc - rc;
		if (x)
			return x;
		}
	// match up to end of shorter string, now compare lengths
	return ll - rl;
	}


TInt TRomDir::BinarySearch(const TDesC& aName, TInt aLengthLimit, TInt aMode, TBool aDir) const
	{
	const TRomDirSortInfo* s = SortInfo();
	TInt l = aDir ? 0 : s->iSubDirCount;
	TInt r = aDir ? s->iSubDirCount : s->iSubDirCount + s->iFileCount;
	TBool found = EFalse;
	while (r>l)
		{
		TInt m=(l+r)>>1;
		const TRomEntry* e = SortedEntry(m);
		TInt nl = Min(e->iNameLength, aLengthLimit);
		TPtrC en((const TText*)&e->iName[0], nl);
		TInt k = Compare(aName, en);
		if (k==0)
			{
			if (aMode == EArrayFindMode_Any)
				{
//				RDebug::Printf("Found %d", m);
				return m;
				}
			found = ETrue;
			if (aMode == EArrayFindMode_First)
				r=m;
			else
				l=m+1;
			}
		else if (k>0)
			l=m+1;
		else
			r=m;
		}
	return found ? r : KErrNotFound;
	}


// Navigate the path to find the leaf directory, starting at this.
const TRomDir* TRomDir::FindLeafDir(const TDesC& aPath) const
	{
	TLex lex(aPath);
	TInt r;
	const TRomDir* d = this;
	FOREVER
		{
		lex.Inc(); // Skip the file separator
		lex.Mark();
		r=lex.Remainder().Locate(KPathDelimiter);
		if (r==KErrNotFound)
			r=lex.Remainder().Length();
		if (r==0) // End of the path
			break;
		lex.Inc(r); // Set the token length
		TInt ix = d->BinarySearch(lex.MarkedToken(), KMaxTInt, EArrayFindMode_Any, ETrue);
		if (ix<0)
			return NULL;
		const TRomEntry* e = d->SortedEntry(ix);
//		if (!(e->iAtt & KEntryAttDir))
//			return NULL;
		d = (const TRomDir*)e->iAddressLin;
		}
	return d;
	}




LOCAL_C TBool MatchEntryAtt(TUint anAtt, TUint aMatt)
	{
	if (aMatt & KEntryAttMatchExclude)
		{ // Include any except
		if ((anAtt & aMatt) == 0)
			return (ETrue);
		return (EFalse);
		}

	anAtt &= KEntryAttMaskSupported;
	//if ((aMatt&KEntryAttMustBeFile) && (anAtt&KEntryAttIllegal))
	//	return(EFalse); // Not a file

	if ((aMatt & KEntryAttHidden) == 0 && (anAtt & KEntryAttHidden))
		return (EFalse); // Ignore hidden unless requested
	if ((aMatt & KEntryAttSystem) == 0 && (anAtt & KEntryAttSystem))
		return (EFalse); // Ignore system unless requested
	if ((aMatt & KEntryAttDir) == 0 && (anAtt & KEntryAttDir))
		return (EFalse); // Ignore directory unless requested
	if (anAtt & KEntryAttVolume)
		return (EFalse); // Ignore volumes

	anAtt &= (~(KEntryAttHidden | KEntryAttSystem)); // remove hidden and system

	if (aMatt & KEntryAttMatchExclusive)
		{ // Exclude all except
		if ((anAtt & aMatt) != 0)
			return (ETrue);
		return (EFalse);
		}
	return (ETrue);
	}

LOCAL_C TLinAddr FindBinaryL(const TDesC& aName, TUint aAtt, TBool aAttKnown, TLinAddr aDir)
//
//Identical to FindL, but uses binary search for faster performance.
//However, can't deal with wildcards, whereas FindL can.
//
	{
	//Although the value of aEntry is not used, we expect it to be zero,
	//	__ASSERT_DEBUG(aEntry==0,Fault(ERomInvalidArgument));
	const TRomDir* d = (const TRomDir*) aDir;
	TBool ix;
	if (aAttKnown)
		ix = d->BinarySearch(aName, KMaxTInt, EArrayFindMode_Any, aAtt & KEntryAttDir);
	else
		{
		//We don't know whether we're looking for a file or a directory, so
		//look through both
		ix = d->BinarySearch(aName, KMaxTInt, EArrayFindMode_Any, EFalse);
		if (ix < 0 || !MatchEntryAtt(d->SortedEntry(ix)->iAtt, aAtt))
			ix = d->BinarySearch(aName, KMaxTInt, EArrayFindMode_Any, ETrue);
		}
	if (ix >= 0)
		{
		const TRomEntry* e = d->SortedEntry(ix);
		if (MatchEntryAtt(e->iAtt, aAtt))
			{
			return (TLinAddr) e;
			}
		}
	User::Leave(KErrNotFound);
	return 0;
	}
/**/

//
// Locate an entry from its full path name.
//
LOCAL_C void FindEntryL(const TDesC& aName, TUint anAtt, TBool aAttKnown, TLinAddr& aDir, TLinAddr& anEntry)
	{
#ifdef __WINSCW__
	return;
#endif
	// Bug-Fix... Se il path contiene il delimitatore allora deve essere presente anche all'inizio...
	// se non lo contiene allora si tratta di un file della root, va bene cosi (forse)
	TFileName name;
	if (aName.Locate(KPathDelimiter) >= 0)
		{
		if (aName.Length() > 0 && aName[0] != KPathDelimiter)
			name.Append(KPathDelimiter);
		}
	name.Append(aName);

	TLinAddr roomRootDir = UserSvr::RomRootDirectoryAddress();
	TInt namePos = name.LocateReverse(KPathDelimiter) + 1; // There is always a path delimiter
	const TRomDir* d = ((const TRomDir*) roomRootDir)->FindLeafDir(name.Left(namePos));
	if (!d)
		User::Leave(KErrPathNotFound);
	anEntry = 0;
	aDir = (TLinAddr) d;
	anEntry = FindBinaryL(name.Mid(namePos), anAtt, aAttKnown, aDir);
	}

TBool CLineAbs::FindAddressEntryL(const TDesC& aName, TLinAddr& aAddr, TUint32& aLength)
	{
#ifdef __WINSCW__
	return ETrue;
#endif
	TLinAddr dir;
	TLinAddr entry;
	TRAPD(err, FindEntryL(aName, KEntryAttMaskSupported, EFalse, dir, entry));
	if (err != KErrNone)
		return EFalse;
	TRomEntry* romEntry = (TRomEntry *) entry;
	aAddr = romEntry->iAddressLin;
	aLength = romEntry->iSize;
	if ((romEntry->iAtt & 0x80) > 0)
		{
		// Eseguibile
		aLength += 0x78;
		}
	return ETrue;
	}

