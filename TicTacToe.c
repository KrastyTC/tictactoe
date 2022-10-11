// TicTacToe.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "TicTacToe.h"
#include <windows.h>

#define MAX_LOADSTRING 100
#define ID_HOTKEY 1
#define IDM_MODE_SMALL 1
#define IDM_MODE_BIG 2
#define IDM_RESET_RESET 3
#define SIZE 3
#define EX 1
#define OH 2


// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name
//HWND hwndMain;

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK    PanelProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

void                RegisterRedPanelClass(void);
void                RegisterBluePanelClass(void);
void                CenterWindow(HWND);
void                AddMenus(HWND);
void                DrawBoard(HDC);
RECT                GetRectID(POINT point);
void                DrawFigure(HDC hdc, RECT position);
int                 CheckForWin();
void                ResetGame();
BOOL                IsDraw();

HMENU hMenuModes;
HMENU hMenuReset;
              
RECT squares[3][3] = { 0 };
int field[3][3] = { 0 };
char nextFig = EX;



int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);
    

    // TODO: Place code here.

    /*MessageBoxW(NULL, L"First Program", L"First", MB_OK);

    return 0;*/

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_TICTACTOE, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_TICTACTOE));

    MSG msg;

    

    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            
            SetRect(&squares[i][j], (i * 110), (j * 110), ((i + 1) * 110), ((j + 1) * 110));
           // SetRect(&squares[i][j], (i * 110) + 10, (j * 110) + 10, ((i + 1) * 110) - 10, ((j + 1) * 110)) - 10;
        }
    }

    

    

    // Main message loop:
    while (GetMessage(&msg, NULL, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}




//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TICTACTOE));
    wcex.hCursor        = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_TICTACTOE);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateWindowW(szWindowClass,  szTitle,  WS_OVERLAPPEDWINDOW, 
      CW_USEDEFAULT, 0, 345, 385, NULL, NULL, hInstance, NULL, L"Hot key");

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)

{
    HDC hdc;
    static POINT pt;
    switch (message)
    {
        case WM_CREATE: 
        {
            RegisterHotKey(hWnd, ID_HOTKEY, MOD_CONTROL, 0x43);
            AddMenus(hWnd);
            RegisterRedPanelClass();
            

            /*RegisterRedPanelClass();
           
            CreateWindowW(L"RedPanelClass", NULL,
                WS_CHILD | WS_VISIBLE,
                20, 20, 80, 80,
                hWnd, (HMENU)1, NULL, NULL);

            RegisterBluePanelClass();

            CreateWindowW(L"BluePanelClass", NULL,
                WS_CHILD | WS_VISIBLE,
                120, 20, 80, 80,
                hWnd, (HMENU)2, NULL, NULL);*/


            CenterWindow(hWnd);
            break;
        }
        case WM_HOTKEY:

            if ((wParam) == ID_HOTKEY) {

                CenterWindow(hWnd);
            }

            break;
        case WM_LBUTTONDOWN:
            pt.x = LOWORD(lParam);
            pt.y = HIWORD(lParam);
            int colum, row;
            RECT foundSquare = GetRectID(pt);
            hdc = GetDC(hWnd);
            if (&foundSquare != NULL)
                colum = foundSquare.left / 110;
                row = foundSquare.top / 110;
                if (field[colum][row] == 0)
                {
                    DrawFigure(hdc, foundSquare);
                    int winner = CheckForWin();
                    if (winner == EX || winner == OH )
                    {
                        MessageBox(hWnd, ((winner == EX) ? _T("X wins!") : _T("O wins!")),
                            _T("Game Over"), MB_ICONEXCLAMATION | MB_OK);
                        ResetGame(hWnd);
                    }
                    if (IsDraw()) {
                        MessageBox(hWnd, _T("It's a draw!"), _T("Game Over"), MB_ICONEXCLAMATION | MB_OK);
                        ResetGame(hWnd);
                    }
                }
            break;

    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            case IDM_MODE_SMALL:
                CheckMenuRadioItem(hMenuModes, IDM_MODE_SMALL, IDM_MODE_BIG,
                    IDM_MODE_SMALL, MF_BYCOMMAND);
                MessageBeep(MB_ICONERROR);
                break;

            case IDM_MODE_BIG:
                CheckMenuRadioItem(hMenuModes, IDM_MODE_SMALL, IDM_MODE_BIG,
                    IDM_MODE_BIG, MF_BYCOMMAND);
                MessageBeep(0xFFFFFFFF);
                break;
            case IDM_RESET_RESET:
                ResetGame(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            
            SetBkMode(hdc, TRANSPARENT);
            DrawBoard(hdc);

            /*SelectObject(hdc, hOldPen);*/
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        UnregisterHotKey(hWnd, ID_HOTKEY);
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
    
}

LRESULT CALLBACK PanelProc(HWND hwnd, UINT msg,
    WPARAM wParam, LPARAM lParam) {
    HDC hdc;
    //int Pos;
    static POINT pt;

    switch (msg) {

    case WM_LBUTTONUP:

        MessageBeep(MB_OK);
        break;

    /*case WM_LBUTTONDOWN:
        pt.x = LOWORD(lParam);
        pt.y = HIWORD(lParam);
        RECT foundSquare = GetRectID(pt);
        hdc = GetDC(hwnd);
        if (&foundSquare != NULL && (IsRectEmpty(&foundSquare)))
        
            DrawFigure(hdc, foundSquare);
       
        break;*/
       /* 
        m_nGameGrid[nPos] = EX;
        m_nNextChar = OH;
        hdc = GetDC(hwndMain);
        DrawX(hdc, nPos);
        CheckForGameOver();
        return 0;*/
    }


    return DefWindowProcW(hwnd, msg, wParam, lParam);
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

RECT GetRectID(POINT point)
{
    
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {

            if (PtInRect(&squares[i][j], point))
                return squares[i][j];
        }
       
    }
    
}


void CenterWindow(HWND hWnd) {

    RECT rc = { 0 };

    GetWindowRect(hWnd, &rc);
    int win_w = rc.right - rc.left;
    int win_h = rc.bottom - rc.top;

    int screen_w = GetSystemMetrics(SM_CXSCREEN);
    int screen_h = GetSystemMetrics(SM_CYSCREEN);

    SetWindowPos(hWnd, HWND_TOP, (screen_w - win_w) / 2,
        (screen_h - win_h) / 2, 0, 0, SWP_NOSIZE);
}

void RegisterRedPanelClass(void) {

    HBRUSH hbrush = CreateSolidBrush(RGB(255, 0, 0));

    WNDCLASSW rwc = { 0 };

    rwc.lpszClassName = L"RedPanelClass";
    rwc.hbrBackground = hbrush;
    rwc.lpfnWndProc = PanelProc;
    rwc.hCursor = LoadCursor(0, IDC_ARROW);
    RegisterClassW(&rwc);
}

void RegisterBluePanelClass(void) {

    HBRUSH hbrush = CreateSolidBrush(RGB(0, 0, 255));

    WNDCLASSW rwc = { 0 };

    rwc.lpszClassName = L"BluePanelClass";
    rwc.hbrBackground = hbrush;
    rwc.lpfnWndProc = PanelProc;
    rwc.hCursor = LoadCursor(0, IDC_ARROW);

    RegisterClassW(&rwc);
}

void DrawBoard(HDC hdc) {

    HPEN pen = CreatePen(PS_SOLID, 10, RGB(0, 0, 0));
    void* oldPen = SelectObject(hdc, pen);
    MoveToEx(hdc, 0, 110, NULL);
    LineTo(hdc, 330, 110);

    MoveToEx(hdc, 0, 220, NULL);
    LineTo(hdc, 330, 220);

    MoveToEx(hdc, 110, 0, NULL);
    LineTo(hdc, 110, 330);

    MoveToEx(hdc, 220, 0, NULL);
    LineTo(hdc, 220, 330);

    SelectObject(hdc, (HGDIOBJ)oldPen);
    DeleteObject(pen);
}

void DrawFigure(HDC hdc, RECT position) {
    int colum, row;
    if (nextFig == EX) {
        HPEN pen = CreatePen(PS_SOLID, 10, RGB(255, 0, 0));
        void* oldPen = SelectObject(hdc, pen);
        
        colum = position.left / 110;
        row = position.top / 110;

        position.top += 20;
        position.bottom -= 20;
        position.left += 20;
        position.right -= 20;

        MoveToEx(hdc, position.left, position.top, NULL);
        LineTo(hdc, position.right, position.bottom);
        MoveToEx(hdc, position.left, position.bottom, NULL);
        LineTo(hdc, position.right, position.top);

        SelectObject(hdc, (HGDIOBJ)oldPen);
        DeleteObject(pen);
        nextFig = OH;
        field[colum][row] = EX;

    }
    else {
        HPEN pen = CreatePen(PS_SOLID, 10, RGB(0, 0, 255));
        void* oldPen = SelectObject(hdc, pen);
        void* oldBr = SelectObject(hdc, GetStockObject(NULL_BRUSH));


        colum = position.left / 110;
        row = position.top / 110;

        position.top += 20;
        position.bottom -= 20;
        position.left += 20;
        position.right -= 20;

        Ellipse(hdc, position.left, position.top, position.right, position.bottom);
        SelectObject(hdc, (HGDIOBJ)oldBr);
        SelectObject(hdc, (HGDIOBJ)oldPen);
        DeleteObject(pen);
        nextFig = EX;
        field[colum][row] = OH;

    }
   
    

}

int CheckForWin() {
    int Count = 0;
    int ForWin = 3;
    int FieldValue = 0;
    int nWiner = 0;

    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {
            if (field[i][j] == EX || field[i][j] == OH) {
                FieldValue = field[i][j];
                if(i + ForWin - 1 < SIZE)
                { 
                    int c = i;
                    Count = 0;
                    while (c < i + ForWin) {
                        if (field[c][j] == FieldValue)
                        {
                            Count++;
                        }
                        c++;
                    }
                    if (Count == ForWin)
                        return nWiner = FieldValue;
                }
                if (j + ForWin - 1 < SIZE)
                {
                    int r = j;
                    Count = 0;
                    while (r < j + ForWin) {
                        if (field[i][r] == FieldValue)
                        {
                            Count++;
                        }
                        r++;
                    }
                    if (Count == ForWin)
                        return nWiner = FieldValue;
                }
                if (i + ForWin - 1 < SIZE && j + ForWin - 1 < SIZE)
                {
                    int c = i;
                    int r = j;
                    Count = 0;
                    while (c < i + ForWin) {
                        if (field[c][r] == FieldValue)
                        {
                            Count++;
                        }
                        c++;
                        r++;
                    }
                    if (Count == ForWin)
                        return nWiner = FieldValue;
                }
                if (i - ForWin - 1 <= 0 && j + ForWin - 1 < SIZE)
                {
                    int c = i;
                    int r = j;
                    Count = 0;
                    while (c > i - ForWin) {
                        if (field[c][r] == FieldValue)
                        {
                            Count++;
                        }
                        c--;
                        r++;
                    }
                    if (Count == ForWin)
                        return nWiner = FieldValue;
                }
                    

            }
        }
        
    }
    return 0;

}


void AddMenus(HWND hwnd) {

    HMENU hMenubar;

    hMenubar = CreateMenu();
    hMenuModes = CreateMenu();
    hMenuReset = CreateMenu();

    AppendMenuW(hMenuModes, MF_STRING, IDM_MODE_SMALL, L"&3x3");
    AppendMenuW(hMenuModes, MF_STRING, IDM_MODE_BIG, L"&Big");

    AppendMenuW(hMenuReset, MF_STRING, IDM_RESET_RESET, L"&Reset");

    CheckMenuRadioItem(hMenuModes, IDM_MODE_SMALL, IDM_MODE_BIG,
        IDM_MODE_SMALL, MF_BYCOMMAND);

    AppendMenuW(hMenubar, MF_POPUP, (UINT_PTR)hMenuModes, L"&Mode");

    AppendMenuW(hMenubar, MF_POPUP, (UINT_PTR)hMenuReset, L"&Reset");
    SetMenu(hwnd, hMenubar);
}

void ResetGame(HWND hwnd)
{
    RECT rect;

    nextFig = EX;
    ZeroMemory(field, 9 * sizeof(int));
    GetClientRect(hwnd, &rect);
    InvalidateRect(hwnd, &rect, TRUE);
}

BOOL IsDraw() {
    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {
            if (field[i][j] == 0) {
                return 0;
            }
        }
        
    }
    return 1;
}
