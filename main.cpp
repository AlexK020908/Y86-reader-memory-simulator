#include "sim.h"
#include <vector>
int main() {
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
    std::string instruction1 = "irmovq $15, %rax";
    //let us make it simple first


    /*
        typedef struct instruction
        {
            uint8_t rA;
            uint8_t rB;
            uint64_t valC;
            std::string name;
        } instruction_t;
    */
    int n = 0;
    instruction_t insts[1];
    instruction_t testInst;
    testInst.rA = 0xF; //NONE
    testInst.rB = 0; //rax 
    testInst.valC = 15;
    testInst.name = "irmovq";
    insts[0] = testInst;
    n = 1;
    
    runMySimulator(globalState, insts, n);
    printOutState(globalState);

    return 0;
}