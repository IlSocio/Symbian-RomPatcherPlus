/*
 ============================================================================
 Name		: LineComment.cpp
 Author	  : Marco Bellino
 Version	 : 1.0
 Copyright   : Guardian-Mobile.com
 Description : CLineComment implementation
 ============================================================================
 */

#include "LineComment.h"

CLineComment::CLineComment() : CAbstractLine(EComment)
	{
	// No implementation required
	}

CLineComment::~CLineComment()
	{
	}

CLineComment* CLineComment::NewLC()
	{
	CLineComment* self = new (ELeave) CLineComment();
	CleanupStack::PushL(self);
	self->ConstructL();
	return self;
	}

CLineComment* CLineComment::NewL()
	{
	CLineComment* self = CLineComment::NewLC();
	CleanupStack::Pop(); // self;
	return self;
	}

void CLineComment::ConstructL()
	{

	}


HBufC8* CLineComment::GetUnpatchDataL()
	{
	HBufC8* buf = HBufC8::NewL(1);
	buf->Des().Append(TChar(';'));
	return buf;
	}
