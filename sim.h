#include <inttypes.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

//the implmentation is based on little endian
#define O 0x10   //overflow flag 
#define Z 0x40   //zero flag
#define S 0x80   //sign flag 

typedef struct state
{
        uint8_t memory[1024];   // Up to 1024 bytes of memory
        uint64_t start;    // Address at which memory starts
        uint64_t size;     // Number of bytes in memory that
                                // are valid
        uint64_t regs[16]; // Contents of the registers
        uint64_t pc;            // Program counter
        uint8_t flags;          // Holds O, Z, and S flags
} state_t;

typedef struct instruction
{
        uint8_t rA;
        uint8_t rB;
        uint64_t valC;
        char name[10];	// nul-terminated
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
inst_t inst_to_enum(const char *str);

//for testing purposes, maybe we want to check if two states are equal 
int is_equal(y86_state_t *s1, y86_state_t *s2);


int read_8_bits(y86_state_t *state, uint64_t address, uint64_t *value);

int write_8_bits(y86_state_t *state, uint64_t address, uint64_t value);

int runInstruction(y86_state_t * state, y86_inst_t * ins);

int runMySimulator(y86_state_t *state, y86_inst_t *instructions, int n_inst);


