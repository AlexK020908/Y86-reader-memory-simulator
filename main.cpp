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
        while(input[i] != ' ' && i < input.size()) {
            i++;
        }
        inst_map_t toreturn = inst_to_enum(input.substr(0, i));
        input = i + 1 < input.size() ? input.substr(i + 1) : "";
        return toreturn;
}
int readFirstArgAndCut(std::string & input) {
        int i = 0;
        
        while(input[i] != ' ' && i < input.size()) {
            i++;
        }
        int toreturn = NameToIndex[input.substr(0, i)];
        if(i + 1 < input.size()) input = input.substr(i + 1);
        else input = "";
        std::cout << "new input: " << input << std::endl;
        std::cout << "toreturn is: " << toreturn << std::endl;
        return toreturn;

}
int readValc(std::string & input) {
        int i = 0;
        int xexists = 0;

        
        while(input[i] != ' '&& i < input.size()) {
            if(input[i] == 'x') {
                xexists = 1;
            }
            i++;
        }
        std::string extractedNumber = input.substr(0, i);
        if(xexists) {
            extractedNumber = extractedNumber.substr(2);
        } else {
            extractedNumber = extractedNumber.substr(0);
        }
        std::cout << extractedNumber << std::endl;
        if(i + 1 < input.size()) input = input.substr(i + 1);
        else input = "";
        int toreturn = xexists ? stoi(extractedNumber, 0, 16) : stoi(extractedNumber);
        return toreturn;

}

int readOffset(std::string & input) {
    int i = 0;
    int start = 0;
    int end = 0;
    int xexists = 0;
    while(input[i] != '('&& i < input.size()) {
        if(input[i] == 'x') {
            xexists = 1;
        }
        i++;
    }
    start = i;
    int toreturn = xexists ? stoi(input.substr(0, i).substr(2), 0, 16) : stoi(input.substr(0, i));
    while(input[i] != ')'&& i < input.size()) {
        i++;
    }
    end = i;
    std::string extract = input.substr(start + 1, end - start - 1);
        if(i + 1 < input.size()) input = input.substr(i + 1);
        else input = "";
    input = extract + input;
    std::cout << "extracted: " << extract << std::endl;
    std::cout << "new input: " << input << std::endl;
    std::cout << "extracted offset: " << toreturn << std::endl;

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
                toReturn.rA = 15;
                toReturn.rB = 15;
                break;
            }
            case I_HALT:
            {          
                toReturn.name = "halt";    	        		

                break;
            }
            case I_RRMOVQ:
            {			
                input.erase(remove(input.begin(), input.end(), ','), input.end());
                toReturn.name = "rrmovq";
                toReturn.rA = readFirstArgAndCut(input);
                toReturn.rB = readFirstArgAndCut(input);

                break;
            }
            case I_IRMOVQ:
            {			
                input.erase(remove(input.begin(), input.end(), ','), input.end());
                toReturn.name = "irmovq";
                toReturn.valC = readValc(input);
                toReturn.rA = 15;
                toReturn.rB = readFirstArgAndCut(input);
                break;	
            }
            case I_RMMOVQ:
            {   	
                input.erase(remove(input.begin(), input.end(), ','), input.end());	
                toReturn.name = "rmmovq";
                toReturn.rA = readFirstArgAndCut(input);
                toReturn.valC = readOffset(input);
                toReturn.rB = readFirstArgAndCut(input);

                break;
            }
            case I_MRMOVQ:
            {		
                input.erase(remove(input.begin(), input.end(), ','), input.end());
                toReturn.name = "mrmovq";
                toReturn.valC = readOffset(input);
                toReturn.rB = readFirstArgAndCut(input);
                toReturn.rA = readFirstArgAndCut(input);
                // std::cout << "VALC: " << toReturn.valC << std::endl;
                // std::cout << "RA: " << toReturn.rA << std::endl;
                // std::cout << "Rb: " << toReturn.rB << std::endl;

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
                    input.erase(remove(input.begin(), input.end(), ','), input.end());
                toReturn.name = "addq";
                toReturn.rA = readFirstArgAndCut(input);
                toReturn.rB = readFirstArgAndCut(input);
                break;
            }
            case I_SUBQ:
            {			
                    input.erase(remove(input.begin(), input.end(), ','), input.end());
                toReturn.name = "subq";
                toReturn.rA = readFirstArgAndCut(input);
                toReturn.rB = readFirstArgAndCut(input);
                break;
            }
            case I_MULQ:
            {   	
                input.erase(remove(input.begin(), input.end(), ','), input.end());
                toReturn.name = "mulq";
                toReturn.rA = readFirstArgAndCut(input);
                toReturn.rB = readFirstArgAndCut(input);		
                break;
            }
            case I_MODQ:
                
            {		
                    input.erase(remove(input.begin(), input.end(), ','), input.end());

                toReturn.name = "modq";
                toReturn.rA = readFirstArgAndCut(input);
                toReturn.rB = readFirstArgAndCut(input);	
                break;
            }
            case I_DIVQ:
            {		
                    input.erase(remove(input.begin(), input.end(), ','), input.end());	
                toReturn.name = "divq";
                toReturn.rA = readFirstArgAndCut(input);
                toReturn.rB = readFirstArgAndCut(input);
                break;
            }	
            case I_ANDQ:
            {		
                    input.erase(remove(input.begin(), input.end(), ','), input.end());
                toReturn.name = "andq";
                toReturn.rA = readFirstArgAndCut(input);
                toReturn.rB = readFirstArgAndCut(input);	
                break;
            }
            case I_XORQ:
            {		
                    input.erase(remove(input.begin(), input.end(), ','), input.end());	
                toReturn.name = "xorq";
                toReturn.rA = readFirstArgAndCut(input);
                toReturn.rB = readFirstArgAndCut(input);
                break;
            }
            case I_CMOVEQ:
            {		
                    input.erase(remove(input.begin(), input.end(), ','), input.end());	
                toReturn.name = "cmoveq";
                toReturn.rA = readFirstArgAndCut(input);
                toReturn.rB = readFirstArgAndCut(input);
                break;
            }
             case I_CMOVNE:
            {		
                    input.erase(remove(input.begin(), input.end(), ','), input.end());	
                toReturn.name = "cmovne";
                toReturn.rA = readFirstArgAndCut(input);
                toReturn.rB = readFirstArgAndCut(input);
                break;
            }
        
            case I_CMOVL:
            {		
                    input.erase(remove(input.begin(), input.end(), ','), input.end());
                toReturn.name = "cmovl";
                toReturn.rA = readFirstArgAndCut(input);
                toReturn.rB = readFirstArgAndCut(input);	
                break;
            }
            case I_CMOVLE:
            {		
                    input.erase(remove(input.begin(), input.end(), ','), input.end());	
                toReturn.name = "cmovle";
                toReturn.rA = readFirstArgAndCut(input);
                toReturn.rB = readFirstArgAndCut(input);	
                break;	
            }
            case I_CMOVG:
            {			
                    input.erase(remove(input.begin(), input.end(), ','), input.end());
                toReturn.name = "cmovg";
                toReturn.rA = readFirstArgAndCut(input);
                toReturn.rB = readFirstArgAndCut(input);	
                break;
            }
            case I_CMOVGE:
            {		
                    input.erase(remove(input.begin(), input.end(), ','), input.end());
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
    globalState->size = 0x700;
    memset(globalState->R, 0, sizeof(globalState->R));
    globalState->pc = 0x1000;
    globalState->flags = 0x0;

    //printOutState(globalState);

    //make an instruction 
    //for example irmovq $15 %rax
    //we need to design a function to parse this input 
    //basically read till first space is the instruction name 
    //for example let us have 
    std::string instruction0 = "irmovq 0x1008, %rsp";
    std::string instruction1 = "irmovq 0x1024, %rax";
    std::string instruction2 = "irmovq 0x2ab3, %rbx";
    //let us make it simple first
    std::string instruction3 = "rmmovq %rbx, 0(%rax)";
    std::string instruction4 = "mrmovq 0(%rax), %rcx";
    std::string instruction5 = "irmovq -10, %r13";
    std::string instruction6 = "pushq %rbx";
    std::string instruction7 = "popq %r14";
    //checking ALU operations, add, sub, div, mod...
    std::string i8 = "irmovq 0x10, %r14";
    std::string i9 = "addq %r13, %r14";
    std::string i10 = "mulq %r14, %r14";



    /*
    check list
        irmovq works 
        addq works 
        rrmovq works 
        rmmovq works
        mrmovq works
        popq works
        pushq works
        jump/unconditional works 
        ALU ops?
        add: works
        subq: works
        divq: works
    */


    int n = 11;
    instruction_t insts[11];
    instruction_t testInst0 = parseInput(instruction0);
    instruction_t testInst = parseInput(instruction1);
    instruction_t testInst2 = parseInput(instruction2);
    instruction_t testInst3 = parseInput(instruction3);
    instruction_t testInst4 = parseInput(instruction4);
    instruction_t testInst5 = parseInput(instruction5);
    instruction_t testInst6 = parseInput(instruction6);
    instruction_t testinst7 = parseInput(instruction7);
    instruction_t testinst8 = parseInput(i8);
    instruction_t testinst9 = parseInput(i9);
    instruction_t testinst10 = parseInput(i10);
    
    insts[0] = testInst0;
    insts[1] = testInst;
    insts[2] = testInst2;
    insts[3] = testInst3;
    insts[4] = testInst4;
    insts[5] = testInst5;
    insts[6] = testInst6;
    insts[7] = testinst7;
    insts[8] = testinst8;
    insts[9] = testinst9;
    insts[10] = testinst10;
    
    
    runMySimulator(globalState, insts, n);
    printOutState(globalState);
    delete(globalState);
    return 0;
}
