#include <windows.h>
#include <stdio.h>
#include <string.h>

#define MAX_TASKS 100
#define MAX_TASK_LENGTH 256
#define IDC_EDIT 101
#define IDC_LIST 102
#define IDC_ADD 103
#define IDC_COMPLETE 104
#define IDC_DELETE 105
#define IDC_EXIT 106

// Structure to represent a task
typedef struct {
    char description[MAX_TASK_LENGTH];
    int completed;
} Task;

// Global variables
Task tasks[MAX_TASKS];
int task_count = 0;
HWND hEdit, hList;

// Function to add a task
void add_task(const char* description, HWND hwndList) {
    if (task_count >= MAX_TASKS) {
        MessageBox(NULL, "Task list is full!", "Error", MB_OK | MB_ICONERROR);
        return;
    }
    strncpy(tasks[task_count].description, description, MAX_TASK_LENGTH - 1);
    tasks[task_count].description[MAX_TASK_LENGTH - 1] = '\0';
    tasks[task_count].completed = 0;
    char display_text[MAX_TASK_LENGTH + 20];
    sprintf(display_text, "%s [%s]", tasks[task_count].description, "Pending");
    SendMessage(hwndList, LB_ADDSTRING, 0, (LPARAM)display_text);
    task_count++;
    MessageBox(NULL, "Task added successfully!", "Success", MB_OK | MB_ICONINFORMATION);
}

// Function to mark a task as completed
void complete_task(int index, HWND hwndList) {
    if (index < 0 || index >= task_count) {
        MessageBox(NULL, "Invalid task selection!", "Error", MB_OK | MB_ICONERROR);
        return;
    }
    tasks[index].completed = 1;
    SendMessage(hwndList, LB_DELETESTRING, index, 0);
    char display_text[MAX_TASK_LENGTH + 20];
    sprintf(display_text, "%s [%s]", tasks[index].description, "Completed");
    SendMessage(hwndList, LB_INSERTSTRING, index, (LPARAM)display_text);
    MessageBox(NULL, "Task marked as completed!", "Success", MB_OK | MB_ICONINFORMATION);
}

// Function to delete a task
void delete_task(int index, HWND hwndList) {
    if (index < 0 || index >= task_count) {
        MessageBox(NULL, "Invalid task selection!", "Error", MB_OK | MB_ICONERROR);
        return;
    }
    for (int i = index; i < task_count - 1; i++) {
        tasks[i] = tasks[i + 1];
    }
    task_count--;
    SendMessage(hwndList, LB_DELETESTRING, index, 0);
    MessageBox(NULL, "Task deleted successfully!", "Success", MB_OK | MB_ICONINFORMATION);
}

// Window procedure
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
        case WM_CREATE:
            // Create edit control for task input
            hEdit = CreateWindow("EDIT", "", WS_CHILD | WS_VISIBLE | WS_BORDER,
                                 10, 10, 300, 25, hwnd, (HMENU)IDC_EDIT, NULL, NULL);
            // Create list box for tasks
            hList = CreateWindow("LISTBOX", "", WS_CHILD | WS_VISIBLE | WS_BORDER | LBS_NOTIFY,
                                 10, 45, 300, 150, hwnd, (HMENU)IDC_LIST, NULL, NULL);
            // Create buttons
            CreateWindow("BUTTON", "Add Task", WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,
                         320, 10, 100, 25, hwnd, (HMENU)IDC_ADD, NULL, NULL);
            CreateWindow("BUTTON", "Complete Task", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                         320, 45, 100, 25, hwnd, (HMENU)IDC_COMPLETE, NULL, NULL);
            CreateWindow("BUTTON", "Delete Task", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                         320, 80, 100, 25, hwnd, (HMENU)IDC_DELETE, NULL, NULL);
            CreateWindow("BUTTON", "Exit", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                         320, 115, 100, 25, hwnd, (HMENU)IDC_EXIT, NULL, NULL);
            break;

        case WM_COMMAND:
            switch (LOWORD(wParam)) {
                case IDC_ADD:
                    {
                        char buffer[MAX_TASK_LENGTH];
                        GetWindowText(hEdit, buffer, MAX_TASK_LENGTH);
                        if (strlen(buffer) > 0) {
                            add_task(buffer, hList);
                            SetWindowText(hEdit, ""); // Clear input field
                        } else {
                            MessageBox(hwnd, "Please enter a task description!", "Warning", MB_OK | MB_ICONWARNING);
                        }
                    }
                    break;
                case IDC_COMPLETE:
                    {
                        int index = SendMessage(hList, LB_GETCURSEL, 0, 0);
                        complete_task(index, hList);
                    }
                    break;
                case IDC_DELETE:
                    {
                        int index = SendMessage(hList, LB_GETCURSEL, 0, 0);
                        delete_task(index, hList);
                    }
                    break;
                case IDC_EXIT:
                    PostQuitMessage(0);
                    break;
            }
            break;

        case WM_DESTROY:
            PostQuitMessage(0);
            break;

        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

// Main function
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    const char CLASS_NAME[] = "ToDoListWindow";
    WNDCLASS wc = {0};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    RegisterClass(&wc);

    HWND hwnd = CreateWindow(CLASS_NAME, "Resume To-Do List App",
                            WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
                            450, 250, NULL, NULL, hInstance, NULL);

    if (hwnd == NULL) {
        MessageBox(NULL, "Window creation failed!", "Error", MB_OK | MB_ICONERROR);
        return 0;
    }

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    MSG msg = {0};
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return msg.wParam;
}