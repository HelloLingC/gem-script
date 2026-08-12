# gem-script Roadmap & Implementation Todo List

Welcome to the development roadmap for **gem-script**! This document outlines the step-by-step plan to evolve `gem-script` from a simple AST tree-walking interpreter into a high-performance **Bytecode Virtual Machine**.

---

## 📊 Current Architecture Overview
Currently, `gem-script` features:
- **Lexer**: Tokenizes numbers, identifiers, simple keywords (`if`, `else`, `while`), basic operators (`+`, `-`, `*`, `/`, `=`, `==`, `<`, `>`), `()`, `{}`.
- **Parser**: Builds an AST using recursive descent.
- **AST Nodes**: Numbers, Identifiers, Binary/Unary Ops, Assignments, Blocks, If Statements, While Loops.
- **Evaluator**: Tree-walking evaluator recursively traversing AST nodes to compute runtime `Value`s.

---

## 🎯 Target Milestones

### Milestone 1: Language & Frontend Enhancements
Before compiling to Bytecode or LLVM IR, the language constructs need to support modular code execution (functions & types).

- [X] **1.1 Boolean & String Literals**
  - Add `TOKEN_TRUE`, `TOKEN_FALSE`, `TOKEN_STRING` to `lexer`.
  - Extend `Value` tagged union to support strings (`ObjString *` or dynamic string buffer).
- [ ] **1.2 Function Support (`fn` & Return)**
  - Lexer: Add `fn` and `return` keywords.
  - AST: `AST_FUNCTION_DECL` (name, params, body) and `AST_CALL` (callee, arguments).
  - Scope resolution for local parameters vs outer variables.
- [ ] **1.3 Print / Standard I/O Native Functions**
  - Add native `print()` support for debugging scripts without raw `evaluate` prints.

---

### Milestone 2: Stack-Based Bytecode Virtual Machine (VM)
A virtual machine replaces tree-walking evaluation with a compact linear bytecode instruction stream executed by a fast stack-based loop.

- [ ] **2.1 Instruction Set Architecture (ISA) & Bytecode Specification**
  - Create `chunk.h` and `chunk.c`.
  - Define opcodes:
    - **Literals**: `OP_CONSTANT`, `OP_NIL`, `OP_TRUE`, `OP_FALSE`
    - **Arithmetic & Logic**: `OP_ADD`, `OP_SUB`, `OP_MUL`, `OP_DIV`, `OP_EQUAL`, `OP_GREATER`, `OP_LESS`, `OP_NOT`
    - **Variables**: `OP_GET_GLOBAL`, `OP_SET_GLOBAL`, `OP_GET_LOCAL`, `OP_SET_LOCAL`
    - **Control Flow**: `OP_JUMP_IF_FALSE`, `OP_JUMP`, `OP_LOOP`
    - **Functions**: `OP_CALL`, `OP_RETURN`
- [ ] **2.2 Chunk & Constant Pool Representation**
  - `Chunk` struct: Dynamic `uint8_t *code`, capacity, count, and line number array.
  - `ValueArray` constant pool for numbers and strings.
- [ ] **2.3 Bytecode Compiler (`compiler.c` / `compiler.h`)**
  - Traverse AST (or direct single-pass compile) and emit corresponding opcodes.
  - Local variable resolution: map variable names to stack frame offsets at compile time.
  - Jump backpatching for conditional `if`/`else` branches and `while` loop target addresses.
- [ ] **2.4 Virtual Machine Execution Engine (`vm.c` / `vm.h`)**
  - VM State: `Value stack[STACK_MAX]`, `Value *stackTop`, `CallFrame frames[FRAMES_MAX]`.
  - Execution loop: `switch (*ip++)` dispatch loop.
  - Stack manipulation macros/functions (`push`, `pop`).
- [ ] **2.5 Bytecode Disassembler & Debugging (`debug.c`)**
  - `disassembleChunk()` and `disassembleInstruction()` to inspect opcode streams.

---

### Milestone 3: Memory Management & Garbage Collection
- [ ] **3.1 Dynamic Object Model (`Obj`)**
  - Struct hierarchy for heap-allocated items: Strings, Functions, Closures.
- [ ] **3.2 Mark-and-Sweep Garbage Collector (`memory.c`)**
  - Mark Phase: Trace roots from VM stack, global table, and active call frames.
  - Sweep Phase: Reclaim un-marked heap objects.

---

## 🛠 Recommended Next Action
Start with **Milestone 2.1 & 2.2** (Defining opcodes & the `Chunk` struct), or complete **Milestone 1.2** (adding functions) if you want functions fully designed before writing the VM compiler!
