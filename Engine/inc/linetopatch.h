/*
 ============================================================================
 Name		: LineToPatch.h
 Author	  : Marco Bellino
 Version	 : 1.0
 Copyright   : Guardian-Mobile.com
 Description : CLineToPatch declaration
 ============================================================================
 */

#ifndef LINETOPATCH_H
#define LINETOPATCH_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>

// CLASS DECLARATION

/**
 *  CLineToPatch
 * 
 */
class CLineToPatch : public CBase
	{
public:
	// Constructors and destructor

	/**
	 * Destructor.
	 */
	~CLineToPatch();

	/**
	 * Two-phased constructor.
	 */
	static CLineToPatch* NewL(TLinAddr addr, const TDesC8& oriBytes, const TDesC8& newBytes);

	/**
	 * Two-phased constructor.
	 */
	static CLineToPatch* NewLC(TLinAddr addr, const TDesC8& oriBytes, const TDesC8& newBytes);

	void SetSearchOption(TUint32 searchLen);
	
	TBool Search();
private:

	/**
	 * Constructor for performing 1st stage construction
	 */
	CLineToPatch(TLinAddr addr);

	/**
	 * EPOC default constructor for performing 2nd stage construction
	 */
	void ConstructL(const TDesC8& oriBytes, const TDesC8& newBytes);

public:
	TUint32 iSearchLen;
	TLinAddr iAddr;
	RBuf8 iOriBytes;
	RBuf8 iNewBytes;
	RBuf8 iWildCardPos;
	};

#endif // LINETOPATCH_H
