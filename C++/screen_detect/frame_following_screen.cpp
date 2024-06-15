// Detect application and move fram
// g++ -o red_frame_window helloworld.cpp -lgdi32 -luser32

#include <windows.h>
#include <tchar.h>

// Global constants
const int BORDER_WIDTH = 10;
const TCHAR TARGET_WINDOW_TITLE[] = _T("*treefolder.txt - Notepad");

// Global variables
RECT initialTargetRect;

// Forward declarations of functions included in this code module
HWND FindTargetWindow();
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

// Function to find the target window
HWND FindTargetWindow() {
    return FindWindow(NULL, TARGET_WINDOW_TITLE);
}

// Main entry point
int APIENTRY WinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPSTR lpCmdLine,
                     _In_ int nCmdShow) {
    // Register the window class
    const char CLASS_NAME[] = "RedFrameWindowClass";

    WNDCLASS wc = { };

    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hbrBackground = (HBRUSH)GetStockObject(NULL_BRUSH);

    RegisterClass(&wc);

    // Find the target window
    HWND targetHwnd = FindTargetWindow();
    if (targetHwnd == NULL) {
        MessageBox(NULL, _T("Target window not found!"), _T("Error"), MB_ICONERROR);
        return 0;
    }

    // Get the target window's dimensions initially
    GetWindowRect(targetHwnd, &initialTargetRect);

    // Create a borderless, topmost window
    HWND hwnd = CreateWindowEx(
        WS_EX_TOPMOST | WS_EX_LAYERED,
        CLASS_NAME,
        "Red Frame Window",
        WS_POPUP,
        initialTargetRect.left, initialTargetRect.top,
        initialTargetRect.right - initialTargetRect.left,
        initialTargetRect.bottom - initialTargetRect.top,
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

        // Check if target window moved or resized
        RECT currentTargetRect;
        GetWindowRect(targetHwnd, &currentTargetRect);

        if (!EqualRect(&currentTargetRect, &initialTargetRect)) {
            // Update overlay window position and size
            SetWindowPos(hwnd, nullptr,
                         currentTargetRect.left, currentTargetRect.top,
                         currentTargetRect.right - currentTargetRect.left,
                         currentTargetRect.bottom - currentTargetRect.top,
                         SWP_NOZORDER | SWP_NOACTIVATE);

            // Update initial position and size
            initialTargetRect = currentTargetRect;
        }
    }

    return 0;
}

// Process messages for the main window
LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);

        // Get the window dimensions
        RECT rect;
        GetClientRect(hwnd, &rect);
        int width = rect.right;
        int height = rect.bottom;

        // Create a red brush
        HBRUSH hBrush = CreateSolidBrush(RGB(255, 0, 0));

        // Draw the top border
        RECT topRect = {0, 0, width, BORDER_WIDTH};
        FillRect(hdc, &topRect, hBrush);

        // Draw the bottom border
        RECT bottomRect = {0, height - BORDER_WIDTH, width, height};
        FillRect(hdc, &bottomRect, hBrush);

        // Draw the left border
        RECT leftRect = {0, 0, BORDER_WIDTH, height};
        FillRect(hdc, &leftRect, hBrush);

        // Draw the right border
        RECT rightRect = {width - BORDER_WIDTH, 0, width, height};
        FillRect(hdc, &rightRect, hBrush);

        // Clean up
        DeleteObject(hBrush);

        EndPaint(hwnd, &ps);
    }
    return 0;

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;

    case WM_MOVE:
    case WM_SIZE:
        // Reposition or resize the overlay window
        SetWindowPos(hwnd, nullptr,
                     initialTargetRect.left, initialTargetRect.top,
                     initialTargetRect.right - initialTargetRect.left,
                     initialTargetRect.bottom - initialTargetRect.top,
                     SWP_NOZORDER | SWP_NOACTIVATE);
        return 0;

    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }

    return 0;
}
