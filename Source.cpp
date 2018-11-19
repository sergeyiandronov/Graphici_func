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
	HPEN hpen = CreatePen(PS_SOLID,0,0);
	
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
	DeleteObject(hpen);
	
	
	for (int i = 1; i < 2000;i++) {
		funcres.push_back(func.calc(ax+((double)i/2000)*(bx-ax),0));
		double f1 = func.calc(ax + ((double)(i-1) / 2000) * (bx-ax), 0);
		double x1 =  ((double)(i - 1) / 2000);
		double f2 = func.calc(ax+((double)i/2000)*(bx-ax),0);
		double x2 = ((double)i/ 2000);
		MoveToEx(hDc,x1*(double)width,Oy- (f1<0 ? (double)(height - Oy)*f1 / std::abs(ay) : (double)(Oy)*f1 / std::abs(by)),  0);
	    LineTo(hDc,x2*(double)width,Oy-  (f2<0? (double)(height-Oy)*f2 / std::abs(ay): (double)(Oy)*f2 / std::abs(by)));
	}
	
	DeleteObject(hpen);

	
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

