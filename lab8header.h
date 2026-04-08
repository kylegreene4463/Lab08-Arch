/*
 * File:   lab8header.h
 * Author:
 */

#ifndef LAB8HEADER_H
#define LAB8HEADER_H

 // PCRegister is defined in lab8_9.c, declared extern here
extern unsigned int PCRegister;  // PC register, always pointing to the next instruction.

unsigned int CPU_fetchCode(char* mem, int codeOffset);
unsigned char CPU_Decode(unsigned int machineCode);
void CPU_Execution(unsigned char opcode, unsigned int machineCode, char* mem);
void printRegisterFiles();
void printDataMemoryDump(char* mem);  // mem parameter needed to call memory_dump

#endif /* LAB8HEADER_H */

