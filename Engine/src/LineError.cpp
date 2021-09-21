/*
 ============================================================================
 Name		: LineError.cpp
 Author	  : Marco Bellino
 Version	 : 1.0
 Copyright   : Guardian-Mobile.com
 Description : CLineError implementation
 ============================================================================
 */
 
#include "LineError.h" 

CLineError::CLineError() : CAbstractLine(EError)
	{
	// No implementation required
	}  

CLineError::~CLineError()
	{  
	iMsg.Close();
	}

CLineError* CLineError::NewLC(CStringTokenizer& st)
	{
	CLineError* self = new (ELeave) CLineError();
	CleanupStack::PushL(self);
	self->ConstructL(st);
	return self;
	}

CLineError* CLineError::NewL(CStringTokenizer& st)
	{
	CLineError* self = CLineError::NewLC(st);
	CleanupStack::Pop(); // self;
	return self;
	}

void CLineError::ConstructL(CStringTokenizer& st)
	{
	iMsg.Create( st.NextToken() );
	}

TBool CLineError::ApplyL(RDevPatcherClient& client, TBool& aWarning)
	{
	aWarning = EFalse;
	
	RProcess proc;
	TBool fromGUI = (proc.SecureId().iId == 0xE0050C10);
	if (!fromGUI)
		return EFalse;
	
	// Show Message...
	RNotifier notif;
	User::LeaveIfError( notif.Connect() );
	TInt btnVal;
	TRequestStatus stat;
	notif.Notify(_L("ERROR:"), iMsg, _L("Ok"), _L(""), btnVal, stat);
	User::WaitForRequest( stat );
	notif.Close();
	return EFalse;
	}

