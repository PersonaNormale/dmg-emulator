#include <cstdint>
#include <gb/cpu/registers.hpp>
#include <sys/types.h>

namespace gb::cpu {

namespace {
constexpr uint8_t FMask = 0xF0;
}

void Registers::sanitize_f() noexcept { f_ &= FMask; }

void Registers::reset() noexcept {
  a_ = f_ = b_ = c_ = d_ = e_ = h_ = l_ = 0;
  sp_ = pc_ = 0;
  sanitize_f();
};

uint8_t Registers::read8(Reg8 reg) const noexcept {
  switch (reg) {
  case Reg8::A:
    return a_;
  case Reg8::B:
    return b_;
  case Reg8::C:
    return c_;
  case Reg8::D:
    return d_;
  case Reg8::E:
    return e_;
  case Reg8::H:
    return h_;
  case Reg8::L:
    return l_;
  }

  return 0;
}

void Registers::write8(Reg8 reg, uint8_t value) noexcept {

  switch (reg) {
  case Reg8::A:
    a_ = value;
    break;
  case Reg8::B:
    b_ = value;
    break;
  case Reg8::C:
    c_ = value;
    break;
  case Reg8::D:
    d_ = value;
    break;
  case Reg8::E:
    e_ = value;
    break;
  case Reg8::H:
    h_ = value;
    break;
  case Reg8::L:
    l_ = value;
    break;
  }
}

uint16_t Registers::read16(Reg16 reg) const noexcept {
  switch (reg) {
  case Reg16::AF:
    return (uint16_t(a_) << 8) | f_;
  case Reg16::BC:
    return (uint16_t(b_) << 8) | c_;
  case Reg16::DE:
    return (uint16_t(d_) << 8) | e_;
  case Reg16::HL:
    return (uint16_t(h_) << 8) | l_;
  case Reg16::SP:
    return sp_;
  case Reg16::PC:
    return pc_;
  }

  return 0;
}

void Registers::write16(Reg16 reg, uint16_t value) noexcept {

  // hi goes to the upper bits of the 16 bit register
  // lo goes to the lower bits of the 16 bit register
  const auto hi = uint8_t(value >> 8);
  const auto lo = uint8_t(value);

  switch (reg) {
  case Reg16::AF:
    a_ = hi;
    f_ = lo;
    // need to sanitize to maintain lower bits of F zero as invariance
    sanitize_f();
    break;
  case Reg16::BC:
    b_ = hi;
    c_ = lo;
    break;
  case Reg16::DE:
    d_ = hi;
    e_ = lo;
    break;
  case Reg16::HL:
    h_ = hi;
    l_ = lo;
    break;
  case Reg16::SP:
    sp_ = value;
    break;
  case Reg16::PC:
    pc_ = value;
    break;
  }
}

bool Registers::flag(Flag flag) const noexcept {
  // flag is an enum
  // Z = 7, N = 6, H = 5, C = 4
  // so shifting by that positions tells where in the 8 bit register the
  // bit flag will be to be checked
  return bool(f_ & (uint8_t(1) << uint8_t(flag)));
}

void Registers::set_flag(Flag flag, bool value) noexcept {
  // flag is an enum
  // Z = 7, N = 6, H = 5, C = 4
  // so shifting by that positions tells where in the 8 bit register the
  // bit flag will be to be checked
  // Example -> to set Z flag
  // 0000 0001 shifted by 7 position -> // 1000 0000 
  const auto mask = uint8_t(1) << uint8_t(flag);

  f_ = value ? uint8_t(f_ | mask) : uint8_t(f_ & ~mask);
  
  sanitize_f();
}

} // namespace gb::cpu
