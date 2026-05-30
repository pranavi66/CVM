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
| Lexer | `lexer.cpp` | Breaks raw source text into tokens |
| Parser | `parser.cpp` | Arranges tokens into an Abstract Syntax Tree |
| Compiler | `compiler.cpp` | Walks the AST and emits bytecode instructions |
| VM | `vm.cpp` | Executes bytecode using a stack-based engine |

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
├── s1.cvm                      Sample: arithmetic
├── s2.cvm                      Sample: if/else
├── s3.cvm                      Sample: while loop
└── s4.cvm                      Sample: multiplication + equality
```

---

## Build

Requires **g++ with C++17** support.

```bash
g++ -std=c++17 -Wall -o cvm main.cpp lexer.cpp parser.cpp compiler.cpp vm.cpp
```

On Windows (PowerShell):
```powershell
g++ -std=c++17 -Wall -o cvm.exe main.cpp lexer.cpp parser.cpp compiler.cpp vm.cpp
```

---

## Run

### Run a script file
```bash
./cvm script.cvm          # Linux / Mac
.\cvm.exe script.cvm      # Windows PowerShell
```

### Run with bytecode debug output
```bash
./cvm script.cvm --debug
.\cvm.exe script.cvm --debug
```

### REPL mode (interactive)
```bash
./cvm
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



## Testing Individual Stages

Build and run each stage in isolation:

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
