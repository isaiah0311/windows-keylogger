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
        printf("vkCode = %d\n", input->vkCode);
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
