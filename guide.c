#include "data.h"
#include "input.h"
#include "symbols.h"
#include "guide.h"
#include <string.h>
#include <stdio.h>

/*======================================================================================================================================*/
/*------------------------------------------function to check if word is guide statement------------------------------------------------*/
/*======================================================================================================================================*/
int is_guideline ( char word [MAX_LINE_LENGTH] )
{
    /*---------------------------------------------------------------------------*/
    /*----------------compare word with all guideline statement------------------*/
    /*---------------------------------------------------------------------------*/
    guideline_cmp ( DATA_STATEMENT, DATA );
    guideline_cmp ( STRING_STATEMENT, STRING );
   /* guideline_cmp ( ENTRY_STATEMENT, ENTRY );*/
    guideline_cmp ( EXTERN_STATEMENT, EXTERN );
    return FALSE;
}

/*======================================================================================================================================*/
/*-----------------------------------------------function to analyze guideline statement------------------------------------------------*/
/*======================================================================================================================================*/
int analyze_guideline ( int status, char line [MAX_LINE_LENGTH], char word [MAX_LINE_LENGTH], int line_number, ptr_symbols * sym, char symbol_name [MAX_LINE_LENGTH], char names_extern [MAX_DATA_LENGTH] [MAX_SYMBOL_LENGTH], int * names_count_extern) /* add parameter to prototype */
{
     int guide = TRUE, external = FALSE, nums_length = 0, success = TRUE;
     int nums [MAX_DATA_LENGTH], i, check = TRUE, comma_space = 0, single_comma = FALSE;
     switch ( status )
     { 
             case DATA:
                  next_word ( line, word );

                  /*---------------------------------------------------------------------------*/
                  /*--add space after each comma to separate it to tokens for scan it properly-*/
                  /*---------------------------------------------------------------------------*/ 
                  while ( ( comma_space = add_spaces ( line, comma_space ) ) != FALSE )
                          ;
                  
                  /*---------------------------------------------------------------------------*/
                  /*-----------------search for at least one parameter-------------------------*/
                  /*---------------------------------------------------------------------------*/ 
                  while ( get_word ( line, word ) == TRUE ) 
                  {  
                       check = FALSE; /* input/parameter exist */

                       /*---------------------------------------------------------------------------*/
                       /*-----------------check if first word is a legal number---------------------*/
                       /*---------------------------------------------------------------------------*/ 
                       if ( is_number_comma_end ( word ) >= TRUE ) /* 3 */
                       { 
                            /*---------------------------------------------------------------------------*/
                            /*-----------in case first number without comma, add to array----------------*/
                            /*---------------------------------------------------------------------------*/ 
                            if ( check_num ( line, word, nums, &nums_length, line_number ) == TRUE  ) /* 2 */
                            { 
                                 if ( single_comma == TRUE )
                                 {
                                      printf( "error at line %d: missing comma\n", line_number );
                                      success = FALSE;
                                      break;
                                 }

                                 single_comma = TRUE; /* single comma input allowed */
                                 continue;
                            } /* 2 */   
                              
                            /*---------------------------------------------------------------------------*/
                            /*-----------in case first number end with comma, add to array---------------*/
                            /*---------------------------------------------------------------------------*/ 
                            if ( check_comma_end ( line, word, nums, &nums_length, line_number ) == TRUE )
                            {      
                                    check = TRUE; /* status: error, missing parameter */
                                    continue;
                            }                
                      }
                      else /* 3 word isn't legal number */
                      {
                           /*---------------------------------------------------------------------------*/
                           /*----------------------in case input is a single comma----------------------*/
                           /*---------------------------------------------------------------------------*/ 
                           if ( is_comma ( word ) == TRUE )
                           { 
                               /*---------------------------------------------------------------------------*/
                               /*---------------------------single comma allowed----------------------------*/
                               /*---------------------------------------------------------------------------*/
                               if ( single_comma == TRUE ) 
                               {
                                   next_word ( line, word );
                                   single_comma = FALSE;
                                   check = TRUE;
                                   continue;
                               }
                                
                               /*---------------------------------------------------------------------------*/
                               /*---------------------more than one comma in a row--------------------------*/
                               /*---------------------------------------------------------------------------*/     
                               printf( "error at line %d: too many commas\n", line_number );
                               success = FALSE; /* return status: failed */
                               break;
                           }

                           /*---------------------------------------------------------------------------*/
                           /*-----------------------input isn't a legal number--------------------------*/
                           /*---------------------------------------------------------------------------*/
                           printf( "error at line %d: parameter is not a number\n", line_number );
                           success = FALSE; /* return status: failed */
                           break;
                      }

                  } /* end while */

                  /*---------------------------------------------------------------------------*/
                  /*-----------------input ended with comma, missing number--------------------*/
                  /*---------------------------------------------------------------------------*/
                  if ( check == TRUE )
                       printf( "error at line %d: missing parameter\n", line_number );

                 nums[nums_length] = NULL; /* end array */ 
                 add_symbol ( sym, symbol_name, guide, external, FALSE, nums ); /* add numbers values to symbol table */      
                 break;

      case STRING: 
                  guide++; /* for symbols table to sign it is string ( for encoding it later with 0 at end ) */
                  next_word ( line, word );  
                  /*---------------------------------------------------------------------------*/
                  /*------------------------should be string input-----------------------------*/
                  /*---------------------------------------------------------------------------*/
                  if ( get_word ( line, word ) == TRUE )
                  {
                       /*---------------------------------------------------------------------------*/
                       /*----------------------input isn't a legal string---------------------------*/
                       /*---------------------------------------------------------------------------*/
                       if ( is_string ( word ) != TRUE )
                       {
                            printf( "error at line %d: undefined string syntax\n", line_number );
                            success = FALSE; /* return status: failed */
                            break;
                       }
                       
                       add_husky ( nums, word ); /* add to nums array string's characters by its ascii values */
                       add_symbol ( sym, symbol_name, guide, external, TRUE, nums ); /* add numbers values to symbol table */   
                       break;   
                  }

                  else /* no string input */
                  {
                       printf( "error at line %d: missing string\n", line_number );
                       success = FALSE; /* return status: failed */
                       break;    
                  }
                       
                  break;
            
       case EXTERN:
       external = TRUE;
       next_word ( line, word );

      /*---------------------------------------------------------------------------*/
      /*------------------------should be symbol input-----------------------------*/
      /*---------------------------------------------------------------------------*/
      if ( get_word ( line, word ) == TRUE )
      {
           /*---------------------------------------------------------------------------*/
           /*-------------------word is a legal symbol name-----------------------------*/
           /*---------------------------------------------------------------------------*/
           if ( is_symbol_without_colon ( word ) == TRUE )
           {
                strcpy ( names_extern[*names_count_extern], word); /* add to 2d array */
                *names_count_extern = *names_count_extern + 1; 
                break;
           }
                         
           else /* symbol name isn't legal */
           {
                 printf( "error at line %d: undefined symbol name\n", line_number );
                 success = FALSE; /* return status: failed */
           }
      }

      else /* no symbol name input */
      {
            printf( "error at line %d: missing symbol name\n", line_number );
            success = FALSE; /* return status: failed */     
      }

     break;
  }  /* switch */

     return success;
}

/*======================================================================================================================================*/
/*-------------------------------function to add number to array. assuming word is a legal number---------------------------------------*/
/*======================================================================================================================================*/
void add_number ( int nums [MAX_DATA_LENGTH], char word [MAX_LINE_LENGTH], int len )
{
      int num = atoi ( word );
      nums [len] = num;
}

/*======================================================================================================================================*/
/*----------------------------function to check if word is number ending with comma and add it to array---------------------------------*/
/*======================================================================================================================================*/
int check_comma_end ( char line [MAX_LINE_LENGTH], char word [MAX_LINE_LENGTH], int nums [MAX_DATA_LENGTH], int * nums_length, int line_number )
{
      int comma;
      if ( (comma = is_number_comma_end ( word )) > TRUE ) /* word is a legal number end with comma */
      {
          delete_comma_end ( word ); /* delete comma from end */
          add_number ( nums, word, *nums_length ); /* add number to nums array */
          *nums_length = *nums_length + 1;  /* increment for next index of nums */
          next_word ( line, word ); 
          return TRUE;
      }
      return FALSE;                                         
}

/*=====================================================================================================================================*/
/*-------------------------------------function to check if word is a legal string-----------------------------------------------------*/
/*=====================================================================================================================================*/
int is_string ( char word [MAX_LINE_LENGTH])
{
    int length =  strlen(word) ; 
    if ( word[START] == DOUBLE_QUOTES && word[length - 1] == DOUBLE_QUOTES )
        return TRUE;
    else
        return FALSE;
}

/*=====================================================================================================================================*/
/*------------------------------------function to add husky values of string to array--------------------------------------------------*/
/*=====================================================================================================================================*/
void add_husky ( int nums [MAX_DATA_LENGTH], char word [MAX_LINE_LENGTH] )
{
     int i, j, len;
     len = strlen ( word ) - 1; /* not include double quotes char */
     for ( i = 1, j = START; i < len; i++, j++)
          nums [j] = (int) word [i];
    
     nums [j++] = NULL;
} 

/*=====================================================================================================================================*/
/*--------------------------------------------function to analyze entry statement------------------------------------------------------*/
/*=====================================================================================================================================*/
int analyze_entry (  char line [MAX_LINE_LENGTH], char word [MAX_LINE_LENGTH], int line_number ) 
{ 
     int success = TRUE;
     next_word ( line, word );

     /*---------------------------------------------------------------------------*/
     /*------------------------should be symbol input-----------------------------*/
     /*---------------------------------------------------------------------------*/
     if ( get_word ( line, word ) == TRUE )
     {
          /*---------------------------------------------------------------------------*/
          /*-------------------word is a legal symbol name-----------------------------*/
          /*---------------------------------------------------------------------------*/
          if ( is_symbol_without_colon ( word ) == TRUE )
          {
               return success;             
          }
                         
          else /* symbol name isn't legal */
          {
                printf( "error at line %d: undefined symbol name\n", line_number );
                success = FALSE; /* return status: failed */
          }
    }

    else /* no symbol name input */
    {
          printf( "error at line %d: missing symbol name\n", line_number );
          success = FALSE; /* return status: failed */     
    }

    return success;

}

/*=====================================================================================================================================*/
/*--------------------------------------------function to analyze entry statement------------------------------------------------------*/
/*=====================================================================================================================================*/
int check_names_entry ( char word [MAX_LINE_LENGTH], ptr_symbols * sym ) 
{
     ptr_symbols p;
     for ( p = *sym ; p; p = p-> next )
     {
           if ( strcmp ( word, p-> name ) == 0 )
           {
                p-> entry = TRUE;    
                return TRUE;
           }   
     }
     return FALSE;
}







