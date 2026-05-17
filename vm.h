#pragma once
#include "compiler.h"
#include <vector>
#include <string>
#include <map>
using namespace std;

class VM {
public:
    void run(const vector<Instruction>& code);
};