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
	int width, height;
	HPEN hpen = CreatePen(PS_SOLID,0,RGB(0,0,0));
	
	SelectObject(hDc, hpen);
	height = field.bottom -field.top;
	width = field.right - field.left;
	int Oy = (double)height*std::abs(by)/(by-ay);
	int Ox = (double)width*std::abs(ax) / (bx - ax);
	MoveToEx(hDc, 0, Oy,0);
	LineTo(hDc, width,  Oy);
	MoveToEx(hDc, Ox, 0, 0);
	LineTo(hDc, Ox, height);
	std::vector<double> funcres;
	HPEN hpenr = CreatePen(PS_SOLID, 0, RGB(255, 0, 0));
	DeleteObject(hpen);
	SelectObject(hDc, hpenr);
	
	for (int i = 1; i < 2000;i++) {
		funcres.push_back(func.calc(ax+((double)i/2000)*(bx-ax),0));
		double f1 = func.calc(ax + ((double)(i-1) / 2000) * (bx-ax), 0);
		double x1 =  ((double)(i - 1) / 2000);
		double f2 = func.calc(ax+((double)i/2000)*(bx-ax),0);
		double x2 = ((double)i/ 2000);
		MoveToEx(hDc,x1*(double)width,Oy- (f1<0 ? (double)(height - Oy)*f1 / std::abs(ay) : (double)(Oy)*f1 / std::abs(by)),  0);
	    LineTo(hDc,x2*(double)width,Oy-  (f2<0? (double)(height-Oy)*f2 / std::abs(ay): (double)(Oy)*f2 / std::abs(by)));
	}
	
	DeleteObject(hpenr);

	
}
void save(HWND hw, HDC hDc,RECT field) {
	BITMAPFILEHEADER bfh;
	BITMAPINFOHEADER bih;
	BITMAPINFO bi;
	
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

	BYTE* memory;
	HDC     hDcC = CreateCompatibleDC(hDc);
	HBITMAP hBitmap = CreateDIBSection(hDc, &bi, DIB_RGB_COLORS, (void**)&memory, NULL, 0);;
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
			GetWindowText(hEdit, lptext, 1000);
			text = lptext;
			newnodefunc = new ariphmetic_node(text);
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
			//	SetWindowText(hEdit, newnodefunc->calc_str(1, 1).c_str());
				newnodefunc->calc_str(1, 1).c_str();
				nodefunc = newnodefunc;
				hDc = GetDC(MainWindow);
				Rectangle(hDc, 0, 0, Rw.right, Rw.bottom);
				DrawFunc(hDc, Rw, *nodefunc, ax, bx, ay, by);
				ReleaseDC(MainWindow, hDc);

			}
			catch (const char*e) { SetWindowText(hEdit, e);  }

		}/* сообщение от управляющих элементов */
		return TRUE;
	case WM_CLOSE:
		EndDialog(hw, 0);
		PostQuitMessage(0);
		
	}
	return FALSE;
}

