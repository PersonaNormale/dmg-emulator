#pragma once
#include <cstdint>

namespace gb::cpu::operand_types {

enum class Reg8 : uint8_t { A, B, C, D, E, H, L };
enum class Reg16 : uint8_t { AF, BC, DE, HL, SP, PC };

} // namespace gb::cpu::operand_types
