#include <stdio.h>
#include <string.h>
#include "file.h"
#include "data.h"

/*======================================================================================================================================*/
/*-----------------------------------function to create object file ending with .ob-----------------------------------------------------*/
/*======================================================================================================================================*/
int make_object_file ( FILE ** output_file, char input_file_name [], char output_file_name [] )
{
    char * end = OBJECT_END; 
    strcpy ( output_file_name, input_file_name ); 
    strcat ( output_file_name, end ) ;
    *output_file = fopen ( output_file_name, CREATE_FILE);
    if ( !*output_file )
    {
        printf("cannot create file: %s.\n", output_file_name );
        return FALSE;
    }
    else
    {
        /*printf("object file: %s created successfuly.\n\n",output_file_name);*/
        return TRUE;
    }
}

/*======================================================================================================================================*/
/*-------------------------------------function to open assembly file ending with .as---------------------------------------------------*/
/*======================================================================================================================================*/
int open_assembly_file ( FILE ** input_file, char input_file_name [] )
{
    char * end = ASSEMBLY_END;
    int len = strlen ( input_file_name );
    char temp [len];
    strcpy ( temp, input_file_name );
    strcat ( temp, end );
    *input_file = fopen ( temp, READ_FILE );
    if ( !(*input_file) )
    {
         printf("\ncannot open file: %s.\n", temp );
         return FALSE;
    }
    else
    {
         /*printf("\nassembly file: %s opened successfuly.\n",temp);*/
         return TRUE;
    }
}

/*======================================================================================================================================*/
/*-------------------------------------function to open extern file ending with .ext----------------------------------------------------*/
/*======================================================================================================================================*/
int open_external_file ( FILE ** extern_file, char input_file_name [] )
{
    char * end = EXTERNAL_END;
    int len = strlen ( input_file_name );
    char temp [len];
    strcpy ( temp, input_file_name );
    strcat ( temp, end );
    *extern_file = fopen ( temp, CREATE_FILE );
    if ( !(*extern_file) )
    {
         printf("\ncannot open file: %s.\n", temp );
         return FALSE;
    }
    else
    {
         /*printf("\nextern file: %s opened successfuly.\n",temp);*/
         return TRUE;
    }
}

/*======================================================================================================================================*/
/*---------------------------------------function to open entry file ending with .ext---------------------------------------------------*/
/*======================================================================================================================================*/
int open_entry_file ( FILE ** extern_file, char input_file_name [] )
{
    char * end = ENTRY_END;
    int len = strlen ( input_file_name );
    char temp [len];
    strcpy ( temp, input_file_name );
    strcat ( temp, end );
    *extern_file = fopen ( temp, CREATE_FILE );
    if ( !(*extern_file) )
    {
         printf("\ncannot open file: %s.\n", temp );
         return FALSE;
    }
    else
    {
         /*printf("\nentry file: %s opened successfuly.\n",temp);*/
         return TRUE;
    }
}



