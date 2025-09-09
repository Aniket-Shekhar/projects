#include <windows.h>
#include <stdio.h>
#include <string.h>

#define MAX_STUDENTS 100
#define IDC_ADD_BUTTON 101
#define IDC_DISPLAY_BUTTON 102
#define IDC_ROLL_EDIT 103
#define IDC_NAME_EDIT 104
#define IDC_PHONE_EDIT 105
#define IDC_ALLOCATE_BUTTON 106
#define IDC_ROOM_EDIT 107

// Structure to store student details
struct Student {
    int rollNo;
    char name[50];
    char phone[15];
    int roomNo;
    int isAllocated;
};

// Global array to store students
struct Student students[MAX_STUDENTS];
int studentCount = 0;

// Function to check for duplicate roll number
int isDuplicateRollNo(int rollNo) {
    for (int i = 0; i < studentCount; i++) {
        if (students[i].rollNo == rollNo) {
            return 1;
        }
    }
    return 0;
}

// Window procedure function
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    static HWND hRollEdit, hNameEdit, hPhoneEdit, hRoomEdit;
    
    switch (uMsg) {
        case WM_CREATE: {
            // Create labels
            CreateWindow("STATIC", "Roll No:", WS_VISIBLE | WS_CHILD, 20, 20, 80, 20, hwnd, NULL, NULL, NULL);
            CreateWindow("STATIC", "Name:", WS_VISIBLE | WS_CHILD, 20, 50, 80, 20, hwnd, NULL, NULL, NULL);
            CreateWindow("STATIC", "Phone:", WS_VISIBLE | WS_CHILD, 20, 80, 80, 20, hwnd, NULL, NULL, NULL);
            CreateWindow("STATIC", "Room No:", WS_VISIBLE | WS_CHILD, 20, 110, 80, 20, hwnd, NULL, NULL, NULL);

            // Create input fields
            hRollEdit = CreateWindow("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER, 100, 20, 150, 20, hwnd, (HMENU)IDC_ROLL_EDIT, NULL, NULL);
            hNameEdit = CreateWindow("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER, 100, 50, 150, 20, hwnd, (HMENU)IDC_NAME_EDIT, NULL, NULL);
            hPhoneEdit = CreateWindow("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER, 100, 80, 150, 20, hwnd, (HMENU)IDC_PHONE_EDIT, NULL, NULL);
            hRoomEdit = CreateWindow("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER, 100, 110, 150, 20, hwnd, (HMENU)IDC_ROOM_EDIT, NULL, NULL);

            // Create buttons
            CreateWindow("BUTTON", "Add Student", WS_VISIBLE | WS_CHILD, 20, 150, 100, 30, hwnd, (HMENU)IDC_ADD_BUTTON, NULL, NULL);
            CreateWindow("BUTTON", "Display Students", WS_VISIBLE | WS_CHILD, 130, 150, 120, 30, hwnd, (HMENU)IDC_DISPLAY_BUTTON, NULL, NULL);
            CreateWindow("BUTTON", "Allocate Room", WS_VISIBLE | WS_CHILD, 260, 150, 100, 30, hwnd, (HMENU)IDC_ALLOCATE_BUTTON, NULL, NULL);
            return 0;
        }
        case WM_COMMAND: {
            switch (LOWORD(wParam)) {
                case IDC_ADD_BUTTON: {
                    char rollStr[10], name[50], phone[15];
                    GetWindowText(hRollEdit, rollStr, sizeof(rollStr));
                    GetWindowText(hNameEdit, name, sizeof(name));
                    GetWindowText(hPhoneEdit, phone, sizeof(phone));

                    int rollNo = atoi(rollStr);
                    if (rollNo <= 0 || strlen(name) == 0 || strlen(phone) == 0) {
                        MessageBox(hwnd, "Please fill all fields correctly!", "Error", MB_OK | MB_ICONERROR);
                        return 0;
                    }

                    if (isDuplicateRollNo(rollNo)) {
                        MessageBox(hwnd, "Roll number already exists!", "Error", MB_OK | MB_ICONERROR);
                        return 0;
                    }

                    if (studentCount >= MAX_STUDENTS) {
                        MessageBox(hwnd, "Student limit reached!", "Error", MB_OK | MB_ICONERROR);
                        return 0;
                    }

                    struct Student newStudent;
                    newStudent.rollNo = rollNo;
                    strncpy(newStudent.name, name, sizeof(newStudent.name) - 1);
                    newStudent.name[sizeof(newStudent.name) - 1] = '\0';
                    strncpy(newStudent.phone, phone, sizeof(newStudent.phone) - 1);
                    newStudent.phone[sizeof(newStudent.phone) - 1] = '\0';
                    newStudent.roomNo = -1;
                    newStudent.isAllocated = 0;

                    students[studentCount++] = newStudent;
                    MessageBox(hwnd, "Student added successfully!", "Success", MB_OK | MB_ICONINFORMATION);

                    // Clear input fields
                    SetWindowText(hRollEdit, "");
                    SetWindowText(hNameEdit, "");
                    SetWindowText(hPhoneEdit, "");
                    return 0;
                }
                case IDC_DISPLAY_BUTTON: {
                    char buffer[1024] = "";
                    if (studentCount == 0) {
                        MessageBox(hwnd, "No students registered!", "Info", MB_OK | MB_ICONINFORMATION);
                        return 0;
                    }

                    strcat(buffer, "Roll No\tName\tPhone\tRoom No\n");
                    for (int i = 0; i < studentCount; i++) {
                        char line[100];
                        sprintf(line, "%d\t%s\t%s\t%d\n", 
                                students[i].rollNo, 
                                students[i].name, 
                                students[i].phone, 
                                students[i].isAllocated ? students[i].roomNo : -1);
                        strcat(buffer, line);
                    }
                    MessageBox(hwnd, buffer, "Student List", MB_OK | MB_ICONINFORMATION);
                    return 0;
                }
                case IDC_ALLOCATE_BUTTON: {
                    char rollStr[10], roomStr[10];
                    GetWindowText(hRollEdit, rollStr, sizeof(rollStr));
                    GetWindowText(hRoomEdit, roomStr, sizeof(roomStr));

                    int rollNo = atoi(rollStr);
                    int roomNo = atoi(roomStr);
                    if (rollNo <= 0 || roomNo <= 0) {
                        MessageBox(hwnd, "Please enter valid roll and room numbers!", "Error", MB_OK | MB_ICONERROR);
                        return 0;
                    }

                    int studentIndex = -1;
                    for (int i = 0; i < studentCount; i++) {
                        if (students[i].rollNo == rollNo) {
                            studentIndex = i;
                            break;
                        }
                    }

                    if (studentIndex == -1) {
                        MessageBox(hwnd, "Student not found!", "Error", MB_OK | MB_ICONERROR);
                        return 0;
                    }

                    if (students[studentIndex].isAllocated) {
                        MessageBox(hwnd, "Student already has a room allocated!", "Error", MB_OK | MB_ICONERROR);
                        return 0;
                    }

                    // Simple room allocation (no room array for simplicity)
                    students[studentIndex].isAllocated = 1;
                    students[studentIndex].roomNo = roomNo;
                    char msg[100];
                    sprintf(msg, "Room %d allocated to %s!", roomNo, students[studentIndex].name);
                    MessageBox(hwnd, msg, "Success", MB_OK | MB_ICONINFORMATION);

                    // Clear room input
                    SetWindowText(hRoomEdit, "");
                    return 0;
                }
            }
            return 0;
        }
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
        case WM_CLOSE:
            DestroyWindow(hwnd);
            return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    // Register the window class
    const char CLASS_NAME[] = "Hostel Management Window";
    WNDCLASS wc = { 0 };
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);

    RegisterClass(&wc);

    // Create the window
    HWND hwnd = CreateWindow(
        CLASS_NAME,
        "Hostel Management System",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 400, 250,
        NULL, NULL, hInstance, NULL
    );

    if (hwnd == NULL) {
        MessageBox(NULL, "Window creation failed!", "Error", MB_OK | MB_ICONERROR);
        return 0;
    }

    // Show the window
    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    // Message loop
    MSG msg = { 0 };
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}