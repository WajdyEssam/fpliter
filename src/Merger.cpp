
#include "Merger.h"

Merger :: Merger ( const string& fileName ) {
	hFile = CreateFile(getOrignalFileName(fileName).c_str(),GENERIC_WRITE,0,NULL,CREATE_ALWAYS,
					FILE_ATTRIBUTE_NORMAL,NULL);
					
	if ( hFile == INVALID_HANDLE_VALUE ) {
		return  ;
	}
}

BOOL Merger :: Merge ( const string& file ) {
	DWORD SIZE = Utility::KiloToByte(10)  ;
	BYTE Buffer[SIZE];
	
	HANDLE hOldFile = CreateFile(file.c_str(),GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL,NULL);
					
	if ( hOldFile == INVALID_HANDLE_VALUE ) {
		return FALSE ;
	}	
		
	DWORD dwIn , dwOut ;
	
	while ( ReadFile( hOldFile,Buffer,sizeof(Buffer),&dwIn,NULL) && dwIn > 0) {
		WriteFile(hFile,Buffer,dwIn,&dwOut,NULL);
		
		if ( dwIn != dwOut )
			return FALSE ;
	}
	
	CloseHandle(hOldFile);
	return TRUE ;
}

VOID Merger :: Close () {
	CloseHandle(hFile);
}

/*
void Merger:: Merge ( const vector<string>& fileNames ) {
	HANDLE hFile = CreateFile(getOrignalFileName(fileNames[0]).c_str(),GENERIC_WRITE,FILE_SHARE_WRITE,NULL,CREATE_ALWAYS,
					FILE_ATTRIBUTE_NORMAL,NULL);
					
	if ( hFile == INVALID_HANDLE_VALUE ) {
		return  ;
	}
	
	DWORD SIZE = Utility::KiloToByte(1) ;
	BYTE Buffer[SIZE];
	
	for (unsigned int i=0 ; i<fileNames.size() ; i++) {
		HANDLE hOldFile = CreateFile(fileNames[i].c_str(),GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,
					FILE_ATTRIBUTE_NORMAL,NULL);
					
		if ( hOldFile == INVALID_HANDLE_VALUE ) {
			return  ;
		}	
		
		DWORD dwIn , dwOut ;
		while ( ReadFile( hOldFile,Buffer,sizeof(Buffer),&dwIn,NULL) && dwIn > 0) {
			WriteFile(hFile,Buffer,dwIn,&dwOut,NULL);
			
			if ( dwIn != dwOut )
				break ;
		}
		
		CloseHandle(hOldFile);
	}
	
	CloseHandle(hFile);
}

*/

string  Merger :: getOrignalFileName( const string& s ) {
	string tmp  = s.substr(0,s.find_last_of("\\") ) + "\\-" ;	
	string x    = tmp + s.substr( s.find_last_of("\\") + 1 , s.length()) ;
	string m = x.substr( 0  , x.find_last_of('-') );
	
	return m ;
}
