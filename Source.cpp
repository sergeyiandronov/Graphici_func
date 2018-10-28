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

	/* ������� ������� ���� � ���������� ��� */
	HWND hMainWnd = CreateWindow("Example 4 MainWnd Class", "EXAMPLE 4", WS_OVERLAPPEDWINDOW,
		0, 0, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInst, NULL);
	if (!hMainWnd) return FALSE;
	ShowWindow(hMainWnd, ss);
	UpdateWindow(hMainWnd);



	/* ���� ��������� �������: */
	MSG msg;
	while (GetMessage(&msg, (HWND)NULL, 0, 0)) {
		
		TranslateMessage(&msg);
		DispatchMessage(&msg);
		
	}
	return msg.wParam;
}

LRESULT CALLBACK MainWinProc(HWND hw, UINT msg, WPARAM wp, LPARAM lp) {
	/* ��������� ��������� �������� ���� */
	HWND hWinBtn = 0;
	int sx =0; //������
	int sy = 0;//
	
	RECT Rw;
	HDC hDc;
	switch (msg) {
	case WM_SIZE:
		 hWinBtn = GetWindow(hw,GW_CHILD);
		 sx = LOWORD(lp); //������
		 sy = HIWORD(lp);//������

		SetWindowPos(
			hWinBtn,           // ���������� ����
			HWND_TOP,       // ���������� ������� ����������
			10,              // ������� �� �����������
			sy-40,              // ������� �� ���������
			100,              // ������
			30,              // ������
			0 // ������ ���������������� ����
			);
		hWinBtn = GetWindow(hWinBtn, GW_HWNDNEXT);
		SetWindowPos(
			hWinBtn,           // ���������� ����
			HWND_TOP,       // ���������� ������� ����������
			120,              // ������� �� �����������
			sy - 40,              // ������� �� ���������
			90,              // ������
			30,              // ������
			0 // ������ ���������������� ����
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
		/* �������� ������ */
		
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
