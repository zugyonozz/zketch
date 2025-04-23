// platform/win32/main.cpp
#include <windowsx.h>
#include <windows.h>
#include "zketch.h"
#include <iostream>
#include <memory>

class ZketchDemo {
	public:
		ZketchDemo();
		~ZketchDemo();
		bool initialize();
		void run();
		void cleanup();
	};

// Global variables
HWND g_hwnd = NULL;
bool g_isRunning = true;
std::unique_ptr<ZketchDemo> g_demo;

// Forward declarations
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
void TranslateWin32MouseEvent(UINT message, WPARAM wParam, LPARAM lParam);
void TranslateWin32KeyEvent(UINT message, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    // Register window class
    WNDCLASSEXW wcex = {0};
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.hInstance = hInstance;
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszClassName = L"ZketchDemoWindow";
    
    if (!RegisterClassExW(&wcex)) {
        MessageBoxW(NULL, L"Window Registration Failed!", L"Error", MB_ICONEXCLAMATION | MB_OK);
        return 1;
    }
    
    // Create window
    g_hwnd = CreateWindowExW(
        WS_EX_CLIENTEDGE,
        L"ZketchDemoWindow",
        L"zketch Demo Application",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 
        640, 480,
        NULL, NULL, hInstance, NULL
    );
    
    if (g_hwnd == NULL) {
        MessageBoxW(NULL, L"Window Creation Failed!", L"Error", MB_ICONEXCLAMATION | MB_OK);
        return 1;
    }
    
    // Initialize zketch demo
    g_demo = std::make_unique<ZketchDemo>();
    if (!g_demo->initialize()) {
        MessageBoxW(g_hwnd, L"Failed to initialize zketch demo!", L"Error", MB_ICONEXCLAMATION | MB_OK);
        return 1;
    }
    
    // Show window
    ShowWindow(g_hwnd, nCmdShow);
    UpdateWindow(g_hwnd);
    
    // Message loop
    MSG msg;
    DWORD lastTime = GetTickCount();
    
    while (g_isRunning) {
        // Process Windows messages
        while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            if (msg.message == WM_QUIT) {
                g_isRunning = false;
                break;
            }
            
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        
        // Calculate delta time
        DWORD currentTime = GetTickCount();
        float deltaTime = (currentTime - lastTime) / 1000.0f;
        lastTime = currentTime;
        
        // Update and render zketch UI
        auto& uiManager = zketch::UIManager::getInstance();
        uiManager.update(deltaTime);
        
        // Render (in a real app, this would use Direct2D, GDI, etc.)
        HDC hdc = GetDC(g_hwnd);
        // Set up rendering context...
        
        uiManager.render();
        
        // Clean up rendering context...
        ReleaseDC(g_hwnd, hdc);
        
        // Limit frame rate
        Sleep(16); // ~60 FPS
    }
    
    // Cleanup
    g_demo->cleanup();
    
    return static_cast<int>(msg.wParam);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    auto& uiManager = zketch::UIManager::getInstance();
    
    switch (message) {
        case WM_CLOSE:
            DestroyWindow(hwnd);
            break;
            
        case WM_DESTROY:
            g_isRunning = false;
            PostQuitMessage(0);
            break;
            
        case WM_LBUTTONDOWN:
        case WM_LBUTTONUP:
        case WM_RBUTTONDOWN:
        case WM_RBUTTONUP:
        case WM_MOUSEMOVE:
            TranslateWin32MouseEvent(message, wParam, lParam);
            break;
            
        case WM_KEYDOWN:
        case WM_KEYUP:
            TranslateWin32KeyEvent(message, wParam, lParam);
            break;
            
        default:
            return DefWindowProc(hwnd, message, wParam, lParam);
    }
    
    return 0;
}

void TranslateWin32MouseEvent(UINT message, WPARAM wParam, LPARAM lParam) {
    auto& uiManager = zketch::UIManager::getInstance();
    
    // Create mouse event
    zketch::MouseEvent mouseEvent;
    mouseEvent.position.x = GET_X_LPARAM(lParam);
    mouseEvent.position.y = GET_Y_LPARAM(lParam);
    mouseEvent.leftButton = (wParam & MK_LBUTTON) != 0;
    mouseEvent.rightButton = (wParam & MK_RBUTTON) != 0;
    mouseEvent.middleButton = (wParam & MK_MBUTTON) != 0;
    
    // Determine event type
    zketch::EventType eventType;
    switch (message) {
        case WM_LBUTTONDOWN:
            eventType = zketch::EventType::MouseDown;
            break;
        case WM_LBUTTONUP:
            eventType = zketch::EventType::MouseUp;
            break;
        case WM_MOUSEMOVE:
            eventType = zketch::EventType::MouseMove;
            break;
        default:
            return;
    }
    
    // Dispatch to UI manager
    uiManager.handleMouseEvent(mouseEvent, eventType);
}

void TranslateWin32KeyEvent(UINT message, WPARAM wParam, LPARAM lParam) {
    auto& uiManager = zketch::UIManager::getInstance();
    
    // Create key event
    zketch::KeyEvent keyEvent;
    keyEvent.keyCode = static_cast<int>(wParam);
    keyEvent.shift = (GetKeyState(VK_SHIFT) & 0x8000) != 0;
    keyEvent.control = (GetKeyState(VK_CONTROL) & 0x8000) != 0;
    keyEvent.alt = (GetKeyState(VK_MENU) & 0x8000) != 0;
    
    // Determine event type
    zketch::EventType eventType = (message == WM_KEYDOWN) ? 
        zketch::EventType::KeyDown : zketch::EventType::KeyUp;
    
    // Dispatch to UI manager
    uiManager.handleKeyEvent(keyEvent, eventType);
}