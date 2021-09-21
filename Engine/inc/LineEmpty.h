/*
 ============================================================================
 Name		: LineEmpty.h
 Author	  : Marco Bellino
 Version	 : 1.0
 Copyright   : Guardian-Mobile.com
 Description : CLineEmpty declaration
 ============================================================================
 */

#ifndef LINEEMPTY_H
#define LINEEMPTY_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>
#include "abstractline.h"

// CLASS DECLARATION

/**
 *  CLineEmpty
 * 
 */
class CLineEmpty : public CAbstractLine
	{
public:
	// Constructors and destructor

	/**
	 * Destructor.
	 */
	~CLineEmpty();

	/**
	 * Two-phased constructor.
	 */
	static CLineEmpty* NewL();

	/**
	 * Two-phased constructor.
	 */
	static CLineEmpty* NewLC();

private:

	/**
	 * Constructor for performing 1st stage construction
	 */
	CLineEmpty();

	/**
	 * EPOC default constructor for performing 2nd stage construction
	 */
	void ConstructL();

	};

#endif // LINEEMPTY_H
