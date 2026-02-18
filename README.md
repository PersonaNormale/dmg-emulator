# DMG Game Boy Emulator

Game Boy DMG emulator for `.gb` cartridges.

## Scope

* Target hardware: Nintendo Game Boy DMG (original model).
* Goal: run ROMs with video and input.
* Audio: planned in a later milestone.

## Requirements

* CMake 3.23+
* GCC with C++20 support (GCC-only for now)
* Internet access on first configure step (Catch2 is fetched by CMake)
* For coverage: `lcov` and `genhtml`

## Build

Test build:

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
cmake --build build --parallel
````

## Tests

This project uses Catch2 and integrates with CTest.

```bash
ctest --test-dir build --output-on-failure
```

## Code Coverage

Coverage requires tests to be enabled and `lcov`/`genhtml` to be available. When enabled, the `coverage` target is created only if both tools are found.

```bash
cmake -S . -B build-cov -DCMAKE_BUILD_TYPE=Debug -DGB_ENABLE_COVERAGE=ON -DBUILD_TESTING=ON
cmake --build build-cov --target coverage
```

Open report:

```bash
xdg-open build-cov/coverage/index.html
```

## Project layout

* `include/gb/`: public headers
* `src/gb/`: implementation sources
* `tests/`: unit tests

## Current status

* CPU register file

  * 8-bit registers: A, B, C, D, E, H, L
  * F is accessed via flags and as part of AF; the lower nibble of F is always zero
  * 16-bit pairs: AF, BC, DE, HL, plus SP and PC
  * Flags: Z, N, H, C

* Unit tests for register mapping and flags

## References

* [https://gbdev.io/pandocs/](https://gbdev.io/pandocs/)
* [https://rylev.github.io/DMG-01/public/book/introduction.html](https://rylev.github.io/DMG-01/public/book/introduction.html)

## License

MIT. See `LICENSE`.
