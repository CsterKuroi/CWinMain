#include <windows.h>
#include <io.h>
#include <fcntl.h>
#include <stdlib.h>

int hahaha()
{
	int x;
	x = 1 + 1;
	return x;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC         hdc;
	PAINTSTRUCT ps;
	RECT        rect;
	static HWND hEdit;
	static HWND hBtn;
	//	 TCHAR buf[1000];
	switch (message)
	{
	case WM_CREATE:
		//PlaySound (TEXT ("hellowin.wav"), NULL, SND_FILENAME | SND_ASYNC) ;
		hEdit = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("EDIT"), TEXT(""),
			WS_CHILDWINDOW | ES_MULTILINE | WS_VSCROLL, 10, 10, 300, 200,
			hwnd, NULL, NULL, NULL);
		ShowWindow(hEdit, SW_SHOW);
		hBtn = CreateWindowEx(WS_EX_WINDOWEDGE, TEXT("BUTTON"), TEXT("select"),
			WS_CHILDWINDOW, 320, 10, 50, 30,
			hwnd, NULL, NULL, NULL);
		ShowWindow(hBtn, SW_SHOW);
		return 0;
	case WM_COMMAND:
		if (lParam == (LPARAM)hBtn)
		{
			/*GetWindowText(hEdit, buf, 1000);
			if(!strlen(buf))
			{
			MessageBox(hwnd, TEXT("文本框为空!"), TEXT("错误"), MB_OK);
			}*/
			OPENFILENAME ofn;
			TCHAR szFileName[255] = "";
			//定义过滤字符串
			char szFilter[255] = TEXT("文本文件(*.txt)\0*.txt\0所有文件(*.*)\0*.*\0");
			//为ofn结构申请内存并好初始化为0
			ZeroMemory(&ofn, sizeof(OPENFILENAME));
			//初始化ofn结构
			ofn.lStructSize = sizeof(OPENFILENAME);
			ofn.hwndOwner = hwnd;
			ofn.lpstrFilter = szFilter;
			ofn.nFilterIndex = 1;
			ofn.nMaxFile = 255;
			ofn.lpstrTitle = "请选择要打开的文件";
			ofn.lpstrFile = szFileName;
			ofn.lpstrInitialDir = "C:\\";
			ofn.Flags = OFN_FILEMUSTEXIST;

			if (GetOpenFileName(&ofn))
			{
				//MessageBox(hwnd, szFileName, TEXT("错误"), MB_OK);
				int fhandle = _open(szFileName, _O_RDONLY | _O_BINARY);
				int hhh = hahaha();

				int flen = _filelength(fhandle);
				char *p = (char *)malloc(flen + 1);
				memset(p, '\0', flen + 1);
				_read(fhandle, (void*)p, flen);
				_close(fhandle);
				SetWindowText(hEdit, p);
				free(p);
			}
		}
		return 0;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);

		GetClientRect(hwnd, &rect);

		//          DrawText (hdc, TEXT ("Hello!"), -1, &rect,
		//                    DT_SINGLELINE | DT_CENTER | DT_VCENTER) ;

		EndPaint(hwnd, &ps);
		return 0;

	case WM_DESTROY:
		DestroyWindow(hEdit);
		DestroyWindow(hBtn);
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, message, wParam, lParam);
}

int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    PSTR szCmdLine, int iCmdShow)
{
     static TCHAR szAppName[] = TEXT ("HelloWin") ;
     HWND         hwnd ;
     MSG          msg ;
     WNDCLASS     wndclass ;

     wndclass.style         = CS_HREDRAW | CS_VREDRAW ;
     wndclass.lpfnWndProc   = WndProc ;
     wndclass.cbClsExtra    = 0 ;
     wndclass.cbWndExtra    = 0 ;
     wndclass.hInstance     = hInstance ;
     wndclass.hIcon         = LoadIcon (NULL, IDI_APPLICATION) ;
     wndclass.hCursor       = LoadCursor (NULL, IDC_ARROW) ;
     //wndclass.hbrBackground = (HBRUSH) GetStockObject (WHITE_BRUSH) ;
	 wndclass.hbrBackground = (HBRUSH)GetSysColorBrush(COLOR_BTNFACE);
     wndclass.lpszMenuName  = NULL ;
     wndclass.lpszClassName = szAppName ;

     if (!RegisterClass (&wndclass))
     {
          MessageBox (NULL, TEXT ("This program requires Windows NT!"), 
                      szAppName, MB_ICONERROR) ;
          return 0 ;
     }
     
     hwnd = CreateWindow (szAppName,                  // window class name
                          TEXT ("SHELL"),             // window caption
                          WS_OVERLAPPEDWINDOW,        // window style
                          CW_USEDEFAULT,              // initial x position
                          CW_USEDEFAULT,              // initial y position
                          CW_USEDEFAULT,              // initial x size
                          CW_USEDEFAULT,              // initial y size
                          NULL,                       // parent window handle
                          NULL,                       // window menu handle
                          hInstance,                  // program instance handle
                          NULL) ;                     // creation parameters
     
     ShowWindow (hwnd, iCmdShow) ;
     UpdateWindow (hwnd) ;
     
     while (GetMessage (&msg, NULL, 0, 0))
     {
          TranslateMessage (&msg) ;
          DispatchMessage (&msg) ;
     }
     return msg.wParam ;
}