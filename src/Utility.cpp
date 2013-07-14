
#define _WIN32_WINNT  0x0500 	// for GetFileSizeEx

#include "Utility.h"

BigInt Utility :: makeData (string str) {
	istringstream istr(str);	// make string stream
	BigInt tmp = 0 ;
	
	istr >> tmp ;
	if ( istr.fail() || !istr.eof() )
		return (0);
		
	return tmp ;
}

string Utility :: makeData (BigInt data) {
	ostringstream ostr ;
	ostr << data ;
	return (ostr.str());
}

BigInt Utility :: ByteToKilo ( BigInt byte ) {
	return byte/1024 ;
}

BigInt Utility :: BteToMega ( BigInt byte ) {
	return ( ByteToKilo(byte) / 1024 ) ;
}

BigInt Utility :: ByteToGega ( BigInt byte ) {
	return ( BteToMega(byte) / 1024 ) ;
}

BigInt Utility :: KiloToByte ( BigInt byte  ) {
	return byte * 1024 ;
}

BigInt Utility :: MegaToByte ( BigInt bytes ) {
	return ( KiloToByte(bytes) * 1024) ;
}

BigInt Utility :: GegaToByte ( BigInt bytes ) { 
	return ( MegaToByte(bytes) * 1024) ;
}

BigInt Utility :: GetSize ( HANDLE hFile ) {
	LARGE_INTEGER bInt ;
	
	if ( ! GetFileSizeEx ( hFile , &bInt ) ) {
		return (-1);
	}
	
	return (bInt.QuadPart) ;
}

BigInt Utility :: GetSize ( const string& fileName ) {
	HANDLE hFile = CreateFile(fileName.c_str(),GENERIC_READ,0,NULL,OPEN_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL); 
	if ( hFile == INVALID_HANDLE_VALUE ) {
		return -1 ;
	}
	
	BigInt size = GetSize(hFile);
	CloseHandle(hFile);
	
	return size ;
}

int Utility :: GetSplitterNumber ( BigInt fileSizeInBytes , BigInt splitSizeInBytes ) {
	int number = fileSizeInBytes / splitSizeInBytes ;
	
	if ( fileSizeInBytes % splitSizeInBytes != 0 )
		number++;
		
	return number ;
}

vector<string> Utility :: GetSplitterName ( const string& fileName ) {
	string file = fileName.substr(0,fileName.find_last_of('-')+1);

	int i = 0 ;
	bool more = true ;
	vector<string> result ;
	
	while  ( more ) {
		string newName = setSplitName( file , i++ );
		ifstream fin(newName.c_str()) ;
		
		if ( fin.fail() ) {
			more = false ;
		}
		else	
			result.push_back( newName ) ;
		
		fin.close();
	}
	
	return result ;
}

string Utility :: setSplitName (const string& fileName,int count)  {
	string result = fileName ;
	ostringstream oss ;
	oss << count ;
		
	result += oss.str();
	result += ".SPLITTER" ; // add ext
	
	return result ;
}
