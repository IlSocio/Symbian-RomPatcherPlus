/*
========================================================================
 Name        : RomPatcherPlusApplication.h
 Author      : Marco Bellino
 Copyright   : Guardian-Mobile.com
 Description : 
========================================================================
*/
#ifndef ROMPATCHERPLUSAPPLICATION_H
#define ROMPATCHERPLUSAPPLICATION_H

// [[[ begin generated region: do not modify [Generated Includes]
#include <aknapp.h>
// ]]] end generated region [Generated Includes]

// [[[ begin generated region: do not modify [Generated Constants]
const TUid KUidRomPatcherPlusApplication = { 0xE0050C10 };
// ]]] end generated region [Generated Constants]

/**
 *
 * @class	CRomPatcherPlusApplication RomPatcherPlusApplication.h
 * @brief	A CAknApplication-derived class is required by the S60 application 
 *          framework. It is subclassed to create the application's document 
 *          object.
 */
class CRomPatcherPlusApplication : public CAknApplication
	{
private:
	TUid AppDllUid() const;
	CApaDocument* CreateDocumentL();
	
	};
			
#endif // ROMPATCHERPLUSAPPLICATION_H		
