#define MAX_SYMBOL_LENGTH 31
/*=======================================================================================================================================*/
/*---------------------------------------------------------input.c-----------------------------------------------------------------------*/
/*=======================================================================================================================================*/
int get_line ( FILE *, char [MAX_LINE_LENGTH],int, char * );
int check_length_line ( int );
int line_length ( FILE * );
int get_word ( char [MAX_LINE_LENGTH], char [MAX_LINE_LENGTH] );
void remove_whitespaces ( char * );
int delete_word ( char [MAX_LINE_LENGTH], char [MAX_LINE_LENGTH] );
void next_word ( char [MAX_LINE_LENGTH], char [MAX_LINE_LENGTH] );
int delete_word ( char [MAX_LINE_LENGTH], char [MAX_LINE_LENGTH] );
void delete_comma_end ( char * );
int check_num ( char [MAX_LINE_LENGTH], char [MAX_LINE_LENGTH], int [MAX_DATA_LENGTH], int *, int );
int add_spaces ( char [MAX_LINE_LENGTH] );
int arr_size ( int [MAX_DATA_LENGTH] );
int is_symbol ( char [MAX_LINE_LENGTH] );
int is_comma ( char [MAX_LINE_LENGTH] );
int is_number_comma_end ( char * );
int is_comment ( char [MAX_LINE_LENGTH] );
int is_symbol_without_colon ( char [MAX_LINE_LENGTH] );

/*=======================================================================================================================================*/
/*---------------------------------------------------------guide.c-----------------------------------------------------------------------*/
/*=======================================================================================================================================*/
int analyze_guideline ( int, char [MAX_LINE_LENGTH], char [MAX_LINE_LENGTH], int, ptr_symbols * sym,  char [MAX_LINE_LENGTH], char [MAX_DATA_LENGTH] [MAX_SYMBOL_LENGTH],int *);
void add_husky ( int [MAX_DATA_LENGTH], char [MAX_LINE_LENGTH] );
int is_string ( char [MAX_LINE_LENGTH] );
int check_comma_end ( char [MAX_LINE_LENGTH], char  [MAX_LINE_LENGTH], int  [MAX_DATA_LENGTH], int * , int );
void add_number ( int [MAX_DATA_LENGTH], char [MAX_LINE_LENGTH], int );
int is_guideline ( char [MAX_LINE_LENGTH] );
int analyze_entry ( char  [MAX_LINE_LENGTH], char [MAX_LINE_LENGTH], int );
int check_names_entry ( char [MAX_LINE_LENGTH], ptr_symbols * );

/*=======================================================================================================================================*/
/*---------------------------------------------------------file.c------------------------------------------------------------------------*/
/*=======================================================================================================================================*/
int make_object_file ( FILE *, char [], char [] );
int open_assembly_file ( FILE **, char [] );
int open_external_file ( FILE **, char [] );
int open_entry_file ( FILE **, char [] );

/*=======================================================================================================================================*/
/*--------------------------------------------------------encode.c-----------------------------------------------------------------------*/
/*=======================================================================================================================================*/
void convert_to_binary (byte, char[BYTE_LENGTH]);
void reset_bits (char [BYTE_LENGTH]);
int to_ascii (char);
void fill_object_file ( byte [RAM_LENGTH], FILE *, int, int );
int demical_first_instruction (unsigned, unsigned, unsigned );
int demical_secound_instruction (unsigned, unsigned );
int demical_first_immidiate_address_registers (unsigned, unsigned );
int demical_secound_immidiate_address_registers ( unsigned );
int demical_secound_registers ( unsigned, unsigned );
int demical_first_data ( unsigned );
int secound_demical_data ( unsigned  );

/*=======================================================================================================================================*/
/*-------------------------------------------------------symbols.c-----------------------------------------------------------------------*/
/*=======================================================================================================================================*/
void add_symbol ( ptr_symbols *, char [MAX_NAME_LENGTH], int, int,int, int * );
void save_symbol_name ( char [MAX_LINE_LENGTH], char [MAX_LINE_LENGTH] );
void free_symbols ( ptr_symbols * );
void print_symbol_table ( ptr_symbols );
int update_address ( byte [RAM_LENGTH], int, ptr_symbols ) ;
int check_entry ( ptr_symbols );
void fill_entry ( ptr_symbols, FILE * );

/*=======================================================================================================================================*/
/*--------------------------------------------------------ram.c--------------------------------------------------------------------------*/
/*=======================================================================================================================================*/
void encode_ram (byte [RAM_LENGTH], int, int, int);
void byte_status (byte [RAM_LENGTH],int, int);
void add_instruction (byte [RAM_LENGTH], int, unsigned, unsigned, unsigned, unsigned );
void add_immidiate (byte [RAM_LENGTH], int, unsigned, unsigned );
void add_address (byte [RAM_LENGTH], int, unsigned, unsigned );
void add_registers (byte [RAM_LENGTH], int, unsigned, unsigned, unsigned );
void add_data (byte [RAM_LENGTH], int, unsigned );
void add_registers_src (byte [RAM_LENGTH], int, unsigned, unsigned );
void add_registers_des (byte [RAM_LENGTH], int, unsigned, unsigned );
void print_ram ( byte [RAM_LENGTH], int ); /* @cheker@ */

/*=======================================================================================================================================*/
/*----------------------------------------------------command.c--------------------------------------------------------------------------*/
/*=======================================================================================================================================*/
int analyze_command ( char [MAX_LINE_LENGTH], char [MAX_LINE_LENGTH], const char * [MAX_COMMANDS], byte [RAM_LENGTH], int *, ptr_symbols, char [MAX_DATA_LENGTH] [MAX_SYMBOL_LENGTH], int, int, char [MAX_DATA_LENGTH] );
int is_command ( char [MAX_LINE_LENGTH], const char * [MAX_COMMANDS] );
int is_register_name ( char [MAX_LINE_LENGTH] );







