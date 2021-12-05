# Advent of Code 2021

My solutions to Advent of Code 2021

For more information, see: https://adventofcode.com/2021/

---

## Compile

Build: `cd` to any `d_/` directory and type `make` or `make -C d_/` from parent directory.

---

## Uses

From any `d_/` run
1. `./main` will prompt user for input file. Output is written to `d_/output`.
2. `./main <input file>` uses `<input file>` as input file. Output is written to `d_/output`.
3. `./main <input file> <output file>` uses `<input file>` as input file and `<output file>` as output file
4. `./main <input file> <output file> <something else>` throws error.

--- 
## Other make commands

* `make clean` or `make -C d_/ clean` removes associated support files, e.g. `main, *.o, *.log`
* `make test` or `make -C d_/ test` times execution time and stores result to `d_/test.log`. Uses `./main input`, i.e. `input` is required. 