/*
 ============================================================================
 Name		: LineInfo.h
 Author	  : Marco Bellino
 Version	 : 1.0
 Copyright   : Guardian-Mobile.com
 Description : CLineInfo declaration
 ============================================================================
 */

#ifndef LINEINFO_H
#define LINEINFO_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>
#include "abstractLine.h"
#include <FuzzyByte\StringUtils.h>

// CLASS DECLARATION

/**
 *  CLineInfo
 * 
 */
class CLineInfo : public CAbstractLine
	{
public:
	// Constructors and destructor

	/**
	 * Destructor.
	 */
	~CLineInfo();

	/**
	 * Two-phased constructor.
	 */
	static CLineInfo* NewL(CStringTokenizer& st);

	/**
	 * Two-phased constructor.
	 */
	static CLineInfo* NewLC(CStringTokenizer& st);
	TBool ApplyL(RDevPatcherClient& client, TBool& aWarning);

private:

	/**
	 * Constructor for performing 1st stage construction
	 */
	CLineInfo();

	/**
	 * EPOC default constructor for performing 2nd stage construction
	 */
	void ConstructL(CStringTokenizer& st);

private:
	RBuf iMsg;
	};

#endif // LINEINFO_H
