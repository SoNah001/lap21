#include <windows.h>
#include <stdio.h>

HWND hEdit1, hEdit2 ,hStatic;

LRESULT CALLBACK WndProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam) {
	char text1[50], text2[50];
    double num1, num2, result;
    char resultText[100];
	switch(Message) {
		
		/* Upon destruction, tell the main thread to stop */
		case WM_DESTROY: {
			PostQuitMessage(0);
			break;
		}
		
		case WM_CREATE:
			hStatic = CreateWindow("STATIC", "Please input two numbers",WS_CHILD | WS_VISIBLE | SS_CENTER,
									50, 20, 200, 20, hwnd, NULL, NULL, NULL);
            hEdit1 = CreateWindow("EDIT", "", WS_CHILD | WS_VISIBLE | WS_BORDER,
                                  50, 50, 150, 20, hwnd, (HMENU)1, NULL, NULL);
            hEdit2 = CreateWindow("EDIT", "", WS_CHILD | WS_VISIBLE | WS_BORDER,
                                  50, 80, 150, 20, hwnd, (HMENU)2, NULL, NULL);

            CreateWindow("BUTTON", "+", WS_CHILD | WS_VISIBLE,
                         50, 120, 40, 30, hwnd, (HMENU)3, NULL, NULL);
            CreateWindow("BUTTON", "-", WS_CHILD | WS_VISIBLE,
                         100, 120, 40, 30, hwnd, (HMENU)4, NULL, NULL);
            CreateWindow("BUTTON", "*", WS_CHILD | WS_VISIBLE,
                         150, 120, 40, 30, hwnd, (HMENU)5, NULL, NULL);
            CreateWindow("BUTTON", "/", WS_CHILD | WS_VISIBLE,
                         200, 120, 40, 30, hwnd, (HMENU)6, NULL, NULL);
            break;

			case WM_COMMAND:
            GetWindowText(hEdit1, text1, 50);
            GetWindowText(hEdit2, text2, 50);
            num1 = atof(text1);
            num2 = atof(text2);

            switch (LOWORD(wParam)) {
                case 3: result = num1 + num2; break;
                case 4: result = num1 - num2; break;
                case 5: result = num1 * num2; break;
                case 6:
                    if (num2 == 0) {
                        MessageBox(hwnd, "Cannot divide by zero!", "Error", MB_OK | MB_ICONERROR);
                        return 0;
                    }
                    result = num1 / num2;
                    break;
                default: return 0;
            }
            sprintf(resultText, "Result: %.2f", result);
            MessageBox(hwnd, resultText, "Result", MB_OK);
            break;

        case WM_CLOSE:
            DestroyWindow(hwnd);
            break;

		default:
			return DefWindowProc(hwnd, Message, wParam, lParam);
	}
	return 0;
}

/* The 'main' function of Win32 GUI programs: this is where execution starts */
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	WNDCLASSEX wc = {0};
	HWND hwnd; /* A 'HANDLE', hence the H, or a pointer to our window */
	MSG msg; /* A temporary location for all messages */

	/* zero out the struct and set the stuff we want to modify */
	memset(&wc,0,sizeof(wc));
	wc.cbSize	 = sizeof(WNDCLASSEX);
	wc.lpfnWndProc	 = WndProc; /* This is where we will send messages to */
	wc.hInstance	 = hInstance;
	wc.hCursor	 = LoadCursor(NULL, IDC_ARROW);
	
	/* White, COLOR_WINDOW is just a #define for a system color, try Ctrl+Clicking it */
	wc.hbrBackground = CreateSolidBrush(RGB(0, 255, 255));
	wc.lpszClassName = "WindowClass";
	wc.hIcon	 = LoadIcon(NULL, IDI_APPLICATION); /* Load a standard icon */
	wc.hIconSm	 = LoadIcon(NULL, IDI_APPLICATION); /* use the name "A" to use the project icon */

	if(!RegisterClassEx(&wc)) {
		MessageBox(NULL, "Window Registration Failed!","Error!",MB_ICONEXCLAMATION|MB_OK);
		return 0;
	}

	hwnd = CreateWindowEx(0,"WindowClass","My Calculator",WS_OVERLAPPED|WS_CAPTION|WS_SYSMENU,
		CW_USEDEFAULT, /* x */
		CW_USEDEFAULT, /* y */
		250, /* width */
		200, /* height */
		NULL,NULL,hInstance,NULL);

	if(hwnd == NULL) {
		MessageBox(NULL, "Window Creation Failed!","Error!",MB_ICONEXCLAMATION|MB_OK);
		return 0;
	}

	ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

	while(GetMessage(&msg, NULL, 0, 0) > 0) { /* If no error is received... */
		TranslateMessage(&msg); /* Translate key codes to chars if present */
		DispatchMessage(&msg); /* Send it to WndProc */
	}
	return msg.wParam;
}
