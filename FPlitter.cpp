
// FPliter++
// Splitter and Mereger Win32 Program

// define for InitCommonControlsEx - using for init progress bar
#define _WIN32_IE 	 0x0300

// Windows Header
#include <windows.h>
#include <tchar.h>

// using for calling InitCommonControlsEx function - application must link with lcomctl32.lib
#include <commctrl.h> 		

// Resource Files
#include "resource\resource.h"	

// Spliter & Merger Classes
#include "src\Utility.h"
#include "src\Splitter.h"
#include "src\Merger.h"


/**************************************************************************
 *							Enums Type
 *************************************************************************/
enum OPERATION_TYPE {
	SPLIT ,
	MERGE 
} ;

/**************************************************************************
 *						Functions Declerations
 *************************************************************************/
BOOL CALLBACK WndProc 	(HWND hWnd , UINT uMsg , WPARAM wParam , LPARAM lParam );  // Message Processing
BOOL CALLBACK SplitProc	(HWND hWnd , UINT uMsg , WPARAM wParam , LPARAM lParam );  // split processing
BOOL CALLBACK MergeProc	(HWND hWnd , UINT uMsg , WPARAM wParam , LPARAM lParam );  // Merge Processing

DWORD WINAPI StartSplitting  (LPVOID lpParam) ;									   // Splitting Function
DWORD WINAPI StartMerging    (LPVOID lpParam) ;									   // Merging Function
VOID  ShowFileChooser  		 (HWND hwnd , OPERATION_TYPE type );				   // show File Chooser Menu
string GetFileName 		     (const string& path );								   // extrat file name from path 

/**************************************************************************
 *							 Global Data
 *************************************************************************/
HINSTANCE g_hInstance ;		// application instance 
string    g_FilePath  ;		// string contain path and name of file to splitting/merging
HANDLE	  g_hThread	  ;		// Handle to Thread
DWORD	  g_dwTreadID ;		// Thread ID 
BigInt	  g_SplitSize ;		// File Split Size 

/**************************************************************************
 *							 Main Entry Point
 *************************************************************************/
int WINAPI _tWinMain (HINSTANCE hInstance , HINSTANCE hPrev , LPTSTR szCmdLine , int iCmdShow ) {
	g_hInstance = hInstance ;
	
	// some compnent need initialization
	INITCOMMONCONTROLSEX icmcex;
	icmcex.dwSize = sizeof(INITCOMMONCONTROLSEX);
	icmcex.dwICC  = ICC_PROGRESS_CLASS;		// Load progress bar control class
	InitCommonControlsEx(&icmcex);
	
	// Show Main Dialog
	return DialogBox ( hInstance , MAKEINTRESOURCE( IDD_MAIN ) , NULL , WndProc ) ;
}

// Message Processing
BOOL CALLBACK WndProc ( HWND hWnd , UINT uMsg , WPARAM wParam , LPARAM lParam ) {
	switch ( uMsg ) {
		case WM_INITDIALOG :
			// Set Icon To Application
            HICON hIcon = LoadIcon(g_hInstance,MAKEINTRESOURCE(IDI_ICON1));
            SendMessage(hWnd,WM_SETICON,ICON_BIG,(LPARAM)hIcon);
		return TRUE ;
		
		case WM_COMMAND :
			switch ( LOWORD ( wParam ) ) {
				case IDC_SPLITCHOOSER :
					ShowFileChooser(hWnd,SPLIT);	// show file chooser for Splite
				break ;
				
				case IDC_MERGECHOOSER :
					ShowFileChooser(hWnd,MERGE);	// show file chooser for merging files
				break ;
				
				case IDC_SPLITBTN :
					int length = GetWindowTextLength ( GetDlgItem(hWnd,IDC_SPLITFILE ) ) ;
					if ( length != 0 ) {
						TCHAR* path = (TCHAR*) GlobalAlloc ( GPTR , length + 1 );
						GetDlgItemText(hWnd,IDC_SPLITFILE,path,length + 1 );
						g_FilePath = path ;
						GlobalFree(path);
						
						// get split size in bega byte 
						int sizeLength = GetWindowTextLength ( GetDlgItem(hWnd,IDC_SIZEBOX ) );
						if ( sizeLength != 0 ) {
							TCHAR* number = (TCHAR*) GlobalAlloc( GPTR , sizeLength + 1  ) ;
							GetDlgItemText(hWnd , IDC_SIZEBOX , number , sizeLength + 1  ) ;
							g_SplitSize = Utility::MegaToByte ( Utility:: makeData ( number ) ) ;
							GlobalFree(number) ;
							
							// show dialog box
							DialogBox(g_hInstance,MAKEINTRESOURCE(IDD_PROCESSING),hWnd,SplitProc);
						}
						else {
							MessageBox(hWnd,TEXT("Please set the size for splitting file"),TEXT("type the split size!"),
							MB_ICONERROR);
						}
					}
					else {
						MessageBox(hWnd,TEXT("Please Select File to Split first"),TEXT("Choose Object First!"),
							MB_ICONERROR);
					}
				break ;
				
				case IDC_MERGEBTN :
					length = GetWindowTextLength ( GetDlgItem(hWnd,IDC_MERGEFILE ) ) ;
					if ( length != 0 ) {
						TCHAR* path = (TCHAR*) GlobalAlloc ( GPTR , length + 1 );
						GetDlgItemText(hWnd,IDC_MERGEFILE,path,length + 1 );
						g_FilePath = path ;
						GlobalFree(path);
						
						// show dialog box
						DialogBox(g_hInstance,MAKEINTRESOURCE(IDD_PROCESSING),hWnd,MergeProc);
					}
					else {
						MessageBox(hWnd,TEXT("Please select one of the Mergerd files first"),TEXT("Choose Object First!"),
							MB_ICONERROR);
					}
				break ;
				
			}
		return TRUE ;
		
		case WM_CLOSE : {
			MessageBox(NULL,TEXT("Thanks you for using FPlitter++\nBy: Wajdy Essam\nEmail: wajdyessam@hotmail.com\n\nFPlitter++ under GPL v.2"),
				TEXT("press ok to exit ") , MB_OK );
			
			EndDialog(hWnd,0);
		}
		return TRUE ;
	}
	
	return FALSE ;
}


// Splitting Processing Message
BOOL CALLBACK SplitProc	(HWND hWnd , UINT uMsg , WPARAM wParam , LPARAM lParam ) {
	switch ( uMsg ) {
		case WM_INITDIALOG :
			g_hThread = CreateThread( NULL , 0 , StartSplitting , hWnd , 0 ,& g_dwTreadID) ;
			SetDlgItemText ( hWnd , IDC_STATE , TEXT("Initiliazing now")  );
			SetDlgItemText ( hWnd , IDC_SELECTEDFILE , GetFileName( g_FilePath ).c_str() );
			SetDlgItemText ( hWnd , IDC_TYPE , TEXT("Splitting")  );
			SetDlgItemText ( hWnd , IDCS_MSIZE , TEXT("Splitting")  );
		return TRUE ;
		
		case WM_COMMAND:
			switch ( LOWORD ( wParam ) ) {
				case IDC_FINISH :
					SendMessage(hWnd,WM_CLOSE,0,0);
				break ;
			}
		return TRUE ;
		
		case WM_CLOSE :
			TerminateThread(g_hThread,1);
			EndDialog(hWnd,0);
		return TRUE ;
	}
	
	return FALSE ;
}

// Merging Processing Message
BOOL CALLBACK MergeProc	(HWND hWnd , UINT uMsg , WPARAM wParam , LPARAM lParam ) {
	switch ( uMsg ) {
		case WM_INITDIALOG :
			g_hThread = CreateThread( NULL , 0 , StartMerging , hWnd , 0 ,& g_dwTreadID) ;
			SetDlgItemText ( hWnd , IDC_STATE , TEXT("Initiliazing now")  );
			SetDlgItemText ( hWnd , IDC_SELECTEDFILE , GetFileName( g_FilePath ).c_str() );
			SetDlgItemText ( hWnd , IDC_TYPE , TEXT("Merging")  );
			SetDlgItemText ( hWnd , IDCS_MSIZE , TEXT("Merging")  );
		return TRUE ;
		
		case WM_COMMAND:
			switch ( LOWORD ( wParam ) ) {
				case IDC_FINISH :
					SendMessage(hWnd,WM_CLOSE,0,0);
				break ;
			}
		return TRUE ;
		
		case WM_CLOSE :
			TerminateThread(g_hThread,1);
			EndDialog(hWnd,0);
		return TRUE ;
	}
	
	return FALSE ;
}

// Splitting Function
DWORD WINAPI StartSplitting  (LPVOID lpParam) {
	HWND hWnd = (HWND) lpParam ;
	
	BigInt fileSize   = Utility::GetSize ( g_FilePath ) ;
	
	SetDlgItemInt  ( hWnd , IDCS_MSIZE , fileSize , TRUE );
	SetDlgItemText ( hWnd , IDC_STATE , TEXT("Start Splitting ")  );
	
	if ( g_SplitSize > fileSize ) {
		MessageBox(hWnd,TEXT("splite size is bigger than file size"),TEXT("Choose big size for splitting"),
			MB_ICONERROR);
			
		SendMessage(hWnd,WM_CLOSE,0,0);
		
		return (0);
	}
	
	Splitter splitter ( g_FilePath , fileSize,  g_SplitSize );
	int upper=splitter.GetSplitNumber() ,lower=0;
	
	HWND hwndProgress = GetDlgItem(hWnd,IDC_PROGRESS1);	
	SendMessage(hwndProgress, PBM_SETRANGE32, (WPARAM)lower, (LPARAM)upper);
	SendMessage(hwndProgress, PBM_SETPOS, (WPARAM)0, 0);
		
	for (int i=0 ; i<upper ; i++) {
		SendMessage(hwndProgress,PBM_SETPOS,(WPARAM)i+1,0);
		SetDlgItemText ( hWnd , IDC_STATE , TEXT("splitting now")  );
		splitter.Split( i  );
	}
	
	SetDlgItemText ( hWnd , IDC_STATE , TEXT("Finish Splitting")  );
	splitter.Close();
	
	for (int i=5 ; i>0 ; i--) {
		CHAR Buffer[MAX_PATH];
		wsprintf(Buffer,TEXT("Finish (%d) "),i);
		SetDlgItemText(hWnd,IDC_FINISH,Buffer );
		Sleep( 1 * 1000 ) ;
	}
	
	SendMessage(hWnd,WM_CLOSE,0,0);
	return (0);
}

// Merging Functiion
DWORD WINAPI StartMerging ( LPVOID lpParam ) {
	HWND hWnd = (HWND) lpParam ;
	
	vector<string> filesName = Utility :: GetSplitterName ( g_FilePath );
	int upper= filesName.size() ,lower=0;
	
	HWND hwndProgress = GetDlgItem(hWnd,IDC_PROGRESS1);	
	SendMessage(hwndProgress, PBM_SETRANGE32, (WPARAM)lower, (LPARAM)upper);
	SendMessage(hwndProgress, PBM_SETPOS, (WPARAM)0, 0);
	
	Merger merger ( filesName[0] ) ;
	
	for (unsigned int i=0 ; i<filesName.size() ; i++ ) {
		SendMessage(hwndProgress,PBM_SETPOS,(WPARAM)i+1,0);
		SetDlgItemText ( hWnd , IDC_STATE , TEXT("Merging now")  );
		
		merger.Merge ( filesName[i] );
	}
	
	merger.Close();
	SetDlgItemText ( hWnd , IDC_STATE , TEXT("Finish Merging")  );
	
	
	for (int i=5 ; i>0 ; i--) {
		CHAR Buffer[MAX_PATH];
		wsprintf(Buffer,TEXT("Finish (%d) "),i);
		SetDlgItemText(hWnd,IDC_FINISH,Buffer );
		Sleep( 1 * 1000 ) ;
	}
	
	SendMessage(hWnd,WM_CLOSE,0,0);
	return (0);
}

// Display File Chooser Menu for Merger/Splitter 
VOID ShowFileChooser ( HWND hwnd , OPERATION_TYPE type) {
	OPENFILENAME ofn;
	TCHAR szFile[MAX_PATH];
		
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	
	if ( type == SPLIT ) {
		ofn.lpstrTitle = TEXT("Choose Any file you want to Split");
		ofn.lpstrFilter = TEXT("All Files ( *.* )\0*.*\0\0");
	}
	else {	// type == MERGE
		ofn.lpstrTitle = TEXT("Choose one of the Merger Files");
		ofn.lpstrFilter = TEXT("Splitter Files \0*.SPLITTER\0\0");
	}
	
	ofn.lpstrFile = szFile;
	ofn.lpstrFile[0] = '\0';
	ofn.hwndOwner = hwnd;
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrInitialDir = NULL;
	ofn.lpstrFileTitle = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR ;
	
	DWORD returnValue =  GetOpenFileName(&ofn) ;	// show dialog 
	
	if ( returnValue && type == SPLIT )
		SetDlgItemText(hwnd,IDC_SPLITFILE,ofn.lpstrFile);
	
	else if ( returnValue  && type == MERGE ) 
		SetDlgItemText(hwnd,IDC_MERGEFILE,ofn.lpstrFile);
}

// extrat file name from path 
string GetFileName ( const string& s ) {
	string x = s.substr( s.find_last_of("\\") + 1 , s.length()) ;
	return x ;
}

