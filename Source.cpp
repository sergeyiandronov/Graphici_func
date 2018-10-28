#include <windows.h>
LRESULT CALLBACK MainWinProc(HWND, UINT, WPARAM, LPARAM);
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
	
	RECT Rw;
	HDC hDc;
	switch (msg) {
	case WM_SIZE:
		 hWinBtn = GetWindow(hw,GW_CHILD);
		 sx = LOWORD(lp); //ширина
		 sy = HIWORD(lp);//высота

		SetWindowPos(
			hWinBtn,           // дескриптор окна
			HWND_TOP,       // дескриптор порядка размещения
			10,              // позиция по горизонтали
			sy-40,              // позиция по вертикали
			100,              // ширина
			30,              // высота
			0 // флажки позиционирования окна
			);
		hWinBtn = GetWindow(hWinBtn, GW_HWNDNEXT);
		SetWindowPos(
			hWinBtn,           // дескриптор окна
			HWND_TOP,       // дескриптор порядка размещения
			120,              // позиция по горизонтали
			sy - 40,              // позиция по вертикали
			90,              // ширина
			30,              // высота
			0 // флажки позиционирования окна
		);

		return 0;
	case WM_CREATE:
		hWinBtn=CreateWindow("edit", "My edit", WS_CHILD  | WS_VISIBLE|WS_BORDER,
			0, 0, CW_USEDEFAULT,CW_USEDEFAULT, hw, NULL, NULL, NULL);
		hWinBtn = CreateWindow("button", "My button", WS_CHILD | WS_VISIBLE | WS_BORDER,
			0, 0, CW_USEDEFAULT, CW_USEDEFAULT, hw, NULL, NULL, NULL);
		/* ... */
		return 0;
	case WM_PAINT:
		
		hDc = GetDC(hw);
		
		
		GetWindowRect(hw,&Rw);
		Rectangle(hDc,0,0,Rw.right,Rw.bottom);
		/* операции вывода */
		
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
