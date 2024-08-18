/*
 * File: init.c 
 *
 * Descri��o:
 *     Arquivo principal do m�dulo INIT kernel.
 *     Faz inicializa��es.
 *     Esse m�dulo est� em kernel base.
 *
 * In this file:
 *    +init
 *    +init_test_disk
 *    +init_video
 *    +init_globals
 *
 * History
 *     2015 - Created by Fred Nora.
 *     //...
 */ 


#include <kernel.h>


//Onde ficam os c�digos e arquivos de configura��o usados na inicializa��o.
//A ideia � que se a inicializa��o precisar de algum arquivo, deve procur�-lo
//nos diret�rios previamente combinados.
//melhor que sejam poucos e que sempre sejam os mesmos.

#define INIT_ROOT_PATH  "/"
#define INIT_BIN_PATH   "/BIN"
//...


//externs
//@todo:
//isso dever� ser definido em /gramado.
//extern char *GramadoSystemRoot;
//extern char *GramadoMajorVersion;
//extern char *GramadoMinorVersion;
//extern char *GramadoVersionString;
//...


//Vari�veis internas.
//int initStatus;
//...


/*
void init_set_current_runlevel ( int number )
void init_set_current_runlevel ( int number )
{
	//#todo: limits.
    current_runlevel = number;
}
*/

/*
int init_get_current_runlevel (void)
int init_get_current_runlevel (void)
{
    return (int) current_runlevel;
}
*/


// Deprecated
void save_kernel_args (void)
{
    panic ("save_kernel_args: DEPRECATED");
}



/*
 **************************************************
 * init_globals:
 *     Init globals. 
 *     Inicia vari�veis globais do Kernel Base.
 *     Obs: Inicializar por categorias.
 */

void init_globals (void){


    int i=0;


//#ifdef EXECVE_VERBOSE
    debug_print("init_globals:\n");
    printf     ("init_globals:\n");
//#endif


    // stdio
    // Inicializa as estruturas do fluxo padr�o.
    // Isso vai usar a file table.
    // #todo: Mudar para kstdioInitialize.

    stdioInitialize();

    // Screen
    // Now we can print strings in the screen.

    screenInit();

//
// == First message ====================================
//

    // #todo
    // Maybe we can print the banner.


    debug_print("init_globals: WE HAVE MESSAGES NOW!\n");
    printf     ("init_globals: WE HAVE MESSAGES NOW!\n");


    // ===================
    // Vamos atrasar configura��o de janela 
    // em favor de configura��o de mensagem

    // ??
    // What is this?
    // Atalho para a pr�xima mensagem de teclado..(test) debug
    gNextKeyboardMessage = (int) 0;

    // Essa flag bloqueia e impede que uma janela obtenha o foco.
    gFocusBlocked = (int) 0;

//
// == Profiler =============================
//

// See: 
// include/kernel/profiler/pints.h


    // Legacy hardware interrupts (irqs) (legacy pic)
    g_profiler_ints_irq0  = 0;
    g_profiler_ints_irq1  = 0;
    g_profiler_ints_irq2  = 0;
    g_profiler_ints_irq3  = 0;
    g_profiler_ints_irq4  = 0;
    g_profiler_ints_irq5  = 0;
    g_profiler_ints_irq6  = 0;
    g_profiler_ints_irq7  = 0;
    g_profiler_ints_irq8  = 0;
    g_profiler_ints_irq9  = 0;
    g_profiler_ints_irq10 = 0;
    g_profiler_ints_irq11 = 0;
    g_profiler_ints_irq12 = 0;
    g_profiler_ints_irq13 = 0;
    g_profiler_ints_irq14 = 0;
    g_profiler_ints_irq15 = 0;
    // ...

    // Interrup��o para servi�os do sistema.
    g_profiler_ints_gde_services = 0;


//
// == User ===========================================
//

	// As globais relativas � usu�rio s�o independentes do 
	// ambiente gr�fico.

    // =========================================
    //User and group.
    current_user  = 0;
    current_group = 0;


    // =========================================
    // Security layers.
    // user session, room(window station), desktop.
    current_usersession = (int) 0;
    current_room        = (int) 0;
    current_desktop     = (int) 0;


//
// == ps ===================================================
//

    // =========================================
    // Process, Thread.
    // See: kernel.h
    
    foreground_process = (int) 0;
    foreground_thread  = (int) 0;
    
    current_process    = (int) 0;
    current_thread     = (int) 0;


//
// == Network ===================================
//

    // =========================================
    // Initialize the ports table used by the socket infrastruture.
    // todo: Create socket_init_gramado_ports();
    for (i=0; i<GRAMADO_PORT_MAX; i++)
    {
        gramado_ports[i] = 0;
    };

//
// == Kernel requests ===================================
//

    // The kernel request
    // See: request.c

    clear_request();


//
// == kgws ===========================================
//

    // =========================================
    if ( g_useGUI != TRUE ){
        panic("init_globals: No GUI\n");
    }

    // ===============================================
    // Window support.
 
    current_window    = (int) 0;  // Current Window.
    windows_count     = (int) 0;  // Window count.
    window_with_focus = (int) 0;  // Window with focus.
    current_menu      = (int) 0;  // Current Menu.
    // Continua ...

    // windows
    init_windows();
        
    // #todo: Delete?
    init_menus();

    // Continua...

    // ??
    // Messages.
    g_new_message = 0;


//
// == fs ==============================
//

    // ==============================
    // fs support

    // FS type.
    // type 1, fat16.
    
    g_currentvolume_filesystem_type = FS_TYPE_FAT16;    
    g_currentvolume_fatbits = (int) 16;



	//===================
	
	// #importante:
	// provavelmente isso altere o comportamento do cursor,
	// levando o cursor par ao in�cio da tela. Ent�o precisamos
	// repintar o background para recome�armos.
	
	//printf("#breakpoint glob");
    //refresh_screen(); 
    //while(1){}	
	
	//#bugbug isso esta dando problemas.
//#ifdef EXECVE_VERBOSE
    // backgroundDraw ( (unsigned long) COLOR_BLUE ); 
//#endif

    //printf("#breakpoint glob");
    //refresh_screen(); 
    //while(1){}

	//
	// Keyboard support.
	//
	
	//ldisc_init_modifier_keys();
	//ldisc_init_lock_keys();	


    //Continua ...
}



/*
 ******************************************
 * init:
 *     Base initializations.
 *     (Four phases).
 *     Isso foi chamado por systemStartUp() em core/system.c
 *   
 *    #todo: 
 *    Descrever aqui as fazer dessa rotina.
 */ 

// It was called by x86Main() in x86/x86init.c

int init (void){

    int Status = 0;
    unsigned char ProcessorType=0;


    debug_print ("==== core-init:\n");

    // Check kernel phase.

    if ( KeInitPhase != 0 )
    {
        debug_print ("core-init: KeInitPhase fail\n");
        panic       ("core-init: KeInitPhase fail\n");
    }


    PROGRESS("Kernel:2:1\n"); 
    // Globals.

    // #IMPORTANT
    // Globals.
    debug_print ("core-init: Globals\n");
    init_globals();

    // ==============================================================
    // #importante
    // � partir daqui podemos exibir strings na tela.
    // Ou usarmos a barra de progresso. Depende do runlevel.
    // ==============================================================



    PROGRESS("Kernel:2:2\n"); 
    // Create the progress bar.


#ifdef EXECVE_VERBOSE
    printk ("core-init: init_globals ok\n");     
#endif  


    PROGRESS("Kernel:2:3\n"); 
    // io manager

	//#bugbug:
	//Depois de iniciar as globais, provavelmente o cursor mude 
	//para o in�cio da tela.
	//ent�o ap�s iniciarmos as globais temos que repintar o background e 
	// fazer um refresh da tela se a flag de verbose estiver ligada.

    //Object manager.
#ifdef EXECVE_VERBOSE
    printk ("core-init: init_object_manager\n");
#endif

    debug_print ("core-init: Object manager\n");
    init_object_manager ();

    // i/o Manager.
#ifdef EXECVE_VERBOSE
    printk ("core-init: ioInit\n");
#endif

    debug_print ("core-init: io manager\n");
    ioInit ();


    PROGRESS("Kernel:2:4\n"); 
    // device manager.

    // Inicializa o gerenciamento de dispositivos.
    // Inicializa a lista de dispositivos.
    debug_print ("core-init: device manager\n");
    
    init_device_manager ();



    PROGRESS("Kernel:2:5\n"); 
    // storage manager


    // == STORAGE ===========================

    // #ordem:
    // +storage
    // +disk
    // +volume 
    // +file system

	//#importante 
	//A estrutura 'storage' vai ser o n�vel mais baixo.
	//� nela que as outras partes devem se basear.
    debug_print ("core-init: storage structure\n");
    
    storage = (void *) kmalloc ( sizeof(struct storage_d) );

    if ( (void *) storage == NULL ){
       panic ("core-init: storage");
    }


#ifdef EXECVE_VERBOSE
    printk ("core-init: disk_init\n");
#endif 

    debug_print ("core-init: disk\n");
    disk_init ();


#ifdef EXECVE_VERBOSE
    printk ("core-init: volume_init\n");
#endif

    debug_print ("core-init: volume\n");
    volume_init ();


#ifdef EXECVE_VERBOSE
     printk ("core-init: VFS..\n");
#endif

    debug_print ("core-init: vfs\n");
    vfsInit ();


// deletar
#ifdef EXECVE_VERBOSE
    printk ("core-init: fsInit\n");
#endif   

    debug_print ("core-init: fs\n");
    fsInit ();


    // #todo
    // Mount root fs.
    // See: mountedList[] in sci/fs/fs.c
    // #test: Vamos tentar montar o volume root em mountedList[0].
    debug_print ("core-init: [FIXME] Initialize mounted list in fs.c\n");


    PROGRESS("Kernel:2:6\n"); 
    // network

	//
	// Network
	//

    debug_print ("core-init: network\n");
    networkInit ();


    PROGRESS("Kernel:2:7\n"); 
    // Initialize Platform structure.


#ifdef EXECVE_VERBOSE
    printk ("core-init: Platform\n");
#endif

    // #important
    // This is the Root struct. :)
    debug_print ("core-init: Platform struct\n");
    
    Platform = (void *) kmalloc ( sizeof(struct platform_d) );

    if ( (void *) Platform ==  NULL ){
        panic ("core-init: Platform\n");
    }else{

        // UP or MP.
        Platform->system_type = SYSTEM_TYPE_NULL;

        // Hardware
        Hardware = (void *) kmalloc ( sizeof(struct hardware_d) );

        if ( (void *) Hardware ==  NULL ){
            panic ("core-init: Hardware\n");
        }else{
            Platform->Hardware = (void *) Hardware;
            //printf(".");
        };

		//Firmware
        Firmware = (void *) kmalloc ( sizeof(struct firmware_d) );

        if ((void *) Firmware ==  NULL ){
            panic ("core-init: Firmware\n");
        }else{
            Platform->Firmware = (void *) Firmware;
            //printf("."); 
        };


		//System (software)

		// #IMPORTATE: 
		// Aqui estamos inicializando a estrutura do systema.

        System = (void *) kmalloc ( sizeof(struct system_d) );

        if ( (void *) System ==  NULL ){
            panic ("core-init: System\n");
        }else{
            System->used  = TRUE;  // Sinaliza que a estrutura esta em uso.
            System->magic = 1234;  // Sinaliza que a estrutura n�o esta corrompida.
            
            Platform->System = (void *) System;
            //printf(".");
        };

		//printf(" Done!\n");	
		//...
    };


//
// ====================================================================
//


    // #important
    // We need to be in the phase 0.
    
    if (KeInitPhase != 0){
        panic ("init: KeInitPhase\n");
    }


    PROGRESS("Kernel:2:8\n"); 
    // hal


	// #bugbug
	// Se � HAL � dependente da arquitetura.
	// Isso deveria ficar na outra rotina.
	// N�o mudaremos por enquanto.

//#todo: Mudar o nome EXECVE_VERBOSE
#ifdef EXECVE_VERBOSE
    printk ("init: Initializing HAL..\n");
#endif

    // #bugbug
    // Isso eh dependente, pode mudar para a outra rotina desse documento.

    Status = init_hal();

    if (Status != 0){
        panic ("init: init_hal fail\n");
    }


    PROGRESS("Kernel:2:9\n"); 
    // microkernel components:
    // mm, ipc, ps ...

    // ================
    // Microkernel:
#ifdef EXECVE_VERBOSE
	// Obs: O Microkernel lida com informa��es dependentes da arquitetura,
	// por�m inicializa a gerencia de processos e threads e de comunica��o
	//entre processos.
	//#bugbug @todo: Se � microkernel � processo � registrador ... 
	// acho que leva em considera��o a arquitetura.
    printk ("init_architecture_independent: Initializing Microkernel..\n");
#endif

    // Isso tambem eh dependente, pode ir para a outra rotina, 
    // nesse mesmo documento.
    Status = init_microkernel();

    if (Status != 0){
        panic ("init_architecture_independent: init_microkernel fail\n");
    }


    PROGRESS("Kernel:2:10\n"); 
    // Executive components

    // =====================
    // Executive:
#ifdef EXECVE_VERBOSE
    printk ("init_architecture_independent: Initializing Executive..\n");
#endif

    Status = init_executive();

    if (Status != 0){
        panic ("init_architecture_independent: init_executive\n"); 
    }

    // =====================
    // Gramado:
#ifdef EXECVE_VERBOSE
    printk ("init_architecture_independent: Initializing Gramado..\n");
#endif


    PROGRESS("Kernel:2:11\n"); 
    // some gui components.
    // #todo: rever 

    // #bugbug
    // Deprecated?
    // onde?

    Status = init_gramado();

    if (Status != 0){
        panic ("init_architecture_independent: init_gramado fail\n"); 
    }


    PROGRESS("Kernel:2:12\n"); 
    // window manager

    //
    // == window manager ================================
    //

    // #important:
    // This is the window manager embedded in the base kernel.

    // Text mode not supported.

    if (g_useGUI != TRUE){
        panic ("init_architecture_independent: [PANIC] No ring0 GUI!\n");
    }


#ifdef EXECVE_VERBOSE
    printk ("init_architecture_independent: init_window_manager\n");
#endif

    // debug
    //printf("W\n");
    //refresh_screen();
    //while(1){}
    
    init_window_manager();

    // debug
    //printf("~W\n");
    //refresh_screen();
    //while(1){}


    // More? ...


//done:

#ifdef EXECVE_VERBOSE
    printk ("init_architecture_independent: Done\n");
    //refresh_screen();
    //while(1){}
#endif


//
// ====================================================================
//


    printf("=========================\n");
    printf("core-init: end of phase 0\n");
    

    //
    // == phase 1 ? ================================================
    //

    KeInitPhase = 1;


//
// ====================================================================
//


    // Fase 2: 
    // Inicia a parte de arquitetura especifica da m�quina atual.
    // Ou seja, considera a marca do processador.
    
    //Status = (int) init_architecture_dependent ();
    //if (Status != 0){
    //    panic ("core-init: init_architecture_dependent fail\n"); 
    //}



    //debug_print ("init:\n");

    //
    // Fase. 
    // (Verificar se essa rotina foi chamada na fase certa 
    // de inicializa��o.)
    //

    if ( KeInitPhase != 1 ){
        panic ("init: KeInitPhase\n");
    }


	// #### IMPORTANTE ####
	//
	// VAMOS ANTECIPAR ESSA INICIALIZA��O NA TENTATIVA DE
	// ANTECIPARMOS O USO DE MENSAGENS.
    // >>> mas essa rotina precisa do kmalloc ,,,
	//ent�o tem que ser depois da inicializa��o do stdio.
	
	
	// Os par�metros de tela dependem das propriedades de hardware
	// como monitor e placa de v�deo.
	
	//screenInit();

    //printf("init_architecture_dependent: #Debug");
    //refresh_screen();
    //while(1){};		
	


    PROGRESS("Kernel:2:13\n"); 
    // processor structure.


    //
    // == Processor ===================================
    //

    // The 'processor' structure.
    // ?? Is it 'up' or 'smp' ?

    processor = (void *) kmalloc ( sizeof( struct processor_d ) ); 

    if ( (void *) processor == NULL ){
        panic("init: processor\n");
    }


    // #todo
    // Check if cpuid instruction is available.
    // See: _x86_test_cpuid_support on bootmx/headlib.asm
    // #todo: extern int x86_test_cpuid_support(void);



    // Sonda pra ver qual � a marca do processador.
    // #todo: 
    // � a segunda vez que fazemos a sondagem ?!
    // See: hal/detect.c
    // This routine is valid for intel and amd processors.
    // Ok.
    // Let's make some initialization and 
    // get more information about the processor
    // using the cpuid instruction.
    // See: 
    // 1pump/arch/x86/x86.c
    // 1pump/arch/amd/cpuamd.c

    ProcessorType = (int) hal_probe_processor_type();

    // Error
    if (ProcessorType <= 0){
        panic("init_architecture_dependent: [ERROR] ProcessorType\n");
    }

    processor->Type = (int) ProcessorType;

    switch (ProcessorType){
        case Processor_INTEL:  x86_init_intel();   break;
        case Processor_AMD:    init_amd();         break;
        // ...
        default:
            panic ("init_architecture_dependent: [ERROR] default Type\n");
            break;
    };


    //
    // qemu
    //

    // Check if we are running on qemu.
    // and set flag.

    int isQEMU=FALSE;
    isQEMU = detect_IsQEMU();
    if( isQEMU == TRUE ){
        printf ("Running on QEMU\n");
    }


    PROGRESS("Kernel:2:14\n"); 
    // process manager.

	// #obs: O contexto � depedente.
	// Inicializando o Process manager.

    init_process_manager();

    // Continua ...

    // Done.


    // debug
    //printf("~P\n");
    //refresh_screen();
    //while(1){}


//
// ====================================================================
//

    PROGRESS("Kernel:2:15\n"); 
    // Load root dir.
    // #todo: We can move this above to the 'storage' section.

    // #importante
    // S� podemos carregar o diret�rio raiz depois que 
    // inicializamos o controlador de IDE e as estruturas de 
    // sistema de arquivos.
    // #todo: Precisamos inicializar a estrutura de diretorio
    // usada pelo diretorio raiz, para colocarmos o ponteiro
    // na estrutura de processos, juntamente com o ponteiro
    // para o cwd. 
    // Talvez essa seja a hora de criarmos a estrutura para
    // o diretorio raiz, mas ainda nao temos estrutura de processo.

    debug_print ("core-init: load root dir.\n");
	// Carregando o diret�rio raiz.
    fs_load_rootdir( VOLUME1_ROOTDIR_ADDRESS, VOLUME1_ROOTDIR_LBA, 32 );


    // Disable interrupts, lock task switch and scheduler.

    // # isso ja foi feito no come�o da rotina de inicializa��o do kernel.
    // Tem que pensar nisso antes de tudo.

    //asm ("cli");
    //set_task_status(LOCKED); 
    //scheduler_lock();


    PROGRESS("Kernel:2:16\n"); 
    // keyboard stuff.

    // #todo
    // Talvez devamos antecipar isso, pois faz parte do teclado.
    
    ldisc_init_modifier_keys ();
    ldisc_init_lock_keys ();


    printf("=========================\n");
    printf("core-init: end of phase 1\n");


    //
    // == phase 2 ? ================================================
    //

    KeInitPhase = 2;


    //#debug
    // printf ("init: *breakpoint\n"); 
    // refresh_screen();
    // while(1){}	


    debug_print ("==== init: done\n");
    printf      ("==== init: done\n");


    // #debug 
    // A primeira mensagem s� aparece ap�s a inicializa��o da runtime.
    // por isso n�o deu pra limpar a tela antes.

#ifdef BREAKPOINT_TARGET_AFTER_INIT
    printk ("core-init: debug breakpoint after init");
    refresh_screen(); 
    asm ("cli");
    while (1){ asm ("hlt"); }
#endif

    // ??
    // Volta para onde?

    // #debug
    //printf("*breakpoint\n");
    //refresh_screen();
    //while(1){}

    return 0;  
}


//
// End.
//
