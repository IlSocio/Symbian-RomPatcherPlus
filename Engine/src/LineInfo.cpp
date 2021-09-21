/*
 ============================================================================
 Name		: LineInfo.cpp
 Author	  : Marco Bellino
 Version	 : 1.0
 Copyright   : Guardian-Mobile.com
 Description : CLineInfo implementation
 ============================================================================
 */

#include "LineInfo.h"

CLineInfo::CLineInfo() : CAbstractLine(EInfo)
	{
	// No implementation required
	}

CLineInfo::~CLineInfo()
	{
	iMsg.Close(); 
	}

CLineInfo* CLineInfo::NewLC(CStringTokenizer& st)
	{
	CLineInfo* self = new (ELeave) CLineInfo();
	CleanupStack::PushL(self);
	self->ConstructL(st);
	return self;
	}

CLineInfo* CLineInfo::NewL(CStringTokenizer& st)
	{
	CLineInfo* self = CLineInfo::NewLC(st);
	CleanupStack::Pop(); // self;
	return self;
	}

void CLineInfo::ConstructL(CStringTokenizer& st)
	{
	iMsg.Create( st.NextToken() );
	}

TBool CLineInfo::ApplyL(RDevPatcherClient& client, TBool& aWarning)
	{
	aWarning = EFalse;
	
	RProcess proc;
	TBool fromGUI = (proc.SecureId().iId == 0xE0050C10);
	if (!fromGUI)
		return ETrue;
	
	// Show Message...
	RNotifier notif;
	User::LeaveIfError( notif.Connect() );
	TInt btnVal;
	TRequestStatus stat;
	notif.Notify(_L("Information:"), iMsg, _L("Ok"), _L(""), btnVal, stat);
	User::WaitForRequest( stat );
	notif.Close();
	return ETrue;
	}
