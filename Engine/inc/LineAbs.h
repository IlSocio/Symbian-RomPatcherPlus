/*
 ============================================================================
 Name		: LineAbs.h
 Author	  : Marco Bellino
 Version	 : 1.0
 Copyright   : Guardian-Mobile.com
 Description : CLineAbs declaration
 ============================================================================
 */

#ifndef LINEABS_H
#define LINEABS_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>
#include "abstractline.h"
#include <FuzzyByte\StringUtils.h>

// CLASS DECLARATION

/**
 *  CLineAbs
 * 
 */
class CLineAbs : public CAbstractLine
	{
public:
	// Constructors and destructor

	/**
	 * Destructor.
	 */
	~CLineAbs();

	/**
	 * Two-phased constructor.
	 */
	static CLineAbs* NewL(CStringTokenizer& st);

	/**
	 * Two-phased constructor.
	 */
	static CLineAbs* NewLC(CStringTokenizer& st);

	TBool ApplyL(RDevPatcherClient& client, TBool& aWarning);
	TBool RemoveL(RDevPatcherClient& client);
	HBufC8* GetUnpatchDataL();

protected:
	virtual TBool IsValidL(RDevPatcherClient& client, TBool& aWarning);
	/**
	 * Constructor for performing 1st stage construction
	 */
	CLineAbs(TLinAddr addr);
	CLineAbs(TLinAddr addr, TLineType aType);
	virtual void BaseConstructL(CStringTokenizer& st);
	
	TBool Ordinal2Addr(RDevPatcherClient& client, const TLinAddr& fileAddr, TInt ordinal, TLinAddr& addr);
	HBufC8* ReadBufferL(RDevPatcherClient& client, TLinAddr& addr, TInt len);
	
	TInt FindOriBytesPatternL(const TDes8& buf, TDes8& oriBytes, RArray<TInt>& oriWildPos, TDes8& newBytes, RArray<TInt>& newWildPos);
	HBufC8* Ascii2BytesL(const TDesC& asciiBuf, RArray<TInt>& wildPos);
	TBool FindAddressEntryL(const TDesC& aName, TLinAddr& aAddr, TUint32& aLength);

private:
	TInt FindWithWildcardsL(const TDes8& buf, const TDesC8& toFind, RArray<TInt>& wildPos);

	/**
	 * EPOC default constructor for performing 2nd stage construction
	 */
	void ConstructL(CStringTokenizer& st);

	
protected:
	TLinAddr iAddr;				
								// 20 ?? ?? 10 30 40
	RBuf8 iOriBytes;			// 20 00 00 10 30 40
	RBuf8 iNewBytes;
	RArray<TInt> iWildOriPos;
	RArray<TInt> iWildNewPos;
	};

#endif // LINEABS_H
