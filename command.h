/*=======================================================================================================================================*/
/*--------------------------------------------macros for defining array of commands------------------------------------------------------*/
/*=======================================================================================================================================*/
#define MAX_COMMANDS 16
#define MAX_REGISTERS 8
#define TEMPORARY 0
#define TWO_BYTES 2
#define THREE_BYTES 3
#define NO_ADDRESSING 0

/*=======================================================================================================================================*/
/*----------------------------------------------identifiers numbers for commands---------------------------------------------------------*/
/*=======================================================================================================================================*/
#define MOV 0
#define CMP 1
#define ADD 2
#define SUB 3
#define NOT 4
#define CLR 5
#define LEA 6
#define INC 7
#define DEC 8
#define JMP 9
#define BNE 10
#define RED 11
#define PRN 12
#define JSR 13
#define RTS 14
#define STOP 15

/*=======================================================================================================================================*/
/*-------------------------------------------macros for encoding SRC, DES fields---------------------------------------------------------*/
/*=======================================================================================================================================*/
#define IMMEDIATE 1
#define DIRECT 3
#define REGISTER 5

/*=======================================================================================================================================*/
/*---------------------------------------------------macros for ARE values---------------------------------------------------------------*/
/*=======================================================================================================================================*/
#define ABSOLUTE 0
#define EXTERNAL 1
#define RELOCATABLE 2

/*=======================================================================================================================================*/
/*---------------------------------------------------identifiers numbers for ram---------------------------------------------------------*/
/*=======================================================================================================================================*/
#define INSTRUCTION_BYTE 1
#define IMMEDIATE_BYTE 2
#define ADDRESS_BYTE 3
#define REGISTERS_BYTE 4
#define DATA_BYTE 5

/*=======================================================================================================================================*/
/*----------------------------------------------------macros for registers names---------------------------------------------------------*/
/*=======================================================================================================================================*/
#define REGISTER_0 "@r0"
#define REGISTER_1 "@r1"
#define REGISTER_2 "@r2"
#define REGISTER_3 "@r3"
#define REGISTER_4 "@r4"
#define REGISTER_5 "@r5"
#define REGISTER_6 "@r6"
#define REGISTER_7 "@r7"

/*=======================================================================================================================================*/
/*-------------------------------------------------macros for encoding register byte-----------------------------------------------------*/
/*=======================================================================================================================================*/
#define REG_0 0
#define REG_1 1
#define REG_2 2
#define REG_3 3
#define REG_4 4
#define REG_5 5
#define REG_6 6
#define REG_7 7

/*=======================================================================================================================================*/
/*--------------------------------------------------macro for is_command function--------------------------------------------------------*/
/*=======================================================================================================================================*/
#define is_command_mac(cmd,ret)\
if ( strcmp ( word, cmd ) == 0 )\
          return ret

/*=======================================================================================================================================*/
/*----------------------------------------------macro for is_register_name function------------------------------------------------------*/
/*=======================================================================================================================================*/
#define is_register_name_mac( reg_name, reg_num )\
if ( strcmp ( word, reg_name ) == 0 )\
             return reg_num 

/*=======================================================================================================================================*/
/*----------------------------------------------macro for analyze_command function-------------------------------------------------------*/
/*=======================================================================================================================================*/

/*---------------------------------------------------------------------------*/
/*-------------------------should be end of input----------------------------*/
/*---------------------------------------------------------------------------*/
#define unnecessary_input_mac() \
next_word ( line, word );\
if ( get_word ( line, word ) == TRUE )\
{ \
     printf( "error at line %d: unnecessary input after destination operand\n", line_number );\
     return FALSE;\
} 

/*=======================================================================================================================================*/
/*----------------------------------------------macro for analyze_command function-------------------------------------------------------*/
/*=======================================================================================================================================*/
#define encode_mac(operand, addressing, up, ARE, address, func, wh)\
operand = addressing; \
ram [ (*IC) + up ].which = wh; /* sign to ram that address byte is encoded */\
func ( ram, (*IC) + up, ARE, address );\
success = TRUE; /* source operand is legal */ 

/*=======================================================================================================================================*/
/*----------------------------------------------macro for analyze_command function-------------------------------------------------------*/
/*=======================================================================================================================================*/

/*---------------------------------------------------------------------------*/
/*--------increment IC depanding on how many bytes is already encoded--------*/
/*---------------------------------------------------------------------------*/
#define increment_IC_mac()\
if ( register_source == TRUE && register_destination == TRUE || one_operand == TRUE )\
     *IC = *IC + TWO_BYTES;\
else\
     *IC = *IC + THREE_BYTES;   

#define error_mac(comm)\
                            else /* no more input, error */\
                            {\
                                 printf( "error at line %d: missing destination operand for " #comm " command\n", line_number );\
                                 return FALSE;\
                            }\
                        }\
\
                        else /* no more input, error */\
                        {\
                              printf( "error at line %d: missing parameters for " #comm " command\n", line_number );\
                              return FALSE;\
                        }



/*=======================================================================================================================================*/
/*----------------------------------------------macro for analyze_command function-------------------------------------------------------*/
/*=======================================================================================================================================*/
#define all_addressing_source_mac(command)\
                        OP = command; /* for encoding first byte of command */\
                        /*---------------------------------------------------------------------------*/\
                        /*------------------------should be source operand---------------------------*/\
                        /*---------------------------------------------------------------------------*/\
                        next_word ( line, word );\
                     \
                        /*---------------------------------------------------------------------------*/\
                        /*---------------------------there is more input-----------------------------*/\
                        /*---------------------------------------------------------------------------*/\
                        if ( get_word ( line, word ) == TRUE )\
                        {\
                            /*---------------------------------------------------------------------------*/\
                            /*-------------------------word ended with comma---------------------------*/\
                            /*---------------------------------------------------------------------------*/\
                            if ( word [ strlen ( word ) - 1 ] == COMMA )\
                            {\
                                 delete_comma_end ( word );\
                                 status_comma = FALSE; /* sign for next operand that current operand ended with comma */\
                            }\
\
                            /*---------------------------------------------------------------------------*/\
                            /*-------------------source operand is immidiate number----------------------*/\
                            /*---------------------------------------------------------------------------*/\
                            if ( is_number_comma_end ( word ) >= TRUE ) /* 1 */\
                            {\
                                 encode_mac ( SRC, IMMEDIATE, 1, ABSOLUTE, atoi ( word ), add_immidiate, IMMEDIATE_BYTE );\
                            } /* 1 */\
                             \
                            /*---------------------------------------------------------------------------*/\
                            /*---------------------Source operand is a symbol name-----------------------*/\
                            /*---------------------------------------------------------------------------*/\
                            if ( ( temp_address = is_symbol_name ( symbol_table, word ) ) != END )\
                            {\
                                 encode_mac ( SRC, DIRECT, 1, RELOCATABLE, temp_address, add_address, ADDRESS_BYTE );\
                            }  \
 \
                            /*---------------------------------------------------------------------------*/\
                            /*------------------Source operand is extern symbol name---------------------*/\
                            /*---------------------------------------------------------------------------*/\
                            if ( is_symbol_name_extern(names_extern, names_count_extern, word) == TRUE )\
                            {\
                                 encode_mac ( SRC, DIRECT, 1, EXTERNAL, TEMPORARY, add_address, ADDRESS_BYTE );\
                            }  \
\
                            /*---------------------------------------------------------------------------*/\
                            /*---------------------Source operand is a register name---------------------*/\
                            /*---------------------------------------------------------------------------*/\
                            if ( ( status_register = is_register_name ( word ) ) > FALSE )\
                            { \
                                  register_source = TRUE;\
                                  encode_mac ( SRC, REGISTER, 1, ABSOLUTE, status_register, add_registers_src, REGISTERS_BYTE );\
                            }\
\
                            /*---------------------------------------------------------------------------*/\
                            /*------------------------Source operand isn't legal-------------------------*/\
                            /*---------------------------------------------------------------------------*/\
                            if ( success == FALSE ) /* undefined source operand */\
                            {\
                                 printf( "error at line %d: undefined source operand\n", line_number );\
                                 return FALSE;\
                            }\
\
                            success = FALSE; /* reset it for next operand check */\

/*=======================================================================================================================================*/
/*----------------------------------------------macro for analyze_command function-------------------------------------------------------*/
/*=======================================================================================================================================*/
#define addressing_3_5_destination_mac()\
/*---------------------------------------------------------------------------*/\
/*----------------------should be destination operand------------------------*/\
/*---------------------------------------------------------------------------*/\
                            next_word ( line, word );\
\
                            /*---------------------------------------------------------------------------*/\
                            /*---------------------------there is more input-----------------------------*/\
                            /*---------------------------------------------------------------------------*/\
                            if ( get_word ( line, word ) == TRUE ) /* 30 */\
                            { \
                                 /*---------------------------------------------------------------------------*/\
                                 /*-----------------check if comma is needed to be in input-------------------*/\
                                 /*---------------------------------------------------------------------------*/\
                                 if ( status_comma == TRUE ) /* word need to be comma */ /* 20 */\
                                 {\
                                      if ( is_comma ( word ) == TRUE ) /* word is comma, continue */\
                                      {\
                                           next_word ( line, word );\
                                           if ( get_word ( line, word ) == TRUE )\
                                                ;\
          \
                                           else /* error, no input after comma */\
                                           {\
                                                printf( "error at line %d: missing destination operand for mov command\n", line_number );\
                                                return FALSE;\
                                           }\
                                      }\
\
                                      else /* error, missing comma */\
                                      {\
                                           printf( "error at line %d: missing comma after source operand\n", line_number );\
                                           return FALSE;\
                                      }\
                                } /* 20 */ \
                                \
                                /*---------------------------------------------------------------------------*/\
                                /*------------------destination operand is a symbol name---------------------*/\
                                /*---------------------------------------------------------------------------*/ \
                                if ( ( temp_address = is_symbol_name ( symbol_table, word ) ) != END ) \
                                {\
                                     encode_mac ( DES, DIRECT, 2, RELOCATABLE, temp_address, add_address, ADDRESS_BYTE );\
                                     unnecessary_input_mac();\
                                } \
                            \
                               /*---------------------------------------------------------------------------*/\
                               /*---------------destination operand is extern symbol name-------------------*/\
                               /*---------------------------------------------------------------------------*/\
                               if ( is_symbol_name_extern(names_extern, names_count_extern, word) == TRUE )\
                               {\
                                    encode_mac ( DES, DIRECT, 2, EXTERNAL, TEMPORARY, add_address, ADDRESS_BYTE );\
                                    unnecessary_input_mac();\
                               }\
\
                               /*---------------------------------------------------------------------------*/\
                               /*------------------destination operand is a register name-------------------*/\
                               /*---------------------------------------------------------------------------*/\
                               if ( ( status_register = is_register_name ( word ) ) > FALSE )\
                               { \
                                      register_destination = TRUE;\
                                      /*---------------------------------------------------------------------------*/\
                                      /*---------------------source operand is a register name---------------------*/\
                                      /*---------------------------------------------------------------------------*/\
                                     if ( register_source == TRUE )\
                                     { \
                                          encode_mac ( DES, REGISTER, 1, ABSOLUTE, status_register, add_registers_des, REGISTERS_BYTE );\
                                          unnecessary_input_mac();\
                                     }\
\
                                     else /* source operand isn't a register name, encode next address */\
                                     {\
                                          encode_mac ( DES, REGISTER, 2, ABSOLUTE, status_register, add_registers_des, REGISTERS_BYTE );\
                                          unnecessary_input_mac();\
                                     }\
                               } \
\
                               /*---------------------------------------------------------------------------*/\
                               /*------------------------Source operand isn't legal-------------------------*/\
                               /*---------------------------------------------------------------------------*/\
                               if ( success == FALSE ) /* undefined destination operand */\
                               {\
                                    printf( "error at line %d: undefined destination operand\n", line_number );\
                                    return FALSE;\
                               }

/*=======================================================================================================================================*/
/*----------------------------------------------macro for analyze_command function-------------------------------------------------------*/
/*=======================================================================================================================================*/
#define all_addressing_destination_mac()\
/*---------------------------------------------------------------------------*/\
/*----------------------should be destination operand------------------------*/\
/*---------------------------------------------------------------------------*/\
                            next_word ( line, word );\
\
                            /*---------------------------------------------------------------------------*/\
                            /*---------------------------there is more input-----------------------------*/\
                            /*---------------------------------------------------------------------------*/\
                            if ( get_word ( line, word ) == TRUE ) /* 30 */\
                            { \
                                 /*---------------------------------------------------------------------------*/\
                                 /*-----------------check if comma is needed to be in input-------------------*/\
                                 /*---------------------------------------------------------------------------*/\
                                 if ( status_comma == TRUE ) /* word need to be comma */ /* 20 */\
                                 {\
                                      if ( is_comma ( word ) == TRUE ) /* word is comma, continue */\
                                      {\
                                           next_word ( line, word );\
                                           if ( get_word ( line, word ) == TRUE )\
                                                ;\
          \
                                           else /* error, no input after comma */\
                                           {\
                                                printf( "error at line %d: missing destination operand for mov command\n", line_number );\
                                                return FALSE;\
                                           }\
                                      }\
\
                                      else /* error, missing comma */\
                                      {\
                                           printf( "error at line %d: missing comma after source operand\n", line_number );\
                                           return FALSE;\
                                      }\
                                } /* 20 */  \
                                /*---------------------------------------------------------------------------*/\
                                /*-----------------destination operand is immidiate number-------------------*/\
                                /*---------------------------------------------------------------------------*/\
                                if ( is_number_comma_end ( word ) == TRUE ) \
                                {\
                                     encode_mac ( DES, IMMEDIATE, 2, ABSOLUTE, atoi ( word ), add_immidiate, IMMEDIATE_BYTE );\
                                     unnecessary_input_mac();\
                                } \
\
                                /*---------------------------------------------------------------------------*/\
                                /*------------------destination operand is a symbol name---------------------*/\
                                /*---------------------------------------------------------------------------*/ \
                                if ( ( temp_address = is_symbol_name ( symbol_table, word ) ) != END ) \
                                {\
                                     encode_mac ( DES, DIRECT, 2, RELOCATABLE, temp_address, add_address, ADDRESS_BYTE );\
                                     unnecessary_input_mac();\
                                } \
                            \
                               /*---------------------------------------------------------------------------*/\
                               /*---------------destination operand is extern symbol name-------------------*/\
                               /*---------------------------------------------------------------------------*/\
                               if ( is_symbol_name_extern(names_extern, names_count_extern, word) == TRUE )\
                               {\
                                    encode_mac ( DES, DIRECT, 2, EXTERNAL, TEMPORARY, add_address, ADDRESS_BYTE );\
                                    unnecessary_input_mac();\
                               }\
\
                               /*---------------------------------------------------------------------------*/\
                               /*------------------destination operand is a register name-------------------*/\
                               /*---------------------------------------------------------------------------*/\
                               if ( ( status_register = is_register_name ( word ) ) > FALSE )\
                               { \
                                      register_destination = TRUE;\
                                      /*---------------------------------------------------------------------------*/\
                                      /*---------------------source operand is a register name---------------------*/\
                                      /*---------------------------------------------------------------------------*/\
                                     if ( register_source == TRUE )\
                                     {\
                                          encode_mac ( DES, REGISTER, 1, ABSOLUTE, status_register, add_registers_des, REGISTERS_BYTE );\
                                          unnecessary_input_mac();\
                                     }\
\
                                     else /* source operand isn't a register name, encode next address */\
                                     {\
                                          encode_mac ( DES, REGISTER, 2, ABSOLUTE, status_register, add_registers_des, REGISTERS_BYTE );\
                                          unnecessary_input_mac();\
                                     }\
                               } \
\
                            /*---------------------------------------------------------------------------*/\
                            /*--------------------destination operand isn't legal------------------------*/\
                            /*---------------------------------------------------------------------------*/\
                            if ( success == FALSE ) /* undefined destination operand */\
                            {\
                                 printf( "error at line %d: undefined destination operand\n", line_number );\
                                 return FALSE;\
                            }

/*=======================================================================================================================================*/
/*----------------------------------------------macro for analyze_command function-------------------------------------------------------*/
/*=======================================================================================================================================*/
#define addressing_3_5_destination_one_operand_mac(command, comm)\
                        OP = command; /* for encoding first byte of command */\
                        /*---------------------------------------------------------------------------*/\
                        /*----------------------should be destination operand------------------------*/\
                        /*---------------------------------------------------------------------------*/\
                            next_word ( line, word );\
\
                            /*---------------------------------------------------------------------------*/\
                            /*---------------------------there is more input-----------------------------*/\
                            /*---------------------------------------------------------------------------*/\
                            if ( get_word ( line, word ) == TRUE ) /* 30 */\
                            { \
                                /*---------------------------------------------------------------------------*/\
                                /*------------------destination operand is a symbol name---------------------*/\
                                /*---------------------------------------------------------------------------*/ \
                                if ( ( temp_address = is_symbol_name ( symbol_table, word ) ) != END ) \
                                {\
                                    if ( is_symbol_name_command ( symbol_table, word ) == TRUE )\
                                    {\
                                        encode_mac ( DES, DIRECT, 1, ABSOLUTE, temp_address, add_address, ADDRESS_BYTE );  \
                                        strcpy(ram [*IC].command_symbol_name, word );  \
                                        unnecessary_input_mac();                 \
                                    }\
                                    else\
                                    {\
                                        encode_mac ( DES, DIRECT, 1, RELOCATABLE, temp_address, add_address, ADDRESS_BYTE );\
                                        unnecessary_input_mac(); \
                                    }\
                                } \
                           \
                               /*---------------------------------------------------------------------------*/\
                               /*---------------destination operand is extern symbol name-------------------*/\
                               /*---------------------------------------------------------------------------*/\
                               if ( is_symbol_name_extern(names_extern, names_count_extern, word) == TRUE )\
                               {\
                                    encode_mac ( DES, DIRECT, 1, EXTERNAL, TEMPORARY, add_address, ADDRESS_BYTE );\
                                    unnecessary_input_mac();\
                               }\
\
                               /*---------------------------------------------------------------------------*/\
                               /*------------------destination operand is a register name-------------------*/\
                               /*---------------------------------------------------------------------------*/\
                               if ( ( status_register = is_register_name ( word ) ) > FALSE )\
                               { \
                                          register_destination = TRUE;\
                                          encode_mac ( DES, REGISTER, 1, ABSOLUTE, status_register, add_registers_des, REGISTERS_BYTE );\
                                          unnecessary_input_mac();\
                               } \
\
                               /*---------------------------------------------------------------------------*/\
                               /*------------------------Source operand isn't legal-------------------------*/\
                               /*---------------------------------------------------------------------------*/\
                               if ( success == FALSE ) /* undefined destination operand */\
                               {\
                                    printf( "error at line %d: undefined destination operand\n", line_number );\
                                    return FALSE;\
                               }\
                        }/* 30 */\
                        else /* no more input, error */\
                        {\
                              printf( "error at line %d: missing destination operand for " #comm " command\n", line_number );\
                              return FALSE;\
                        }
                        
/*=======================================================================================================================================*/
/*----------------------------------------------macro for analyze_command function-------------------------------------------------------*/
/*=======================================================================================================================================*/
#define addressing_3_source_mac(command)\
                        OP = command; /* for encoding first byte of command */\
                        /*---------------------------------------------------------------------------*/\
                        /*------------------------should be source operand---------------------------*/\
                        /*---------------------------------------------------------------------------*/\
                        next_word ( line, word );\
                     \
                        /*---------------------------------------------------------------------------*/\
                        /*---------------------------there is more input-----------------------------*/\
                        /*---------------------------------------------------------------------------*/\
                        if ( get_word ( line, word ) == TRUE )\
                        {\
                            /*---------------------------------------------------------------------------*/\
                            /*-------------------------word ended with comma-----------------------------*/\
                            /*---------------------------------------------------------------------------*/\
                            if ( word [ strlen ( word ) - 1 ] == COMMA )\
                            {\
                                 delete_comma_end ( word );\
                                 status_comma = FALSE; /* sign for next operand that current operand ended with comma */\
                            }\
                             \
                            /*---------------------------------------------------------------------------*/\
                            /*---------------------Source operand is a symbol name-----------------------*/\
                            /*---------------------------------------------------------------------------*/\
                            if ( ( temp_address = is_symbol_name ( symbol_table, word ) ) != END )\
                            {\
                                 encode_mac ( SRC, DIRECT, 1, RELOCATABLE, temp_address, add_address, ADDRESS_BYTE );\
                            }  \
 \
                            /*---------------------------------------------------------------------------*/\
                            /*------------------Source operand is extern symbol name---------------------*/\
                            /*---------------------------------------------------------------------------*/\
                            if ( is_symbol_name_extern(names_extern, names_count_extern, word) == TRUE )\
                            {\
                                 encode_mac ( SRC, DIRECT, 1, EXTERNAL, TEMPORARY, add_address, ADDRESS_BYTE );\
                            }  \
\
                            /*---------------------------------------------------------------------------*/\
                            /*------------------------Source operand isn't legal-------------------------*/\
                            /*---------------------------------------------------------------------------*/\
                            if ( success == FALSE ) /* undefined source operand */\
                            {\
                                 printf( "error at line %d: undefined source operand\n", line_number );\
                                 return FALSE;\
                            }\
\
                            success = FALSE; /* reset it for next operand check */\
                       


/*=======================================================================================================================================*/
/*----------------------------------------------macro for analyze_command function-------------------------------------------------------*/
/*=======================================================================================================================================*/
#define addressing_1_3_5_destination_one_operand_mac(command, comm)\
                        OP = command; /* for encoding first byte of command */\
                        /*---------------------------------------------------------------------------*/\
                        /*----------------------should be destination operand------------------------*/\
                        /*---------------------------------------------------------------------------*/\
                            next_word ( line, word );\
\
                            /*---------------------------------------------------------------------------*/\
                            /*---------------------------there is more input-----------------------------*/\
                            /*---------------------------------------------------------------------------*/\
                            if ( get_word ( line, word ) == TRUE ) /* 30 */\
                            { \
\
                                 /*---------------------------------------------------------------------------*/\
                                 /*-------------------source operand is immidiate number----------------------*/\
                                 /*---------------------------------------------------------------------------*/\
                                 if ( is_number_comma_end ( word ) >= TRUE ) /* 1 */\
                                 {\
                                      encode_mac ( DES, IMMEDIATE, 1, ABSOLUTE, atoi ( word ), add_immidiate, IMMEDIATE_BYTE );\
                                 } /* 1 */\
\
                                /*---------------------------------------------------------------------------*/\
                                /*------------------destination operand is a symbol name---------------------*/\
                                /*---------------------------------------------------------------------------*/ \
                                if ( ( temp_address = is_symbol_name ( symbol_table, word ) ) != END ) \
                                {\
                                     encode_mac ( DES, DIRECT, 1, RELOCATABLE, temp_address, add_address, ADDRESS_BYTE );\
                                     unnecessary_input_mac();\
                                } \
                            \
                               /*---------------------------------------------------------------------------*/\
                               /*---------------destination operand is extern symbol name-------------------*/\
                               /*---------------------------------------------------------------------------*/\
                               if ( is_symbol_name_extern(names_extern, names_count_extern, word) == TRUE )\
                               {\
                                    encode_mac ( DES, DIRECT, 1, EXTERNAL, TEMPORARY, add_address, ADDRESS_BYTE );\
                                    unnecessary_input_mac();\
                               }\
\
                               /*---------------------------------------------------------------------------*/\
                               /*------------------destination operand is a register name-------------------*/\
                               /*---------------------------------------------------------------------------*/\
                               if ( ( status_register = is_register_name ( word ) ) > FALSE )\
                               { \
                                          register_destination = TRUE;\
                                          encode_mac ( DES, REGISTER, 1, ABSOLUTE, status_register, add_registers_des, REGISTERS_BYTE );\
                                          unnecessary_input_mac();\
                               } \
\
                               /*---------------------------------------------------------------------------*/\
                               /*------------------------Source operand isn't legal-------------------------*/\
                               /*---------------------------------------------------------------------------*/\
                               if ( success == FALSE ) /* undefined destination operand */\
                               {\
                                    printf( "error at line %d: undefined destination operand\n", line_number );\
                                    return FALSE;\
                               }\
                        }/* 30 */\
                        else /* no more input, error */\
                        {\
                              printf( "error at line %d: missing destination operand for " #comm " command\n", line_number );\
                              return FALSE;\
                        }
                        
/*=======================================================================================================================================*/
/*----------------------------------------------macro for analyze_command function-------------------------------------------------------*/
/*=======================================================================================================================================*/
#define addressing_no_operands(command, comm)\
                        OP = command;\
                        add_instruction ( ram, *IC, ARE, DES, OP, SRC );\
                        *IC = *IC + TRUE;   \
                        next_word ( line, word );\
                        if ( get_word ( line, word ) == TRUE )\
                        { \
                             printf( "error at line %d: unnecessary input after " #comm " command\n", line_number );\
                             return FALSE;\
                        } 



