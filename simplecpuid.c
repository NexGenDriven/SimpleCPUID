#include <windows.h>

// Test EFLAGS bit 21 (ID flag) to prevent invalid instruction crashes on pre-pentium machines
int HasCPUID(void) {
    int supported = 0;
    __asm__ __volatile__ (
        "pushfl\n\t"
        "pop %%eax\n\t"
        "mov %%eax, %%ecx\n\t"
        "xor $0x00200000, %%eax\n\t"
        "push %%eax\n\t"
        "popfl\n\t"
        "pushfl\n\t"
        "pop %%eax\n\t"
        "xor %%ecx, %%eax\n\t"
        "and $0x00200000, %%eax\n\t"
        "mov %%eax, %0\n\t"
        "push %%ecx\n\t"
        "popfl\n\t"
        : "=r" (supported)
        :
        : "eax", "ecx", "cc"
    );
    return supported != 0;
}

// Check for Nx586
int IsNx586(void) {
    int is_nexgen = 0;
    __asm__ __volatile__ (
        "movl $0x5555, %%eax\n\t"
        "xorl %%edx, %%edx\n\t"
        "movl $2, %%ecx\n\t"
        "clc\n\t"
        "divl %%ecx\n\t"
        "jnz 1f\n\t"             // If ZF is set (jz), it's an Nx586
        "movl $1, %0\n\t"
        "1:\n\t"
        : "=r" (is_nexgen)
        :
        : "eax", "edx", "ecx", "cc"
    );
    return is_nexgen;
}

void GetCPUIDVendor(char *vendor) {
    unsigned int ebx, ecx, edx;
    __asm__ __volatile__ (
        "cpuid"
        : "=b" (ebx), "=c" (ecx), "=d" (edx)
        : "a" (0)
    );
    ((unsigned int*)vendor)[0] = ebx;
    ((unsigned int*)vendor)[1] = edx;
    ((unsigned int*)vendor)[2] = ecx;
    vendor[12] = '\0';
}

void RawWinMain(void) {
    char message[128];

    if (!HasCPUID()) {
        if (IsNx586()) {
            lstrcpyA(message, "NexGenDriven");
        } else {
            lstrcpyA(message, "CPU does not support CPUID (386/old 486 machine)");
        }
    } else {
        GetCPUIDVendor(message);
    }

    MessageBoxA(NULL, message, "SimpleCPUID", MB_OK | MB_ICONINFORMATION);
    ExitProcess(0);
}