/*
 ============================================================================
 Name		: RomPatcherEngine.pan
 Author	  : Marco Bellino
 Copyright   : Guardian-Mobile.com
 Description : Panic codes
 ============================================================================
 */

#ifndef __ROMPATCHERENGINE_PAN__
#define __ROMPATCHERENGINE_PAN__

//  Data Types

enum TRomPatcherEnginePanic
	{
	ERomPatcherEngineNullPointer
	};

//  Function Prototypes

GLREF_C void Panic(TRomPatcherEnginePanic aPanic);

#endif  // __ROMPATCHERENGINE_PAN__

