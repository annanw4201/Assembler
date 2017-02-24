#include "kind.h"
#include "lexer.h"
#include "dotWord.h"
#include "label.h"
#include "jr_jalr.h"
#include "add_sub_slt_sltu.h"
#include "beq_bne.h"
#include "lis_mflo_mfhi.h"
#include "mult_multu_div_divu.h"
#include "sw_lw.h"

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
using std::map;
using std::cout;
using std::endl;
using ASM::Token;
using ASM::Lexer;

// output the machine code
void output_instr(int instr) {
    putchar(instr >> 24);
    putchar(instr >> 16);
    putchar(instr >> 8);
    putchar(instr);
}

string lineString (vector<vector<Token*> >::iterator &it) {
    string line ="";
    vector<Token*>::iterator it2;
    for (it2 = it->begin(); it2 != it->end(); ++it2) {
        line += (*it2)->getLexeme() + " ";
    }
    return line;
}

/////////////////////////////////MAIN/////////////////////////////////
int main(int argc, char* argv[]){
    // Nested vector representing lines of Tokens
    // Needs to be used here to cleanup in the case
    // of an exception
    vector< vector<Token*> > tokLines;
    try{
        // Create a MIPS recognizer to tokenize
        // the input lines
        Lexer lexer;
        // Tokenize each line of the input
        string line;
        while(getline(cin,line)){
            tokLines.push_back(lexer.scan(line));
        }
        
        
        // Iterate over the lines of tokens and print them
        // to standard error
        vector<vector<Token*> >::iterator it;
        
        vector <int> dotWordTable; // declare dotWordTable
        map <string, int> label_table; // declare map for the lable
        int pcAddr = 12; // declare pc counter
        
        
        // first pass to check syntax errors
        for(it = tokLines.begin(); it != tokLines.end(); ++it){
            vector<Token*>::iterator it2;
            for(it2 = it->begin(); it2 != it->end(); ++it2) {
                cerr << "  Token: "  << *(*it2) << endl;
                if ((*it2)->getKind() == ASM::DOTWORD) {
                    pcAddr += 4;
                    check_dotword(it, it2);
                } //check DOTWORD
                
                else if ((*it2)->getKind() == ASM::LABEL) {
                    check_label(it, it2, pcAddr, label_table);
                } // check LABEL
                
                else if ((*it2)->getKind() == ASM::ID) {
                    if ((*it2)->getLexeme() == "jr" || (*it2)->getLexeme() == "jalr") {
                        pcAddr += 4;
                        check_jr_jalr(it, it2);
                    }
                    else if ((*it2)->getLexeme() == "add" || (*it2)->getLexeme() == "sub" ||
                             (*it2)->getLexeme() == "slt" || (*it2)->getLexeme() == "sltu") {
                        pcAddr += 4;
                        check_add_sub_slt_sltu_bin (it, it2);
                    }
                    else if ((*it2)->getLexeme() == "beq" || (*it2)->getLexeme() == "bne") {
                        pcAddr += 4;
                        check_beq_bne(it, it2);
                    }
                    else if ((*it2)->getLexeme() == "lis" || (*it2)->getLexeme() == "mfhi" ||
                             (*it2)->getLexeme() == "mflo") {
                        pcAddr += 4;
                        check_lis_mfhi_mflo (it, it2);
                    }
                    else if ((*it2)->getLexeme() == "mult" || (*it2)->getLexeme() == "multu" ||
                             (*it2)->getLexeme() == "div" || (*it2)->getLexeme() == "divu") {
                        pcAddr += 4;
                        check_mult_multu_div_divu(it, it2);
                    }
                    else if ((*it2)->getLexeme() == "sw" || (*it2)->getLexeme() == "lw") {
                        pcAddr += 4;
                        check_sw_lw (it, it2);
                    }
                } // check opcode
            }
        }
        
        
        // find label definition to see if it is defined
        for(it = tokLines.begin(); it != tokLines.end(); ++it){
            vector<Token*>::iterator it2;
            for(it2 = it->begin(); it2 != it->end(); ++it2) {
                if ((*it2)->getKind() == ASM::ID) {
                    find_labelDef(it2, label_table);
                }
            }
        }
        
        
        /////////////////////////////////////////////////MERL/////////////////////////////////////////////////
        
        // get the dotword label address and push it into dotWordTable
        for(it = tokLines.begin(); it != tokLines.end(); ++it){
            vector<Token*>::iterator it2;
            for(it2 = it->begin(); it2 != it->end(); ++it2) {
                if ((*it2)->getKind() == ASM::DOTWORD) {
                    ++it2;
                    if ((*it2)->getKind() == ASM::ID) {
                        const int dotWordAddr = 1;
                        dotWordTable.push_back(dotWordAddr);
                    }
                }
            }
        }
        
        
        
        
        
        // second pass to convert mips code to machine code
        pcAddr = 12;
        for(it = tokLines.begin(); it != tokLines.end(); ++it){
            vector<Token*>::iterator it2;
            for(it2 = it->begin(); it2 != it->end(); ++it2) {
                if ((*it2)->getKind() == ASM::DOTWORD) {
                    dotWord_bin(it2, dotWordTable, pcAddr, label_table);
                    pcAddr += 4;
                }
                else if ((*it2)->getLexeme() == "jr") {
                    pcAddr += 4;
                    const int immediate = 8;
                    jr_jalr_bin (it2, immediate);
                }
                else if ((*it2)->getLexeme() == "jalr") {
                    pcAddr += 4;
                    const int immediate = 9;
                    jr_jalr_bin (it2, immediate);
                }
                else if ((*it2)->getLexeme() == "add") {
                    pcAddr += 4;
                    const int immediate = 32;
                    add_sub_slt_sltu_bin(it2, immediate);
                }
                else if ((*it2)->getLexeme() == "sub") {
                    pcAddr += 4;
                    const int immediate = 34;
                    add_sub_slt_sltu_bin(it2, immediate);
                }
                else if ((*it2)->getLexeme() == "slt") {
                    pcAddr += 4;
                    const int immediate = 42;
                    add_sub_slt_sltu_bin(it2, immediate);
                }
                else if ((*it2)->getLexeme() == "sltu") {
                    pcAddr += 4;
                    const int immediate = 43;
                    add_sub_slt_sltu_bin(it2, immediate);
                }
                else if ((*it2)->getLexeme() == "beq") {
                    pcAddr += 4;
                    const int opcode = 4;
                    beq_bne_bin(it2, opcode, pcAddr, label_table);
                }
                else if ((*it2)->getLexeme() == "bne") {
                    pcAddr += 4;
                    const int opcode = 5;
                    beq_bne_bin(it2, opcode, pcAddr, label_table);
                }
                else if ((*it2)->getLexeme() == "lis") {
                    pcAddr += 4;
                    int immediate = 20;
                    lis_mflo_mfhi_bin(it2, immediate);
                }
                else if ((*it2)->getLexeme() == "mflo") {
                    pcAddr += 4;
                    int immediate = 18;
                    lis_mflo_mfhi_bin(it2, immediate);
                }
                else if ((*it2)->getLexeme() == "mfhi") {
                    pcAddr += 4;
                    int immediate = 16;
                    lis_mflo_mfhi_bin(it2, immediate);
                }
                else if ((*it2)->getLexeme() == "div") {
                    pcAddr += 4;
                    int immediate = 26;
                    mult_multu_div_divu_bin(it2, immediate);
                }
                else if ((*it2)->getLexeme() == "divu") {
                    pcAddr += 4;
                    int immediate = 27;
                    mult_multu_div_divu_bin(it2, immediate);
                }
                else if ((*it2)->getLexeme() == "mult") {
                    pcAddr += 4;
                    int immediate = 24;
                    mult_multu_div_divu_bin(it2, immediate);
                }
                else if ((*it2)->getLexeme() == "multu") {
                    pcAddr += 4;
                    int immediate = 25;
                    mult_multu_div_divu_bin(it2, immediate);
                }
                else if ((*it2)->getLexeme() == "sw") {
                    pcAddr += 4;
                    int opcode = 43;
                    sw_lw_bin(it2, opcode);
                }
                else if ((*it2)->getLexeme() == "lw") {
                    pcAddr += 4;
                    int opcode = 35;
                    sw_lw_bin(it2, opcode);
                }
            }
        }
        
       
        
        //print lable table
        //print_label_table();
    } // try block ends
    catch(const string& msg){
        // If an exception occurs print the message and end the program
        cerr << msg << endl;
    }
    
    // Delete the Tokens that have been made
    vector<vector<Token*> >::iterator it;
    for(it = tokLines.begin(); it != tokLines.end(); ++it){
        vector<Token*>::iterator it2;
        for(it2 = it->begin(); it2 != it->end(); ++it2){
            delete *it2;
        }
    }
}
