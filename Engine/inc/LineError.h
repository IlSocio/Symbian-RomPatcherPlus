/*
 ============================================================================
 Name		: LineError.h
 Author	  : Marco Bellino
 Version	 : 1.0
 Copyright   : Guardian-Mobile.com
 Description : CLineError declaration
 ============================================================================
 */

#ifndef LINEERROR_H
#define LINEERROR_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>
#include "abstractLine.h"
#include <FuzzyByte\StringUtils.h>

// CLASS DECLARATION

/**
 *  CLineError
 * 
 */
class CLineError : public CAbstractLine
	{
public:
	// Constructors and destructor

	/**
	 * Destructor.
	 */
	~CLineError();

	/**
	 * Two-phased constructor.
	 */
	static CLineError* NewL(CStringTokenizer& st);

	/**
	 * Two-phased constructor.
	 */
	static CLineError* NewLC(CStringTokenizer& st);
	TBool ApplyL(RDevPatcherClient& client, TBool& aWarning);

private:

	/**
	 * Constructor for performing 1st stage construction
	 */
	CLineError();

	/**
	 * EPOC default constructor for performing 2nd stage construction
	 */
	void ConstructL(CStringTokenizer& st);

private:
	RBuf iMsg;
	};

#endif // LINEERROR_H
