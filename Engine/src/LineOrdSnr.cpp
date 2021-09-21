/*
 ============================================================================
 Name		: LineOrdSnr.cpp
 Author	  : Marco Bellino
 Version	 : 1.0
 Copyright   : Guardian-Mobile.com
 Description : CLineOrdSnr implementation
 ============================================================================
 */

#include "LineOrdSnr.h"
#include "LineSnr.h"

CLineOrdSnr::CLineOrdSnr() : CLineSnr(EOrdSnr)
	{
	// No implementation required
	}

CLineOrdSnr::~CLineOrdSnr() 
	{
	} 

CLineOrdSnr* CLineOrdSnr::NewLC(CStringTokenizer& st)
	{
	CLineOrdSnr* self = new (ELeave) CLineOrdSnr();
	CleanupStack::PushL(self);
	self->ConstructL(st);
	return self;
	}

CLineOrdSnr* CLineOrdSnr::NewL(CStringTokenizer& st)
	{
	CLineOrdSnr* self = CLineOrdSnr::NewLC(st);
	CleanupStack::Pop(); // self;
	return self;
	}

void CLineOrdSnr::ConstructL(CStringTokenizer& st)
	{
	CLineOrdSnr::BaseConstructL(st);
	}

void CLineOrdSnr::BaseConstructL(CStringTokenizer& st)
	{
	TLex lex(st.NextToken());
	lex.Val(iOrdinal, EDecimal);
	
	CLineSnr::BaseConstructL(st);
	
	if (iOrdinal <= 0)
		User::Leave(KErrCorrupt);		// Error
	}



TBool CLineOrdSnr::IsValidL(RDevPatcherClient& client, TBool& aWarning)
	{
	aWarning = EFalse;
	if (iFileAddr == 0 || iFileAddr < iNewBytes.Length())
		{
		aWarning = ETrue;
		return ETrue;
		}
	
	TBool res = Ordinal2Addr(client, iFileAddr, iOrdinal, iOrdinalAddr);
	if (res == EFalse)
		return EFalse;
	if (iOrdinalAddr == 0)
		{
		aWarning = ETrue;
		return ETrue;
		}

	if (iOriBytes.Length() != iNewBytes.Length())
		return EFalse;

	TInt fileLenToRemove = iOrdinalAddr-iFileAddr;
//	TInt offset = FindOriBytesPatternL(iFileAddr, iFileLen, client);
	RBuf8 buf( ReadBufferL(client, iOrdinalAddr, iFileLen-fileLenToRemove) );
	buf.CleanupClosePushL();
	TInt offset = FindOriBytesPatternL(buf, iOriBytes, iWildOriPos, iNewBytes, iWildNewPos);
	CleanupStack::PopAndDestroy(&buf);
	
	if (offset < 0)
		{
		aWarning = ETrue;
		return ETrue;
		}
	
	iAddr = iFileAddr + offset;	
	return ETrue;
	}

