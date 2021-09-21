/*
 ============================================================================
 Name		: AbstractLine.h
 Author	  : Marco Bellino
 Version	 : 1.0
 Copyright   : Guardian-Mobile.com
 Description : CAbstractLine declaration
 ============================================================================
 */

#ifndef ABSTRACTLINE_H
#define ABSTRACTLINE_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>
#include <FuzzyByte\RP\patcherc.h>

// CLASS DECLARATION

enum TLineType
	{
	ESuper,
	EAbs,
	EEmpty,
	EComment,
	EOrdRel,
	EOrdSnr,
	ESnr,
	ERel,
	EInfo,
	EReturn,
	EError
	};
/**
 *  CAbstractLine
 * 
 */
class CAbstractLine : public CBase
	{
public:
	// Constructors and destructor

	/**
	 * Destructor.
	 */
	~CAbstractLine();

	virtual TBool ApplyL(RDevPatcherClient& client, TBool& aWarning);
	virtual TBool RemoveL(RDevPatcherClient& client);
	virtual HBufC8* GetUnpatchDataL();

	TLineType Type();
	
public:
	TBool iMemChanged;
	
protected:
	CAbstractLine(TLineType aType);
	
private:
	TLineType iType;
	};

#endif // ABSTRACTLINE_H
