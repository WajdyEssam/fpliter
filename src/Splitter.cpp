
#include "Splitter.h"

Splitter :: Splitter ( const string& fileName , BigInt fileSize , BigInt splitSizeInBytes ) {
	hFile = CreateFile(fileName.c_str(),GENERIC_READ,0,NULL,OPEN_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL); 
	if ( hFile == INVALID_HANDLE_VALUE ) {
		return  ;
	}
	
	FileName    = fileName ;
	BlockSize   = Utility::KiloToByte(10) ;
	SplitSize   = splitSizeInBytes ;
	SplitNumber = Utility::GetSplitterNumber(fileSize , splitSizeInBytes ) ;
}

BOOL Splitter :: Split ( int i ) {
	string SplitName = FileName + "-" ;
	HANDLE hNewFile = CreateFile(Utility::setSplitName(SplitName,i).c_str(),GENERIC_WRITE,
		FILE_SHARE_WRITE,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
			
	if ( hNewFile == INVALID_HANDLE_VALUE ) {
		return  FALSE ;
	}
		
	DWORD ReadBytes = 0 ;
	DWORD dwIn , dwOut ;
	BYTE  Buffer[BlockSize];
	do {
		ReadBytes += BlockSize ;
		ReadFile(hFile,Buffer,sizeof(Buffer),&dwIn,NULL);
		WriteFile(hNewFile,Buffer,dwIn,&dwOut,NULL);
	} while ( ReadBytes <= SplitSize && dwIn > 0 );
		
	CloseHandle(hNewFile);
	return TRUE ;
}

VOID Splitter :: Close () {
	CloseHandle(hFile);
}

int Splitter :: GetSplitNumber () {
	return SplitNumber ;
}

/* this function do everything but we not use it because progress bar need to counting 
void Splitter :: Split  ( const string& fileName , BigInt fileSize , BigInt splitSizeInBytes ){
	HANDLE hFile = CreateFile(fileName.c_str(),GENERIC_READ,0,NULL,OPEN_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL); 
	if ( hFile == INVALID_HANDLE_VALUE ) {
		return  ;
	}
	
	DWORD BLOCK_SIZE = Utility::KiloToByte(10) ;
	BYTE  Buffer[BLOCK_SIZE];
	DWORD SPLIT_SIZE =  splitSizeInBytes ;
	int number = Utility::GetSplitterNumber(fileSize , splitSizeInBytes ) ;

	for (int i=0 ; i<number ; i++ ) {
		string SplitName = fileName + "-" ;
		HANDLE hNewFile = CreateFile(Utility::setSplitName(SplitName,i).c_str(),GENERIC_WRITE,
			FILE_SHARE_WRITE,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
			
		if ( hNewFile == INVALID_HANDLE_VALUE ) {
			return  ;
		}
		
		DWORD ReadBytes = 0 ;
		DWORD dwIn , dwOut ;
		do {
			ReadBytes += BLOCK_SIZE ;
			ReadFile(hFile,Buffer,sizeof(Buffer),&dwIn,NULL);
			WriteFile(hNewFile,Buffer,dwIn,&dwOut,NULL);
		} while ( ReadBytes <= SPLIT_SIZE && dwIn > 0 );
		
		CloseHandle(hNewFile);
	}
	
	CloseHandle(hFile);
}
*/
