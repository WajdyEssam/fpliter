
#ifndef __SPLITTER__
#define __SPLITTER__

#include <windows.h>
#include <iostream>
using namespace std;

typedef __int64 BigInt  ;		// 8 byte long ;

#include "Utility.h"

class Splitter {
	public :
		Splitter ( const string& fileName ,BigInt fileSize, BigInt splitSizeInBytes );
		BOOL Split ( int number ) ;
		VOID Close () ;
		int GetSplitNumber () ;
		
	private :
		string FileName ;
		HANDLE hFile ;
		BigInt BlockSize;
		BigInt SplitSize ;
		BigInt SplitNumber;
		
	// public:
		//static void Split  ( const string& fileName ,BigInt fileSize, BigInt splitSizeInBytes );
};

#endif // __SPLITTER
