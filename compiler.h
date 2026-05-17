#pragma once
#include "ast.h"
#include <vector>
#include <string>
#include <cstdint>
using namespace std;

enum class OpCode : uint8_t {
    PUSH,           // arg = int value
    LOAD,           // arg = var name index
    STORE,          // arg = var name index
    ADD, SUB, MUL, DIV,
    NEG,
    CMP_EQ, CMP_LT, CMP_GT,
    PRINT,
    INPUT,          // arg = var name index
    JMP,            // arg = instruction index
    JMP_IF_FALSE,   // arg = instruction index
    HALT
};

struct Instruction {
    OpCode op;
    int    arg  = 0;   // integer operand (value, index, or jump target)
    string sarg = "";  // string operand (variable name)
};

vector<Instruction> compile(const ProgramNode& program);