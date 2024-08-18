/*
 * File: ps/sched/sched.c
 *
 * Descri��o:
 *     O escalonador de processos do Kernel.
 *     Faz parte do Process Manager, parte fundamental do Kernel Base.
 *     The kernel schedules threads, not processes.
 *     Priority: +boosts for GUI foreground.
 *
 * Atribui��es:
 *     + Salva o contexto da tarefa OLD.
 *     + Incrementa a tarefa atual. (RR).
 *     + Retorna o contexto da tarefa NEW.
 *
 * Obs:
 *     O Scheduler n�o salva nem restaura contexto de tarefa.
 * Apenas troca a tarefa atual e faz um agendamento das pr�ximas
 * threads a rodarem.
 *     Ser� poss�vel usar mais de um m�todo de escalonamento. No momento,
 * o m�todo usado � escalonamento Round Robin, preemptivo,
 * com m�ltiplas filas e prioridade.
 *
 * @todo:
 *     N�o por interface nessa rotina. Perde velocidade.
 *
 * History:
 *     2015 - Created by Fred Nora.
 *     //...
 */


 /*
  * #importante:
  * Para implementarmos um scheduler como um processo, devemos separar 
  * as fun��es em fun��es de biblioteca e fun��es primitivas, que ser�o 
  * aquelas presentes no stub do scheduler. As fun��es primitivas 
  * ser�o as mesmas para todos os schedulers que criarmos.
  */
 

#include <kernel.h>


int schedulerType=0;
unsigned long schedulerQueue[4]; 

// ========================================

/*
 * pick_next_thread:
 *     Selecionamos a 'next_thread' olhando nas filas em QUEUE[].
 *     QUEUE[] cont�m ponteiros para listas encadeadas.
 *     Se n�o tiver nada nas filas ent�o usaremos a InitThread.
 *
 *     #bugbug: Essa InitThread est� configurada corretamente. ?
 *     #bugbug: E se n�o existir IfleThread? 
 */

struct thread_d *pick_next_thread (void){

    struct thread_d *t;
    struct thread_d *next;

    int i=0;      //id da fila selecionada
    int old=0;    //salva id da current thread


	//salva antiga thread
    old = next_thread;
	
	
	//se temos um ponteiro para fila de drivers.
    if ( QUEUES[0] != 0 ){
        i = 0;

	//se temos um ponteiro para a fila de servidores.

    } else if ( QUEUES[1] != 0 ){
        i = 1;

	//nos resta a fila de apps de usu�rio.
    }else{
        i = 2;
    };


	//
	// # Checando o conductor. #
	//


	//se o elemento tem um valor n�o nulo..
    if (QUEUES[i] != 0)
    {
	    //Ok temos uma fila.
		//vamos pegar a primeira thread da fila.
		
        t = (void *) QUEUES[i];

    }else{

		// nenhuma thread est'a no estado de READY ... entao nenhuma das
		// filas foi construida.
		// nos reata usarmos a thread idle. 
		
		//podemos fica nessa condi�~ao at'e que uma thread seja acordada ...
		//talvez ela esteja esperando alguma recurso,.
		//quando ela acordar ir'a pra alguma fila.
		
		
		
		// #todo
		// Mudar para ____IDLE;
		// Selecionamos a idle.
		// Isso est� errado, estamos selecionando a thread de controle
		// do processo init.bin, mas queremos a idle do kernel.
        t = InitThread;
        //t = ____IDLE;
    };



	//
	// Checando a thread selecionada.
	//

    if ( ( void *) t == NULL ){
		next_thread = old;

    }else{

        // #bugbug
        // Se a estrutura falhou ent�o n�o podemos usar essa thread.
        // Tem que abortar a tentativa.
        
        if ( t->used != 1 || t->magic != 1234 )
        {
            next_thread = old;
            goto prepare_next;
        }

		//Ok.
        next_thread = t->tid;
    };


prepare_next:

	//
	// # Checando a validade da next thread. # 
	// THREAD_COUNT_MAX
	//

    next = (void *) threadList[next_thread];

    if ( ( void *) next == NULL )
    {
	    //fail
	    // #debug
	    //N�o conseguimos selecionar nenhuma thread como pr�xima.
		//n�o temos nem mesmo uma thread idle para inicializarmos o round.
			
		//#debug
		//printf ("#DEBUG\n");
        printf ("pick_next_thread: No next_thread, we could't initialize the round\n");
        die ();

    }else{

        if ( next->used != 1 || next->magic != 1234 )
        {
	        //fail
	        // #debug
	        //N�o conseguimos selecionar nenhuma thread como pr�xima.
		    //n�o temos nem mesmo uma thread idle para inicializarmos o round.
		    //#debug
			//printf ("#DEBUG\n");
            printf ("pick_next_thread: No next_thread, we could't initialize the round\n");
            die ();
        }

        if ( next->state == READY ){
            return (struct thread_d *) next;
        }
    };


    return NULL;
}


// Double shot the current thread.
void sched_double_shot(void)
{
    struct thread_d  *t;

    // #todo:
    // Check max limit
    
    if (current_thread < 0 || current_thread >= THREAD_COUNT_MAX)
    {
        return;  //fail
    }

    t = (void *) threadList[current_thread];
    
    if ( (void*) t == NULL ){ return; }
    
    if ( t->used != TRUE || t->magic != 1234 )
    {
        return;  //fail
    }
    
    t->DoubleShot = TRUE;
}



/*
 *********************************************************
 * scheduler:
 *    Troca a thread atual, escolhe uma nova thread atual 
 * para rodar no momento.
 *    O m�todo � cooperativo, Round Robing.
 *
 * Ordem de escolha:
 * ================
 *  +fase 1 - Pega a pr�xima indicada na estrutura.
 *  +fase 2 - Pega nos slots a de maior prioridade.
 *  +fase 3 - Pega a Idle. 
 *            @todo: Nessa fase devemos usar a idle atual, 
 *            indicada em current_idle_thread.  
 *  //...
 *
 * Obs:
 *     O que estamos fazendo aqui � incrementar a tarefa atual e olhando se a
 * pr�xima tarefa da lista threadList[] est� pronta pra rodar.
 *
 * Obs:
 *     Pega na fila ReadyQueue.
 *     O scheduler deve sempre pegar da fila do dispatcher.
 */
 
// #todo
// Podemos contar os rounds.
// Obs: 
// ## IMPORTANTE  ##
// A thread idle somente � usada quando o sistema 
// estiver ocioso ou quando ela for a �nica thread.
// E � importante que a thread idle seja usada, pois 
// ela tem as instru��es sti/hlt que atenua a utiliza��o 
// da CPU, reduzindo o consumo de energia.


int scheduler (void){

    int FirstTID = -1;

    // loop
    register int i=0;


    struct thread_d  *TmpThread;


#ifdef SERIAL_DEBUG_VERBOSE
    debug_print(" [*SCHEDULER*] ");
#endif


//
// rootConductor: 
// The ring0 idle thread.
//

    rootConductor = (void *) ____IDLE;

    // Check
    if ( (void*) rootConductor == NULL )
        panic ("scheduler: rootConductor\n");

    if (rootConductor->used != TRUE || rootConductor->magic != 1234)
        panic ("scheduler: rootConductor validation\n");

    FirstTID = (int) rootConductor->tid;

    if ( FirstTID < 0 || FirstTID >= THREAD_COUNT_MAX )
        panic ("scheduler: FirstTID\n");

//
// rootConductor->next: 
// The control thread of the ring3 init process.
//

    if ( (void*) InitProcess == NULL )
        panic ("scheduler: InitProcess\n");

    if ( (void*) InitProcess->control == NULL )
        panic ("scheduler: InitProcess->control\n");

    rootConductor->next = (void*) InitProcess->control;

    // Check
    if ( (void*) rootConductor->next == NULL )
        panic ("scheduler: rootConductor->next\n");

//
// Conductor
//

    Conductor = (void *) rootConductor;

//
// Conductor->next
//

    Conductor->next = (void *) rootConductor;

//
// tmpConductor
//

    tmpConductor = (void *) rootConductor;

//
// tmpConductor->next
//

    tmpConductor->next = (void *) rootConductor;


//
// Walking ...
//

    // READY threads in the threadList[].
 
    for ( i=0; i < THREAD_COUNT_MAX; ++i )
    {
        TmpThread = (void *) threadList[i];

        if ( (void *) TmpThread != NULL )
        {
            // Single shot
            if ( TmpThread->used  == TRUE && 
                 TmpThread->magic == 1234 && 
                 TmpThread->state == READY )
            {
                // The tmpConductor and it's next.
                tmpConductor       = (void *) tmpConductor->next; 
                tmpConductor->next = (void *) TmpThread;
            }
        }
    };


    // #todo
    // Let's try some other lists.

    // Finalizing the list.
    // The tmpConductor and it's next.
    tmpConductor       = (void *) tmpConductor->next; 
    tmpConductor->next = NULL;


// done:

    // Start with the idle thread.

    return (int) FirstTID;
}


// #deprecated
void scheduler_start (void)
{
    // #maybe
    // scheduler_unlock();

    panic("scheduler_start: deprecated\n");
}


// Lock scheduler
void scheduler_lock (void){
    g_scheduler_status = (unsigned long) LOCKED;
}

// Unlock scheduler
void scheduler_unlock (void)
{
    g_scheduler_status = (unsigned long) UNLOCKED;
}


/*
 * scheduler_get_status:
 *     Pega o status do scheduler, se ele est� travado ou n�o.
 */
 
unsigned long scheduler_get_status (void)
{
    return (unsigned long) g_scheduler_status;
}


/*
 ********************************
 * init_scheduler:
 *    Inicaliza o scheduler.
 *    @todo: Mudar para schedulerInit()
 *    #burbug: faz o mesmo que scheduler_start.
 */

	// #todo: 
	// Implementar inicializa��o de variaveis do scheduler.
	// O nome poderia ser schedulerInit().
	// Formato de classes.Init � um m�todo. 

// Called by init_microkernel in mk.c

void init_scheduler (void)
{
    debug_print ("init_scheduler: [TODO]\n");
    // ...
    
    scheduler_lock();
    
    // #bugbug
    // Maybe there is no IDLE defined 
    // at this point of the initialization.
    
    //set_current (IDLE);
}


//
// End.
//

