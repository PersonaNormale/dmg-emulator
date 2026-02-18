#include <catch2/catch_test_macros.hpp>
#include <gb/cpu/registers.hpp>
#include <gb/cpu/operand_types.hpp>

using namespace gb::cpu;

TEST_CASE("CPU Registers: Mapping and Flag") {
  Registers r{};

  SECTION("AF: lower nibble of F at zero") {
    r.write16(Reg16::AF, 0x12FF);
    REQUIRE(r.read8(Reg8::A) == 0x12);
    REQUIRE(r.read16(Reg16::AF) == 0x12F0);
  }

  SECTION("BC: split/join") {
    r.write16(Reg16::BC, 0xBEEF);
    REQUIRE(r.read8(Reg8::B) == 0xBE);
    REQUIRE(r.read8(Reg8::C) == 0xEF);
    REQUIRE(r.read16(Reg16::BC) == 0xBEEF);
  }

  SECTION("BC: write8 updates read16(BC)") {
    r.write8(Reg8::B, 0xFE);
    r.write8(Reg8::C, 0xDC);
    REQUIRE(r.read16(Reg16::BC) == 0xFEDC);
  }

  SECTION("DE: split/join") {
    r.write16(Reg16::DE, 0xBEEF);
    REQUIRE(r.read8(Reg8::D) == 0xBE);
    REQUIRE(r.read8(Reg8::E) == 0xEF);
    REQUIRE(r.read16(Reg16::DE) == 0xBEEF);
  }

  SECTION("DE: write8 updates read16(BC)") {
    r.write8(Reg8::D, 0xFE);
    r.write8(Reg8::E, 0xDC);
    REQUIRE(r.read16(Reg16::DE) == 0xFEDC);
  }

  SECTION("HL: split/join") {
    r.write16(Reg16::HL, 0xBEEF);
    REQUIRE(r.read8(Reg8::H) == 0xBE);
    REQUIRE(r.read8(Reg8::L) == 0xEF);
    REQUIRE(r.read16(Reg16::HL) == 0xBEEF);
  }

  SECTION("HL: write8 updates read16(BC)") {
    r.write8(Reg8::H, 0xFE);
    r.write8(Reg8::L, 0xDC);
    REQUIRE(r.read16(Reg16::HL) == 0xFEDC);
  }

  SECTION("Flag Z") {
    r.write16(Reg16::AF, 0x1200);

    r.set_flag(Flag::Z, true);
    REQUIRE(r.read16(Reg16::AF) == 0x1280);
    REQUIRE(r.flag(Flag::Z));

    r.set_flag(Flag::Z, false);
    REQUIRE_FALSE(r.flag(Flag::Z));
    REQUIRE(r.read16(Reg16::AF) == 0x1200);
  }

  SECTION("Flag N") {
    r.write16(Reg16::AF, 0x1200);

    r.set_flag(Flag::N, true);
    REQUIRE(r.read16(Reg16::AF) == 0x1240);
    REQUIRE(r.flag(Flag::N));

    r.set_flag(Flag::N, false);
    REQUIRE_FALSE(r.flag(Flag::N));
    REQUIRE(r.read16(Reg16::AF) == 0x1200);
  }

  SECTION("Flag H") {
    r.write16(Reg16::AF, 0x1200);

    r.set_flag(Flag::H, true);
    REQUIRE(r.read16(Reg16::AF) == 0x1220);
    REQUIRE(r.flag(Flag::H));

    r.set_flag(Flag::H, false);
    REQUIRE_FALSE(r.flag(Flag::H));
    REQUIRE(r.read16(Reg16::AF) == 0x1200);
  }

  SECTION("Flag C") {
    r.write16(Reg16::AF, 0x1200);

    r.set_flag(Flag::C, true);
    REQUIRE(r.read16(Reg16::AF) == 0x1210);
    REQUIRE(r.flag(Flag::C));

    r.set_flag(Flag::C, false);
    REQUIRE_FALSE(r.flag(Flag::C));
    REQUIRE(r.read16(Reg16::AF) == 0x1200);
  }

  SECTION("SP: read/write") {
    r.write16(Reg16::SP, 0xFFFE);
    REQUIRE(r.read16(Reg16::SP) == 0xFFFE);
  }

  SECTION("PC: read/write") {
    r.write16(Reg16::PC, 0x0100);
    REQUIRE(r.read16(Reg16::PC) == 0x0100);
  }
}
