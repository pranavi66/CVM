#pragma once
#include "compiler.h"
#include <vector>
#include <string>
#include <map>

class VM {
public:

    void run(const std::vector<Instruction>& code);
};
