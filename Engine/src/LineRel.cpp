/*
 ============================================================================
 Name		: LineRel.cpp
 Author	  : Marco Bellino
 Version	 : 1.0
 Copyright   : Guardian-Mobile.com
 Description : CLineRel implementation
 ============================================================================
 */

#include "LineRel.h"

CLineRel::CLineRel(TLineType aType) : CLineAbs(0, aType)
	{
	// No implementation required
	}

CLineRel::CLineRel() : CLineAbs(0, ERel)
	{
	// No implementation required
	}

CLineRel::~CLineRel()
	{
	}

CLineRel* CLineRel::NewLC(CStringTokenizer& st)
	{
	CLineRel* self = new (ELeave) CLineRel();
	CleanupStack::PushL(self);
	self->ConstructL(st);
	return self;
	}

CLineRel* CLineRel::NewL(CStringTokenizer& st)
	{
	CLineRel* self = CLineRel::NewLC(st);
	CleanupStack::Pop(); // self;
	return self;
	}

void CLineRel::ConstructL(CStringTokenizer& st)
	{
	CLineRel::BaseConstructL(st);
	}
	
void CLineRel::BaseConstructL(CStringTokenizer& st)
	{
	TPtrC filename = st.NextToken();
	FindAddressEntryL(filename, iFileAddr, iFileLen);
	iAddr = iFileAddr;
	
	TLex lex(st.NextToken());
	lex.Val(iOffset, EHex);
	if (iFileAddr != 0)
		iAddr = iFileAddr + iOffset;
	
	CLineAbs::BaseConstructL(st);
	}


TBool CLineRel::IsValidL(RDevPatcherClient& client, TBool& aWarning)
	{
	aWarning = EFalse;
	
	if (iFileAddr == 0)
		{
		aWarning = ETrue;
		return ETrue;
		}
	
	if (iOriBytes.Length() != iNewBytes.Length())
		return EFalse;

//	TInt offset = FindOriBytesPatternL(iAddr, iOriBytes.Length(), client);
	RBuf8 buf( ReadBufferL(client, iAddr, iOriBytes.Length()) );
	buf.CleanupClosePushL();
	TInt offset = FindOriBytesPatternL(buf, iOriBytes, iWildOriPos, iNewBytes, iWildNewPos);
	CleanupStack::PopAndDestroy(&buf);
	return (offset == 0);
	}
