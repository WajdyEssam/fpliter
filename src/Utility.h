
// Utility.h

#ifndef __UTILITY__
#define __UTILITY__

#include <sstream>
#include <windows.h>
#include <iostream>
#include <vector>
#include <fstream>
using namespace std; 

typedef __int64 BigInt  ;		// 8 byte long ;

class Utility {
	public :
		static BigInt makeData (string str);	/* convert from string to number */
		static string makeData (BigInt data);	/* convert from number to string */
		
		// convert from byte to other system
		static BigInt ByteToKilo 	( BigInt byte  ) ;
		static BigInt BteToMega 	( BigInt byte  ) ;
		static BigInt ByteToGega 	( BigInt byte  ) ;

		// convert from other system to byte
		static BigInt KiloToByte 	( BigInt byte  ) ;
		static BigInt MegaToByte 	( BigInt bytes ) ;
		static BigInt GegaToByte   	( BigInt bytes ) ;
		
		// get the size of file
		static BigInt GetSize ( HANDLE hFile ) ;
		static BigInt GetSize ( const string& fileName ) ;
		
		// get number of splitter files
		static int GetSplitterNumber ( BigInt fileSizeInBytes , BigInt splitSizeInBytes );
		
		// get names of all splitter files to merge it
		static vector<string> GetSplitterName ( const string& fileName ) ;
		
		// set split name
		static string setSplitName (const string& fileName,int count) ;	
};

#endif  //__UTILITY__
