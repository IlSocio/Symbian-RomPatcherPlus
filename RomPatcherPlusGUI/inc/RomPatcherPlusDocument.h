/*
========================================================================
 Name        : RomPatcherPlusDocument.h
 Author      : Marco Bellino
 Copyright   : Guardian-Mobile.com
 Description : 
========================================================================
*/
#ifndef ROMPATCHERPLUSDOCUMENT_H
#define ROMPATCHERPLUSDOCUMENT_H

#include <akndoc.h>
		
class CEikAppUi;

/**
* @class	CRomPatcherPlusDocument RomPatcherPlusDocument.h
* @brief	A CAknDocument-derived class is required by the S60 application 
*           framework. It is responsible for creating the AppUi object. 
*/
class CRomPatcherPlusDocument : public CAknDocument
	{
public: 
	// constructor
	static CRomPatcherPlusDocument* NewL( CEikApplication& aApp );

private: 
	// constructors
	CRomPatcherPlusDocument( CEikApplication& aApp );
	void ConstructL();
	
public: 
	// from base class CEikDocument
	CEikAppUi* CreateAppUiL();
	};
#endif // ROMPATCHERPLUSDOCUMENT_H
