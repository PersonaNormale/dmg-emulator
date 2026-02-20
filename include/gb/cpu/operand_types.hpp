#pragma once

#include <cassert>
#include <cstdint>
#include <cstdlib>
#include <source_location>
#include <stdexcept>
#include <type_traits>

namespace gb::cpu::operand_types {

namespace detail {

struct contract_error final : std::logic_error {
  std::source_location loc;
  explicit contract_error(std::source_location l)
      : std::logic_error("Contract Violation"), loc(l) {}
};

[[noreturn]] inline void
contract_violation(std::source_location loc = std::source_location::current()) {
  throw contract_error{loc};
}

constexpr void
require(bool cond, std::source_location loc = std::source_location::current()) {
  if (!cond)
    contract_violation(loc);
}

template <std::uint8_t MaxExclusive>
[[nodiscard]] constexpr std::uint8_t
checked_u8(std::uint8_t v,
           std::source_location loc = std::source_location::current()) {
  require(v < MaxExclusive, loc);
  return v;
}

} // namespace detail

// Converts a uint8-backed enum class to its underlying value.
template <class E>
  requires(std::is_enum_v<E> &&
           std::is_same_v<std::underlying_type_t<E>, std::uint8_t>)
[[nodiscard]] constexpr auto to_u8(E e) noexcept -> std::uint8_t {
  return static_cast<std::uint8_t>(e);
}

// Opcode field r8: b,c,d,e,h,l,[hl],a (values are fixed by the ISA encoding).
enum class Reg8 : std::uint8_t {
  B = 0,
  C = 1,
  D = 2,
  E = 3,
  H = 4,
  L = 5,
  MemHL = 6,
  A = 7,
};

// Opcode field r16: bc,de,hl,sp.
enum class Reg16 : std::uint8_t {
  BC = 0,
  DE = 1,
  HL = 2,
  SP = 3,
};

// Opcode field r16stk: bc,de,hl,af.
enum class Reg16Stack : std::uint8_t {
  BC = 0,
  DE = 1,
  HL = 2,
  AF = 3,
};

// Opcode field r16mem: (bc),(de),(hl+),(hl-).
enum class Reg16Mem : std::uint8_t {
  BC = 0,
  DE = 1,
  HLInc = 2,
  HLDec = 3,
};

// Opcode field cond: nz,z,nc,c.
enum class Cond : std::uint8_t {
  NZ = 0,
  Z = 1,
  NC = 2,
  C = 3,
};

// 3-bit field extracted from the opcode.
// Tagging prevents mixing semantically different 3-bit fields.
template <class Tag> class U3 {
public:
  explicit constexpr U3(std::uint8_t x) : v_{detail::checked_u8<8>(x)} {}

  [[nodiscard]] constexpr auto value() const noexcept -> std::uint8_t {
    return v_;
  }

  friend constexpr auto operator==(U3, U3) noexcept -> bool = default;

private:
  std::uint8_t v_{0};
};

struct BitIndex3Tag {}; // b3
struct Target3Tag {};   // tgt3 (RST target / 8)

using Index3Bit = U3<BitIndex3Tag>;
using Target3Bit = U3<Target3Tag>;

// Strong immediate types.
class Imm8 {
public:
  explicit constexpr Imm8(std::uint8_t x) noexcept : v_{x} {}

  [[nodiscard]] constexpr auto value() const noexcept -> std::uint8_t {
    return v_;
  }

  friend constexpr auto operator==(Imm8, Imm8) noexcept -> bool = default;

private:
  std::uint8_t v_{0};
};

class Imm16 {
public:
  explicit constexpr Imm16(std::uint16_t x) noexcept : v_{x} {}

  [[nodiscard]] constexpr auto value() const noexcept -> std::uint16_t {
    return v_;
  }

  friend constexpr auto operator==(Imm16, Imm16) noexcept -> bool = default;

private:
  std::uint16_t v_{0};
};

// Signed immediate e8 used by JR and ADD SP,e8.
// Stores the raw byte and exposes a well-defined sign-extended value.
class Imm8Signed {
public:
  explicit constexpr Imm8Signed(std::uint8_t raw) noexcept : raw_{raw} {}

  // Building from an already-signed value.
  explicit constexpr Imm8Signed(std::int8_t x) noexcept
      : raw_{static_cast<std::uint8_t>(x)} {}

  [[nodiscard]] constexpr auto raw() const noexcept -> std::uint8_t {
    return raw_;
  }

  [[nodiscard]] constexpr auto value_i16() const noexcept -> std::int16_t {
    return (raw_ < 0x80) ? static_cast<std::int16_t>(raw_)
                         : static_cast<std::int16_t>(raw_) - 0x100;
  }

  [[nodiscard]] constexpr auto value() const noexcept -> std::int8_t {
    return static_cast<std::int8_t>(value_i16()); // guaranteed in [-128, 127]
  }

  friend constexpr auto operator==(Imm8Signed, Imm8Signed) noexcept
      -> bool = default;

private:
  std::uint8_t raw_{0};
};

} // namespace gb::cpu::operand_types
