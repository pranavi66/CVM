# CVM++ — Stack-Based Virtual Machine & Custom Compiler

A lightweight scripting language built from scratch in C++. Source code is lexed,
parsed into an AST, compiled to custom bytecode, and executed by a stack-based VM.
No libraries. No dependencies. Just C++17.

---

## What It Does

You write a simple script in the CVM++ language. The pipeline processes it in four stages:

```
Source Code  →  Lexer  →  Parser  →  Compiler  →  VM  →  Output
              (tokens)    (AST)    (bytecode)   (execute)
```

| Stage | File | Responsibility |
|---|---|---|
| Lexer | `lexer.h / lexer.cpp` | Breaks raw source text into tokens |
| Parser | `parser.h / parser.cpp` | Arranges tokens into an Abstract Syntax Tree |
| Compiler | `compiler.h / compiler.cpp` | Walks the AST and emits bytecode instructions |
| VM | `vm.h / vm.cpp` | Executes bytecode using a stack-based engine |

---

## The Language

CVM++ supports:

```
let x = 10          -- variable declaration and assignment
let y = x * 3 + 1   -- arithmetic: + - * /
print x             -- print to console
input x             -- read integer from user

if x > 5 {          -- conditionals (> < ==)
  print x
} else {
  print 0
}

while x < 10 {      -- loops
  let x = x + 1
}
```

**Supported types:** integers, booleans (`true` / `false`)  
**Operators:** `+`  `-`  `*`  `/`  `==`  `<`  `>`

---

## Project Structure

```
cvm/
├── lexer.h / lexer.cpp         Tokenizer
├── ast.h                       AST node definitions
├── parser.h / parser.cpp       Recursive descent parser
├── compiler.h / compiler.cpp   AST → bytecode compiler
├── vm.h / vm.cpp               Stack-based virtual machine
├── main.cpp                    Entry point (file runner + REPL)
├── trading_parser.h / .cpp     Trading DSL parser
├── trading_main.cpp            Trading DSL entry point
├── s1.cvm                      Sample: arithmetic
├── s2.cvm                      Sample: if/else
├── s3.cvm                      Sample: while loop
├── s4.cvm                      Sample: multiplication + equality
└── trading.tvm                 Sample: trading DSL script
```

---

## Build

Requires **g++ with C++17** support.

### Base CVM++
```bash
g++ -std=c++17 -Wall -o cvm.exe main.cpp lexer.cpp parser.cpp compiler.cpp vm.cpp
```

### Trading DSL
```bash
g++ -std=c++17 -Wall -o trading.exe trading_main.cpp lexer.cpp trading_parser.cpp compiler.cpp vm.cpp
```

---

## Run

### Run a script file
```bash
.\cvm.exe script.cvm          # run CVM++ script
.\trading.exe script.tvm      # run Trading DSL script
```

### Run with bytecode debug output
```bash
.\cvm.exe script.cvm --debug
.\trading.exe script.tvm --debug
```

### REPL mode (interactive)
```bash
.\cvm.exe
```
Type code line by line. Press **Enter on a blank line** to execute. Type `exit` to quit.

---

## Sample Scripts

### s1.cvm — arithmetic
```
let x = 10 + 5
print x
```
Output: `15`

### s2.cvm — if/else
```
let x = 10
if x > 5 {
  print x
} else {
  print 0
}
```
Output: `10`

### s3.cvm — while loop
```
let i = 1
while i < 6 {
  print i
  let i = i + 1
}
```
Output: `1 2 3 4 5` (each on a new line)

### s4.cvm — multiply + equality check
```
let a = 6
let b = 7
let c = a * b
if c == 42 {
  print 1
} else {
  print 0
}
```
Output: `1`

---

## Bytecode Examples

### Compiler test — `let x = 10 + 5 / if x > 12 { print x }`

```
Source:
  let x = 10 + 5
  if x > 12 {
    print x
  }

Bytecode:
  [0] PUSH 10
  [1] PUSH 5
  [2] ADD
  [3] STORE x
  [4] LOAD x
  [5] PUSH 12
  [6] CMP_GT
  [7] JMP_IF_FALSE 10
  [8] LOAD x
  [9] PRINT
  [10] HALT
```

### VM test — while loop counting 1 to 5

```
Source:
  let i = 1
  while i < 6 {
    print i
    let i = i + 1
  }

Output:
  1
  2
  3
  4
  5
```

---

## Trading DSL (Branch: trading-dsl)

A domain-specific language for stock trading built on top of CVM++.
The **same VM and compiler** run both languages — only the parser changes.

### Trading keywords

| Keyword | Example | What it does |
|---|---|---|
| `price` | `price AAPL = 150` | Set stock price (same as `let`) |
| `buy` | `buy AAPL 10` | Buy 10 units — adds to portfolio, deducts from cash |
| `sell` | `sell MSFT 5` | Sell 5 units — adds profit to cash |
| `show` | `show portfolio` | Print a variable (same as `print`) |

### Sample trading script

```
price AAPL  = 150
price MSFT  = 320
price GOOGL = 140

if AAPL < 160 {
    buy AAPL 10
}

if MSFT > 300 {
    sell MSFT 5
}

if GOOGL < 150 {
    buy GOOGL 8
}

show portfolio
show cash
```

### Trading bytecode (--debug output)

```
[0]  PUSH 150          -- price AAPL = 150
[1]  STORE AAPL
[2]  PUSH 320          -- price MSFT = 320
[3]  STORE MSFT
[4]  PUSH 140          -- price GOOGL = 140
[5]  STORE GOOGL
[6]  LOAD AAPL         -- if AAPL < 160
[7]  PUSH 160
[8]  CMP_LT
[9]  JMP_IF_FALSE 22
[10] LOAD portfolio     -- buy AAPL 10 → portfolio += AAPL * 10
[11] LOAD AAPL
[12] PUSH 10
[13] MUL
[14] ADD
[15] STORE portfolio
[16] LOAD cash          -- cash -= AAPL * 10
[17] LOAD AAPL
[18] PUSH 10
[19] MUL
[20] SUB
[21] STORE cash
[22] LOAD MSFT          -- if MSFT > 300
[23] PUSH 300
[24] CMP_GT
[25] JMP_IF_FALSE 32
[26] LOAD cash          -- sell MSFT 5 → cash += MSFT * 5
[27] LOAD MSFT
[28] PUSH 5
[29] MUL
[30] ADD
[31] STORE cash
[32] LOAD GOOGL         -- if GOOGL < 150
[33] PUSH 150
[34] CMP_LT
[35] JMP_IF_FALSE 48
[36] LOAD portfolio     -- buy GOOGL 8 → portfolio += GOOGL * 8
[37] LOAD GOOGL
[38] PUSH 8
[39] MUL
[40] ADD
[41] STORE portfolio
[42] LOAD cash          -- cash -= GOOGL * 8
[43] LOAD GOOGL
[44] PUSH 8
[45] MUL
[46] SUB
[47] STORE cash
[48] LOAD portfolio     -- show portfolio
[49] PRINT
[50] LOAD cash          -- show cash
[51] PRINT
[52] HALT
```

### Trading output

```
buy  AAPL 10  →  portfolio = 0    + 1500 = 1500,  cash = 10000 - 1500 = 8500
sell MSFT  5  →  cash = 8500 + 1600              = 10100
buy  GOOGL 8  →  portfolio = 1500 + 1120 = 2620,  cash = 10100 - 1120 = 8980

portfolio  →  2620
cash       →  8980
```

### Build and run Trading DSL
```powershell
g++ -std=c++17 -Wall -o trading.exe trading_main.cpp lexer.cpp trading_parser.cpp compiler.cpp vm.cpp
.\trading.exe trading.tvm
.\trading.exe trading.tvm --debug
```

---

## How the VM Works

The VM is a **fetch-decode-execute loop** over a flat array of instructions.
It maintains three pieces of state:

- **Operand stack** (`vector<int>`) — values are pushed/popped during expression evaluation
- **Variables map** (`map<string, int>`) — stores named variables
- **Instruction pointer** (`int ip`) — index of the current instruction

### Opcode Reference

| Opcode | Description |
|---|---|
| `PUSH n` | Push integer `n` onto the stack |
| `LOAD x` | Push the value of variable `x` |
| `STORE x` | Pop top of stack, save as variable `x` |
| `ADD` | Pop two values, push their sum |
| `SUB` | Pop two values, push left − right |
| `MUL` | Pop two values, push their product |
| `DIV` | Pop two values, push left ÷ right |
| `NEG` | Negate top of stack |
| `CMP_EQ` | Pop two values, push 1 if equal else 0 |
| `CMP_LT` | Pop two values, push 1 if left < right else 0 |
| `CMP_GT` | Pop two values, push 1 if left > right else 0 |
| `PRINT` | Pop top of stack and print it |
| `INPUT x` | Read integer from stdin, store as `x` |
| `JMP n` | Jump to instruction index `n` |
| `JMP_IF_FALSE n` | Pop stack — if 0 (false), jump to `n` |
| `HALT` | Stop execution |

---

## Testing Individual Stages

```powershell
# Lexer only
g++ -std=c++17 -o test_lexer test_lexer.cpp lexer.cpp
.\test_lexer.exe

# Parser only
g++ -std=c++17 -o test_parser test_parser.cpp lexer.cpp parser.cpp
.\test_parser.exe

# Compiler only
g++ -std=c++17 -o test_compiler test_compiler.cpp lexer.cpp parser.cpp compiler.cpp
.\test_compiler.exe

# VM only
g++ -std=c++17 -o test_vm test_vm.cpp lexer.cpp parser.cpp compiler.cpp vm.cpp
.\test_vm.exe
```

---

## References

- *Crafting Interpreters* — Robert Nystrom (architecture reference)
- "Writing a Lexer in C++"
- "Understanding Stack-Based Virtual Machines"