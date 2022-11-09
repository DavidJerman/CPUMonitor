#include <cstring>

#include "cpumonitor.h"

//
// THIS LIBRARY IS AMD SPECIFIC FOR NOW
// https://www.amd.com/system/files/TechDocs/25481.pdf
//


cString CPUM::getBrandName() {
    auto name = new char[49];
    uint32 data[4];

    __asm__("push %rax\n\t");
    __asm__("push %rbx\n\t");
    __asm__("push %rcx\n\t");
    __asm__("push %rdx\n\t");

    __asm__("xor %eax , %eax\n\t");
    __asm__("xor %ebx , %ebx\n\t");
    __asm__("xor %ecx , %ecx\n\t");
    __asm__("xor %edx , %edx\n\t");

    __asm__("mov $0x80000002, %eax\n\t");
    __asm__("cpuid\n\t");
    __asm__("mov %%eax, %0\n\t":"=r" (data[0]));
    __asm__("mov %%ebx, %0\n\t":"=r" (data[1]));
    __asm__("mov %%ecx, %0\n\t":"=r" (data[2]));
    __asm__("mov %%edx, %0\n\t":"=r" (data[3]));

    __asm__("pop %rdx\n\t");
    __asm__("pop %rcx\n\t");
    __asm__("pop %rbx\n\t");
    __asm__("pop %rax\n\t");

    saveDataEABCDXToCStringBuffer(data, &name[0], &name[16]);

    __asm__("push %rax\n\t");
    __asm__("push %rbx\n\t");
    __asm__("push %rcx\n\t");
    __asm__("push %rdx\n\t");

    __asm__("mov $0x80000003, %eax\n\t");
    __asm__("cpuid\n\t");
    __asm__("mov %%eax, %0\n\t":"=r" (data[0]));
    __asm__("mov %%ebx, %0\n\t":"=r" (data[1]));
    __asm__("mov %%ecx, %0\n\t":"=r" (data[2]));
    __asm__("mov %%edx, %0\n\t":"=r" (data[3]));

    __asm__("pop %rdx\n\t");
    __asm__("pop %rcx\n\t");
    __asm__("pop %rbx\n\t");
    __asm__("pop %rax\n\t");

    saveDataEABCDXToCStringBuffer(data, &name[16], &name[32]);

    __asm__("push %rax\n\t");
    __asm__("push %rbx\n\t");
    __asm__("push %rcx\n\t");
    __asm__("push %rdx\n\t");

    __asm__("mov $0x80000004, %eax\n\t");
    __asm__("cpuid\n\t");
    __asm__("mov %%eax, %0\n\t":"=r" (data[0]));
    __asm__("mov %%ebx, %0\n\t":"=r" (data[1]));
    __asm__("mov %%ecx, %0\n\t":"=r" (data[2]));
    __asm__("mov %%edx, %0\n\t":"=r" (data[3]));

    __asm__("pop %rdx\n\t");
    __asm__("pop %rcx\n\t");
    __asm__("pop %rbx\n\t");
    __asm__("pop %rax\n\t");

    saveDataEABCDXToCStringBuffer(data, &name[32], &name[48]);

    name[48] = 0;

    return (cString) name;
}


// Return nullptr if the char buffer is too small
void CPUM::saveDataEABCDXToCStringBuffer(uint32 data[4], char *beg, const char *const end) {
    if (end - beg < 16) {
        *beg = 0;
        return;
    }
    uint32 data_copy[4];
    std::memcpy(data_copy, data, 4);
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            auto letter = (char) (data[i] & 255);
            data[i] >>= 8;
            *beg = (char) letter;
            beg++;
        }
    }
}


cString CPUM::getVendorName() {
    uint32 data[4];
    auto cStr = new char[13];

    __asm__("push %rax\n\t");
    __asm__("push %rbx\n\t");
    __asm__("push %rcx\n\t");
    __asm__("push %rdx\n\t");

    __asm__("mov $0x0, %rax\n\t");
    __asm__("cpuid\n\t");
    __asm__("mov %%ebx, %0\n\t":"=r" (data[0]));
    __asm__("mov %%edx, %0\n\t":"=r" (data[1]));
    __asm__("mov %%ecx, %0\n\t":"=r" (data[2]));

    __asm__("pop %rdx\n\t");
    __asm__("pop %rcx\n\t");
    __asm__("pop %rbx\n\t");
    __asm__("pop %rax\n\t");

    for (uint8 i = 0; i < 3; i++) {
        for (uint8 j = 0; j < 4; j++) {
            auto letter = (char) (data[i] & 255);
            data[i] >>= 8;
            cStr[i * 4 + j] = (char) letter;
        }
    }

    cStr[12] = 0;

    return (cString) cStr;
}


uint32 CPUM::getFamilyNumber() {
    uint32 data;

    __asm__("push %rax\n\t");
    __asm__("push %rbx\n\t");
    __asm__("push %rcx\n\t");
    __asm__("push %rdx\n\t");

    __asm__("mov $0x1, %eax\n\t");
    __asm__("cpuid\n\t");
    __asm__("mov %%eax, %0\n\t":"=r" (data));

    __asm__("pop %rdx\n\t");
    __asm__("pop %rcx\n\t");
    __asm__("pop %rbx\n\t");
    __asm__("pop %rax\n\t");

    uint8 baseFamily{0};
    uint8 extendedFamily{0};

    baseFamily = (data >> 8) & 15;
    if (baseFamily < 15) {
        return baseFamily;
    }
    extendedFamily = (data >> 20) & 15;

    return baseFamily + extendedFamily;
}


uint32 CPUM::getFamilyBaseNumber() {
    uint32 data;

    __asm__("push %rax\n\t");
    __asm__("push %rbx\n\t");
    __asm__("push %rcx\n\t");
    __asm__("push %rdx\n\t");

    __asm__("mov $0x1, %eax\n\t");
    __asm__("cpuid\n\t");
    __asm__("mov %%eax, %0\n\t":"=r" (data));

    __asm__("pop %rdx\n\t");
    __asm__("pop %rcx\n\t");
    __asm__("pop %rbx\n\t");
    __asm__("pop %rax\n\t");

    uint8 baseFamily = (data >> 8) & 15;

    return baseFamily;
}


uint32 CPUM::getFamilyExtendedNumber() {
    uint32 data;

    __asm__("push %rax\n\t");
    __asm__("push %rbx\n\t");
    __asm__("push %rcx\n\t");
    __asm__("push %rdx\n\t");

    __asm__("mov $0x1, %eax\n\t");
    __asm__("cpuid\n\t");
    __asm__("mov %%eax, %0\n\t":"=r" (data));

    __asm__("pop %rdx\n\t");
    __asm__("pop %rcx\n\t");
    __asm__("pop %rbx\n\t");
    __asm__("pop %rax\n\t");

    uint8 extendedFamily = (data >> 20) & 15;

    return extendedFamily;
}


// Obtaining accurate family name with model names --> TBD
// TODO: Get accurate model names
cString CPUM::getFamilyName() {
    auto familyNumber = getFamilyNumber();

    auto vendorName = getVendorName();

    // This check probably works, but I would have to check
    if (std::strcmp(vendorName, AMDCpuVendor) == 0)
        return familyNumberToNameAMD(familyNumber);
    else
        return "Unknown CPU vendor";
}


// This is just for AMD for now
// Works with CPU generations up to Zen 4
// Data taken from https://en.wikipedia.org/wiki/List_of_AMD_CPU_microarchitectures
cString CPUM::familyNumberToNameAMD(uint32 familyNumber) {
    cString familyName;

    switch (familyNumber) {
        case 5:
            familyName = "K6";
            break;
        case 6:
            familyName = "K7";
            break;
        case 15:
            familyName = "K8 / Hammer";
            break;
        case 16:
            familyName = "K10";
            break;
        case 17:
            familyName = "K8 & K10 \"hybrid\"";
            break;
        case 18:
            familyName = "K10 (Llano) / K12";
            break;
        case 20:
            familyName = "Bobcat";
            break;
        case 21:
            familyName = "Bulldozer / Piledriver / Steamroller / Excavator";
            break;
        case 22:
            familyName = "Jaguar / Puma";
            break;
        case 23:
            familyName = "Zen / Zen+ / Zen 2";
            break;
        case 24:
            familyName = "Hygon Dhyana";
            break;
        case 25:
            familyName = "Zen 3 / Zen 3+ / Zen 4";
            break;
        default:
            familyName = "Unknown family";
            break;
    }

    return familyName;
}


uint32 CPUM::getModelNumber() {
    uint32 data;

    __asm__("push %rax\n\t");
    __asm__("push %rbx\n\t");
    __asm__("push %rcx\n\t");
    __asm__("push %rdx\n\t");

    __asm__("mov $0x1, %eax\n\t");
    __asm__("cpuid\n\t");
    __asm__("mov %%eax, %0\n\t":"=r" (data));

    __asm__("pop %rdx\n\t");
    __asm__("pop %rcx\n\t");
    __asm__("pop %rbx\n\t");
    __asm__("pop %rax\n\t");

    uint8 baseModel{0};
    uint8 extendedModel{0};
    auto baseFamily = getFamilyNumber();

    baseModel = (data >> 4) & 15;
    if (baseFamily < 15) {
        return baseModel;
    }
    extendedModel = (data >> 16) & 15;
    auto model = (extendedModel << 4) + baseModel;

    return model;
}


uint32 CPUM::getBaseModelNumber() {
    uint32 data;

    __asm__("push %rax\n\t");
    __asm__("push %rbx\n\t");
    __asm__("push %rcx\n\t");
    __asm__("push %rdx\n\t");

    __asm__("mov $0x1, %eax\n\t");
    __asm__("cpuid\n\t");
    __asm__("mov %%eax, %0\n\t":"=r" (data));

    __asm__("pop %rdx\n\t");
    __asm__("pop %rcx\n\t");
    __asm__("pop %rbx\n\t");
    __asm__("pop %rax\n\t");

    uint8 baseModel = (data >> 4) & 15;

    return baseModel;
}


uint32 CPUM::getExtendedModelNumber() {
    uint32 data;

    __asm__("push %rax\n\t");
    __asm__("push %rbx\n\t");
    __asm__("push %rcx\n\t");
    __asm__("push %rdx\n\t");

    __asm__("mov $0x1, %eax\n\t");
    __asm__("cpuid\n\t");
    __asm__("mov %%eax, %0\n\t":"=r" (data));

    __asm__("pop %rdx\n\t");
    __asm__("pop %rcx\n\t");
    __asm__("pop %rbx\n\t");
    __asm__("pop %rax\n\t");

    uint8 extendedModel = (data >> 16) & 15;

    return extendedModel;
}


uint32 CPUM::getSteppingNumber() {
    uint32 data;

    __asm__("push %rax\n\t");
    __asm__("push %rbx\n\t");
    __asm__("push %rcx\n\t");
    __asm__("push %rdx\n\t");

    __asm__("mov $0x1, %eax\n\t");
    __asm__("cpuid\n\t");
    __asm__("mov %%eax, %0\n\t":"=r" (data));

    __asm__("pop %rdx\n\t");
    __asm__("pop %rcx\n\t");
    __asm__("pop %rbx\n\t");
    __asm__("pop %rax\n\t");

    uint8 steppingNumber = data & 15;

    return steppingNumber;
}


uint32 CPUM::getFeatures1() {
    uint32 data;

    __asm__("push %rax\n\t");
    __asm__("push %rbx\n\t");
    __asm__("push %rcx\n\t");
    __asm__("push %rdx\n\t");

    __asm__("mov $0x1, %eax\n\t");
    __asm__("cpuid\n\t");
    __asm__("mov %%ecx, %0\n\t":"=r" (data));

    __asm__("pop %rdx\n\t");
    __asm__("pop %rcx\n\t");
    __asm__("pop %rbx\n\t");
    __asm__("pop %rax\n\t");

    return data;
}


uint32 CPUM::getFeatures2() {
    uint32 data;

    __asm__("push %rax\n\t");
    __asm__("push %rbx\n\t");
    __asm__("push %rcx\n\t");
    __asm__("push %rdx\n\t");

    __asm__("mov $0x1, %eax\n\t");
    __asm__("cpuid\n\t");
    __asm__("mov %%edx, %0\n\t":"=r" (data));

    __asm__("pop %rdx\n\t");
    __asm__("pop %rcx\n\t");
    __asm__("pop %rbx\n\t");
    __asm__("pop %rax\n\t");

    return data;
}


bool CPUM::getBit(uint32 value, uint8 bit) {
    return (value >> bit) & 1;
}


// CPU Features
bool CPUM::hasSSE3() {
    return getBit(getFeatures1(), 0);
}


bool CPUM::hasPCLMULQDQ() {
    return getBit(getFeatures1(), 1);
}


bool CPUM::hasMONITOR() {
    return getBit(getFeatures1(), 3);
}


bool CPUM::hasSSSE3() {
    return getBit(getFeatures1(), 9);
}


bool CPUM::hasFMA() {
    return getBit(getFeatures1(), 12);
}


bool CPUM::hasCMPXCHG16B() {
    return getBit(getFeatures1(), 13);
}


bool CPUM::hasSSE41() {
    return getBit(getFeatures1(), 19);
}


bool CPUM::hasSSE42() {
    return getBit(getFeatures1(), 20);
}


bool CPUM::hasPOPCNT() {
    return getBit(getFeatures1(), 23);
}


bool CPUM::hasAES() {
    return getBit(getFeatures1(), 25);
}


bool CPUM::hasXSAVE() {
    return getBit(getFeatures1(), 26);
}


bool CPUM::hasOSXSAVE() {
    return getBit(getFeatures1(), 27);
}


bool CPUM::hasAVX() {
    return getBit(getFeatures1(), 28);
}


bool CPUM::hasF16C() {
    return getBit(getFeatures1(), 29);
}


bool CPUM::hasRAZ() {
    return getBit(getFeatures1(), 31);
}


bool CPUM::hasFPU() {
    return getBit(getFeatures2(), 0);
}


bool CPUM::hasVME() {
    return getBit(getFeatures2(), 1);
}


bool CPUM::hasDE() {
    return getBit(getFeatures2(), 2);
}


bool CPUM::hasPSE() {
    return getBit(getFeatures2(), 3);
}


bool CPUM::hasTSC() {
    return getBit(getFeatures2(), 4);
}


bool CPUM::hasMSR() {
    return getBit(getFeatures2(), 5);
}


bool CPUM::hasPAE() {
    return getBit(getFeatures2(), 6);
}


bool CPUM::hasMCE() {
    return getBit(getFeatures2(), 7);
}


bool CPUM::hasCMPXCHG8B() {
    return getBit(getFeatures2(), 8);
}


bool CPUM::hasAPIC() {
    return getBit(getFeatures2(), 9);
}


bool CPUM::hasSysEnterSysExit() {
    return getBit(getFeatures2(), 11);
}


bool CPUM::hasMTRR() {
    return getBit(getFeatures2(), 12);
}


bool CPUM::hasPGE() {
    return getBit(getFeatures2(), 13);
}


bool CPUM::hasMCA() {
    return getBit(getFeatures2(), 14);
}


bool CPUM::hasCMOV() {
    return getBit(getFeatures2(), 15);
}


bool CPUM::hasPAT() {
    return getBit(getFeatures2(), 16);
}


bool CPUM::hasPSE36() {
    return getBit(getFeatures2(), 17);
}


bool CPUM::hasCLFSH() {
    return getBit(getFeatures2(), 19);
}


bool CPUM::hasMMX() {
    return getBit(getFeatures2(), 23);
}


bool CPUM::hasFXSR() {
    return getBit(getFeatures2(), 24);
}


bool CPUM::hasSSE() {
    return getBit(getFeatures2(), 25);
}


bool CPUM::hasSSE2() {
    return getBit(getFeatures2(), 26);
}


// TODO: This works a bit differently - check documentation
bool CPUM::hasHTT() {
    return getBit(getFeatures2(), 28);
}
