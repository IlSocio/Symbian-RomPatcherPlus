
//#define _BETA_VERSION_

/*
========================================================================
 Name        : RomPatcherPlusListBoxView.cpp
 Author      : Marco Bellino
 Copyright   : Guardian-Mobile.com
 Description : 
========================================================================
*/
// [[[ begin generated region: do not modify [Generated System Includes]
#include <aknviewappui.h>
#include <eikmenub.h>
#include <avkon.hrh>
#include <barsread.h>
#include <stringloader.h>
#include <aknlists.h>
#include <eikenv.h>
#include <akniconarray.h>
#include <eikclbd.h>
#include <akncontext.h>
#include <akntitle.h>
#include <eikbtgpc.h>
#include <RomPatcherPlusGUI.rsg>
// ]]] end generated region [Generated System Includes]

// [[[ begin generated region: do not modify [Generated User Includes]

#include "RomPatcherPlusGUI.hrh"
#include "RomPatcherPlusListBoxView.h"
#include "RomPatcherPlusListBox.hrh"
#include "RomPatcherPlusListBox.h"
// ]]] end generated region [Generated User Includes]

// [[[ begin generated region: do not modify [Generated Constants]
// ]]] end generated region [Generated Constants]

#include <FuzzyByte\RP\RomPatcherEngine.h>
#include <FuzzyByte\FileUtils.h>
#include <FuzzyByte\Utils.h>
#include <FuzzyByte\Notes.h>
#include <FuzzyByte\RStringBuf.h>
#include <FuzzyByte\GuiUtils.h>
#include <FuzzyByte\ProgressBar.h>
#include <FuzzyByte\AsyncWaiter.h>
#include <f32fsys.h>
#include <patcherc.h>
#include <hal.h>
#include <eikprogi.h>                   // CEikProgressInfo 
#include <AKNMESSAGEQUERYDIALOG.H>
#include <browserlauncher.h>
#include <browseroverriddensettings.h>
#include <xml\parser.h>
#include <xml\contenthandler.h>
#include <xml\XmlFrameworkConstants.h>
#include <FuzzyByte\XmlParser.h>
#include <FuzzyByte\AES.h>
#include <hal.H>
#include <hal_data.H>
//#include "FortumoXML.h"
#include <APGCLI.H>

_LIT8(KDownloadLink, "http://www.Symbian-Toys.com/Download.Aspx?File=Data.Bin");
/*_LIT(KLDDNameS3, "patcherS3.ldd");
_LIT(KLDDNameMemCopy, "patcher.ldd");
_LIT(KLDDNameShadow, "patcherShadow.ldd");*/
_LIT(KXMLFile, "d:\\RPPlusData\\Config.bin");

#define DISABLE_ADV

/*
LOCAL_C TInt GetVolumeName(RFs& fs, TDes& volName)
	{
	TVolumeInfo volInfo;
	TInt res = fs.Volume(volInfo, EDriveZ);
	volName = volInfo.iName;
	return res;
	}*/


/*LOCAL_C void FreeDevice()
	{
	User::FreeLogicalDevice(KDevName);	
	}

LOCAL_C TInt LoadDevice()
	{
#ifdef __WINSCW__
	return KErrNone;
#endif
	FreeDevice();
	TInt res = User::LoadLogicalDevice(KLDDNameShadow);
	if (res == KErrNotSupported)
		{
		FreeDevice();
		res = User::LoadLogicalDevice(KLDDNameMemCopy);
		}
	return res;
	}*/



CRomPatcherPlusListBoxView::CRomPatcherPlusListBoxView()
	{
	// [[[ begin generated region: do not modify [Generated Contents]
	iRomPatcherPlusListBox = NULL;
	// ]]] end generated region [Generated Contents]
	}

/** 
 * The view's destructor removes the container from the control
 * stack and destroys it.
 */
CRomPatcherPlusListBoxView::~CRomPatcherPlusListBoxView()
	{
	// [[[ begin generated region: do not modify [Generated Contents]
	delete iRomPatcherPlusListBox;
	iRomPatcherPlusListBox = NULL;
	// ]]] end generated region [Generated Contents]
	
	delete iSendAs;
	delete iLongTask;
	delete iProgressBar;
	iDumpBuf.Close();
	iDumpFile.Close();
	delete iXmlParser;
	delete iFortumoXml;
	
	delete iHttpEngine;
	//
	delete iHttpSession;
	iSocketServ.Close();
	}

/**
 * Symbian two-phase constructor.
 * This creates an instance then calls the second-phase constructor
 * without leaving the instance on the cleanup stack.
 * @return new instance of CRomPatcherPlusListBoxView
 */
CRomPatcherPlusListBoxView* CRomPatcherPlusListBoxView::NewL()
	{
	CRomPatcherPlusListBoxView* self = CRomPatcherPlusListBoxView::NewLC();
	CleanupStack::Pop( self );
	return self;
	}

/**
 * Symbian two-phase constructor.
 * This creates an instance, pushes it on the cleanup stack,
 * then calls the second-phase constructor.
 * @return new instance of CRomPatcherPlusListBoxView
 */
CRomPatcherPlusListBoxView* CRomPatcherPlusListBoxView::NewLC()
	{
	CRomPatcherPlusListBoxView* self = new ( ELeave ) CRomPatcherPlusListBoxView();
	CleanupStack::PushL( self );
	self->ConstructL();
	return self;
	}



LOCAL_C void ShowLongMessageDialogL(const TDesC& title, const TDesC& aMsg)
	{	
	CAknMessageQueryDialog *dialog = CAknMessageQueryDialog::NewL( CONST_CAST(TDesC&, aMsg) );
	dialog->PrepareLC(R_LONGTEXT_DIALOG);
	dialog->SetHeaderTextL(title);
	dialog->RunLD();
	}

#include <sysutil.h>
#include <e32math.h>

_LIT(KText, "If you like this software, then support RP+ with a donation.\nwww.symbian-toys.com/rompatcherplus.aspx");
_LIT(KTitle, "RomPatcher+ Info");


/**
 * Second-phase constructor for view.  
 * Initialize contents from resource.
 */ 
void CRomPatcherPlusListBoxView::ConstructL()
	{
	// [[[ begin generated region: do not modify [Generated Code]
	BaseConstructL( R_ROM_PATCHER_PLUS_LIST_BOX_ROM_PATCHER_PLUS_LIST_BOX_VIEW );
				
	// ]]] end generated region [Generated Code]

	iSocketServ.Connect();
	iHttpSession = CHttpSession::NewL();
	// add your own initialization code here
	iSendAs = CSendAsSms::NewL(*this);
	
	iHttpEngine = CHttpEngineSync::NewL( iHttpSession );
	const TInt BUF_SIZE=20000;
	iDumpBuf.Create( BUF_SIZE );
	iDumpBuf.SetLength( BUF_SIZE );

/*CAknMessageQueryDialog *dialog = CAknMessageQueryDialog::NewL( CONST_CAST(TDesC&, KText()) );	// **** Popup
dialog->PrepareLC(R_LONGTEXT_DIALOG);		// **** Popup
dialog->SetHeaderTextL(KTitle);				// **** Popup
dialog->RunLD();	// **** Popup*/
	
	iFortumoXml = CFortumoXml::NewL(); 
	iXmlParser = CXmlParser::NewL(*this, *iFortumoXml);
	
    iProgressBar = CProgressBar::NewL(this);	
	iLongTask = CLongTaskAO::NewL(*this);		
    
#ifdef _BETA_VERSION_
    TTime aa;
    aa.HomeTime();
    TBool badDay = (aa.DateTime().Day() > 10 && aa.DateTime().Day() < 28);
    if (badDay || aa.DateTime().Year() > 2010)
    	{
    	Notes::Debug_InfoL(_L("This is a BETA VERSION.\nPlease Download RP+ v2.5 Final Version!"));
    	User::Exit(KErrNone);
    	}
#endif
 
    TInt err = RomPatcherEngine::LoadDevice(); 
    if (err != KErrNone)
    	{
    	Notes::WarningL(R_LDD_NOT_FOUND);
    	}
 
/*	// Rileva il nome del file e lo usa per qualcosa...
	TApaAppInfo appInfo;
	RApaLsSession ls;
	User::LeaveIfError(ls.Connect());
	CleanupClosePushL(ls); 
#ifdef __WINSCW__
	ls.GetAppInfo(appInfo, TUid::Uid(0x1020E406));
#else
	ls.GetAppInfo(appInfo, TUid::Uid(0xBAD0B1CF));
#endif
	TParsePtrC parse(appInfo.iFullName);
	
	CleanupStack::PopAndDestroy(&ls);

	iExistsDomainSrv = parse.Name().Length() > 0;*/
    
	
#ifdef __WINSCW__
    TUid KDomainUid3 = TUid::Uid(0x1020E406);	// Original
#else
    TUid KDomainUid3 = TUid::Uid(0xBAD0B1CF);	// Wadowice
#endif
    iExistsDomainSrv = Utils::ExistsExeL(iEikonEnv->FsSession(), _L("DomainSrv.exe"), KDomainUid3);
    		
/*    TUint32 uid3 = Utils::GetUid3FromExeL(iEikonEnv->FsSession(), _L("DomainSrv.exe"));
    iExistsDomainSrv = (uid3 == 0x1020E406);*/
    /*
    _LIT(KDomainImage, "DomainSrv");
    
	const TUidType domainUid(KNullUid, KNullUid, KDomainUid3); 
	RProcess domainProc;			
	
	TInt err1 = domainProc.Create(KDomainImage, KNullDesC, domainUid);
	iExistsDomainSrv = (err1 == KErrNone);
	// TFullName name = domainProc.FullName();
	if (iExistsDomainSrv)
		{
		domainProc.Terminate(KErrNone);
		}
	domainProc.Close();/**/
		
	// [[[ begin generated region: do not modify [Generated Code]
	// ]]] end generated region [Generated Code]	
	}


void CRomPatcherPlusListBoxView::DynInitMenuPaneL(TInt aResourceId,CEikMenuPane* aMenuPane)
	{
#ifdef DISABLE_ADV
	GuiUtils::DeleteMenuItem(aMenuPane, ERomPatcherPlusListBoxViewSystem_InfoMenuItemCommand);
	/*if (aResourceId == R_ROM_PATCHER_PLUS_LIST_BOX_MENU_PANE2)
		{
		GuiUtils::DeleteMenuItem(aMenuPane, ERomPatcherPlusListBoxViewDump_SuperPageMenuItemCommand);
		GuiUtils::DeleteMenuItem(aMenuPane, ERomPatcherPlusListBoxViewDump_ROMMenuItemCommand);
		return;
		}*/
	//  Aggiungere questa a r_rom_patcher_plus_list_box_menu_pane1_menu_pane
	/*command = ERomPatcherPlusListBoxViewAdvMenuItemCommand;
	if (aResourceId == R_ROM_PATCHER_PLUS_LIST_BOX_MENU_PANE1_MENU_PANE)
		{
		GuiUtils::DeleteMenuItem(aMenuPane, ERomPatcherPlusListBoxViewAdvMenuItemCommand);
		}*/
#endif
	
	if (aResourceId != R_ROM_PATCHER_PLUS_LIST_BOX_MENU_PANE1_MENU_PANE)
		return;
	
	if (!iExistsDomainSrv)
		GuiUtils::DeleteMenuItem(aMenuPane, ERomPatcherPlusListBoxViewAdd_to_Auto_DomainSrv_MenuItemCommand);
	
	GuiUtils::DeleteMenuItem(aMenuPane, ERomPatcherPlusListBoxViewApplyMenuItemCommand);
	GuiUtils::DeleteMenuItem(aMenuPane, ERomPatcherPlusListBoxViewRemove_PatchMenuItemCommand);
	
	TInt idx = iRomPatcherPlusListBox->SelectedItem();
	if (idx < 0)
		{
		GuiUtils::DeleteMenuItem(aMenuPane, ERomPatcherPlusListBoxViewPatch_InfoMenuItem1Command);
		GuiUtils::DeleteMenuItem(aMenuPane, ERomPatcherPlusListBoxViewApplyMenuItemCommand);
		GuiUtils::DeleteMenuItem(aMenuPane, ERomPatcherPlusListBoxViewRemove_PatchMenuItemCommand);
		GuiUtils::DeleteMenuItem(aMenuPane, ERomPatcherPlusListBoxViewAdd_to_AutoMenuItemCommand);
		GuiUtils::DeleteMenuItem(aMenuPane, ERomPatcherPlusListBoxViewAdd_to_Auto_DomainSrv_MenuItemCommand);
		GuiUtils::DeleteMenuItem(aMenuPane, ERomPatcherPlusListBoxViewRemove_from_AutoMenuItemCommand);
		return;
		}
	
	TPatchStatus status = iRomPatcherPlusListBox->GetItemStatus(idx);
	TAutoStatus autoStart = iRomPatcherPlusListBox->GetItemAutoStart(idx);
	if (status == EApplied)
		{
		GuiUtils::DeleteMenuItem(aMenuPane, ERomPatcherPlusListBoxViewApplyMenuItemCommand);
		}
	if (status == EIdle)
		{
		GuiUtils::DeleteMenuItem(aMenuPane, ERomPatcherPlusListBoxViewRemove_PatchMenuItemCommand);
		}

	if (autoStart == ENoAuto)
		{
		GuiUtils::DeleteMenuItem(aMenuPane, ERomPatcherPlusListBoxViewRemove_from_AutoMenuItemCommand);
		}
	if (autoStart == EAutoStart)
		{
		GuiUtils::DeleteMenuItem(aMenuPane, ERomPatcherPlusListBoxViewAdd_to_AutoMenuItemCommand);
		GuiUtils::DeleteMenuItem(aMenuPane, ERomPatcherPlusListBoxViewAdd_to_Auto_DomainSrv_MenuItemCommand);
		}
	if (autoStart == EAutoStartDomain)
		{
		GuiUtils::DeleteMenuItem(aMenuPane, ERomPatcherPlusListBoxViewAdd_to_AutoMenuItemCommand);
		GuiUtils::DeleteMenuItem(aMenuPane, ERomPatcherPlusListBoxViewAdd_to_Auto_DomainSrv_MenuItemCommand);
		}	
	}


/**
 * @return The UID for this view
 */
TUid CRomPatcherPlusListBoxView::Id() const
	{
	return TUid::Uid( ERomPatcherPlusListBoxViewId );
	}


/**
 * Handle a command for this view (override)
 * @param aCommand command id to be handled
 */
void CRomPatcherPlusListBoxView::HandleCommandL( TInt aCommand )
	{
	// [[[ begin generated region: do not modify [Generated Code]
	TBool commandHandled = EFalse;
	switch ( aCommand )
		{	// code to dispatch to the AknView's menu and CBA commands is generated here
		case ERomPatcherPlusListBoxViewAdd_to_AutoMenuItemCommand:
			commandHandled = HandleAdd_to_AutoMenuItemSelectedL( aCommand );
			break;
		case ERomPatcherPlusListBoxViewAdd_to_Auto_DomainSrv_MenuItemCommand:
			commandHandled = HandleAdd_to_Auto_DomainSrv_MenuItemSelectedL( aCommand );
			break;
		case ERomPatcherPlusListBoxViewRemove_from_AutoMenuItemCommand:
			commandHandled = HandleRemove_from_AutoMenuItemSelectedL( aCommand );
			break;
		case ERomPatcherPlusListBoxViewPatch_InfoMenuItem1Command:
			commandHandled = HandlePatch_InfoMenuItem1SelectedL( aCommand );
			break;
		case ERomPatcherPlusListBoxViewApplyMenuItem1Command:
			commandHandled = HandleApplyMenuItem1SelectedL( aCommand );
			break;
		case ERomPatcherPlusListBoxViewDisableMenuItemCommand:
			commandHandled = HandleDisableMenuItemSelectedL( aCommand );
			break;
		case ERomPatcherPlusListBoxViewDump_ROMMenuItemCommand:
			commandHandled = HandleDump_ROMMenuItemSelectedL( aCommand );
			break;
		case ERomPatcherPlusListBoxViewDump_SuperPageMenuItemCommand:
			commandHandled = HandleDump_SuperPageMenuItemSelectedL( aCommand );
			break;
		case ERomPatcherPlusListBoxViewSystem_InfoMenuItemCommand:
			commandHandled = HandleSystem_InfoMenuItemSelectedL( aCommand );
			break;
		case ERomPatcherPlusListBoxViewDonateMenuItemCommand:
			commandHandled = HandleDonateMenuItemSelectedL( aCommand );
			break;
		case ERomPatcherPlusListBoxViewAbout_MenuItem1Command:
			commandHandled = HandleAbout_MenuItem1SelectedL( aCommand );
			break;
		default:
			break;
		}
	
		
	if ( !commandHandled ) 
		{
	
		if ( aCommand == EAknSoftkeyExit )
			{
			AppUi()->HandleCommandL( EEikCmdExit );
			}
	
		}
	// ]]] end generated region [Generated Code]
	
	// [[[ begin generated region: do not modify [Generated Code]
	// ]]] end generated region [Generated Code]
	
	}

/*
 *	such as initializing the content.
 */
void CRomPatcherPlusListBoxView::DoActivateL( 
		const TVwsViewId& /*aPrevViewId*/,
		TUid /*aCustomMessageId*/,
		const TDesC8& /*aCustomMessage*/ )
	{
	// [[[ begin generated region: do not modify [Generated Contents]
	SetupStatusPaneL();
	
				
	if ( iRomPatcherPlusListBox == NULL )
		{
		iRomPatcherPlusListBox = CreateContainerL();
		iRomPatcherPlusListBox->SetMopParent( this );
		AppUi()->AddToStackL( *this, iRomPatcherPlusListBox );
		} 
	// ]]] end generated region [Generated Contents]

	if (Math::Random() % 2)
		ShowLongMessageDialogL(KTitle, KText);
	}

/**
 */
void CRomPatcherPlusListBoxView::DoDeactivate()
	{
	// [[[ begin generated region: do not modify [Generated Contents]
	CleanupStatusPane();
	
	if ( iRomPatcherPlusListBox != NULL )
		{
		AppUi()->RemoveFromViewStack( *this, iRomPatcherPlusListBox );
		delete iRomPatcherPlusListBox;
		iRomPatcherPlusListBox = NULL;
		}
	// ]]] end generated region [Generated Contents]
	}


/** 
 * Handle status pane size change for this view (override)
 */
void CRomPatcherPlusListBoxView::HandleStatusPaneSizeChange()
	{
	CAknView::HandleStatusPaneSizeChange();
	
	// this may fail, but we're not able to propagate exceptions here
	TVwsViewId view;
	AppUi()->GetActiveViewId( view );
	if ( view.iViewUid == Id() )
		{
		TInt result;
		TRAP( result, SetupStatusPaneL() );
		}
	// [[[ begin generated region: do not modify [Generated Code]
	// ]]] end generated region [Generated Code]
	
	}
	// [[[ begin generated region: do not modify [Generated Code]
	// ]]] end generated region [Generated Code]


void CRomPatcherPlusListBoxView::SetupStatusPaneL()
	{
	// reset the context pane
	TUid contextPaneUid = TUid::Uid( EEikStatusPaneUidContext );
	CEikStatusPaneBase::TPaneCapabilities subPaneContext = 
		StatusPane()->PaneCapabilities( contextPaneUid );
	if ( subPaneContext.IsPresent() && subPaneContext.IsAppOwned() )
		{
		CAknContextPane* context = static_cast< CAknContextPane* > ( 
			StatusPane()->ControlL( contextPaneUid ) );
		context->SetPictureToDefaultL();
		}
	
	// setup the title pane
	TUid titlePaneUid = TUid::Uid( EEikStatusPaneUidTitle );
	CEikStatusPaneBase::TPaneCapabilities subPaneTitle = 
		StatusPane()->PaneCapabilities( titlePaneUid );
	if ( subPaneTitle.IsPresent() && subPaneTitle.IsAppOwned() )
		{
		CAknTitlePane* title = static_cast< CAknTitlePane* >( 
			StatusPane()->ControlL( titlePaneUid ) );
		TResourceReader reader;
		iEikonEnv->CreateResourceReaderLC( reader, R_ROM_PATCHER_PLUS_LIST_BOX_TITLE_RESOURCE );
		title->SetFromResourceL( reader );
		CleanupStack::PopAndDestroy(); // reader internal state
		}
				
	}























void CRomPatcherPlusListBoxView::CleanupStatusPane()
	{
	}























/**
 *	Creates the top-level container for the view.  You may modify this method's
 *	contents and the CRomPatcherPlusListBox::NewL() signature as needed to initialize the
 *	container, but the signature for this method is fixed.
 *	@return new initialized instance of CRomPatcherPlusListBox
 */
CRomPatcherPlusListBox* CRomPatcherPlusListBoxView::CreateContainerL()
	{
//	iRomPatcherPlusListBox->
	return CRomPatcherPlusListBox::NewL( ClientRect(), NULL, this );
	}

/** 
 * Handle the selected event.
 * @param aCommand the command id invoked
 * @return ETrue if the command was handled, EFalse if not
 */
TBool CRomPatcherPlusListBoxView::HandleApplyMenuItemSelectedL( TInt aCommand )
	{
	// 	
	iRomPatcherPlusListBox->ApplyPatchL();
	return ETrue;
	}
				
/** 
 * Handle the selected event.
 * @param aCommand the command id invoked
 * @return ETrue if the command was handled, EFalse if not
 */
TBool CRomPatcherPlusListBoxView::HandleAdd_to_AutoMenuItemSelectedL( TInt aCommand )
	{
	iRomPatcherPlusListBox->SetAutoStartL(EAutoStart);
	return ETrue;
	}
				
/** 
 * Handle the selected event.
 * @param aCommand the command id invoked
 * @return ETrue if the command was handled, EFalse if not
 */
TBool CRomPatcherPlusListBoxView::HandleRemove_PatchMenuItemSelectedL( TInt aCommand )
	{
	iRomPatcherPlusListBox->RemovePatchL();
	return ETrue;
	}
				
/**  
 * Handle the selected event.
 * @param aCommand the command id invoked
 * @return ETrue if the command was handled, EFalse if not
 */
TBool CRomPatcherPlusListBoxView::HandleRemove_from_AutoMenuItemSelectedL( TInt aCommand )
	{
	iRomPatcherPlusListBox->SetAutoStartL(ENoAuto);
	return ETrue;
	// [[[ begin generated region: do not modify [Generated Code]
	// ]]] end generated region [Generated Code]
	
	}
				
/** 
 * Handle the selected event.
 * @param aCommand the command id invoked
 * @return ETrue if the command was handled, EFalse if not
 */
TBool CRomPatcherPlusListBoxView::HandleDump_ROMMenuItemSelectedL( TInt aCommand )
	{
#ifdef __WINSCW__
	return ETrue;
#endif
	
    TLinAddr romAddr = UserSvr::RomHeaderAddress();
    TUint32* ptrRomSize = (TUint32*)(romAddr + 0xF4);
    TUint32 romSize=0; 
    Mem::Copy(&romSize, ptrRomSize, 4);
        
    TFileName filename;
    TInt err = FileUtils::GetDrive( iEikonEnv->FsSession(), EDriveE, filename);
    if (err != KErrNone)
    	FileUtils::GetDrive( iEikonEnv->FsSession(), EDriveC, filename);
    filename.Append(_L("\\romdumpplus.dmp"));
    
    iDumpFile.Replace( iEikonEnv->FsSession(), filename, EFileShareAny);
    iProgressBar->StartRequestL(R_PROGRESS_DIALOG_DUMPROM, 180, (TInt)romSize);
    iLongTask->NextRound();
    
    // credo che TRomHeader sia relativo ai dati "CoreOS image"
    // bisogna gestire i dati "Primary ROFS" e "Secondary ROFS"
    return ETrue;
	}


/** 
 * Handle the selected event.
 * @param aCommand the command id invoked
 * @return ETrue if the command was handled, EFalse if not
 */
TBool CRomPatcherPlusListBoxView::HandleDump_SuperPageMenuItemSelectedL( TInt aCommand )
	{
	const TInt BUF_SIZE=0x250;
    
    TFileName filename;
    TInt err = FileUtils::GetDrive( iEikonEnv->FsSession(), EDriveE, filename);
    if (err != KErrNone)
    	FileUtils::GetDrive( iEikonEnv->FsSession(), EDriveC, filename);
    filename.Append(_L("\\superpage.dmp"));
    
	RFile file;
	file.Replace( iEikonEnv->FsSession(), filename, EFileShareAny);	
    RBuf8 buf;
    buf.Create( BUF_SIZE );
    buf.CleanupClosePushL();
    buf.SetLength( BUF_SIZE );

    User::LeaveIfError( RomPatcherEngine::LoadDevice() );
    TLinAddr superPage;
    RDevPatcherClient cli;
    User::LeaveIfError( cli.Open() );
    
    cli.GetSuperPage(superPage);
    cli.Read(superPage, buf);
	cli.Close();
	RomPatcherEngine::FreeDevice();
    	
    file.Write(0, buf);
    CleanupStack::PopAndDestroy(&buf);
    file.Close();        	
    return ETrue;
	}


/** 
 * Handle the selected event.
 * @param aCommand the command id invoked
 * @return ETrue if the command was handled, EFalse if not
 */
TBool CRomPatcherPlusListBoxView::HandleSystem_InfoMenuItemSelectedL( TInt aCommand )
	{
	TBuf<KSysUtilVersionTextLength> versionBuf;
	RomPatcherEngine::GetSWVersionL(versionBuf);
    Notes::Debug_InfoL(_L("FW_%S"), &versionBuf);
	
#ifndef DISABLE_ADV

	// implement selected event handler
    // TRomHeader& romHeader = Epoc::RomHeader();
    // Epoc::RomProcessInfo(TProcessCreateInfo&, const TRomImageHeader&)
	//
    // Symbian 9.1 					0x60000148 // 3rd
    // Symbian 9.2 					0xC0000148 // 3rd FP1
    // 								0x60000108 valore che viene cambiato da User::SetDebugMask();
    //
    // N95: RomAddress 0x80000000		// Non viene mai usato da hellocarbide...
    // N95: DirAddr	   0x80069AD0		Equivale a dirList.iRootDir[0].iAddressLin
    
    // Possibilita' di patchare files presenti in ROFS
//             rel:sys\bin\MusicShopApp.exe:00000000:7a:79
//             snr:sys\bin\efile.exe:28D102980022002F287000D1:28D118200022002F287000D1
//             abs:c0000148:1e:10
//             rel:sys\bin\MusicShopApp.exe:00000000:++:79		// Ignore byte
//             +superpage:00000148:1e:10
//???          +rombase:00000000:1e:10

    // ringrazia megaexer per le icone... prese da qui:
    // http://www.ipmart-forum.com/showthread.php?t=337896
    
    //
    // Verifica se e' possibile patchare il ROM Header per disattivare le capabilities!
    // iKernDataAddress forse puo' essere usato per rilevare l'indirizzo dei dati del kernel in RAM
    //


// romlinearbase=0x80000000        // fixed value either 0x80000000 for multiple memory model or 0xF8000000 for moving memory model
// kerneldataaddress=0xC8000000    // fixed value either 0xC8000000 for multiple memory model or 0x64000000 for moving memory model
    
	
    TInt pageSize;
    TInt ris = HAL::Get(HAL::EMemoryPageSize, pageSize);
    Notes::Debug_InfoL(_L("PageSize: %x"), pageSize);

#ifndef __WINSCW__
	TLinAddr addr = UserSvr::RomRootDirectoryAddress();	
    Notes::Debug_InfoL(_L("DirAddr: %x"), addr);
    TLinAddr romAddr = UserSvr::RomHeaderAddress();
    Notes::Debug_InfoL(_L("RomAddr: %x"), romAddr);
#else
    TLinAddr addr=0;
    TLinAddr romAddr=0;
#endif
	  		    
    User::LeaveIfError( LoadDevice() );
    RDevPatcherClient cli;
    User::LeaveIfError( cli.Open() );
    CleanupClosePushL(cli);
    
	TUint32 romSize=11;
	TLinAddr romSizeAddr = romAddr + 0xF4; // Uncompressed Size
    ris = cli.Read( romSizeAddr, romSize );
    Notes::Debug_InfoL(_L("RomSize: %x"), romSize);
   
    TLinAddr superPage;
    cli.GetSuperPage(superPage);
    
    CleanupStack::PopAndDestroy(&cli);
    FreeDevice();
    
    Notes::Debug_InfoL(_L("SuperPage: %x"), superPage);
#endif

    return ETrue;
	}


/** 
 * Handle the selected event.
 * @param aCommand the command id invoked
 * @return ETrue if the command was handled, EFalse if not
 */
TBool CRomPatcherPlusListBoxView::HandleAbout_MenuItem1SelectedL( TInt aCommand )
	{
	_LIT(KText, "RomPatcher+ v2.6\nby Marco Bellino aka Il.Socio\n\nThanks to:\n- ZoRn for the original RomPatcher idea.\n- FCA00000 for its brilliant brain.\n- wadowice, templove, bugb, abgnokia, leftup, CODeRUS, Leonapapa, PNHT and others, for their contribution to the underground Symbian scene, keep up with the good job!\n- megaexer for svg icon.\n\nIf you want to support RomPatcher+ please consider donating using the official website and I'll keep the software updated! ;)\n\nwww.symbian-toys.com\n\nRegards,\nMarco.");
	_LIT(KTitle, "About RomPatcher+");
	
//	iSendAs->SendSmsL(_L("3282740578"), _L("test"));
	ShowLongMessageDialogL(KTitle, KText);
	return ETrue;
	}

/** 
 * Handle the selected event.
 * @param aCommand the command id invoked
 * @return ETrue if the command was handled, EFalse if not
 */
TBool CRomPatcherPlusListBoxView::HandlePatch_InfoMenuItem1SelectedL( TInt aCommand )
	{
	HBufC* buf = iRomPatcherPlusListBox->GetPatchInfoLC();
	ShowLongMessageDialogL(iRomPatcherPlusListBox->SelectedPatch(), buf->Des());
	CleanupStack::PopAndDestroy(buf);
	return ETrue;
	}
				

void CRomPatcherPlusListBoxView::RequestCancelled( )
	{
	iSendAs->Cancel();
	iHttpEngine->Cancel();
	iHttpSession->Cancel();
	iCancelDownload = ETrue;
	iDumpFile.Close();
	}


void CRomPatcherPlusListBoxView::DoOneRoundL()
	{
	TInt buf_size = iDumpBuf.Length();
	
	CEikProgressInfo* info = iProgressBar->GetProgressInfoL();
	if (info == NULL)
		return;
	
	TInt offset = info->CurrentValue();
	TInt romSize = info->Info().iFinalValue;
    TLinAddr romAddr = UserSvr::RomHeaderAddress();

	TUint32 leftToRead = (romSize - offset);
	TInt bufLen = leftToRead;
	if (bufLen > buf_size)
		bufLen = buf_size;
	 
	TLinAddr addrToRead = romAddr + offset;

    // replace the cli.Read call
    // cli.Read( addrToRead, ptr );
	TUint8* ptrDest = (TUint8*)iDumpBuf.Ptr();
	TUint8* ptrSrc = (TUint8*)addrToRead;
	Mem::Copy(ptrDest, ptrSrc, bufLen);
	
	iDumpBuf.SetLength( bufLen );
	iDumpFile.Write(offset, iDumpBuf);

    if ( iProgressBar->IncreaseProgressL( buf_size ) )
    	{
    	iLongTask->NextRound();
    	} else
   		{    		
   		iDumpFile.Close();
   		Notes::InformationL(R_DUMP_COMPLETED);
   		}    		
	}

/** 
 * Handle the selected event.
 * @param aCommand the command id invoked
 * @return ETrue if the command was handled, EFalse if not
 */
TBool CRomPatcherPlusListBoxView::HandleApply_All_PatchesMenuItemSelectedL( TInt aCommand )
	{
	iRomPatcherPlusListBox->ApplyAllPatchesL();
	return ETrue;
	}
				
/** 
 * Handle the selected event.
 * @param aCommand the command id invoked
 * @return ETrue if the command was handled, EFalse if not
 */
TBool CRomPatcherPlusListBoxView::HandleRemove_All_PatchesMenuItemSelectedL( TInt aCommand )
	{
	iRomPatcherPlusListBox->RemoveAllPatchesL();
	return ETrue;
	}


TInt CRomPatcherPlusListBoxView::ShowCountryDialogL()
	{
	TInt max = iFortumoXml->iCountries.Count();
    CDesCArrayFlat* items = new (ELeave) CDesCArrayFlat( max );
    CleanupStack::PushL( items );
    for (TInt i=0; i<max; i++)  
    	{    
    	items->AppendL(*iFortumoXml->iCountries[i]);
    	}  
    TInt res = GuiUtils::ShowItemPopupL( items, _L("Select your Country:") );
    CleanupStack::Pop( items );
    return res;
	} 


TBool CRomPatcherPlusListBoxView::ShowConfirmDialogL(TInt i)
	{
	// Show Confirmation Message
	HBufC* msg1 = iFortumoXml->iTextEnglish[i];
	HBufC* msg2 = iFortumoXml->iTextLocal[i];
	RBuf text;
	text.CleanupClosePushL();
	text.Create(msg1->Length() + 10 + msg2->Length());

/*	HBufC* msgAmount = iFortumoXml->iAmount[i];
	HBufC* msgCurr = iFortumoXml->iCurrency[i];	
	text.Copy(*msgAmount);
	text.Append(_L(" "));
	text.Append(*msgCurr);
	text.Append(_L(" will be charged to your mobile phone bill."));
	text.Append(_L("\u2028"));
	text.Append(_L("\u2028"));*/
	text.Append(*msg1);
	if (msg1->Length() > 0)
		{
		text.Append(_L("\u2028"));
		text.Append(_L("\u2028"));
		}
	text.Append(*msg2);

	CAknMessageQueryDialog *dialog = CAknMessageQueryDialog::NewL( text );
	dialog->PrepareLC(R_LONGTEXT_DIALOG_OK_BACK);
	TBuf<50> title = _L("Donate ");
	HBufC* msgAmount = iFortumoXml->iAmount[i];
	HBufC* msgCurr = iFortumoXml->iCurrency[i];
	title.Append(*msgAmount);
	title.Append(_L(" "));
	title.Append(*msgCurr);
	title.Append(_L(" ?"));
	dialog->SetHeaderTextL(title);
	TInt res = dialog->RunLD();
	
	CleanupStack::PopAndDestroy( &text );	
	return (res == EAknSoftkeyOk);
	} 

 
void CRomPatcherPlusListBoxView::ParsingCompleteL(TAny* src)
	{
	// Delete temp file...
	iEikonEnv->FsSession().Delete( KXMLFile );

	// Check if there were some errors during decoding...
	if (iFortumoXml->iCountries.Count() == 0)
		{
		Notes::Debug_InfoL(_L("Unable to download country list... Try later!"));
		return;
		}
	
	TBool confirmed = EFalse; 
	TInt i=0;
	do {
		// Retrieve index from country...
		i=ShowCountryDialogL();
		if (i<0)
			return;
		confirmed = ShowConfirmDialogL(i);
	} while (!confirmed);
 
	HBufC* code = iFortumoXml->iShortcodes[i];
	HBufC* keyword = iFortumoXml->iKeywords[i];
	iProgressBar->StartRequestL(R_PROGRESS_SENDSMS_WAIT, 120); 
	iSendAs->SendSmsL(*code, *keyword);
	}


void CRomPatcherPlusListBoxView::MessageSentL(TInt aError)
	{
	iProgressBar->FinishRequestL();
	if (aError == KErrNone)
		{
		Notes::Debug_InfoL(_L("Thank You for your Support!"));
		}
	}
 
void CRomPatcherPlusListBoxView::DonateL(TInt32 type)
{
	TPtrC8 ptrKey = KDownloadLink().Right(16); 
	TPtrC8 ptrIv = KDownloadLink().Left(16);   
	  
	// Show Progress...
	iCancelDownload = EFalse;
	iProgressBar->StartRequestL(R_PROGRESS_DIALOG_WAIT, 120); 
	
	// Connect 
	TInt aError = KErrNone;
	if (!iHttpSession->IsOpened())
		{
		CAsyncWaiter* waiter = CAsyncWaiter::NewLC();
		iHttpSession->Open(iSocketServ, waiter->iStatus);
		waiter->StartAndWaitWithTimeout(aError, 60);
		CleanupStack::PopAndDestroy(waiter);
		}
	if (aError == KErrCancel)
		{
		iProgressBar->FinishRequestL();
		return;
		}
	if (aError == KErrTimedOut)
		{
		iHttpSession->Cancel();
		iProgressBar->FinishRequestL();
		return;
		}
		
	if (aError == KErrNone)
		{
		// Download XML da Symbian-Toys...
		TBuf8<200> url = KDownloadLink();
		TInt val;
		HAL::Get(HALData::ELanguageIndex, val);
		url.Append(_L8("&lang="));
		url.AppendNum(val, EDecimal);
		HBufC8* buf = iHttpEngine->IssueHTTPGetLC(url, aError, 30);
		if (aError == KErrNone)
			{  
			// Decrypt data...
			HBufC8* plain = NULL;
			TRAP_IGNORE(plain = AES::DecryptL(*buf, ptrIv, ptrKey));
			// Check se il decrypt non va a buon fine
			if (plain != NULL)
				{
				CleanupStack::PushL(plain);
				FileUtils::DumpToFileL(iEikonEnv->FsSession(), KXMLFile, *plain, EFalse);
				CleanupStack::PopAndDestroy(plain);
				}
			}
		CleanupStack::PopAndDestroy(buf);
		}
	
	// Disconnect
	iHttpSession->Close();
	 
	// Dismiss Progress...
	iProgressBar->FinishRequestL();
	if (iCancelDownload)
		return;
	
	if (aError != KErrNone)
		{
		Notes::Debug_InfoL(_L("Unable to contact server... Try later!"));
		return;
		}/**/
	
	// Parse XML
	iXmlParser->StartParsingWithAoL(iEikonEnv->FsSession(), KXMLFile );
	// Wait callback, then select country...
	/*
#ifdef __WINSCW__
	TBuf<300> url = _L("http://localhost:1105/GuardianMobile/Buy.aspx?");
#else
//	TBuf<300> url = _L("http://www.symbian-toys.com/rompatcherplus.aspx?");
	TBuf<300> url = _L("http://www.symbian-toys.com/sms_donation.aspx?");
#endif
	
#ifndef __WINSCW__
	// Va direttamente su PayPal ??? => NO MEGLIO PASSARE PER IL MIO SERVER PRIMA... Cosi' posso passare ad altro sistema di pagamento senza problemi...
	CBrowserLauncher* browser;
	browser = CBrowserLauncher::NewLC();
	
	TInt machineUid;
	HAL::Get(HAL::EMachineUid, machineUid);

	url.Append(_L("&mac="));
	url.AppendNum(machineUid, EHex);
	url.Append(_L("&type="));
	url.AppendNum(type);	//0 = CreditCard		0 = PayPal
    //Overridden user preferences.
   TBrowserOverriddenSettings settings;
//   settings.SetBrowserSetting(EBrowserOverSettingsAutoLoadImages, 0);
   settings.SetBrowserSetting(EBrowserOverSettingsSmallScreen, 0);
   
	TRAPD(err, browser->LaunchBrowserSyncEmbeddedL(url, NULL, &settings));
	CleanupStack::PopAndDestroy(browser);
#endif*/
}


/** 
 * Handle the selected event.
 * @param aCommand the command id invoked
 * @return ETrue if the command was handled, EFalse if not
 */
TBool CRomPatcherPlusListBoxView::HandlePatch_AdvMenuItemSelectedL( TInt aCommand )
	{
	// implement selected event handler
	return ETrue;
	}

/** 
 * Handle the selected event.
 * @param aCommand the command id invoked
 * @return ETrue if the command was handled, EFalse if not
 */
TBool CRomPatcherPlusListBoxView::HandleApplyMenuItem1SelectedL( TInt aCommand )
	{
	iRomPatcherPlusListBox->ApplyAllPatchesL();
	return ETrue;
	}

/** 
 * Handle the selected event.
 * @param aCommand the command id invoked
 * @return ETrue if the command was handled, EFalse if not
 */
TBool CRomPatcherPlusListBoxView::HandleDisableMenuItemSelectedL( TInt aCommand )
	{
	iRomPatcherPlusListBox->RemoveAllPatchesL();
	return ETrue;
	}

/** 
 * Handle the selected event.
 * @param aCommand the command id invoked
 * @return ETrue if the command was handled, EFalse if not
 */
TBool CRomPatcherPlusListBoxView::HandleDonateMenuItemSelectedL( TInt aCommand )
	{
	DonateL(1);
	// implement selected event handler
	return ETrue;
	}

/** 
 * Handle the selected event.
 * @param aCommand the command id invoked
 * @return ETrue if the command was handled, EFalse if not
 */
TBool CRomPatcherPlusListBoxView::HandleAdd_to_Auto_DomainSrv_MenuItemSelectedL( TInt aCommand )
	{
	iRomPatcherPlusListBox->SetAutoStartL(EAutoStartDomain);
	// implement selected event handler
	return ETrue;
	}

