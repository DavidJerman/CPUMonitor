#ifndef CPUMONITOR_CPUMONITOR_H
#define CPUMONITOR_CPUMONITOR_H

//
// THIS LIBRARY IS AMD SPECIFIC FOR NOW
// https://www.amd.com/system/files/TechDocs/25481.pdf
//


// Includes
#include <fstream>
#include <string>
#include <algorithm>
#include <vector>


// Certain constants
#define AMDCpuVendor   "AuthenticAMD"
#define IntelCpuVendor "GenuineIntel"
#define cpuinfoFile    "/proc/cpuinfo"


// Type definitions
typedef const char *cString;
typedef unsigned char uint8;
typedef unsigned int uint32;
typedef unsigned long long uint64;


// This works with AMD CPUs for now


namespace CPUM {
    cString getBrandName();

    // The character buffer should be at least 16-bytes big
    void saveDataEABCDXToCStringBuffer(uint32 data[4], char *beg, const char * end);

    cString getVendorName();

    uint32 getFamilyNumber();

    uint32 getFamilyBaseNumber();

    uint32 getFamilyExtendedNumber();

    cString getMicroarchitecture();

    cString getMicroarchitectureAMD(uint32 familyNumber);

    cString getMicroarchitectureIntel(uint32 familyNumber);

    uint32 getModelNumber();

    uint32 getBaseModelNumber();

    uint32 getExtendedModelNumber();

    uint32 getSteppingNumber();

    uint32 getFeatures1();

    uint32 getFeatures2();

    uint32 getFn8000_0001_ECX();

    bool getBit(uint32 value, uint8 bit);

    // Other specifications
    uint32 getNC();

    uint32 getCoresPerProcessor();

    // CPU features
    bool hasSSE3();

    bool hasPCLMULQDQ();

    bool hasMONITOR();

    bool hasSSSE3();

    bool hasFMA();

    bool hasCMPXCHG16B();

    bool hasSSE41();

    bool hasSSE42();

    bool hasPOPCNT();

    bool hasAES();

    bool hasXSAVE();

    bool hasOSXSAVE();

    bool hasAVX();

    bool hasF16C();

    bool hasRAZ();

    bool hasFPU();

    bool hasVME();

    bool hasDE();

    bool hasPSE();

    bool hasTSC();

    bool hasMSR();

    bool hasPAE();

    bool hasMCE();

    bool hasCMPXCHG8B();

    bool hasAPIC();

    bool hasSysEnterSysExit();

    bool hasMTRR();

    bool hasPGE();

    bool hasMCA();

    bool hasCMOV();

    bool hasPAT();

    bool hasPSE36();

    bool hasCLFSH();

    bool hasMMX();

    bool hasFXSR();

    bool hasSSE();

    bool hasSSE2();

    bool hasHTT();

    // Features Fn8000_0001_ECX
    bool hasCmpLegacy();

    // Monitoring CPU performance
    // Mhz
    uint32 avgCoreFrequency();

    float avgCoreFrequencyGHz();

    std::vector<uint32> getCoreFrequencies();
}


#endif //CPUMONITOR_CPUMONITOR_H
