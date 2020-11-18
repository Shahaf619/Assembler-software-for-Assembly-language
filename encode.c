#include <stdio.h>
#include "ram.h"
#include "encode.h"
#include "data.h"
#include "command.h"
#include "input.h"

/*=====================================================================================================================================*/
/*--------------------------------------------function for encoding object file--------------------------------------------------------*/
/*=====================================================================================================================================*/
void fill_object_file ( byte ram [RAM_LENGTH], FILE * output_file, int IC, int DC )
{
     int i, to_encode_1, to_encode_2;
     char base_64_val_1, base_64_val_2;
     fprintf ( output_file , "%d %d\n", IC - 100, DC );
     for ( i = START; i < IC + DC; i++ )
     {
         switch ( ram [i].which )
        {
           /*---------------------------------------------------------------------------*/
           /*-----------------------ram is instruction byte-----------------------------*/
           /*---------------------------------------------------------------------------*/
           case INSTRUCTION_BYTE:
            to_encode_1 = demical_first_instruction ( ram_in (i, instruction, ARE), ram_in (i, instruction, DES), ram_in(0, instruction, OP) );
            to_encode_2 = demical_secound_instruction ( ram_in (i, instruction, OP), ram_in (i, instruction, SRC) );
            base_64_val_1 = base_64 [to_encode_1]; 
            base_64_val_2 = base_64 [to_encode_2]; 
            fprintf ( output_file , "%c%c\n", base_64_val_2, base_64_val_1 );
            break;

          /*---------------------------------------------------------------------------*/
          /*-------------------------ram is immidiate byte-----------------------------*/
          /*---------------------------------------------------------------------------*/
          case IMMEDIATE_BYTE:
            to_encode_1 = demical_first_immidiate_address_registers ( ram_in (i, immidiate, ARE), ram_in (i, immidiate, IMD) );
            to_encode_2 = demical_secound_immidiate_address_registers ( ram_in (i, immidiate, IMD) );
            base_64_val_1 = base_64 [to_encode_1]; 
            base_64_val_2 = base_64 [to_encode_2]; 
            fprintf ( output_file , "%c%c\n", base_64_val_2, base_64_val_1 );
            break;

         /*---------------------------------------------------------------------------*/
         /*---------------------------ram is address byte-----------------------------*/
         /*---------------------------------------------------------------------------*/
         case ADDRESS_BYTE:
          to_encode_1 = demical_first_immidiate_address_registers ( ram_in (i, address, ARE), ram_in (i, address, ADR) );
          to_encode_2 = demical_secound_immidiate_address_registers ( ram_in (i, address, ADR) );
          base_64_val_1 = base_64 [to_encode_1]; 
          base_64_val_2 = base_64 [to_encode_2]; 
          fprintf ( output_file , "%c%c\n", base_64_val_2, base_64_val_1 );
          break;

        /*---------------------------------------------------------------------------*/
        /*--------------------------ram is register byte-----------------------------*/
        /*---------------------------------------------------------------------------*/
        case REGISTERS_BYTE:
         to_encode_1 = demical_first_immidiate_address_registers ( ram_in (i, registers, ARE), ram_in (i, registers, DES_REG) );
         to_encode_2 = demical_secound_registers ( ram_in (i, registers, DES_REG), ram_in (i, registers, SRC_REG) );
         base_64_val_1 = base_64 [to_encode_1]; 
         base_64_val_2 = base_64 [to_encode_2]; 
         fprintf ( output_file , "%c%c\n", base_64_val_2, base_64_val_1 );
         break;

       /*---------------------------------------------------------------------------*/
       /*-----------------------------ram is data byte------------------------------*/
       /*---------------------------------------------------------------------------*/
       case DATA_BYTE: 
        to_encode_1 = demical_first_data ( ram_in (i, data, DATA_VAL) );
        to_encode_2 = secound_demical_data ( ram_in (i, data, DATA_VAL) );
        base_64_val_1 = base_64 [to_encode_1]; 
        base_64_val_2 = base_64 [to_encode_2]; 
        fprintf ( output_file , "%c%c\n", base_64_val_2, base_64_val_1, to_encode_2, to_encode_1 );
        break;
        
     }
  }   
} 
    
/*=====================================================================================================================================*/
/*-------------------------------------functions for converting bytes of ram as 6 bit portion-------------------------------------------*/
/*=====================================================================================================================================*/    
int demical_first_instruction (unsigned ARE, unsigned DES, unsigned OP )
{
     unsigned int mask;
     int i, sum = START, mult = TRUE;
     to_demical(ARE_SIZE,ARE,TRUE);
     to_demical(DES_SIZE,DES,TRUE);
     mask = 1;
     if (OP & mask)
         sum += mult;
    return sum;
}

int demical_secound_instruction (unsigned OP, unsigned SRC )
{
     unsigned int mask;
     int i, sum = START, mult = TRUE;
     to_demical(3,OP,2);
     to_demical(SRC_SIZE,SRC,TRUE);
     return sum;
}

int demical_first_immidiate_address_registers (unsigned ARE, unsigned IMD_ADR_DES )
{
     unsigned int mask;
     int i, sum = START, mult = TRUE;
     to_demical(ARE_SIZE,ARE,TRUE);
     to_demical(4,IMD_ADR_DES,1);
    return sum;
}

int demical_secound_immidiate_address_registers ( unsigned IMD_ADR_DES )
{
     unsigned int mask;
     int i, sum = START, mult = TRUE;
     
     to_demical(6,IMD_ADR_DES,16);
    return sum;
}

int demical_secound_registers ( unsigned DES_REG, unsigned SRC_REG )
{
     unsigned int mask;
     int i, sum = START, mult = TRUE;
     to_demical(1,DES_REG,16);
     to_demical(5,SRC_REG,1);
    return sum;
}

int demical_first_data ( unsigned DATA_VAL )
{
     unsigned int mask;
     int i, sum = START, mult = TRUE;
     to_demical(6,DATA_VAL,1);
    return sum;
}

int secound_demical_data ( unsigned DATA_VAL )
{
     unsigned int mask;
     int i, sum = START, mult = TRUE;
     to_demical(6,DATA_VAL,64); 
    return sum;
}



/*=====================================================================================================================================*/
/*------------------------------------------function to reset bits of byte-------------------------------------------------------------*/
/*=====================================================================================================================================*/
void reset_bits (char bits[BYTE_LENGTH])
{
    int i;
    for ( i = START; i < BYTE_LENGTH; i++ )
          bits[i] = BIT_VAL_0;
}

/*=====================================================================================================================================*/
/*-------------------------------------function to get ascii code represantaion--------------------------------------------------------*/
/*=====================================================================================================================================*/
int to_ascii ( char ch )
{
    int ascii_val;
    ascii_val = (int) ch;
    return ascii_val;
}

