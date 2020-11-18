
/*=====================================================================================================================================*/
/*-----------------------------------------macros for defining length of RAM-----------------------------------------------------------*/
/*=====================================================================================================================================*/
#define RAM_LENGTH 1024
#define BYTE_LENGTH 12
#define MAX_SYMBOL_COMMAND_LENGTH 32


/*=====================================================================================================================================*/
/*-----------------------------------------------defining byte structure---------------------------------------------------------------*/
/*=====================================================================================================================================*/
typedef struct instruction
{
    unsigned ARE:2;  
    unsigned DES:3;
    unsigned OP:4;
    unsigned SRC:3;
} instruction;

typedef struct immidiate
{
   unsigned ARE:2;
   int IMD:10;
} immidiate;

typedef struct address
{
   unsigned ARE:2;
   unsigned ADR:10;
} address;

typedef struct registers
{
   unsigned ARE:2;    
   unsigned SRC_REG:5;
   unsigned DES_REG:5;
} registers;

typedef struct data
{
    int DATA_VAL:12; /* include negative numbers */
} data;

typedef struct byte
{
    int which;
    char command_symbol_name [MAX_SYMBOL_COMMAND_LENGTH];
    instruction instruction_byte;
    immidiate immidiate_byte;
    address address_byte;
    registers registers_byte;
    data data_byte;
} byte;

/*=======================================================================================================================================*/
/*----------------------------------------------macro to get short access in ram---------------------------------------------------------*/
/*=======================================================================================================================================*/
#define ram_in( address, union, var ) ram [address]. union ##_byte. var 



/*=======================================================================================================================================*/
/*--------------------------------------------relate byte sectors to numbers-------------------------------------------------------------*/
/*=======================================================================================================================================*/
#define ENC_ARE 0
#define ENC_DES 1
#define ENC_OP 2
#define ENC_SRC 3
#define ENC_IMD 4
#define ENC_ADR 5
#define ENC_REG_SRC 6
#define ENC_REG_DES 7
#define ENC_VALUE 8

