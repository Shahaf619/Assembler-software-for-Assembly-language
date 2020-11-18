
/*======================================================================================================================================*/
/*-----------------------------------------macros for defining length of RAM------------------------------------------------------------*/
/*======================================================================================================================================*/
#define SYMBOL_LENGTH 32
#define MAX_NAME_LENGTH 81
#define MAX_DATA_LENGTH 1024

/*======================================================================================================================================*/
/*-------------------------------------------defining symbols table structure-----------------------------------------------------------*/
/*======================================================================================================================================*/
typedef struct symbols * ptr_symbols;
typedef struct symbols
{
    int address;
    char name [MAX_NAME_LENGTH];
    int guide; /* 1 for numbers, 2 for string */
    int external;
    int entry;
    int string;
    int data [MAX_DATA_LENGTH];
    ptr_symbols next;
} symbols;


