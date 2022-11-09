#include <iostream>
#include "cpumonitor.h"


// The main script just prints out the CPU specs
int main()
{
    int data[3] {0};

    // Processor Vendor
    std::cout << "CPU vendor: ";
    std::cout << CPUM::getVendorName() << std::endl;

    // Brand name
    std::cout << "CPU name: ";
    std::cout << CPUM::getBrandName() << std::endl;

    // Family, Model, Stepping
    std::cout << "CPU family name: ";
    std::cout << CPUM::getFamilyName() << std::endl;
    std::cout << "Base Model: ";
    std::cout << CPUM::getBaseModelNumber() << std::endl;
    std::cout << "Base Family: ";
    std::cout << CPUM::getFamilyBaseNumber() << std::endl;
    std::cout << "Extended Model: ";
    std::cout << CPUM::getExtendedModelNumber() << std::endl;
    std::cout << "Extended Family: ";
    std::cout << CPUM::getFamilyExtendedNumber() << std::endl;
    std::cout << "Stepping: ";
    std::cout << CPUM::getSteppingNumber() << std::endl;

    // Other important data
    int localAPICID {0},
        logicalProcessorCount {0},
        clFlushSize {0},
        _8BitBrandID {0};
    bool htt {true};

    std::cout << std::hex << data[1] << std::endl;

    _8BitBrandID = data[1] & 255;
    clFlushSize = (data[1] >>= 8) & 255;
    logicalProcessorCount = (data[1] >>= 8) & 255;
    localAPICID = (data[1] >>= 8) & 255;
    // Check for cmp legacy!
    htt = (data[3] >> 28) & 1;

    std::cout << "8-Bit Brand: " << _8BitBrandID << std::endl;
    std::cout << "Cl Flush Size: " << clFlushSize << std::endl;
    // std::cout << "Hyper-threading: " << (htt ? "No" : "Yes") << std::endl;
    std::cout << "Logical Processor Count: " << logicalProcessorCount << std::endl;
    std::cout << "Local APIC ID: " << localAPICID << std::endl;

    // Feature identifiers 1
    bool SSE3,
         PCLMULQDQ,
         MONITOR,
         SSSE3,
         FMA,
         CMPXCHG16B,
         SSE41,
         SSE42,
         POPCNT,
         AES,
         XSAVE,
         OSXSAVE,
         AVX,
         F16C,
         RAZ;

    __asm__("mov $0x1, %eax\n\t");
    __asm__("cpuid\n\t");
    __asm__("mov %%ecx, %0\n\t":"=r" (data[2]));
    __asm__("mov %%edx, %0\n\t":"=r" (data[3]));

    // Change this
    SSE3 = (data[2] & 1);
    PCLMULQDQ = (data[2] >>= 1) & 1;
    MONITOR = (data[2] >>= 2) & 1;
    SSSE3 = (data[2] >>= 6) & 1;
    FMA = (data[2] >>= 3) & 1;
    CMPXCHG16B = (data[2] >>= 1) & 1;
    SSE41 = (data[2] >>= 6) & 1;
    SSE42 = (data[2] >>= 1) & 1;
    POPCNT = (data[2] >>= 3) & 1;
    AES = (data[2] >>= 2) & 1;
    XSAVE = (data[2] >>= 1) & 1;
    OSXSAVE = (data[2] >>= 1) & 1;
    AVX = (data[2] >>= 1) & 1;
    F16C = (data[2] >>= 1) & 1;
    RAZ = (data[2] >>= 2) & 1;

    std::cout << "SSE3: " << (SSE3 ? "Yes" : "No") << std::endl;
    std::cout << "PCLMULQDQ: " << (PCLMULQDQ ? "Yes" : "No") << std::endl;
    std::cout << "MONITOR: " << (MONITOR ? "Yes" : "No") << std::endl;
    std::cout << "SSSE3: " << (SSSE3 ? "Yes" : "No") << std::endl;
    std::cout << "FMA: " << (FMA ? "Yes" : "No") << std::endl;
    std::cout << "CMPXCHG16B: " << (CMPXCHG16B ? "Yes" : "No") << std::endl;
    std::cout << "SSE41: " << (SSE41 ? "Yes" : "No") << std::endl;
    std::cout << "SSE42: " << (SSE42 ? "Yes" : "No") << std::endl;
    std::cout << "POPCNT: " << (POPCNT ? "Yes" : "No") << std::endl;
    std::cout << "AES: " << (AES ? "Yes" : "No") << std::endl;
    std::cout << "XSAVE: " << (XSAVE ? "Yes" : "No") << std::endl;
    std::cout << "OSXSAVE: " << (OSXSAVE ? "Yes" : "No") << std::endl;
    std::cout << "AVX: " << (AVX ? "Yes" : "No") << std::endl;
    std::cout << "F16C: " << (F16C ? "Yes" : "No") << std::endl;
    std::cout << "RAZ: " << (RAZ ? "Yes" : "No") << std::endl;

    // Feature identifiers 2
    bool FPU,
         VME,
         DE,
         PSE,
         TSC,
         MSR,
         PAE,
         MCE,
         CMPXCHG8B,
         APIC,
         SysEnterSysExit,
         MTRR,
         PGE,
         MCA,
         CMOV,
         PAT,
         PSE36,
         CLFSH,
         MMX,
         FXSR,
         SSE,
         SSE2,
         HTT;

    FPU = (data[3] >> 0) & 1;
    VME = (data[3] >> 1) & 1;
    DE = (data[3] >> 2) & 1;
    PSE = (data[3] >> 3) & 1;
    TSC = (data[3] >> 4) & 1;
    MSR = (data[3] >> 5) & 1;
    PAE = (data[3] >> 6) & 1;
    MCE = (data[3] >> 7) & 1;
    CMPXCHG8B = (data[3] >> 8) & 1;
    APIC = (data[3] >> 9) & 1;
    SysEnterSysExit = (data[3] >> 11) & 1;
    MTRR = (data[3] >> 12) & 1;
    PGE = (data[3] >> 13) & 1;
    MCA = (data[3] >> 14) & 1;
    CMOV = (data[3] >> 15) & 1;
    PAT = (data[3] >> 16) & 1;
    PSE36 = (data[3] >> 17) & 1;
    CLFSH = (data[3] >> 19) & 1;
    MMX = (data[3] >> 23) & 1;
    FXSR = (data[3] >> 24) & 1;
    SSE = (data[3] >> 25) & 1;
    // Weird, the CPU is supposed to support this feature, will have to check the manual
    SSE2 = (data[3] >> 26) & 1;
    // Check legacy cmp
    HTT = (data[3] >> 28) & 1;

    std::cout << "FPU: " << (FPU ? "Yes" : "No") << std::endl;
    std::cout << "VME: " << (VME ? "Yes" : "No") << std::endl;
    std::cout << "DE: " << (DE ? "Yes" : "No") << std::endl;
    std::cout << "PSE: " << (PSE ? "Yes" : "No") << std::endl;
    std::cout << "TSC: " << (TSC ? "Yes" : "No") << std::endl;
    std::cout << "MSR: " << (MSR ? "Yes" : "No") << std::endl;
    std::cout << "PAE: " << (PAE ? "Yes" : "No") << std::endl;
    std::cout << "MCE: " << (MCE ? "Yes" : "No") << std::endl;
    std::cout << "CMPXCHG8B: " << (CMPXCHG8B ? "Yes" : "No") << std::endl;
    std::cout << "APIC: " << (APIC ? "Yes" : "No") << std::endl;
    std::cout << "SysEnterSysExit: " << (SysEnterSysExit ? "Yes" : "No") << std::endl;
    std::cout << "MTRR: " << (MTRR ? "Yes" : "No") << std::endl;
    std::cout << "PGE: " << (PGE ? "Yes" : "No") << std::endl;
    std::cout << "MCA: " << (MCA ? "Yes" : "No") << std::endl;
    std::cout << "CMOV: " << (CMOV ? "Yes" : "No") << std::endl;
    std::cout << "PAT: " << (PAT ? "Yes" : "No") << std::endl;
    std::cout << "PSE36: " << (PSE36 ? "Yes" : "No") << std::endl;
    std::cout << "CLFSH: " << (CLFSH ? "Yes" : "No") << std::endl;
    std::cout << "MMX: " << (MMX ? "Yes" : "No") << std::endl;
    std::cout << "FXSR: " << (FXSR ? "Yes" : "No") << std::endl;
    std::cout << "SSE: " << (SSE ? "Yes" : "No") << std::endl;
    std::cout << "SSE2: " << (SSE2 ? "Yes" : "No") << std::endl;
    std::cout << "HTT: " << (HTT ? "Yes" : "No") << std::endl;

    // More feature identifiers
    // ...

    return 0;
}
