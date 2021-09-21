/*
 ============================================================================
 Name		: LineReturn.h
 Author	  : Marco Bellino
 Version	 : 1.0
 Copyright   : Guardian-Mobile.com
 Description : CLineReturn declaration
 ============================================================================
 */

#ifndef LINERETURN_H
#define LINERETURN_H
#include "abstractLine.h"
#include <FuzzyByte\StringUtils.h>


// INCLUDES
#include <e32std.h>
#include <e32base.h>

// CLASS DECLARATION

/**
 *  CLineReturn
 * 
 */
class CLineReturn : public CAbstractLine
	{
public:
	// Constructors and destructor

	/**
	 * Destructor.
	 */
	~CLineReturn();

	/**
	 * Two-phased constructor.
	 */
	static CLineReturn* NewL();

	/**
	 * Two-phased constructor.
	 */
	static CLineReturn* NewLC();

private:

	/**
	 * Constructor for performing 1st stage construction
	 */
	CLineReturn();

	/**
	 * EPOC default constructor for performing 2nd stage construction
	 */
	void ConstructL();

	};

#endif // LINERETURN_H
