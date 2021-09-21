/*
 ============================================================================
 Name		: RomPatcherAuto.cpp
 Author	  : Marco Bellino
 Copyright   : Guardian-Mobile.com
 Description : Exe source file
 ============================================================================
 */
//  Include Files  
#include "RomPatcherAuto.h"
#include <e32base.h> 
#include <e32std.h>  
#include <e32cons.h>			// Console
#include <FuzzyByte\RP\RP_Types.H>
#include <FuzzyByte\Notes.H>
#include <FuzzyByte\Processes.H>
#include <FuzzyByte\Utils.h>
#include <FuzzyByte\RP\rompatcherengine.H>
#include <mmf\common\mmfcontrollerpluginresolver.h>
#include <FuzzyByte\RP\Data.h>
#include <APGCLI.H>
//#include "..\..\RomPatcherPlusGUI\inc\RomPatcherPlusAppUi.h"
//  Constants  
//  Global Variables
_LIT(KAutoStartName, "RomPatcherPlus_AutoStart");

LOCAL_C void DoStartL()
	{
	if (!Processes::RenameIfNotRunning(KAutoStartName))
		{
		//__FLOG(_T8("Already Running"));
		return;
		}

	// Create active scheduler (to run active objects)
	CActiveScheduler* scheduler = new (ELeave) CActiveScheduler();
	CleanupStack::PushL(scheduler);
	CActiveScheduler::Install(scheduler);

	// Leggi il file di configurazione e applica le patch...
	RFs fs;
	User::LeaveIfError(fs.Connect());
	CleanupClosePushL(fs);
	
/*	TInt maxTime = 150000;	// ANTICRACK (249F0)
	TUint32 ntick = User::NTickCount() / maxTime; 
#ifdef _DEBUG
	ntick = 0;
#endif*/

	TUint32 sid = RProcess().SecureId().iId;
	CData* data = CData::NewLC(); 
	data->LoadData(fs, KAUTOFILE);

	// Se la GUI non e' mai stata avviata esce... altrimenti il crc schianta...
	if (data->iNameGui.Length() == 0)
		{
		CleanupStack::PopAndDestroy(data);
		CleanupStack::PopAndDestroy(&fs);
		User::After(5 * 1000000);
		CleanupStack::PopAndDestroy(scheduler);
		return;
		}

	// se il domainsrv viene disattivato, potra' essere attivato solo dal auto
	// se auto viene disattivato influenza solo auto
	TBool skipPatches = EFalse;
	
	if (sid == 0xE7771129)
		{  
		RProcess proc;
		TParsePtrC parse(proc.FileName());
		skipPatches = !data->iDoneAuto; 
		data->iNameAuto = parse.Name();
		} else
		{
		//	if (sid == 0xE7771130)
		skipPatches = !data->iDoneAutoDomain;
		}
	
	TUint32 computedSid = data->CrcNamesL();
	TUint32 goodCrc = (computedSid);	// ANTICRACK
	// 0x62BC
	
	// 0xE7771129 deve diventare A4A3
	if (!skipPatches)
		{
		if (sid == 0xE7771130)
			data->iDoneAutoDomain = EFalse;
		if (sid == 0xE7771129)
			data->iDoneAuto = EFalse;
		data->SaveDataL(fs, KAUTOFILE);

		RPointerArray<HBufC> patchList;
		CleanupResetAndDestroyPushL(patchList);
		RArray<TAutoStatus> autoStatus;
		CleanupClosePushL(autoStatus);
		RArray<TPatchStatus> patchStatus;
		CleanupClosePushL(patchStatus);

		RomPatcherEngine::GetPatchesStatusL(data, fs, patchList, patchStatus, autoStatus);
		// TODO:
		// data->iBadGuiCrc = data->iCrcGui % goodCrc;	// ANTICRACK
		data->SaveDataL(fs, KAUTOFILE);
		
		for (TInt i = 0; i < patchList.Count(); i++)
			{
			if (autoStatus[i] == ENoAuto)
				continue;
			if (patchStatus[i] == EApplied)
				continue;
			if ((autoStatus[i] == EAutoStartDomain && sid == 0xE7771130) ||
				(autoStatus[i] == EAutoStart && sid == 0xE7771129))
				{
				TPtrC patchName = patchList[i]->Des();
				TRAP_IGNORE( RomPatcherEngine::ApplyPatchL(fs, patchName) );
				}
			} 
		CleanupStack::PopAndDestroy(&patchStatus);
		CleanupStack::PopAndDestroy(&autoStatus);
		CleanupStack::PopAndDestroy(&patchList);
		   
		if (sid == 0xE7771130)
			{
			data->iDoneAutoDomain = ETrue;
			data->SaveDataL(fs, KAUTOFILE);
			}
		}
	else
		{
		// AutoDomain disattivo... non fa nulla
		// Oppure Auto disattivo... non fa nulla
		}
 
	
	// TODO: Donate by Sms... a quanto pare c'e' qualche problema con la connessione... resta pendente.
	// TODO: Inoltre premendo annulla e uscendo il programma va in crisi...
	
	if (sid == 0xE7771129)
		{
		// Verifica checksum GUI usando %
		// Notes::Debug_NotifyL(_L("CRC"), _L("Names:%x"), computedSid);
		Notes::Debug_NotifyL(_L("CRC"), _L("Gui:%x"), data->iCrcGui); // 0x43F7
		Notes::Debug_NotifyL(_L("CRC"), _L("Comp:%x"), goodCrc); // 0x43F7
		}/**/

	if (sid == computedSid)	// 0xE7771129
		{
		TBool gotFault = (skipPatches || !data->iDoneAutoDomain);
		data->iDoneAutoDomain = data->iDoneAuto =  ETrue;
		data->SaveDataL(fs, KAUTOFILE);
		if (gotFault)
			Notes::Debug_NotifyL(_L("RP+ Fault Recovery"),
					_L("Some Patches Failed to Start"));
		}/**/
	CleanupStack::PopAndDestroy(data);
	CleanupStack::PopAndDestroy(&fs);

	//	CleanupStack::PopAndDestroy( &notif);g

	// Il Delay e' necessario per il 5500 perche' se si chiude troppo presto non rileva che il programma e' stato avviato e mostra un errore.
	// Ad ogni modo, il resto va avanti... e' solo lo starter che attende

	// Se e' partito da DomainSrv non attende il delay...

	User::After(5 * 1000000);

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

