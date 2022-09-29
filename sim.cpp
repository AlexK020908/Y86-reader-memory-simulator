#include <stdio.h>
#include "sim.h"
#include <stdlib.h>
#include <cstdlib>
long long maxpossible = 9223372036854775807;
long minpossible = 0xFFFFFFFF;
std::unordered_map<std::string, inst_map_t> nTe;
int is_equal(state_t *s1, state_t *s2){
	if (s1->start != s2->start) 	return 0;
	if (s1->size  != s2->size) 	return 0;

	for (int i = 0; i < s1->size; i++) if (s1->m[i] != s2->m[i]) return 0;
	
	for (int i = 0; i < 15; i++) if (s1->R[i] != s2->R[i]) return 0;
	
	if (s1->pc != s2->pc) return 0;

	uint8_t s1f = s1->flags & 0xd0;
	uint8_t s2f = s2->flags & 0xd0;
	if (s1f != s2f) return 0;

	return 1;


/*
The code still needs improvement --> here is my general understanding 
explanation: we only want to compare the set bits , namely the valid bits, the code still needs some changes 

say we have 
	0xff , which is 1111 1111 --> this represents ALL THREE flags are set: why?
		0x10 | 0x80 | 0x40 = 0xd0 , it is 1101 0000, so why is 0xff the same as 0xd0?
		the reason why 1111 1111 is the same as all three flags are set is because all the bits that must be set are set 
		1111 1111 
		^^ ^      --> ^ represents the bits the MUST be set to show all three flags are on! we don't care about the other one bits 

	so if we compare and ask if 0xff and 0xd0 are equal, since the number and position of 1 bits in 0xd0 are the same in 0xff, it is valid 

	visual :
				0xd0 :   1101 0000
 				         ^^ ^
						 || |
				0xff: 	 1111 1111   notice the overlap of one bits 


	so maybe we can check first if s1f is a valid flag combination , if it iss, compare the position set(1) bits in s1f with set bits in s2f, if valid ---> return true

	=============================== do this seperately with else if....
	then if s1f was not a valid flag combination, do the same with s2f
	if s2f has a valid flag, compare position of set(1) bits in s2f with set bits of 1's in s1f, return true if valid 

*/
}

int read_8_bits(state_t *state, uint64_t address, uint64_t *value) {
	//so we read 4 index
	int startaddr = address - state->start;
	*value = 0;
	if(address < state->start) return 0;
	if(address == 0) state->flags = Z;
	if(startaddr + 8 > state->size) return 0;
	//then we get value from little endian format
	int shift = 0; 
	for(int i = startaddr ; i < startaddr + 8 ; i++) {
		uint64_t twoBytes = (uint64_t)state->m[i];
		twoBytes = twoBytes << (shift * 8);
		*value = *value | twoBytes;
		shift++;
	}

	return 1;
}


int write_8_bits(state_t *state, uint64_t address, uint64_t value) {
	int startaddr = address - state->start;
	if(address < state->start) return 0;
	if(address == 0) state->flags = Z;
	if(startaddr + 8  > state->size) return 0;
	//then we get value from little endian format
	uint64_t shift = 0xff; 
	
	for(int i = 0 ; i < 8 ; i++) {
		uint64_t twoBytes = value & (shift << (i * 8));
		uint8_t valueToInsert = twoBytes >> (i * 8);
		state->m[startaddr + i] = valueToInsert;
	}
	return 1;
}

//0 on fail, 1 on success
int is_valid_address(state_t *state, uint64_t address) {
	if(address < state->start || address > state->start + state->size) return 0;
	return 1;
}

int is_valid_register(instruction_t* ins) {
	if(ins->rA < 0 || ins->rA > 15 || ins->rB < 0 || ins->rB > 15) return 0;
	return 1;
}

int is_signed(uint64_t value) {
	return ((value & 0x8000000000000000) > 0);
}

//0 on success, 1 on fail 
int runInstruction(state_t * state, instruction_t* ins) {
	//first let us get the instruction in string format 
	std::cout << "name: " << ins->name << std::endl;
	inst_map_t enumType = inst_to_enum(ins->name);
	std::cout << "enumtype: " << enumType << std::endl;
	
	switch(enumType) {
		case I_NOP:
{			state->pc += 1;
			return 0;
			break;
            }
		case I_HALT:
{			return 1;
			break;}
		case I_RRMOVQ:
{			if (!is_valid_register(ins)) return 1;
			state->R[ins->rB] = state->R[ins->rA];
			state->pc += 2;
			return 0;
			break;}
		case I_IRMOVQ:
{			if (!is_valid_register(ins)) return 1;
			state->R[ins->rB] = ins->valC;
			state->pc += 10;
			return 0;
			break;	}
		case I_RMMOVQ:
{			if (!is_valid_register(ins)) return 1;
			if (!is_valid_address(state, ins->valC + state->R[ins->rB])) return 1;
			write_8_bits(state, ins->valC + state->R[ins->rB], state->R[ins->rA]);
			state->pc += 10;
			return 0;
			break;}
		case I_MRMOVQ:
{			if (!is_valid_register(ins)) return 1;
			if (!is_valid_address(state, ins->valC + state->R[ins->rB])) return 1;
			read_8_bits(state, ins->valC + state->R[ins->rB], &state->R[ins->rA]);
			state->pc += 10;
			return 0;
			break;}
		case I_PUSHQ:
{			if (!is_valid_register(ins)) return 1;
			if (!is_valid_address(state, state->R[4] - 8)) return 1;
			write_8_bits(state, state->R[4] - 8, state->R[ins->rA]);
			state->R[4] -= 8;
			state->pc += 2;
			return 0;
			break;}
		case I_POPQ:
{			if (!is_valid_register(ins)) return 1;
			if (!is_valid_address(state, state->R[4])) return 1;
			read_8_bits(state, state->R[4], &state->R[ins->rA]);
			state->R[4] += 8;
			state->pc += 2;
			return 0;
			break;}
		case I_CALL:
{			if (!is_valid_address(state, state->R[4] - 8)) return 1;
			write_8_bits(state, state->R[4] - 8, state->pc + 9);
			state->R[4] -= 8;
			state->pc = ins->valC;
			return 0;
			break;}
		case I_RET:
{			if (!is_valid_address(state, state->R[4])) return 1;
			read_8_bits(state, state->R[4], &state->pc);
			state->R[4] += 8;
			return 0;
			break;	}
		case I_J:
{			state->pc = ins->valC;
			return 0;
			break;}
		case I_JEQ:
{			if(state->flags == Z) {
				state->pc = ins->valC;
			} else {
				state->pc += 9;
			}
			return 0;
			break;}
		case I_JNE:
{			if(state->flags != Z) {
				state->pc = ins->valC;
			} else {
				state->pc += 9;
			}
			return 0;
			break;}
		case I_JL:
{
			if(state->flags == S) {
				state->pc = ins->valC;
			} else {
				state->pc += 9;
			}
			return 0;
			break;}
		case I_JLE:
{			if(state->flags == S || state->flags == Z) {
				state->pc = ins->valC;
			} else {
				state->pc += 9;
			}
			return 0;
			break;}
		case I_JG:
{			if(state->flags != S && state->flags != Z) {
				state->pc = ins->valC;
			} else {
				state->pc += 9;
			}
			return 0;
			break;	}
		case I_JGE:
{			if(state->flags != S || state->flags == Z) {
				state->pc = ins->valC;
			} else {
				state->pc += 9;
			}
			return 0;
			break;}
		case I_ADDQ:
{			state->flags = 0;
			uint64_t rA = state->R[ins->rA];
			uint64_t rB = state->R[ins->rB];
	        if(!is_signed(rB) && !is_signed(rA)) {
				if (maxpossible - rA < rB) state->flags = S;
			} else if(is_signed(rB) && is_signed(rA)) {
				if (minpossible - rA > rB) state->flags = S;
			}
			
			state->R[ins->rB] = rB + rA;
			if (is_signed(state->R[ins->rB])) state->flags = S;
			else if (state->R[ins->rB] == 0) state->flags = Z;
			state->pc += 2;
			return 0;
			break;}
		case I_SUBQ:
{			state->flags = 0;
			uint64_t rA_1 = state->R[ins->rA];
			uint64_t rB_1 = state->R[ins->rB];
	        if(rB_1 >= 0 && rA_1 < 0) {
				if (maxpossible - abs((int)rA_1) < rB_1) state->flags = S;
			} else if(rB_1 < 0 && rA_1 > 0) {
				if (minpossible + rA_1 > rB_1) state->flags = S;
			}
	
			state->R[ins->rB] = state->R[ins->rB] - state->R[ins->rA];
			if (is_signed(state->R[ins->rB])) state->flags = S;
			else if (rA_1 == rB_1) state->flags = Z;
			state->pc += 2;
			return 0;
			break;}
		case I_MULQ:
{			//add cases for overflow
			state->flags = 0;
			state->R[ins->rB] = state->R[ins->rB] * state->R[ins->rA];
			if (is_signed(state->R[ins->rB])) state->flags = S;
			else if (state->R[ins->rB] == 0) state->flags = Z;
			state->pc += 2;
			return 0;
			break;}
		case I_MODQ:
			
{			if (state->R[ins->rA] == 0 && state->R[ins->rB] == 0) {
				state->flags = Z;
				return 1;
			}
			if (state->R[ins->rA] == 0) {
				return 1;
			}
			state->flags = 0;
			state->R[ins->rB] = (state->R[ins->rB]) % (state->R[ins->rA]);
			if (is_signed(state->R[ins->rB])) state->flags = S;
			else if (state->R[ins->rB] == 0) state->flags = Z;
			state->pc += 2;
			return 0;
			break;}
		case I_DIVQ:
{			if(state->R[ins->rA] == 0 && state->R[ins->rB] == 0) {
				state->flags = Z;
				return 1;
			}
			else if (state->R[ins->rA] == 0) return 1;
			state->flags = 0;
			state->R[ins->rB] = state->R[ins->rB] / state->R[ins->rA];
			if (is_signed(state->R[ins->rB])) state->flags = S;
			else if (state->R[ins->rB] == 0) state->flags = Z;
			state->pc += 2;
			return 0;
			break;}	
		case I_ANDQ:
{			state->flags = 0;
			state->R[ins->rB] = (state->R[ins->rB]) & (state->R[ins->rA]);
			if (is_signed(state->R[ins->rB])) state->flags = S;
			else if (state->R[ins->rB] == 0) state->flags = Z;
			state->pc += 2;
			return 0;
			break;}
		case I_XORQ:
{			state->flags = 0;
			state->R[ins->rB] = (state->R[ins->rB]) ^ (state->R[ins->rA]);
			if (is_signed(state->R[ins->rB])) state->flags = S;
			else if (state->R[ins->rB] == 0) state->flags = Z;
			state->pc += 2;
			return 0;
			break;}
		case I_CMOVEQ:
{			if (state->flags == Z) state->R[ins->rB] = state->R[ins->rA];
			state->pc += 2;
			return 0;
			break;}
		case I_CMOVNE:
{			if (state->flags != Z) state->R[ins->rB] = state->R[ins->rA];
			state->pc += 2;
			return 0;
			break;}
		case I_CMOVL:
{			if (state->flags == S) state->R[ins->rB] = state->R[ins->rA];
			state->pc += 2;
			return 0;
			break;}
		case I_CMOVLE:
{			if (state->flags == S || state->flags == Z) state->R[ins->rB] = state->R[ins->rA];
			state->pc += 2;
			return 0;
			break;	}
		case I_CMOVG:
{			if (state->flags != S && state->flags != Z) state->R[ins->rB] = state->R[ins->rA];
			state->pc += 2;
			return 0;
			break;}
		case I_CMOVGE:
{			if (state->flags != S || state->flags == Z) state->R[ins->rB] = state->R[ins->rA];
			state->pc += 2;
			return 0;
			break;}
		case I_INVALID:
{			return 1;
			break;}
	}
	return 1;

}
//returns 1 on fail 0 on success
int runMySimulator(state_t *state, instruction_t *instructions, int n_inst) {
	instruction_t * currentInstruction = instructions;
	int currNumber = 0;
	while(currNumber < n_inst) {
		if(runInstruction(state, currentInstruction)) return 1;
		currentInstruction++;
		currNumber++;
	}
	return 0;
}


/* Takes name field instruction and convert to enum stated above */
inst_map_t inst_to_enum(std::string str) {
    return nTe[str];
}

 void initMap() {
     nTe["nop"] = I_NOP;
    nTe["halt"] = I_HALT;
    nTe["rrmovq"] = I_RRMOVQ;
    nTe["irmovq"] = I_IRMOVQ;
    nTe["rmmovq"] = I_RMMOVQ;
    nTe["mrmovq"] = I_MRMOVQ;
    nTe["pushq"] = I_PUSHQ;
    nTe["popq"] = I_POPQ;
    nTe["call"] = I_CALL;
    nTe["ret"] = I_RET;
    nTe["j"] = I_J;
    nTe["jeq"] = I_JEQ;
    nTe["jne"] = I_JNE;
    nTe["jl"] = I_JL;
    nTe["jle"] = I_JLE;
    nTe["jeq"] = I_JEQ;
    nTe["jg"] = I_JG;
    nTe["jge"] = I_JGE;
    nTe["addq"] = I_ADDQ;
    nTe["subq"] = I_SUBQ;
    nTe["mulq"] = I_MULQ;
    nTe["modq"] = I_MODQ;
    nTe["divq"] = I_DIVQ;
    nTe["andq"] = I_ANDQ;
    nTe["xorq"] = I_XORQ;
    nTe["cmoveq"] = I_CMOVEQ;
    nTe["cmovne"] = I_CMOVNE;
    nTe["cmovl"] = I_CMOVL;
    nTe["cmovle"] = I_CMOVLE;
    nTe["cmovg"] = I_CMOVG;
    nTe["cmovge"] = I_CMOVGE;
 }

void printOutState(state_t * state) {
	//first print out start address
	std::cout << "start address: " <<  std::hex << state->start << std::endl;
	//print out allowed size 
	std::cout << "Allowed size: " <<  std::hex << state->size << std::endl;
	//print out pc 
	std::cout << "PC:" <<  std::hex << state->pc<< std::endl;
	//print out all registers 
    std::cout << "Register values : " ;
	for(int i = 0 ; i < 16 ; i++) {
		std::cout << std::hex << state->R[i] << " ";
	}
	//print out the flag 
	std::cout << std::endl;
    std::string flagValue = "";
    if(state->flags == 0x0) flagValue = "---";
    else if(state->flags == O) flagValue = "--O";
    else if(state->flags == S) flagValue = "S--";
    else if(state->flags == Z) flagValue = "-Z-";
    else if (state->flags == (O | S)) flagValue = "S-O";
    else if (state->flags == (O | Z)) flagValue = "-ZO";
    else if (state->flags == (Z | S)) flagValue = "SZ-";
    else if (state->flags == (O | S | Z)) flagValue = "SZO";
	std::cout << "FLAGS: " << flagValue << std::endl;
	std::cout << "==============================================================================" << std::endl;
	std::cout << "Memeory:  ";
	for(int i = 0 ; i < 1024 ; i++) {
        char hex_string[16];
        sprintf(hex_string, "%X", state->m[i]); //convert number to hex
        std::cout << "0x" << hex_string << " ";
		
	}
    std::cout << std::endl;


}