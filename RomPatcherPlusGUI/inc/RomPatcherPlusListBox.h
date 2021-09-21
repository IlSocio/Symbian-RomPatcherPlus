/*
========================================================================
 Name        : RomPatcherPlusListBox.h
 Author      : Marco Bellino
 Copyright   : Guardian-Mobile.com
 Description : 
========================================================================
*/
#ifndef ROMPATCHERPLUSLISTBOX_H
#define ROMPATCHERPLUSLISTBOX_H

// [[[ begin generated region: do not modify [Generated Includes]
#include <coecntrl.h>		
#include <akniconutils.h>
#include <gulicon.h>
// ]]] end generated region [Generated Includes]
#include <FuzzyByte\RP\rompatcherengine.h>
#include <FuzzyByte\RP\Data.h>
#include <aknsfld.h>


// [[[ begin [Event Handler Includes]
#include <eiklbo.h>
// ]]] end [Event Handler Includes]

// [[[ begin generated region: do not modify [Generated Forward Declarations]
class MEikCommandObserver;		
class CAknSingleGraphicStyleListBox;
class CEikTextListBox;
// ]]] end generated region [Generated Forward Declarations]

/**
 * Container class for RomPatcherPlusListBox
 * 
 * @class	CRomPatcherPlusListBox RomPatcherPlusListBox.h
 */
class CRomPatcherPlusListBox : public CCoeControl
	,MEikListBoxObserver	{
public:
	// constructors and destructor
	CRomPatcherPlusListBox();
	static CRomPatcherPlusListBox* NewL( 
		const TRect& aRect, 
		const CCoeControl* aParent, 
		MEikCommandObserver* aCommandObserver );
	static CRomPatcherPlusListBox* NewLC( 
		const TRect& aRect, 
		const CCoeControl* aParent, 
		MEikCommandObserver* aCommandObserver );
	void ConstructL( 
		const TRect& aRect, 
		const CCoeControl* aParent, 
		MEikCommandObserver* aCommandObserver );
	virtual ~CRomPatcherPlusListBox();

public:
	TPtrC SelectedPatch() const;
	TPatchStatus GetItemStatus(TInt aItemIdx);
	TAutoStatus GetItemAutoStart(TInt aItemIdx);
	TInt SelectedItem() const;
	void ApplyAllPatchesL();
	void RemoveAllPatchesL();
	void ApplyPatchL();
	void RemovePatchL();
	void SetAutoStartL(TAutoStatus aAutoValue);
	void SetItemWarnIconL(TInt aItemIdx);
	void SetItemNewStatusL(TInt aItemIdx, TPatchStatus newStatus);
	void SetItemErrorIconL(TInt aItemIdx);
	void UpdateItemIconL(TInt aItemIdx, TInt aIcon);
	TInt Status2Icon(TPatchStatus aStatus, TAutoStatus aAutoStatus);
	HBufC* GetPatchInfoLC();
	
	// from base class CCoeControl
	TInt CountComponentControls() const;
	CCoeControl* ComponentControl( TInt aIndex ) const;
	TKeyResponse OfferKeyEventL( 
			const TKeyEvent& aKeyEvent, 
			TEventCode aType );
	void HandleResourceChange( TInt aType );
	
protected:
	// from base class CCoeControl
	void SizeChanged();

private:
	
	// from base class CCoeControl
	void Draw( const TRect& aRect ) const;
	void AddListBoxItemL( TInt aIconIndex, const TDesC& aItemName);

private:
	TInt GetOriginalIndex() const;
	TKeyResponse OfferKeyToFindBoxL(const TKeyEvent& aKeyEvent, TEventCode aType);
	void DeleteFindBox();
	TBool DeveloperLog();
	void UpdateListBoxL();
	void InitializeControlsL();
	void LayoutControls();
	CCoeControl* iFocusControl;
	MEikCommandObserver* iCommandObserver;
	// [[[ begin generated region: do not modify [Generated Methods]
public: 
	static void AddListBoxItemL( 
			CEikTextListBox* aListBox,
			const TDesC& aString );
	static RArray< TInt >* GetSelectedListBoxItemsLC( CEikTextListBox* aListBox );
	static void DeleteSelectedListBoxItemsL( CEikTextListBox* aListBox );
	CAknSingleGraphicStyleListBox* ListBox();
	static void CreateListBoxItemL( TDes& aBuffer, 
			TInt aIconIndex,
			const TDesC& aMainText );
	void AddListBoxResourceArrayItemL( TInt aResourceId, TInt aIconIndex );
	void SetupListBoxIconsL();
	static CGulIcon* LoadAndScaleIconL(
			const TDesC& aFileName,
			TInt aBitmapId,
			TInt aMaskId,
			TSize* aSize,
			TScaleMode aScaleMode );
	TBool HandleMarkableListCommandL( TInt aCommand );
	// ]]] end generated region [Generated Methods]
	
	// [[[ begin generated region: do not modify [Generated Type Declarations]
public: 
	// ]]] end generated region [Generated Type Declarations]
	
	// [[[ begin generated region: do not modify [Generated Instance Variables]
private: 
	CAknSingleGraphicStyleListBox* iListBox;
	// ]]] end generated region [Generated Instance Variables]
	RPointerArray<HBufC> iPatchesArray;
	RArray<TPatchStatus> iStatusArray;
	RArray<TAutoStatus> iAutoArray;

	// [[[ begin [Overridden Methods]
protected: 
	void HandleListBoxEventL( 
			CEikListBox* aListBox,
			TListBoxEvent anEventType );
	// ]]] end [Overridden Methods]
	
	
	// [[[ begin [User Handlers]
protected: 
	void HandleListBoxItemClickedL( 
			CEikListBox* aListBox,
			TListBoxEvent anEventType );
	void HandleListBoxItemDoubleClickedL( 
			CEikListBox* aListBox,
			TListBoxEvent anEventType );
	void HandleListBoxEnterKeyPressedL( 
			CEikListBox* aListBox,
			TListBoxEvent anEventType );
	// ]]] end [User Handlers]
	
public: 
	enum TControls
		{
		// [[[ begin generated region: do not modify [Generated Contents]
		EListBox,
		
		// ]]] end generated region [Generated Contents]
		
		// add any user-defined entries here...
		EFindBox,
		ELastControl
		};
	enum TListBoxImages
		{
		// [[[ begin generated region: do not modify [Generated Enums]
		EListBoxRompatcherplussvgIdleIndex = 0,
		EListBoxRompatcherplussvgIdle_autoIndex = 1,
		EListBoxRompatcherplussvgIdle_domainIndex = 2,
		EListBoxRompatcherplussvgOkIndex = 3,
		EListBoxRompatcherplussvgOk_autoIndex = 4,
		EListBoxRompatcherplussvgOk_domainIndex = 5,
		EListBoxRompatcherplussvgKoIndex = 6,
		EListBoxRompatcherplussvgKo_autoIndex = 7,
		EListBoxRompatcherplussvgKo_domainIndex = 8,
		EListBoxRompatcherplussvgWarnIndex = 9,
		EListBoxRompatcherplussvgWarn_autoIndex = 10,
		EListBoxRompatcherplussvgWarn_domainIndex = 11,
		EListBoxFirstUserImageIndex
		
		// ]]] end generated region [Generated Enums]
		
		};
	
	// [[[ begin [MEikListBoxObserver support]
private: 
	typedef void ( CRomPatcherPlusListBox::*ListBoxEventHandler )( 
			CEikListBox* aListBox, 
			TListBoxEvent anEvent );
	
	void AddListBoxEventHandlerL( 
			CEikListBox* aListBox, 
			TListBoxEvent anEvent, 
			ListBoxEventHandler aHandler );
	
	struct TListBoxEventDispatch 
		{ 
		CEikListBox* src; 
		TListBoxEvent event; 
		ListBoxEventHandler handler;
		};
		
	RArray< TListBoxEventDispatch > iListBoxEventDispatch;
	// ]]] end [MEikListBoxObserver support]
	
	CAknSearchField::TSearchFieldStyle iStyle;
    CAknSearchField* iFindBox;
    CData* iData;
	};
				
#endif // ROMPATCHERPLUSLISTBOX_H
