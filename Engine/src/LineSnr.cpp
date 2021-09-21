/*
 ============================================================================
 Name		: LineSnr.cpp
 Author	  : Marco Bellino
 Version	 : 1.0
 Copyright   : Guardian-Mobile.com
 Description : CLineSnr implementation
 ============================================================================
 */

#include "LineSnr.h"

CLineSnr::CLineSnr(TLineType aType) : CLineAbs(0, aType)
	{
	// No implementation required
	}

CLineSnr::CLineSnr() : CLineAbs(0, ESnr)
	{
	// No implementation required
	}

CLineSnr::~CLineSnr() 
	{
	}

CLineSnr* CLineSnr::NewLC(CStringTokenizer& st)
	{
	CLineSnr* self = new (ELeave) CLineSnr();
	CleanupStack::PushL(self);
	self->ConstructL(st);
	return self;
	}

CLineSnr* CLineSnr::NewL(CStringTokenizer& st)
	{
	CLineSnr* self = CLineSnr::NewLC(st);
	CleanupStack::Pop(); // self;
	return self;
	}

void CLineSnr::ConstructL(CStringTokenizer& st)
	{
	CLineSnr::BaseConstructL(st);
	}

void CLineSnr::BaseConstructL(CStringTokenizer& st)
	{
	TPtrC filename = st.NextToken();
	FindAddressEntryL(filename, iFileAddr, iFileLen);
	
	CLineAbs::BaseConstructL(st);
	}



TBool CLineSnr::IsValidL(RDevPatcherClient& client, TBool& aWarning)
	{
	aWarning = EFalse;
	if (iFileAddr == 0 || iFileAddr < iNewBytes.Length())
		{
		aWarning = ETrue;
		return ETrue;
		}
	
	if (iOriBytes.Length() != iNewBytes.Length())
		{
		return EFalse;
		}

	//	TInt offset = FindOriBytesPatternL(iFileAddr, iFileLen, client);
	RBuf8 buf( ReadBufferL(client, iFileAddr, iFileLen) );
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

