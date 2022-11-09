#ifndef CPUMONITOR_CPUMONITOR_H
#define CPUMONITOR_CPUMONITOR_H

//
// THIS LIBRARY IS AMD SPECIFIC FOR NOW
// https://www.amd.com/system/files/TechDocs/25481.pdf
//

// Certain constants
#define AMDCpuVendor   "AuthenticAMD"
#define IntelCpuVendor "GenuineIntel"


// Type definitions
typedef const char *cString;
typedef unsigned char uint8;
typedef unsigned int uint32;


// This works with AMD CPUs for now


namespace CPUM {
    cString getBrandName();

    // The character buffer should be at least 16-bytes big
    void saveDataEABCDXToCStringBuffer(uint32 data[4], char *beg, const char *const end);

    cString getVendorName();

    uint32 getFamilyNumber();

    uint32 getFamilyBaseNumber();

    uint32 getFamilyExtendedNumber();

    cString getFamilyName();

    cString familyNumberToNameAMD(uint32 familyNumber);

    uint32 getModelNumber();

    uint32 getBaseModelNumber();

    uint32 getExtendedModelNumber();

    uint32 getSteppingNumber();

    uint32 getFeatures1();

    uint32 getFeatures2();

    bool getBit(uint32 value, uint8 bit);

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
}


#endif //CPUMONITOR_CPUMONITOR_H
