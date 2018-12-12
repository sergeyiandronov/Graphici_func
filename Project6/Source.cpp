#include <windows.h>
#include "resource.h"
#include "ariphmetic_parser.h"

LRESULT CALLBACK MainWinProc(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK DlgProc(HWND hw, UINT msg, WPARAM wp, LPARAM lp);
ariphmetic_node *nodefunc = nullptr;
double ax, bx, ay, by;
HINSTANCE h;
HWND MainWindow;

void DrawFunc(HDC hDc, RECT field, ariphmetic_node func,double ax,double bx,double ay,double by) {
	
	int width, height ;
	
	int tenthx = round(log10(bx-ax));
	int tenthy =round(log10(by-ay));
	tenthx--;
    tenthy--;
	double tenx=pow(10,double(tenthx));
	double teny = pow(10, double(tenthy));
	
	height = field.bottom -field.top;
	width = field.right - field.left;
	int Oy = (double)height*(max(ay,by))/(by-ay);
	int Ox = (double)width*(-min(ax,bx)) / (bx - ax);
	
	HPEN hpengr = CreatePen(PS_SOLID, 0, RGB(160, 160, 160));
	SelectObject(hDc, hpengr);
	for (double i = double(ax<0 ? (int)(ax / pow(10, tenthx))*pow(10, tenthx) : pow(10, tenthx)); i<=bx; i += tenx) {
		
		MoveToEx(hDc, ((i - ax)*width) / (bx - ax), 0, 0);
		LineTo(hDc, ((i - ax)*width) / (bx - ax), height);
		std::string is = std::to_string((abs(i)<0.000001 ? 0: i));
		TextOut(hDc, ((i - ax)*width) / (bx - ax)+1,height-20,is.c_str(),(i<0?abs(tenthx)+5:abs(tenthx)+4 ));
	}
	for (double i = double(ay<0 ? (int)(ay/pow(10, tenthy))*pow(10, tenthy) : pow(10, tenthy)); i<=by; i += teny) {
		
		MoveToEx(hDc, 0, height - ((i - ay)*height) / (by - ay), 0);
		LineTo(hDc, width, height - ((i - ay)*height) / (by - ay));
		std::string is = std::to_string((abs(i)<0.000001 ? 0 : i));
		TextOut(hDc,width- (i<0 ? abs(tenthy) + 5 : abs(tenthy) + 4)* 10+(i<0 ? 5: 0), height - ((i - ay)*height) / (by - ay)+1, is.c_str(), (i<0 ? abs(tenthy) + 5 : abs(tenthy) + 4));
	}
	DeleteObject(hpengr);
	HPEN hpen = CreatePen(PS_SOLID, 0, RGB(0, 0, 0));
	SelectObject(hDc, hpen);
	MoveToEx(hDc, 0, Oy,0);
	LineTo(hDc, width,  Oy);
	MoveToEx(hDc, Ox, 0, 0);
	LineTo(hDc, Ox, height);
	DeleteObject(hpen);
	
	HPEN hpenr = CreatePen(PS_SOLID, 0, RGB(255, 0, 0));
	SelectObject(hDc, hpenr);
	
	for (int i = 1; i < 100*(bx-ax);i++) {
		double f1y;
		double f2y;
		double f1 = func.calc(ax + ((double)(i-1) / (100*(bx-ax))) * (bx-ax));
		double x1 =  ((double)(i - 1) / (100*(bx-ax)));
		double f2 = func.calc(ax+((double)i/ (100*(bx-ax)))*(bx-ax));
		double x2 = ((double)i/ (100*(bx-ax)));
		f1y = (double)height / (by - ay)*f1;
		f2y = (double)height / (by - ay)*f2;
		if (f2  > by&&f1<ay) {

			
			continue;
		}
		if (f2  < ay&&f1>by) {

			
			continue;
		}
		

		
		MoveToEx(hDc,x1*(double)width,Oy-f1y ,  0);
	    LineTo(hDc,x2*(double)width,Oy-f2y);
	}
	
	DeleteObject(hpenr);
	
	
	
}
void save(HWND hw, HDC hDc,RECT field) {
	BITMAPFILEHEADER bfh;
	BYTE* memory;
	BITMAPINFO bi;
	BITMAPINFOHEADER bih;
	
	
	
	int height = field.bottom - field.top;
	int width = field.right - field.left;
	
	bih.biSize = sizeof(bih);
	bih.biWidth = width;
	bih.biHeight = height;
	bih.biPlanes = 1;
	bih.biBitCount = 32;
	bih.biCompression = BI_RGB;
	bih.biSizeImage = width * height*(32/ 8);
	bih.biXPelsPerMeter = 0;
	bih.biYPelsPerMeter = 0;
	bih.biClrUsed = 0;
	bih.biClrImportant = 0;
	bi.bmiHeader = bih;
	bfh.bfType = 0x4d42;
	bfh.bfSize = (DWORD)(sizeof(BITMAPFILEHEADER) + bih.biSize + bih.biSizeImage); ;
	bfh.bfReserved1 = 0;
	bfh.bfReserved2 = 0;
	bfh.bfOffBits = sizeof(BITMAPFILEHEADER) + bih.biSize;

	
	HDC     hDcC = CreateCompatibleDC(hDc);
	HBITMAP hBitmap = CreateDIBSection(hDc, &bi, DIB_RGB_COLORS, (void**)&memory, NULL, 0);
	HGDIOBJ old_obj = SelectObject(hDcC, hBitmap);
	BOOL    bRet = BitBlt(hDcC, 0, 0, width, height, hDc, field.left, field.top, SRCCOPY);
	OPENFILENAME ofn;//Указатель на структуру с данными инициализации диалогового окна
	char strFilter[] = "Файлы данных *.bmp\0";
	char strFileName[MAX_PATH] = "";
	memset(&ofn, 0, sizeof(OPENFILENAME));//Обнуление ofn
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = MainWindow;
	ofn.lpstrFilter = strFilter;
	ofn.lpstrFile = strFileName;//Буфер для имени файла
	ofn.nMaxFile = MAX_PATH;//Размер файла
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_SHOWHELP | OFN_OVERWRITEPROMPT;
	strcpy_s(strFileName, ofn.lpstrFile);
	if (GetSaveFileName(&ofn)) {

		HANDLE hFile = CreateFile(
			ofn.lpstrFile, GENERIC_WRITE,
			0, NULL, OPEN_ALWAYS,
			FILE_ATTRIBUTE_NORMAL, NULL);

		if (!(hFile == INVALID_HANDLE_VALUE)) {


			DWORD dwWritten = 0;
			WriteFile(hFile, &bfh, sizeof(BITMAPFILEHEADER), &dwWritten, NULL);
			WriteFile(hFile, &bih, sizeof(BITMAPINFOHEADER), &dwWritten, NULL);
			WriteFile(hFile, memory, bih.biSizeImage, &dwWritten, NULL);
			CloseHandle(hFile);
		}
	}
	// clean up
	SelectObject(hDcC, old_obj);
	DeleteDC(hDcC);
	DeleteObject(hBitmap);

}
int WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR cmdline, int ss) {
	WNDCLASS wc;
	wc.style = CS_HREDRAW | CS_VREDRAW;;
	wc.lpfnWndProc = MainWinProc;
	wc.cbClsExtra = wc.cbWndExtra = 0;
	wc.hInstance = hInst;
	wc.hIcon = NULL;
	wc.hCursor = NULL;
	wc.hbrBackground = 0;
	wc.lpszMenuName = NULL;
	wc.lpszClassName = "Example 4 MainWnd Class";
	if (!RegisterClass(&wc)) return FALSE;
	HMENU hMenu = LoadMenu(hInst,MAKEINTRESOURCE(IDR_MENU1));
	/* создаем главное окно и отображаем его */
	HWND hMainWnd = CreateWindow("Example 4 MainWnd Class", "EXAMPLE 4", WS_OVERLAPPEDWINDOW,
		0, 0, CW_USEDEFAULT, CW_USEDEFAULT, NULL, hMenu, hInst, NULL);
	if (!hMainWnd) return FALSE;
	ShowWindow(hMainWnd, ss);
	UpdateWindow(hMainWnd);
	h = hInst;
	MainWindow = hMainWnd;

	/* Цикл обработки событий: */
	MSG msg;
	while (GetMessage(&msg, (HWND)NULL, 0, 0)) {
		
		TranslateMessage(&msg);
		DispatchMessage(&msg);
		
		
	}
	return msg.wParam;
}

LRESULT CALLBACK MainWinProc(HWND hw, UINT msg, WPARAM wp, LPARAM lp) {
	/* Обработка сообщений главного окна */

	RECT Rw;
	HDC hDc;
	HWND dlg;
	
	switch (msg) {
	case WM_SIZE:
		
		return 0;
	case WM_CREATE:
		
		dlg=CreateDialog(h, MAKEINTRESOURCE(IDD_DIALOG1), hw, DlgProc);

		ShowWindow(dlg, SW_SHOW);
		
		/* ... */
		return 0;
	case WM_PAINT:
		
		hDc = GetDC(hw);
	    
	    
		
		GetClientRect(hw,&Rw);
		Rectangle(hDc,0,0,Rw.right,Rw.bottom);
		if (nodefunc != nullptr) {
			DrawFunc(hDc, Rw, *nodefunc,ax,bx,ay,by);
		}
		
		ReleaseDC(hw,hDc);

		break;
	
	case WM_COMMAND:
		
		if ((HIWORD(wp) == 0) && (LOWORD(wp) == ID_40001)) {
			hDc = GetDC(hw);
			GetClientRect(hw, &Rw);
			save(hw,hDc,Rw);
			ReleaseDC(hw, hDc);
		}
		
		/* ... */
		return 0;
	
		/* ... */
	
	case WM_DESTROY:
		/* ... */
		PostQuitMessage(0);
		return 0;
		/* ... */
	}
	return DefWindowProc(hw, msg, wp, lp);
}
BOOL CALLBACK DlgProc(HWND hw, UINT msg, WPARAM wp, LPARAM lp) {
	HWND hEdit;
	HWND hEax;
	HWND hEay;
	HWND hEbx;
	HWND hEby;
	std::istringstream strax;
	std::istringstream strbx;
	std::istringstream stray;
	std::istringstream strby;
	ariphmetic_node *newnodefunc=nullptr;

	LPSTR lptext = new char[100];
	std::string text;
	RECT Rw;
	HDC hDc;
	switch (msg) {
	case WM_INITDIALOG: /* сообщение о создании диалога */
		hEax = GetDlgItem(hw, 2);
		hEbx = GetDlgItem(hw, 3);
		hEay = GetDlgItem(hw, 1);
		hEby = GetDlgItem(hw, 0);
		SetWindowText(hEax, "-1");
		SetWindowText(hEbx, "1");
		SetWindowText(hEay, "-1");
		SetWindowText(hEby, "1");
		return TRUE;
	case WM_COMMAND:
		if (LOWORD(wp) == 5) {
			hEdit = GetDlgItem(hw, 4);
			hEax = GetDlgItem(hw, 2);
			hEbx = GetDlgItem(hw, 3);
			hEay = GetDlgItem(hw, 1);
			hEby = GetDlgItem(hw, 0);
			
			GetClientRect(MainWindow, &Rw);
			GetWindowText(hEax, lptext, 1000);
			strax = std::istringstream(lptext);
			if (!(strax >> ax)) { ax = -1; SetWindowText(hEax,"-1" );}
			
			GetWindowText(hEbx, lptext, 1000);
			strbx = std::istringstream(lptext);
			if (!(strbx >> bx)) { bx = 1 ; SetWindowText(hEbx, "1");}
			
			GetWindowText(hEay, lptext, 1000);
			stray = std::istringstream(lptext);
			if (!(stray >> ay)) { ay = -1; SetWindowText(hEay, "-1");}
			
			
			GetWindowText(hEby, lptext, 1000);
			strby = std::istringstream(lptext);
			if (!(strby >> by)) { by = 1; SetWindowText(hEby, "1");}
			
			

			try {
			
				if (ax >= bx || ay>=by|| (bx-ax)>200|| (bx - ax)<0.002) {
					throw "недопустимый  диапазон";
				}
				GetWindowText(hEdit, lptext, 1000);
				text = lptext;
				newnodefunc = new ariphmetic_node(text);
				newnodefunc->check_except();
				nodefunc = newnodefunc;
				hDc = GetDC(MainWindow);
				Rectangle(hDc, 0, 0, Rw.right, Rw.bottom);
				DrawFunc(hDc, Rw, *nodefunc, ax, bx, ay, by);
				ReleaseDC(MainWindow, hDc);

			}
			catch (const char*e) { MessageBox(NULL,e,NULL,MB_OK); }

		} 
		return TRUE;
	case WM_CLOSE:
		EndDialog(hw, 0);
		PostQuitMessage(0);
		
	}
	return FALSE;
}

