#pragma once

#include <cassert>
#include <cstdint>

namespace gb::cpu::operand_types {

enum class Reg8 { B, C, D, E, H, L, MemHL, A };
enum class Reg16 { BC, DE, HL, SP };
enum class Reg16Stack { BC, DE, HL, AF };
enum class Reg16Mem { BC, DE, HighHL, LowHL };

enum class Cond { NZ, Z, NC, C };

// Invariant: Value has to be [0...7]
struct Index3Bit {
private:
  uint8_t val;

public:
  explicit constexpr Index3Bit(uint8_t x) : val{x} { assert(x < 8); }
  constexpr uint8_t value() const noexcept { return val; }
};

// Invariant: Value has to be [0...7]
struct Target3Bit {
private:
  uint8_t val;

public:
  explicit constexpr Target3Bit(uint8_t x) : val{x} { assert(x < 8); }
  constexpr uint8_t value() const noexcept { return val; }
};

struct Imm8Bit {
private:
  uint8_t val;

public:
  explicit constexpr Imm8Bit(uint8_t x) : val{x} {}
  constexpr uint8_t value() const noexcept { return val; }
};

struct Imm16Bit {
private:
  uint16_t val;

public:
  explicit constexpr Imm16Bit(uint16_t x) : val{x} {}
  constexpr uint16_t value() const noexcept { return val; }
};

} // namespace gb::cpu::operand_types
