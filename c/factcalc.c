#include <windows.h>
#include <stdio.h>

// Controls
#define IDC_INPUT 101
#define IDC_CALCULATE 102

// Function to calculate factorial
unsigned long long factorial(int n) {
    if (n < 0) return 0; // invalid
    if (n == 0 || n == 1) return 1;
    return n * factorial(n - 1);
}

// Window procedure
LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    static HWND hInput, hButton;

    switch (msg) {
        case WM_CREATE: {
            // Label
            CreateWindow("STATIC", "Enter number:", WS_VISIBLE | WS_CHILD,
                         20, 20, 100, 20, hwnd, NULL, NULL, NULL);

            // Input field
            hInput = CreateWindow("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER,
                                  130, 20, 100, 20, hwnd, (HMENU)IDC_INPUT, NULL, NULL);

            // Button
            hButton = CreateWindow("BUTTON", "Calculate", WS_VISIBLE | WS_CHILD,
                                   90, 60, 80, 30, hwnd, (HMENU)IDC_CALCULATE, NULL, NULL);
            return 0;
        }

        case WM_COMMAND: {
            if (LOWORD(wParam) == IDC_CALCULATE) {
                char buffer[50];
                GetWindowText(hInput, buffer, sizeof(buffer));
                int n = atoi(buffer);

                if (n < 0 || n > 20) { // limit to prevent overflow
                    MessageBox(hwnd, "Enter a number between 0 and 20", "Error", MB_OK | MB_ICONERROR);
                    return 0;
                }

                unsigned long long result = factorial(n);
                char msg[100];
                sprintf(msg, "Factorial of %d is: %llu", n, result);
                MessageBox(hwnd, msg, "Result", MB_OK | MB_ICONINFORMATION);
            }
            return 0;
        }

        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
    }
    return DefWindowProc(hwnd, msg, wParam, lParam);
}

// Entry point
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    const char CLASS_NAME[] = "FactorialApp";

    WNDCLASS wc = {0};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);

    RegisterClass(&wc);

    HWND hwnd = CreateWindow(CLASS_NAME, "Factorial Calculator",
                             WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME ^ WS_MAXIMIZEBOX, // fixed window
                             CW_USEDEFAULT, CW_USEDEFAULT, 300, 150,
                             NULL, NULL, hInstance, NULL);

    if (!hwnd) return 0;

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    MSG msg = {0};
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return 0;
}
