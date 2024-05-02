#include "BromineMath.h"
#include <windows.h>

#define ld long double

// Forward declaration of window procedure
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

// Function to create the main window
HWND CreateMainWindow(HINSTANCE hInstance, int nCmdShow) {
    WNDCLASS wc = {};
    wc.lpfnWndProc = WindowProc; // Set the window procedure
    wc.hInstance = hInstance; // Instance handle
    wc.lpszClassName = "MyWindowClass"; // Class name

    RegisterClass(&wc);

    HWND hwnd = CreateWindowEx(
        0,
        "MyWindowClass",
        "BromineSCE", // Window title
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        600, 400, // Window size
        NULL, NULL,
        hInstance,
        NULL
    );

    if (hwnd == NULL) { // Check if window creation was successful
        return NULL;
    }

    ShowWindow(hwnd, nCmdShow); // Show the window

    return hwnd; // Return the window handle
}

// Window procedure to handle messages
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    static HWND hComboBox; // Handle to the combo box
    static HWND hInputX1, hInputY1, hInputZ1; // Text inputs for 3D vector (point 1)
    static HWND hInputX2, hInputY2, hInputZ2; // Text inputs for 3D vector (point 2)
    static HWND hButtonCalculate; // Button to initiate calculation

    switch (uMsg) {
    case WM_CREATE: {
        // Create a combo box (dropdown) with operations
        hComboBox = CreateWindow(
            "COMBOBOX",
            "",
            CBS_DROPDOWN | WS_CHILD | WS_VISIBLE,
            10, 10,
            200, 200,
            hwnd,
            NULL,
            GetModuleHandle(NULL),
            NULL
        );

        SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)"Vector2D");
        SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)"Vector3D");
        SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)"Object2D");
        SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)"Object3D");
        
        // Create text inputs for the first 3D vector point
        hInputX1 = CreateWindow(
            "X1", "",
            WS_BORDER | WS_CHILD | WS_VISIBLE,
            10, 40,
            100, 20,
            hwnd,
            NULL,
            GetModuleHandle(NULL),
            NULL
        );

        hInputY1 = CreateWindow(
            "Y1", "",
            WS_BORDER | WS_CHILD | WS_VISIBLE,
            120, 40,
            100, 20,
            hwnd,
            NULL,
            GetModuleHandle(NULL),
            NULL
        );

        hInputZ1 = CreateWindow(
            "Z1", "",
            WS_BORDER | WS_CHILD | WS_VISIBLE,
            230, 40,
            100, 20,
            hwnd,
            NULL,
            GetModuleHandle(NULL),
            NULL
        );

        // Create text inputs for the second 3D vector point
        hInputX2 = CreateWindow(
            "X2", "",
            WS_BORDER | WS_CHILD | WS_VISIBLE,
            10, 70,
            100, 20,
            hwnd,
            NULL,
            GetModuleHandle(NULL),
            NULL
        );

        hInputY2 = CreateWindow(
            "Y2", "",
            WS_BORDER | WS_CHILD | WS_VISIBLE,
            120, 70,
            100, 20,
            hwnd,
            NULL,
            GetModuleHandle(NULL),
            NULL
        );

        hInputZ2 = CreateWindow(
            "Z2", "",
            WS_BORDER | WS_CHILD | WS_VISIBLE,
            230, 70,
            100, 20,
            hwnd,
            NULL,
            GetModuleHandle(NULL),
            NULL
        );

        // Create a button to initiate the calculation
        hButtonCalculate = CreateWindow(
            "BUTTON",
            "Calculate",
            WS_CHILD | WS_VISIBLE,
            10, 100,
            100, 30,
            hwnd,
            NULL,
            GetModuleHandle(NULL),
            NULL
        );

        break;
    }

    case WM_COMMAND: {
        if (lParam == (LPARAM)hButtonCalculate) { // When the calculate button is pressed
            int selected = SendMessage(hComboBox, CB_GETCURSEL, 0, 0); // Get the selected operation

            if (selected == 0) { // 2D Distance Calculation
                double x1 = GetDlgItemInt(hwnd, (int)hInputX1, NULL, FALSE);
                double y1 = GetDlgItemInt(hwnd, (int)hInputY1, NULL, FALSE);
                double x2 = GetDlgItemInt(hwnd, (int)hInputX2, NULL, FALSE);
                double y2 = GetDlgItemInt(hwnd, (int)hInputY2, NULL, FALSE);
                
                Vector2D vec2;
                vec2.p1 = {x1, y1};
                vec2.p2 = {x2, y2};

                std::cout << "2D Distance: " << vec2.distance2D() << std::endl; // Display the result
            } else if (selected == 1) { // 3D Distance Calculation
                double x1 = GetDlgItemInt(hwnd, (int)hInputX1, NULL, FALSE);
                double y1 = GetDlgItemInt(hwnd, (int)hInputY1, NULL, FALSE);
                double z1 = GetDlgItemInt(hwnd, (int)hInputZ1, NULL, FALSE);
                double x2 = GetDlgItemInt(hwnd, (int)hInputX2, NULL, FALSE);
                double y2 = GetDlgItemInt(hwnd, (int)hInputY2, NULL, FALSE);
                double z2 = GetDlgItemInt(hwnd, (int)hInputZ2, NULL, FALSE);

                Vector3D vec3;
                vec3.p1 = {x1, y1, z1};
                vec3.p2 = {x2, y2, z2};

                std::cout << "3D Distance: " << vec3.distance3D() << std::endl; // Display the result
            }
        }
        break;
    }

    case WM_DESTROY:
        PostQuitMessage(0); // Signal the application to exit
        return 0;

    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam); // Default message handling
    }
}

// Main function for the Windows application
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    HWND hwnd = CreateMainWindow(hInstance, nCmdShow); // Call the function to create the main window

    if (hwnd == NULL) { // If window creation fails
        return 0;
    }

    MSG msg = {}; // Structure to store messages
    while (GetMessage(&msg, NULL, 0, 0)) { // Main message loop
        TranslateMessage(&msg); // Translate keyboard input
        DispatchMessage(&msg); // Dispatch messages to the window procedure
    }

    return msg.wParam; // Return the exit code
}
