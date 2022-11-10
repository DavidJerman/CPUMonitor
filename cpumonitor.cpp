#include <cstring>

#include "cpumonitor.h"

//
// Obtaining frequency is a linux specific features for now
// Thus this needs to bo compiled with a gcc compiler
// MVSC won't work for now, as the inline assembly syntax
// differs there
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

    uint8 extendedFamily = (data >> 20) & 255;

    return extendedFamily;
}


// Obtaining accurate family name with model names --> TBD
// TODO: Get accurate model names
cString CPUM::getMicroarchitecture() {
    auto familyNumber = getFamilyNumber();

    auto vendorName = getVendorName();

    // This check probably works, but I would have to check
    if (std::strcmp(vendorName, AMDCpuVendor) == 0)
        return getMicroarchitectureAMD(familyNumber, getBaseModelNumber(), getExtendedModelNumber());
    else if (std::strcmp(vendorName, IntelCpuVendor) == 0)
        return getMicroarchitectureIntel(familyNumber, getBaseModelNumber(), getExtendedModelNumber());
    else
        return "Unknown CPU vendor";
}


// This is just for AMD for now
// Works with CPU generations up to Zen 4
// Data taken from https://en.wikipedia.org/wiki/List_of_AMD_CPU_microarchitectures
cString CPUM::getMicroarchitectureAMD(uint32 familyNumber, uint32 model, uint32 extendedModel) {
    cString microarchitecture;
    auto _model = (extendedModel << 4) + model;

    switch (familyNumber) {
        case 5:
            microarchitecture = "K6";
            break;
        case 6:
            microarchitecture = "K7";
            break;
        case 15:
            microarchitecture = "K8 / Hammer";
            break;
        case 16:
            microarchitecture = "K10";
            break;
        case 17:
            microarchitecture = "K8 & K10 \"hybrid\"";
            break;
        case 18:
            microarchitecture = "K10 (Llano) / K12";
            break;
        case 20:
            microarchitecture = "Bobcat";
            break;
        case 21: {
            switch (_model) {
                case 0x01:
                    microarchitecture = "Bulldozer";
                    break;
                case 0x02:
                case 0x10:
                case 0x13:
                    microarchitecture = "Piledriver";
                    break;
                case 0x30:
                case 0x38:
                    microarchitecture = "Steamroller";
                    break;
                case 0x60:
                case 0x65:
                case 0x70:
                    microarchitecture = "Excavator";
                    break;
                default:
                    microarchitecture = "Bulldozer / Piledriver / Steamroller / Excavator";
                    break;
            }
            break;
        }
        case 22:
            switch (_model)  {
                case 0x00:
                    microarchitecture = "Jaguar";
                    break;
                case 0x30:
                    microarchitecture = "Puma";
                    break;
                default:
                    microarchitecture = "(Extended) Jaguar / Puma";
                    break;
            }
            break;
        case 23:
            switch (_model) {
                case 0x01:
                case 0x11:
                case 0x20:
                    microarchitecture = "Zen";
                    break;
                case 0x08:
                    microarchitecture = "Zen+";
                    break;
                case 0x18:
                    microarchitecture = "Zen / Zen+";
                    break;
                case 0x31:
                case 0x47:
                case 0x60:
                case 0x68:
                case 0x71:
                case 0x90:
                    microarchitecture = "Zen 2";
                    break;
                default:
                    microarchitecture = "Zen / Zen+ / Zen 2";
                    break;
            }
            break;
        case 24:
            microarchitecture = "Hygon Dhyana";
            break;
        case 25:
            // Stepping number can be used to differentiate between engineering samples
            // Will save the data to a file to extend this functionality
            switch (_model) {
                case 0x00:
                case 0x01:
                case 0x08:
                case 0x21:
                case 0x50:
                    microarchitecture = "Zen 3";
                    break;
                case 0x10:
                    microarchitecture = "Zen 4";
                    break;
                default:
                    if (_model >= 0x70 && _model < 0x80)
                        microarchitecture = "Zen 4";
                    else
                        microarchitecture = "Zen 3 / Zen 4";
                    break;
            }
            break;
        default:
            microarchitecture = "Unknown Microarchitecture";
            break;
    }

    return microarchitecture;
}


// This requires modification
// https://en.wikichip.org/wiki/intel/cpuid#Family_4
cString CPUM::getMicroarchitectureIntel(uint32 familyNumber, uint32 model, uint32 extendedModel) {
    cString microarchitecture;
    auto _model = (extendedModel << 4) + model;

    switch (familyNumber) {
        case 3: {
            microarchitecture = "80836";
            break;
        }
        case 4: {
            microarchitecture = "80486";
            break;
        }
        case 5: {
            switch (_model) {
                case 0x1:
                case 0x2:
                case 0x4:
                case 0x7:
                case 0x8:
                    microarchitecture = "P5";
                    break;
                case 0x9:
                case 0xA:
                    microarchitecture = "Lakemont";
                    break;
                default:
                    microarchitecture = "Unknown Microarchitecture";
                    break;
            }
            break;
        }
        case 6: {
            switch (_model) {
                // Small cores
                case 28:
                case 38:
                    microarchitecture = "Bonnell";
                    break;
                case 39:
                case 53:
                case 54:
                    microarchitecture = "Saltwell";
                    break;
                case 55:
                case 74:
                case 77:
                case 90:
                case 93:
                    microarchitecture = "Silvermont";
                    break;
                case 76:
                    microarchitecture = "Airmont";
                    break;
                case 92:
                case 95:
                    microarchitecture = "Goldmont";
                    break;
                case 122:
                    microarchitecture = "Goldmont Plus";
                    break;
                case 138:
                case 150:
                case 156:
                    microarchitecture = "Tremont";
                    break;
                // Big cores (server)
                case 10:
                    microarchitecture = "P6";
                    break;
                case 23:
                    microarchitecture = "Penryn (Server / Client)";
                    break;
                case 29:
                    microarchitecture = "Penryn (Server)";
                    break;
                case 30:
                    microarchitecture = "Nehalem (Server / Client)";
                    break;
                case 26:
                case 46:
                    microarchitecture = "Nehalem (Server)";
                    break;
                case 44:
                case 47:
                    microarchitecture = "Westmere (Server)";
                    break;
                case 45:
                    microarchitecture = "Sandy Bridge (Server)";
                    break;
                case 62:
                    microarchitecture = "Ivy Bridge (Server)";
                    break;
                case 63:
                    microarchitecture = "Haswell (Server)";
                    break;
                case 79:
                case 86:
                    microarchitecture = "Broadwell (Server)";
                    break;
                case 85:
                    microarchitecture = "Cooper Lake / Cascade Lake / Skylake (Server)";
                    break;
                case 106:
                case 108:
                    microarchitecture = "Ice Lake (Server)";
                    break;
                case 143:
                    microarchitecture = "Sapphire Rapids (Server)";
                    break;
                // Big Cores (Client)
                case 1:
                case 3:
                case 5:
                case 6:
                case 7:
                case 8:
                case 11:
                    microarchitecture = "P6";
                    break;
                case 9:
                case 13:
                case 21:
                    microarchitecture = "Pentium M";
                    break;
                case 14:
                    microarchitecture = "Modified Pentium M";
                    break;
                case 15:
                case 22:
                    microarchitecture = "Core (Client)";
                    break;
                case 31:
                    microarchitecture = "Nehalem (Client)";
                    break;
                case 37:
                    microarchitecture = "Westmere (Client)";
                    break;
                case 42:
                    microarchitecture = "Sandy Bridge (Client)";
                    break;
                case 58:
                    microarchitecture = "Ivy Bridge (Client)";
                    break;
                case 60:
                case 69:
                case 70:
                    microarchitecture = "Haswell (Client)";
                    break;
                case 61:
                case 71:
                    microarchitecture = "Broadwell (Client)";
                    break;
                case 78:
                case 94:
                    microarchitecture = "Skylake (Client)";
                    break;
                case 142:
                    microarchitecture = "Kaby Lake / Coffee Lake / Whiskey Lake / Amber Lake / Comet Lake";
                    break;
                case 158:
                    microarchitecture = "Kaby Lake / Coffee Lake";
                    break;
                case 102:
                    microarchitecture = "Cannon Lake";
                    break;
                case 165:
                    microarchitecture = "Comet Lake";
                    break;
                case 125:
                case 126:
                    microarchitecture = "Ice Lake (Client)";
                    break;
                case 140:
                case 141:
                    microarchitecture = "Tiger Lake";
                    break;
                case 167:
                    microarchitecture = "Rocket Lake";
                    break;
                case 154:
                case 151:
                    microarchitecture = "Alder Lake";
                    break;
                case 183:
                    microarchitecture = "Raptor Lake";
                    break;
                default:
                    microarchitecture = "Unknown Microarchitecture";
                    break;
            }
            break;
        }
        default:
            microarchitecture = "Unknown Microarchitecture";
            break;
    }

    return microarchitecture;
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


// Monitoring CPU performance
// Linux only - this implementation uses the linux system files
// to determine the CPU frequency
uint32 CPUM::avgCoreFrequency() {
    auto fileName = cpuinfoFile;
    std::ifstream ifStream;

    ifStream.open(fileName);

    if (!ifStream.good()) {
        return 0;
    }

    char * line {nullptr};
    while (!ifStream.eof()) {

    }

    return 0.f;
}


float CPUM::avgCoreFrequencyGHz() {
    return (float) avgCoreFrequency() / 1000.0f;
}
