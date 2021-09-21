/*
 ============================================================================
 Name		: LineSuper.h
 Author	  : Marco Bellino
 Version	 : 1.0
 Copyright   : Guardian-Mobile.com
 Description : CLineSuper declaration
 ============================================================================
 */

#ifndef LINESUPER_H
#define LINESUPER_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>
#include "lineabs.h"
#include <FuzzyByte\StringUtils.h>

// CLASS DECLARATION

/**
 *  CLineSuper
 * 
 */
class CLineSuper : public CLineAbs
	{
public:
	// Constructors and destructor

	/**
	 * Destructor.
	 */
	~CLineSuper();

	/**
	 * Two-phased constructor.
	 */
	static CLineSuper* NewL(TLinAddr aSuperPageAddr, CStringTokenizer& st);

	/**
	 * Two-phased constructor.
	 */
	static CLineSuper* NewLC(TLinAddr aSuperPageAddr, CStringTokenizer& st);

private:

	/**
	 * Constructor for performing 1st stage construction
	 */
	CLineSuper(TLinAddr aSuperPageAddr);

	/**
	 * EPOC default constructor for performing 2nd stage construction
	 */
	void ConstructL(CStringTokenizer& st);
	};

#endif // LINESUPER_H
