#include <windows.h>
#include <iostream>

void ClickAt(int x, int y) {
    // Move the cursor to the specified screen coordinates
    SetCursorPos(x, y);

    // Simulate the mouse down and up events for the left button click
    mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
    mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
}

int main() {
    // Example coordinates for the mouse click
    int x = 130;
    int y = 169;

    std::cout << "Clicking at (" << x << "," << y << ")\n";
    
    // Perform the click at the specified coordinates
    ClickAt(x, y);

    return 0;
}
