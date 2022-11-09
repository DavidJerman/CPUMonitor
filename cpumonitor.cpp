#include <cstring>

#include "cpumonitor.h"

//
// THIS LIBRARY IS AMD SPECIFIC FOR NOW
// https://www.amd.com/system/files/TechDocs/25481.pdf
//


cString CPUM::getBrandName()
{
    auto name = new char[48];
    uint32 data[4];

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
    saveDataEABCDXToCStringBuffer(data, &name[0], &name[16]);

    __asm__("mov $0x80000003, %eax\n\t");
    __asm__("cpuid\n\t");
    __asm__("mov %%eax, %0\n\t":"=r" (data[0]));
    __asm__("mov %%ebx, %0\n\t":"=r" (data[1]));
    __asm__("mov %%ecx, %0\n\t":"=r" (data[2]));
    __asm__("mov %%edx, %0\n\t":"=r" (data[3]));
    saveDataEABCDXToCStringBuffer(data, &name[16], &name[32]);

    __asm__("mov $0x80000004, %eax\n\t");
    __asm__("cpuid\n\t");
    __asm__("mov %%eax, %0\n\t":"=r" (data[0]));
    __asm__("mov %%ebx, %0\n\t":"=r" (data[1]));
    __asm__("mov %%ecx, %0\n\t":"=r" (data[2]));
    __asm__("mov %%edx, %0\n\t":"=r" (data[3]));
    saveDataEABCDXToCStringBuffer(data, &name[32], &name[48]);

    return (cString)name;
}


// Return nullptr if the char buffer is too small
void CPUM::saveDataEABCDXToCStringBuffer(uint32 data[4], char * beg, const char * const end)
{
    if (end - beg < 16)
    {
        *beg = 0;
        return;
    }
    uint32 data_copy[4];
    std::memcpy(data_copy, data, 4);
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            auto letter = (char)(data[i] & 255);
            data[i] >>= 8;
            *beg = (char)letter;
            beg++;
        }
    }
}


cString CPUM::getVendorName()
{
    uint32 data[4];
    auto cStr = new char[13];

    __asm__("mov $0x0, %rax\n\t");
    __asm__("cpuid\n\t");
    __asm__("mov %%ebx, %0\n\t":"=r" (data[0]));
    __asm__("mov %%edx, %0\n\t":"=r" (data[1]));
    __asm__("mov %%ecx, %0\n\t":"=r" (data[2]));
    for (uint8 i = 0; i < 3; i++) {
        for (uint8 j = 0; j < 4; j++) {
            auto letter = (char)(data[i] & 255);
            data[i] >>= 8;
            cStr[i * 4 + j] = (char)letter;
        }
    }

    cStr[12] = 0;

    return (cString)cStr;
}


uint32 CPUM::getFamilyNumber()
{
    uint32 data[1];

    __asm__("mov $0x1, %eax\n\t");
    __asm__("cpuid\n\t");
    __asm__("mov %%eax, %0\n\t":"=r" (data[0]));

    uint8 baseFamily {0};
    uint8 extendedFamily {0};

    baseFamily = (data[0] >> 8) & 15;
    if (baseFamily < 15)
    {
        return baseFamily;
    }
    extendedFamily = (data[0] >> 20) & 15;

    return baseFamily + extendedFamily;
}


uint32 CPUM::getFamilyBaseNumber()
{
    uint32 data[1];
    auto &date = data[0];

    __asm__("mov $0x1, %eax\n\t");
    __asm__("cpuid\n\t");
    __asm__("mov %%eax, %0\n\t":"=r" (date));

    uint8 baseFamily = (date >> 8) & 15;

    return baseFamily;
}


uint32 CPUM::getFamilyExtendedNumber()
{
    uint32 data[1];
    auto &date = data[0];

    __asm__("mov $0x1, %eax\n\t");
    __asm__("cpuid\n\t");
    __asm__("mov %%eax, %0\n\t":"=r" (date));

    uint8 extendedFamily = (date >> 20) & 15;

    return extendedFamily;
}


// Obtaining accurate family name with model names --> TBD
// TODO: Get accurate model names
cString CPUM::getFamilyName()
{
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
cString CPUM::familyNumberToNameAMD(uint32 familyNumber)
{
    cString familyName;

    switch (familyNumber)
    {
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


uint32 CPUM::getModelNumber()
{
    uint32 data[1];

    __asm__("mov $0x1, %eax\n\t");
    __asm__("cpuid\n\t");
    __asm__("mov %%eax, %0\n\t":"=r" (data[0]));

    uint8 baseModel {0};
    uint8 extendedModel {0};
    auto baseFamily = getFamilyNumber();

    baseModel = (data[0] >> 4) & 15;
    if (baseFamily < 15)
    {
        return baseModel;
    }
    extendedModel = (data[0] >> 16) & 15;
    auto model = (extendedModel << 4) + baseModel;

    return model;
}


uint32 CPUM::getBaseModelNumber()
{
    uint32 data[1];

    __asm__("mov $0x1, %eax\n\t");
    __asm__("cpuid\n\t");
    __asm__("mov %%eax, %0\n\t":"=r" (data[0]));

    uint8 baseModel = (data[0] >> 4) & 15;

    return baseModel;
}


uint32 CPUM::getExtendedModelNumber()
{
    uint32 data[1];

    __asm__("mov $0x1, %eax\n\t");
    __asm__("cpuid\n\t");
    __asm__("mov %%eax, %0\n\t":"=r" (data[0]));

    uint8 extendedModel = (data[0] >> 16) & 15;

    return extendedModel;
}


uint32 CPUM::getSteppingNumber()
{
    uint32 data[1];

    __asm__("mov $0x1, %eax\n\t");
    __asm__("cpuid\n\t");
    __asm__("mov %%eax, %0\n\t":"=r" (data[0]));

    uint8 steppingNumber = data[0] & 15;

    return steppingNumber;
}

