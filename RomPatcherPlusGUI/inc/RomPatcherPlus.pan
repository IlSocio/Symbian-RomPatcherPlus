
#ifndef ROMPATCHERPLUS_PAN_H
#define ROMPATCHERPLUS_PAN_H

/** RomPatcherPlus application panic codes */
enum TRomPatcherPlusPanics
	{
	ERomPatcherPlusUi = 1
	// add further panics here
	};

inline void Panic(TRomPatcherPlusPanics aReason)
	{
	_LIT(applicationName,"RomPatcherPlus");
	User::Panic(applicationName, aReason);
	}

#endif // ROMPATCHERPLUS_PAN_H
