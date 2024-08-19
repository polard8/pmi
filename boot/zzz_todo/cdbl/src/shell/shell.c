/*
 * File: shell\shell.c 
 *
 * Descri��o:
 *     Mini-shell do Boot Loader.
 *
 * Objetivo:
 *    Oferecer comandos b�sicos que ajudem na inicializa��o do sistema.
 *    Importante: No Boot Loader, o interpretador de comandos se revela
 * importante especialmente quando mostra informa��es sobre os recursos
 * de armazenamento, e sobre os arquivos que ser�o carrregados.
 *    Nesse est�gio de processo de boot alguns decis�es j� foram tomadas,
 * como qual ser� o modo de video. O ponto forte do Boot Loader, que foi 
 * escrito em linguagem de alto-nivel de 32bit � a manipula��o de sistema de 
 * arquivos e de gerenciamento dos arquivos que ser�o carregados. Esse deve ser
 * o foco do boot loader e do interpretador de comando do shell.
 *
 * Hist�rico:
 *     Vers�o: 1.0, 2015 - Created.
 *     Vers�o: 1.0, 2016 - Revision.
 *     ...
 */

 
#include <bootloader.h>


// Fun��es importadas.

extern void asm_shut_down();


// Vari�veis internas.

unsigned long deslocamento;
int ret_string;
int ShellInitialized;
int shell_status;
//int shellStatus;
//int shellError;
//...


// #todo: 
// Diminuir o tamanho do buffer.
// ?? Usar static

unsigned char *shell_string_buffer[256];    

// String para mensagem de ajuda.
// #todo: 
// ?? Usar static.		 

char help_string[] = "\n help, format, install, makeboot, reboot \n";


// Prot�tipos de fun��es internas.

// Prot�tipo do procedimento de janela do shell do Boot Loader.

int 
shellProcedure ( unsigned long window, 
                 unsigned long msg, 
			     unsigned long long1, 
			     unsigned long long2 );
				 
void testa_mbr ();
void testa_root ();
void debug ();


/*
 *************************************************
 * shellMain:
 *     Fun��o principal do Shell do Boot Loader.
 *
 *     #todo: Usar argumentos padr�o argc, argv.
 *     #todo: usar delimtador e separador de tokens igual no app shell.
 */
 
// #todo: int shellMain ( int argc, char **argv )
// #todo: int shellMain ( int argc, char *argv[] )
 
int shellMain ( int argc, char *argv ){
	
    unsigned long ret_proc;
	
	// Title.	
	
    bl_clear (0);
    printf("[BL-SHELL:] Auxilia a instalacao do sistema\n");
	
	//
    // Ativa o procedimento do shell.      
	//
	
	//@todo registrar procedimento.
    //ret_proc = registra_procedimento( (unsigned long) shell_proc, 1, 0, 0);	
	if(ret_proc != 0){
	    goto fail_shell;
	};
	
	//
	// Status - Inicia o shell.
	//
	
	shell_status = 1;
	
	
//Loop	
sh_loop:
    
	//status.
	if(shell_status != 1){ 
	    goto exit_shell; 
	};
	
	//prompt.
	shellInitializePrompt();
	
	//comparar.
	if( shellCompare() != 0 )
	{
	    //printf("shell: palavra nao reservada!");
		goto sh_loop;
	}
	else
	{
	    goto sh_loop;
	};

fail_shell:
  	printf("shell_main: Fail\n"); 
	return (int) 1;
	
exit_shell:	    
	return (int) 0;     
};


/*
 ************************************************
 * shellProcedure: 
 *     Procedimento de janela do Shell do Boot Loader. 
 */
 
int 
shellProcedure ( unsigned long window, 
                 unsigned long msg, 
				 unsigned long long1, 
				 unsigned long long2 )
{
    unsigned long input_ret;    
	
	//Tela para debug em text mode.
   	unsigned char *screen = (unsigned char *) 0x000B8000;      

	// @todo: 
	// Esse debug s� deve aparecer se estivermos em Text Mode.
	
	//Debug:
	//if useGUI == 0 (){...}
	//screen[78] = (char) long1;
	//screen[79] = (char) 0x09; //blue    
	
    switch (msg)
    {      
        case MSG_KEYDOWN:
            switch(long1)
            {
                case VK_ESCAPE:
  		           	shell_status = 0;
				   //shell_main( 0,0,0,0);
                   break;           
                              
               //texto - envia o caractere
               default:			       
			       input_ret = input(long1);				   
				   if(input_ret == KEY_RETURN)
				   {
				        g_cmd_status = 1;    //�ltimo caractere.
				   }
				   else
				   {
				        g_cmd_status = 0;    //Ainda tem mais caractere.
				   };				   
                   break;               
            };
            break;
		
		default:
		    //@todo: Bug Bug, isso pode ser problema.
			bl_procedure ( window, msg, long1, long2 );
		    break;	         
    };	

//done:
	
    return (int) 0;	
};  



/*
 * shellInitializePrompt: 
 *     Inicializa o prompt do shell.
 *     Limpa o buffer de string.
 */ 
 
int shellInitializePrompt (){
	
    int i;
    
	//@todo: Usar 128.
	
	for ( i=0; i<250; i++){
		
        prompt[i] = (char) '\0';
    };
    
	prompt[0] = (char) '\0';
	prompt_pos = 0;
	
    g_cmd_status = 0;

	g_cursor_y++;
	g_cursor_x = 0;
	printf("BL-SHELL:>");
	
	printf("%c",'_');
	g_cursor_x--;
 
//done: 

    return (int) 0;
};


/*
 * shellWaitCmd: 
 *     Espera completar o comando com um [ENTER].
 *     @todo: Isso pode travar o Boot Loader?!
 */
 
void shellWaitCmd (){
	
wcLoop:  
    
	if (g_cmd_status == 1)
	{
       g_cmd_status = 0;
       return;
    };	
	
	goto wcLoop;
};



/*
 * shellCompare: 
 *     Compara comandos.
 *     Obs: O mini-shell do Boot Loader N�O deve oferece muitos comandos.
 */
 
unsigned long shellCompare (){
	
    unsigned long ret_value;
	
	
	// Espera o comando terminar.
	
	shellWaitCmd();	

palavra_reservada:

    //Cls.
    if ( strncmp( prompt, "cls", 3 ) == 0 )
	{ 
	    bl_clear(0);    
        goto exit_cmp;
	};
	
	//Help.
    if( strncmp( prompt, "help", 4 ) == 0 ){
		//printf(help_string);
		shellHelp();
		goto exit_cmp;
    };

	//Format.
	if( strncmp( prompt, "format", 6 ) == 0 ){
	    printf("~format\n");
		fs_format(); 
        goto exit_cmp;
    };

	//Dir.
	if( strncmp( prompt, "dir", 3 ) == 0 ){
	    printf("~dir\n");
		fs_show_dir(0); 
        goto exit_cmp;
    };	
	
	//cria arquivos e diretorios principais..
	if( strncmp( prompt, "makeboot", 8 ) == 0 )
	{
	    printf("~makeboot\n");
		
		ret_value = fs_makeboot();
		if(ret_value != 0){
		    printf("shell: makeboot fail");
		};
        goto exit_cmp;
    };
		
	if( strncmp( prompt, "debug", 5 ) == 0 ){
	    printf("~debug\n");
		debug();
        goto exit_cmp;
    };
	
	// newfile
	if( strncmp( prompt, "newfile", 7 ) == 0 ){
	    printf("~newfile - create empty file\n");
		fsCreateFile( "novo    txt", 0);
        goto exit_cmp;
    };
	
	// newdir
	if( strncmp( prompt, "newdir", 7 ) == 0 ){
	    printf("~newdir - create empty folder\n");
		fsCreateDir( "novo    dir", 0);
        goto exit_cmp;
    };
	
    //testa mbr
    if( strncmp( prompt, "mbr", 3 ) == 0 ){
	    printf("~mbr\n");
		testa_mbr();
		goto exit_cmp;
    }; 
	
    //testa /root
    if( strncmp( prompt, "root", 4 ) == 0 ){
	    printf("~/root\n");
		testa_root();
		goto exit_cmp;
    }; 

	
    if( strncmp( prompt, "start", 5 ) == 0 ){
	    printf("~start\n");
		goto exit_cmp;
    }; 
	
    if( strncmp( prompt, "hd", 2 ) == 0 ){
	    printf("~hd\n");
        goto exit_cmp;
    };
	
	
	if( strncmp( prompt, "save", 4 ) == 0 ){
	    printf("~save root\n");
        goto exit_cmp;
    };
	
	//muda um arquivo da area de transferencia para 
	//o sistema de arquivos...
	if( strncmp( prompt, "install", 7 ) == 0 ){
	    printf("~install\n");
		fs_install();
        goto exit_cmp;
    };
	
	
	//boot - inicia o sistema carregado
	if( strncmp( prompt, "boot", 4 ) == 0 ){
	    printf("~boot\n");
		boot();
        goto exit_cmp;
    };
	
	//Exit.
    if( strncmp( prompt, "exit", 4 ) == 0 ){
        printf("~exit\n");
		shell_status = 0;
		goto exit_cmp;
    };

	//Reboot.
    if( strncmp( prompt, "reboot", 6 ) == 0 ){
	    printf("~reboot\n");
		reboot();
        goto exit_cmp;
    };

	
	//
	// Continua...
	//
	
	
palavra_nao_reservada:
	return 1;
	
exit_cmp: 
    return 0;
};


void shellHelp (){
	
	printf("\n help, format, install, makeboot, reboot\n");
    //return;
};


/* boot: Realiza o boot do sistema. */

void boot (){
    
    /*
	 * @todo: Chama a re-inicializa��o do bootloader.
	 *
	 */ 

	//return;
};

			 
/*
 * debug: 
 *    Uma rotina de debug do bootloader.
 *
 * Objetivo:
 *    Checar a consistencia dos elementos de inicializa�ao.
 *    Efetuar corre��es poss�veis no processo de inicializa��o.
 *    Por exemplo, se h� uma falha no mbr, no bootmanager, ou no sistema
 *    de arquivos, a falha pode ser reportada ou corrigida.
 *
 */
 
void debug (){
	
    //procura arquivos corrompidos
	
	fs_check_cluster(0);
    
	fsCheckFat();
	
	//fs_test_fat_vector();
	
	
	//mostra os arquivos do diretorio raiz.
    //fs_show_dir(0);	
	
	
//done:

#ifdef BL_VERBOSE	
	printf ("debug: Done \n");
#endif
	
	//return;
};


void testa_mbr (){
	
    my_read_hd_sector ( MBR_ADDRESS, MBR_LBA, 0, 0 );
	
    printf ("%s", MBR_ADDRESS );
	
    //return;   
};


// Mudar nome.
void testa_root (){
	
    my_read_hd_sector ( FAT16_ROOTDIR_ADDRESS, FAT16_ROOTDIR_LBA, 0, 0 );
	
    printf ("%s", FAT16_ROOTDIR_ADDRESS );
	
	//return;   
};


/* reboot: Reboot. */

void reboot (){
	
	// #bugbug
	// Devemos chamar asm_reboot()
	
    asm_shut_down();
};


/* init_shell: Inicializa o shell do Boot Loader. */

void shellInit (){
	
    // Checa quem esta tentando inicializar o shell.
	
	//Prompt
	prompt[0] = (char) '\0';
	prompt_pos = 0;
	
	//cursor
	//g_cursor_y = 0;
	//g_cursor_x = 0;
	
//done:

    ShellInitialized = 1;
    
	//return;
};


//
// End.
//
