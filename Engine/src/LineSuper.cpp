/*
 ============================================================================
 Name		: LineSuper.cpp
 Author	  : Marco Bellino
 Version	 : 1.0
 Copyright   : Guardian-Mobile.com
 Description : CLineSuper implementation
 ============================================================================
 */

#include "LineSuper.h"

CLineSuper::CLineSuper(TLinAddr aSuperPageAddr) : CLineAbs(aSuperPageAddr, ESuper)
	{
	// No implementation required
	}

CLineSuper::~CLineSuper()
	{
	}

CLineSuper* CLineSuper::NewLC(TLinAddr aSuperPageAddr, CStringTokenizer& st)
	{
	CLineSuper* self = new (ELeave) CLineSuper(aSuperPageAddr);
	CleanupStack::PushL(self);
	self->ConstructL(st);
	return self;
	}

CLineSuper* CLineSuper::NewL(TLinAddr aSuperPageAddr, CStringTokenizer& st)
	{
	CLineSuper* self = CLineSuper::NewLC(aSuperPageAddr, st);
	CleanupStack::Pop(); // self;
	return self;
	}

void CLineSuper::ConstructL(CStringTokenizer& st)
	{
	TLinAddr offs;
	TLex lex( st.NextToken() );
	lex.Val(offs, EHex);
	iAddr += offs;
	
	BaseConstructL(st);	
	}
