// f32test\testusbcldd\src\dPatcherchannel.cpp
//
// Copyright (c) 2004-2004 Symbian Ltd.  All rights reserved.
//

#include "patcherdev.h"
#include "patcherc.h"
#include <e32rom.h>
//#include <platform\memmodel\epoc\platform.h>	

// TODO: rileva lo UID del chiamante... se e' != da RP+ corrompe qualcosa
// TODO: rileva se RP+ e' in memoria... se non c'e' corrompe qualcosa


// The EKA1 base class needs a DLogicalDevice* for its constructor
DPatcherChannel::DPatcherChannel()
	{	
	iClient = &Kern::CurrentThread();
	iClient->Open();
	} 

DPatcherChannel::~DPatcherChannel()
	{
    Kern::SafeClose((DObject*&)iClient,NULL);
    }
	
TInt DPatcherChannel::DoCreate(TInt , const TDesC8*, const TVersion& )
    {
    return KErrNone;
    }


TInt DPatcherChannel::AllocShadowPage(TLinAddr ptrStartAddr, TUint32 len) const
	{
	TUint32 pageSize = Kern::RoundToPageSize(1);		// 0x1000
	TUint32 pageMask = pageSize-1;						// 0x0FFF
	TLinAddr ptrEndAddr = ptrStartAddr + len - 1;
	ptrStartAddr &= ~pageMask;	
	ptrEndAddr &= ~pageMask;

	// Just to be sure that the correct page will be allocated
//	ptrStartAddr++;
//	ptrEndAddr++;
	
	TInt err = KErrNone;
	NKern::ThreadEnterCS();
	for (TLinAddr i=ptrStartAddr; i<=ptrEndAddr; i+=pageSize)
		{
			err |= Epoc::AllocShadowPage( i );
			if (err == KErrArgument)
				err = KErrNone;
			if (err == KErrAlreadyExists)
				err = KErrNone;
			// KErrArgument   not a valid XIP ROM address
			// KErrNoMemory   insufficient free RAM Memory
			// KErrAlreadyExists)   page already shadowed in RAM
		}	
	NKern::ThreadLeaveCS();
	return err;
	}


TInt DPatcherChannel::FreeShadowPage(TLinAddr ptrStartAddr, TUint32 len) const
	{
	TUint32 pageSize = Kern::RoundToPageSize(1);		// 0x1000
	TUint32 pageMask = pageSize-1;						// 0x0FFF
	TLinAddr ptrEndAddr = ptrStartAddr + len - 1;
	ptrStartAddr &= ~pageMask;	
	ptrEndAddr &= ~pageMask;
	
	TInt result = KErrNone;
	NKern::ThreadEnterCS();
	for (TLinAddr i=ptrStartAddr; i<=ptrEndAddr; i+=pageSize)
		{
		result |= Epoc::FreeShadowPage( i );
		}
	NKern::ThreadLeaveCS();
	return result;
	}




TInt DPatcherChannel::MemCopy(TLinAddr ptrDest, TLinAddr ptrSrc, TInt len) const
	{
	TInt result = KErrNone;
	NKern::ThreadEnterCS();	
	memcpy((TUint8 *)ptrDest, (TUint8 *) ptrSrc, len);	
	NKern::ThreadLeaveCS();
	return result;
	}


TInt DPatcherChannel::AllocShadowAndCopy(TLinAddr ptrDest, TLinAddr ptrSrc, TInt len) const
	{
	TInt result = AllocShadowPage(ptrDest, len);
	if (result != KErrNone)
		return result;
		
	NKern::ThreadEnterCS();
	//result = Epoc::CopyToShadowMemory(ptrDest, ptrSrc, len);
	// Restituisce un KErrArgument se len supera i 32 bytes...
	TInt BUF_SIZE = 32;
	for (TInt offset = 0; offset < len; offset+=BUF_SIZE)
		{
		TInt leftToRead = (len - offset);
		TInt bufLen = leftToRead;
		if (bufLen > BUF_SIZE)
			bufLen = BUF_SIZE;
		
//**** Usando CopyToShadow non viene caricato il driver su alcuni device (E63)... Riportando errore -5 KErrNotSupported
//		result |= Epoc::CopyToShadowMemory(ptrDest+offset, ptrSrc+offset, bufLen);
//**** Usando memcpy viene generato Kern-Exec3 error su alcuni device (OmniaHD)
		memcpy((TUint8 *)ptrDest+offset, (TUint8 *) ptrSrc+offset, bufLen);	
		}		
	NKern::ThreadLeaveCS();
	
	return result;
	}


TInt DPatcherChannel::CopyFromTo(TLinAddr ptrDest, TLinAddr ptrSrc, TInt len) const
	{
	const TRomHeader& hdr = Epoc::RomHeader();
	if ((ptrDest >= hdr.iRomBase) && (ptrDest < hdr.iRomBase + hdr.iUncompressedSize))
	{
		// Write in Rom...
		return AllocShadowAndCopy(ptrDest, ptrSrc, len);
	}
	return MemCopy(ptrDest, ptrSrc, len);
	}


TInt DPatcherChannel::Request(TInt aFunction, TAny* a1, TAny* a2)
	{
	// Viene richiamata quando parte RomPatcher con parametro 0
	TInt r=KErrNone;

	switch (aFunction)
		{
		case RDevPatcherClient::EControlReadInt:
			{
			// TLinAddr& ptrAddr = *(TLinAddr*)a1;
			// ptrAddr contiere il valore passato come parametro
			
			// TLinAddr* ptrAddr = (TLinAddr*)a1;
			// ptrAddr punta al valore passato come parametro
			// *ptrAddr contiere il valore passato come parametro
			
			TLinAddr& ptrAddr = *((TLinAddr*)a1);
			TUint32& val = *((TUint32*)a2);			
			if (ptrAddr == 0)
				return KErrNone;

			r = CopyFromTo((TLinAddr)&val, ptrAddr, 4);
			break;
			}
		case RDevPatcherClient::EControlRead:
			{
			TLinAddr& ptrAddr = *((TLinAddr*)a1);
			TDes8& bufData = *((TDes8*)a2);
			if (ptrAddr == 0)
				return KErrNone;
			if (bufData.Length() > 0)
				{
				r = CopyFromTo((TLinAddr)bufData.Ptr(), ptrAddr, bufData.Length());
				}
			break;
			}
		case RDevPatcherClient::EControlFreePage:
			{
			TLinAddr& ptrAddr = *((TLinAddr*)a1);
			TInt& len = *((TInt*)a2);
			if (ptrAddr == 0)
				return KErrNone;

			r = FreeShadowPage(ptrAddr, len);
			break;
			}
		case RDevPatcherClient::EControlWriteInt:
			{
			TLinAddr& ptrDestAddr = *((TLinAddr*)a1);
			TUint32& data = *((TUint32*)a2);
			if (ptrDestAddr == 0)
				return KErrNone;
			
			r = CopyFromTo(ptrDestAddr, (TLinAddr) &data, 4);
			
			// 108 = iDebugMask[0]
			// 124 = iDebugMask[7]
			// 138 = iSignature[0]
			// 13C = iSignature[1]
			// 148 = iDisabledCapabilities...
//			CopyFromTo( (TLinAddr) &Kern::SuperPage().iDisabledCapabilities, (TLinAddr)&newVal, 4);
			break;
			}
		case RDevPatcherClient::EControlWrite:
			{
			TLinAddr& ptrDestAddr = *((TLinAddr*)a1);
			TDes8& bufData = *((TDes8*)a2);
			if (ptrDestAddr == 0)
				return KErrNone;
			
			// La ROM e' suddivisa in pagine da 0x1000 bytes ciascuna
			// 0x83400000 - 0x83400FFF
			// 0x83401000 - 0x83401FFF
			// 0x83402000 - 0x83402FFF
			
			//			val = Kern::RoundToPageSize(0x83400FFF);	=> 0x83401000
			//			val = Kern::RoundToPageSize(0x83500000);	=> 0x83500000
			//			val = Kern::RoundToPageSize(0x83500001);    => 0x83501000

			// Alloca le varie ShadowPage che servono per scrivere questi dati...
			
			r = CopyFromTo( ptrDestAddr, (TLinAddr)bufData.Ptr(), bufData.Length());
			
			// CodeModifier 
			// DebugSupport::ModifyCode()
			
			// Su ArmV7 le pagine vengono tutte rese shadow e freezate con Epoc::FreezeShadowPage()
			// La Freeze fa panicare se si tenta di modificare il codice con memcpy o con CopyToShadowMemory
			break;
			}
		case RDevPatcherClient::EControlGetSuperPageAddr:
			{		
			TLinAddr& val = *((TLinAddr*)a1);
			TLinAddr* spage = (TLinAddr*) &Kern::SuperPage();
			val = (TLinAddr)spage;			
			break;
			}
//		executeInSupervisorMode()
			
		}/**/
	return r;
	}

