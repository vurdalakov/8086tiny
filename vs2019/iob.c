// https://stackoverflow.com/a/34655235/11196063

#include <windows.h>
#include <stdio.h>
#include <dbghelp.h>

/* #define LOG */

#if defined(_M_IX86)

#define GET_CURRENT_CONTEXT(c, contextFlags) \
  do { \
    c.ContextFlags = contextFlags; \
    __asm    call x \
    __asm x: pop eax \
    __asm    mov c.Eip, eax \
    __asm    mov c.Ebp, ebp \
    __asm    mov c.Esp, esp \
  } while(0);

#else

/* This should work for 64-bit apps, but doesn't */
#define GET_CURRENT_CONTEXT(c, contextFlags) \
  do { \
    c.ContextFlags = contextFlags; \
    RtlCaptureContext(&c); \
} while(0);

#endif

FILE* __cdecl __iob_func(void)
{
    CONTEXT c = { 0 };
    STACKFRAME64 s = { 0 };
    DWORD imageType;
    HANDLE hThread = GetCurrentThread();
    HANDLE hProcess = GetCurrentProcess();

    GET_CURRENT_CONTEXT(c, CONTEXT_FULL);

#ifdef _M_IX86
    imageType = IMAGE_FILE_MACHINE_I386;
    s.AddrPC.Offset = c.Eip;
    s.AddrPC.Mode = AddrModeFlat;
    s.AddrFrame.Offset = c.Ebp;
    s.AddrFrame.Mode = AddrModeFlat;
    s.AddrStack.Offset = c.Esp;
    s.AddrStack.Mode = AddrModeFlat;
#elif _M_X64
    imageType = IMAGE_FILE_MACHINE_AMD64;
    s.AddrPC.Offset = c.Rip;
    s.AddrPC.Mode = AddrModeFlat;
    s.AddrFrame.Offset = c.Rsp;
    s.AddrFrame.Mode = AddrModeFlat;
    s.AddrStack.Offset = c.Rsp;
    s.AddrStack.Mode = AddrModeFlat;
#elif _M_IA64
    imageType = IMAGE_FILE_MACHINE_IA64;
    s.AddrPC.Offset = c.StIIP;
    s.AddrPC.Mode = AddrModeFlat;
    s.AddrFrame.Offset = c.IntSp;
    s.AddrFrame.Mode = AddrModeFlat;
    s.AddrBStore.Offset = c.RsBSP;
    s.AddrBStore.Mode = AddrModeFlat;
    s.AddrStack.Offset = c.IntSp;
    s.AddrStack.Mode = AddrModeFlat;
#else
#error "Platform not supported!"
#endif

    if (!StackWalk64(imageType, hProcess, hThread, &s, &c, NULL, SymFunctionTableAccess64, SymGetModuleBase64, NULL))
    {
#ifdef LOG
        printf("Error: 0x%08X (Address: %p)\n", GetLastError(), (LPVOID)s.AddrPC.Offset);
#endif
        return NULL;
    }

    if (s.AddrReturn.Offset == 0)
    {
        return NULL;
    }

    {
        unsigned char const* assembly = (unsigned char const*)(s.AddrReturn.Offset);
#ifdef LOG
        printf("Code bytes proceeding call to __iob_func: %p: %02X,%02X,%02X\n", assembly, *assembly, *(assembly + 1), *(assembly + 2));
#endif
        if (*assembly == 0x83 && *(assembly + 1) == 0xC0 && (*(assembly + 2) == 0x20 || *(assembly + 2) == 0x40))
        {
            if (*(assembly + 2) == 32)
            {
                return (FILE*)((unsigned char*)stdout - 32);
            }
            if (*(assembly + 2) == 64)
            {
                return (FILE*)((unsigned char*)stderr - 64);
            }

        }
        else
        {
            return stdin;
        }
    }
    return NULL;
}
