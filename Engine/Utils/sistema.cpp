// Deixe o WIN32_LEAN_AND_MEAN apenas se for ajudar no tempo de compilação, 
// mas não fará diferença no Linux pois essa parte será ignorada pelo compilador.

#if defined(_WIN32)
    #include <windows.h>
    #include <psapi.h>
#else
    #include <unistd.h>
    #include <fstream>
#endif

#include "sistema.h"

size_t GetRAMUsedMB() {
#if defined(_WIN32)
    // Implementação para Windows
    PROCESS_MEMORY_COUNTERS pmc;
    if (GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc))) {
        return pmc.WorkingSetSize / (1024 * 1024);
    }
    return 0;
#else
    // Implementação para Linux
    std::ifstream statm("/proc/self/statm");
    if (statm.is_open()) {
        size_t size, resident;
        // O segundo valor do statm é a memória física residente (RSS) em "páginas"
        statm >> size >> resident;
        statm.close();
        
        // Descobre o tamanho de uma página de memória no sistema (geralmente 4096 bytes)
        long page_size = sysconf(_SC_PAGESIZE); 
        
        // Multiplica o número de páginas pelo tamanho da página para ter o total em bytes,
        // depois converte para MB.
        return (resident * page_size) / (1024 * 1024);
    }
    return 0;
#endif
}