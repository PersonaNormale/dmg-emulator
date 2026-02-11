#pragma once
#include <concepts>
#include <cstdint>

namespace gb::cpu {

enum class Reg8 : uint8_t { A, F, B, C, D, E, H, L };
enum class Reg16 : uint8_t { AF, BC, DE, HL, SP, PC };
enum class Flag : uint8_t { Z, N, H, C };

template <class RF>
concept RegisterFile = requires(RF &rf, RF const &crf, Reg8 r8, Reg16 r16,
                                Flag fl, uint8_t v8, uint16_t v16, bool b) {
  { crf.read8(r8) } -> std::same_as<uint8_t>;
  { rf.write8(r8, v8) } -> std::same_as<void>;
  { crf.read16(r16) } -> std::same_as<uint16_t>;
  { rf.write16(r16, v16) } -> std::same_as<void>;
  { crf.flag(fl) } -> std::same_as<bool>;
  { rf.set_flag(fl, b) } -> std::same_as<void>;
};

} // namespace gb::cpu
