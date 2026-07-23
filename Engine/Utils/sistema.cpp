//#define WIN32_LEAN_AND_MEAN Eu tirei porque defini no cpp

#include <windows.h>
#include <psapi.h>

#include "sistema.h"

size_t GetRAMUsedMB() {
    PROCESS_MEMORY_COUNTERS pmc;
    GetProcessMemoryInfo( GetCurrentProcess(), &pmc, sizeof(pmc) );

    return pmc.WorkingSetSize / (1024 * 1024);
}
