#include <inttypes.h>
#include <stdio.h>
#include <stdint.h>
#include <string>
#include <cstring>
#include <unordered_map>
#include <iostream>

//the implmentation is based on little endian
#define O 0x10   //overflow flag 
#define Z 0x40   //zero flag
#define S 0x80   //sign flag 

typedef struct state
{
        uint8_t m[1024];   // Up to 1024 bytes of memory
        uint64_t start;         // Address at which memory starts
        uint64_t size;          // Number of bytes in memory that
                                // are valid
        uint64_t R[16];      // Contents of the registers
        uint64_t pc;            // Program counter
        uint8_t flags;          // Holds O, Z, and S flags
} state_t;

typedef struct instruction
{
        uint8_t rA;
        uint8_t rB;
        uint64_t valC;
        std::string name;
} instruction_t;


/* enum types made for translation */
typedef enum instructionMap
{
        I_NOP,
        I_HALT,
        I_RRMOVQ,
        I_IRMOVQ,
        I_RMMOVQ,
        I_MRMOVQ,
        I_PUSHQ,
        I_POPQ,
        I_CALL,
        I_RET,
        I_J,
        I_JEQ,
        I_JNE,
        I_JL,
        I_JLE,
        I_JG,
        I_JGE,
        I_ADDQ,
        I_SUBQ,
        I_MULQ,
        I_MODQ,
        I_DIVQ,
        I_ANDQ,
        I_XORQ,
        I_CMOVEQ,
        I_CMOVNE,
        I_CMOVL,
        I_CMOVLE,
        I_CMOVG,
        I_CMOVGE,
        I_INVALID
} inst_map_t;


/* Takes name field instruction and convert to enum stated above */
inst_map_t inst_to_enum(std::string str);

//for testing purposes, maybe we want to check if two states are equal 
int is_equal(state_t *s1, state_t *s2);

int read_8_bits(state_t *state, uint64_t address, uint64_t *value);

int write_8_bits(state_t *state, uint64_t address, uint64_t value);

int runInstruction(state_t * state, instruction_t * ins);

int runMySimulator(state_t *state, instruction_t *instructions, int n_inst);

void initMap();

void printOutState(state_t * state);
