/*
 * File: fs/search.c 
 * 
 * Descri��o:
 *     Rotinas de procura de arquivos, nomes...
 *
 * History:
 *    2015 - Created by Fred Nora.
 */


#include <kernel.h>





/*
 **********************************************************************
 * search_in_dir:
 * 
 *    Procura por um arquivo, dado o endere�o do diret�rio, 
 * com o diret�rio j� carregado na mem�ria.
 *
 *   file_name: Nome do arquivo.
 *     address: Endere�o na mem�ria onde o diret�rio est�.
 *
 * #todo: 
 *    Atender � especifica��es do sistemas de arquivos, como:
 *    +Tamanho do cluster
 *    +Tamanho do disco
 *    +Tipo de sistema de arquivos. 
 *    +...
 */ 

// #todo
// We need to search in the file table first of all.
// The structure found there will give us the inode structure pointer.

// IN:
// File name. "1234578XYZ"
// Address of the directory.

// OUT:
// 1 = Found.

int 
search_in_dir ( 
    const char *file_name, 
    unsigned long dir_address )
{

    int Status = -1;

    unsigned long i = 0;                  // Deslocamento do dir. 
    unsigned long j = 0;                  // Deslocamento no nome.
    unsigned long NumberOfEntries = 512;  // N�mero m�ximo de entradas no diret�rio.
    // O n�mero de entradas poderia ser passado via argumento.

    char NameX[13];
    
    // Copy here
    char NameBuffer[13];


	// Buffer.
	// #importante: 
	// O endere�o do diret�rio foi passado via argumento.

    char *dir = (char *) dir_address;


    // #bugbug
    // Vamos checar o tamanho da string
    // Me parece que quando um nome tem extens�o
    // com menos de tr�s letras, ent�o as �ltimas 
    // letras et�o com '0' e n�o espa�os.

    size_t stringSize = 0;


    debug_print ("search_in_dir: $\n");
    
    //
    // Copy file name
    //

    stringSize = strlen(file_name);
    printf ("Name size = {%d}\n",stringSize);


    strncpy (NameBuffer, file_name, stringSize);



    if (stringSize < 11 )
    {
        //while(stringSize<11)
        //{
        //    strcat(NameBuffer," ");
        //    stringSize++;
        //}
        //printf ("NameBuffer={%s}\n",NameBuffer);
            
            //#debug
        //refresh_screen();
            //while(1){}
    
        if (stringSize == 10)
        { 
            NameBuffer[10] = ' '; 
            stringSize=11;
        }
        
        if (stringSize ==  9)
        { 
            NameBuffer[10] = ' '; 
            NameBuffer[9]  = ' '; 
            stringSize=11;
        }
        
        if (stringSize ==  8)
        { 
            NameBuffer[10] = ' '; 
            NameBuffer[9]  = ' '; 
            NameBuffer[8]  = ' '; 
            stringSize=11;
        }
        
    }
    
    NameBuffer[11] = 0;    
    

    // hack hack
    if (stringSize != 11 ){
        printf ("search_in_dir: [ERROR] Wrong name size. {%d} \n", stringSize);
        goto fail;
    }


    if ( (void*) file_name == NULL ){
        printf ("search_in_dir: [ERROR] file_name\n");
        goto fail;
    }

    if ( *file_name == 0 ){
        printf ("search_in_dir: [ERROR] *file_name\n");
        goto fail;
    }

    if ( *file_name == '/' ){
        printf ("search_in_dir: [FIXME] absolute pathname not supported yet.\n");
        goto fail;
    }


    // Address Limits:
    // Endere�o de mem�ria onde o diret�rio est�.
    
    if (dir_address == 0){
        printf ("search_in_dir: Invalid dir address\n");
        goto fail;
    }

    // Search

    for ( i=0; i < NumberOfEntries; i++ )
    {
        // FAT_DIRECTORY_ENTRY_FREE
        if ( dir[j] == (char) 0xE5 ){ j += 0x20; continue; }

        // diret�rio atual ou diret�rio pai.
        // '.' ou '..'
        if ( dir[j] == '.' )        { j += 0x20; continue; }

        //#TODO
        //pegar o tamanho da string para determinar o quanto comparar.

        // Entrada normal. Diferente de zero.
        // Copia o nome e termina incluindo o char 0.
        if ( dir[j] != 0 )
        {
            memcpy( NameX, &dir[j], 11 );
            NameX[11] = 0;

            //Status = (int) strncmp ( file_name, NameX, 11 );
            Status = (int) strncmp ( NameBuffer, NameX, 11 );
            
            // Found!
            if (Status == 0)
            {
                // #debug
                debug_print("search_in_dir: Found $\n");
                printf ("search_in_dir: Found\n"); 
                return (int) TRUE; 
            }

            //Nothing.
        }   
		//Pr�xima entrada. Repete 512 vezes.
        j += 0x20;
    };

fail:
    debug_print("search_in_dir: Not found $\n");
    printf ("search_in_dir: File not found\n");
    // return FALSE;
    return (int) -1;
}


int search_in_root ( const char *file_name )
{

    if ( (void*) file_name == NULL ){
        debug_print ("search_in_root: [ERROR] file_name\n");
        return -1;
    }

    if ( *file_name == 0 ){
        debug_print ("search_in_root: [ERROR] *file_name\n");
        return -1;
    }

    // IN: filename, dir address
    return (int) search_in_dir ( file_name, VOLUME1_ROOTDIR_ADDRESS );
}


/*
 **********************************************************************
 * fsSearchFile:
 *    Procura por um arquivo no diret�rio raiz de uma parti��o.
 *    Com o diret�rio raiz j� carregado na mem�ria.
 *
 * @todo: 
 *    Atender � especifica��es diferentes de sistemas de arquivos, como:
 *    +Tamanho do cluster
 *    +Tamanho do disco
 *    +Tipo de sistema de arquivos. (nao por enquanto)
 *    +...
 */

// #todo
// Include 'dir address' as parameter.

// only on root dir.

//int fsSearchFile( const char *name ) 
int fsSearchFile (const char *file_name)
{
    debug_print ("fsSearchFile:\n");
    
    // #todo
    // Well, here we can search on different places 
    // using search_in_dir(,);

    return (int) search_in_root(file_name);
}




/*
 *****************************************************
 * fs_find_empty_entry:
 *     Encontrar uma entrada vazia na fat.
 *     @todo: Isso pe importante:
 */

	//@todo:
	//encontrar uma entrada vazia na fat.
	//fornecer o endere�o da fat na mem�ria.

unsigned short fs_find_empty_entry ( char *fat_address )
{
    debug_print ("fs_find_empty_entry: [TODO]\n");
    return (unsigned short) 0;
}


/*
 **************************************************************
 * findEmptyDirectoryEntry:
 * Procura uma entrada vazia no diret�rio 
 *
 * IN:
 *     Endere�o do diret�rio e n�mero m�ximo de entradas.
 */

int 
findEmptyDirectoryEntry ( 
    unsigned long dir_address, 
    int number_of_entries )
{
    int i=0;
    int j=0;
    unsigned char *dir = (unsigned char *) dir_address;


    if ( dir_address == 0 )     { goto fail; }
    if ( number_of_entries < 0 ){ goto fail; }


    // The entry size is 32 bytes.

    for ( i=0; i<number_of_entries; i++ )
    {
        if ( dir[j] == 0 ){ return (int) i; }

        j = (j+32);
    };

fail:
    return (int) (-1);
}


// #bugbug
// Not tested yet.
// OUT: index.
int search_path_in_the_inode_table( const char *path )
{

    struct inode_d *tmp_inode;
    
    int i=0;
    size_t PathSize = 0;
    int Status = -1;
    

    debug_print("search_path_in_the_inode_table: [FIXME] Not tested yet\n");


    if ( (void*) path == NULL ){
        debug_print("search_path_in_the_inode_table: [ERROR] path\n");
        return -1;
    }

    if (*path == 0){
        debug_print("search_path_in_the_inode_table: [ERROR] *path\n");
        return -1;
    }
 
    PathSize = (size_t) strlen(path);
    
    for (i=0; i<32; i++)
    {
        if ( inode_table[i] != 0 )
        {
            tmp_inode = (struct inode_d *) inode_table[i];
            if( (void*) tmp_inode != NULL)
            {
                //#todo validation
                
                //#bugbug: types = (const char *)
                Status = strncmp( path, tmp_inode->path, PathSize );
                if ( Status == 0 ){ return (int) i; }; //ok
            }
        } 
    };
    
    return -1;
}



/*
 ***************************************************************
 * fs_find_n_empty_entries:
 *     Encontrar uma quantidade espec�fica de entradas na fat.
 *     Pois um arquivo demanda uma ou v�rias entradas, 
 *  dependendo do tamanho.
 * @todo: Corrigir essa fun��o, ela � importante.
 *  Obs: tem algo parecido na fun��o que salva um arquivo.
 *  O retorno deve ser short, mesmo tipo do cluster.
 */

// #bugbug
// empty uninitialized.
// Search in file_cluster_list[]

unsigned short fs_find_n_empty_entries ( int n )
{
    int i = 0;
    int l = 0;
    unsigned short empty = 0;

	// Limits.
    if ( n < 0 || n > 1024 ){ goto fail; }


	// Loop ~ Procurar uma quantidade de entradas vazias.
    for ( i=0; i < n; i++ )
    {
		//empty = (unsigned short) fs_find_empty_entry(?);
		
		// Preenche a lista de entradas vazias.	
        if ( empty != 0 && empty < 1024 )
        {
            file_cluster_list[l] = (unsigned short) empty;
            l++;
        
        }else{ goto fail; };

    };

    // Finaliza a lista com uma assinatura.

    file_cluster_list[l] = 0xFFF8; 


// Retorna o primeiro da lista.
done:
    return (unsigned short) file_cluster_list[0];
fail:
    return (unsigned short) 0;
}


//
// End.
//
