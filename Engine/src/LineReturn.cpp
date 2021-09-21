/*
 ============================================================================
 Name		: LineReturn.cpp
 Author	  : Marco Bellino
 Version	 : 1.0
 Copyright   : Guardian-Mobile.com
 Description : CLineReturn implementation
 ============================================================================
 */

#include "LineReturn.h"

CLineReturn::CLineReturn() : CAbstractLine(EReturn)
	{
	// No implementation required
	}
 
CLineReturn::~CLineReturn()
	{
	} 

CLineReturn* CLineReturn::NewLC()
	{
	CLineReturn* self = new (ELeave) CLineReturn();
	CleanupStack::PushL(self);
	self->ConstructL();
	return self;
	}

CLineReturn* CLineReturn::NewL()
	{
	CLineReturn* self = CLineReturn::NewLC();
	CleanupStack::Pop(); // self;
	return self;
	}

void CLineReturn::ConstructL()
	{

	}
