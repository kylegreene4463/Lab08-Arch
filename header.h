/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

 /*
  * File:   header.h
  * Created on February 28, 2019, 8:47 PM
  */

#ifndef HEADER_H
#define HEADER_H

#include <string.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>

  // Windows compatibility fixes
#define _CRT_SECURE_NO_WARNINGS
#define strlwr _strlwr
#define Endian 0   // 0 = little endian (Windows x86 is always little endian)

#define MEM_SIZE 1024*1024   // reserve 1 M bytes to play.
#define MIN_DUMP_SIZE  256   // minimum size of the dump size is set to 256 bytes.
#define DUMP_LINE 16         // the size of the memory to be dumped on each row.

#define N_REG 34
#define MAX_SIZE 2*1024  // set storage of data and code as 2k instructions/byte each.
#define MAX_LABEL 256

#define REG_LO 32
#define REG_HI 33

#define STACK_SEGMENT_BASE (MAX_SIZE - 4)
#define DATA_SEGMENT_BASE 0
#define TEXT_SEGMENT_BASE 0

#define DEBUG_CODE   1

#define WORD_SIZE 4
#define DATASECTION 0x2000
#define CODESECTION 0x0000

/**
 * How an instruction is stored in the storage
 */
typedef struct MIPS_Instruction
{
    char instruction[8];
    int rd;
    int rs;
    int rt;
    int shamt;
    int function;
    union {
        int immediate;
        int address;
    };
    unsigned int machineCode;
}MIPS_Instruction;

// Declare as extern — defined once in parser.c
extern MIPS_Instruction Instruction_storage[MAX_SIZE];
extern int regFile[N_REG];
extern char Data_storage[MAX_SIZE];
extern unsigned int totalDataByte;

#define BUFMAX 128

enum segment { DATA = 1, TEXT = 2 };

typedef struct labelType
{
    char label[20];
    int addr;
    char type;
}labelType;

extern labelType labelTab[MAX_LABEL];

// Function declarations
void parse_MIPS(FILE* fp);
void setup_memory();
void loadCodeToMem(char* mem);
char* init_memory();
void free_memory(char* base_address);
unsigned char rand_generator();

void write_byte(char* base_address, int offset, unsigned char byte_data);
void write_dword(char* base_address, int offset, unsigned int dword_data);
unsigned char read_byte(char* base_address, int offset);
unsigned int read_dword(char* base_address, int offset);
void memory_dump(char* base_address, int offset, unsigned int dumpsize);
void CPU(char* mem);

#endif /* HEADER_H */