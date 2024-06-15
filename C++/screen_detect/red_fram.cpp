// g++ -o red_frame_window red_fram.cpp -lgdi32 -luser32

#include <windows.h>

// Global constants
const int BORDER_WIDTH = 10;

// Forward declarations of functions included in this code module
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

// Main entry point
int APIENTRY WinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPSTR lpCmdLine,
                     _In_ int nCmdShow) {
    // Register the window class
    const char CLASS_NAME[] = "Red Frame Window Class";

    WNDCLASS wc = { };

    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hbrBackground = (HBRUSH)GetStockObject(NULL_BRUSH);

    RegisterClass(&wc);

    // Get the screen dimensions
    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);

    // Create a borderless, topmost window
    HWND hwnd = CreateWindowEx(
        WS_EX_TOPMOST | WS_EX_LAYERED,
        CLASS_NAME,
        "Red Frame Window",
        WS_POPUP,
        0, 0, screenWidth, screenHeight,
        nullptr,
        nullptr,
        hInstance,
        nullptr
    );

    if (hwnd == nullptr) {
        return 0;
    }

    // Make the window transparent
    SetLayeredWindowAttributes(hwnd, RGB(0, 0, 0), 0, LWA_COLORKEY);

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    // Run the message loop
    MSG msg = { };
    while (GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

// Process messages for the main window
LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);

        // Get the screen dimensions
        int screenWidth = GetSystemMetrics(SM_CXSCREEN);
        int screenHeight = GetSystemMetrics(SM_CYSCREEN);

        // Create a red brush
        HBRUSH hBrush = CreateSolidBrush(RGB(255, 0, 0));

        // Draw the top border
        RECT topRect = {0, 0, screenWidth, BORDER_WIDTH};
        FillRect(hdc, &topRect, hBrush);

        // Draw the bottom border
        RECT bottomRect = {0, screenHeight - BORDER_WIDTH, screenWidth, screenHeight};
        FillRect(hdc, &bottomRect, hBrush);

        // Draw the left border
        RECT leftRect = {0, 0, BORDER_WIDTH, screenHeight};
        FillRect(hdc, &leftRect, hBrush);

        // Draw the right border
        RECT rightRect = {screenWidth - BORDER_WIDTH, 0, screenWidth, screenHeight};
        FillRect(hdc, &rightRect, hBrush);

        // Clean up
        DeleteObject(hBrush);

        EndPaint(hwnd, &ps);
    }
    return 0;

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;

    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }

    return 0;
}
