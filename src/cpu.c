#include "cpu.h"

void cpu_init(Cpu *cpu, char* filename){
	FILE* f = fopen(filename, "rb");

	// Getting file size
	fseek(f, 0, SEEK_END);
	long sz = ftell(f);
	fseek(f, 0, SEEK_SET);

	for (int i = 0; i < 32; ++i)
	{
		cpu->reg[i] = 0;
	}

	cpu->pc = 0;
	cpu->program_size = sz;
	cpu->ram = malloc(64*1024*1024);

	fread(cpu->ram, 1, sz, f);

	fclose(f); 
}

int cpu_step(Cpu *cpu){
	if(cpu->pc >= cpu->program_size)
		return CPU_EOP_ERROR;

	if(cpu->ram[cpu->pc]&0b11 == 3){ // Classic instruction
		uint32_t inst = *(uint32_t*)&cpu->ram[cpu->pc];
		switch(inst & 0x7f){
			case 0b0110011: // R instruction format
				uint32_t rd = (inst >> 7) & GEN_MASK(5);
				uint32_t rs1 = (inst >> 15) & GEN_MASK(5);
				uint32_t rs2 = (inst >> 20) & GEN_MASK(5);
				uint32_t f7 = (inst >> 12) & GEN_MASK(3);
				switch((inst >> 12) & GEN_MASK(3)){
					case 0x0:
						if(f7){
							printf("SUB\n");
							cpu->reg[rd] = cpu->reg[rs1] - cpu->reg[rs2];
						}else{
							printf("ADD\n");
							cpu->reg[rd] = cpu->reg[rs1] + cpu->reg[rs2];
						}
						break;
					case 0x4:
						printf("XOR\n");
						cpu->reg[rd] = cpu->reg[rs1] ^ cpu->reg[rs2];
						break;
					case 0x6:
						printf("OR\n");
						cpu->reg[rd] = cpu->reg[rs1] | cpu->reg[rs2];
						break;
					case 0x7:
						printf("AND\n");
						cpu->reg[rd] = cpu->reg[rs1] & cpu->reg[rs2];
						break;
					case 0x1:
						printf("SLL\n");
						cpu->reg[rd] = cpu->reg[rs1] << cpu->reg[rs2];
						break;
					case 0x5:
						uint32_t r1val = cpu->reg[rs1];
						uint32_t r2val = cpu->reg[rs2];
						cpu->reg[rd] = r1val >> r2val;
						if(f7 && r1val>>31){
							cpu->reg[rd] |= GEN_MASK(r2val) << (32-r2val);
							printf("SRA\n");
						}else{
							printf("SRL\n");
						}
						break;
					case 0x2:
					case 0x3:
							printf("SLT/U\n");
							cpu->reg[rd] = (cpu->reg[rs1] < cpu->reg[rs2]) ? 1: 0;
						break;
					default:
						printf("UNKNOWN");
				}
				break;
			case 0b0010011: // I instruction format
				uint32_t rrd = (inst >> 7) & GEN_MASK(5);
				uint32_t rrs1 = (inst >> 15) & GEN_MASK(5);
				uint32_t rrs2 = (inst >> 20) & GEN_MASK(12);
				uint32_t rf7 = (inst >> 12) & GEN_MASK(3);
				switch((inst >> 12) & GEN_MASK(3)){
					case 0x0:
						printf("ADDI\n");
						cpu->reg[rrd] = cpu->reg[rrs1] + rrs2;
						break;
					case 0x4:
						printf("XORI\n");
						cpu->reg[rrd] = cpu->reg[rrs1] ^ rrs2;
						break;
					case 0x6:
						printf("XORI\n");
						cpu->reg[rrd] = cpu->reg[rrs1] | rrs2;
						break;
					case 0x7:
						printf("ANDI\n");
						cpu->reg[rrd] = cpu->reg[rrs1] & rrs2;
						break;
					case 0x1:
						printf("SLLI\n");
						cpu->reg[rrd] = cpu->reg[rrs1] << (rrs2&0x1f);
						break;
					case 0x5:
						uint32_t r1val = cpu->reg[rrs1];
						uint32_t r2val = rrs2&0x1f;
						cpu->reg[rrd] = r1val >> r2val;
						if(rf7 && r1val>>31){
							cpu->reg[rrd] |= GEN_MASK(r2val) << (32-r2val);
							printf("SRAI\n");
						}else{
							printf("SRLI\n");
						}
						break;
					case 0x2:
					case 0x3:
							printf("SLTI/U");
							cpu->reg[rrd] = (cpu->reg[rrs1] < rrs2) ? 1: 0;
						break;
					default:
						printf("UNKNOWN");
				}
				break;
			case 0b0000011:
				switch((inst >> 12) & GEN_MASK(3)){
					case 0x0:
					case 0x4:
						break;
					case 0x1:
					case 0x5:
						break;
					case 0x2:
						break;
				}
				break;
		}
		cpu->pc += 4;
	}else { // Compressed instruction
		uint16_t inst = *(uint16_t*)&cpu->program[cpu->pc];
		cpu->pc += 2;
	}

	return CPU_VALID;
}