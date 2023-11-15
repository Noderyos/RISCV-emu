#ifndef CPU_H
#define CPU_H

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#define CPU_VALID 0 // Everything is valid
#define CPU_EOP_ERROR 1 // End Of Program

char *reg_names[] = {
	"zero", "ra  ", "sp  ", "gp  ", "tp  ", "t0  ", "t1  ", "t2  ",
	"s0  ", "s1  ", "a0  ", "a1  ", "a2  ", "a3  ", "a4  ", "a5  ", 
	"a6  ", "a7  ", "s2  ", "s3  ", "s4  ", "s5  ", "s6  ", "s7  ", 
	"s8  ", "s9  ", "s10 ", "s11 ", "t3  ", "t4  ", "t5  ", "t6  "
};

#define GEN_MASK(x) ((1<<x)-1)

typedef struct {
	uint32_t reg[32];
	uint32_t pc;
	uint32_t program_size;
	uint8_t *ram;
} Cpu;


void cpu_init(Cpu *cpu, char* filename);

int cpu_step(Cpu *cpu);

#endif