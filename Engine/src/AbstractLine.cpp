/*
 ============================================================================
 Name		: AbstractLine.cpp
 Author	  : Marco Bellino
 Version	 : 1.0
 Copyright   : Guardian-Mobile.com
 Description : CAbstractLine implementation
 ============================================================================
 */

#include "AbstractLine.h"

CAbstractLine::CAbstractLine(TLineType aType) : iType(aType)
	{
	// No implementation required
	}

CAbstractLine::~CAbstractLine()
	{
	}


TLineType CAbstractLine::Type()
	{
	return iType;
	}

TBool CAbstractLine::ApplyL(RDevPatcherClient& client, TBool& aWarning)
	{
	aWarning = EFalse;
	return ETrue;
	}

TBool CAbstractLine::RemoveL(RDevPatcherClient& client)
	{
	return ETrue;
	}


HBufC8* CAbstractLine::GetUnpatchDataL()
	{
	return HBufC8::NewL(1);
	}
