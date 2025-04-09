#pragma once

// Machine architecture's.
#define ELF_ARCH_X86 0x0003
#define ELF_ARCH_AARCH 0x0028
#define ELF_ARCH_X86_64 0x003E
#define ELF_ARCH_AARCH64 0x00B7
#define ELF_ARCH_RISCV 0x00F3

/// Architecture's memory bit-depth.
#define ELF_BIT_DEPTH_32 0x01
#define ELF_BIT_DEPTH_64 0x02

/// Architecture's endianness.
#define ELF_END_LE 0x01
#define ELF_END_BE 0x02