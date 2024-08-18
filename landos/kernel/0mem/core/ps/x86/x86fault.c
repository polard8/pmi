/*
 * File: ps/x86/x86fault.c
 * 
 *     x86 faults.
 *
 * History:
 *     2015 - Created by Fred Nora.
 */


// See: 
// https://wiki.osdev.org/Exceptions
// ...



#include <kernel.h>


extern unsigned long get_page_fault_adr (void);

//
// # Flags #
//

// Pagina��o sob demanda.
// Se essa flag estiver acionada, estamos avisando que devemos 
// continuar com o sistema pois estamos apenas alocando uma p�gina faltante.
int demand_paging_flag;

// Estamos fechando uma thread que apresentou problemas.
int thread_failed_flag;


// Estamos sinalizando que n�o h� mais o que fazer.
// Devemos parar.
int fatal_error_flag;


// Prot�tipo de fun��o interna.
void do_pagefault (void);


//
// Re-Entrant Exceptions ?
// Avoiding infinity loop.
//

int x86fault_InProgress_CurrentException;  // exception number.
int x86fault_InProgress_Finished;          // finifhed or not.


void x86fault_initialize(void)
{

    // Re-Entrant Exceptions ?
    // Avoiding infinity loop.

    x86fault_InProgress_CurrentException = -1;
    x86fault_InProgress_Finished = TRUE;

    // ...
}


/*
 *****************************************
 * faults:
 *
 *    x86 faults and exceptions.
 *    [0~31]
 * 
 *    #importante:
 *    Quem chamou isso? Pois temos a inten��o de retornar,
 *    inicializando outra a mesma thread, as agora com a inclus�o da 
 *    p�gina faltante, ou reinicializar com outra thread, pois fechamos 
 *    a que estava com problemas.  
 *    Por fim ainda temos o caso quando n�o iremos retornar.
 */

// #bugbug
// Actualy this routine is for all the non maskable interrupts
// mapped in the system.
// Some of the non maskable interrupts are reserved,
// Here comes only the not reserved ones.

void faults ( unsigned long number ){

    struct thread_d  *t;

    unsigned long Step=0;
    unsigned long upRunningThreads = (unsigned long) UPProcessorBlock.threads_counter;

    // ??
    // Isso impede a reentrada ??

    asm ("cli");

    // Hello
    debug_print ("faults: [x86]\n");

    // ul
    if (number > 31){
        debug_print ("faults: [ERROR] Invalid number\n");
    }  


    kprintf ("\n\n ======================= \n\n");
    kprintf ("x86fault-faults: *FAULTS: totalticks=%d \n\n", jiffies );


    //
    // re-entrant exception support
    //

    // ==================

    // #todo
    // This is a work in progress.

    if (x86fault_InProgress_CurrentException == number ){
        debug_print ("faults: [ERROR] re-entrant exception, number\n");
    }
    x86fault_InProgress_CurrentException = number;

    if (x86fault_InProgress_Finished == FALSE ){
        debug_print ("faults: [ERROR] re-entrant exception, not finished\n");
    }
    x86fault_InProgress_Finished = FALSE;

    // ==================
 



	//
	// Thread.
	//
	
	// #test
	// Filtrar current_thread.
	// #todo max.

    if ( current_thread < 0 ){
        printf ("x86fault-faults: [FAIL] current_thread\n");
        goto fail;
    }

    t = (void *) threadList[current_thread];

    if ( (void *) t == NULL ){
        printf ("x86fault-faults: [FAIL] t\n");
        goto fail;
    }

    if ( t->used != TRUE || t->magic != 1234 )
    {
        printf ("x86fault-faults: [FAIL] t validation\n");
        goto fail;
    }

//
// Get thread info:
//

    Step = (unsigned long) t->step;
    // ...


    // Salva o contexto se a tarefa j� esteve rodando.

    // #bugbug
    // Devemos salvar tamb�m quando ainda n�o rodou, pois
    // podemos errar na inicializa��o da thread.
        
        /* 
        if ( ProcessorBlock.threads_counter >= 1 && 
		     t->step > 0 )
		{
            // #importante
            // pegamos o contexto de vari�veis globais e colocamos na estrutura
            // para poder exib�-los
            // Tem que salvar.
            
            printf ("faults: Saving context\n");
			save_current_context ();    
	    }
        */
        
        // salvando os registradores
        // que refletem o momento em que houve a exce��o.
        // Isso � perfeito para a int 3.
    printf ("x86fault-faults: [DEBUG] Saving context\n");
    save_current_context();

    printf ("Number = {%d}\n", number); 
    printf ("TID %d | Step %d \n", current_thread, Step );
    printf ("UP Running Threads: %d \n", upRunningThreads );

    //printf ("Init Phase %d \n", KeInitPhase);
    //printf ("logonStatus %d | guiStatus %d \n", logonStatus, guiStatus );

    refresh_screen(); 

    // OP��O. 
    // KiInformation ();

	
	// Mostra erro de acordo com o n�mero.

    switch (number){

       // EXCEPTION
       case 1:
            printf("EXCEPTION\n");
            show_reg (current_thread);
            break;


        // Debug breakpoint interrupt.
        case 3:
            printf("BREAKPOINT\n");
            show_slot (current_thread);
            show_reg (current_thread);
            break;

        // DOUBLE FAULT
        // #todo: dump tss
        case 8:
            printf("DOUBLE FAULT\n");
            show_slot (current_thread);
            break;

        // STACK
        case 12:
            printf("STACK FAULT\n");
            show_reg (current_thread);
            break;

        // GP
        case 13:
            printf("GP\n");
            show_reg (current_thread);
            break;

        // PAGE FAULT
        // Obs: � o contr�rio de page hit.
        // #todo: Check current instruction info.
        case 14:
            printf ("PF\n");
            do_pagefault();
            break;
        
        // #test
        //  Floating point error.
        case 16:
            printf ("FP error\n");
            break;
        
            

        // ...

        default:
            printf("Default number\n");
            show_reg (current_thread);
            //show_slots();
            show_slot (current_thread);
            break;
    };

    //
    // # Flags #
    //


    /*
	if( fatal_error_flag == 1 ){
        
		// We can't do anything else.
		goto done;			
	}else if ( demand_paging_flag == 1 ){
		
		// Uma p�gina faltante foi atribu�da � uma threa.
		goto tryagain;
	}else if ( thread_failed_flag == 1 ) {
		
		// Uma thread falhou, ent�o fechamos ela e vamos tentar continuar.
		goto tryagain;
	};
    */


    //
    // die or continue running.
    //


done:
    // Reinicializa o gerenciamento e faltas reentrantes
    // x86fault_initialize();

    //printf("done");
    //die();

fail:
    //printf("# FAIL #");
    //die();

tryagain:
    printf ("*HANG\n");
    //refresh_screen();

	// #debug
	// N�o estamos continuando por enquanto pois 
	// ainda estamos iplementando isso.
	// See: core/system.c

    die();
}


/* Interface */
 
void KiCpuFaults ( unsigned long number )
{
	// #todo
	// Checar a validade do argumento.

    faults (number);
}


/*
 ******************************
 * do_pagefault:
 *     14 PAGE FAULT.
 *     Obs: Podemos apenas fechar a thread atual e retomar o sistema.
 */

void do_pagefault (void){

    unsigned long page_fault_address=0;

	// We can't close idle thread.
	// Se a thread que apresentou problemas foi a idle.
	// uma op��o � tentar recri�-la.
	
    /*
	if ( current_thread == idle )
	{
		printf("do_pagefault: We can't close idle thread \n");
		fatal_error_flag = 1;
		return;
	};
	*/
	
	// #importante
    // Devemos ter informa��es na estrutura da thread que indiquem 
	// se todas as p�ginas foram atribu�das � ela ou n�o na hora 
	// do carregamento ...
    // Pois assim a rotina de pagefault poder� saber se deve 
	// realizar a pagina��o sob demanda ou fechar a thread.

    printf ("do_pagefault:\n");
	
	// #todo:
	// +Checar se a current � uma thread v�lida.
	// +Checar se a current est� esperando por pagina��o sob demanda.
	// +Pegar o endere�o.
	// +Realizar o mapeamento da p�gina faltante.
	
	//Page Fault Linear Address - PFLA.

    page_fault_address = (unsigned long) get_page_fault_adr();

    printf ("Address={%x}\n", 
        (unsigned long) page_fault_address );

    //
    // Mostra registradores.
    //
    
    // #bugbug
    // Precisamos mudar os nomes.

    //if (current_thread<0)
    //    return;

    show_reg(current_thread);


    show_slots();


	/*
	// Se o endere�o for igual a 0 � porque o eip est� perdido.
	// Devemos fechar a thread.
	if( page_fault_address == 0 )
	{
		//tentando apenas terminar a thread, se isso for poss�vel.
		printf ("Killing thread %d \n" ,current_thread );
		kill_thread ( current_thread );
		current_thread = idle;
        thread_failed_flag = 1;
		return;
	}
	*/
	
	
	// #todo
	// podemos checar se o processo atual eh um processo em ring3
	// e tentar fechar ele.
	// Se ele for window server, entao precisamos desregistrar
	// o window server para que as mensagens do sistema
	// possam ir para os outros processos. Depois podemos
	// reabilitar o ws.
	// Mesma coisa para o network server ...
	//
	
}



//
// End.
//

