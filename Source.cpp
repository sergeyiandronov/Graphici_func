#include <windows.h>
#include "ariphmetic_parser.h"
LRESULT CALLBACK MainWinProc(HWND, UINT, WPARAM, LPARAM);
ariphmetic_node *nodefunc = nullptr;
void DrawFunc(HDC hDc, RECT field, ariphmetic_node func) {
	int width, height;
	HPEN hpen;
	hpen = CreatePen(PS_SOLID,0,0);
	SelectObject(hDc, hpen);
	height = field.bottom -field.top;
	width = field.right - field.left;
	int Oy = height / 2;
	int Ox = width / 2;
	MoveToEx(hDc, 0, Oy,0);
	LineTo(hDc, width,  Oy);
	MoveToEx(hDc, Ox, 0, 0);
	LineTo(hDc, Ox, height);
	std::vector<double> funcres;
	for (int i = 1; i < 2000;i++) {
		funcres.push_back(func.calc(-10+((double)i/2000)*20,0));
		double f1 = func.calc(-10 + ((double)(i-1) / 2000) * 20, 0);
		double x1 =  -0.5+((double)(i - 1) / 2000);
		double f2 = func.calc(-10+((double)i/2000)*20,0);
		double x2 = -0.5+((double)i/ 2000) ;
		MoveToEx(hDc,Ox+ x1*(double)width,Oy-f1*Oy,  0);
	    LineTo(hDc,Ox+x2*(double)width,Oy-f2*Oy);
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

	/* создаем главное окно и отображаем его */
	HWND hMainWnd = CreateWindow("Example 4 MainWnd Class", "EXAMPLE 4", WS_OVERLAPPEDWINDOW,
		0, 0, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInst, NULL);
	if (!hMainWnd) return FALSE;
	ShowWindow(hMainWnd, ss);
	UpdateWindow(hMainWnd);
	

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
	HWND hWinBtn = 0;
	int sx =0; //ширина
	int sy = 0;//
	LPSTR lptext = new char[100];
	std::string text;
	RECT Rw;
	HDC hDc;
	PAINTSTRUCT ps;
	
	switch (msg) {
	case WM_SIZE:
	     hWinBtn = FindWindowEx(hw, NULL, NULL, "My edit"); 
		 GetClientRect(hw, &Rw);
		 sx = LOWORD(lp); //ширина
		 sy = HIWORD(lp);//высота

		SetWindowPos(
			hWinBtn,           // дескриптор окна
			HWND_TOP,       // дескриптор порядка размещения
			10,              // позиция по горизонтали
			sy-60,              // позиция по вертикали
			200,              // ширина
			30,              // высота
			0 // флажки позиционирования окна
			);
		hWinBtn = FindWindowEx(hw,NULL,NULL, "My button");
		SetWindowPos(
			hWinBtn,           // дескриптор окна
			HWND_TOP,       // дескриптор порядка размещения
			280,              // позиция по горизонтали
			sy - 60,              // позиция по вертикали
			90,              // ширина
			30,              // высота
			0 // флажки позиционирования окна
		);


		return 0;
	case WM_CREATE:
		hWinBtn=CreateWindow("edit", "My edit", WS_CHILD  | WS_VISIBLE| WS_BORDER,
			0, 0, 500,500, hw, NULL, NULL, NULL);
		hWinBtn = CreateWindow("button", "My button", WS_CHILD | WS_VISIBLE|WS_BORDER ,
			0, 0, 1, 1, hw, NULL, NULL, NULL);

		/* ... */
		return 0;
	case WM_PAINT:
		
		hDc = GetDC(hw);
		
		GetClientRect(hw,&Rw);
		Rectangle(hDc,0,0,Rw.right,Rw.bottom);
		if (nodefunc != nullptr) {
			DrawFunc(hDc, Rw, *nodefunc);
		}
		
		ReleaseDC(hw,hDc);
		break;
	case WM_COMMAND:
		if (HIWORD(wp) == BN_CLICKED) {
			hWinBtn = FindWindowEx(hw,NULL,NULL,"My edit");
			GetWindowText(hWinBtn,lptext,1000);
			text = lptext;
			nodefunc=new ariphmetic_node(text);
			GetClientRect(hw,&Rw);
			
			try {
			SetWindowText(hWinBtn, nodefunc->calc_str(1, 1).c_str());
			hDc = GetDC(hw);
			DrawFunc(hDc, Rw, *nodefunc);
			ReleaseDC(hw, hDc);

			}
			catch (const char*e) { SetWindowText(hWinBtn, e); }
			
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
