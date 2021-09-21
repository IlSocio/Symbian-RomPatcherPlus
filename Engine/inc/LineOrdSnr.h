/*
 ============================================================================
 Name		: LineOrdSnr.h
 Author	  : Marco Bellino
 Version	 : 1.0
 Copyright   : Guardian-Mobile.com
 Description : CLineOrdSnr declaration
 ============================================================================
 */

// ord_snr:1:sys\bin\securitymanager.dll:AABBCC:AABBCC

#ifndef LineOrdSnr_H
#define LineOrdSnr_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>
#include "abstractline.h"
#include "linesnr.h"

// CLASS DECLARATION

/**
 *  CLineOrdSnr
 * 
 */
class CLineOrdSnr : public CLineSnr
	{
public:
	// Constructors and destructor

	/**
	 * Destructor.
	 */
	~CLineOrdSnr();

	/**
	 * Two-phased constructor.
	 */
	static CLineOrdSnr* NewL(CStringTokenizer& st);

	/**
	 * Two-phased constructor.
	 */
	static CLineOrdSnr* NewLC(CStringTokenizer& st);
	TBool IsValidL(RDevPatcherClient& client, TBool& aWarning);

protected:
	void BaseConstructL(CStringTokenizer& st);
	
private:

	/**
	 * Constructor for performing 1st stage construction
	 */
	CLineOrdSnr();

	/**
	 * EPOC default constructor for performing 2nd stage construction
	 */
	void ConstructL(CStringTokenizer& st);

private:
	TUint16 iOrdinal;
	TLinAddr iOrdinalAddr;
	};

#endif // LineOrdSnr_H
