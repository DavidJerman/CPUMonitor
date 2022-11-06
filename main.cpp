#include <iostream>


int main()
{
    int data[3] {0};

    // Processor Vendor
    __asm__("mov $0x0, %eax\n\t");
    __asm__("cpuid\n\t");
    __asm__("mov %%ebx, %0\n\t":"=r" (data[0]));
    __asm__("mov %%edx, %0\n\t":"=r" (data[1]));
    __asm__("mov %%ecx, %0\n\t":"=r" (data[2]));
    char str[16] {0};
    for (int i = 0; i < 3; i++) {
        auto c = data[i];
        for (int j = 0; j < 4; j++) {
            char letter = (char)c;
            c >>= 8;
            str[i * 4 + j] = letter;
        }
    }
    std::cout << str << std::endl;

    // Family, Model, Stepping
    __asm__("mov $0x1, %eax\n\t");
    __asm__("cpuid\n\t");
    __asm__("mov %%eax, %0\n\t":"=r" (data[0]));
    __asm__("mov %%ebx, %0\n\t":"=r" (data[1]));
    __asm__("mov %%ecx, %0\n\t":"=r" (data[2]));
    __asm__("mov %%edx, %0\n\t":"=r" (data[3]));

    int stepping {0},
        baseModel {0},
        baseFamily{0},
        reserved1 {0},
        extModel {0},
        extFamily {0},
        reserved2 {0};

    stepping = data[0] & 15;
    baseModel = (data[0] >>= 4) & 15;
    baseFamily = (data[0] >>= 4) & 15;
    reserved1 = (data[0] >>= 4) & 15;
    extModel = (data[0] >>= 4) & 15;
    extFamily = (data[0] >>= 4) & 255;
    reserved2 = (data[0] >>= 8) & 15;

    std::cout << "Stepping: " << stepping << std::endl;
    std::cout << "Base Model: " << baseModel << std::endl;
    std::cout << "Base Family: " << baseFamily << std::endl;
    std::cout << "Reserved 1: " << reserved1 << std::endl;
    std::cout << "Extended Model: " << extModel << std::endl;
    std::cout << "Extended Family: " << extFamily << std::endl;
    std::cout << "Reserved 2: " << reserved2 << std::endl;

    //  AMD
    //  05	05h	K6
    //  06	06h	K7
    //  15	0Fh	K8 / Hammer
    //  16	10h	K10
    //  17	11h	K8 & K10 "hybrid"
    //  18	12h	K10 (Llano) / K12 (ARM-based)
    //  20	14h	Bobcat
    //  21	15h	Bulldozer / Piledriver / Steamroller / Excavator
    //  22	16h	Jaguar / Puma
    //  23	17h	Zen / Zen+ / Zen 2
    //  24	18h	Hygon Dhyana
    //  25	19h	Zen 3 / Zen 3+ / Zen 4

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
    
    return 0;
}
