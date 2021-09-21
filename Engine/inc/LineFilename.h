/*
 ============================================================================
 Name		: LineFilename.h
 Author	  : Marco Bellino
 Version	 : 1.0
 Copyright   : Your copyright notice
 Description : CLineFilename declaration
 ============================================================================
 */

#ifndef LINEFILENAME_H
#define LINEFILENAME_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>
#include "abstractline.h"
#include "lineabs.h"

// CLASS DECLARATION

/**
 *  CLineFilename
 * 
 */
class CLineFilename : public CLineAbs
	{
public:
	// Constructors and destructor

	/**
	 * Destructor.
	 */
	~CLineFilename();

protected:
	virtual void BaseConstructL(CStringTokenizer& st, TBool hasOffet);

	/**
	 * Constructor for performing 1st stage construction
	 */
	CLineFilename(TLineType aType);

	TBool FindAddressEntryL(const TDesC& aName, TLinAddr& aAddr, TUint32& aLength);

private:

	/**
	 * EPOC default constructor for performing 2nd stage construction
	 */
	void ConstructL();

	
protected:
	TLinAddr iFileAddr;
	TLinAddr iFileLen;
	};

#endif // LINEFILENAME_H
