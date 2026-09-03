# SimpleCPUID

A simple C application to check the CPUID brand name from Windows 95 to 11

Compile with this command:
i686-w64-mingw32-gcc -o simplecpuid2.exe simplecpuid_nx586.c -march=i486 -nostdlib -e _RawWinMain -lkernel32 -luser32 -mwindows
