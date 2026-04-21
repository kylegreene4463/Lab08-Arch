/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
 // Please finish the following functions for lab 8.
 // Lab 8 will perform the following functions:
 //   1. Fetch the code stored in memory
 //   2. Decode the code and prepare for the execution of the code.
 //   3. Setup the execution function for CPU.

 // Lab 9 will perform the following functions:
 //   4. Execute the code stored in the memory and print the results. 
#include "header.h"
#include "lab8header.h"

unsigned int PCRegister = 0;  // PC register definition (declared extern in lab8header.h)

extern char* regNameTab[N_REG];

void CPU(unsigned char* mem) {
    unsigned int machineCode = 0;
    unsigned char opcode = 0;
    PCRegister = CODESECTION;  // at the beginning, PCRegister is the starting point,
    // it should be a global integer defined in header.h
    do {
        printf("\nPC:%x\n", PCRegister);
        machineCode = CPU_fetchCode(mem, PCRegister);
        if (machineCode == 0)  // quit the program when machineCode is 0, that is the end of the code.
            break;  // break the infinite loop. 
        PCRegister += 4;                                                     // update the program counter
        opcode = CPU_Decode(machineCode);
        printf("Decoded Opcode is: %02X. \n", opcode);

        // Lab 9: Finish the execution of the code.
        // Only finish this part when the CPU_Decode is done.
    CPU_Execution(opcode, machineCode, mem);
    } while (1);  // This is an infinite while loop
    // When you fetch a machineCode of 00000000, the loop breaks.
    printRegisterFiles();     // After the code execution, print all the register contents on screen.
    printDataMemoryDump(mem); // After the code execution, print the memory dump of the data section.
}

// Lab 8 - Step 1. Finish the CPU_fectchCode function to
//         read the code section from memory and
//         get the 32-bit machine code from the memory.
unsigned int CPU_fetchCode(char* mem, int codeOffset) {
    // Use read_dword to fetch the 32-bit instruction at the current PC offset
    return read_dword(mem, codeOffset);
}

// Lab 8 - Step 2. Finish the CPU_Decode function to
//         decode the instruction and return the
//         opcode/function of the instruction.
//         Hints: Need to consider how to find the opcode/function from different types of instructions:
//                i.e., I-, J- and R-type instructions. 
//                The return value should be a 6-bit bianry code. 
unsigned char CPU_Decode(unsigned int machineCode) {
    // Just return the top 6 bits. 
    // This is the standard "Opcode" the lab manual is looking for.
    return (machineCode >> 26) & 0x3F;
}

// Lab 9: Finish the function CPU_Execution to run all the instructions.
void CPU_Execution(unsigned char opcode, unsigned int machineCode, char* mem) {
    unsigned char rs, rt, rd, funct;
    short immediate;
    unsigned int target;

    // Decoding common fields for I-type and R-type
    rs = (machineCode >> 21) & 0x1F;
    rt = (machineCode >> 16) & 0x1F;
    rd = (machineCode >> 11) & 0x1F;
    funct = machineCode & 0x3F;
    immediate = (short)(machineCode & 0xFFFF); // Cast to short for sign extension
    target = (machineCode & 0x03FFFFFF);      // 26-bit target for J-type

    switch (opcode) {
    case 0x00: // R-Type instructions use opcode 0
        if (funct == 0x20) { // "add" instruction
            regFile[rd] = regFile[rs] + regFile[rt];
        }
        break;

    case 0b101111: // "la" (Pseudo-instruction handled as I-type in this lab) [cite: 10]
        regFile[rt] = machineCode & 0x0000FFFF;
        break;

    case 0b100000: // "lb" (load byte)
        regFile[rt] = (signed char)mem[regFile[rs] + immediate];
        break;

    case 0b100011: // "lw" (load word)
        regFile[rt] = read_dword(mem, regFile[rs] + immediate);
        break;

    case 0b101011: // "sw" (store word)
        write_dword(mem, regFile[rs] + immediate, regFile[rt]);
        break;

    case 0x01: // "bge" instruction
        // Compare the registers (rs and rt)
        // If rs is greater than or equal to rt, branch to the offset
        if (regFile[rs] >= regFile[rt]) {
            // Note: PCRegister was already incremented by 4 in the CPU loop,
            // so we add the offset to the updated PC.
            PCRegister += (immediate << 2);
        }
        break;

    case 0b001000: // "addi" (add immediate)
        regFile[rt] = regFile[rs] + immediate;
        break;

    case 0b000100: // "beq" (Used for bge logic in some MIPS variants)
        if (regFile[rs] >= regFile[rt]) {
            PCRegister += (immediate << 2);
        }
        break;

    case 0b000010: // "j" (jump)
        PCRegister = (PCRegister & 0xF0000000) | (target << 2);
        return; // Return early to prevent the default PC+4 increment if applicable

    default:
        printf("Wrong instruction! %02X %08X\n", opcode, machineCode);
        system("PAUSE");
        exit(3);
        break;
    }

    if (DEBUG_CODE) {
        printf("Code Executed: %08X\n", machineCode);
    }
}

// Lab 8 - Step 3. Print all the 32 registers in regFile and names saved in
//         regNameTab. For example, it should print
//         $zero = 0x00000000
//         $at  = ... ... etc.
void printRegisterFiles() {
    int i;
    printf("\n---- Register File ----\n");
    // Loop through all N_REG registers and print name and value
    for (i = 0; i < N_REG; i++) {
        printf("%s = 0x%08X\n", regNameTab[i], (unsigned int)regFile[i]);
    }
}

// Lab 8 - Step 4. Call function memory_dump and pass the proper parameters to dump the first 256
//          bytes from Data section.
void printDataMemoryDump(char* mem) {
    printf("\n---- Data Memory Dump ----\n");
    // Dump 256 bytes starting from the data section offset (0x2000)
    memory_dump(mem, DATASECTION, 256);
}
