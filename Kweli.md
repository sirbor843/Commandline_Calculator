# Advanced CLI Calculator — C Programming Mastery

[![Build Status](https://img.shields.io/badge/build-passing-brightgreen.svg)](https://github.com/yourusername/Commandline_Calculator)
[![License](https://img.shields.io/badge/license-MIT-blue.svg)](LICENSE)
[![C Standard](https://img.shields.io/badge/C%20Standard-C99-orange.svg)](https://en.wikipedia.org/wiki/C99)
[![Valgrind](https://img.shields.io/badge/Memory-Valgrind%20Clean-blueviolet.svg)](https://valgrind.org/)

A **production-grade CLI calculator** built over **30 days** to demonstrate the full spectrum of C programming—from basic syntax to advanced systems-level concepts.

---

## Key Highlights

- **12+ Feature Categories**: From arithmetic to numerical methods, financial math, scripting, and plotting.
- **Comprehensive C Coverage**: Core language + systems concepts demonstrated across real modules.
- **Industrial Standards**: Modular architecture, robust signal handling, and memory-safe patterns.
- **Modern CLI**: Rich visual feedback, colored output, command routing, and structured help.

---

## Feature Roadmap

| Category         | Typical Command              | Deep C Concepts              |
| :--------------- | :--------------------------- | :--------------------------- |
| **Basic Ops**    | `basic sum 1 2 3 4 5`        | Variadic logic, operators    |
| **Scientific**   | `scientific sin 30 deg`      | Math library, recursion      |
| **Statistical**  | `statistical median 1 5 3`   | Dynamic arrays, sorting      |
| **Graphical**    | `graphical plot x^2`         | 2D buffers, ASCII rendering  |
| **Solvers**      | `solver quadratic 1 -5 6`    | Root finding, linear algebra |
| **Calculus**     | `calculus integral x^2 0 1`  | Numerical methods            |
| **Memory**       | `memory store x 42`          | BST storage, named variables |
| **History**      | `history`                    | Queue-based result tracking  |
| **Complex**      | `complex mul 3+4i 1+2i`      | Structs, custom parsing      |
| **Converters**   | `converter to_binary 42`     | Bitwise masks, unit tables   |
| **Programming**  | `programming ds_demo all`    | Stack, Graph, List, callbacks|
| **Concept View** | `programming concepts`       | Coverage introspection       |
| **Financial**    | `financial irr -100 50 60`   | Newton's method, precision   |
| **Persistence**  | `extra save_bin data.bin 1 2`| File I/O, binary buffers     |
| **System**       | `help`, `quit`               | Async signals, safe cleanup  |


---

## The 30-Day Curriculum

Each day of development introduced a fundamental or advanced C concept:

<details>
<summary><b>Week 1: Foundations & Control (Days 1-7)</b></summary>

- **Day 1**: CLI Foundation (Preprocessor, I/O, `main`)
- **Day 2**: Basic Syntax (Data types, constants, scope)
- **Day 3**: Input Mastery (`printf`/`scanf` formats, `fgets`)
- **Day 4**: Operators (Arithmetic, relational, precedence)
- **Day 5**: Control Flow (`if`/`switch`, `for`/`while` loops)
- **Day 6**: Functions (Prototypes, parameters, stack frames)
- **Day 7**: Arrays & Strings (`string.h`, `strtok`, `sscanf`)

</details>

<details>
<summary><b>Week 2: Pointers & Memory (Days 8-14)</b></summary>

- **Day 8**: Recursion (Stack depth, base cases)
- **Day 9**: Advanced Arrays (2D initialization, multi-dim)
- **Day 10**: Strings Deep-Dive (`char*` vs arrays)
- **Day 11**: Pointer Basics (`&` / `*` operators)
- **Day 12**: Pointer Arithmetic (Offsets, array decay)
- **Day 13**: Dynamic Memory (`malloc`, `calloc`, `free`)
- **Day 14**: Memory Safety (Leaks, `valgrind` patterns)

</details>

<details>
<summary><b>Week 3: Data Structures (Days 15-21)</b></summary>

- **Day 15**: Structures (`struct`, member access)
- **Day 16**: Advanced Structs (`typedef`, nesting)
- **Day 17**: Unions & Storage (Memory layout, `extern`)
- **Day 18**: File I/O Basics (`fopen`, `fprintf`, `fscanf`)
- **Day 19**: Binary Files (`fread`, `fwrite`, `fseek`)
- **Day 20**: Dynamic Arrays (`realloc`, capacity management)
- **Day 21**: Error Handling (`ferror`, `errno`, robust I/O)

</details>

<details>
<summary><b>Week 4: Advanced Systems (Days 22-30)</b></summary>

- **Day 22**: Double Pointers (`**ptr`, matrix ops)
- **Day 23**: Function Pointers (Callbacks, dispatch tables)
- **Day 24**: Bit Manipulation (`& | ^ ~ << >>`, masks)
- **Day 25**: Preprocessor Advanced (Macros, `#ifdef`)
- **Day 26**: Stacks & Queues (LIFO/FIFO implementation)
- **Day 27**: Trees (Binary Search Trees, traversal)
- **Day 28**: Graphs (Adjacency lists, traversal)
- **Day 29**: Build Systems (Makefiles, dependencies)
- **Day 30**: Integration & Debugging (`gdb`, `signals`)

</details>

---

## Getting Started

### Prerequisites

- GCC or Clang (C99 compatible)
- `make` build utility
- `valgrind` (optional, for memory analysis)

### Installation & Execution

```bash
# Clone the repository
git clone <repo_url> Commandline_Calculator
cd Commandline_Calculator

# Build the project
make

# Run the calculator
./calculator

# Build with release optimizations
make BUILD=release

# Build with address sanitizer
make BUILD=asan

# Build with undefined behavior sanitizer
make BUILD=ubsan

# Run memory leak checks
make valgrind

# Run CLI regression tests
make test

# CI-style local check (rebuild + tests)
make ci
```

---

## Developer Tools

- **Debug Build**: `make` (defaults to `BUILD=debug`).
- **Release Build**: `make BUILD=release`.
- **Sanitizers**: `make BUILD=asan` and `make BUILD=ubsan`.
- **Run**: `make run`.
- **Tests**: `make test` for command-flow checks.
- **CI-style Local Gate**: `make ci`.
- **Leak Check**: `make valgrind`.
- **Discoverability**: `make help`.

---

## Project Layout

- `src/core/`: main loop, command dispatch, parser, lifecycle hooks.
- `src/ops/`: all calculator operation domains.
- `src/data_structs/`: queue, stack, BST, graph, linked list.
- `src/utils/`: output formatting and signal handling.
- `include/`: public headers mirroring source structure.
- `tests/`: CLI integration smoke tests.

---

## C Concept Coverage Map

This project is designed to **comprehensively cover practical C programming concepts** in one cohesive, non-trivial application.

| C Concept Area                                | Where It Is Demonstrated                                  |
| :-------------------------------------------- | :-------------------------------------------------------- |
| Preprocessor macros / conditional compilation | `include/common.h`, `DEBUG_PRINT`                         |
| Primitive types, casts, numeric formatting    | `src/utils/utils.c`, all operation handlers               |
| Control flow (`if`, `switch`, loops)          | `src/core/command_handler.c`, operation modules           |
| Functions / modular interfaces                | `include/` + `src/` boundaries                            |
| Arrays / strings / tokenization               | `src/core/command_handler.c`, `src/core/evaluation.c`     |
| Pointers / pointer arithmetic                 | parser and data-structure traversals                      |
| Dynamic memory (`malloc/realloc/free`)        | `src/ops/*`, `src/data_structs/*`                         |
| Structs / typedefs                            | `include/common.h`, operation result types                |
| Unions / tagged-data pattern                  | `include/common.h`                                        |
| Enums                                         | `CommandType` in `include/common.h`                       |
| Function pointers / dispatch tables           | `src/core/command_handler.c`, `src/ops/programming_ops.c` |
| Recursion                                     | expression parser + recursive traversals                  |
| File I/O                                      | `src/ops/extra_ops.c`                                     |
| Bitwise operations                            | `src/ops/converter_ops.c`                                 |
| Data structures (stack/queue/tree/graph/list) | `src/data_structs/`                                       |
| Error handling strategy                       | `CalculationResult`, `log_error`, parser strictness       |
| Signals and graceful shutdown                 | `src/utils/signals.c`, `src/core/cleanup.c`               |
| Build engineering / toolchain                 | `Makefile`, multiple build profiles                       |

### Note on “all concepts”

No single project can literally implement every edge-case of the full C language and every POSIX/system extension.  
This calculator aims for **comprehensive practical coverage** of mainstream C and systems programming topics while staying coherent and maintainable.

---

## Quality and Flow Guarantees

- Strict expression parsing rejects trailing garbage and malformed tokens.
- Numerical error paths return explicit failures (e.g., divide-by-zero in parser).
- Command flow supports spaced expressions for key features like `solver roots` and `calculus integral`.
- Automated CLI tests validate behavior continuously (`make test`).
- Build profiles help catch memory and undefined-behavior issues early.

---

## Coding Standards

- **Concept Comments**: Every file identifies specific C concepts using `// CONCEPT: [Name]`.
- **Warning Free**: Compiled with `-Wall -Wextra -Werror` to ensure high code quality.
- **Modular**: Logic is strictly separated into `ops/` (math), `data_structs/` (storage), and `utils/` (system).

---

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

---

_
