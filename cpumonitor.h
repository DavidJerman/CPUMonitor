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
typedef const char * cString;
typedef unsigned char uint8;
typedef unsigned int uint32;


// This works with AMD CPUs for now


namespace CPUM {
    cString getBrandName();

    // The character buffer should be at least 16-bytes big
    void saveDataEABCDXToCStringBuffer(uint32 data[4], char * beg, const char *const end);

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

    // CPU features
}




#endif //CPUMONITOR_CPUMONITOR_H
