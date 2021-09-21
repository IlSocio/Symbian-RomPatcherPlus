/*
 ============================================================================
 Name		: RomPatcherInstall.cpp
 Author	  : Marco Bellino
 Copyright   : Guardian-Mobile.com
 Description : Exe source file
 ============================================================================
 */

//  Include Files  

#include "RomPatcherInstall.h"
#include <e32base.h>
#include <e32std.h>
#include <e32cons.h>			// Console
#include <bautils.h>
#include <FuzzyByte\Utils.h>
#include <FuzzyByte\FileUtils.h>
#include <FuzzyByte\Notes.h>


LOCAL_C void DoStartL()
	{
	// Create active scheduler (to run active objects)
	CActiveScheduler* scheduler = new (ELeave) CActiveScheduler();
	CleanupStack::PushL(scheduler);
	CActiveScheduler::Install(scheduler);
	
/*	RFs fs;
	User::LeaveIfError( fs.Connect() );
	CleanupClosePushL(fs); 

	TFileName patchDir;
	if (!GetPatchesDriveAndPath(fs, patchDir))
		{
		patchDir = _L("c:\\patches\\");
		fs.MkDirAll(patchDir);
		}
	
	_LIT(KZipArchive, "c:\\TMP_Patches.zip");
	Utils::ExtractZipL(fs, KZipArchive, patchDir);
	BaflUtils::DeleteFile(fs, KZipArchive);
	
	CleanupStack::PopAndDestroy(&fs);*/
	  
	// Delete active scheduler
	CleanupStack::PopAndDestroy(scheduler);
	}  

 
//  Global Functions

GLDEF_C TInt E32Main()
	{
	// Create cleanup stack
	__UHEAP_MARK;
	CTrapCleanup* cleanup = CTrapCleanup::New();

	// Run application code inside TRAP harness, wait keypress when terminated
	TRAPD(mainError, DoStartL());

	delete cleanup;
	__UHEAP_MARKEND;
	return KErrNone;
	}
