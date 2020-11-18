#include <stdio.h>
#include "ram.h"
#include "command.h"

/*=====================================================================================================================================*/
/*-------------------------------------------------encode instruction byte-------------------------------------------------------------*/
/*=====================================================================================================================================*/
void add_instruction (byte ram [RAM_LENGTH], int address, unsigned ARE, unsigned DES, unsigned OP, unsigned SRC)
{
    ram_in ( address, instruction, ARE ) = ARE;
    ram_in ( address, instruction, SRC ) = SRC;
    ram_in ( address, instruction, OP ) = OP;
    ram_in ( address, instruction, DES ) = DES;
    
}

/*=====================================================================================================================================*/
/*--------------------------------------------------encode immidiate byte--------------------------------------------------------------*/
/*=====================================================================================================================================*/
void add_immidiate (byte ram [RAM_LENGTH], int address, unsigned ARE, unsigned IMD )
{
    ram_in ( address, immidiate, ARE ) = ARE;
    ram_in ( address, immidiate, IMD ) = IMD;
}

/*=====================================================================================================================================*/
/*---------------------------------------------------encode address byte---------------------------------------------------------------*/
/*=====================================================================================================================================*/
void add_address (byte ram [RAM_LENGTH], int address, unsigned ARE, unsigned ADR )
{
    ram_in ( address, address, ARE ) = ARE;
    ram_in ( address ,address, ADR ) = ADR;
}

/*=====================================================================================================================================*/
/*---------------------------------------------encode source registers byte------------------------------------------------------------*/
/*=====================================================================================================================================*/
void add_registers_src (byte ram [RAM_LENGTH], int address, unsigned ARE, unsigned SRC_REG )
{
    ram_in ( address, registers, ARE ) = ARE;
    ram_in ( address, registers, SRC_REG ) = SRC_REG;
}

/*=====================================================================================================================================*/
/*-------------------------------------------encode destination registers byte---------------------------------------------------------*/
/*=====================================================================================================================================*/
void add_registers_des (byte ram [RAM_LENGTH], int address, unsigned ARE, unsigned DES_REG )
{
    ram_in ( address, registers, ARE ) = ARE;
    ram_in ( address, registers, DES_REG ) = DES_REG;
}

/*=====================================================================================================================================*/
/*-------------------------------------------------encode registers byte---------------------------------------------------------------*/
/*=====================================================================================================================================*/
void add_registers (byte ram [RAM_LENGTH], int address, unsigned ARE, unsigned SRC_REG, unsigned DES_REG )
{
    ram_in ( address, registers, ARE ) = ARE;
    ram_in ( address, registers, SRC_REG ) = SRC_REG;
    ram_in ( address, registers, DES_REG ) = DES_REG;
}

/*=====================================================================================================================================*/
/*---------------------------------------------------encode data byte------------------------------------------------------------------*/
/*=====================================================================================================================================*/
void add_data (byte ram [RAM_LENGTH], int address, unsigned DATA_VAL )
{ 
    ram_in ( address, data, DATA_VAL ) = DATA_VAL;
}

/*=====================================================================================================================================*/
/*----------------------------------------------function to print ram values-----------------------------------------------------------*/
/*=====================================================================================================================================*/
void print_ram ( byte ram [RAM_LENGTH], int end ) /* @cheker@ */
{
    int i;
    printf("==================================================================\n");
    for ( i = 100 ; i < end; i++ )
    {
        switch ( ram [i].which )
        {
             case INSTRUCTION_BYTE:
                                  printf("address: %d ", i );
                                  printf("ARE: %u ", ram [i].instruction_byte.ARE );
                                  printf("SRC: %u ", ram [i].instruction_byte.SRC );
                                  printf("OP: %u ", ram [i].instruction_byte.OP );
                                  printf("DES: %u\n", ram [i].instruction_byte.DES );
                                  break;

            case IMMEDIATE_BYTE:
                                  printf("address: %d ", i );
                                  printf("ARE: %u ", ram [i].immidiate_byte.ARE );
                                  printf("IMD: %d\n", ram [i].immidiate_byte.IMD );
                                  break;
 
           case ADDRESS_BYTE:
                                  printf("address: %d ", i );
                                  printf("ARE: %u ", ram [i].address_byte.ARE );
                                  printf("ADR: %u\n", ram [i].address_byte.ADR );
                                  break;

          case REGISTERS_BYTE:
                                  printf("address: %d ", i );
                                  printf("ARE: %u ", ram [i].registers_byte.ARE );
                                  printf("SRC_REG: %u ", ram [i].registers_byte.SRC_REG );
                                  printf("DES_REG: %u\n", ram [i].registers_byte.DES_REG );
                                  break;

          case DATA_BYTE: 
                                  printf("address: %d ", i );
                                  printf("DATA_VAL: %d\n", ram [i].data_byte.DATA_VAL );
                                  break;
        }
    }
    printf("==================================================================\n");
}








