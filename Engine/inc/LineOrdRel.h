/*
 ============================================================================
 Name		: LineOrdRel.h
 Author	  : Marco Bellino
 Version	 : 1.0
 Copyright   : Guardian-Mobile.com
 Description : CLineOrdRel declaration
 ============================================================================
 */

// ord_snr:1:sys\bin\securitymanager.dll:AABBCC:AABBCC
// ord_rel:1:sys\bin\securitymanager.dll:10203040:AABBCC:AABBCC


#ifndef LineOrdRel_H
#define LineOrdRel_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>
#include "linerel.h"

// CLASS DECLARATION

/**
 *  CLineOrdRel
 * 
 */
class CLineOrdRel : public CLineRel
	{
public:
	// Constructors and destructor

	/**
	 * Destructor.
	 */
	~CLineOrdRel();

	/**
	 * Two-phased constructor.
	 */
	static CLineOrdRel* NewL(CStringTokenizer& st);

	/**
	 * Two-phased constructor.
	 */
	static CLineOrdRel* NewLC(CStringTokenizer& st);
	
	TBool IsValidL(RDevPatcherClient& client, TBool& aWarning);

protected:
	void BaseConstructL(CStringTokenizer& st);

private:

	/**
	 * Constructor for performing 1st stage construction
	 */
	CLineOrdRel();

	/**
	 * EPOC default constructor for performing 2nd stage construction
	 */
	void ConstructL(CStringTokenizer& st);

private:
	TUint16 iOrdinal;
	TLinAddr iOrdinalAddr;
	};

#endif // LineOrdRel_H
