/*
========================================================================
 Name        : RomPatcherPlusListBoxView.h
 Author      : Marco Bellino
 Copyright   : Guardian-Mobile.com
 Description : 
========================================================================
*/
#ifndef ROMPATCHERPLUSLISTBOXVIEW_H
#define ROMPATCHERPLUSLISTBOXVIEW_H

// [[[ begin generated region: do not modify [Generated Includes]
#include <aknview.h>
// ]]] end generated region [Generated Includes]


// [[[ begin [Event Handler Includes]
// ]]] end [Event Handler Includes]

// [[[ begin generated region: do not modify [Generated Constants]
// ]]] end generated region [Generated Constants]

// [[[ begin generated region: do not modify [Generated Forward Declarations]
class CRomPatcherPlusListBox;
// ]]] end generated region [Generated Forward Declarations]

/**
 * Avkon view class for RomPatcherPlusListBoxView. It is register with the view server
 * by the AppUi. It owns the container control.
 * @class	CRomPatcherPlusListBoxView RomPatcherPlusListBoxView.h
 */						
			

#include <FuzzyByte\LongRunTask.h>
#include <FuzzyByte\ProgressBar.h>
#include <FuzzyByte\XmlParser.h>
#include <FuzzyByte\SendAsSms.h>
#include <FuzzyByte\HttpSession.h>
#include <FuzzyByte\HttpEngineSync.h>
#include "FortumoXml.h"


class CRomPatcherPlusListBoxView : public CAknView, public MLongTaskCallBack, public MProgressBarObserver, public MXmlNotifier, public MSendAsSmsNotifier
	{
	
	// [[[ begin [Public Section]
public:
	// constructors and destructor
	CRomPatcherPlusListBoxView();
	static CRomPatcherPlusListBoxView* NewL();
	static CRomPatcherPlusListBoxView* NewLC();
	void ConstructL();
	virtual ~CRomPatcherPlusListBoxView();
						
	// from base class CAknView
	TUid Id() const;
	void HandleCommandL( TInt aCommand );
	
	// [[[ begin generated region: do not modify [Generated Methods]
	CRomPatcherPlusListBox* CreateContainerL();
	// ]]] end generated region [Generated Methods]
	
	// ]]] end [Public Section]
	virtual void DynInitMenuPaneL(TInt aResourceId,CEikMenuPane* aMenuPane);
    virtual void RequestCancelled( );
	virtual void DoOneRoundL();
	
	// [[[ begin [Protected Section]
protected:
	// from base class CAknView
	void DoActivateL(
		const TVwsViewId& aPrevViewId,
		TUid aCustomMessageId,
		const TDesC8& aCustomMessage );
	void DoDeactivate();
	void HandleStatusPaneSizeChange();
	
	// [[[ begin generated region: do not modify [Overridden Methods]
	// ]]] end generated region [Overridden Methods]
	
	
	// [[[ begin [User Handlers]
	TBool HandleApplyMenuItemSelectedL( TInt aCommand );
	TBool HandleAdd_to_AutoMenuItemSelectedL( TInt aCommand );
	TBool HandleRemove_PatchMenuItemSelectedL( TInt aCommand );
	TBool HandleRemove_from_AutoMenuItemSelectedL( TInt aCommand );
	TBool HandleDump_ROMMenuItemSelectedL( TInt aCommand );
	TBool HandleDump_SuperPageMenuItemSelectedL( TInt aCommand );
	TBool HandleSystem_InfoMenuItemSelectedL( TInt aCommand );
	TBool HandleAbout_MenuItem1SelectedL( TInt aCommand );
	TBool HandlePatch_InfoMenuItem1SelectedL( TInt aCommand );
	TBool HandleApply_All_PatchesMenuItemSelectedL( TInt aCommand );
	TBool HandleRemove_All_PatchesMenuItemSelectedL( TInt aCommand );
	TBool HandlePatch_InfoMenuItemSelectedL( TInt aCommand );
	TBool HandleAdvMenuItemSelectedL( TInt aCommand );
	TBool HandlePatch_AdvMenuItemSelectedL( TInt aCommand );
	TBool HandleApplyMenuItem1SelectedL( TInt aCommand );
	TBool HandleDisableMenuItemSelectedL( TInt aCommand );
	TBool HandleDonateMenuItemSelectedL( TInt aCommand );
	TBool HandleAdd_to_Auto_DomainSrv_MenuItemSelectedL( TInt aCommand );
	// ]]] end [User Handlers]
	
	// ]]] end [Protected Section]
	
    virtual void ParsingCompleteL(TAny* src);
    virtual void MessageSentL(TInt aError);
    TInt ShowCountryDialogL();
    TBool ShowConfirmDialogL(TInt i);
    
	// [[[ begin [Private Section]
private:
	void SetupStatusPaneL();
	void CleanupStatusPane();
	void DonateL(TInt32 type);

	
	// [[[ begin generated region: do not modify [Generated Instance Variables]
	CRomPatcherPlusListBox* iRomPatcherPlusListBox;
	// ]]] end generated region [Generated Instance Variables]
	
	CProgressBar* iProgressBar;
	CXmlParser* iXmlParser;
	CFortumoXml* iFortumoXml;
	CSendAsSms* iSendAs;
	TBool iCancelDownload;
	CHttpSession* iHttpSession;
	CHttpEngineSync* iHttpEngine;
	RSocketServ iSocketServ;
	
	TBool iExistsDomainSrv;
	CLongTaskAO* iLongTask;
	RBuf8 iDumpBuf;
	RFile iDumpFile;
	// [[[ begin generated region: do not modify [Generated Methods]
	// ]]] end generated region [Generated Methods]
	
	// ]]] end [Private Section]
	
	};

#endif // ROMPATCHERPLUSLISTBOXVIEW_H
