#include <catch2/catch_test_macros.hpp>
#include <gb/cpu/registers.hpp>

using namespace gb::cpu;

TEST_CASE("CPU Registers: Mapping and Flag") {
  Registers r{};

  SECTION("AF: lower nibble of F at zero") {
    r.write16(Reg16::AF, 0x12FF);
    REQUIRE(r.read8(Reg8::A) == 0x12);
    REQUIRE(r.read16(Reg16::AF) == 0x12F0);
  }

  SECTION("HL: split/join") {
    r.write16(Reg16::HL, 0xBEEF);
    REQUIRE(r.read8(Reg8::H) == 0xBE);
    REQUIRE(r.read8(Reg8::L) == 0xEF);
    REQUIRE(r.read16(Reg16::HL) == 0xBEEF);
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
}
