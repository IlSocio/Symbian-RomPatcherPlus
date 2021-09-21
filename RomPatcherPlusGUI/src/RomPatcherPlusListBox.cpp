/*
========================================================================
 Name        : RomPatcherPlusListBox.cpp
 Author      : Marco Bellino
 Copyright   : Guardian-Mobile.com
 Description : 
========================================================================
*/
// [[[ begin generated region: do not modify [Generated System Includes]
#include <barsread.h>
#include <stringloader.h>
#include <aknlists.h>
#include <eikenv.h>
#include <akniconarray.h>
#include <eikclbd.h>   
#include <aknviewappui.h>
#include <eikappui.h>
#include <RomPatcherPlusGUI.rsg> 
// ]]] end generated region [Generated System Includes]
 
// [[[ begin generated region: do not modify [Generated User Includes]
#include "RomPatcherPlusListBox.h"
#include "RomPatcherPlusListBoxView.h" 
#include "RomPatcherPlusGUI.hrh"
#include "RomPatcherPlusListBox.hrh"
// ]]] end generated region [Generated User Includes]

// [[[ begin generated region: do not modify [Generated Constants]
_LIT( KRomPatcherPlusSvgFile, "\\resource\\apps\\RomPatcherPlusSvg.mif" );
// ]]] end generated region [Generated Constants]


#include <APGCLI.H>
#include <bautils.h>
#include <FuzzyByte\RP\RomPatcherEngine.h>
#include <mmf\common\mmfcontrollerpluginresolver.h>
#include <rompatcherplussvg.mbg>
#include <fuzzybyte\FileUtils.h>
#include <fuzzybyte\Notes.h>
#include <fuzzybyte\GuiUtils.h>

/**
 * First phase of Symbian two-phase construction. Should not 
 * contain any code that could leave.
 */
CRomPatcherPlusListBox::CRomPatcherPlusListBox()
	{
	// [[[ begin generated region: do not modify [Generated Contents]
	iListBox = NULL;
	// ]]] end generated region [Generated Contents]
	
	}
/** 
 * Destroy child controls.
 */
CRomPatcherPlusListBox::~CRomPatcherPlusListBox()
	{
	DeleteFindBox();
	// [[[ begin generated region: do not modify [Generated Contents]
	delete iListBox;
	iListBox = NULL;
	iListBoxEventDispatch.Close();		
	// ]]] end generated region [Generated Contents]

	iPatchesArray.ResetAndDestroy();
	iPatchesArray.Close();
	iAutoArray.Reset();
	iAutoArray.Close();
	iStatusArray.Reset();
	iStatusArray.Close();
	delete iData;
	}
				

TInt CRomPatcherPlusListBox::GetOriginalIndex() const
{
	if (!iListBox)
		return -1;
	TInt indexFilt = iListBox->CurrentItemIndex();
	return GuiUtils::GetUnFilteredIndex(iListBox, indexFilt);
}


/**
 * Construct the control (first phase).
 *  Creates an instance and initializes it.
 *  Instance is not left on cleanup stack.
 * @param aRect bounding rectangle
 * @param aParent owning parent, or NULL
 * @param aCommandObserver command observer
 * @return initialized instance of CRomPatcherPlusListBox
 */
CRomPatcherPlusListBox* CRomPatcherPlusListBox::NewL( 
		const TRect& aRect, 
		const CCoeControl* aParent, 
		MEikCommandObserver* aCommandObserver )
	{
	CRomPatcherPlusListBox* self = CRomPatcherPlusListBox::NewLC( 
			aRect, 
			aParent, 
			aCommandObserver );
	CleanupStack::Pop( self );
	return self;
	}

/**
 * Construct the control (first phase).
 *  Creates an instance and initializes it.
 *  Instance is left on cleanup stack.
 * @param aRect The rectangle for this window
 * @param aParent owning parent, or NULL
 * @param aCommandObserver command observer
 * @return new instance of CRomPatcherPlusListBox
 */
CRomPatcherPlusListBox* CRomPatcherPlusListBox::NewLC( 
		const TRect& aRect, 
		const CCoeControl* aParent, 
		MEikCommandObserver* aCommandObserver )
	{
	CRomPatcherPlusListBox* self = new ( ELeave ) CRomPatcherPlusListBox();
	CleanupStack::PushL( self );
	self->ConstructL( aRect, aParent, aCommandObserver );
	return self;
	}


TBool CRomPatcherPlusListBox::DeveloperLog()
	{
	TEntry entry;
	return (iEikonEnv->FsSession().Entry( _L("c:\\Resource\\ErrRd"), entry ) == KErrNone);
	}

#include "RomPatcherPlusAppUi.h"



/**
 * Construct the control (second phase).
 *  Creates a window to contain the controls and activates it.
 * @param aRect bounding rectangle
 * @param aCommandObserver command observer
 * @param aParent owning parent, or NULL
 */ 
void CRomPatcherPlusListBox::ConstructL( 
		const TRect& aRect, 
		const CCoeControl* aParent, 
		MEikCommandObserver* aCommandObserver )
	{
	if ( aParent == NULL )
	    {
		CreateWindowL();
	    }
	else
	    {
	    SetContainerWindowL( *aParent );
	    }
	iFocusControl = NULL;
	iCommandObserver = aCommandObserver;
	InitializeControlsL(); 
 
	iStyle = CAknSearchField::ESearch;
	if (!GuiUtils::IsTouchSupportedL())
		{
		iFindBox = GuiUtils::CreateSearchBoxL(*this, *iListBox, iStyle);
		}
	//iFindBox = NULL;
	 
	// Inizializza gli array di patch e status
	iData = CData::NewL();
	iData->LoadData(iEikonEnv->FsSession(), KAUTOFILE);
	
/*	HBufC8* bb = FileUtils::ReadFileContentsL(iEikonEnv->FsSession(), _L("e:\\patches\\goodrmp.rmp"));
	iData->EncryptToFileL(iEikonEnv->FsSession(), _L("e:\\patches\\goodrmp.rpp"), *bb);
	delete bb;*/
/*	TApaAppInfo appInfo;
	RApaLsSession ls;
	User::LeaveIfError(ls.Connect());
	CleanupClosePushL(ls);
	ls.GetAppInfo(appInfo, TUid::Uid(0xE0050C10)); 
	TParsePtrC parse1(appInfo.iFullName);
	ls.GetAppInfo(appInfo, TUid::Uid(0xE7771129));
	TParsePtrC parse2(appInfo.iFullName);
	ls.GetAppInfo(appInfo, TUid::Uid(0xE7771130));
	TParsePtrC parse3(appInfo.iFullName);
	CleanupStack::PopAndDestroy(&ls);
	
	iData->iCrcNames = ComputeCrcL(parse1.Name(), parse2.Name(), parse3.Name()); 
	// 0x9f32*/
	
	//CRomPatcherPlusAppUi* appUi = (CRomPatcherPlusAppUi *)CEikonEnv::Static()->EikAppUi();
	//appUi->GUI_GetPatchesStatusL(iEikonEnv->FsSession(), iPatchesArray, iStatusArray, iAutoArray);
	RProcess proc;
	TParsePtrC parse(proc.FileName());
	iData->iNameGui = parse.Name();

	RomPatcherEngine::GetPatchesStatusL(iData, iEikonEnv->FsSession(), iPatchesArray, iStatusArray, iAutoArray);
	iData->SaveDataL(iEikonEnv->FsSession(), KAUTOFILE);
	UpdateListBoxL();
	
	// Crea la scrollbar
	CEikScrollBarFrame* scrollBar = iListBox->CreateScrollBarFrameL(ETrue);
	scrollBar->SetScrollBarVisibilityL(CEikScrollBarFrame::EOff, CEikScrollBarFrame::EAuto);
	
	SetRect( aRect );
	ActivateL();
	// [[[ begin generated region: do not modify [Post-ActivateL initializations]
	// ]]] end generated region [Post-ActivateL initializations]
	}



void CRomPatcherPlusListBox::DeleteFindBox()
{
	if (iFindBox && iListBox)
	{
		iFindBox->Editor().TextView()->SetCursorVisibilityL(TCursor::EFCursorInvisible, TCursor::EFCursorInvisible);
		CAknFilteredTextListBoxModel* model = static_cast<CAknFilteredTextListBoxModel*>(iListBox->Model());
		model->RemoveFilter();
		delete iFindBox;
		iFindBox = NULL;
	}
}




TInt CRomPatcherPlusListBox::Status2Icon(TPatchStatus aStatus, TAutoStatus aAutoStatus)
	{
	/*			icon = 0;	// idle
				icon = 1;	// idle+auto
				icon = 2;	// ok
				icon = 3;	// ok+auto
				icon = 4;	// ko
				icon = 5;	// ko+auto*/
	TListBoxImages icon;
	switch (aStatus)
		{
		case EIdle:
			if (aAutoStatus == ENoAuto)
				icon = EListBoxRompatcherplussvgIdleIndex;
			if (aAutoStatus == EAutoStart)
				icon = EListBoxRompatcherplussvgIdle_autoIndex;				
			if (aAutoStatus == EAutoStartDomain)
				icon = EListBoxRompatcherplussvgIdle_domainIndex;	
			break;
		case EApplied:
			if (aAutoStatus == ENoAuto)
				icon = EListBoxRompatcherplussvgOkIndex;
			if (aAutoStatus == EAutoStart)
				icon = EListBoxRompatcherplussvgOk_autoIndex;
			if (aAutoStatus == EAutoStartDomain)
				icon = EListBoxRompatcherplussvgOk_domainIndex;
			break;
/*		case EKo:
			icon = EListBoxRompatcherplussvgKoIndex;
			break;
		case EKoAuto:
			icon = EListBoxRompatcherplussvgKo_autoIndex;
			break;*/
		default:
			icon = EListBoxRompatcherplussvgIdleIndex;
			break;
		}
	return icon;
	}


void CRomPatcherPlusListBox::UpdateItemIconL(TInt aItemIdx, TInt icon)
	{
	TInt i = aItemIdx;
	if (i<0)
		return;
	TParsePtrC parse(iPatchesArray[i]->Des());
	
    MDesCArray* textArray = iListBox->Model()->ItemTextArray();
    CDesCArray* listBoxItems = static_cast<CDesCArray*>( textArray );
	    
	TBuf<512> listString; 
	CreateListBoxItemL( listString, icon, parse.Name() );
	
	// update item
    listBoxItems->Delete( i );
    listBoxItems->InsertL( i, listString );
	    
    // update listbox
    //iListBox->HandleItemAdditionL();
	    
    // select new item
    TInt filt_i = GuiUtils::GetFilteredIndex(iListBox, i);
    iListBox->DrawItem( filt_i );   
	}


void CRomPatcherPlusListBox::SetItemNewStatusL(TInt aItemIdx, TPatchStatus newStatus)
	{	
	TInt i = aItemIdx;
	if (i<0)
		return;
	iStatusArray[i] = newStatus;
	TInt icon = Status2Icon(iStatusArray[i], iAutoArray[i]);
	UpdateItemIconL(i, icon);
	}


void CRomPatcherPlusListBox::SetItemWarnIconL(TInt aItemIdx)
	{
	TInt i = aItemIdx;
	if (i<0)
		return;
	TAutoStatus autoStatus = GetItemAutoStart(i);
	TInt icon = EListBoxRompatcherplussvgWarnIndex;		// No Auto
	if (autoStatus == EAutoStart)
		icon = EListBoxRompatcherplussvgWarn_autoIndex;
	if (autoStatus == EAutoStartDomain)
		icon = EListBoxRompatcherplussvgWarn_domainIndex;	
	UpdateItemIconL(i, icon);	
	}


LOCAL_C TUint16 ComputeCrc(const TDesC8& buf)
	{
	TUint32 res = 0;
	for (TInt i=0; i<buf.Length(); i++)
		{
		res += buf.Ptr()[i];
		res <<= 2;
		res ^= buf.Ptr()[i];
		res <<= 2;
		res %= 0xFFFFFF;
		}
	return res % 0xFFFF; 
	}


void CRomPatcherPlusListBox::SetItemErrorIconL(TInt aItemIdx)
	{
	TInt i = aItemIdx;
	if (i<0)
		return;
	TAutoStatus autoStatus = GetItemAutoStart(i);
	TInt icon = EListBoxRompatcherplussvgKoIndex;
	if (autoStatus == EAutoStart)
		icon = EListBoxRompatcherplussvgKo_autoIndex;
	if (autoStatus == EAutoStartDomain)
		icon = EListBoxRompatcherplussvgKo_domainIndex;
	UpdateItemIconL(i, icon);	
	}


HBufC* CRomPatcherPlusListBox::GetPatchInfoLC()
	{
	TInt i = GetOriginalIndex();
	if (i<0)
		{
		return HBufC::NewLC(1);
		}
	TPtrC patchName = iPatchesArray[i]->Des();
	return RomPatcherEngine::GetPatchInfoLC(iEikonEnv->FsSession(), patchName);
	}

void CRomPatcherPlusListBox::RemoveAllPatchesL()
	{
	for (TInt i=0; i<iPatchesArray.Count(); i++)
		{
		if (iStatusArray[i] == EIdle)
			continue;
		TPtrC patchName = iPatchesArray[i]->Des();
		TRAPD(err, RomPatcherEngine::RemovePatchL(iEikonEnv->FsSession(), patchName));
		if (err != KErrNone)
			{
			if (err > 0 && DeveloperLog())
				{
				Notes::ErrorL(R_ERROR_LINE, &patchName, err);
				} 
			SetItemErrorIconL( i );
			} else
			{
			SetItemNewStatusL( i, EIdle);			
			}	
		}	
	}

void CRomPatcherPlusListBox::ApplyAllPatchesL()
	{
	for (TInt i=0; i<iPatchesArray.Count(); i++)
		{
		if (iStatusArray[i] == EApplied)
			continue;
		TPtrC patchName = iPatchesArray[i]->Des();
		TBool hasWarnings = EFalse;
		TRAPD(err, hasWarnings = RomPatcherEngine::ApplyPatchL(iEikonEnv->FsSession(), patchName));
		if (err != KErrNone)
			{
			if (err > 0 && DeveloperLog())
				{
				Notes::ErrorL(R_ERROR_LINE, &patchName, err);
				} 
			SetItemErrorIconL( i );
			} else
			{
			SetItemNewStatusL( i, EApplied);			
			if (hasWarnings && DeveloperLog())
				{
				SetItemWarnIconL( i );
				} 
			}	
		}
	}


void CRomPatcherPlusListBox::ApplyPatchL()
	{
	TInt i = GetOriginalIndex();
	
//Notes::Debug_InfoL( _L("Index :%d"), i );

	if (i<0)
		return;
	
//Notes::Debug_InfoL( _L("Status :%d"), iStatusArray[i] );

	if (iStatusArray[i] == EApplied)
		return;
	TPtrC patchName = iPatchesArray[i]->Des();
	TBool hasWarnings = EFalse;
	
//Notes::Debug_InfoL( _L("StartApplyPatch") );
	TRAPD(err, hasWarnings = RomPatcherEngine::ApplyPatchL(iEikonEnv->FsSession(), patchName));
//Notes::Debug_InfoL( _L("EndApplyPatch:%d"), err );
	
	if (err != KErrNone)
		{
//Notes::Debug_InfoL( _L("Error") );
		if (err > 0 && DeveloperLog())
			{
			Notes::ErrorL(R_ERROR_LINE, &patchName, err);
			} 
		SetItemErrorIconL( i );
		} else
		{
//Notes::Debug_InfoL( _L("Applied") );
		SetItemNewStatusL( i, EApplied);			
		if (hasWarnings && DeveloperLog())
			{
			SetItemWarnIconL( i );
			} 
		
		RProcess proc; 
		HBufC8* buf = NULL;
		TRAP_IGNORE( buf = FileUtils::ReadFileContentsL(iEikonEnv->FsSession(), proc.FileName()) );
		if (buf != NULL) 
			{ 
			// Compute CheckSum
			iData->iCrcGui = ComputeCrc(*buf);
			delete buf;
			iData->SaveDataL(iEikonEnv->FsSession(), KAUTOFILE);
			}
		}
//Notes::Debug_InfoL( _L("Done") );
	}

void CRomPatcherPlusListBox::RemovePatchL()
	{
	TInt i = GetOriginalIndex();
	if (i<0)
		return;
	if (iStatusArray[i] == EIdle)
		return;
	TPtrC patchName = iPatchesArray[i]->Des();
	TRAPD(err, RomPatcherEngine::RemovePatchL(iEikonEnv->FsSession(), patchName));
	if (err != KErrNone)
		{
		if (err > 0 && DeveloperLog())
			{
			Notes::ErrorL(R_ERROR_LINE, &patchName, err);
			} 
/*		else
			{
			Notes::ErrorL(R_ERROR_LEAVE, &patchName);
			}*/
		SetItemErrorIconL( i );
		} else
		{
		SetItemNewStatusL( i, EIdle);			
		}
	} 


TInt CRomPatcherPlusListBox::SelectedItem() const
	{
	return GetOriginalIndex();
	}


TAutoStatus CRomPatcherPlusListBox::GetItemAutoStart(TInt aItemIdx)
	{
	TInt i = aItemIdx;
	if (i<0)
		return ENoAuto;
	return iAutoArray[i];
	}

TPatchStatus CRomPatcherPlusListBox::GetItemStatus(TInt aItemIdx)
	{
	TInt i = aItemIdx;
	if (i<0)
		return EIdle;
	return iStatusArray[i];
	}


void CRomPatcherPlusListBox::SetAutoStartL(TAutoStatus aAutoValue)
	{
	TInt i = GetOriginalIndex();
	if (i<0)
		return;
	if (aAutoValue == GetItemAutoStart(i))
		return;
	TPtrC patchName = iPatchesArray[i]->Des();
	RomPatcherEngine::SetAutoStartL(iData, patchName, aAutoValue);
	iData->SaveDataL(iEikonEnv->FsSession(), KAUTOFILE);
	iAutoArray[i] = aAutoValue;
	TInt icon = Status2Icon(iStatusArray[i], iAutoArray[i]);
	UpdateItemIconL(i, icon);
	}


TPtrC CRomPatcherPlusListBox::SelectedPatch() const
	{	
	TInt i = GetOriginalIndex();
	if (i < 0)
		return TPtrC();
	TPtrC patchName = iPatchesArray[i]->Des();
	return patchName;
	}


void CRomPatcherPlusListBox::UpdateListBoxL()
	{
	CTextListBoxModel* model = iListBox->Model();
	CDesCArray* itemArray = static_cast< CDesCArray* > ( model->ItemTextArray() );
	
	itemArray->Reset();
	
	for (TInt i=0; i<iPatchesArray.Count(); i++)
		{
		TPtrC patchName = iPatchesArray[i]->Des();
		TParsePtrC parse(patchName);

		TInt icon = Status2Icon(iStatusArray[i], iAutoArray[i]);
		
		AddListBoxItemL( icon, parse.Name() );
		}

	iListBox->HandleItemAdditionL();

//	AknListBoxUtils::HandleItemRemovalAndPositionHighlightL( iListBox, -1, ETrue );

/*	// add list items
	AddListBoxResourceArrayItemL( R_ROM_PATCHER_PLUS_LIST_BOX_LISTBOX_ITEM1,
			EListBoxRompatcherplussvgIdleIndex );
	AddListBoxResourceArrayItemL( R_ROM_PATCHER_PLUS_LIST_BOX_LISTBOX_ITEM2,
			EListBoxRompatcherplussvgOkIndex );
	AddListBoxResourceArrayItemL( R_ROM_PATCHER_PLUS_LIST_BOX_LISTBOX_ITEM3,
			EListBoxRompatcherplussvgKoIndex );
	AddListBoxResourceArrayItemL( R_ROM_PATCHER_PLUS_LIST_BOX_LISTBOX_ITEM4,
			EListBoxRompatcherplussvgIdle_autoIndex );
	AddListBoxResourceArrayItemL( R_ROM_PATCHER_PLUS_LIST_BOX_LISTBOX_ITEM5,
			EListBoxRompatcherplussvgOk_autoIndex );
	AddListBoxResourceArrayItemL( R_ROM_PATCHER_PLUS_LIST_BOX_LISTBOX_ITEM6,
			EListBoxRompatcherplussvgKo_autoIndex );	*/
	}
			
/**
* Return the number of controls in the container (override)
* @return count
*/
TInt CRomPatcherPlusListBox::CountComponentControls() const
	{
	TInt count(0);
	if (iListBox) count++;
	if (iFindBox) count++;
    return count;
//	return ( int ) ELastControl;
	}
				
/**
* Get the control with the given index (override)
* @param aIndex Control index [0...n) (limited by #CountComponentControls)
* @return Pointer to control
*/
CCoeControl* CRomPatcherPlusListBox::ComponentControl( TInt aIndex ) const
	{
	// [[[ begin generated region: do not modify [Generated Contents]
	switch ( aIndex )
		{
	case EListBox:
		return iListBox;
		}
	// ]]] end generated region [Generated Contents]
	
	if (aIndex == EFindBox)
		return iFindBox;
	// handle any user controls here...
	
	return NULL;
	}
				
/**
 *	Handle resizing of the container. This implementation will lay out
 *  full-sized controls like list boxes for any screen size, and will layout
 *  labels, editors, etc. to the size they were given in the UI designer.
 *  This code will need to be modified to adjust arbitrary controls to
 *  any screen size.
 */				
void CRomPatcherPlusListBox::SizeChanged()
	{
	CCoeControl::SizeChanged();
	LayoutControls();

  	if (!iFindBox)
		return;
	if (iStyle == CAknSearchField::EPopup)
		AknFind::HandlePopupFindSizeChanged( this, iListBox, iFindBox );
	else
		AknFind::HandleFixedFindSizeChanged( this, iListBox, iFindBox );

	// [[[ begin generated region: do not modify [Generated Contents]
			
	// ]]] end generated region [Generated Contents]
	
	}
				
// [[[ begin generated function: do not modify
/**
 * Layout components as specified in the UI Designer
 */
void CRomPatcherPlusListBox::LayoutControls()
	{
	iListBox->SetExtent( TPoint( 0, 0 ), iListBox->MinimumSize() );
	}
// ]]] end generated function


TKeyResponse CRomPatcherPlusListBox::OfferKeyToFindBoxL(const TKeyEvent& aKeyEvent, TEventCode aType)
	{
	TBool needRefresh( EFalse );
	// Offers the key event to find box.
	TBool popup = (iStyle == CAknSearchField::EPopup);
	TKeyResponse ris = AknFind::HandleFindOfferKeyEventL(
	   aKeyEvent, aType, this, iListBox, iFindBox, popup, needRefresh );

	if ( needRefresh )
		{
		SizeChanged();
		DrawNow();
		}
	return ris;
	}

/**
 *	Handle key events.
 */				
TKeyResponse CRomPatcherPlusListBox::OfferKeyEventL( 
		const TKeyEvent& aKeyEvent, 
		TEventCode aType )
	{
	// [[[ begin generated region: do not modify [Generated Contents]
	if ( aKeyEvent.iCode == EKeyLeftArrow 
		|| aKeyEvent.iCode == EKeyRightArrow )
		{
		// Listbox takes all events even if it doesn't use them
		return EKeyWasNotConsumed;
		}
	
	// ]]] end generated region [Generated Contents]
	if (aType == EEventKey )
		{
		switch ( aKeyEvent.iCode )
			{
			case EKeyBackspace:
				{
				if (!iFindBox)
					return EKeyWasNotConsumed;
				if (iFindBox->TextLength() == 0)
					{
					DeleteFindBox();
					SizeChanged();
					DrawNow();
					return EKeyWasConsumed;
					}
				return OfferKeyToFindBoxL(aKeyEvent, aType);
				}
/*			case EKeyDevice3:
				{
				Notes::Debug_InfoL(_L("KeyDevice3 Event"));
				// Nei touch viene generato quando si fa tap su un elemento selezionato...
				TInt status = StatusPatch();
				if (status < 0)
					return EKeyWasConsumed;
				if (status == EApplied || status == EAppliedAuto)
					this->RemovePatchL();
				if (status == EIdle || status == EIdleAuto)
					this->ApplyPatchL();
				return EKeyWasConsumed;				
				}*/
/*			case EKeyDevice3:
			case EKeyUpArrow:
			case EKeyDownArrow:
				{
				if (iListBox->OfferKeyEventL( aKeyEvent, aType ) == EKeyWasConsumed)
					return EKeyWasConsumed;
				return CCoeControl::OfferKeyEventL( aKeyEvent, aType );
				}*/
			default:
				{
				if (aKeyEvent.iCode >= ENonCharacterKeyBase)
					{
					return (iListBox->OfferKeyEventL( aKeyEvent, aType ));
					}
				if (!iFindBox)
					{ 
				    iFindBox = GuiUtils::CreateSearchBoxL(*this, *iListBox, iStyle);
	                SizeChanged();
					iFindBox->SetFocus(ETrue, EDrawNow);
					return EKeyWasNotConsumed;
					// Non e' possibile xche' viene inserito un numero invece di una lettera...
	//				return OfferKeyEventL(aKeyEvent, aType);
					}
				return OfferKeyToFindBoxL(aKeyEvent, aType);
				}
			}
		}
	
	
	if ( iFocusControl != NULL
		&& iFocusControl->OfferKeyEventL( aKeyEvent, aType ) == EKeyWasConsumed )
		{
		return EKeyWasConsumed;
		}
	return CCoeControl::OfferKeyEventL( aKeyEvent, aType );
	}
				
// [[[ begin generated function: do not modify
/**
 *	Initialize each control upon creation.
 */				
void CRomPatcherPlusListBox::InitializeControlsL()
	{
	iListBox = new ( ELeave ) CAknSingleGraphicStyleListBox;
	iListBox->SetContainerWindowL( *this );
		{
		TResourceReader reader;
		iEikonEnv->CreateResourceReaderLC( reader, R_ROM_PATCHER_PLUS_LIST_BOX_LIST_BOX );
		iListBox->ConstructFromResourceL( reader );
		CleanupStack::PopAndDestroy(); // reader internal state
		}
	// the listbox owns the items in the list and will free them
	iListBox->Model()->SetOwnershipType( ELbmOwnsItemArray );
	
	// setup the icon array so graphics-style boxes work
	SetupListBoxIconsL();
	
	iListBox->SetListBoxObserver( this );
	AddListBoxEventHandlerL( 
			iListBox, 
			EEventEnterKeyPressed, 
			&CRomPatcherPlusListBox::HandleListBoxEnterKeyPressedL );
	AddListBoxEventHandlerL( 
			iListBox, 
			EEventItemClicked, 
			&CRomPatcherPlusListBox::HandleListBoxItemClickedL );
	AddListBoxEventHandlerL( 
			iListBox, 
			EEventItemDoubleClicked, 
			&CRomPatcherPlusListBox::HandleListBoxItemDoubleClickedL );
	
	// add list items
	AddListBoxResourceArrayItemL( R_ROM_PATCHER_PLUS_LIST_BOX_LISTBOX_ITEM1,
			EListBoxRompatcherplussvgIdleIndex );
	AddListBoxResourceArrayItemL( R_ROM_PATCHER_PLUS_LIST_BOX_LISTBOX_ITEM2,
			EListBoxRompatcherplussvgIdle_autoIndex );
	AddListBoxResourceArrayItemL( R_ROM_PATCHER_PLUS_LIST_BOX_LISTBOX_ITEM9,
			EListBoxRompatcherplussvgIdle_domainIndex );
	AddListBoxResourceArrayItemL( R_ROM_PATCHER_PLUS_LIST_BOX_LISTBOX_ITEM3,
			EListBoxRompatcherplussvgOkIndex );
	AddListBoxResourceArrayItemL( R_ROM_PATCHER_PLUS_LIST_BOX_LISTBOX_ITEM4,
			EListBoxRompatcherplussvgOk_autoIndex );
	AddListBoxResourceArrayItemL( R_ROM_PATCHER_PLUS_LIST_BOX_LISTBOX_ITEM10,
			EListBoxRompatcherplussvgOk_domainIndex );
	AddListBoxResourceArrayItemL( R_ROM_PATCHER_PLUS_LIST_BOX_LISTBOX_ITEM5,
			EListBoxRompatcherplussvgKoIndex );
	AddListBoxResourceArrayItemL( R_ROM_PATCHER_PLUS_LIST_BOX_LISTBOX_ITEM6,
			EListBoxRompatcherplussvgKo_autoIndex );
	AddListBoxResourceArrayItemL( R_ROM_PATCHER_PLUS_LIST_BOX_LISTBOX_ITEM11,
			EListBoxRompatcherplussvgKo_domainIndex );
	AddListBoxResourceArrayItemL( R_ROM_PATCHER_PLUS_LIST_BOX_LISTBOX_ITEM7,
			EListBoxRompatcherplussvgWarnIndex );
	AddListBoxResourceArrayItemL( R_ROM_PATCHER_PLUS_LIST_BOX_LISTBOX_ITEM8,
			EListBoxRompatcherplussvgWarn_autoIndex );
	AddListBoxResourceArrayItemL( R_ROM_PATCHER_PLUS_LIST_BOX_LISTBOX_ITEM12,
			EListBoxRompatcherplussvgWarn_domainIndex );
	
	iListBox->SetFocus( ETrue );
	iFocusControl = iListBox;
	
	}
// ]]] end generated function

/** 
 * Handle global resource changes, such as scalable UI or skin events (override)
 */
void CRomPatcherPlusListBox::HandleResourceChange( TInt aType )
	{
	CCoeControl::HandleResourceChange( aType );
	SetRect( iAvkonViewAppUi->View( TUid::Uid( ERomPatcherPlusListBoxViewId ) )->ClientRect() );
	// [[[ begin generated region: do not modify [Generated Contents]
	// ]]] end generated region [Generated Contents]
	
	}
				
/**
 *	Draw container contents.
 */				
void CRomPatcherPlusListBox::Draw( const TRect& aRect ) const
	{
	// [[[ begin generated region: do not modify [Generated Contents]
	CWindowGc& gc = SystemGc();
	gc.Clear( aRect );
	
	// ]]] end generated region [Generated Contents]
	
	}
				
// [[[ begin generated function: do not modify
/**
 *	Add a list box item to a list.
 */
void CRomPatcherPlusListBox::AddListBoxItemL( 
		CEikTextListBox* aListBox,
		const TDesC& aString )
	{
	CTextListBoxModel* model = aListBox->Model();
	CDesCArray* itemArray = static_cast< CDesCArray* > ( model->ItemTextArray() );
	itemArray->AppendL( aString );
	aListBox->HandleItemAdditionL();
	}

// ]]] end generated function

// [[[ begin generated function: do not modify
/**
 * Get the array of selected item indices, with respect to the list model.
 * The array is sorted in ascending order.
 * The array should be destroyed with two calls to CleanupStack::PopAndDestroy(),
 * the first with no argument (referring to the internal resource) and the
 * second with the array pointer.
 * @return newly allocated array, which is left on the cleanup stack;
 *	or NULL for empty list. 
 */
RArray< TInt >* CRomPatcherPlusListBox::GetSelectedListBoxItemsLC( CEikTextListBox* aListBox )
	{
	CAknFilteredTextListBoxModel* model = 
		static_cast< CAknFilteredTextListBoxModel *> ( aListBox->Model() );
	if ( model->NumberOfItems() == 0 )
		return NULL;
		
	// get currently selected indices
	const CListBoxView::CSelectionIndexArray* selectionIndexes =
		aListBox->SelectionIndexes();
	TInt selectedIndexesCount = selectionIndexes->Count();
	if ( selectedIndexesCount == 0 )
		return NULL;
		
	// copy the indices and sort numerically
	RArray<TInt>* orderedSelectedIndices = 
		new (ELeave) RArray< TInt >( selectedIndexesCount );
	
	// push the allocated array
	CleanupStack::PushL( orderedSelectedIndices );
	
	// dispose the array resource
	CleanupClosePushL( *orderedSelectedIndices );
	
	// see if the search field is enabled
	CAknListBoxFilterItems* filter = model->Filter();
	if ( filter != NULL )
		{
		// when filtering enabled, translate indices back to underlying model
		for ( TInt idx = 0; idx < selectedIndexesCount; idx++ )
			{
			TInt filteredItem = ( *selectionIndexes ) [ idx ];
			TInt actualItem = filter->FilteredItemIndex ( filteredItem );
			orderedSelectedIndices->InsertInOrder( actualItem );
			}
		}
	else
		{
		// the selection indices refer directly to the model
		for ( TInt idx = 0; idx < selectedIndexesCount; idx++ )
			orderedSelectedIndices->InsertInOrder( ( *selectionIndexes ) [ idx ] );
		}	
		
	return orderedSelectedIndices;
	}

// ]]] end generated function

// [[[ begin generated function: do not modify
/**
 * Delete the selected item or items from the list box.
 */
void CRomPatcherPlusListBox::DeleteSelectedListBoxItemsL( CEikTextListBox* aListBox )
	{
	CAknFilteredTextListBoxModel* model = 
		static_cast< CAknFilteredTextListBoxModel *> ( aListBox->Model() );
	if ( model->NumberOfItems() == 0 )
		return;
	
	RArray< TInt >* orderedSelectedIndices = GetSelectedListBoxItemsLC( aListBox );		
	if ( !orderedSelectedIndices )
		return;
		
	// Delete selected items from bottom up so indices don't change on us
	CDesCArray* itemArray = static_cast< CDesCArray* > ( model->ItemTextArray() );
	TInt currentItem = 0;
	
	for ( TInt idx = orderedSelectedIndices->Count(); idx-- > 0; ) 
		{
		currentItem = ( *orderedSelectedIndices )[ idx ];
		itemArray->Delete ( currentItem );
		}
	
	// dispose the array resources
	CleanupStack::PopAndDestroy();
	
	// dispose the array pointer
	CleanupStack::PopAndDestroy( orderedSelectedIndices );
	
	// refresh listbox's cursor now that items are deleted
	AknListBoxUtils::HandleItemRemovalAndPositionHighlightL(
		aListBox, currentItem, ETrue );
	}

// ]]] end generated function

// [[[ begin generated function: do not modify
/**
 *	Get the listbox.
 */
CAknSingleGraphicStyleListBox* CRomPatcherPlusListBox::ListBox()
	{
	return iListBox;
	}

// ]]] end generated function

// [[[ begin generated function: do not modify
/**
 *	Create a list box item with the given column values.
 */
void CRomPatcherPlusListBox::CreateListBoxItemL( TDes& aBuffer, 
		TInt aIconIndex,
		const TDesC& aMainText )
	{
	_LIT ( KStringHeader, "%d\t%S" );
	
	aBuffer.Format( KStringHeader(), aIconIndex, &aMainText );
	} 
				
// ]]] end generated function

// [[[ begin generated function: do not modify
/**
 *	Add an item to the list by reading the text items from the array resource
 *	and setting a single image property (if available) from an index
 *	in the list box's icon array.
 *	@param aResourceId id of an ARRAY resource containing the textual
 *	items in the columns
 *	@param aIconIndex the index in the icon array, or -1
 */
void CRomPatcherPlusListBox::AddListBoxResourceArrayItemL( TInt aResourceId, TInt aIconIndex )
	{
	CDesCArray* array = iCoeEnv->ReadDesCArrayResourceL( aResourceId );
	CleanupStack::PushL( array );
	// This is intended to be large enough, but if you get 
	// a USER 11 panic, consider reducing string sizes.
	TBuf<512> listString; 
	CreateListBoxItemL( listString, aIconIndex, ( *array ) [ 0 ] );
	AddListBoxItemL( iListBox, listString );
	CleanupStack::PopAndDestroy( array );
	} 
				
// ]]] end generated function

void CRomPatcherPlusListBox::AddListBoxItemL( TInt aIconIndex, const TDesC& aItemName)
	{
	TBuf<512> listString; 
	CreateListBoxItemL( listString, aIconIndex, aItemName );
	AddListBoxItemL( iListBox, listString );
	}

// [[[ begin generated function: do not modify
/**
 *	Set up the list's icon array.
 */
void CRomPatcherPlusListBox::SetupListBoxIconsL()
	{
	CArrayPtr< CGulIcon >* icons = NULL;		
	icons = new (ELeave) CAknIconArray( 12 );
	CleanupStack::PushL( icons );
	CGulIcon* icon;
	// for EListBoxRompatcherplussvgIdleIndex
	icon = LoadAndScaleIconL(
			KRomPatcherPlusSvgFile, EMbmRompatcherplussvgIdle, EMbmRompatcherplussvgIdle_mask,
			NULL, EAspectRatioPreserved );
	CleanupStack::PushL( icon );
	icons->AppendL( icon );
	CleanupStack::Pop( icon );
	// for EListBoxRompatcherplussvgIdle_autoIndex
	icon = LoadAndScaleIconL(
			KRomPatcherPlusSvgFile, EMbmRompatcherplussvgIdle_auto, EMbmRompatcherplussvgIdle_auto_mask,
			NULL, EAspectRatioPreserved );
	CleanupStack::PushL( icon );
	icons->AppendL( icon );
	CleanupStack::Pop( icon );
	// for EListBoxRompatcherplussvgIdle_domainIndex
	icon = LoadAndScaleIconL(
			KRomPatcherPlusSvgFile, EMbmRompatcherplussvgIdle_domain, EMbmRompatcherplussvgIdle_domain_mask,
			NULL, EAspectRatioPreserved );
	CleanupStack::PushL( icon );
	icons->AppendL( icon );
	CleanupStack::Pop( icon );
	// for EListBoxRompatcherplussvgOkIndex
	icon = LoadAndScaleIconL(
			KRomPatcherPlusSvgFile, EMbmRompatcherplussvgOk, EMbmRompatcherplussvgOk_mask,
			NULL, EAspectRatioPreserved );
	CleanupStack::PushL( icon );
	icons->AppendL( icon );
	CleanupStack::Pop( icon );
	// for EListBoxRompatcherplussvgOk_autoIndex
	icon = LoadAndScaleIconL(
			KRomPatcherPlusSvgFile, EMbmRompatcherplussvgOk_auto, EMbmRompatcherplussvgOk_auto_mask,
			NULL, EAspectRatioPreserved );
	CleanupStack::PushL( icon );
	icons->AppendL( icon );
	CleanupStack::Pop( icon );
	// for EListBoxRompatcherplussvgOk_domainIndex
	icon = LoadAndScaleIconL(
			KRomPatcherPlusSvgFile, EMbmRompatcherplussvgOk_domain, EMbmRompatcherplussvgOk_domain_mask,
			NULL, EAspectRatioPreserved );
	CleanupStack::PushL( icon );
	icons->AppendL( icon );
	CleanupStack::Pop( icon );
	// for EListBoxRompatcherplussvgKoIndex
	icon = LoadAndScaleIconL(
			KRomPatcherPlusSvgFile, EMbmRompatcherplussvgKo, EMbmRompatcherplussvgKo_mask,
			NULL, EAspectRatioPreserved );
	CleanupStack::PushL( icon );
	icons->AppendL( icon );
	CleanupStack::Pop( icon );
	// for EListBoxRompatcherplussvgKo_autoIndex
	icon = LoadAndScaleIconL(
			KRomPatcherPlusSvgFile, EMbmRompatcherplussvgKo_auto, EMbmRompatcherplussvgKo_auto_mask,
			NULL, EAspectRatioPreserved );
	CleanupStack::PushL( icon );
	icons->AppendL( icon );
	CleanupStack::Pop( icon );
	// for EListBoxRompatcherplussvgKo_domainIndex
	icon = LoadAndScaleIconL(
			KRomPatcherPlusSvgFile, EMbmRompatcherplussvgKo_domain, EMbmRompatcherplussvgKo_domain_mask,
			NULL, EAspectRatioPreserved );
	CleanupStack::PushL( icon );
	icons->AppendL( icon );
	CleanupStack::Pop( icon );
	// for EListBoxRompatcherplussvgWarnIndex
	icon = LoadAndScaleIconL(
			KRomPatcherPlusSvgFile, EMbmRompatcherplussvgWarn, EMbmRompatcherplussvgWarn_mask,
			NULL, EAspectRatioPreserved );
	CleanupStack::PushL( icon );
	icons->AppendL( icon );
	CleanupStack::Pop( icon );
	// for EListBoxRompatcherplussvgWarn_autoIndex
	icon = LoadAndScaleIconL(
			KRomPatcherPlusSvgFile, EMbmRompatcherplussvgWarn_auto, EMbmRompatcherplussvgWarn_auto_mask,
			NULL, EAspectRatioPreserved );
	CleanupStack::PushL( icon );
	icons->AppendL( icon );
	CleanupStack::Pop( icon );
	// for EListBoxRompatcherplussvgWarn_domainIndex
	icon = LoadAndScaleIconL(
			KRomPatcherPlusSvgFile, EMbmRompatcherplussvgWarn_domain, EMbmRompatcherplussvgWarn_domain_mask,
			NULL, EAspectRatioPreserved );
	CleanupStack::PushL( icon );
	icons->AppendL( icon );
	CleanupStack::Pop( icon );
	CleanupStack::Pop( icons );
	
	if ( icons != NULL )
		{
		iListBox->ItemDrawer()->ColumnData()->SetIconArray( icons );
		}
	}

// ]]] end generated function

// [[[ begin generated function: do not modify
/** 
 *	Handle commands relating to markable lists.
 */
TBool CRomPatcherPlusListBox::HandleMarkableListCommandL( TInt aCommand )
	{
	return EFalse;
	}

// ]]] end generated function

// [[[ begin generated function: do not modify
/**
 *	This routine loads and scales a bitmap or icon.
 *
 *	@param aFileName the MBM or MIF filename
 *	@param aBitmapId the bitmap id
 *	@param aMaskId the mask id or -1 for none
 *	@param aSize the TSize for the icon, or NULL to use real size
 *	@param aScaleMode one of the EAspectRatio* enums when scaling
 *
 */
CGulIcon* CRomPatcherPlusListBox::LoadAndScaleIconL( 
		const TDesC& aFileName,
		TInt aBitmapId, 
		TInt aMaskId, 
		TSize* aSize, 
		TScaleMode aScaleMode )
	{
	CFbsBitmap* bitmap;
	CFbsBitmap* mask;
	AknIconUtils::CreateIconL( bitmap, mask, aFileName, aBitmapId, aMaskId );
	
	TSize size;
	if ( aSize == NULL )
		{
		// Use size from the image header.  In case of SVG,
		// we preserve the image data for a while longer, since ordinarily
		// it is disposed at the first GetContentDimensions() or SetSize() call.
		AknIconUtils::PreserveIconData( bitmap );
		AknIconUtils::GetContentDimensions( bitmap, size );
		}
	else
		{
		size = *aSize;
		}
	
	AknIconUtils::SetSize( bitmap, size, aScaleMode );
	AknIconUtils::SetSize( mask, size, aScaleMode );
	
	if ( aSize == NULL )
		{
		AknIconUtils::DestroyIconData( bitmap );
		}
	
	return CGulIcon::NewL( bitmap, mask );
	}

// ]]] end generated function

/** 
 * Override of the HandleListBoxEventL virtual function
 */
void CRomPatcherPlusListBox::HandleListBoxEventL( 
		CEikListBox* aListBox,
		TListBoxEvent anEventType )
	{
	for (int i = 0; i < iListBoxEventDispatch.Count(); i++)
		{
		const TListBoxEventDispatch& currEntry = iListBoxEventDispatch[i];
		if ( currEntry.src == aListBox && currEntry.event == anEventType )
			{
			( this->*currEntry.handler )( aListBox, anEventType );
			break;
			}
		}
	}

/** 
 * Helper function to register MEikListBoxObserver event handlers
 */
void CRomPatcherPlusListBox::AddListBoxEventHandlerL( 
		CEikListBox* aListBox,
		TListBoxEvent anEvent,
		ListBoxEventHandler aHandler )
	{
	TListBoxEventDispatch entry;
	entry.src = aListBox;
	entry.event = anEvent;
	entry.handler = aHandler;
	TInt err = iListBoxEventDispatch.Append( entry );
	User::LeaveIfError( err );
	}

/**
 *	Handle the itemClicked event
 */
void CRomPatcherPlusListBox::HandleListBoxItemClickedL( 
		CEikListBox* aListBox,
		TListBoxEvent anEventType )
	{
	// Viene richiamato ogni volta che si fa click su un elem. della lista
	// Anche se e' quello selezionato
/*	iOldIndex++;
	if (iOldIndex < 10)
		Notes::Debug_InfoL(_L("Clicked Event"));*/
/*	TBuf<2> drive;
	FileUtils::CompleteWithCurrentDrive(drive);
	
	// Stranamente, quando viene avviato dalla Rom non viene generato EKeyDevice3
	TBool fromRom = (drive == _L("Z:") || drive == _L("z:"));
	if (GuiUtils::IsTouchSupportedL() && fromRom)
		{
		TKeyEvent event;
		event.iCode = EKeyDevice3;
		OfferKeyEventL(event, EEventKey);
		}
	
/*	if (GuiUtils::IsTouchSupportedL())
		{
	    if (iListBox->CurrentItemIndex() != iOldIndex)
			{
			iOldIndex = iListBox->CurrentItemIndex();
			return;
			}
		iOldIndex = iListBox->CurrentItemIndex();
		}
	TInt status = StatusPatch();
	if (status < 0)
		return ;
	if (status == EApplied || status == EAppliedAuto)
		this->RemovePatchL();
   	if (status == EIdle || status == EIdleAuto)
		this->ApplyPatchL();*/
	}

/**
 *	Handle the itemDoubleClicked event
 */
void CRomPatcherPlusListBox::HandleListBoxItemDoubleClickedL( 
		CEikListBox* aListBox,
		TListBoxEvent anEventType )
	{
	// Viene richiamato se si fa click sull'elemento selezionato
 	// Notes::Debug_InfoL(_L("Touch - DoubleClicked Event"));

	// Touch
	TInt i = GetOriginalIndex();
	if (i<0)
		return;
	TPatchStatus status = iStatusArray[i];
	if (status == EApplied)
		this->RemovePatchL();
	if (status == EIdle)
		this->ApplyPatchL();
	}

/**
 *	Handle the enterKeyPressed event
 */
void CRomPatcherPlusListBox::HandleListBoxEnterKeyPressedL( 
		CEikListBox* aListBox,
		TListBoxEvent anEventType )
	{
 	// Notes::Debug_InfoL(_L("NoTouch - EnterKey Event"));
	// implement enterKeyPressed event handler		
	
	// NON Touch
	TInt i = GetOriginalIndex();
	if (i<0)
		return;
	TPatchStatus status = iStatusArray[i];
	if (status == EApplied)
		this->RemovePatchL();
	if (status == EIdle)
		this->ApplyPatchL();
	}

