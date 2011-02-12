/* Separate values of text file into a csv */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "spFile.h"

/* #ifdef _WIN32 */
/* #include <windows.h> */
/* #endif */

/* #ifdef _WIN32 */

/* #define C_WINDOW_WIDTH 300 */
/* #define C_WINDOW_HEIGHT 200 */
/* #define C_BUTTON_WIDTH 50 */
/* #define C_BUTTON_HEIGHT 20 */
/* #define C_BUTTON_CLEARANCE 10 */

/* #define C_BUTTON_LOCATION_X 20 */
/* #define C_BUTTON_FLOCATION_Y 50 */

/* #define IDM_FILE_NEW 1 */

/* typedef unsigned int UNIT; */
/* /\* Callback procedure *\/ */
/* LRESULT CALLBACK WndProc(HWND hwnd, UNIT msg, WPARAM wParam, LPARAM lParam); */

/* void OpenDialog(HWND); */


/* /\* Main windows entry function *\/ */
/* int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, */
/* 		   LPSTR lpCmdLine, int nCmdShow) */
/* { */
/*     MSG msg; */

/*     WNDCLASS wc = {0};		/\* windows class *\/ */
/*     wc.lpszClassName = TEXT("cMain"); */
/*     wc.hInstance = hInstance; */
/*     wc.hbrBackground = GetSysColorBrush(COLOR_3DFACE); */
/*     wc.lpfnWndProc = WndProc; */
/*     wc.hCursor = LoadCursor(0, IDC_ARROW); */

/*     /\* register the window *\/ */
/*     RegisterClass(&wc); */

/*     /\* Create the window *\/ */
/*     CreateWindow(wc.lpszClassName,				/\* windows class name *\/ */
/* 		 TEXT("David Andrews - Council Benefits"),	/\* window name *\/ */
/* 		 WS_OVERLAPPEDWINDOW | WS_VISIBLE,		/\* style *\/ */
/* 		 150,						/\* x *\/ */
/* 		 150,						/\* y *\/ */
/* 		 300,						/\* window width *\/ */
/* 		 200,						/\* window height *\/ */
/* 		 0,						/\* parent window *\/ */
/* 		 0,						/\* menu handle *\/ */
/* 		 hInstance,					/\* instance handle *\/ */
/* 		 0); */

/*     /\* Main message loop *\/ */
/*     while(GetMessage(&msg, NULL, 0, 0)) */
/* 	{ */
/* 	    TranslateMessage(&msg); */
/* 	    DispatchMessage(&msg); */
/* 	} */

/*     return (int) msg.wParam; */
		 
/* } */

/* /\* Message Handling *\/ */
/* LRESULT CALLBACK WndProc(HWND hwnd, UNIT msg, WPARAM wParam, LPARAM lParam) */
/* { */
/*     switch(msg) */
/* 	{ */
/* 	case WM_CREATE: */
/* 	    { */
/* 		/\* Create two buttons *\/ */
/* 		CreateWindow(TEXT("button"),			/\* class name *\/ */
/* 			     TEXT("Open"),			/\* button name *\/ */
/* 			     WS_VISIBLE | WS_CHILD,		/\* style *\/ */
/* 			     C_BUTTON_LOCATION_X,		/\* x *\/ */
/* 			     C_BUTTON_FLOCATION_Y,		/\* y *\/ */
/* 			     C_BUTTON_WIDTH,			/\* button width *\/ */
/* 			     C_BUTTON_HEIGHT,			/\* button height *\/ */
/* 			     hwnd,				/\* parent *\/ */
/* 			     (HMENU) 1,				/\* menu handle *\/ */
/* 			     NULL, */
/* 			     NULL); */

/* 		CreateWindow(TEXT("button"),			/\* class name *\/ */
/* 			     TEXT("Save"),			/\* button name *\/ */
/* 			     WS_VISIBLE | WS_CHILD,		/\* style *\/ */
/* 			     C_BUTTON_LOCATION_X,		/\* x *\/ */
/* 			     C_BUTTON_FLOCATION_Y + */
/* 			     C_BUTTON_HEIGHT + */
/* 			     C_BUTTON_CLEARANCE,		/\* y *\/ */
/* 			     C_BUTTON_WIDTH,			/\* button width *\/ */
/* 			     C_BUTTON_HEIGHT,			/\* button height *\/ */
/* 			     hwnd,				/\* parent *\/ */
/* 			     (HMENU) 1,				/\* menu handle *\/ */
/* 			     NULL, */
/* 			     NULL); */
/* 		break; */
/* 	    } */
	    
/* 	case WM_COMMAND: */
/* 	    { */
/* 		if(LOWORD(wParam) == 1) */
/* 		    OpenDialog(hwnd); */

/* 		if(LOWORD(wParam) == 2) */
/* 		    PostQuitMessage(0); */

/* 		break; */
/* 	    } */

/* 	case WM_DESTROY: */
/* 	    { */
/* 		PostQuitMessage(0); */
/* 		break; */
/* 	    } */
	    
/* 	} */

/*     /\* process default *\/ */
/*     return DefWindowProc(hwnd, msg, wParam, lParam); */
/* } */

/* /\* call to open dialogue *\/ */
/* void OpenDialog(HWND hwnd) */
/* { */
/*     OPENFILENAME ofn; */

/*     TCHAR szFile[MAX_PATH]; */

/*     ZeroMemory(&ofn, sizeof(ofn)); */
/*     ofn.lStructSize= sizeof(ofn); */
/*     ofn.lpstrFile = szFile; */
/*     ofn.lpstrFile[0] = '\0'; */
/*     ofn.hwndOwner = hwnd; */
/*     ofn.nMaxFile = sizeof(szFile); */
/*     ofn.lpstrFilter = TEXT("All Files(*.*)\0*.*\0"); */
/*     ofn.nFilterIndex = 1; */
/*     ofn.lpstrInitialDir = NULL; */
/*     ofn.lpstrFileTitle= NULL; */
/*     ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST; */
    
/* } */

/* #else */

int main(int argc, char** argv)
{
    char ifile[255];
    char ofile[255];
    /* clock_t start; */
    if(argc < 2)
	{
	    printf("%s", "Enter Text File Name: ");
	    scanf("%s",ifile);

	    printf("%s", "Enter save file name: ");
	    scanf("%s", ofile);
	}
    else
	{
	    strcpy(ifile, argv[1]);
	    strcpy(ofile, argv[2]);
	}

    /* create new file */
    spfile* fp = spfile_new(ifile);

    if(!fp)
	return 1;
    
    /* set out file name */
    spfile_set_outpath(fp, ofile);

    start = clock();
    /* read file */
    spfile_read(fp);

    int rt_val = 0;
    if(!spfile_exe(fp))
	{
	    rt_val = 1;
	}

    /* delete spfile object */
    spfile_delete(&fp);

    /* printf("%s%f\n", "Exec time: ", */
    /* 	   ((double) clock() - start) / CLOCKS_PER_SEC); */
    
    /* char ch[2]; */
    /* printf("%s", "press any key: "); */
    /* scanf("%c", ch); */
    return rt_val;
}

/* #endif */	/* _WIN32 */
