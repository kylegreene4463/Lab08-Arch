 /*
 ============================================================================
 Name        : Lab6.c
 Author      : Austin Tian
 Revised by  :
 Version     :
 Copyright   : Copyright 2023
 Description : Lab 6 in C, ANSI-C Style
 ============================================================================
 */

#define _CRT_SECURE_NO_WARNINGS
#include "header.h"

  // Global variable definitions (declared extern in header.h)
MIPS_Instruction Instruction_storage[MAX_SIZE];
int regFile[N_REG];
char Data_storage[MAX_SIZE];
unsigned int totalDataByte;
labelType labelTab[MAX_LABEL];

char* menu = "\n" \
" ***********Please select the following options**********************\n" \
" *    This is the memory operation menu                             *\n" \
" ********************************************************************\n" \
" *    1. Write a double-word (32-bit) to the memory                 *\n"  \
" ********************************************************************\n" \
" *    2. Read a byte (8-bit) data from the memory                   *\n" \
" *    3. Read a double-word (32-bit) data from the memory           *\n" \
" ********************************************************************\n" \
" *    4. Generate a memory dump from any memory location            *\n" \
" ********************************************************************\n" \
" *    e. To Exit, Type 'e'  or 'E'                                  *\n" \
" ********************************************************************\n";

unsigned char rand_generator()
{
    return rand() % 255;
}

void free_memory(char* base_address)
{
    free(base_address);
    return;
}

char* init_memory()
{
    char* mem = malloc(MEM_SIZE);
    int i = 0;
    for (i = 0; i < MEM_SIZE; i++) {
        *(mem + i) = 0;  // fill with 0 as required by lab 7
    }
    return mem;
}

void write_byte(char* base_address, int offset, unsigned char byte_data)
{
    *(base_address + offset) = byte_data;
}

void write_dword(char* base_address, int offset, unsigned int dword_data)
{
    unsigned int* data = (unsigned int*)(base_address + offset);
    *data = dword_data;
}

unsigned char read_byte(char* base_address, int offset)
{
    unsigned char data = *(base_address + offset);
    if (DEBUG_CODE)
        printf("-----The Byte at offset %X: %02X ------\n", offset, data);
    return data;
}

unsigned int read_dword(char* base_address, int offset)
{
    unsigned int* dataptr = (unsigned int*)(base_address + offset);
    if (DEBUG_CODE)
        printf("-----The 32-bit word at offset %X is: %08X ------\n", offset, *dataptr);
    return *dataptr;
}

void memory_dump(char* base_address, int offset, unsigned int dumpsize)
{
    unsigned char printout;
    int i = 0, j = 0;
    if (dumpsize < MIN_DUMP_SIZE || dumpsize > MEM_SIZE)
        dumpsize = MIN_DUMP_SIZE;

    printf("\n");
    for (i = 0; i < (int)(dumpsize / DUMP_LINE); i++)
    {
        printf("%X: ", (unsigned int)(base_address + offset + i * DUMP_LINE));
        for (j = 0; j < DUMP_LINE; j++)
        {
            printout = (unsigned char)(*(base_address + offset + i * DUMP_LINE + j));
            printf("%02X ", printout);
        }
        printf(" --- ");
        for (j = 0; j < DUMP_LINE; j++)
        {
            printout = (unsigned char)(*(base_address + offset + i * DUMP_LINE + j));
            if (printout > 0x20 && printout < 0x7E)
                printf("%c ", printout);
            else
                printf(". ");
        }
        printf("\n");
    }
    getchar();
    return;
}

void setup_memory()
{
    char* mem = init_memory();
    char options = 0;
    unsigned int offset, dumpsize;
    unsigned int dword_data;
    do {
        if (options != 0x0a)
        {
            puts(menu);
            printf("\nThe base address of your memory is: %Xh (HEX)\n", (unsigned int)mem);
            puts("Please make a selection:");
        }
        options = getchar();
        switch (options)
        {
        case '1':
            puts("Please input your memory's offset address (in HEX):");
            scanf("%x", (unsigned int*)&offset);
            puts("Please input your DOUBLE WORD data to be written (in HEX):");
            scanf("%x", (unsigned int*)&dword_data);
            write_dword(mem, offset, dword_data);
            continue;
        case '2':
            puts("Please input your memory's offset address (in HEX):");
            scanf("%x", &offset);
            read_byte(mem, offset);
            continue;
        case '3':
            puts("Please input your memory's offset address (in HEX):");
            scanf("%x", &offset);
            read_dword(mem, offset);
            continue;
        case '4':
            puts("Please input your memory's offset address (in HEX):");
            scanf("%x", &offset);
            puts("Please input the size of the memory to be dumped:");
            scanf("%d", &dumpsize);
            memory_dump(mem, offset, dumpsize);
            continue;
        case 'e':
        case 'E':
            puts("Code finished, exit now");
            free_memory(mem);
            getchar();
            return;
        default:
            continue;
        }
    } while (1);
    return;
}