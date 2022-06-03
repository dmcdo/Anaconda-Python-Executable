#include <stdio.h>
#include <string.h>
#include <windows.h>

#define PWSH1 ("powershell.exe -ExecutionPolicy ByPass -Command \"& ")
#define PWSH3 ("\\shell\\condabin\\conda-hook.ps1 ; conda activate ")
#define PWSH5 (" ; ")

#ifdef PYTHONW
#define PYTHONCMD ("pythonw ")
#define CREATIONFLAGS (0)
#else
#define PYTHONCMD ("python ")
#define CREATIONFLAGS (CREATE_NEW_PROCESS_GROUP)
#endif

char *strcpy_return_end(char *dest, const char *src)
{
    while (*dest++ = *src++)
        ;

    return dest - 1;
}

int main(int argc, char const *argv[])
{
    char *ANACONDAPATH = getenv("ANACONDAPATH");
    if (ANACONDAPATH == NULL)
    {
        fprintf(stderr, "FATAL ERROR: ANACONDAPATH is not defined.\n");
        exit(EXIT_FAILURE);
    }

    char *preamble[] = {PWSH1, ANACONDAPATH, PWSH3, ANACONDAPATH, PWSH5, PYTHONCMD};
    size_t plen = sizeof(preamble) / sizeof(preamble[0]);

    // Calculate size (in chars) of commandline
    size_t commandline_size = 1;
    for (size_t i = 0; i < plen; i++)
        commandline_size += strlen(preamble[i]);
    for (size_t i = 1; i < argc; i++)
        commandline_size += strlen(argv[i]) + 1;

    // Assemble commandline
    char *commandline = malloc(commandline_size);
    char *end = commandline;
    for (size_t i = 0; i < plen; i++)
        end = strcpy_return_end(end, preamble[i]);
    for (size_t i = 1; i < argc; i++)
    {
        end = strcpy_return_end(end, argv[i]);
        end = strcpy_return_end(end, " ");
    }

    // Create Python process
    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    ZeroMemory(&si, sizeof(si));
    ZeroMemory(&pi, sizeof(pi));
    si.cb = sizeof(si);

    BOOL success = CreateProcess(
        NULL,
        commandline,
        NULL,
        NULL,
        FALSE,
        CREATIONFLAGS,
        NULL,
        NULL,
        &si,
        &pi);

    if (success)
    {
        free(commandline);
        WaitForSingleObject(pi.hProcess, INFINITE);
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
    }
    else
    {
        DWORD error = GetLastError();
        fprintf(stderr, "FATAL ERROR CREATING PYTHON PROCESS (#%ld)", error);
        exit(EXIT_FAILURE);
    }

    return 0;
}
