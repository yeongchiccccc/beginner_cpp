// TETRIS.cpp : 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"


LPCTSTR			lpszClass = TEXT("Tetris3");
tag_Status GameStatus;
Point			Shape[][4][4]=
{
	{{0,0,1,0,2,0,-1,0},{0,0,0,1,0,-1,0,-2},{0,0,1,0,2,0,-1,0},{0,0,0,1,0,-1,0,-2}},
	{{0,0,1,0,0,1,1,1,},{0,0,1,0,0,1,1,1},{0,0,1,0,0,1,1,1},{0,0,1,0,0,1,1,1}},
	{{0,0,-1,0,0,-1,1,-1},{0,0,0,1,-1,0,-1,-1},{0,0,-1,0,0,-1,1,-1},{0,0,0,1,-1,0,-1,-1}},
	{{0,0,-1,-1,0,-1,1,0},{0,0,-1,0,-1,1,0,-1},{0,0,-1,-1,0,-1,1,0},{0,0,-1,0,-1,1,0,-1}},
	{{0,0,-1,0,1,0,-1,-1},{0,0,0,-1,0,1,-1,1},{0,0,-1,0,1,0,1,1},{0,0,0,-1,0,1,1,-1}},
	{{0,0,1,0,-1,0,1,-1},{0,0,0,1,0,-1,-1,-1},{0,0,1,0,-1,0,-1,1},{0,0,0,-1,0,1,1,1}},
	{{0,0,-1,0,1,0,0,1},{0,0,0,-1,0,1,1,0},{0,0,-1,0,1,0,0,-1},{0,0,-1,0,0,-1,0,1}},
	{{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0}},
	{{0,0,0,0,0,-1,1,0},{0,0,0,0,-1,0,0,-1},{0,0,0,0,0,1,-1,0},{0,0,0,0,0,1,1,0}},
};


int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	
	MSG			iMessage;
	HACCEL		hAccelTable;


	g_hInst		= hInstance;


	DialogBox(g_hInst, MAKEINTRESOURCE(IDD_DIALOG_MAIN), NULL, DlgProc_Waiting);

	//Window Create
	MyRegisterClass(g_hInst);

	//Window Load
	if (!InitInstance (g_hInst, nCmdShow))
	{
		return FALSE;
	}

	//Set Accelerators
	hAccelTable = LoadAccelerators(g_hInst, MAKEINTRESOURCE(IDC_TETRIS));


	DialogBox(g_hInst, MAKEINTRESOURCE(IDD_DIALOG1), NULL, DlgProc);

	
	while (GetMessage(&iMessage, NULL, 0, 0))
	{
		if (!TranslateAccelerator(iMessage.hwnd, hAccelTable, &iMessage))
		{
			TranslateMessage(&iMessage);
			DispatchMessage(&iMessage);
		}
	}

	//Send buffer about decreasing the number of people in game room
	ZeroMemory(buf, BUFSIZE);
	buf[0] = '$';
	buf[1] = '$';
	buf[2] = '\0';
	send(sock, buf, sizeof(char) * 3, NULL);

	//Close Socket
	closesocket(sock);
	WSACleanup();
	

	return (int) iMessage.wParam;
}



//
//  함수: MyRegisterClass()
//
//  목적: 창 클래스를 등록합니다.
//
//  설명:
//
//    Windows 95에서 추가된 'RegisterClassEx' 함수보다 먼저
//    해당 코드가 Win32 시스템과 호환되도록
//    하려는 경우에만 이 함수를 사용합니다. 이 함수를 호출해야
//    해당 응용 프로그램에 연결된
//    '올바른 형식의' 작은 아이콘을 가져올 수 있습니다.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= 0;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TETRIS));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_TETRIS);
	wcex.lpszClassName	= lpszClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   함수: InitInstance(HINSTANCE, int)
//
//   목적: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   설명:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   g_hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

   hWnd = CreateWindow(lpszClass, lpszClass, WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
	   CW_USEDEFAULT, CW_USEDEFAULT, 0, 0, NULL, (HMENU)NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  목적: 주 창의 메시지를 처리합니다.
//
//  WM_COMMAND	- 응용 프로그램 메뉴를 처리합니다.
//  WM_PAINT	- 주 창을 그립니다.
//  WM_DESTROY	- 종료 메시지를 게시하고 반환합니다.
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int i		= 0;
	int trot	= 0;
	int x		= 0;
	int y		= 0;
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;
	RECT crt;
	
	

	switch (message)
	{
	case WM_CREATE:
		hWndMain	= hWnd;
		SetRect(&crt, 0, 0, (BW+12)*TS, (BH+2)*TS);
		AdjustWindowRect(&crt, WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX, TRUE);
		SetWindowPos(hWndMain, NULL, 0, 0, crt.right - crt.left, crt.bottom - crt.top, SWP_NOMOVE | SWP_NOZORDER);

		GameStatus	= GAMEOVER;
		srand(GetTickCount());
		for (i = 0; i<11; i++)
		{
			hBit[i]	= LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP1+i));
		}
		return 0;
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// 메뉴의 선택 영역을 구문 분석합니다.
		switch (wmId)
		{
		case IDM_GAME_START:
			if (GameStatus != GAMEOVER)
			{
				break;
			}
			for (x = 0; x<BW + 2; x++)
			{
				for (y = 0; y<BH + 2; y++)
				{
					board[x][y] = (y==0 || y==BH+1 || x==0 || x==BW+1)?WALL:EMPTY;
				}
			}
			score		= 0;
			bricknum	= 0;
			GameStatus	= RUNNING;
			nbrick		= random(sizeof(Shape)/sizeof(Shape[0]));
			MakeNewBrick();
			iInterval	= 1000;
			SetTimer(hWnd, 1, iInterval, NULL);
			break;
		case IDM_GAME_PAUSE:
			if (GameStatus == RUNNING)
			{
				GameStatus	= PAUSE;
				KillTimer(hWnd, 1);
			}
			else if (GameStatus == PAUSE)
			{
				GameStatus	= RUNNING;
				SetTimer(hWnd, 1, iInterval, NULL);
			}
			break;
		case IDM_GAME_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		return 0;
	case WM_TIMER:
		if (MoveDown() == TRUE)
		{
			MakeNewBrick();
		}
		return 0;
	case WM_KEYDOWN:
		if (GameStatus != RUNNING || brick == -1)
		{
			return 0;
		}
		switch (wParam)
		{
		case VK_LEFT:
			if (GetAround(nx - 1, ny, brick, rot) == EMPTY)
			{
				nx--;
				InvalidateRect(hWnd, NULL, FALSE);
			}
			break;
		case VK_RIGHT:
			if (GetAround(nx + 1, ny, brick, rot) == EMPTY)
			{
				nx++;
				InvalidateRect(hWnd, NULL, FALSE);
			}
			break;
		case VK_UP:
			trot	= (rot == 3? 0 : rot + 1);
			if (GetAround(nx, ny, brick, trot) == EMPTY)
			{
				rot	= trot;
				InvalidateRect(hWnd, NULL, FALSE);
			}
			break;
		case VK_DOWN:
			if (MoveDown() == TRUE)
			{
				MakeNewBrick();
			}
			break;
		case VK_SPACE:
			while(MoveDown() == FALSE){;}
			MakeNewBrick();
			break;
		}
		return 0;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);		
		DrawScreen(hdc);
		EndPaint(hWnd, &ps);
		return 0;
	case WM_DESTROY:
		KillTimer(hWndMain, 1);
		for (i = 0; i<11; i++)
		{
			DeleteObject(hBit[i]);
		}
		
		PostQuitMessage(0);
		return 0;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}


VOID DrawScreen(HDC hdc)
{
	int x		= 0;
	int y		= 0;
	int i		= 0;
	TCHAR str[128] = {0,};

	//Draw the WALL
	for (x = 0; x<BW+1; x++)
	{
		PrintTile(hdc, x, 0, WALL);
		PrintTile(hdc, x, BH+1, WALL);
	}
	for (y = 0; y<BH+2; y++)
	{
		PrintTile(hdc, 0, y, WALL);
		PrintTile(hdc, BW+1, y, WALL);
	}

	for (x = 1; x<BW+1; x++)
	{
		for (y = 1; y<BH+1; y++)
		{
			PrintTile(hdc, x, y, board[x][y]);
		}
	}

	//draw the moving brick
	if (GameStatus != GAMEOVER && brick != -1)
	{
		for (i = 0; i<4; i++)
		{
			PrintTile(hdc, nx+Shape[brick][rot][i].x, ny+Shape[brick][rot][i].y, brick+1);
		}
	}
	
	//draw the next brick
	for (x=BW+3; x<=BW+11; x++)
	{
		for (y=BH-5; y<=BH+1; y++)
		{
			if (x==BW+3 || x==BW+11 || y==BH-5 || y==BH+1)
			{
				PrintTile(hdc, x, y, WALL);
			}
			else
			{
				PrintTile(hdc, x, y, 0);
			}
		}
	}
	if (GameStatus != GAMEOVER)
	{
		for (i = 0; i<4; i++)
		{
			PrintTile(hdc, BW+7+Shape[nbrick][0][i].x, BH-2+Shape[nbrick][0][i].y, nbrick+1);
		}
	}

	lstrcpy(str, TEXT("Tetris Ver 1.2"));
	TextOut(hdc, (BW+4)*TS, 30, str, lstrlen(str));
	wsprintf(str, TEXT("Score : %d   "), score);
	TextOut(hdc, (BW+4)*TS, 60, str, lstrlen(str));
	wsprintf(str, TEXT("Bricks: %d   "), bricknum);
	TextOut(hdc, (BW+4)*TS, 80, str, lstrlen(str));
}

VOID MakeNewBrick()
{
	bricknum++;
	brick		= nbrick;
	nbrick		= random(sizeof(Shape)/sizeof(Shape[0]));
	nx			= BW/2;
	ny			= 3;
	rot			= 0;
	InvalidateRect(hWndMain, NULL, FALSE);
	if (GetAround(nx, ny, brick, rot) != EMPTY)
	{
		KillTimer(hWndMain, 1);
		GameStatus		= GAMEOVER;
		MessageBox(hWndMain, TEXT("GameOver... Do you want to play, again?"), TEXT("NOTICE"), MB_OK);
	}
}


int GetAround(int x, int y, int b, int r)
{
	int i, k = EMPTY;
	
	for (i = 0; i<4; i++)
	{
		k	= max(k, board[x+Shape[b][r][i].x][y+Shape[b][r][i].y]);
	}
	return k;
}

BOOL MoveDown()
{
	if (GetAround(nx, ny+1, brick, rot) != EMPTY)
	{
		TestFull();
		return TRUE;
	}
	ny++;

	InvalidateRect(hWndMain, NULL, FALSE);
	UpdateWindow(hWndMain);
	return FALSE;
}

VOID TestFull()
{
	int i		= 0;
	int x		= 0;
	int y		= 0;
	int ty		= 0;
	int count	= 0;
	static int arScoreInc[]	= {0, 1, 3, 8, 20};

	for (i = 0; i<4; i++)
	{
		board[nx+Shape[brick][rot][i].x][ny+Shape[brick][rot][i].y]		= brick + 1;
	}

	brick = -1;

	for (y = 1; y<BH+1; y++)
	{
		for (x = 1; x<BW+1; x++)
		{
			if (board[x][y] == EMPTY)
			{
				break;
			}
		}

		if (x == BW+1)
		{
			count++;
			for (ty = y; ty > 1; ty--)
			{
				for (x = 1; x<BW+1; x++)
				{
					board[x][ty]	= board[x][ty-1];
				}
			}
			InvalidateRect(hWndMain, NULL, FALSE);
			UpdateWindow(hWndMain);
			Sleep(150);
		}
	}
	score += arScoreInc[count];
	if (bricknum % 10 == 0 && iInterval > 200)
	{
		iInterval -= 50;
		SetTimer (hWndMain, 1, iInterval, NULL);
	}
}

VOID DrawBitmap(HDC hdc, int x, int y, HBITMAP hBit)
{
	HDC			MemDC;
	HBITMAP		OldBitmap;
	int			bx		= 0;
	int			by		= 0;
	BITMAP		bit;

	MemDC		= CreateCompatibleDC(hdc);
	OldBitmap	= (HBITMAP)SelectObject(MemDC, hBit);

	GetObject(hBit, sizeof(BITMAP), &bit);
	bx			= bit.bmWidth;
	by			= bit.bmHeight;

	BitBlt(hdc, x, y, bx, by, MemDC, 0, 0, SRCCOPY);

	SelectObject(MemDC, OldBitmap);
	DeleteDC(MemDC);
}


VOID PrintTile(HDC hdc, int x, int y, int c)
{
	DrawBitmap(hdc, x*TS, y*TS, hBit[c]);
	return;
}

