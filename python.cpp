#include <stdio.h>
#include <string>
#include <windows.h>

#ifdef PYTHONW
#define PYTHONCMD ("pythonw ")
#define CREATIONFLAGS (0)
#else
#define PYTHONCMD ("python ")
#define CREATIONFLAGS (CREATE_NEW_PROCESS_GROUP)
#endif

int main(int argc, char const *argv[])
{
    char *ANACONDAPATH = getenv("ANACONDAPATH");
    if (ANACONDAPATH == NULL)
    {
        fprintf(stderr, "FATAL ERROR: ANACONDAPATH is not defined.\n");
        exit(EXIT_FAILURE);
    }

    std::string commandline = getenv("WINDIR");
    commandline += "\\System32\\WindowsPowerShell\\v1.0\\powershell.exe ";
    commandline += "-ExecutionPolicy ByPass -Command \"& ";
    commandline += getenv("ANACONDAPATH");
    commandline += "\\shell\\condabin\\conda-hook.ps1 ; conda activate ";
    commandline += getenv("ANACONDAPATH");
    commandline += " ; ";
    commandline += PYTHONCMD;
    for (int i = 1; i < argc; i++)
    {
        commandline += argv[i];
        commandline += " ";
    }
    commandline += " \"";

    // Create Python process
    STARTUPINFOA si;
    PROCESS_INFORMATION pi;
    ZeroMemory(&si, sizeof(si));
    ZeroMemory(&pi, sizeof(pi));
    si.cb = sizeof(si);

    BOOL success = CreateProcessA(
        NULL,
        const_cast<char *>(commandline.c_str()),
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
