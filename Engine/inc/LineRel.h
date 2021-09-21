/*
 ============================================================================
 Name		: LineRel.h
 Author	  : Marco Bellino
 Version	 : 1.0
 Copyright   : Guardian-Mobile.com
 Description : CLineRel declaration
 ============================================================================
 */

// rel:sys\bin\securitymanager.dll:10203040:AABBCC:AABBCC


#ifndef LINEREL_H
#define LINEREL_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>
#include "lineabs.h"

// CLASS DECLARATION

/**
 *  CLineRel
 * 
 */
class CLineRel : public CLineAbs
	{
public:
	// Constructors and destructor

	/**
	 * Destructor.
	 */
	~CLineRel();

	/**
	 * Two-phased constructor.
	 */
	static CLineRel* NewL(CStringTokenizer& st);

	/**
	 * Two-phased constructor.
	 */
	static CLineRel* NewLC(CStringTokenizer& st);
	
	TBool IsValidL(RDevPatcherClient& client, TBool& aWarning);

protected:
	CLineRel(TLineType aType);
	void BaseConstructL(CStringTokenizer& st);

private:

	/**
	 * Constructor for performing 1st stage construction
	 */
	CLineRel();

	/**
	 * EPOC default constructor for performing 2nd stage construction
	 */
	void ConstructL(CStringTokenizer& st);

protected:
	TUint32	 iOffset;
	TLinAddr iFileAddr;
	TUint32  iFileLen;
	};

#endif // LINEREL_H
