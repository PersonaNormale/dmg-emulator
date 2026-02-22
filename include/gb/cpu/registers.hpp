#pragma once
#include <cstdint>

namespace gb::cpu {

enum class Flag { Z = 7, N = 6, H = 5, C = 4 };
enum class Reg8 { A, B, C, D, E, H, L };
enum class Reg16 { AF, BC, DE, HL, SP, PC };

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

  // Keep the lower nibble of the register F at 0
  void sanitize_f() noexcept;
};

} // namespace gb::cpu
