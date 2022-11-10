#include <iostream>
#include "cpumonitor.h"


// The main script just prints out the CPU specs
int main() {
    int data[3]{0};

    // Processor Vendor
    std::cout << "CPU vendor: " << CPUM::getVendorName() << std::endl;

    // Brand name
    std::cout << "CPU name: " << CPUM::getBrandName() << std::endl;

    // Family, Model, Stepping
    std::cout << "CPU microarchitecture: " << CPUM::getMicroarchitecture() << std::endl;
    std::cout << "Base Model: " << CPUM::getBaseModelNumber() << std::endl;
    std::cout << "Base Family: " << CPUM::getFamilyBaseNumber() << std::endl;
    std::cout << "Extended Model: " << CPUM::getExtendedModelNumber() << std::endl;
    std::cout << "Extended Family: " << CPUM::getFamilyExtendedNumber() << std::endl;
    std::cout << "Stepping: " << CPUM::getSteppingNumber() << std::endl;

    // Feature identifiers 1
    std::cout << "SSE3: " <<  (CPUM::hasSSE3() ? "Yes" : "No") << std::endl;
    std::cout << "PCLMULQDQ: " << (CPUM::hasPCLMULQDQ() ? "Yes" : "No") << std::endl;
    std::cout << "MONITOR: " << (CPUM::hasMONITOR() ? "Yes" : "No") << std::endl;
    std::cout << "SSSE3: " << (CPUM::hasSSSE3() ? "Yes" : "No") << std::endl;
    std::cout << "FMA: " << (CPUM::hasFMA() ? "Yes" : "No") << std::endl;
    std::cout << "CMPXCHG16B: " << (CPUM::hasCMPXCHG16B() ? "Yes" : "No") << std::endl;
    std::cout << "SSE41: " << (CPUM::hasSSE41() ? "Yes" : "No") << std::endl;
    std::cout << "SSE42: " << (CPUM::hasSSE42() ? "Yes" : "No") << std::endl;
    std::cout << "POPCNT: " << (CPUM::hasPOPCNT() ? "Yes" : "No") << std::endl;
    std::cout << "AES: " << (CPUM::hasAES() ? "Yes" : "No") << std::endl;
    std::cout << "XSAVE: " << (CPUM::hasXSAVE() ? "Yes" : "No") << std::endl;
    std::cout << "OSXSAVE: " << (CPUM::hasOSXSAVE() ? "Yes" : "No") << std::endl;
    std::cout << "AVX: " << (CPUM::hasAVX() ? "Yes" : "No") << std::endl;
    std::cout << "F16C: " << (CPUM::hasF16C() ? "Yes" : "No") << std::endl;
    std::cout << "RAZ: " << (CPUM::hasRAZ() ? "Yes" : "No") << std::endl;

    // Feature identifiers 2
    std::cout << "FPU: " << (CPUM::hasFPU() ? "Yes" : "No") << std::endl;
    std::cout << "VME: " << (CPUM::hasVME() ? "Yes" : "No") << std::endl;
    std::cout << "DE: " << (CPUM::hasDE() ? "Yes" : "No") << std::endl;
    std::cout << "PSE: " << (CPUM::hasPSE() ? "Yes" : "No") << std::endl;
    std::cout << "TSC: " << (CPUM::hasTSC() ? "Yes" : "No") << std::endl;
    std::cout << "MSR: " << (CPUM::hasMSR() ? "Yes" : "No") << std::endl;
    std::cout << "PAE: " << (CPUM::hasPAE() ? "Yes" : "No") << std::endl;
    std::cout << "MCE: " << (CPUM::hasMCE() ? "Yes" : "No") << std::endl;
    std::cout << "CMPXCHG8B: " << (CPUM::hasCMPXCHG8B() ? "Yes" : "No") << std::endl;
    std::cout << "APIC: " << (CPUM::hasAPIC() ? "Yes" : "No") << std::endl;
    std::cout << "SysEnterSysExit: " << (CPUM::hasSysEnterSysExit() ? "Yes" : "No") << std::endl;
    std::cout << "MTRR: " << (CPUM::hasMTRR() ? "Yes" : "No") << std::endl;
    std::cout << "PGE: " << (CPUM::hasPGE() ? "Yes" : "No") << std::endl;
    std::cout << "MCA: " << (CPUM::hasMCA() ? "Yes" : "No") << std::endl;
    std::cout << "CMOV: " << (CPUM::hasCMOV() ? "Yes" : "No") << std::endl;
    std::cout << "PAT: " << (CPUM::hasPAT() ? "Yes" : "No") << std::endl;
    std::cout << "PSE36: " << (CPUM::hasPSE36() ? "Yes" : "No") << std::endl;
    std::cout << "CLFSH: " << (CPUM::hasCLFSH() ? "Yes" : "No") << std::endl;
    std::cout << "MMX: " << (CPUM::hasMMX() ? "Yes" : "No") << std::endl;
    std::cout << "FXSR: " << (CPUM::hasFXSR() ? "Yes" : "No") << std::endl;
    std::cout << "SSE: " << (CPUM::hasSSE() ? "Yes" : "No") << std::endl;
    std::cout << "SSE2: " << (CPUM::hasSSE2() ? "Yes" : "No") << std::endl;
    std::cout << "HTT: " << (CPUM::hasHTT() ? "Yes" : "No") << std::endl;

    // More feature identifiers
    // ...

    // Frequency - windows
    std::cout << "Linux CPU frequency: " << CPUM::avgCoreFrequencyGHz() << " GHz" << std::endl;
    auto frequencies = CPUM::getCoreFrequencies();
    for (uint32 i = 0; i < frequencies.size(); ++i) {
        std::cout << i << ": " << frequencies[i] << " MHz" << std::endl;
    }

    // Core count
    std::cout << "No. cores: " << CPUM::getNC() << std::endl;

    return 0;
}
