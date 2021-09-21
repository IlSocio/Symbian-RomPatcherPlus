/*
 ============================================================================
 Name		: LineEmpty.cpp
 Author	  : Marco Bellino
 Version	 : 1.0
 Copyright   : Guardian-Mobile.com
 Description : CLineEmpty implementation
 ============================================================================
 */
 
#include "LineEmpty.h"

CLineEmpty::CLineEmpty() : CAbstractLine(EEmpty)
	{  
	// No implementation required 
	}

CLineEmpty::~CLineEmpty()
	{
	}

CLineEmpty* CLineEmpty::NewLC()
	{
	CLineEmpty* self = new (ELeave) CLineEmpty();
	CleanupStack::PushL(self);
	self->ConstructL();
	return self;
	}

CLineEmpty* CLineEmpty::NewL()
	{
	CLineEmpty* self = CLineEmpty::NewLC();
	CleanupStack::Pop(); // self;
	return self;
	}

void CLineEmpty::ConstructL()
	{

	}
