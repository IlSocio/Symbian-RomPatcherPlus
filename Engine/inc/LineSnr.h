/*
 ============================================================================
 Name		: LineSnr.h
 Author	  : Marco Bellino
 Version	 : 1.0
 Copyright   : Guardian-Mobile.com
 Description : CLineSnr declaration
 ============================================================================
 */

// snr:sys\bin\securitymanager.dll:AABBCC:AABBCC

#ifndef LINESNR_H
#define LINESNR_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>
#include "abstractline.h"
#include "lineabs.h"

// CLASS DECLARATION

/**
 *  CLineSnr
 * 
 */
class CLineSnr : public CLineAbs
	{
public:
	// Constructors and destructor

	/**
	 * Destructor.
	 */
	~CLineSnr();

	/**
	 * Two-phased constructor.
	 */
	static CLineSnr* NewL(CStringTokenizer& st);

	/**
	 * Two-phased constructor.
	 */
	static CLineSnr* NewLC(CStringTokenizer& st);
	TBool IsValidL(RDevPatcherClient& client, TBool& aWarning);

protected:
	CLineSnr(TLineType aType);
	void BaseConstructL(CStringTokenizer& st);
	
private:

	/**
	 * Constructor for performing 1st stage construction
	 */
	CLineSnr();

	/**
	 * EPOC default constructor for performing 2nd stage construction
	 */
	void ConstructL(CStringTokenizer& st);

protected:
	TLinAddr iFileAddr;
	TLinAddr iFileLen;
	};

#endif // LINESNR_H
