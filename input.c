#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "guide.h"
#include "data.h"
#include "input.h"
#include "symbols.h"


/*======================================================================================================================================*/
/*----------------------------------------function to store next string in line---------------------------------------------------------*/
/*======================================================================================================================================*/
int get_line ( FILE * input_file, char line[MAX_LINE_LENGTH], int line_number, char * first_char_in_line )
{
    fpos_t file_pointer;
    fgetpos ( input_file, &file_pointer);
    *first_char_in_line = fgetc ( input_file ); 
    fsetpos ( input_file, &file_pointer );
    /*---------------------------------------------------------------------------*/
    /*---------------------there is more input in file---------------------------*/
    /*---------------------------------------------------------------------------*/ 
    if ( fgets ( line, MAX_LINE_LENGTH, input_file) != NULL )
         return TRUE;

    else
         return FALSE; /* no more input */
}

/*======================================================================================================================================*/
/*----------------------------------------function to check if line length is legal-----------------------------------------------------*/
/*======================================================================================================================================*/
int check_length_line ( int len)
{
     return len > MAX_LINE_LENGTH ? FALSE: TRUE;
}

/*======================================================================================================================================*/
/*------------------function to calculate line length, assume input file pointer is pointing to start of line---------------------------*/
/*======================================================================================================================================*/
int line_length ( FILE * input_file )
{
    int c, count;
    rewind (input_file);
    count = 0;
   
    /*---------------------------------------------------------------------------*/
    /*-------------count all characters in line until EOF is reached-------------*/
    /*---------------------------------------------------------------------------*/ 
    for( ;; )
    {
        c = fgetc(input_file);
        if( c == EOF || c == '\n' )
            break;
        ++count;
    }
    return count;
}

/*======================================================================================================================================*/
/*----------------------------------function to check if line is a start of comment line -----------------------------------------------*/
/*======================================================================================================================================*/
int is_comment ( char word[MAX_LINE_LENGTH] )
{
     return word[START] == ';' ? TRUE: FALSE;
}

/*======================================================================================================================================*/
/*---------------------------------------function to store first word of string---------------------------------------------------------*/
/*======================================================================================================================================*/
int get_word ( char line [MAX_LINE_LENGTH], char word [MAX_LINE_LENGTH] )
{
    /*---------------------------------------------------------------------------*/
    /*----------scan line and search for any non whitespace characters-----------*/
    /*---------------------------------------------------------------------------*/ 
    if ( sscanf ( line, "%s", word ) == TRUE )
         return TRUE; 
         
    else
         return FALSE; /* scan failed, no more input in line */
}

/*======================================================================================================================================*/
/*----------------------function to delete all whitespace characters at the beggining of string-----------------------------------------*/
/*======================================================================================================================================*/
void remove_whitespaces (char * line)
{
    int index, i, j;
    index = 0;

    /*---------------------------------------------------------------------------*/
    /*------------search index's start of non whitespace characters--------------*/
    /*---------------------------------------------------------------------------*/
    while ( line [index] == ' ' || line [index] == '\t' || line [index] == '\n' )
    {
        index++;
    }

    if ( index != 0 ) /* there is whitespace characters at start of line */
    {
        i = 0;
        
        /*---------------------------------------------------------------------------*/
        /*------------assign non whitespace characters to start of line--------------*/
        /*---------------------------------------------------------------------------*/
        while ( line [i + index] != '\0' )
        {
            line [i] = line [ i + index ];
            i++;
        }
        line [i] = '\0'; 
    }
}

/*======================================================================================================================================*/
/*----------------------function to delete first word in a line, assuming word is at start of line--------------------------------------*/
/*======================================================================================================================================*/
int delete_word ( char line [MAX_LINE_LENGTH], char word [MAX_LINE_LENGTH] )
{
    int i, j;
    
    for ( j = 0; word [j] != '\0'; j++ ); /* loop as length of word */
        for ( i = 0; line [i] != '\0'; i++) /* loop as length of line */
        {
            line [i] = line [i + j + 1]; /* build new line without word */
        }
}

/*======================================================================================================================================*/
/*--------------------------------function to set first index of string to to point on next word----------------------------------------*/
/*======================================================================================================================================*/
void next_word ( char line[MAX_LINE_LENGTH], char word[MAX_LINE_LENGTH] )
{
    remove_whitespaces (line); /* remove all whitespace at start of line, to make function requirements */
    delete_word ( line,word ); /* remove first word from line */
}

/*======================================================================================================================================*/
/*---------------------function to check if string is a legal number or legal number ending with comma----------------------------------*/
/*======================================================================================================================================*/
int is_number_comma_end ( char * num)
{
    int i, end, is_comma = FALSE;
    int length =  strlen(num) ; 

    /*---------------------------------------------------------------------------*/
    /*----------------------check if last char is a comma------------------------*/
    /*---------------------------------------------------------------------------*/
    if ( num [length-1]  == COMMA )
    {
         end = length - 1;
         is_comma = TRUE;
    }
    else
         end = length;
    
    /*---------------------------------------------------------------------------*/
    /*---------check if first char is number, plus sign or minus sign------------*/
    /*---------------------------------------------------------------------------*/     
    if ( isdigit ( num [START] ) != FALSE || num [START] == PLUS || num [START] == MINUS )
    {
         /*---------------------------------------------------------------------------*/
         /*--------------check if the rest of the characters is numbers---------------*/
         /*---------------------------------------------------------------------------*/  
         for ( i = 1; i < end; i++)
         {
             if ( isdigit(num [i]) != FALSE)
                 continue;
             else
                 return FALSE;
         }
         return is_comma + TRUE; 
    }
    else
        return FALSE;
}

/*======================================================================================================================================*/
/*---------------------------function to delete comma from end of string, assuming last char is a comma---------------------------------*/
/*======================================================================================================================================*/
void delete_comma_end ( char * num )
{
    int len = strlen ( num );
    num [len - 1] = NULL;
}

/*======================================================================================================================================*/
/*--------------------------------------------------function to check if string comma---------------------------------------------------*/
/*======================================================================================================================================*/
int is_comma ( char word [MAX_LINE_LENGTH] )
{
    int length =  strlen(word) ;

    if ( length == TRUE && word [START] == COMMA )
         return TRUE;
    else
         return FALSE;
} 

/*======================================================================================================================================*/
/*----------------------------------function to check if word is number and add it to array---------------------------------------------*/
/*======================================================================================================================================*/
int check_num ( char line [MAX_LINE_LENGTH], char word [MAX_LINE_LENGTH], int nums [MAX_DATA_LENGTH], int * nums_length, int line_number )
{
        if ( is_number_comma_end ( word ) == TRUE ) /* word is a legal number */
        {
          add_number ( nums, word, *nums_length );  /* add number to nums array */
          *nums_length = *nums_length + 1; /* increment for next index of nums */
          next_word ( line, word ); 
          return TRUE; 
        }
        return FALSE;
}

/*======================================================================================================================================*/
/*--------------------------------------------function to add space after first comma---------------------------------------------------*/
/*======================================================================================================================================*/
int add_spaces ( char line[MAX_LINE_LENGTH], int start ) 
{
     int j, k, length, status = FALSE;
     char temp [MAX_LINE_LENGTH];
     length = strlen(line);
     
     if ( length == TRUE ) /* word's length is 1 */
          return FALSE;
     
    /*---------------------------------------------------------------------------*/
    /*------------search for first comma starting with index: start--------------*/
    /*---------------------------------------------------------------------------*/  
     for ( ; start < length; start++ )
     {
         if ( line [start] == COMMA ) /* found comma */
         {
              start++; /* increment index to start editing */
              status = start; /* save index to return it and for enter next loop */
              break;
         }
              
     }
     
     if ( status > FALSE ) /* cooma is found */
     {
         /*-----------------------------------------------------------------------------------------------*/
         /*-----copy all characters after comma to new array for adding it to line with space at start----*/
         /*-----------------------------------------------------------------------------------------------*/ 
          for ( k = 0, j = start; j <= length; j++, k++ )
          {
               temp [k] = line [j];
          }
    
          line [start++] = ' '; /* add space */

          /*---------------------------------------------------------------------------*/
          /*-----------------copy all characters after comma to line ------------------*/
          /*---------------------------------------------------------------------------*/
          for ( k = 0; start <= length; start++ , k++)
          {
               line [start] = temp [k];
          } 
          
          line [start] = NULL; /* end string */
          return status;
    }
    return status; 
}

/*======================================================================================================================================*/
/*--------------------------------------function to check if string is a legal symbol---------------------------------------------------*/
/*======================================================================================================================================*/
int is_symbol ( char name [MAX_LINE_LENGTH] )
{
    int i; char c = ' ';
    int length =  strlen(name) ;
    
    /*---------------------------------------------------------------------------*/
    /*------------------check if length of symbol name is legal------------------*/
    /*---------------------------------------------------------------------------*/
    if ( length <= MAX_SYMBOL_LENGTH)
    {
       /*---------------------------------------------------------------------------*/
       /*--------------------check if last character is a colon---------------------*/
       /*---------------------------------------------------------------------------*/
       if ( name [length -1] == COLON )
       {
           /*---------------------------------------------------------------------------*/
           /*----------------check if first character is a alphabetic-------------------*/
           /*---------------------------------------------------------------------------*/
           if ( isalpha(name[START]) != FALSE )
           {
                /*---------------------------------------------------------------------------*/
                /*--------check if rest of the characters are alphabetic or digits-----------*/
                /*---------------------------------------------------------------------------*/
                for ( i = 1; i < length-1; i++)
                {
                      if ( isalpha(name[i]) != FALSE || isdigit(name[i]) != FALSE )
                           continue;
                      return FALSE;  
                }
                return TRUE;
           }
           else /* first char is no a letter */
           {
                 /*printf("first char is not a letter%c%c\n",c ,c );*/
                 return FALSE;
           }
               
       }
       else /* colon missing at end of name */
       {
             /*printf("missing colon at end of name%c%c\n",c ,c );*/
             return FALSE;
       }
            
    }
    else /* length is too big */
    {
          /*printf("length name is too big%c%c\n",c ,c );*/
          return FALSE;
    }
          
}


/*======================================================================================================================================*/
/*--------------------------------------function to check if string is a legal symbol---------------------------------------------------*/
/*======================================================================================================================================*/
int is_symbol_without_colon ( char name [MAX_LINE_LENGTH] )
{
    int i; char c = ' ';
    int length =  strlen(name);
    
    /*---------------------------------------------------------------------------*/
    /*------------------check if length of symbol name is legal------------------*/
    /*---------------------------------------------------------------------------*/
    if ( length <= MAX_SYMBOL_LENGTH)
    { 
         /*---------------------------------------------------------------------------*/
         /*----------------check if first character is a alphabetic-------------------*/
         /*---------------------------------------------------------------------------*/
         if ( isalpha(name[START]) != FALSE )
         {
              /*---------------------------------------------------------------------------*/
              /*--------check if rest of the characters are alphabetic or digits-----------*/
              /*---------------------------------------------------------------------------*/
              for ( i = 1; i < length; i++)
              {  
                    if ( isalpha(name[i]) != FALSE || isdigit(name[i]) != FALSE )
                         continue;
                    return FALSE; 
              }
              return TRUE;
         }
         else /* first char is no a letter */
               return FALSE;
  }
  else /* length is too big */
        return FALSE;   
}



/*=====================================================================================================================================*/
/*-----------------------------------------function to calculate array length----------------------------------------------------------*/
/*=====================================================================================================================================*/
int arr_size ( int data [MAX_DATA_LENGTH] )
{
    int i, cnt = 0;
    for ( i = 0; data[i]; i++ )
    {
        cnt++;
    }
    return cnt;
}









