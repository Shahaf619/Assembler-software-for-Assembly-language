#include "symbols.h"
#include "data.h"
#include "input.h"
#include "ram.h"
#include "command.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/*=====================================================================================================================================*/
/*-------------------------------------function to add new symbol to symbol table------------------------------------------------------*/
/*=====================================================================================================================================*/
void add_symbol ( ptr_symbols * sym, char symbol_name[MAX_NAME_LENGTH], int guide, int external, int string, int * num )
{
     int i,len;
     ptr_symbols new_symbol, last_symbol, p_symbol;
     new_symbol = (ptr_symbols) malloc ( sizeof(symbols) );
     len = arr_size (num);

     /*---------------------------------------------------------------------------*/
     /*------------------------failed to allocate memory--------------------------*/
     /*---------------------------------------------------------------------------*/ 
     if ( !new_symbol )
          return;
    
     /*---------------------------------------------------------------------------*/
     /*------------------fill data in symbol table's new node---------------------*/
     /*---------------------------------------------------------------------------*/  
     new_symbol-> address = START;
     strcpy ( new_symbol-> name, symbol_name );
     new_symbol-> guide = guide;
     new_symbol-> external = external;
     new_symbol-> string = string;
     for ( i = 0; i < len; i ++) 
           new_symbol-> data[i] = num[i];
    
     new_symbol-> next = NULL;      
    
     /*---------------------------------------------------------------------------*/
     /*--------------------------symbol table is empty----------------------------*/
     /*---------------------------------------------------------------------------*/  
     if (! *sym)
     {
         *sym = new_symbol;
         return;
     }
    
     /*---------------------------------------------------------------------------*/
     /*----------------symbol table isn't empty, search last node-----------------*/
     /*---------------------------------------------------------------------------*/ 
     last_symbol = *sym;

     if ( guide == FALSE )
     {
         while ( last_symbol-> next )
                last_symbol = last_symbol-> next;

         last_symbol-> next =  new_symbol;
         return;
     }     

     while ( last_symbol-> next && last_symbol-> guide != FALSE )
            last_symbol = last_symbol-> next;
          
     new_symbol-> address = last_symbol-> address + arr_size (last_symbol-> data); 
     last_symbol-> next =  new_symbol; 
}

/*=====================================================================================================================================*/
/*----------------------------------------------function to save string----------------------------------------------------------------*/
/*=====================================================================================================================================*/
void save_symbol_name ( char word [MAX_LINE_LENGTH], char symbol_name [MAX_LINE_LENGTH] )
{
     strcpy (symbol_name, word );
     symbol_name [ strlen (symbol_name) - 1 ] = NULL;
}

/*=====================================================================================================================================*/
/*--------------------------------------------function to print symbol table-----------------------------------------------------------*/
/*=====================================================================================================================================*/
void print_symbol_table ( ptr_symbols sym ) /* @cheker@ */
{ 
    printf("==================================================================\n");
    while ( sym )
    {
         printf("address: %d %s data: ", sym-> address, sym-> name ); 
         int i;

         for ( i = 0; sym-> data[i]; i++ )
         {
              printf("%d ", sym-> data[i] );
         }
         if ( sym-> guide > TRUE ) /* symbol is a string */
              printf("%d ", sym-> data[i] ); /* print null at end */
         putchar('\n');
         sym = sym-> next;
    }
    printf("==================================================================\n");
}

/*=====================================================================================================================================*/
/*--------------------------------------function to free all nodes of symbol table-----------------------------------------------------*/
/*=====================================================================================================================================*/
void free_symbols ( ptr_symbols * sym )
{
    ptr_symbols to_free;
    
    while ( *sym )
    {
        to_free = * sym;
        * sym = to_free-> next;
        free(to_free);
    }
}

/*=====================================================================================================================================*/
/*------------------------------------function to update real address of symbols in ram------------------------------------------------*/
/*=====================================================================================================================================*/
int update_address ( byte ram [RAM_LENGTH], int IC, ptr_symbols * sym ) 
{ 
    int i, j, data_count = 0;
    ptr_symbols p = *sym;
    

    while ( p )
    {
         if ( p-> external == TRUE )
         {
              p = p-> next;
              continue;
         }

         if ( p-> guide == FALSE )  
         { 
             for ( j = 0; j < RAM_LENGTH; j++ )
             {
                  if ( strcmp ( ram [j].command_symbol_name, p-> name ) == 0 )
                       p-> address = j;
             }
             p = p-> next;
             continue;
         }       

         p-> address = IC; 
         for ( i = 0; p-> data[i]; i++, IC++ ) 
         {
              add_data ( ram, IC, p-> data[i] );
              ram [IC].which = DATA_BYTE; /* sign to ram that address byte is encoded */
              data_count++;
         }

         if ( p-> guide > TRUE ) /* symbol is a string */
         { 
              add_data ( ram, IC, NULL ); /* add null to end of string */
              ram [IC].which = DATA_BYTE; /* sign to ram that address byte is encoded */
              data_count++;
              IC++;
         }
         p = p-> next;
    } 
    return data_count;
}

/*=====================================================================================================================================*/
/*----------------------------------------function to check if there is entry symbol---------------------------------------------------*/
/*=====================================================================================================================================*/
int check_entry ( ptr_symbols sym )
{
    while ( sym )
   {
        if ( sym-> entry == TRUE )
             return TRUE;

        sym = sym-> next;
   }
   return FALSE;
}

/*=====================================================================================================================================*/
/*-------------------------------------------------function to fill entry file---------------------------------------------------------*/
/*=====================================================================================================================================*/
void fill_entry ( ptr_symbols sym, FILE ** entry_file )
{
    while ( sym )
    {
      if ( sym-> entry == TRUE )
      {
           fprintf ( *entry_file, "%s %d\n", sym-> name, sym-> address );
      }

      sym = sym-> next;
   }
}

void fill_external (  byte ram[RAM_LENGTH], char names_extern [MAX_DATA_LENGTH] [SYMBOL_LENGTH], FILE ** external_file, int IC)
{
    int i, j = 0; 
    for (  i = 0; i < IC; i++ )
    { 
  	 if ( ram [i].which == ADDRESS_BYTE )
	 {
     	     if ( ram [i].address_byte.ARE == EXTERNAL  )
             {
	         fprintf ( *external_file, "%s %d\n", names_extern[j++], i ); 
	     }
	 }
   }
}

void update_command_address ( ptr_symbols * sym, byte ram [RAM_LENGTH], char word [MAX_LINE_LENGTH], int IC )
{
   int i; 
   ptr_symbols p = * sym;
   for ( i = 100; i < RAM_LENGTH; i++ )
   {   
          if ( strcmp ( ram [i].command_symbol_name, word ) == 0 )
          { 
              for ( ; p; p = p-> next )
              {
                   if ( strcmp ( p-> name, ram [i].command_symbol_name ) == 0 )
                        p-> address = IC; 
              }
          }
   }
}






