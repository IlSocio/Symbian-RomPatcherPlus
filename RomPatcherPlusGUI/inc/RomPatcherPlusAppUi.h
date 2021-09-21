/*
========================================================================
 Name        : RomPatcherPlusAppUi.h
 Author      : Marco Bellino
 Copyright   : Guardian-Mobile.com
 Description : 
========================================================================
*/
#ifndef ROMPATCHERPLUSAPPUI_H
#define ROMPATCHERPLUSAPPUI_H

// [[[ begin generated region: do not modify [Generated Includes]
#include <aknviewappui.h>
// ]]] end generated region [Generated Includes]

// [[[ begin generated region: do not modify [Generated Forward Declarations]
class CRomPatcherPlusListBoxView;
// ]]] end generated region [Generated Forward Declarations]


#include <FuzzyByte\RP\RP_Types.h>


/**
 * @class	CRomPatcherPlusAppUi RomPatcherPlusAppUi.h
 * @brief The AppUi class handles application-wide aspects of the user interface, including
 *        view management and the default menu, control pane, and status pane.
 */
class CRomPatcherPlusAppUi : public CAknViewAppUi
	{
public: 
	// constructor and destructor
	CRomPatcherPlusAppUi();
	virtual ~CRomPatcherPlusAppUi();
	void ConstructL();
	
//	IMPORT_C static void GUI_GetPatchesStatusL(RFs& aFs, RPointerArray<HBufC>& aPatchList, RArray<TPatchStatus>& aPatchStatus, RArray<TAutoStatus>& aAutoStatus);

public:
	// from CCoeAppUi
	TKeyResponse HandleKeyEventL(
				const TKeyEvent& aKeyEvent,
				TEventCode aType );

	// from CEikAppUi
	void HandleCommandL( TInt aCommand );
	void HandleResourceChangeL( TInt aType );

	// from CAknAppUi
	void HandleViewDeactivation( 
			const TVwsViewId& aViewIdToBeDeactivated, 
			const TVwsViewId& aNewlyActivatedViewId );

private:
	void InitializeContainersL();
	// [[[ begin generated region: do not modify [Generated Methods]
public: 
	// ]]] end generated region [Generated Methods]
	
	// [[[ begin generated region: do not modify [Generated Instance Variables]
private: 
	CRomPatcherPlusListBoxView* iRomPatcherPlusListBoxView;
	// ]]] end generated region [Generated Instance Variables]
	
	
	// [[[ begin [User Handlers]
protected: 
	// ]]] end [User Handlers]
	
	};

#endif // ROMPATCHERPLUSAPPUI_H			
