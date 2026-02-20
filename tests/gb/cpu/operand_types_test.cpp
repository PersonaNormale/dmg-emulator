#include <catch2/catch_test_macros.hpp>

#include <gb/cpu/operand_types.hpp>

#include <cstdint>
#include <type_traits>

namespace ot = gb::cpu::operand_types;

namespace {

// Compile-time: type-safety and constructor explicit
static_assert(!std::is_convertible_v<ot::Index3Bit, ot::Target3Bit>);
static_assert(!std::equality_comparable_with<ot::Index3Bit, ot::Target3Bit>);

static_assert(!std::is_convertible_v<std::uint8_t, ot::Imm8>);
static_assert(!std::is_convertible_v<std::uint16_t, ot::Imm16>);
static_assert(!std::is_convertible_v<std::uint8_t, ot::Imm8Signed>);

// Compile-time: enum encoding mapping (now fixed by explicit enumerator values)
static_assert(ot::to_u8(ot::Reg8::B) == 0);
static_assert(ot::to_u8(ot::Reg8::C) == 1);
static_assert(ot::to_u8(ot::Reg8::D) == 2);
static_assert(ot::to_u8(ot::Reg8::E) == 3);
static_assert(ot::to_u8(ot::Reg8::H) == 4);
static_assert(ot::to_u8(ot::Reg8::L) == 5);
static_assert(ot::to_u8(ot::Reg8::MemHL) == 6);
static_assert(ot::to_u8(ot::Reg8::A) == 7);

static_assert(ot::to_u8(ot::Reg16::BC) == 0);
static_assert(ot::to_u8(ot::Reg16::DE) == 1);
static_assert(ot::to_u8(ot::Reg16::HL) == 2);
static_assert(ot::to_u8(ot::Reg16::SP) == 3);

static_assert(ot::to_u8(ot::Reg16Stack::BC) == 0);
static_assert(ot::to_u8(ot::Reg16Stack::DE) == 1);
static_assert(ot::to_u8(ot::Reg16Stack::HL) == 2);
static_assert(ot::to_u8(ot::Reg16Stack::AF) == 3);

static_assert(ot::to_u8(ot::Reg16Mem::BC) == 0);
static_assert(ot::to_u8(ot::Reg16Mem::DE) == 1);
static_assert(ot::to_u8(ot::Reg16Mem::HLInc) == 2);
static_assert(ot::to_u8(ot::Reg16Mem::HLDec) == 3);

static_assert(ot::to_u8(ot::Cond::NZ) == 0);
static_assert(ot::to_u8(ot::Cond::Z) == 1);
static_assert(ot::to_u8(ot::Cond::NC) == 2);
static_assert(ot::to_u8(ot::Cond::C) == 3);

} // namespace

TEST_CASE("Operand types: U3 range and tagging") {
  SECTION("Index3Bit accepts [0..7]") {
    for (std::uint8_t x = 0; x < 8; ++x) {
      REQUIRE_NOTHROW(ot::Index3Bit{x});
      const ot::Index3Bit b{x};
      REQUIRE(b.value() == x);
    }
  }

  SECTION("Target3Bit accepts [0..7]") {
    for (std::uint8_t x = 0; x < 8; ++x) {
      REQUIRE_NOTHROW(ot::Target3Bit{x});
      const ot::Target3Bit t{x};
      REQUIRE(t.value() == x);
    }
  }

  SECTION("Index3Bit rejects >= 8 (contract throws)") {
    REQUIRE_THROWS_AS(ot::Index3Bit{8}, ot::detail::contract_error);
    REQUIRE_THROWS_AS(ot::Index3Bit{255}, ot::detail::contract_error);
  }

  SECTION("Target3Bit rejects >= 8 (contract throws)") {
    REQUIRE_THROWS_AS(ot::Target3Bit{8}, ot::detail::contract_error);
  }
}

TEST_CASE("Operand types: immediates") {
  SECTION("Imm8 stores exact value") {
    REQUIRE(ot::Imm8{0x00}.value() == 0x00);
    REQUIRE(ot::Imm8{0xFF}.value() == 0xFF);
  }

  SECTION("Imm16 stores exact value") {
    REQUIRE(ot::Imm16{0x1234}.value() == 0x1234);
    REQUIRE(ot::Imm16{0x0000}.value() == 0x0000);
    REQUIRE(ot::Imm16{0xFFFF}.value() == 0xFFFF);
  }

  SECTION("Imm8Signed raw/value/value_i16 sign extension") {
    {
      const ot::Imm8Signed s{std::uint8_t{0x00}};
      REQUIRE(s.raw() == 0x00);
      REQUIRE(s.value() == 0);
      REQUIRE(s.value_i16() == 0);
    }
    {
      const ot::Imm8Signed s{std::uint8_t{0x7F}};
      REQUIRE(s.raw() == 0x7F);
      REQUIRE(s.value() == 127);
      REQUIRE(s.value_i16() == 127);
    }
    {
      const ot::Imm8Signed s{std::uint8_t{0x80}}; // -128
      REQUIRE(s.raw() == 0x80);
      REQUIRE(s.value() == static_cast<std::int8_t>(-128));
      REQUIRE(s.value_i16() == -128);
    }
    {
      const ot::Imm8Signed s{std::uint8_t{0xFF}}; // -1
      REQUIRE(s.raw() == 0xFF);
      REQUIRE(s.value() == static_cast<std::int8_t>(-1));
      REQUIRE(s.value_i16() == -1);
    }
  }

  SECTION("Imm8Signed from int8_t preserves raw") {
    const ot::Imm8Signed s{static_cast<std::int8_t>(-1)};
    REQUIRE(s.raw() == 0xFF);
    REQUIRE(s.value() == static_cast<std::int8_t>(-1));
  }
}

TEST_CASE("Operand types: RST target computation") {
  SECTION("tgt3 -> address = tgt3 * 8") {
    for (std::uint8_t t = 0; t < 8; ++t) {
      const ot::Target3Bit tgt{t};
      const std::uint16_t addr = static_cast<std::uint16_t>(tgt.value()) * 8u;
      REQUIRE(addr == (static_cast<std::uint16_t>(t) << 3));
    }

    REQUIRE(static_cast<std::uint16_t>(ot::Target3Bit{0}.value()) * 8u == 0x00);
    REQUIRE(static_cast<std::uint16_t>(ot::Target3Bit{1}.value()) * 8u == 0x08);
    REQUIRE(static_cast<std::uint16_t>(ot::Target3Bit{7}.value()) * 8u == 0x38);
  }
}

TEST_CASE("Operand types: internal contract helpers") {
  SECTION("detail::require(false) throws contract_error") {
    REQUIRE_THROWS_AS(ot::detail::require(false), ot::detail::contract_error);
  }

  SECTION("detail::checked_u8 enforces MaxExclusive") {
    REQUIRE(ot::detail::checked_u8<8>(0) == 0);
    REQUIRE(ot::detail::checked_u8<8>(7) == 7);

    REQUIRE_THROWS_AS(ot::detail::checked_u8<8>(8), ot::detail::contract_error);
    REQUIRE_THROWS_AS(ot::detail::checked_u8<8>(255), ot::detail::contract_error);
  }
}
