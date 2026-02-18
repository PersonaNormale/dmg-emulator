#pragma once
#include <cstdint>
#include <gb/cpu/operand_types.hpp>

namespace gb::cpu {

// Flag here because it isn't an operand type but an implementation
// For conditions
enum class Flag : uint8_t { Z = 7, N = 6, H = 5, C = 4 };

using operand_types::Reg8;
using operand_types::Reg16;

class Registers final {
public:
  void reset() noexcept;

  [[nodiscard]] uint8_t read8(Reg8 reg) const noexcept;
  void write8(Reg8 reg, uint8_t value) noexcept;

  [[nodiscard]] uint16_t read16(Reg16 r) const noexcept;
  void write16(Reg16 reg, uint16_t value) noexcept;

  [[nodiscard]] bool flag(Flag flag) const noexcept;
  void set_flag(Flag flag, bool value) noexcept;

private:
  uint8_t a_{}, f_{}, b_{}, c_{}, d_{}, e_{}, h_{}, l_{};
  uint16_t sp_{}, pc_{};

  // Set 0 the lower nibble of the F register
  void sanitize_f() noexcept;
};

} // namespace gb::cpu
