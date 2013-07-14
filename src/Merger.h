
#ifndef __MERGER__
#define __MERGER__

#include <windows.h>
#include <iostream>
using namespace std;

typedef __int64 BigInt  ;		// 8 byte long ;

#include "Utility.h"

class Merger {
	public :
		Merger      ( const string& fileName ) ;
		BOOL Merge  ( const string& fileName ) ;
		VOID Close () ;
		
	private:
		HANDLE hFile ;
		string getOrignalFileName( const string& s ) ;
		
	//public :
		//static void Merge ( const vector<string>& fileNames ) ;
};

#endif // __MERGER__
