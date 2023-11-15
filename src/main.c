#include <stdlib.h>

#include "cpu.h"

int main(int argc, char **argv)
{
  if(argc != 2){
    printf("Usage : %s [file to run]\n", argv[0]);
    return EXIT_FAILURE;
  }
  Cpu cpu;


  cpu_init(&cpu, argv[1]);


  printf("Registers\n");
  for (int i = 0; i < 32; ++i)
  {
    printf("x%d = %d\n", i, cpu.reg[i]);
  }
  while(cpu_step(&cpu) != CPU_EOP_ERROR){
    printf("Registers\n");
    for (int i = 0; i < 32; ++i)
    {
      printf("%s = %08x", reg_names[i], cpu.reg[i]);
      if((i+1)%6)
        printf("    ");
      else
        putchar('\n');
    }
  }

  free(cpu.ram;)

  return EXIT_SUCCESS;
}


