/*
 ============================================================================
 Name		: LineComment.h
 Author	  : Marco Bellino
 Version	 : 1.0
 Copyright   : Guardian-Mobile.com
 Description : CLineComment declaration
 ============================================================================
 */

#ifndef LINECOMMENT_H
#define LINECOMMENT_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>
#include "abstractline.h"

// CLASS DECLARATION

/**
 *  CLineComment
 * 
 */
class CLineComment : public CAbstractLine
	{
public:
	// Constructors and destructor

	/**
	 * Destructor.
	 */
	~CLineComment();

	/**
	 * Two-phased constructor.
	 */
	static CLineComment* NewL();

	/**
	 * Two-phased constructor.
	 */
	static CLineComment* NewLC();
	HBufC8* GetUnpatchDataL();

private:

	/**
	 * Constructor for performing 1st stage construction
	 */
	CLineComment();

	/**
	 * EPOC default constructor for performing 2nd stage construction
	 */
	void ConstructL();

	};

#endif // LINECOMMENT_H
