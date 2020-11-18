#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "input.h"
#include "data.h"
#include "ram.h"
#include "symbols.h"
#include "command.h"
#include "functions.h"
#include "file.h"
#include "guide.h"

int main ( int argc, char * argv [] )
{ 
    /*---------------------------------------------------------------------------*/
    /*--------------------------defining ram structure---------------------------*/
    /*---------------------------------------------------------------------------*/
    byte ram[RAM_LENGTH];
    int address = FIRST_ADDRESS;
    
    /*---------------------------------------------------------------------------*/
    /*---------------------defining symbol table structure-----------------------*/
    /*---------------------------------------------------------------------------*/
    ptr_symbols symbol_table = NULL;

    /*---------------------------------------------------------------------------*/
    /*-----------counters for instruction, data and counter for lines------------*/
    /*---------------------------------------------------------------------------*/   
    int IC = 100, DC = 0, line_number;
   
    /*---------------------------------------------------------------------------*/
    /*---------------check if is there at least one file input-------------------*/
    /*---------------------------------------------------------------------------*/
    { /* bracket to avoid complier warning */
         if ( argc == 1 )
         {
             printf("missing input, program terminated.\n");
             return FALSE;
         }
    } /* bracket to avoid complier warning */

    FILE * input_file, * output_file, extern_file, entry_file; /* pointer to file for open input and output files */
    char first_char_in_line;
    int file_number = FIRST_FILE; /* counter for numbers of files readed */
    int input_name_length = strlen( argv[file_number] ); /* size of file's name */
    char output_file_name [input_name_length]; /* for storing name output file */      
    char line [MAX_LINE_LENGTH]; /* for storing current line of file */    
    char word [MAX_LINE_LENGTH]; /* for storing current word of file */ 
    char symbol_name [MAX_LINE_LENGTH]; /* for storing symbol name */ 
    int status_symbol, status_guide, status_entry; /* save status for continue flow */
    char names_extern [MAX_DATA_LENGTH] [MAX_SYMBOL_LENGTH]; /* for checking if symbol names has defined as extern */
    int names_count_extern = 0; /* counter for numbers of names added to names */
    int status_encode = TRUE;
    char extern_address [MAX_DATA_LENGTH];
    char symbol_save [MAX_LINE_LENGTH];
    int empty [0];
    char symbol_command [MAX_DATA_LENGTH];
    const char * commands [16] = { "mov","cmp","add","sub","not","clr","lea","inc","dec","jmp","bne","red","prn","jsr","rts","stop" };

    for ( --argc; argc; argc--, file_number++ ) /* 5 */
    { 
         /*---------------------------------------------------------------------------*/
         /*------------open assembly input file and create output file----------------*/
         /*---------------------------------------------------------------------------*/ 
         if ( open_assembly_file ( &input_file, argv[file_number] ) == TRUE ) /* 4 */
         {
              if ( make_object_file ( &output_file, argv[file_number], output_file_name ) == TRUE ) /* 3 */
              { 
                  /*---------------------------------------------------------------------------*/
                  /*------------------------------scan file session 1--------------------------*/
                  /*----------------------read line by line and analyze input------------------*/
                  /*---------------------------------------------------------------------------*/
                  for ( line_number = 1; get_line (input_file, line, line_number, &first_char_in_line) == TRUE; line_number++ ) /* 2 */
                  { 

                       /*---------------------------------------------------------------------------*/
                       /*--------------------check if line is a comment line------------------------*/
                       /*---------------------------------------------------------------------------*/
                       if ( first_char_in_line == SEMICOLON )
                            continue;

                       /*---------------------------------------------------------------------------*/
                       /*--------------there is a non whitespace characters in line-----------------*/
                       /*---------------------------------------------------------------------------*/
                       if ( get_word ( line, word ) == TRUE ) /* 1 */
                       {
                              /*---------------------------------------------------------------------------*/
                              /*-----------------------line is a comment line------------------------------*/
                              /*---------------------------------------------------------------------------*/
                                    
                              status_symbol = is_symbol ( word );

                              /*---------------------------------------------------------------------------*/
                              /*-----------------check if at start line symbol has defined-----------------*/
                              /*---------------------------------------------------------------------------*/
                              if ( status_symbol == TRUE ) /* symbol is defined */
                              {
                                   save_symbol_name ( word, symbol_name ); /* save symbol name for add it later to symbol table */
                                   next_word ( line, word );
                                   if ( get_word ( line, word ) != TRUE ) /* no more input, error */
                                   {
                                         printf( "error at line %d: guideline/commandline statements are missing\n", line_number );
                                         status_encode = FALSE;
                                         continue;
                                   }
                              } 

                              status_guide = is_guideline ( word ); /* holds the macro value that represents the statements */

                              /*---------------------------------------------------------------------------*/
                              /*------------------------check if line is a guideline-----------------------*/
                              /*---------------------------------------------------------------------------*/
                              if ( is_guideline ( word ) != FALSE )
                              {
                                  if (analyze_guideline (
                                      status_guide,line,word,line_number,&symbol_table,symbol_name,names_extern,&names_count_extern ) == FALSE )
                                     {
                                            status_encode = FALSE;
                                            continue;
                                     }
                                      
                                  else
                                       status_guide = TRUE;
                              }
                              
                              if ( strcmp ( word , ENTRY_STATEMENT ) == 0 )
                              {
                                   if ( ( status_entry = analyze_entry ( line, word, line_number ) ) != TRUE )
                                   {
                                          status_encode = FALSE;
                                          continue;
                                   }
                                         
                              }
                              
                              /*---------------------------------------------------------------------------*/
                              /*-------------------word isn't guide/command/symbol statement---------------*/
                              /*---------------------------------------------------------------------------*/
                              if ( status_symbol != TRUE && status_guide != TRUE && status_entry != TRUE )
                              {
                                    /*printf( "error at line %d: undefined coomand/guide line statement\n", line_number );*/
                                    continue;
                              } 
                              
                              DC++; /* count numbers of guidelines */
                             
                        } /* 1 */
                        
                  } /* 2 */         
                  putchar('\n'); /* @cheker@ */
                  /*---------------------------------------------------------------------------*/
                  /*---------------------------scan file session 2-----------------------------*/
                  /*-------------------read line by line and analyze input---------------------*/
                  /*---------------------------------------------------------------------------*/
                  rewind ( input_file ); IC = 100;
                  for ( line_number = 1; get_line (input_file, line, line_number, &first_char_in_line) == TRUE; line_number++ ) /* 22 */
                  { 
                        
                       /*---------------------------------------------------------------------------*/
                       /*--------------------check if line is a comment line------------------------*/
                       /*---------------------------------------------------------------------------*/
                       if ( first_char_in_line == SEMICOLON )
                            continue;

                       /*---------------------------------------------------------------------------*/
                       /*--------------there is a non whitespace characters in line-----------------*/
                       /*---------------------------------------------------------------------------*/
                       if ( get_word ( line, word ) == TRUE ) /* 11 */
                       {      
                              status_symbol = is_symbol ( word ); 

                              /*---------------------------------------------------------------------------*/
                              /*-----------------check if at start line symbol has defined-----------------*/
                              /*---------------------------------------------------------------------------*/
                              if ( status_symbol == TRUE ) /* symbol is defined */
                              { 
                                   strcpy ( symbol_save, word );
                                   symbol_save [strlen(symbol_save) - 1] = NULL; 
                                   next_word ( line, word );
                                   if ( get_word ( line, word ) != TRUE ) /* no more input, error */
                                         continue; /* analyzed in session 1 */
                              }

                              status_guide = is_guideline ( word ); /* holds the macro value that represents the statements */ 

                              /*---------------------------------------------------------------------------*/
                              /*------------------------check if line is a guideline-----------------------*/
                              /*---------------------------------------------------------------------------*/
                              if ( is_guideline ( word ) != FALSE )
                              {
                                   continue; /* analyzed in session 1 */
                              }
                              
                              /*---------------------------------------------------------------------------*/
                              /*-------------------------check if entry name defined-----------------------*/
                              /*---------------------------------------------------------------------------*/
                              if ( strcmp ( word , ENTRY_STATEMENT ) == 0 )
                              {
                                       next_word ( line, word );
                                       get_word ( line, word ); 
                                       if ( check_names_entry ( word, &symbol_table ) != TRUE )
                                       {
                                             printf( "error at line %d: undefined entry symbol\n", line_number );
                                             status_encode = FALSE;
                                             continue;
                                       } 
                                       continue; /* analyzed in session 1 */
                              }
                              
                              /*---------------------------------------------------------------------------*/
                              /*-------word isn't guide/command/symbol statement, check command line-------*/
                              /*---------------------------------------------------------------------------*/ 
                              if ( status_symbol == TRUE ) 
                                   add_symbol ( &symbol_table, symbol_save, FALSE, FALSE, FALSE, empty ); 
                              
                              continue;
                        } /* 11 */ 
                  }/* 22 */
                  putchar('\n');





                  /*---------------------------------------------------------------------------*/
                  /*---------------------------scan file session 3-----------------------------*/
                  /*-------------------read line by line and analyze input---------------------*/
                  /*---------------------------------------------------------------------------*/
                  rewind ( input_file ); IC = 100;
                  for ( line_number = 1; get_line (input_file, line, line_number, &first_char_in_line) == TRUE; line_number++ ) /* 22 */
                  {
                        

                       /*---------------------------------------------------------------------------*/
                       /*--------------------check if line is a comment line------------------------*/
                       /*---------------------------------------------------------------------------*/
                       if ( first_char_in_line == SEMICOLON )
                            continue;

                       /*---------------------------------------------------------------------------*/
                       /*--------------there is a non whitespace characters in line-----------------*/
                       /*---------------------------------------------------------------------------*/
                       if ( get_word ( line, word ) == TRUE ) /* 11 */
                       {      
                              status_symbol = is_symbol ( word );

                              /*---------------------------------------------------------------------------*/
                              /*-----------------check if at start line symbol has defined-----------------*/
                              /*---------------------------------------------------------------------------*/
                              if ( status_symbol == TRUE ) /* symbol is defined */
                              {
                                   strcpy ( symbol_save, word );
                                   symbol_save [strlen(symbol_save) - 1] = NULL; 
                                   next_word ( line, word );
                                   if ( get_word ( line, word ) != TRUE ) /* no more input, error */
                                         continue; /* analyzed in session 1 */
                              }

                              status_guide = is_guideline ( word ); /* holds the macro value that represents the statements */ 

                              /*---------------------------------------------------------------------------*/
                              /*------------------------check if line is a guideline-----------------------*/
                              /*---------------------------------------------------------------------------*/
                              if ( is_guideline ( word ) != FALSE )
                              {
                                   continue; /* analyzed in session 1 */
                              }
                              
                              /*---------------------------------------------------------------------------*/
                              /*-------------------------check if entry name defined-----------------------*/
                              /*---------------------------------------------------------------------------*/
                              if ( strcmp ( word , ENTRY_STATEMENT ) == 0 )
                              {
                                       next_word ( line, word );
                                       get_word ( line, word ); 
                                       if ( check_names_entry ( word, &symbol_table ) != TRUE )
                                       {
                                             printf( "error at line %d: undefined entry symbol\n", line_number );
                                             status_encode = FALSE;
                                             continue;
                                       } 
                                       continue; /* analyzed in session 1 */
                              }
                              
                              /*---------------------------------------------------------------------------*/
                              /*-------word isn't guide/command/symbol statement, check command line-------*/
                              /*---------------------------------------------------------------------------*/ 
                               
                              analyze_command ( line, word, commands, ram, &IC, symbol_table, names_extern, names_count_extern, line_number, symbol_command );
                              continue;
                        } /* 11 */ 
                  }/* 22 */
                  putchar('\n');






                   
                  /*---------------------------------------------------------------------------*/
                  /*---------------------------scan file session 4-----------------------------*/
                  /*-------------------read line by line and analyze input---------------------*/
                  /*---------------------------------------------------------------------------*/
                  DC = update_address ( ram, IC, &symbol_table ); /* update real address in ram and count data bytes */
                  rewind ( input_file ); 
                  IC = 100;    
                  for ( line_number = 1; get_line (input_file, line, line_number, &first_char_in_line) == TRUE; line_number++ ) /* 22 */
                  {
                       

                       /*---------------------------------------------------------------------------*/
                       /*--------------------check if line is a comment line------------------------*/
                       /*---------------------------------------------------------------------------*/
                       if ( first_char_in_line == SEMICOLON )
                            continue;

                       /*---------------------------------------------------------------------------*/
                       /*--------------there is a non whitespace characters in line-----------------*/
                       /*---------------------------------------------------------------------------*/
                       if ( get_word ( line, word ) == TRUE ) /* 11 */
                       {      
                              status_symbol = is_symbol ( word );

                              /*---------------------------------------------------------------------------*/
                              /*-----------------check if at start line symbol has defined-----------------*/
                              /*---------------------------------------------------------------------------*/
                              if ( status_symbol == TRUE ) /* symbol is defined */
                              { 
                                   word [ strlen ( word ) - 1] = NULL;
                                   strcpy ( symbol_command, word );
                                   symbol_command [strlen ( symbol_command ) - 1 ] = NULL; 
                                   update_command_address ( &symbol_table, ram, word, IC);
                                   next_word ( line, word ); 
                                   if ( get_word ( line, word ) != TRUE ) /* no more input, error */
                                         continue; /* analyzed in session 1 */
                              }

                              status_guide = is_guideline ( word ); /* holds the macro value that represents the statements */ 

                              /*---------------------------------------------------------------------------*/
                              /*------------------------check if line is a guideline-----------------------*/
                              /*---------------------------------------------------------------------------*/
                              if ( is_guideline ( word ) != FALSE )
                              {
                                   continue; /* analyzed in session 1 */
                              }
                              
                              /*---------------------------------------------------------------------------*/
                              /*-------------------------check if entry name defined-----------------------*/
                              /*---------------------------------------------------------------------------*/
                              if ( strcmp ( word , ENTRY_STATEMENT ) == 0 )
                              {
                                       next_word ( line, word );
                                       get_word ( line, word ); 
                                       if ( check_names_entry ( word, &symbol_table ) != TRUE )
                                       {
                                             printf( "error at line %d: undefined entry symbol\n", line_number );
                                             status_encode = FALSE;
                                             continue;
                                       } 
                                       continue; /* analyzed in session 1 */
                              }
                              
                              /*---------------------------------------------------------------------------*/
                              /*-------word isn't guide/command/symbol statement, check command line-------*/
                              /*---------------------------------------------------------------------------*/
                              
                              analyze_command ( line, word, commands, ram, &IC, symbol_table, names_extern, names_count_extern, line_number, symbol_command );
                              continue;
                        } /* 11 */ 
                  }/* 22 */

                  /*---------------------------------------------------------------------------*/
                  /*---------------------------scan file session 4-----------------------------*/
                  /*-------------------read line by line and analyze input---------------------*/
                  /*---------------------------------------------------------------------------*/
                  
                  rewind ( input_file ); 
                  IC = 100;    
                  for ( line_number = 1; get_line (input_file, line, line_number, &first_char_in_line) == TRUE; line_number++ ) /* 22 */
                  {
                       

                       /*---------------------------------------------------------------------------*/
                       /*--------------------check if line is a comment line------------------------*/
                       /*---------------------------------------------------------------------------*/
                       if ( first_char_in_line == SEMICOLON )
                            continue;

                       /*---------------------------------------------------------------------------*/
                       /*--------------there is a non whitespace characters in line-----------------*/
                       /*---------------------------------------------------------------------------*/
                       if ( get_word ( line, word ) == TRUE ) /* 11 */
                       {      
                              status_symbol = is_symbol ( word );

                              /*---------------------------------------------------------------------------*/
                              /*-----------------check if at start line symbol has defined-----------------*/
                              /*---------------------------------------------------------------------------*/
                              if ( status_symbol == TRUE ) /* symbol is defined */
                              { 
                                   word [ strlen ( word ) - 1] = NULL;
                                   strcpy ( symbol_command, word );
                                   symbol_command [strlen ( symbol_command ) - 1 ] = NULL; 
                                   
                                   next_word ( line, word ); 
                                   if ( get_word ( line, word ) != TRUE ) /* no more input, error */
                                         continue; /* analyzed in session 1 */
                              }

                              status_guide = is_guideline ( word ); /* holds the macro value that represents the statements */ 

                              /*---------------------------------------------------------------------------*/
                              /*------------------------check if line is a guideline-----------------------*/
                              /*---------------------------------------------------------------------------*/
                              if ( is_guideline ( word ) != FALSE )
                              {
                                   continue; /* analyzed in session 1 */
                              }
                              
                              /*---------------------------------------------------------------------------*/
                              /*-------------------------check if entry name defined-----------------------*/
                              /*---------------------------------------------------------------------------*/
                              if ( strcmp ( word , ENTRY_STATEMENT ) == 0 )
                              {
                                       next_word ( line, word );
                                       get_word ( line, word ); 
                                       if ( check_names_entry ( word, &symbol_table ) != TRUE )
                                       {
                                             printf( "error at line %d: undefined entry symbol\n", line_number );
                                             status_encode = FALSE;
                                             continue;
                                       } 
                                       continue; /* analyzed in session 1 */
                              }
                              
                              /*---------------------------------------------------------------------------*/
                              /*-------word isn't guide/command/symbol statement, check command line-------*/
                              /*---------------------------------------------------------------------------*/
                              
                              analyze_command ( line, word, commands, ram, &IC, symbol_table, names_extern, names_count_extern, line_number, symbol_command );
                              continue;
                        } /* 11 */ 
                  }/* 22 */

                  /*---------------------------------------------------------------------------*/
                  /*------------------------there is extern symbol-----------------------------*/
                  /*---------------------------------------------------------------------------*/
                  if ( names_count_extern > FALSE )
                  {
                       open_external_file ( &extern_file, argv[file_number] );
                       fill_external ( ram, names_extern, &extern_file, IC );
                  }
                  
                  /*---------------------------------------------------------------------------*/
                  /*------------------------there is extern symbol-----------------------------*/
                  /*---------------------------------------------------------------------------*/
                  if ( check_entry ( symbol_table ) == TRUE )
                  {
                       open_entry_file ( &entry_file, argv[file_number] );
                       fill_entry ( symbol_table, &entry_file );
                  }
                  putchar('\n');
                

              } /* 3 */ 
              
              fill_object_file ( ram, output_file, IC, DC ); 
              free_symbols ( &symbol_table );
              
              
         } /* 4 */
    } /* 5 */

    return 0;
}












