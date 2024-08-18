// thread.h
// Thread support.
// 2015 - Created by Fred Nora.


#ifndef __THREAD_H
#define __THREAD_H    1

#define THREAD_MAGIC  1234

//O primeiro �ndice na contagem das threads do sistema e o
//primeiro �ndice na contagem das threads dos usu�rios.
#define SYSTEM_BASE_TID  0  
#define USER_BASE_TID    100  

//
// Preempt support.
//

//?? Se pode ou n�o entrar em preemp��o. 
#define PREEMPTABLE    1    //PODE ENTRAR
#define UNPREEMPTABLE  0    //NAO PODE ENTRAR EM PREEMP��O


// ??
// #bugbug: rever isso!
// Identificadores de thread.
// thread 0 n�o significa idle thread.
#define IDLE_ID        0 
#define IDLE           IDLE_ID 
#define THREAD_IDLE    IDLE_ID  
// ... 


 
/*
 * Globais.
 */ 
 
 
 
/*
 * Estruturas para threads.
 */


	//#importante
	//raz�es para esperar
	//#todo: isso precisa ser inicializado.
	//@todo: tem que fazer um enum para enumerar as raz�es.
	//o �ndice � o selecionador da raz�o pela 
	//qual a thread est� esperando.
	//existem umas 20 raz�es pra esperar.
	// 0 - esperando por mensagem.(presa num loop)
	// 1 - esperando outra thread finalizar. wait4tid
	// 2 - esperando um processo finalizar. wait4pid
	// 3 - esperando um objeto. (BLOCKED)(espera gen�rica)
	// ...

// #importante
// isso ser� usado na estrutura de thread em wait_reason[]
// Com limite de 10 por enquanto.

typedef enum {
	WAIT_REASON_NULL,
	WAIT_REASON_LOOP,           
	WAIT_REASON_EXIT,
	WAIT_REASON_BLOCKED,
	WAIT_REASON_PREMMPTED,      // ?? Esperando a preemp��o de thread de menor prio.
	WAIT_REASON_SEMAPHORE,      // ?? Sem�foro.
	WAIT_REASON_WAIT4PID,       // Esperando o processo filho morrer.
	WAIT_REASON_WAIT4TID,       // Esperando uma thread morrer.
	WAIT_REASON_TEST            // # Usada pelo desenvolvedor para testes.
	// ...
}thread_wait_reason_t;




/*
 * thread_type_t:
 *     Enumerando os tipos de threads:
 */

typedef enum {
    THREAD_TYPE_NULL,
    THREAD_TYPE_SYSTEM,     // first-come-first-served.
    THREAD_TYPE_IDLE,       // 
    THREAD_TYPE_PERIODIC,   // periodic threads with predefined intervals.
    THREAD_TYPE_RR,         // first-come-first-served cooperative.
    THREAD_TYPE_REALTIME,
    THREAD_TYPE_UI, 
    THREAD_TYPE_IO, 
    // ...
}thread_type_t;



/*
This part is about input redirection ...
What is the place where i need to send the 
input that came from the hardware?
If the input model is an unix-like stuff, 
so i need to send the input to a file and 
an application will read this file.
if the input model is windows-like, 
so i will send the input to an event queue in the raw input thread.
*/

typedef enum {
    THREAD_INPUTMODEL_NULL,      // Not defined.
    THREAD_INPUTMODEL_KERNEL,    // Kernel thread running in ring0.
    THREAD_INPUTMODEL_COMMAND,   // Command running on console or terminal.
    THREAD_INPUTMODEL_UNIXLIKE,  // Send the input to a file.
    THREAD_INPUTMODEL_KGWS,      // Running on Setup environment and using kgws.
    THREAD_INPUTMODEL_LOADABLEWINDOWSERVER   // Using the current loadable window server.
}thread_inputmodel_t;


/*
 * thread_state_t:
 *    Enumeram os estados de uma threads, (8 estados).
 *    Existem 2 grupos, 'Earth' e 'Space'.
 *    Obs: Rodam no 'espa�o'.
 *
 * Earth: (INITIALIZED, STANDBY, ZOMBIE, DEAD).
 * Space: (READY, RUNNING, WAITING, BLOCKED).
 *
 *  INITIALIZED,    //Earth, Criado o contexto e par�metros.
 *  STANDBY,        //Earth, Pronta para rodar pela primeira vez. Ir para o 'espa�o'.
 *  ZOMBIE,         //Earth, Terminou a execu��o. Voltou para a 'terra'.
 *  DEAD,           //Earth, Deleted.
 * 
 *  READY,          //Space, Thread is ready to run again.
 *  RUNNING,        //Space, Thread is currently running.
 *  WAITING,        //Space, Thread is waiting.	
 *  BLOCKED,        //Space, Thread is blocked by an event.
 *
 * Obs: 
 *     Na pr�tica, a troca de status est� seguindo um organograma de 
 * movimentos. 
 *     @todo: Descrever esses movimentos na documenta��o. Incluir a 
 * visualiza��o atrav�s do gr�fico.
 */
 
typedef enum {
	INITIALIZED,    //0 Earth, Criado o contexto e par�metros.
	STANDBY,        //1 Earth, Pronta para rodar pela primeira vez. Ir para o 'espa�o'.
	ZOMBIE,         //2 Earth, Terminou a execu��o. Voltou para a 'terra'.
	DEAD,           //3 Earth, Deleted.
	READY,          //4 Space, Thread is ready to run again.
	RUNNING,        //5 Space, Thread is currently running.
	WAITING,        //6 Space, Thread is waiting.
	BLOCKED,        //7 Space, Thread is blocked by an event.
}thread_state_t;


/*
 ***********************************************************
 * thread_d: 
 *    TCB - Thread Control Block.
 *    In the end of the struct we find the elements 
 *    not frequently used.
 */

struct thread_d 
{

    // Object control.
    object_type_t   objectType;
    object_class_t  objectClass;
    struct object_d *object;

    int used;
    int magic;

    int position;

    // #todo
    // Other process can't take some actions on this thread
    // if it is protected. 
    // ex: It can't be killed by another process.
    int _protected;


//
// Identifiers
//
    // Thread id and owner process id.
    int tid;

    // Owner process.
    struct process_d  *process; 
    int ownerPID;

    // type: 
    // Tipo de tarefa.
    // (SYSTEM, PERIODIC, RR, IDLE).
    thread_type_t type;

    thread_inputmodel_t input_model;

    //flag, Estado atual da tarefa. ( RUNNING, DEAD ... ).
    thread_state_t state;    

    // #todo
    // Identifica uma thread como sendo uma thread que 
    // pertence � um servidor. Ent�o as threads desse tipo 
    // ganham mais prioridade e mais tempo de processamento.

    // int isServerThread;

	// flag. 
	// 1 = Sinaliza que a thread est� dando a prefer�ncia
	// e que deve sair quando for seguro fazer isso.
    int _yield;

	// error. @todo:
	//unsigned long error;
	
	//ext.

    //plano de execu��o.
    int plane;

//
// Names
//

    //char *name;  //@todo: Usar isso.
    unsigned long name_address;
    unsigned long name_pointer; 
    char short_name[4];
    char *cmd;
    
    // #test
    // estamos usando esse aqui.
    // Assim fica mais f�cil enviar para o aplicativo.
    char __threadname[64];    // HOSTNAME_BUFFER_SIZE
    size_t threadName_len;    // len 

//
// == CPU =========================================
//

    // What processor the thread is running.
    int cpu;
    
    // What will be the next processor for this thread.
    int next_cpu;
    
    // Processor affinity. (CPU pinning)
    // The thread will execute only on the designated CPU.
    // See: https://en.wikipedia.org/wiki/Processor_affinity
    int affinity;

	// ORDEM: 
	// O que segue � referenciado durante a interrup��o de timer.

	//...

	// ORDEM: 
	// O que segue � referenciado durante o processo de task switch.

//
// == Directory =======================================
//

    // COLOCAR O DIRET�RIO DE P�GINAS QUE A THREAD USA, 
    // ISSO AJUDA NA HORA DO TASKSWITCH.

    unsigned long DirectoryPA;

    // ring
    unsigned long iopl;
    unsigned long PreviousMode;

//
// == Context ======================= 
//

	// #todo: 
	// Usars uma estrutura.
	// #todo: 
	// Isso deve virar um ponteiro de estrutura.
    // ?? O que faremos, pois temos mais de uma arquitetura.
    // struct x86_context_d *context;

	//stack frame;
    unsigned short ss;
    unsigned long  esp;
    unsigned long  eflags;
    unsigned short cs;
    unsigned long  eip;    //usado com o pd do processo

	// para o kernel saltar para o novo processo.
    unsigned long ring0_eip;  //usado com o pd do kernel
    unsigned long eipPA;

    unsigned short ds;
    unsigned short es;
    unsigned short fs;
    unsigned short gs;

    unsigned long eax;
    unsigned long ebx;
    unsigned long ecx;
    unsigned long edx;
    unsigned long esi;
    unsigned long edi;
    unsigned long ebp;
	//continua o contexto ...

	//O endere�o incial, para controle.
    unsigned long initial_eip;

//
//  tss
//

	//#todo
	//isso � muito necess�rio.
    struct i386tss_d *tss;


	// ORDEM: 
	// O que segue � referenciado durante o processo de scheduler.

//
// == Priorities ===================
//

    // Priority levels.
    // Used by processes and threads.
    // Classes:
    // 1 ~ 5 = variable.
    // 6 ~ 9 = realtime.
    // variable:
    //     Can be changed on the fly.
    // realtime:
    //     Can't be changed on the fly.
    // # ps:
    // The base priority is never changed. It's used to classify
    // the priority level.
    // The priority can't be changed to a level below the base priority.
    // The base priority is static and the current priority is dynamic.

    unsigned long base_priority;  // static 
    unsigned long priority;       // dynamic

    // preempted:
    // flag ~ Sinaliza que uma tarefa pode ou n�o sofrer preemp��o.
    // Uma tarefa de menor prioridade pode deixar o estado running 
    // para assumir o estado ready em favor de uma tarefa de maior prioridade
    // que assumir� o estado running.
    // todo: isso pode ser int, bool ou char.

    unsigned long preempted;

    // #test: 
    // Escalonado duas vezes.
    int DoubleShot;


	// ORDEM: 
	// O que segue � referenciado durante o processo de dispatch.

    // Sinaliza que a tarefa teve o seu contexto salvo.
    // #todo: use 'int'
    unsigned long saved;

    //todo: Usar a estrutura. 
    //struct heap_d *heap;

    // Heap and Stack.

    unsigned long Heap;
    unsigned long HeapSize;
    unsigned long Stack;
    unsigned long StackSize;


	// Endere�o de um array contendo ponteiros para variso servi�os
	// que a thread pode usar.
	// unsigned long ServiceTable;

//
// == Time =============================
//

    // Podemos criar a estrutura 'thread_time_d' t->time.step

    // Quanto tempo passou, mesmo quando a tarefa n�o esteve rodando.
    // unsigned long jiffies_alive;


    // step: 
    // How many jiffies. total_jiffies.
    // Quantas vezes ela j� rodou no total.
    unsigned long step; 

    // Quando ela foi criada.
    // systime inicial da thread.
    unsigned long initial_time_ms;

    // Tempo total dado em milisegundos.
    unsigned long total_time_ms; 

    // Quantum. 
    // time-slice or quota. 
    // Quantos jiffies a thread pode rodar em um round.
    // Quantidade limite de jiffies que uma thread pode rodar em um round.
    // Limite quando dado boost. 
    unsigned long quantum;
    unsigned long quantum_limit;   

    // Quantos jiffies a thread ficou no estado e espera para
    // pronta para rodar.
    unsigned long standbyCount;

    // Quantos jiffies ela est� rodando antes de parar.
    unsigned long runningCount; 

    // Quantos milisegundos ela est� rodando antes de parar.
    unsigned long runningCount_ms; 

    // obs: 
    // ??
    // A soma das 3 esperas � a soma do tempo de espera
    // depois que ela rodou pela primeira vez.

    // Contando o tempo nos estados de espera.

    // Tempo de espera para retomar a execu��o.
    // Limite esperando para rodar novamente.
    // Talvez essa contagem nao precise agora. 
    unsigned long readyCount;
    unsigned long ready_limit;

    // Quantos jiffies esperando por algum evento.
    // Quantos jiffies a thread pode esperar no maximo.
    unsigned long waitingCount; 
    unsigned long waiting_limit;

    // Quantos jiffies ficou bloqueada.
    // Qauntos jiffies a thread pode esperar bloqueada.
    unsigned long blockedCount;
    unsigned long blocked_limit;

    // #todo: 
    // Deadline.
    // Quando tempo a tarefa tem para que ela complete a sua execu��o.

    // unsigned long DeadLine.
    // unsigned long RemainingTime; 

    // Ticks remaining. (Deadline)
    // Contagem do prazo limite.
    // Contagem regressiva.
    // Isso eh usado por threads 'real-time'
    unsigned long ticks_remaining;


//
// == Profiler ==================================
//

    //quanto por cento do tempo o processo ficou rodando.
    //� a soma do quanto ficou rodando todas as suas threads.
    unsigned long profiler_percentage_running;
    unsigned long profiler_percentage_running_res;
    unsigned long profiler_percentage_running_mod;
    unsigned long profiler_ticks_running;
    unsigned long profiler_last_ticks;

    //Tempo para o pr�ximo alarme, dado em ticks.
    //unsigned long alarm; 


	// ORDEM: 
	// O que segue � referenciado com pouca frequencia.
	
	//lista de arquivos ??
	//fluxo padr�o. stdio, stdout, stderr
	//unsigned long iob[8];
    
	//#bugbug: o vetor Stream[] conter� essas stream tamb�m.
	//ponteiros para as streams do fluxo padr�o.
	//O processo tem streams ... Stream[] ...
	//cada tread pode ter suas stream ... mesmo que herde streams 
	//de processo ...
	// ?? threads diferentes do mesmo processo podem atuar em streams 
	// diferentes ??
	//unsigned long standard_streams[3];
	//unsigned long Streams[8];

	//Obs: Cada processo est� atuando em um diret�rio,
	// mas ser� cada thread precisa atuar em um diret�rio diferente??
	//
	//struct _iobuf *root;  // 4 root directory
    //struct _iobuf *pwd;     // 5 (print working directory) 
	//...
		
	
	//@todo: Uma thread pode estar esperando varias outras por motivos diferenes.
	//struct wait_d WaitBlock;


//
// == tty ==========================
//

    // ID da tty usada.
    int tty_id;



//
// Security
//

    // #importante
    // Isso � usado para ger�ncia de mem�ria e ... 
    
    //struct usession_d *usession;  //#todo
    struct room_d     *room;        // (Window Station) da thread.
    struct desktop_d  *desktop;     // Desktop da thread.


    // ??

    // ?? procedimento de janela.
    unsigned long procedure; //Endere�o do procedimento de janela da tarefa. 
	//unsigned long control_menu_procedure; //procedimento do control menu.


	// #ORDEM: 
	// O que segue � referenciado durante as trocas de mensagens.
	// utiliza��o de canais e IPC.


//
// LIS - Local Input State
//
    
    // #important
    // Here we have some elements to handle the input events
    // in this thread.
    // + We have a block for single shot event.
    // + We have a block for a queue of events.
    // + We have a set of variables to handle the flow.
    // ...
    
    // Keyboard input and window focus information:
    // + Which window has keyboard focus?
    // + Which window is active?
    // + Which keys are considered pressed down?
    // + The state of the caret.
    
    // Mouse cursor management information
    // + Which window has mouse capture?
    // + The shape of the mouse cursor?
    // + The visibility of the mouse cursor.

    // ...
 
    // #todo
    // We don't need to have a window as an element in the event.
    // We can include this window later when the app calls get_message().
    // When the thread calls GetMessage(), 
    // the keyboard event is removed from the queue and 
    // assigned to the window that currently has input focus.
    

//
// == kernel event ====================================
//

    // #test
    // Sending a message to the kernel.
    // no queue.
    // Flag avisando que tem nova mensagem.
    // O kernel deve checar essa flag. Se ela estiver acionada,
    // significa que o kernel deve processar essa mensagem.

    //++
    struct window_d  *ke_window;
    int               ke_msg;
    unsigned long     ke_long1;
    unsigned long     ke_long2;
    int               ke_newmessageFlag; 
    //--


//
// == Event queue ===========================================
//

    // This is the event queue.
    // It is used by the ring 3 processes.
    // #todo: 
    // We can put all these into a structure.

    // Lists:
    // window_list: pointer to window structure.
    // msg_list:    event type.
    // long1_list:  data 1
    // long2_list:  data 2
    // long3_list:  data 3
    // long4_list:  data 4

    // MAXEVENTS
    // See: events.h
    
    //++
    struct window_d  *window_list[32];
    int                  msg_list[32];
    unsigned long      long1_list[32];
    unsigned long      long2_list[32];
    unsigned long      long3_list[32];
    unsigned long      long4_list[32];
    
    // offsets
    int tail_pos;
    int head_pos;
    //--

    // ====================================================


//
// Message Queue
//

    // Coloca-se em tail, quande chegar ao fim do buffer, recome�a.
    // Se o tail encontrar o head � porque o processo n�o est� 
    // respondendo.
    // #bugbug
    // isso ao esta sendo usado no momento.
    
    unsigned long MsgQueue[32];
    int MsgQueueHead;  //retira. 
    int MsgQueueTail;  //coloca.


    // Quando um processo s� pode receber mensagens de um 
    // determinado processo. Ou de qualquer um.
    // Ex: ANY=-1, PID ...
    // pid_t receive_from_pid;

	//?? mensagens pendentes.
	//struct thread_d *sendersList; //Lista encadeada de threads querendo enviar mensagem
	//struct thread_d *nextSender;  //pr�xima thread a enviar mensagem.
	


    // Each thread has its own mouse shape. ?
    // Each window has its own mouse shape. ?
    //int mouse_pointer_type;

    // Is this valid for a thread or for a window?
    // int mouse_is_captured;


//
// == Wait ======================================
//

	//#importante
	//raz�es para esperar
	//#todo: isso precisa ser inicializado.
	//@todo: tem que fazer um enum para enumerar as raz�es.
	//o �ndice � o selecionador da raz�o pela 
	//qual a thread est� esperando.
	//existem umas 20 raz�es pra esperar.
	// 0 - esperando por mensagem.(presa num loop)
	// 1 - esperando outra thread finalizar. wait4tid
	// 2 - esperando um processo finalizar. wait4pid
	// 3 - esperando um objeto. (espera gen�rica)
	// ...

    int wait_reason[10]; 

    int wait4pid;   //id do processo que a thread est� esperando morrer.
    int wait4tid;   //id da thread que a thread est� esperando morrer.

	// Objeto pelo qual a thread est� esperando.
	// #todo: mudar esses nomes, pode confundir com o header no in�cio da 
	// estrutura. (waiting_object_type ...  woType woClass )
    object_type_t   woType;   //obType;   //woType
    object_class_t  woClass;  //obClass;  //woClass

    // ====================================

//
// Signal support
//

    unsigned long signal;
    unsigned long umask;

    // Exit
    // Reason to close the thread.

    int exit_code;

    // Navigation

    struct thread_d  *prev;
    struct thread_d  *next;
};


// Ponteiro para a idle thread atual
// Sempre que mudar a idle thread devemos usar esse ponteiro
// para mostrar qual ser� a nova idle thread.
// Cada idle thread pode prestar um servi�o diferente, como o
// gerenciamento de energia.
extern struct thread_d *____IDLE;


// Essa � a criada para o processo kernel.
// Ela roda em ring0 e ser� usada como idle.
// RING0 IDLE Thread. TID=3

    // #bugbug
    // O problema � que se essa thread come�a afuncionar
    // antes mesmo do processo init habilitar as interrup��es,
    // ent�o o sistema vai falhar.
    // #todo
    // Como essa thread funciona sendo apenas uma rotina sti/hlt,
    // podemos deixar ela como idle thread somente nos est�gios 
    // iniciais, sendo substituida por outra quando fot poss�vel.

extern struct thread_d *EarlyRING0IDLEThread;    


// Essa � a thread de controle do processo init2.bin
// � o primeiro processo em ring3.
// Idle Thread. TID=0    
extern struct thread_d *InitThread;         

// Ponteiro para a thread usada na hora da clonagem de processos.
extern struct thread_d *ClonedThread;

/* 
 * Listas encadeadas de threads.
 * Usadas no gerenciamento de rounds 
 */

// The fixed conductor to mark the start.
extern struct thread_d *Conductor;
// The flexible conductor to create the list.
extern struct thread_d *tmpConductor;  //Conductor2;
// The created root conductor.
extern struct thread_d *rootConductor;

extern int conductorIndex;


// #todo
// Podemos planejar o uso de listas nesse formato.
// Mas estamos trabalhando em filas de tamanho fixo no formato de array.
//struct thread_d *blocked_list_head;
//struct thread_d *waiting_list_head;
//struct thread_d *ready_list_head;


//
// Thread list.
//

// #Aten��o
// Esse � a lista principal. Cont�m todas as threads.
 
// N�mero m�ximo de threads.
#define THREAD_COUNT_MAX 1024  

extern unsigned long threadList[THREAD_COUNT_MAX];


struct thread_list_d 
{
    struct thread_d *Threads;    //List.
    unsigned long priority;      //Prioridade.
    // ...
};

//struct thread_list_d *DispatcherList[10]; //list
//struct thread_list_d *DispathcerPriorityQueue[10];
//unsigned long DispatcherList[10];
//DispatcherList[2].Threads[4].tid


//
// ==  prototypes ============================================
//


// Early ring0 idle thread.
void early_ring0_IdleThread (void);
void *create_CreateEarlyRing0IdleThread (void);

// Control thread for ring 3 init process.
void *create_CreateRing3InitThread (void);


// Clona uma thread e retorna o ponteira da clone.
struct thread_d *threadCopyThread ( struct thread_d *thread );


/* 
 ******************************
 * create_thread: 
 *    Create thread.
 */ 

struct thread_d *create_thread ( 
    struct room_d *room,
    struct desktop_d *desktop,
    struct window_d *window,
    unsigned long init_eip, 
    unsigned long init_stack, 
    int pid, 
    char *name );


void *GetCurrentThread (void);

void *FindReadyThread (void);

int GetThreadState (struct thread_d *Thread);
int GetThreadType (struct thread_d *Thread);
void dispatch_thread (struct thread_d *thread);
void set_thread_priority (struct thread_d *t, unsigned long priority);


// muda a prioridade para alem dos limites.
// teste.
void threadi_power(
    struct thread_d *t, 
    unsigned long priority );


// ## Page directory support ## 

// Pega o endere�o do diret�rio de p�ginas usado pela thread.
unsigned long GetThreadDirectory( struct thread_d *thread );

// Altera o endere�o do diret�rio de p�ginas de uma thread.
void SetThreadDirectory ( struct thread_d *thread, unsigned long Address );

/* Thread heap support. */
unsigned long GetThreadHeapStart (struct thread_d *thread);
//...

/*
 * Thread Stack support.
 */
unsigned long GetThreadStackStart(struct thread_d *thread);
//...

void SelectForExecution (struct thread_d *Thread);


// Show info about all threads.
void show_slots (void);

// Show info about a thread.
void show_slot(int tid);

// Show the register of the context.
void show_reg (int tid);

void show_thread_information (void); 

int init_threads (void);

int GetCurrentThreadId (void);

void spawn_thread (int tid);


// ## Finaliza��es ##


// Liberar uma thread que estava bloqueada ou esperando.
void release ( int tid );

// Torna zumbi uma thread.
void exit_thread ( int tid );       

// exit current thread.
void exit_current_thread(void);

//Destr�i uma thread.
void kill_thread (int tid);       

void dead_thread_collector (void);

void kill_all_threads (void);

int thread_getchar (void);

// se a flag estiver habilitada, ent�o devemos acorar a
// thread do dead thread collector.
void check_for_dead_thread_collector (void);

int thread_profiler( int service );

// pegar a porcentagem de vezes que a thread rodou durante um determinado
//per�odo.
unsigned long 
thread_get_profiler_percentage ( struct thread_d *thread );

void thread_show_profiler_info (void);


int getthreadname ( int tid, char *buffer );


unsigned long __GetThreadStats ( int tid, int index );


#endif    


//
// End.
//


