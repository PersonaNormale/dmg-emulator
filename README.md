# DMG Game Boy Emulator

Game Boy DMG emulator for `.gb` cartridges.

## Scope

* Target hardware: Nintendo Game Boy DMG (original model).
* Goal: run ROMs with video and input.
* Audio: planned in a later milestone.

## Requirements

* CMake 3.23+
* C++20 compiler (GCC or Clang)
* Internet access on first configure step (Catch2 is fetched by CMake)

## Build

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build --parallel
```

## Tests

This project uses Catch2 and integrates with CTest.

```bash
ctest --test-dir build --output-on-failure
```

## Project layout

* `include/gb/`: public headers
* `src/gb/`: implementation sources
* `tests/`: unit tests

## Current status

* CPU register file

  * 8-bit registers (F not accessible): A, B, C, D, E, H, L
  * 16-bit pairs: AF, BC, DE, HL, plus SP and PC
  * Flags: Z, N, H, C with masking so the lower nibble of F is always zero

* Unit tests for register mapping and flags

## References

* [https://gbdev.io/pandocs/](https://gbdev.io/pandocs/)
* [https://rylev.github.io/DMG-01/public/book/introduction.html](https://rylev.github.io/DMG-01/public/book/introduction.html)

## License

MIT. See `LICENSE`.
