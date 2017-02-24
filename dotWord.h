//
//  dotWord.hpp
//  assembler
//
//  Created by Wang Annan on 2017-02-02.
//  Copyright © 2017 Wang Annan. All rights reserved.
//

#ifndef dotWord_hpp
#define dotWord_hpp

#include "kind.h"
#include "lexer.h"
#include <vector>
#include <string>
#include <iostream>
#include <cstdio>
#include <map>

// Use only the neeeded aspects of each namespace
using std::string;
using std::vector;
using std::cerr;
using std::cin;
using std::getline;
using ASM::Token;
using ASM::Lexer;
using std::map;


void output_instr(int instr);

// check dotword
void check_dotword (vector<vector<Token*> >::iterator &it, vector<Token*>::iterator &it2) {
    ++it2;
    if (it2 == it->end() || (*it2)->getKind() == ASM::REGISTER) {
        --it2;
        throw "ERROR: Expect a value after word directive: " + (*it2)->getLexeme();
    }
    if ((*it2)->getKind() == ASM::INT ||
        (*it2)->getKind() == ASM::HEXINT ||
        (*it2)->getKind() == ASM::ID) {
        ++it2;
        if (it2 != it->end()) {
            throw string ("ERROR: Expect end of line but have extra");
        }
        else {
            --it2; // process token after .word again
        }
    }
}

// output machine code of dotWord
void dotWord_bin(vector<Token*>::iterator &it2, vector <int> &dotWordTable,
                 const int pcAddr, map<string, int> &label_table) {
    ++it2;
    unsigned int dotWordVal;
    if ((*it2)->getKind() == ASM::ID) {
        dotWordVal = label_table[((*it2)->getLexeme())];
        dotWordTable.push_back(pcAddr);
    }
    else {
        dotWordVal = (*(*it2)).toInt();
    }
    output_instr(dotWordVal);
}

#endif /* dotWord_hpp */
