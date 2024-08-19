/*
 * File: fs\fs.c
 *
 * Descri��o:
 *     Arquivo principal do sistema de arquivos usado no Boot Loader.
 *     
 * Sobre o sistema de arquivos:
 *    + HD IDE.
 *    + Parti��o 0. 
 *    
 *      Meus Discos:
 *    + FAT16, 8MB, cluster size = 512 bytes. (VHD)
 *    + FAT16, 255 heads, 63 spt, cylinder 243. (Pen Drive, Kingstone DT 2GB).
 *    + FAT16, 255 heads, 63 spt, cylinder 1946. (Pen Drive, SanDisk 16GB).
 *    + ...
 *
 *  Sobre o Boot Loader:   
 *      O prop�sito do Boot Loader � fazer inicializa��es, carregar o Kernel, 
 * alguns m�dulos, aplicativos a GUI. Depois passa o comando para o Kernel.
 *
 * Observa��o: 
 *     Os valores das estruturas podem ser salvos em um registro. No registro 
 * pode ter um registro das opera��es efetuadas no sistema de arquivo.
 *
 * 2015 - Created by Fred Nora.
 * 2016 - Revision.
 */


#include <bootloader.h>


/*
 * fs_load_file:
 *     Carrega um arquivo na mem�ria.
 */
 
/* 
int fs_load_file( unsigned char *file_name, unsigned long file_address ){    
	return (int) 0;    //Ainda n�o implementada.
}; 

*/


/*
 * fs_search_file:
 *    Procura por um nome de arquivo no diret�rio raiz.
 */
 
/*
int fs_search_file(unsigned char *file_name){
	return (int) 0;
};
*/


/*
/// Checks if the sector loaded in user_data is a Fat16 boot sector
unsigned short is_FAT16()
{
    char* label="FAT16";
    
	unsigned short res;
    res = memcmp( label, user_data+0x36, 5);  // Compares with label in FAT16 Boot
    if(res)
    {	
	    return 0;
    }		
	else
    {	
		return 1;
	}
};
*/


/*
 * fatClustToSect:
 *     Converte. 
 *     Suporte a clusters.
 */
 
unsigned long 
fatClustToSect ( unsigned short cluster, 
                 unsigned long spc, 
                 unsigned long first_data_sector )
{
    unsigned long C = (unsigned long) cluster;

    return (unsigned long) ( ( C - 2 ) * spc) + first_data_sector;
};


/*
 * fatLoadCluster:
 *     Load a cluster full of data
 */
 
void 
fatLoadCluster ( unsigned long sector, 
                 unsigned long address, 
				 unsigned long spc )
{
	unsigned long i;

	// Come�a do primeiro setor do cluster.
	for ( i=0; i < spc; i++ )
	{
        read_lba ( address, sector + i );
		
		address = (unsigned long) address + SECTOR_SIZE;    //+512;
	};
};


/*
 * fs_format:
 *     Formata o diret�rio raiz.
 *
 * @todo: 
 *     Formatar uma parti��o.
 *     Deve ficar em aplicativo de formata��o.
 *     Mudar para fsFormat();.
 */
 
void fs_format (){
	
	printf ("fs_format:\n");
};


/*
 * fs_search_empty_entry: 
 *     Encontrar uma entrada vazia em um dado diret�rio.
 *    
 */
unsigned long fs_search_empty_entry (unsigned long id){
	
    return 0; 	
};


/*
 * fs_find_not_empty_entry:
 *     Procura entrada N�O vazia na FAT.
 *     A entrada 0, sempre esta ocupada.
 */

unsigned long fs_find_not_empty_entry (){
	
    return (unsigned long) 0;
};


/* */
void fs_test_fat_vector (){

};


/* */
unsigned long fs_get_fat_entry (unsigned long n){
	
    return (unsigned long) 0;
};


/*
 * fs_create_entry:
 *     Cria uma entrada de diret�rio.
 *
 * Arguments:
 *     +Nome do arquivo.  
 *     +Id do diret�rio.
 *     +Id da entrada de diret�rio.
 *     +N�mero do cluster inicial na FAT.
 *     +Tamanho do arquivo.
 *
 */
 
void 
fs_create_entry ( char *name,  
                  unsigned long id,
				  unsigned long eid,
				  unsigned long cluster,
				  unsigned long size )
{
    //return;    //Nothing for now.
};


/*
 * fsCreateFileName:
 *     Cria um nome de arquivo.    
 */
void 
fsCreateFileName ( char *name, 
                   unsigned long id,
				   unsigned long eid )
{
    //return;
};


/*
 * fsSaveFileName: 
 *     Salva um nome no diret�rio dado o cluster inicial.
 */
 
void 
fsSaveFileName ( unsigned char *name, 
                 unsigned long id, 
				 unsigned long eid, 
				 unsigned long initial_cluster )
{        
	//return;    //Nothing for now.
};


/*
 * fs_save_entry_on_disc:
 *     @todo: O buffer deve ser salvo no disco.
 */
void fs_save_entry_on_disc ( unsigned long id, unsigned long eid ){
	
};


/* */
 
void fs_set_structures (){
	
};


/* */

void fs_set_entry ( unsigned long id, unsigned long eid ){
	
};


/* */

void fs_get_entry ( unsigned long id, unsigned long eid ){
 	 
};

  
/* */

void fs_show_dir_entry ( unsigned long id, unsigned long eid ){
	 
};


/* */

void fs_show_dir (unsigned long id){
	 
};


/*
 * fs_check_cluster:
 *     Confere os clusters de um diret�rio, procurando por falhas.
 *
 *     Objetivo: 
 *          Encontrar falha de diret�rio na hora 
 *          do carregamento dos arquivos do sistema.
 *
 *     Obs: Pertence ao Boot Loader.
 */
 
unsigned long fs_check_cluster (unsigned long id){
	
	return 0;    // Ainda n�o implementada.
};


/*
 * fsSaveRootDir:
 *     Salva o diret�rio raiz no disco.

 *     @todo: Desenvolver essa fun��o.
 *
 *     Observa��es: 
 *
 *          O Boot Loader pode querer salvar o
 *          arquivo de log de inicializa��o ou
 *          modificar alguma configura��o de inicializa��o
 *          no arquivo de configura��o de inicializa��o,
 *          ent�o salvar� os arquivos no diret�rio e
 *          gravar� o diret�rio no disco. 
 *
 *          Fun��o importante para o processo
 *          de restaura��o da inicializa��o, pois
 *          o objetivo do Boot Loader � apenas
 *          carregar arquivos e n�o salvar arquivos.
 *          
 */
 
void fsSaveRootDir (){
	
};


/*
 * fs_load_dir: 
 *     Carrega um diret�rio na mem�ria.
 * 
 */
void fs_load_dir (unsigned long id){
    
};


/*
 * fs_save_dir:
 *     Grava um diret�rio no disco.
 *
 */
void fs_save_dir (unsigned long id){
	
	//return;    //Ainda n�o implementada.
};


/* */

void fs_save_structure (){
	
};


/*
 *  fs_save_entry_on_root:
 *      Salva uma entrada de diret�rio no diret�rio raiz,
 *      dado o n�mero da entrada.
 *      @todo: bugug ? tem alguma incoer�ncia aqui.
 */
void fs_save_entry_on_root (unsigned long eid){
	
	//return;    //Ainda n�o implementada.
};


/*
 * fs_show_entry:
 *     Mostra os dados da entrada de diret�rio.
 */
void fs_show_entry (unsigned long id, unsigned long eid){
    
	//return;    //Ainda n�o implementada.
};


/*
 * fs_delete_entry:
 *     Deleta uma entrada.
 */
void fs_delete_entry (unsigned long id, unsigned long eid){
	
	//return;    //Ainda n�o implementada.
};


/* */

void fs_init_bootfile_struct (){
	
};


/* */

unsigned long fs_get_entry_status (unsigned long id, unsigned long eid){
	
    return 0;    //Ainda n�o implementada.
};


/* */

void 
fs_set_entry_status ( unsigned long id, 
                      unsigned long eid, 
					  unsigned long status )
{
    //return;    //Ainda n�o implementada.
};


/*
 * fs_makeboot:
 *     @todo: Reanalizar o prop�sito dessa rotina.
 */
unsigned long fs_makeboot (){
   	
    return 0;    //Suspensa.
};


/* */
											 
void fs_set_fat_entry (unsigned long n, unsigned long value){
	
};


/*
 * fs_find_empty_entry:
 *
 */
unsigned short fs_find_empty_entry (){
	
    return 0;
};


/*
 * fs_install:
 *     Instalador.
 *     @todo: Essa fun��o deve ser usada em aplicativo.
 */
void fs_install (){
	
	//return;   //@todo: Instala o bootmanager no disco.
};


/*
 * fs_relood_dir: 
 *     Recarrega na mem�ria um diret�rio dado seu Id.
 */
void fs_relood_dir (unsigned long id){
	
	//return;    //Ainda n�o implementada.   
};


/*
 *****************************************************************
 * fsLoadFile:
 *     Carrega um arquivo na mem�ria. (#fat16)
 *     O Boot Loader utiliza essa rotina para carregar os arquivos 
 * do sistema.
 *
 * @todo:
 *     +A libC chama essa fun��o atravez de open(...).
 *     +Mudar o tipo de retorno para 'int'.
 *     +...
 */
 
unsigned long 
fsLoadFile ( unsigned char *name, 
             unsigned long address )
{
	int Status;
	
	//Declara vari�veis.
	unsigned long i = 0;
	unsigned long j = 0;
    unsigned short next;

    //Root support.
    unsigned short *root = (unsigned short *) FAT16_ROOTDIR_ADDRESS;
    unsigned long max = 512;    //N�mero m�ximo de entradas no root dir.
	unsigned long z = 0;        //Deslocamento no diret�rio raiz.
    unsigned long n = 0;        //Deslocamento no nome.
	char name_x[13];

	//FAT support.
    unsigned short *fat  = (unsigned short *) FAT16_FAT_ADDRESS;
	unsigned short cluster;   //Cluster inicial.


	//char name_buffer[32];

	//FILE f;
	//f = (void *) malloc( sizeof(FILE) );
	//if( (void*) f == NULL ){
		//fail.
	//};
	
	//#bugbug
	//Essa refresh screen leva muito tempo.

	//Debug
	//printf("fsLoadFile: Loading %s\n", (const char*) name );
	//refresh_screen();
	
//loadRoot:

    //#test: 
	// Suspendendo o carregamento do root.
	// Ele ser� previamente carregado em main().
	// Carrega o diret�rio raiz na mem�ria.
	//fs_load_rootdirEx();
	
	
	if ( g_fat16_root_status != 1 ){
		
        printf("fsLoadFile: Root Status \n");
        goto fail;	
	}
	
	
	//#debug 
	//printf("carregar_arquivo: Searching File=[%s]\n",file_name);		

	// Procura o arquivo no diret�rio raiz.
//search_file:
	
	while (max > 0)
    {     
        if ( root[z] != 0 )
        {   
			//Copia o nome e termina com 0.
			memcpy( name_x, &root[z], 11);
			name_x[11] = 0;
			
            //Compara 11 caracteres.
			Status = (int) strncmp( name, name_x, 11 );
            
			if ( Status == 0 ){
				cluster = (unsigned short) root[z +13];
                goto found;
			}; 
        };   
        z += 16;    // (32/2) pr�xima entrada!
        max--;      // 512 vezes!
    }; 

   // Se o arquivo n�o for encontrado.	
file_not_found:
    printf("fsLoadFile: File not found\n");     
    goto fail;

    // Se o arquivo for encontrado.    
found:
    	 
    //Pega cluster inicial (word).
	//Limites: 16 entradas por setor, 32 setores de tamanho.
	//(0x1A/2) = 13.
	cluster = (unsigned short) root[z +13];   
	
	// Limits.
	if( cluster <= 0 || cluster > (0xFFF0) )
	{
	    printf("fsLoadFile fail: Cluster limits File=[%s] Cluster=[%d]\n", 
		    &root[z], cluster );
		goto fail;
	};
	

	// Obs: root[z] � o in�cio da entrada. Podemos pegar v�rias informa��es sobre
	// o arquivo. @todo: Precisamos alocar uma estrutura para salvarmos as inform��es
	// sobre o arquivo.
	
//loadFAT:
	
	//#debug. 
	//printf("carregar_arquivo: Loading FAT ...\n");

	
    //#test: 
	// Suspendendo o carregamento do root.
	// Ele ser� previamente carregado em main().	
    // Carrega a FAT na mem�ria. 		
	//fs_load_fatEx();
	
	
	if ( g_fat16_fat_status != 1 )
	{
        printf("fsLoadFile: FAT Status \n");     
        goto fail;
	}

	
	//#debug. 
    //printf("carregar_arquivo: Loading file ...\n"); 

	//
	// Carregar o arquivo.
	//
	
//Loop.	
next_entry:

/*
 * @todo: 
 * Carrega clusters com mais de um setor por cluster. As rotinas para isso 
 * est�o no in�cio desse arquivo.
 *
 
while(1)
    {	
	    //Calcula.
		//Primeiro setor do cluster.
	    unsigned long S;
		S = fatClustToSect(cluster, Spc, FAT16_DATAAREA_LBA); 
		
		//Carrega 'x' setores come�ando de S.
		fatLoadCluster( S, file_address, Spc);
	
	     // Pegar o pr�ximo cluster na FAT.
	     //O cluster atual cont�m o n�mero do pr�ximo.
	     
	    cluster = fat[cluster];	

	    //ver se o cluster carregado era o �ltimo.
	    if(cluster == 0xffff || cluster == 0xfff8){
            goto done;
	    };
    };
 
 *	
 */
 
	// Ler um setor. 
	// (data_area_base + next_cluster - 2)
	// 512 bytes por cluster.
	
	read_lba ( address, FAT16_DATAAREA_LBA + cluster -2 ); 
	
	//Incrementa o buffer. +512;
	address = (unsigned long) address + SECTOR_SIZE;    
	
	//Pega o pr�ximo cluster na FAT.
	next = (unsigned short) fat[cluster];	
	
	//Configura o cluster atual.
	cluster = (unsigned short) next;
	
	//Ver se o cluster carregado era o �ltimo cluster do arquivo.
	
	if ( cluster == 0xFFFF || cluster == 0xFFF8 ){
		
	    goto done; 
	};
	
	//#debug.
	//printf("%d ", cluster);
	
	// Loop: 
	// Vai para pr�xima entrada na FAT.
	goto next_entry;
	
	
   //
   // Fail: O arquivo n�o pode ser carregado.
   //		

fail:
	printf("fsLoadFile: Fail\n");
    refresh_screen();	
    return (unsigned long) 1;
	
    // Done: 
	// Arquivo carregado com sucesso.
	
done:

#ifdef BL_VERBOSE
    printf("fsLoadFile: Done\n");
	refresh_screen(); 
#endif

    return (unsigned long) 0;
};


/*
 ***********************************************************
 * fsSearchFile: 
 *    Procura um arquivo no diret�rio raiz.
 */
 
unsigned long fsSearchFile (unsigned char *name){
	
    int Status;
    unsigned long z = 0;        //Deslocamento no rootdir. 
    unsigned long n = 0;        //Deslocamento no nome.
    unsigned long max = 512;    //N�mero m�ximo de entradas no diret�rio raiz.
	char name_x[13];
    unsigned short *root = (unsigned short *) FAT16_ROOTDIR_ADDRESS;
	
	// Carrega diret�rio raiz na mem�ria.
	fs_load_rootdirEx();
    
	// Fixa o n�mero m�ximo de entradas de acordo 
	// com o tipo de sistema de arquivos.
	 
	switch (g_file_system_type)
    {	    
		//fat16
		case 1:
		    max = 512;
		    break; 
	    
		//fat32
		case 2:
		    break;

        //ext2			
	    case 3:
		    break; 
        //...
		
		default:
		    max = 512;
		    break;
	};	
	
	
	// Procura o arquivo no diret�rio raiz. 
	// Dado o nome via argumento.
	
	while (max > 0)
    {     
        //Checa a primeira letra do nome.
		if (root[z] != 0)
        {           			
			//Copia o nome e termina incluindo um 0.
			memcpy(name_x, &root[z], 11);
			name_x[11] = 0;
			
            //Compara 11 caracteres.
			Status = strncmp(name, name_x, 11);
            
			if (Status == 0){
                goto found;
			}	
		};   
        z += 16;    //(32/2) pr�xima entrada!
        max--;      //512 vezes!
    }; 
	
	//Nothing.
	
//Se o arquivo n�o for encontrado.	
file_not_found:
    printf("fsSearchFile: File not found\n");     
    return (unsigned long) 1;
	
//Se o arquivo for encontrado.    
found:
    return (unsigned long) 0;
};


/*
 * fsSearchFileName:
 *     Procura um nome de arquivo no diret�rio.
 *     @todo: Rever o tipo de retorno.
 */
unsigned long fsSearchFileName ( unsigned char *name ){   
     
    return fsSearchFile (name);
};


/*
 * fs_load_rootdirEx:    
 *    Carrega o diret�rio raiz na mem�ria.
 *
 * Obs:
 *    Carregando 32 setores do rootdir.
 *    Size = 32 setores. 
 *    512 entradas de 32 bytes cada.
 *
 */
 
void fs_load_rootdirEx (){
	
	unsigned long i;
	unsigned long n = 0;
	unsigned long root_size = 32;    //@todo: Pegar de estrutura. 
	
	// debug
	// printf("Loading root dir ...\n");
	
	//
	// Carregar 32 setores na mem�ria.
	//
	 
	for ( i=0; i < root_size; i++ )
	{	
		read_lba( FAT16_ROOTDIR_ADDRESS + n, FAT16_ROOTDIR_LBA + i); 
		n = n+512;    //Incrementa buffer.	
	};
	
	//@todo: test ??

//done:	
    //return;
};


/*
 * fs_load_rootdirEx:
 *     Carrega a FAT na mem�ria. (size = 64 setores).
 *
 */
 
void fs_load_fatEx (){
	
	unsigned long i;
	unsigned long n = 0;
	
	//@todo: Pegar de estrutura.
	unsigned long fat_size = 64;    //64 setores de tamanho.    
	
	// debug
	// printf("Loading Cluster Table.\n");//fat
	
	//
	// Carregar FAT na mem�ria.
	//
	
	for ( i=0; i < fat_size; i++ )
	{
	    read_lba( FAT16_FAT_ADDRESS + n, FAT16_FAT_LBA + i);   
		n = n+512;    //Incrementa Buffer
	};
	
	//@todo: test ??

//done:
    //return;
};


/*
 * fs_put_list_on_fat:
 *     Grava na fat uma lista de clusters.
 *     Uma lista de clusters, forma um arquivo.
 */
 
void fs_put_list_on_fat (){
	
    unsigned short i;
    unsigned short lista_size = 1024;
	
    for ( i=0; i < lista_size; i++ )
	{
        //�ltimo elemento da lista.
		if ( file_cluster_list[i] == 0xFFFF || 
		     file_cluster_list[i] == 0xFFF8 )
        {
	        fs_set_fat_entry ( file_cluster_list[i], 0xFFFF );
			return;
			//goto done;
        };
		
		//um elemento qualquer da lista.
   	    if ( file_cluster_list[i] > 0 && 
		     file_cluster_list[i] < lista_size )
        {	
	        fs_set_fat_entry ( file_cluster_list[i], file_cluster_list[i+1] );
        };
    };
	
//done:
    //return;
};


/*
 * fs_find_n_empty_entries:
 *
 */
 
unsigned long fs_find_n_empty_entries (unsigned long n){
	
    unsigned long i;
	unsigned long l;
	unsigned short empty;
	unsigned short lista_size = 1024;
	
	//@todo: Pegar de estrutura.
	//N�mero max de entradas na FAT.
	unsigned short fat_max = (64*512/2);    
	
	// Limits.
	
	if ( n < 0 || n > lista_size )
	{
	    goto fail;
	};
	
	
	for(i = 0; i < n; i++)
	{
		empty = fs_find_empty_entry();
		
		//cria a lista de entradas vazias.
		if( empty != 0 && empty < fat_max )
		{
		    file_cluster_list[l] = empty;
            l++;
		}
        else
        {
		    goto fail;
		};		
	};
	
    file_cluster_list[l] = 0xFFFF;    
	
//
// Done.
//	
done:
	//retorna o primeiro da lista.
	return file_cluster_list[0];
	
fail:	
    return 0;
};

 
/* fs_load_rootdir: Carrega o diret�rio raiz na mem�ria. */ 
 
void fs_load_rootdir (){
	
    fs_load_rootdirEx ();
};


/*
 * read_lba: 
 *     L� uma lba no hd. (um setor)
 *     Opera��o de hardware.
 */
 
void read_lba ( unsigned long address, unsigned long lba){   
    
    my_read_hd_sector ( address, lba, 0, 0 );
};


/*
 * write_lba: 
 *     Grava uma lba no HD. (um setor).
 */
 
void write_lba ( unsigned long address, unsigned long lba ){    
 
    my_write_hd_sector ( address, lba, 0, 0 );     
};


/*
 * fsSaveFile:
 * Salvando um arquivo.
 * @todo: Manter a estrutura dessa fun��o mas mudar as rotinas chamadas.
 *           A lib C chama essa fun��o.
 * #todo: A rotina do kernel funciona bem.
 */
 
unsigned long 
fsSaveFile ( unsigned char *file_name, 
             unsigned long file_size, 
			 unsigned long file_address )  
{
	
	//#suspensa.
	
	return 0;
	
	/*
	
    unsigned long i;  //Deslocamento. Procurando espa�o livre.
    unsigned long j;  //Deslocamento. Salvando cluster number.
    unsigned long c;
    //@todo: Pegar esse valor de uma estrutura de FAT.
    unsigned long max = 128;  //N�mero m�ximo de entradas na FAT.
    unsigned long first;
    unsigned long next;
    unsigned long sector;
    unsigned long endereco;  //File address.    	
	unsigned long *fat_base = (unsigned long *) FAT16_FAT_ADDRESS; 
	//Obs: Endere�o imposto.
    
	//
	// #### BUGBUG ####
	// ENDERE�O.
	
	//@todo: Alocar din�micamente.
    unsigned long *list = (unsigned long *) 0x04600000;  //@todo: Rever esse endere�o.    
	
	//@todo: Check file address limits.
    //if(file_address == 0){}
	
    //File address.
    endereco = (unsigned long) file_address;
    
    //Carrega 2 setores da FAT.
	//@todo: Isso precisa ser ampliado.
    read_lba(FAT16_FAT_ADDRESS, FAT16_FAT_LBA);
    read_lba(FAT16_FAT_ADDRESS + 0x200, FAT16_FAT_LBA + 1);
	
    j = 0;     //Deslocamento na lista, tem que ser zero.
    c = 32;    //Deslocamaento na FAT.
    i = 32;    //Contagem de entradas  na FAT, come�a do 32 e vai at� muito.
    
    //Loop. 
    
	while(i != max)
    {
        //procurando espa�o livre   
        if(fat_base[c] == 0)
        {
            //fim do arquivo ?
            if(file_size == 0)
            {
                //marca o fim          
                list[j] = 0x0000FFFF;  //eof           
                //encontrado todos os espa�os livres que o arquivo precisa.
                goto fat_save;
            };    

            //salva um endere�o livre           
            list[j] = c;

            //incrementa a lisa
            j++; 

            //decrementa o tamanho do arquivo!
            file_size--;                  
         };
         
        //incrementa a fat
        c++; 
        
        //incrementa a quantidade de busca.
        i++;
   }; 
   
  // MessageBox("Max entries !");
   fatseg2: goto fatseg2;
    return 1;           

fat_save:
       
    //In�cio da lista.
    j = (512*4);
    i = 0;
    
    first = list[i];
    
    //
    // Salva no diret�rio raiz o nome e o primeiro cluster ?? @todo: Check.
    // Corrige deslocamento ?? @todo: Check.
    
	// ??
	// salva_nome(file_name, first); 
    
    while(j)
    {            
        //Pega um conte�do da lista.
        next = list[i];
        
        //Salva a trilha indicada pela lista.
        // salva_trilha( next, endereco);
        
		//??
		//save_file( 0, endereco, next, 8);
        
		
        //Encontrada a assinatura na lista.
        if(next == 0x0000FFFF)    //@todo: Usar constante. 
        {
            next = list[i-1];
            fat_base[next] = 0x0000FFFF;    //@todo: Usar constante.  
            goto done;         
        }
        else    //N�o � assinatura ainda.
        { 
            //-0x60 = � uma corre��o por causa valor do tipo (unsigned long)!!
            //grava na fat o endere�o do proximo cluster
            fat_base[next] = list[i+1];
            
            //grava - aqui next esta certo!!!
            write_lba( endereco, next);
            
            endereco += 512; 
        }; 
         
        i++;    //Pr�ximo valor da lista.           
        j--;
    };
    
    //MessageBox("nao encontra assinatura na lista!");
    
	//
	// @todo: Precisa de uma mensagem de erro aqui.
	//
	
	fatseg3: goto fatseg3;

done:        
    //Salva dois setores da FAT!
    //@todo: Isso precisa ser ampliado.	
    write_lba(FAT16_FAT_ADDRESS        , FAT16_FAT_LBA);
    write_lba(FAT16_FAT_ADDRESS + 0x200, FAT16_FAT_LBA + 1);
    
	return (unsigned long) 0;
	*/
};


/* fsCreateDir: Cria um diret�rio. */
	
unsigned long fsCreateDir ( char *name , unsigned long id ){
	
	//#bugbug 
	
	return fsCreateFile ( name, id );   
};


/* fsCreateFile: Cria um arquivo. */

unsigned long fsCreateFile ( char *name, unsigned long id ){
	
    unsigned long fat_entry;
    unsigned long dir_entry;
	unsigned long size = 1;
	
	//encontra uma entrada vazia na fat.
	fat_entry = fs_find_empty_entry();
	
	//testar antes a validade da entrada.
	
	if( fat_entry == 0 ){
	    printf("fs_create_file: error, o cluster vazio eh 0 \n" );
		goto fail;
	};
			
	//erro 2 - nao ha mais entradas vazias
	if( fat_entry == FAT_ERRO_1 ){
		printf("FAT_ERRO_1 nao ha mais entradas vazias\n" );
		goto fail;
	};
		
	//
	if( fat_entry < 0 || fat_entry > 1024 ){
	    printf("FAT entry fora dos limites!\n");
		goto fail;
	};
			
	//marca na entrada da fat como fim de arquivo.(size 1)
	//fs_set_fat_entry( fat_entry, 0x0000fff8 );
	fs_set_fat_entry( fat_entry, 0x0000FFFF );  //EOF 0x0000fff8
		
	//procura uma entrada vazia no diretorio.
	dir_entry = fs_search_empty_entry(id);
	
    if( dir_entry == 0 ){
        printf("fs_create_file: nao ha entradas vazias no diretorio escolhido\n");
        goto fail;		
	};	
  		 
	if ( dir_entry < 0 || dir_entry > 64 )
	{
        printf("fs_create_file: entrada fora dos limites\n");
        goto fail;		
	};	
	
	//cria a entrada no diretorio
	fs_create_entry( name, id, dir_entry, fat_entry, size ); 
	
	//fs_save_entry_on_root(i);

done:
#ifdef BL_VERBOSE
    printf("fs_create_file: done ! cluster = %d \n",fat_entry);
#endif    
	return 0;	
fail:   
    printf("fail cluster = %d \n",fat_entry);
	return 1;
};


/*
 * fsClearFat:
 *     Apaga a fat, colocando zero em tudo.
 */
void fsClearFat (){
	
    fsInitFat();  //Provis�rio.	 
};


/*
 * fsCheckFat: 
 *     Confere os par�metros da FAT.
 *     @todo: Usar estrutura.
 */
int fsCheckFat()
{
	unsigned long i;
	unsigned long fat_entry;
	
	printf("\n");
	
    for(i = 0; i < 256; i++)
	{
        //Pega a FAT entry.
		fat_entry = fs_get_fat_entry(i);
		
		//Se a entrada da FAT estiver vazia.
		if(fat_entry == 0){
			printf("%c ",'#');
		}else{
			printf("%d ",fat_entry);		
		};
		//Nothing.
	};	

	//Nothing.
	
exit:
#ifdef BL_VERBOSE
    printf("Done.!\n");
#endif
	return 0;
fail:
    printf("Fail!\n");
    return 1;
};


/*
 * fsInitFat:
 *    Inicializa as estruturas da FAT.
 */
void fsInitFat(){
	//return;  //Ainda n�o implementada.
};


/*
 * fsInitStructures: 
 *     +Limpa as estruturas para primeiro uso.
 *     +@todo: Configura as estruturas com os falores encontrados
 *             no mbr, no rootdir.
 *
 * OBS: Quem vai usar essas estuturas � o bootloader.
 */
void fsInitStructures(){
	//return;  //Ainda n�o implementada.
};


/*
 ***************************************************************
 * fsInit:
 *     Inicializa o sistema de arquivos da parti��o do sistema.
 *     Obs: Isso deve ficar no fim do arquivo.
 */
 
int fsInit (){
	

#ifdef BL_VERBOSE
    printf("fsInit: Initializing structs..\n");
#endif
	fsInitStructures();

#ifdef BL_VERBOSE
	printf("fsInit: Inicializing file system..\n");
#endif
	fsInitFat();

	//More?!

    return (int) 0;
};


//
// End.
//


