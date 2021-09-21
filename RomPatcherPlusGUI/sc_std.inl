// f32\scomp\sc_std.inl
//
// Copyright (c) 2003-2007 Symbian Ltd.  All rights reserved.
//

// Class TCompMount
inline TCompMount::TCompMount(CFileSystem* aFs, CMountCB* aMount)
			{iFs = aFs; iMount=aMount;}

// Class CCompMountCB
inline void CCompMountCB::SetMountNumber(TInt aNum) 
			{iMountNumber=aNum;}		

inline CMountCB* CCompMountCB::RomMount() const 
			{return(iMounts[0].iMount);}

inline void CCompMountCB::NullCompFileSystem(void) 
			{iFileSystem=NULL;}	

// Class CCompFileCB
inline void CCompFileCB::SetTrueFile(CFileCB* aFile) 
			{iTrueFile=aFile;}

inline CFileCB* CCompFileCB::TrueFile() const 
			{return(iTrueFile);}

// Class CCompFileSystem
inline void CCompFileSystem::NullMount(void) 
			{iMount=NULL;}	

