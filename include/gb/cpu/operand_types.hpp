#pragma once

#include <cstdint>
namespace gb::cpu::operand_types {

enum class Reg8 { B, C, D, E, H, L, MemHL, A };
enum class Reg16 { BC, DE, HL, SP };
enum class Reg16Stack { BC, DE, HL, AF };
enum class Reg16Mem { BC, DE, HighHL, LowHL };

enum class Cond { NZ, Z, NC, C };

// Invariant: Value has to be [0...7]
struct Index3Bit {
  uint8_t value;
};

// Invariant: Value has to be [0...7]
struct Target3Bit {
  uint8_t value;
};

struct Imm8Bit {
  uint8_t value;
};

struct Imm16Bit {
  uint16_t value;
};

} // namespace gb::cpu::operand_types
