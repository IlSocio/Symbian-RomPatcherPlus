
/*
 ============================================================================
 Name		: RomPatcherEngine.cpp
 Author	  : Marco Bellino
 Copyright   : Guardian-Mobile.com
 Description : RomPatcherEngine DLL source
 ============================================================================
 */

//  Include Files  

#include <mmf\common\mmfcontrollerpluginresolver.h>
#include <bautils.h> 
#include <f32file.h>
#include <s32file.h> 
#include <e32rom.h>   
#include <hal.h>  
#include <SysUtil.h>  

#include "RomPatcherEngine.h"	// RomPatcherEngine
#include "RomPatcherEngine.pan"	// panic codes
#include "AbstractLine.h"
#include "LineEmpty.h"
#include "LineComment.h"
#include "LineAbs.h"
#include "LineSnr.h"
#include "LineRel.h"
#include "LineOrdSnr.h"
#include "LineOrdRel.h"
#include "LineReturn.h"
#include "LineInfo.h"
#include "LineError.h"
#include "LineSuper.h"

#include <FuzzyByte\RP\patcherc.h>
#include <FuzzyByte\Utils.h> 
#include <FuzzyByte\StringUtils.h> 
#include <FuzzyByte\RStringBuf.h> 
#include <FuzzyByte\FileUtils.h>
#include "Data.h" 


#ifdef __WINSCW__
_LIT(KUNPATCHPATH, "c:\\RPPlusData\\");
#else
_LIT(KUNPATCHPATH, "d:\\RPPlusData\\");
#endif


#include <FuzzyByte\Notes.h>

_LIT(KLDDNameMemCopy, "patcher.ldd");
_LIT(KLDDNameShadow, "patcherShadow.ldd");
_LIT(KLDDNameS3, "patcherS3.ldd");





void RomPatcherEngine::FreeDevice()
	{
	User::FreeLogicalDevice(KDevName);	
	}

TInt RomPatcherEngine::LoadDeviceL()
	{
#ifdef __WINSCW__
	return KErrNone;
#endif
	
	RFs fs;
	User::LeaveIfError( fs.Connect() );
	CleanupClosePushL(fs);
	TUint major;
	TUint minor;
	Utils::GetS60PlatformL(fs, major, minor);
	CleanupStack::PopAndDestroy(&fs);
	
	if (major >= 5 && minor >= 2)
		{
		// S^3
		FreeDevice();
		TInt res = User::LoadLogicalDevice(KLDDNameS3);
		if (res == KErrNotSupported)
			{
			FreeDevice();
			}
		return res;
		}
	
	FreeDevice();
	TInt res = User::LoadLogicalDevice(KLDDNameShadow);
	if (res == KErrNotSupported)
		{
		FreeDevice();
		res = User::LoadLogicalDevice(KLDDNameMemCopy);
		}
	return res;
	}


TLinAddr RomPatcherEngine::SuperPageL()
	{
#ifdef __WINSCW__
	return 0xC0000000;
#endif
	TInt ris = 0;
	TUint32 superPage;
	
	User::LeaveIfError( LoadDeviceL() );
	RDevPatcherClient cli; 
	User::LeaveIfError( cli.Open() );
	ris = cli.GetSuperPage(superPage);
	cli.Close();
	FreeDevice();
	
    if (ris != KErrNone)
    	User::Leave(ris);
	return superPage; 
	}


_LIT(KINFO,  "info");
_LIT(KERROR, "error");
_LIT(KRETURN,"return");
_LIT(KREL, "rel");
_LIT(KABS, "abs");
_LIT(KSNR, "snr");
_LIT(KORDREL, "ord_rel");
_LIT(KORDSNR, "ord_snr");
_LIT(KSUP, "+superpage");


 
CAbstractLine* RomPatcherEngine::ParseLineL(const TDesC& aLine)
	{
	TPtrC line = StringUtils::Trim(aLine);
	if (line.Length() == 0)
		return CLineEmpty::NewL();
	if (line[0] == '\r')
		return CLineEmpty::NewL();
	if (line[0] == '\n')
		return CLineEmpty::NewL();
	
	//
	// Addr, isSearch, oriBytes, newBytes
	//
	// *           +superpage:00000148:1e:10
	// *           abs:c0000148:1e:10
	// *           rel:sys\bin\MusicShopApp.exe:00000000:7a:79
	//             snr:sys\bin\efile.exe:28D102980022002F287000D1:28D118200022002F287000D1
	//             +rombase:00000000:1e:10
	//             rel:sys\bin\MusicShopApp.exe:00000000:??:79		// Ignore byte
	CStringTokenizer* st = CStringTokenizer::NewLC();
	st->InitializeL(line);
	st->AddCharsTokenL(_L(":")); 
 
	TPtrC cmd = st->NextToken();
	
	TBool comment = (cmd[0] == ';');
	TBool sup = (cmd.MatchC(KSUP) == 0);
	TBool abs = (cmd.MatchC(KABS) == 0);
	TBool rel = (cmd.MatchC(KREL) == 0);
	TBool snr = (cmd.MatchC(KSNR) == 0);
	TBool ord_rel = (cmd.MatchC(KORDREL) == 0);
	TBool ord_snr = (cmd.MatchC(KORDSNR) == 0);
	TBool info = (cmd.MatchC(KINFO) == 0);
	TBool err = (cmd.MatchC(KERROR) == 0);
	TBool ret = (cmd.MatchC(KRETURN) == 0);
	
	TBool validCommand = comment || sup || abs || rel || snr || ord_rel || ord_snr || info || err || ret;
	if (!validCommand)
		{
		User::Leave(KErrCorrupt);		// Error
		}

	CAbstractLine* patchLine=NULL;
	if (comment && patchLine == NULL)
		{
		patchLine = CLineComment::NewL();
		}
	
	if (ret && patchLine == NULL)
		{		
		patchLine = CLineReturn::NewL();
		}
	
	if (info && patchLine == NULL)
		{	
		patchLine = CLineInfo::NewL( *st );
		}
	
	if (err && patchLine == NULL)
		{		
		patchLine = CLineError::NewL( *st );
		}
	
	if (sup && patchLine == NULL)
		{
		TLinAddr addr = SuperPageL();
		patchLine = CLineSuper::NewL( addr, *st );
		}
	
	if (abs && patchLine == NULL)
		{
		patchLine = CLineAbs::NewL( *st );
		}

	if (rel && patchLine == NULL)
		{
		patchLine = CLineRel::NewL( *st );
		}
	
	if (snr && patchLine == NULL)
		{
		patchLine = CLineSnr::NewL( *st );
		}

	if (ord_rel && patchLine == NULL)
		{
		patchLine = CLineOrdRel::NewL( *st );
		}
	
	if (ord_snr && patchLine == NULL)
		{
		patchLine = CLineOrdSnr::NewL( *st );
		}
	
	CleanupStack::PopAndDestroy(st);
	return patchLine;
	}


_LIT(KDEFINE, "#define");
_LIT(KIFDEF,  "#ifdef");
_LIT(KIFNDEF, "#ifndef");
_LIT(KELSE,   "#else");
_LIT(KENDIF,  "#endif");


enum TDefineStatus
	{
	ESearchForIf = 0,
	EFoundIf,
	EFoundElse
	};


LOCAL_C HBufC* GetKeyLC(const TDesC& aLine, const TDesC& aToken)
	{
	TInt pos = aLine.FindC(aToken);
	if (pos < 0)
		User::Leave(KErrCorrupt);
	HBufC* res = aLine.Mid( pos+aToken.Length() ).AllocLC();
	TPtr ptrRes = res->Des();
	StringUtils::ReplaceAll(ptrRes, _L(" "), _L(""));
	StringUtils::ReplaceAll(ptrRes, _L("\t"), _L(""));
	ptrRes.UpperCase();
	return res;
	}


LOCAL_C TDefineStatus ConvertL(RPointerArray<HBufC>& definitions, CStringTokenizer* st, TInt& lineCount, TDes& outBuf, TDefineStatus aStatus, TBool acceptLines)
	{
	TInt lineError = 0;
	while ( st->HasMoreTokens() && lineError == 0)
		{
		lineCount++;
  		TPtrC line = st->NextToken();
  		
  		TInt commentPos = line.FindC(_L(";"));
  		if (commentPos < 0)  
  			commentPos = 0xFFFF;
  		
  		TInt defPos = line.FindC(KDEFINE);
  		TBool foundDef = ((defPos < commentPos) && (defPos >= 0));
  		if (foundDef)
  			{
  			outBuf.Append(KDELIM_LF);
  			if (acceptLines)
  				{
				// Add definition to Array...
				HBufC* keyBuf = GetKeyLC(line, KDEFINE);
				TIdentityRelation<HBufC> stringMatch( StringUtils::StringCompareF );
				if (definitions.Find( keyBuf, stringMatch ) >= 0)
					{
					CleanupStack::PopAndDestroy(keyBuf);
					} else
					{
					definitions.Append(keyBuf);
					CleanupStack::Pop(keyBuf);					
					}
  				}
  			continue;
  			}
  		
  		TInt elsePos = line.FindC(KELSE);
  		TBool foundElse = ((elsePos < commentPos) && (elsePos >= 0));
  		if (foundElse)
  			{
  			if (aStatus != EFoundIf)
  				User::Leave(KErrCorrupt);
  			outBuf.Append(KDELIM_LF);
  			return EFoundElse;
  			}

  		TInt endIfPos = line.FindC(KENDIF);
  		TBool foundEndIf = ((endIfPos < commentPos) && (endIfPos >= 0));
  		if (foundEndIf)
  			{
  			if (aStatus == ESearchForIf)
  				User::Leave(KErrCorrupt);
  			outBuf.Append(KDELIM_LF);
  			return ESearchForIf;
  			}
  		
  		TInt ifDefPos = line.FindC(KIFDEF);
  		TBool foundIfDef = ((ifDefPos < commentPos) && (ifDefPos >= 0));
  		TInt ifNDefPos = line.FindC(KIFNDEF);
  		TBool foundIfNDef = ((ifNDefPos < commentPos) && (ifNDefPos >= 0));
  		
  		if (foundIfDef || foundIfNDef)
  			{
  			outBuf.Append(KDELIM_LF);
  			TBool exprFlag = foundIfDef;

  			// GetKey
  			HBufC* keyBuf = NULL;
  			if (exprFlag)
  				keyBuf = GetKeyLC(line, KIFDEF);
  			else 
  				keyBuf = GetKeyLC(line, KIFNDEF);
  				
			TIdentityRelation<HBufC> stringMatch( StringUtils::StringCompareF );
			TBool defined = (definitions.Find( keyBuf, stringMatch ) >= 0);

			// Evaluate expression:   if ( IsDefined(key) == exprFlag )
  			TBool expr = (defined == exprFlag);
  			CleanupStack::PopAndDestroy(keyBuf);
  			
  			TDefineStatus newStat = ESearchForIf;
  			if (expr)
  				{
  				// Accept lines until an #else or #endif is found...
  				newStat = ConvertL(definitions, st, lineCount, outBuf, EFoundIf, acceptLines & ETrue);
  	  			if (newStat == EFoundElse)
  	  				newStat = ConvertL(definitions, st, lineCount, outBuf, EFoundElse, EFalse);
  				} 
  			else 
  				{
  				// Ignore lines until an #else or #endif is found...	
  	  			newStat = ConvertL(definitions, st, lineCount, outBuf, EFoundIf, EFalse);
  	  			if (newStat == EFoundElse)
  	  				newStat = ConvertL(definitions, st, lineCount, outBuf, EFoundElse, acceptLines & ETrue);
  				}
			if (newStat != ESearchForIf)
				User::Leave(KErrCorrupt);
  			// st punta ad #endif il prox. elem e' la prox. istruz utile...
  			continue;
  			}
  		
  		if (acceptLines)
  			{
	  		outBuf.Append(line);
  			} 
		outBuf.Append(KDELIM_LF);
		}
	return aStatus;
	}


_LIT(KMANUF, 	"MANUFACTURER_%x");
_LIT(KMACHINE, 	"MACHINE_%x");
_LIT(KCPU, 		"CPU_%d");
_LIT(KCPUABI, 	"CPUABI_%d");
_LIT(KPLAT, 	"PLATFORM_%d.%d");
_LIT(KLANG, 	"LANG_%d");
_LIT(KRPVER, 	"RP+VERSION_%d.%d");
_LIT(KFW, 		"FW_%S");


void RomPatcherEngine::GetSWVersionL(TDes& versionBuf)
	{
	SysUtil::GetSWVersion(versionBuf);
	versionBuf.UpperCase();
	
	CStringTokenizer* st = CStringTokenizer::NewLC();
	st->InitializeL( versionBuf );
	st->AddCharsTokenL( _L("\r,_ \nV\t()[]") );
	TPtrC foundToken = TPtrC();
	while (foundToken.Length() == 0 && st->HasMoreTokens())
		{
		TPtrC token = st->NextToken();
		if (token.Length() > 0)
			{
			if ( StringUtils::ValidateStringL( _L("0123456789."), token, ETrue ) )
				{
				foundToken.Set( token );
				}
			}
		}
	versionBuf = foundToken;
	CleanupStack::PopAndDestroy();
	}


LOCAL_C void PopulateDefinitionsL(RFs& fs, RPointerArray<HBufC>& definitions)
	{
//    definitions.Append( );
    RStringBuf buf;
    buf.CleanupClosePushL();
    buf.Create(30);
    
	TInt val;
    HAL::Get(HAL::EManufacturer, val);			// MANUFACTURER_
    buf.Zero();
    buf.AppendFormatL( KMANUF, val );
    definitions.Append( buf.Alloc() );

    HAL::Get(HAL::EMachineUid, val);			// MACHINE_
    buf.Zero();
    buf.AppendFormatL( KMACHINE, val );
    definitions.Append( buf.Alloc() );
    
    HAL::Get(HAL::ECPU, val);											//	<--- CPU_0
    buf.Zero();
    buf.AppendFormatL( KCPU, val );
    definitions.Append( buf.Alloc() );

    HAL::Get(HAL::ECPUABI, val);										//	<--- CPUABI_0
    buf.Zero();
    buf.AppendFormatL( KCPUABI, val );
    definitions.Append( buf.Alloc() );

	TUint major;
	TUint minor;
	Utils::GetS60PlatformL( fs, major, minor );							// 	<--- PLATFORM_3.1
    buf.Zero();
    buf.AppendFormatL( KPLAT, major, minor ); 
    definitions.Append( buf.Alloc() );
    
	TLanguage lang = User::Language();									//	<--- LANG_5
    buf.Zero();
    buf.AppendFormatL( KLANG, lang ); 
    definitions.Append( buf.Alloc() );

    buf.Zero();
    buf.AppendFormatL( KRPVER, 2, 6 ); 
    definitions.Append( buf.Alloc() );
   
	// 2.0618.06.05_v12													// 	<--- FW_2.0618.06.05
	TBuf<KSysUtilVersionTextLength> versionBuf;
	RomPatcherEngine::GetSWVersionL(versionBuf);
    buf.Zero();
    buf.AppendFormatL( KFW, &versionBuf );
    definitions.Append( buf.Alloc() );
	
	CleanupStack::PopAndDestroy(&buf);
	}


LOCAL_C TInt PreProcessL(RPointerArray<HBufC>& definitions, TDes& outBuf)
	{
	RBuf oriBuf;
	oriBuf.Create( outBuf );
	oriBuf.CleanupClosePushL();
	
	CStringTokenizer* st = CStringTokenizer::NewLC();
	st->InitializeL( oriBuf );
	st->AddStringTokenL( KDELIM_CRLF );
	st->AddStringTokenL( KDELIM_CR );
	st->AddStringTokenL( KDELIM_LF );
	
	TInt lineCount = 0;
	outBuf.Zero();
	TRAPD(err, ConvertL(definitions, st, lineCount, outBuf, ESearchForIf, ETrue ) );
	
	CleanupStack::PopAndDestroy( st );
	CleanupStack::PopAndDestroy( &oriBuf );
	
	if (err != KErrNone)
		return lineCount;
	return 0;
	}


LOCAL_C void DumpDefsL(RFs& fs, const TDesC& fname, RPointerArray<HBufC>& defs)
	{
	RStringBuf buf;
	buf.CleanupClosePushL();
	for (TInt i=0; i<defs.Count(); i++)
		{
		TPtrC ptr = defs[i]->Des();
		buf.AppendL( ptr );
		buf.AppendL( KDELIM_LF );
		}
	FileUtils::DumpToFileL(fs, fname, buf);
	CleanupStack::PopAndDestroy(&buf);
	}


LOCAL_C HBufC* DecryptPatchL(RFs& aFs, const TDesC& aFileName)
	{
	CData* data = CData::NewLC();
	RBuf8 buf8(data->DecryptFromFileL(aFs, aFileName));
	buf8.CleanupClosePushL();
	HBufC* buf = HBufC::NewL( buf8.Length() * 2);
	buf->Des().Copy(buf8);
	CleanupStack::PopAndDestroy(&buf8);
	CleanupStack::PopAndDestroy(data);	
	return buf;
	}
 

HBufC* RomPatcherEngine::ReadAndPreProcessFileL(RFs& aFs, const TDesC& aFileName, TInt& aLineError)
	{
	HBufC* buf = NULL;
	TParsePtrC parse(aFileName);
	if (parse.Ext().FindC(KPATCHEXTRPP) >= 0)
		{
		// TODO: Test... con patch buone e altre cifrate male...
		TRAP_IGNORE(buf = DecryptPatchL(aFs, aFileName));
		if (buf == NULL)
			buf = HBufC::NewL(0);
		} 
	else
		{
		buf = FileUtils::ReadTextFileL(aFs, aFileName);
		}
	CleanupStack::PushL( buf );

	/*
	HBufC* buf = FileUtils::ReadTextFileL(aFs, aFileName);
	CleanupStack::PushL( buf );/**/	

	TPtr ptrBuf = buf->Des();
	
	RPointerArray<HBufC> definitions;
	CleanupResetAndDestroyPushL(definitions);		
	PopulateDefinitionsL(aFs, definitions);

	//DumpDefsL(aFs, _L("c:\\PreDefs.txt"), definitions);
	aLineError = PreProcessL(definitions, ptrBuf);		//FileUtils::DumpToFileL(aFs, _L("c:\\Patch.txt"), ptrBuf);
	//DumpDefsL(aFs, _L("c:\\PostDefs.txt"), definitions);
	
	CleanupStack::PopAndDestroy(&definitions);	
	CleanupStack::Pop( buf );
	return buf;
	}


TInt RomPatcherEngine::LoadPatchValuesL(RFs& fs, const TDesC& filename, RPointerArray<CAbstractLine>& array)
	{
	TInt lineError = 0;
	RBuf buf( ReadAndPreProcessFileL( fs, filename, lineError ) );
	buf.CleanupClosePushL();	
	buf.LowerCase();
//	StringUtils::ReplaceAll(buf, _L(" "), _L(""));
	StringUtils::ReplaceAll(buf, _L("\t"), _L(""));
	 
	TInt lineCount = 0;
	CStringTokenizer* st = CStringTokenizer::NewLC();
	st->InitializeL( buf );
	st->AddStringTokenL( KDELIM_CRLF );
	st->AddStringTokenL( KDELIM_CR );
	st->AddStringTokenL( KDELIM_LF );
	
	while ( st->HasMoreTokens() && lineError == 0)
		{
		lineCount++;
  		TPtrC line = st->NextToken();
		
  		CAbstractLine* toPatch = NULL;
		TRAPD(err, toPatch = ParseLineL( line ));
		if (err != KErrNone)
			lineError = lineCount;
		if (toPatch != NULL)
			{
			// Add To Array
			array.Append( toPatch );
			}
		}
	CleanupStack::PopAndDestroy( st );
	CleanupStack::PopAndDestroy(&buf);
	return lineError;
	}


/*
TIdentityRelation<TRomPageItem> romPageMatch( TRomPageItemCompare );

TBool TRomPageItemCompare(const TRomPageItem& aStringA, const TRomMapItem& aStringB)
	{
	
	}
	
void ExternalizeL(RWriteStream& aStream) const;
void InternalizeL(RReadStream& aStream);

TInt iAddr;
TInt iUsageCounter;



Crea anche una classe CRomPages simile a CData che incapsula RArray<TRomPageItem> ed e' questa che viene passata da una parte all'altra...
Si occupa della persistenza dei dati su file e della gestione dell'array degli elementi TRomPageItem
*/


TInt RomPatcherEngine::ApplyPatchValuesL(RPointerArray<CAbstractLine>& array, TBool& hasWarnings)
	{
    User::LeaveIfError( LoadDevice() );
    RDevPatcherClient cli;
    CleanupClosePushL(cli);
	User::LeaveIfError( cli.Open() );

	hasWarnings = EFalse;
    TInt lineError = 0;
    TInt i=0;
    TBool skip = EFalse;
    TBool memChanged = EFalse;
    while (i<array.Count() && lineError == 0 && skip == EFalse)
    	{
    	CAbstractLine* line = array[i];
    	if (line->Type() == EReturn)
    		skip = ETrue;
    	
    	if (!line->ApplyL(cli, hasWarnings))
    		{
    		lineError = (i + 1);
    		}
    	memChanged |= line->iMemChanged;
    	i++;     
    	}
	
    CleanupStack::PopAndDestroy(&cli);
    FreeDevice();
    
    // Error Detected...
    if (lineError > 0)
    	{
    	// Removes the lines that have not been processed yet.
    	// To avoid memory corruption in RAM.
    	TInt i = lineError-1;
    	while (i < array.Count())
    		{
    		CAbstractLine* line = array[i];
    		delete line;
    		array.Remove(i);    		
    		}
    	
    	RemovePatchValuesL(array);    
    	return lineError;
    	}
    
    if (!memChanged)
    	return array.Count()+1;
    
	return 0;
	}



TInt RomPatcherEngine::RemovePatchValuesL(const RPointerArray<CAbstractLine>& array)
	{
	// Creare una patch con lo stesso nome ma con estensione .bak
	// Questa conterra' il riferimento assoluto in memoria (calcolato dal CheckPatch)... In pratica salva l'intero array di CAbstractLine
	// ...Viene ricreata ogni volta che viene applicata una patch SNR... e la crea nella directory privata, cosi' l'utente non fa danni!!!
	// Magari crearla in D: e farlo per tutte quante le patch applicate, non solo per le SNR!
	// Cosi' sappiamo anche quali patch sono state applicate e quali no... Inoltre abbiamo modo di ritornare all'originale semplicemente applicando una patch!
	// La patch sara' salvata usando esclusivamente il prefisso abs: e salvando un bel po' di zeri tanti quanta la lunghezza dei dati da liberare...
	//
	// Bisogna considerare che alcune patch non lavorano sulla ROM quindi il vecchio valore e' da salvare!
	// User::IsRomAddress()
	// 
	
    User::LeaveIfError( LoadDevice() );
    RDevPatcherClient cli;
    CleanupClosePushL(cli);
	User::LeaveIfError( cli.Open() );

    TInt errorLine = 0;
    for (TInt i=0; i<array.Count(); i++)
    	{
    	CAbstractLine* line = array[i];
    	if (!line->RemoveL(cli) && errorLine == 0)
    		errorLine = i;
    	}
    
    CleanupStack::PopAndDestroy(&cli);
    FreeDevice();
    return errorLine;
	}


LOCAL_C TBool GetPatchesDriveAndPath(RFs& aFs, TDes& driveAndPath)
	{
#ifdef __WINSCW__
_LIT(KPATCHESPATH, "\\patches\\");
#else
_LIT(KPATCHESPATH, "\\patches\\");
#endif
	for (TInt driveNumber=EDriveZ; driveNumber>=EDriveA; driveNumber--)
		{		
		TBuf<2> drive;
		if ( FileUtils::GetDrive( aFs, driveNumber, drive ) != KErrNone ) continue;
		// Drive found...
		
		driveAndPath.Zero();
		driveAndPath.Append(drive);
		driveAndPath.Append(KPATCHESPATH);
		driveAndPath.LowerCase();
		if (BaflUtils::FolderExists(aFs, driveAndPath))
			return ETrue;
		}
	return EFalse;
	}


LOCAL_C TBool GetPatchFileName(RFs& aFs, const TDesC& aPatchNameAndExt, TDes& fname)
	{
	if (!GetPatchesDriveAndPath(aFs, fname))
		return EFalse;
	fname.Append( aPatchNameAndExt );
	return (BaflUtils::FileExists(aFs, fname));
	}


LOCAL_C void GetUnpatchFileName(RFs& aFs, const TDesC& aPatchNameAndExt, TDes& fname)
	{
	TInt err = aFs.MkDirAll(KUNPATCHPATH);
	if (err == KErrNone)
		{
		aFs.SetAtt(KUNPATCHPATH, KEntryAttHidden|KEntryAttSystem, KEntryAttNormal);
		}
	fname = KUNPATCHPATH;
	fname.Append( aPatchNameAndExt );
	fname.Append( KPATCHEXT );
	}


void RomPatcherEngine::GetPatchesStatusL(CData* aData, RFs& aFs, RPointerArray<HBufC>& aPatchList, RArray<TPatchStatus>& aPatchStatus,  RArray<TAutoStatus>& aAutoStatus)
	{
	TFileName patchesPath;
	TBool foundPatchesPath = GetPatchesDriveAndPath(aFs, patchesPath);
	if (!foundPatchesPath)
		{
		patchesPath = _L("c:\\patches\\");
		aFs.MkDirAll( patchesPath );		
		}
		
	// Process Zip file and unzip it in the Patches dir...
	TFileName zipArchive = KZIPFILE();
	FileUtils::CompleteWithCurrentDrive(zipArchive);
	if (aData->iZipAlreadyProcessed == 0)
		{ 
		TRAP_IGNORE( Utils::ExtractZipL(aFs, zipArchive, patchesPath) );
		aData->iZipAlreadyProcessed = 1;
		}
	TRAP_IGNORE( BaflUtils::DeleteFile(aFs, zipArchive) );
	CleanupStack::PopAndDestroy( aData->iBadGuiCrc ); 
	 
	RPointerArray<HBufC> patchFileNames; 
	CleanupResetAndDestroyPushL(patchFileNames); 
	FileUtils::ListFilesInDirectoryL(aFs, patchesPath, patchFileNames); 
	
	for (TInt i=0; i<patchFileNames.Count(); i++)
		{
		TPtrC patchFile = patchFileNames[i]->Des();
		
		TParsePtrC parse(patchFile);
		if (parse.DriveAndPath().MatchC(patchesPath) != 0)
			continue;
		if ((parse.Ext().FindC(KPATCHEXT) < 0) && (parse.Ext().FindC(KPATCHEXTRPP) < 0))
			continue;
		
		HBufC* patchName = parse.NameAndExt().AllocL();
		aPatchList.Append( patchName );
		
		TFileName unPatchFilename;
		GetUnpatchFileName(aFs, parse.NameAndExt(), unPatchFilename);		
		if (BaflUtils::FileExists(aFs, unPatchFilename))
			{
			aPatchStatus.Append( EApplied );
			} else
			{
			aPatchStatus.Append( EIdle );				
			}
		
		// Check lowParse.Name is in Config?
		// If Auto then status++; 
		TAutoStatus autoStatus = aData->GetAutoStartL( parse.NameAndExt() );
		aAutoStatus.Append(autoStatus);		
		}	 
	
	// Remove unmatched entries from Config file
	aData->RemoveOldAutoEntriesL(aPatchList);
	 
	patchFileNames.ResetAndDestroy();
	CleanupStack::PopAndDestroy(&patchFileNames);
	}


HBufC* RomPatcherEngine::GetPatchInfoLC(RFs& aFs, const TDesC& aPatchName)
	{
	RStringBuf result;
	result.Create(1000);
	result.CleanupClosePushL();
	
	// Get Patch Filename
	TFileName filename;
	if (!GetPatchFileName(aFs, aPatchName, filename))
		{
		return HBufC::NewLC(1);
		}
	TInt lineError = 0;
	RBuf buf( ReadAndPreProcessFileL( aFs, filename, lineError ) );
//	RBuf buf( FileUtils::ReadTextFileL(aFs, filename) );
	buf.CleanupClosePushL(); 

	StringUtils::ReplaceAll(buf, _L("\t"), _L(""));
	
	CStringTokenizer* st = CStringTokenizer::NewLC();
	st->InitializeL( buf );
	st->AddStringTokenL( KDELIM_CRLF );
	st->AddStringTokenL( KDELIM_CR );
	st->AddStringTokenL( KDELIM_LF );
	
	while ( st->HasMoreTokens() )
		{
  		TPtrC aLine = st->NextToken();
  		TPtrC line = StringUtils::Trim(aLine);
		if (line.Length() > 0 && line[0] == ';')
			{
			TPtrC descrOnly = line.Mid(1);
			result.AppendL( descrOnly );
			result.AppendL( KDELIM_LF );
			}
		}
	CleanupStack::PopAndDestroy( st );
		
	CleanupStack::PopAndDestroy(&buf);
	HBufC* resBuf = result.PopAndReturnAsHBufL();
	CleanupStack::PushL(resBuf);
	return resBuf;
	}


void RomPatcherEngine::SetAutoStartL(CData* aData, const TDesC& patchName, TAutoStatus aAutoValue)
	{
/*	TParsePtrC parse(RProcess().FileName());
#ifdef __WINSCW__
	TFileName configFileName = _L("c:");
#else
	TFileName configFileName = parse.Drive();
#endif
	configFileName.Append( KAUTOFILE );*/
	
	// Read Config File... 
	// Save lowPatchName in Config File...
	aData->SetAutoStartL( patchName, aAutoValue);
	}


void RomPatcherEngine::RemovePatchL(RFs& aFs, const TDesC& aPatchNameAndExt)
	{
	TFileName filename;
	GetUnpatchFileName(aFs, aPatchNameAndExt, filename);

	// Meglio lasciare che venga generato errore in fase di rimozione patch, cosi' se l'utente ha cancellato il file manualmente viene segnalato errore...
	//if (!BaflUtils::FileExists(aFs, aPatchName))
	//	return;
	
	RPointerArray<CAbstractLine> array;
	CleanupResetAndDestroyPushL(array);
	
	TInt errLine = 0;
	errLine = RomPatcherEngine::LoadPatchValuesL(aFs, filename, array);
	if (errLine > 0)
		User::Leave(errLine);
	
	errLine = RomPatcherEngine::RemovePatchValuesL(array);
	if (errLine > 0)
		User::Leave(errLine);
	
	CleanupStack::PopAndDestroy( &array );
	
	// Elimina "filename" che consente la rimozione della patch (contenente solo keywords "abs:")
	BaflUtils::DeleteFile( aFs, filename );
	}


void RomPatcherEngine::CreateUnpatchFileL(RFs& aFs, const TDesC& fname, RPointerArray<CAbstractLine> array)
	{
	// Save abs values in file...	
	RFile file;
	CleanupClosePushL(file);
	file.Replace( aFs, fname, EFileShareAny);
	
    TUint32 offset = 0;
    for (TInt i=0; i<array.Count(); i++)
    	{
    	CAbstractLine* line = array[i];
    	HBufC8* buf = line->GetUnpatchDataL();
    	CleanupStack::PushL(buf);
    	TPtr8 ptrBuf = buf->Des();
		file.Write(offset, ptrBuf);
		offset += ptrBuf.Length();
    	CleanupStack::PopAndDestroy(buf);
    	
		file.Write(offset, _L8("\n"));
		offset += 1;
    	}
    CleanupStack::PopAndDestroy(&file);
	}


TBool RomPatcherEngine::ApplyPatchL(RFs& aFs, const TDesC& aPatchNameAndExt)
	{	
	// Get Patch Filename
	TFileName unpatch_fname;
	GetUnpatchFileName(aFs, aPatchNameAndExt, unpatch_fname);

	if (BaflUtils::FileExists(aFs, unpatch_fname))
		User::Leave(KErrAlreadyExists);

	TFileName fname;
	if (!GetPatchFileName(aFs, aPatchNameAndExt, fname))
		User::Leave(KErrNotFound);
	
	
	RPointerArray<CAbstractLine> array;
	CleanupResetAndDestroyPushL(array);	
	TInt errLine = 0;

	errLine = RomPatcherEngine::LoadPatchValuesL(aFs, fname, array);
	
//Notes::Debug_InfoL( _L("Loaded :%d"), errLine );

	if (errLine > 0)
		User::Leave(errLine);
	

	TBool hasWarnings = EFalse;
	errLine = RomPatcherEngine::ApplyPatchValuesL(array, hasWarnings);

//Notes::Debug_InfoL( _L("Applied :%d:%d"), errLine, hasWarnings );
	
	if (errLine > 0)
		User::Leave(errLine);
	
	// Bisogna ripristinare il vecchio valore per le patch che non lavorano con la ROM... Quindi i bytes original vanno mantenuti tutti.
	// Salva l'array in un file contenente solo keywords "abs:" che consentira' di rimuovere la patch...
	RomPatcherEngine::CreateUnpatchFileL(aFs, unpatch_fname, array);
	
//Notes::Debug_InfoL( _L("CreatedUnpatchFile") );
	
	CleanupStack::PopAndDestroy( &array );
	return hasWarnings;
	}
