/*
 ============================================================================
 Name		: FortumoXML.cpp
 Author	  : Marco Bellino
 Version	 : 1.0
 Copyright   : FuzzyByte
 Description : CFortumoXml implementation
 ============================================================================
 */

#include "FortumoXML.h"
#include <UTF.h>
#include <FuzzyByte\Notes.h>
#include <AKNMESSAGEQUERYDIALOG.H>
#include <RomPatcherPlusGUI.rsg>
 
 
CFortumoXml::CFortumoXml()
	{
	// No implementation required
	}
 
CFortumoXml::~CFortumoXml()
	{
	Reset();
	iTmpMsg.Close();
	iAmount.Close();
	iCurrency.Close();
	iCountries.Close();
	iKeywords.Close();
	iShortcodes.Close();
	iTextLocal.Close();
	iTextEnglish.Close();
	}

CFortumoXml* CFortumoXml::NewLC()
	{
	CFortumoXml* self = new (ELeave) CFortumoXml();
	CleanupStack::PushL(self);
	self->ConstructL();
	return self;
	}

CFortumoXml* CFortumoXml::NewL()
	{
	CFortumoXml* self = CFortumoXml::NewLC();
	CleanupStack::Pop(); // self;
	return self;
	}

void CFortumoXml::ConstructL()
	{
	iTmpMsg.Create(2000);
	}

void CFortumoXml::Reset()
	{
	iTmpMsg.Zero();
	iAmount.ResetAndDestroy();
	iCurrency.ResetAndDestroy();
	iCountries.ResetAndDestroy();
	iKeywords.ResetAndDestroy();
	iShortcodes.ResetAndDestroy();
	iTextLocal.ResetAndDestroy();
	iTextEnglish.ResetAndDestroy();
	}

void CFortumoXml::OnStartDocumentL( const RDocumentParameters& /*aDocParam*/,
        TInt aErrorCode )
    {
    Reset();
    if ( KErrNone != aErrorCode )
    	return;
        // Do something here when the parser at the start of the document.
    }
 
void CFortumoXml::OnEndDocumentL( TInt aErrorCode )
    {
    if ( KErrNone != aErrorCode )
    	return;
    }


LOCAL_C void RemoveLastElemL(RPointerArray<HBufC>& array)
	{
	HBufC* last = array[array.Count()-1];
	array.Remove(array.Count()-1);
	delete last;	
	}

LOCAL_C void ReplaceLastElemL(RPointerArray<HBufC>& array, const TDesC8& attrVal)
	{
	RemoveLastElemL(array);
	HBufC* buf = CnvUtfConverter::ConvertToUnicodeFromUtf8L(attrVal);
	array.Append(buf);
	}


void CFortumoXml::OnStartElementL( const RTagInfo& aElement,
        const RAttributeArray& aAttributes, TInt aErrorCode )
    {
    if ( KErrNone != aErrorCode )
    	return;
        // Found start of an element, for example: "<tag>"
        // The name of the element is stored in aElement.LocalName().DesC().
 
        // Do something with the start of an element.
    const TDesC8& elementName = aElement.LocalName().DesC();
/*    if (elementName.MatchC(_L8("countries")) == 0)
    	{
		for (int i=0; i<aAttributes.Count(); i++)
			{
			RAttribute attr = aAttributes[i];
			const TDesC8& attrName = attr.Attribute().LocalName().DesC();
			const TDesC8& attrVal = attr.Value().DesC();
			if (attrName.MatchC(_L8("default")) == 0)
				{
				iDefCountry.Close();
				iDefCountry.Assign( CnvUtfConverter::ConvertToUnicodeFromUtf8L(attrVal) );
				}
			}
		return;
    	}*/
    if (elementName.MatchC(_L8("country")) == 0)
    	{
		// approved name
		for (int i=0; i<aAttributes.Count(); i++)
			{
			RAttribute attr = aAttributes[i];
			const TDesC8& attrName = attr.Attribute().LocalName().DesC();
			const TDesC8& attrVal = attr.Value().DesC();
			if (attrName.MatchC(_L8("name")) == 0)
				{
				HBufC* buf = CnvUtfConverter::ConvertToUnicodeFromUtf8L(attrVal);
				iCountries.Append(buf);
				iKeywords.Append(KNullDesC().Alloc());
				iShortcodes.Append(KNullDesC().Alloc());
				iTextLocal.Append(KNullDesC().Alloc());
				iTextEnglish.Append(KNullDesC().Alloc());
				iCurrency.Append(KNullDesC().Alloc());
				iAmount.Append(KNullDesC().Alloc());
				}
			if (attrName.MatchC(_L8("approved")) == 0)
				{
				if (attrVal.MatchC(_L8("false")) == 0)
					{
					RemoveLastElemL(iCountries);
					RemoveLastElemL(iKeywords);
					RemoveLastElemL(iShortcodes);
					RemoveLastElemL(iTextLocal);
					RemoveLastElemL(iTextEnglish);
					RemoveLastElemL(iCurrency);
					RemoveLastElemL(iAmount);
					iIgnoreCountry = ETrue;
					return;
					}
				iIgnoreCountry = EFalse;
				}
			}
		return;
    	}
	if (iIgnoreCountry)
		return;
    
	if (elementName.MatchC(_L8("price")) == 0)
    	{
		for (int i=0; i<aAttributes.Count(); i++)
			{
			//		amount="5.00" currency="EUR"
			RAttribute attr = aAttributes[i];
			const TDesC8& attrName = attr.Attribute().LocalName().DesC();
			const TDesC8& attrVal = attr.Value().DesC();
			if (attrName.MatchC(_L8("amount")) == 0)
				{
				ReplaceLastElemL(iAmount, attrVal);
				}
			if (attrName.MatchC(_L8("currency")) == 0)
				{
				ReplaceLastElemL(iCurrency, attrVal);
				}
			}
		return;
    	}
	
    if (elementName.MatchC(_L8("message_profile")) == 0)
    	{
		// keyword shortcode
		for (int i=0; i<aAttributes.Count(); i++)
			{
			RAttribute attr = aAttributes[i];
			const TDesC8& attrName = attr.Attribute().LocalName().DesC();
			const TDesC8& attrVal = attr.Value().DesC();
			if (attrName.MatchC(_L8("keyword")) == 0)
				{
				ReplaceLastElemL(iKeywords, attrVal);
				// save "TXT5 RPP" 
				}
			if (attrName.MatchC(_L8("shortcode")) == 0)
				{
				ReplaceLastElemL(iShortcodes, attrVal);
				// save "17163"				
				}
			}
		return;
    	}
    if (elementName.MatchC(_L8("local")) == 0)
    	{
		iTmpMsg.Zero();
		iGetLocal = ETrue;
		return;
    	}
    if (elementName.MatchC(_L8("english")) == 0)
    	{
		iTmpMsg.Zero();
		iGetEnglish = ETrue;
		return;
    	}
    }
 

LOCAL_C void ShowLongMessageDialogL(const TDesC& title, const TDesC& aMsg)
	{	
	CAknMessageQueryDialog *dialog = CAknMessageQueryDialog::NewL( CONST_CAST(TDesC&, aMsg) );
	dialog->PrepareLC(R_LONGTEXT_DIALOG);
	dialog->SetHeaderTextL(title);
	dialog->RunLD();
	}


void CFortumoXml::OnContentL( const TDesC8 &aBytes, TInt aErrorCode )
    {
    if ( KErrNone != aErrorCode )
    	return;
	if (iIgnoreCountry)
		return;
    if (iGetLocal)
    	{
		iTmpMsg.Append(aBytes);
		// save "Hinta: €5,00 Asiakaspalvelu: m.bellino@symbian-toys.com Palvelun toteutus: fortumo.fi"
    	}
    if (iGetEnglish)
    	{
		iTmpMsg.Append(aBytes);
		// save "Price: €5.00 Support: m.bellino@symbian-toys.com Mobile Payment by fortumo.com"
    	}
    }


void CFortumoXml::OnEndElementL( const RTagInfo &aElement, TInt aErrorCode )
    {
    if ( KErrNone != aErrorCode )
    	return;
	if (iIgnoreCountry)
		return;
	// Found the end of an element, for example: "</tag>"
	// The name of the element is stored in aElement.LocalName().DesC().

	// Do something with the end of an element.
    const TDesC8& elementName = aElement.LocalName().DesC();
    if (elementName.MatchC(_L8("local")) == 0)
    	{
		ReplaceLastElemL(iTextLocal, iTmpMsg);
		iTmpMsg.Zero();
		iGetLocal = EFalse;
		return;
    	}
    if (elementName.MatchC(_L8("english")) == 0)
    	{
		// --- Potrebbe non esistere ---
		ReplaceLastElemL(iTextEnglish, iTmpMsg);
		iTmpMsg.Zero();
		iGetEnglish = EFalse;
		return;
    	}
    }
 
void CFortumoXml::OnStartPrefixMappingL( const RString& /*aPrefix*/, const RString& /*aUri*/, TInt aErrorCode )
    {
    }
 
void CFortumoXml::OnEndPrefixMappingL( const RString& /*aPrefix*/, TInt aErrorCode )
    {
    }
 
void CFortumoXml::OnIgnorableWhiteSpaceL( const TDesC8& /*aBytes*/, TInt aErrorCode )
    {
    }
 
void CFortumoXml::OnSkippedEntityL( const RString& /*aName*/, TInt aErrorCode )
    {
    }
 
void CFortumoXml::OnProcessingInstructionL( const TDesC8& /*aTarget*/, const TDesC8& /*aData*/, TInt aErrorCode )
    {
    }
 
void CFortumoXml::OnError( TInt aErrorCode )
    {
    }
 
TAny* CFortumoXml::GetExtendedInterface( const TInt32 aUid )
	{
	return NULL;
	}
