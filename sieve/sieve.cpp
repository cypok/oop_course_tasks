// sieve.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "sieve.h"
#include "algorithm.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
TCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);

const unsigned REDRAW_PERIOD = 75;

const unsigned TOTAL_PASSES = 1000; // = sqrt(ARR_SIZE)

const unsigned POINTS_NUM = 250;

const unsigned PLOT_WIDTH = 1000;
const unsigned PLOT_HEIGHT = 300;

const unsigned WIDTH = PLOT_WIDTH + 4;
const unsigned HEIGHT = PLOT_HEIGHT*2 + 8;

const unsigned PRIMES_MAX = Algorithm::ARR_SIZE/POINTS_NUM/2;
const unsigned X_STEP = PLOT_WIDTH/POINTS_NUM;

const UINT_PTR timer = 1;


void PlotBorder(HDC hdc, int left, int top)
{
    static HPEN hpen_border = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));

    SelectObject(hdc, hpen_border);
    Rectangle(hdc,
        left + 1,
        top + 1,
        left + PLOT_WIDTH + 2,
        top + PLOT_HEIGHT + 2
    );
}

void PlotEvolution(HDC hdc, int left, int top)
{
    static HPEN hpen_plot = CreatePen(PS_SOLID, 2, RGB(0, 0, 255));
    unsigned passes = Algorithm::get_passes();
    static POINT last[TOTAL_PASSES] = {{
        left + 2 + passes,
        top + 1 + PLOT_HEIGHT - PLOT_HEIGHT*Algorithm::count_primes(0, Algorithm::ARR_SIZE)/Algorithm::ARR_SIZE
    }};

    static unsigned draw_times = 1;

    PlotBorder(hdc, left, top);
    TextOut(hdc, left+10, top+10, _T("Evolution"), 9);

    Algorithm::enter_cs();
    {
        last[draw_times].x = left + 2 + passes*PLOT_WIDTH/TOTAL_PASSES;
        last[draw_times].y = top + 1 + PLOT_HEIGHT - PLOT_HEIGHT*Algorithm::count_primes(0, Algorithm::ARR_SIZE)/Algorithm::ARR_SIZE;
        ++draw_times;
    }
    Algorithm::leave_cs();

    SelectObject(hdc, hpen_plot);
    Polyline(hdc, last, draw_times);
}

void PlotDensity(HDC hdc, int left, int top)
{
    static unsigned range = Algorithm::ARR_SIZE/POINTS_NUM;
    static HPEN hpen_plot = CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
    static HPEN hpen_cursor = CreatePen(PS_SOLID, 2, RGB(50, 50, 50));
    static POINT plot[POINTS_NUM];

    PlotBorder(hdc, left, top);
    TextOut(hdc, left+10, top+10, _T("Density"), 7);

    Algorithm::enter_cs();
    {
        for(unsigned i = 0; i < POINTS_NUM; ++i)
        {
            plot[i].x = left + 2 + i*X_STEP;
            plot[i].y = top + 1 + PLOT_HEIGHT - min(PLOT_HEIGHT, PLOT_HEIGHT*Algorithm::count_primes(i*range, (i+1)*range)/PRIMES_MAX);
        }
    }
    Algorithm::leave_cs();

    SelectObject(hdc, hpen_plot);
    Polyline(hdc, plot, POINTS_NUM);
    SelectObject(hdc, hpen_cursor);
    unsigned passes = Algorithm::get_passes();
    MoveToEx(hdc, left+2+passes*passes*PLOT_WIDTH/Algorithm::ARR_SIZE, top + 1, NULL);
    LineTo(hdc, left+2+passes*passes*PLOT_WIDTH/Algorithm::ARR_SIZE, top + 1 + PLOT_HEIGHT);
}

void ErrorExit(HWND hWnd)
{
    MessageBox(hWnd, _T("Unexpected internal error"), _T("Error"), MB_OK | MB_ICONWARNING);
    DestroyWindow(hWnd);
}

int APIENTRY _tWinMain(HINSTANCE hInstance,
                       HINSTANCE hPrevInstance,
                       LPTSTR    lpCmdLine,
                       int       nCmdShow)
{


    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    MSG msg;
    HACCEL hAccelTable;

    LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadString(hInstance, IDC_SIEVE, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_SIEVE));
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

ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEX wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SIEVE));
    wcex.hCursor        = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = 0;//MAKEINTRESOURCE(IDC_SIEVE);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassEx(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    HWND hWnd;

    hInst = hInstance; // Store instance handle in our global variable

    Algorithm::init();
    RECT rect = { 50, 50, WIDTH+50, HEIGHT+50 };
    AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME, FALSE);
    hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME,
        rect.left, rect.top, rect.right-rect.left, rect.bottom-rect.top, NULL, NULL, hInstance, NULL);

    if (!hWnd)
        return FALSE;

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    int wmId, wmEvent;
    PAINTSTRUCT ps;
    HDC hdc;

    switch (message)
    {
    case WM_COMMAND:
        wmId    = LOWORD(wParam);
        wmEvent = HIWORD(wParam);
        // Parse the menu selections:
        switch (wmId)
        {
        case IDM_EXIT:
            DestroyWindow(hWnd);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
        break;
    case WM_LBUTTONDOWN:
        {
            static bool started = false;
            if (!started)
            {
                if (!Algorithm::start() || !SetTimer(hWnd, timer, REDRAW_PERIOD, NULL))
                    ErrorExit(hWnd);
                started = true;
            }
            break;
        }
    case WM_PAINT:
        {
            hdc = BeginPaint(hWnd, &ps);

            PlotEvolution(hdc, 0, 0);
            PlotDensity(hdc, 0, PLOT_HEIGHT+2);

            EndPaint(hWnd, &ps);
            break;
        }
    case WM_TIMER:
        if (static_cast<UINT_PTR>(wParam) != timer)
            break;

        if (Algorithm::is_running())
        {
            InvalidateRect(hWnd, NULL, TRUE);
        }
        else
        {
            if (KillTimer(hWnd, timer) == 0)
            {
                ErrorExit(hWnd);
                break;
            }
            MessageBox(hWnd, _T("Calculation finished"), _T("Calculation finished"), MB_OK | MB_ICONINFORMATION);
            DestroyWindow(hWnd);
        }
        break;
    case WM_DESTROY:
        Algorithm::stop();
        Algorithm::deinit();
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}
