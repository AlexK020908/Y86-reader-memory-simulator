// #include "sim.cpp"
#include "sim.h"
#include <vector>
#include <algorithm>
std::unordered_map<std::string, int> NameToIndex;
void initNameToIndex() {
    NameToIndex["%rax"] = 0;
    NameToIndex["%rcx"] = 1;
    NameToIndex["%rdx"] = 2;
    NameToIndex["%rbx"] = 3;
    NameToIndex["%rsp"] = 4;
    NameToIndex["%rbp"] = 5;
    NameToIndex["%rsi"] = 6;
    NameToIndex["%rdi"] = 7;
    NameToIndex["%r8"] = 8;
    NameToIndex["%r9"] = 9;
    NameToIndex["%r10"] = 10;
    NameToIndex["%r11"] = 11;
    NameToIndex["%r12"] = 12;
    NameToIndex["%r13"] = 13;
    NameToIndex["%r14"] = 14;
    NameToIndex["NONE"] = 15;

}
inst_map_t getName(std::string & input) {
        int i = 0;
        while(input[i] != ' ') {
            i++;
        }
        inst_map_t toreturn = inst_to_enum(input.substr(0, i));
        input = input.substr(i + 1);
        return toreturn;
}
int readFirstArgAndCut(std::string & input) {
        int i = 0;
        
        while(input[i] != ' ') {
            i++;
        }
        int toreturn = NameToIndex[input.substr(0, i)];
        if(i + 1 < input.size()) input = input.substr(i + 1);
        return toreturn;

}
int readValc(std::string & input) {
        int i = 0;
        
        while(input[i] != ' ') {
            i++;
        }
        int toreturn = stoi(input.substr(0, i));
        return toreturn;

}
instruction_t parseInput(std::string input) {
    //getting the name 
    instruction_t toReturn;
    inst_map_t name = getName(input);
    int rA = 0;
    int rB = 0;
    int valC = 0;
    //these are jump just compliation --> not fully implmented yet
    switch(name) {
            case I_NOP:
            {		
                toReturn.name = "nop";	
                break;
            }
            case I_HALT:
            {          
                toReturn.name = "halt";    	        		

                break;
            }
            case I_RRMOVQ:
            {			
                toReturn.name = "rrmovq";
                toReturn.rA = readFirstArgAndCut(input);
                toReturn.rB = readFirstArgAndCut(input);

                break;
            }
            case I_IRMOVQ:
            {			
                toReturn.name = "irmovq";
                toReturn.valC = readValc(input);
                toReturn.rB = readFirstArgAndCut(input);
                break;	
            }
            case I_RMMOVQ:
            {   		
                toReturn.name = "rmmovq";
                toReturn.rA = readFirstArgAndCut(input);
                toReturn.rB = readFirstArgAndCut(input);
                toReturn.valC = readValc(input);

                break;
            }
            case I_MRMOVQ:
            {		
                toReturn.name = "mrmovq";
                toReturn.rA = readFirstArgAndCut(input);
                toReturn.rB = readFirstArgAndCut(input);
                toReturn.valC = readValc(input);

                break;
            }
            case I_PUSHQ:
            {			
                toReturn.name = "pushq";
                toReturn.rA = readFirstArgAndCut(input);
                toReturn.rB = 0xF;
                break;
            }
            case I_POPQ:
            {		
                toReturn.name = "popq";
                toReturn.rA = readFirstArgAndCut(input);
                toReturn.rB = 0xF;
                break;
            }
            case I_CALL:
            {			
                toReturn.name = "call";
                toReturn.valC = readValc(input);
                break;
            }
            case I_RET:
            {			
                toReturn.name = "ret";
                break;	
            }
            case I_J:
            {			
                toReturn.name = "j";
                toReturn.valC = readValc(input);
                break;
            }
            case I_JEQ:
            {			
                toReturn.name = "jeq";
                toReturn.valC = readValc(input);
                break;
            }
            case I_JNE:
            {	
                toReturn.name = "jne";
                toReturn.valC = readValc(input);		
                break;
            }
            case I_JL:
            {
                toReturn.name = "jl";
                toReturn.valC = readValc(input);	
                
                break;
            }
            case I_JLE:
            {		
                toReturn.name = "jle";
                toReturn.valC = readValc(input);		
                break;
            }
            case I_JG:
            {		
                toReturn.name = "jg";
                toReturn.valC = readValc(input);		
                break;
            }
            case I_JGE:
             {		
                toReturn.name = "jge";
                toReturn.valC = readValc(input);	
                break;
            }
            case I_ADDQ:
            {		
                toReturn.name = "addq";
                toReturn.rA = readFirstArgAndCut(input);
                toReturn.rB = readFirstArgAndCut(input);
                break;
            }
            case I_SUBQ:
            {			
                toReturn.name = "subq";
                toReturn.rA = readFirstArgAndCut(input);
                toReturn.rB = readFirstArgAndCut(input);
                break;
            }
            case I_MULQ:
            {   	
                toReturn.name = "mulq";
                toReturn.rA = readFirstArgAndCut(input);
                toReturn.rB = readFirstArgAndCut(input);		
                break;
            }
            case I_MODQ:
                
            {		
                toReturn.name = "modq";
                toReturn.rA = readFirstArgAndCut(input);
                toReturn.rB = readFirstArgAndCut(input);	
                break;
            }
            case I_DIVQ:
            {			
                toReturn.name = "divq";
                toReturn.rA = readFirstArgAndCut(input);
                toReturn.rB = readFirstArgAndCut(input);
                break;
            }	
            case I_ANDQ:
            {		
                toReturn.name = "andq";
                toReturn.rA = readFirstArgAndCut(input);
                toReturn.rB = readFirstArgAndCut(input);	
                break;
            }
            case I_XORQ:
            {			
                toReturn.name = "xorq";
                toReturn.rA = readFirstArgAndCut(input);
                toReturn.rB = readFirstArgAndCut(input);
                break;
            }
            case I_CMOVEQ:
            {			
                toReturn.name = "cmoveq";
                toReturn.rA = readFirstArgAndCut(input);
                toReturn.rB = readFirstArgAndCut(input);
                break;
            }
             case I_CMOVNE:
            {			
                toReturn.name = "cmovne";
                toReturn.rA = readFirstArgAndCut(input);
                toReturn.rB = readFirstArgAndCut(input);
                break;
            }
        
            case I_CMOVL:
            {		
                toReturn.name = "cmovl";
                toReturn.rA = readFirstArgAndCut(input);
                toReturn.rB = readFirstArgAndCut(input);	
                break;
            }
            case I_CMOVLE:
            {			
                toReturn.name = "cmovle";
                toReturn.rA = readFirstArgAndCut(input);
                toReturn.rB = readFirstArgAndCut(input);	
                break;	
            }
            case I_CMOVG:
            {			
                toReturn.name = "cmovg";
                toReturn.rA = readFirstArgAndCut(input);
                toReturn.rB = readFirstArgAndCut(input);	
                break;
            }
            case I_CMOVGE:
            {		
                toReturn.name = "cmovge";
                toReturn.rA = readFirstArgAndCut(input);
                toReturn.rB = readFirstArgAndCut(input);
                break;
            }
            default: {
                std::cout << "invalid" << std::endl;
                break;
            }
        }
        return toReturn;
}
int main() {
    initNameToIndex();
    initMap();
    //first let us make a global state 
    state_t * globalState = new state_t();
    memset(globalState->m, 15, sizeof(globalState->m));
    globalState->start = 0x1000;
    globalState->start = 700;
    memset(globalState->R, 0, sizeof(globalState->R));
    globalState->pc = 0x1000;
    globalState->flags = 0x0;

    //printOutState(globalState);

    //make an instruction 
    //for example irmovq $15 %rax
    //we need to design a function to parse this input 
    //basically read till first space is the instruction name 
    //for example let us have 
    std::string instruction1 = "irmovq 100 %rax";
    std::string instruction2 ="addq %rax %rax";
    //let us make it simple first


    int n = 0;
    instruction_t insts[2];
    instruction_t testInst = parseInput(instruction1);
    instruction_t testInst2 = parseInput(instruction2);
    insts[0] = testInst;
    insts[1] = testInst2;
    n = 2;
    
    runMySimulator(globalState, insts, n);
    printOutState(globalState);

    return 0;
}
