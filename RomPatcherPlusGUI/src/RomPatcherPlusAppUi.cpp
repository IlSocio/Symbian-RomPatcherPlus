
//
// Se manca il file rel: RP+ segnala warning
// Se manca il file snr: RP+ segnala warning
//   
//  New: #ifdef... 
//  New: info: return: error: 
//	New: Added support for the Warning/Yellow Icon
//  Improved: When applying a patch the error messages and the yellow icon will be shown ONLY if exists the path c:\resource\ErrRd (ErrRd can be a file or a directory). Disabled by default, because these features are useful to the patch-makers only, but could confuse the end-users. To enable them, just create the file or folder c:\resource\ErrRd
//  Improved: The same Open4All.rmp file can now be used to patch all the devices (including N96)
//  Fixed: Restored the "LDD Not Found" error message in case that the LDD file is missing
//  Crea unica patch per Open4All e per InstallServer
//  Crea patch che visualizza testo in Italiano/Inglese/Russo
//   

//  Fixed: The message "Application not compatible with the phone" is not shown anymore.
//  Fixed: Replaced the Open4All_N96 templove's version with wadowice's version.
//  Fixed: Sometimes the green\red patch icon wasn't showed properly
//  New: Popup when launching RP+ to remember to everyone the 6th September deadline.
//  New: ApplyAll/RemoveAll options to quicly Apply/Remove all the patches.
//  Improved: Touch Support, the tap will select the patch, a tap on a selected patch will apply/remove it
//  Improved: Patches can be placed in any drive, RP+ will follow the order Z: -> A: and will use the first drive which contain the \patches\ folder
//  Improved: Error messages will report the patch's name also.
//  Removed: Apply/Remove options have been removed because redundant. Just click on a patch to apply/remove it.
//  Removed: The annoying popups for apply patch have gone :) yeppi!!! 
//  Removed: The advanced menu' has been temporary removed, it will came back again with more options available like "Dump/View any user choosen area of memory"...
//  New: Donate options menu...
		
	
	

 

/*
========================================================================
 Name        : RomPatcherPlusAppUi.cpp
 Author      : Marco Bellino
 Copyright   : Guardian-Mobile.com
 Description : 
========================================================================
*/ 
// [[[ begin generated region: do not modify [Generated System Includes]
#include <eikmenub.h>
#include <akncontext.h>
#include <akntitle.h>
#include <RomPatcherPlusGUI.rsg>
// ]]] end generated region [Generated System Includes]
  
// [[[ begin generated region: do not modify [Generated User Includes]
#include "RomPatcherPlusAppUi.h"
#include "RomPatcherPlusGUI.hrh"
#include "RomPatcherPlusListBox.hrh"
#include "RomPatcherPlusListBoxView.h"
// ]]] end generated region [Generated User Includes]
      
// [[[ begin generated region: do not modify [Generated Constants]
// ]]] end generated region [Generated Constants]
     
 
#include <APGCLI.H>
#include <patcherc.h>
#include <FuzzyByte\notes.h> 
#include <FuzzyByte\GuiUtils.h>
#include <FuzzyByte\FileUtils.h>
#include <FuzzyByte\Utils.h>
#include <baUtils.h>
#include <f32file.h>
#include <FuzzyByte\RP\RomPatcherEngine.h>
#include <mmf\common\mmfcontrollerpluginresolver.h>

    
/*EXPORT_C void CRomPatcherPlusAppUi::GUI_GetPatchesStatusL(RFs& aFs, RPointerArray<HBufC>& aPatchList, RArray<TPatchStatus>& aPatchStatus, RArray<TAutoStatus>& aAutoStatus)
	{
	RomPatcherEngine::GetPatchesStatusL(aFs, aPatchList, aPatchStatus, aAutoStatus);
	} */

/** 
 * Construct the CRomPatcherPlusAppUi instance
 */ 
CRomPatcherPlusAppUi::CRomPatcherPlusAppUi() 
	{
	// [[[ begin generated region: do not modify [Generated Contents]
	// ]]] end generated region [Generated Contents]
	
	}

/** 
 * The appui's destructor removes the container from the control
 * stack and destroys it.
 */
CRomPatcherPlusAppUi::~CRomPatcherPlusAppUi()
	{
	// [[[ begin generated region: do not modify [Generated Contents]
	// ]]] end generated region [Generated Contents]
	
	}

// [[[ begin generated function: do not modify 
void CRomPatcherPlusAppUi::InitializeContainersL()
	{
	iRomPatcherPlusListBoxView = CRomPatcherPlusListBoxView::NewL();
	AddViewL( iRomPatcherPlusListBoxView );
	SetDefaultViewL( *iRomPatcherPlusListBoxView );
	}  
// ]]] end generated function 

/**
 * Handle a command for this appui (override)
 * @param aCommand command id to be handled
 */
void CRomPatcherPlusAppUi::HandleCommandL( TInt aCommand )
	{
	// [[[ begin generated region: do not modify [Generated Code]
	TBool commandHandled = EFalse;
	switch ( aCommand )
		{ // code to dispatch to the AppUi's menu and CBA commands is generated here
		default:
			break;
		}
	
		
	if ( !commandHandled ) 
		{
		if ( aCommand == EAknSoftkeyExit || aCommand == EEikCmdExit )
			{
			Exit();
			}
		}
	// ]]] end generated region [Generated Code]
	
	}

/** 
 * Override of the HandleResourceChangeL virtual function
 */
void CRomPatcherPlusAppUi::HandleResourceChangeL( TInt aType )
	{
	CAknViewAppUi::HandleResourceChangeL( aType );
	// [[[ begin generated region: do not modify [Generated Code]
	// ]]] end generated region [Generated Code]
	
	}
				
/** 
 * Override of the HandleKeyEventL virtual function
 * @return EKeyWasConsumed if event was handled, EKeyWasNotConsumed if not
 * @param aKeyEvent 
 * @param aType 
 */
TKeyResponse CRomPatcherPlusAppUi::HandleKeyEventL(
		const TKeyEvent& aKeyEvent,
		TEventCode aType )
	{
	// The inherited HandleKeyEventL is private and cannot be called
	// [[[ begin generated region: do not modify [Generated Contents]
	// ]]] end generated region [Generated Contents]
	
	return EKeyWasNotConsumed;
	}

/** 
 * Override of the HandleViewDeactivation virtual function
 *
 * @param aViewIdToBeDeactivated 
 * @param aNewlyActivatedViewId 
 */
void CRomPatcherPlusAppUi::HandleViewDeactivation( 
		const TVwsViewId& aViewIdToBeDeactivated, 
		const TVwsViewId& aNewlyActivatedViewId )
	{
	CAknViewAppUi::HandleViewDeactivation( 
			aViewIdToBeDeactivated, 
			aNewlyActivatedViewId );
	// [[[ begin generated region: do not modify [Generated Contents]
	// ]]] end generated region [Generated Contents]
	
	}  
     
  

//#include "advstringloader.h"
/**
 * @brief Completes the second phase of Symbian object construction. 
 * Put initialization code that could leave here. 
 */ 
void CRomPatcherPlusAppUi::ConstructL()
	{
	// [[[ begin generated region: do not modify [Generated Contents]
	
	TUint32 EAknTouchCompatibleFlag = 0x00800000;
	TUint32 flag = 0;
	if (GuiUtils::IsTouchSupportedL())
		flag = EAknTouchCompatibleFlag;
	
	BaseConstructL( EAknEnableSkin | flag );
	InitializeContainersL();
	// ]]] end generated region [Generated Contents] 
	}

