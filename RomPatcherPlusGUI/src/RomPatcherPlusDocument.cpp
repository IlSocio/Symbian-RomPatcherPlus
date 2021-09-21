/*
========================================================================
 Name        : RomPatcherPlusDocument.cpp
 Author      : Marco Bellino
 Copyright   : Guardian-Mobile.com
 Description : 
========================================================================
*/
// [[[ begin generated region: do not modify [Generated User Includes]
#include "RomPatcherPlusDocument.h"
#include "RomPatcherPlusAppUi.h"
// ]]] end generated region [Generated User Includes]

/**
 * @brief Constructs the document class for the application.
 * @param anApplication the application instance
 */
CRomPatcherPlusDocument::CRomPatcherPlusDocument( CEikApplication& anApplication )
	: CAknDocument( anApplication )
	{  
	}    
  
/** 
 * @brief Completes the second phase of Symbian object construction. 
 * Put initialization code that could leave here.   
 */  
void CRomPatcherPlusDocument::ConstructL()
	{
	}
	
/**
 * Symbian OS two-phase constructor.
 *
 * Creates an instance of CRomPatcherPlusDocument, constructs it, and
 * returns it.
 *
 * @param aApp the application instance
 * @return the new CRomPatcherPlusDocument
 */
CRomPatcherPlusDocument* CRomPatcherPlusDocument::NewL( CEikApplication& aApp )
	{
	CRomPatcherPlusDocument* self = new ( ELeave ) CRomPatcherPlusDocument( aApp );
	CleanupStack::PushL( self );
	self->ConstructL();
	CleanupStack::Pop( self );
	return self;
	}

/**
 * @brief Creates the application UI object for this document.
 * @return the new instance
 */	
CEikAppUi* CRomPatcherPlusDocument::CreateAppUiL()
	{
	return new ( ELeave ) CRomPatcherPlusAppUi();
	}
				
