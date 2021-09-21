/*
 ============================================================================
 Name		: LineOrdRel.cpp
 Author	  : Marco Bellino
 Version	 : 1.0
 Copyright   : Guardian-Mobile.com
 Description : CLineOrdRel implementation
 ============================================================================
 */

#include "LineOrdRel.h"
 
CLineOrdRel::CLineOrdRel() : CLineRel(EOrdRel)
	{
	// No implementation required
	} 
 
CLineOrdRel::~CLineOrdRel()
	{ 
	} 

CLineOrdRel* CLineOrdRel::NewLC(CStringTokenizer& st)
	{
	CLineOrdRel* self = new (ELeave) CLineOrdRel();
	CleanupStack::PushL(self);
	self->ConstructL(st);
	return self;
	}

CLineOrdRel* CLineOrdRel::NewL(CStringTokenizer& st)
	{
	CLineOrdRel* self = CLineOrdRel::NewLC(st);
	CleanupStack::Pop(); // self;
	return self;
	}

void CLineOrdRel::ConstructL(CStringTokenizer& st)
	{
	CLineOrdRel::BaseConstructL(st);
	}
	
void CLineOrdRel::BaseConstructL(CStringTokenizer& st)
	{	
	TLex lex(st.NextToken());
	lex.Val(iOrdinal, EDecimal);
	
	CLineRel::BaseConstructL(st);
	
	if (iOrdinal <= 0)
		User::Leave(KErrCorrupt);		// Error
	}



TBool CLineOrdRel::IsValidL(RDevPatcherClient& client, TBool& aWarning)
	{
	aWarning = EFalse;
	
	if (iFileAddr == 0)
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

	iAddr = iOrdinalAddr+iOffset;
	
	//TInt offset = FindOriBytesPatternL(iAddr, iOriBytes.Length(), client);
	RBuf8 buf( ReadBufferL(client, iAddr, iOriBytes.Length()) );
	buf.CleanupClosePushL();
	TInt offset = FindOriBytesPatternL(buf, iOriBytes, iWildOriPos, iNewBytes, iWildNewPos);
	CleanupStack::PopAndDestroy(&buf);

	return (offset == 0);
	}
