/*
 * File: timer.h
 *
 * Descrição:
 *     Header para o módulo timer. irq0.
 *     @todo: strucs ... definições de frequências ...
 *
 * 2015 - Created by Fred Nora.
 */


#ifndef ____PIT_H
#define ____PIT_H


//
// Count support variables.
//


#define HZ  100


// total ticks
extern unsigned long jiffies;
// por quantos segundos o sistema esta rodando
// jiffies/sys_time_hz
extern unsigned long seconds;
// Por quantos ms o sistema esta rodando.
extern unsigned long sys_time_ms;
// pit frequency
extern unsigned long sys_time_hz;


//
// Working set support.
// 

// Isso será usado pelo profiler para
// medir a quantidade de páginas que possuem frames em um determinado
// período de tempo.
// Também será usado pelo profiler para saber quantas vezes 
// Uma thread rodou num determinado período de tempo.
// Etc.
// Será o tempo de referência para o profiler.


#define PROFILER_TICKS_DEFAULT_LIMIT  200


//contador do profiler ticks.
//só vai de zero até o limite configurável.
extern unsigned long profiler_ticks_count;
//limite da contagem do profile ticks.
//quando chegar aqui, então devemos calcular 
//a porcentagem para todas as threads e processos.
extern unsigned long profiler_ticks_limit;
extern unsigned long profiler_percentage_all_normal_threads;
extern unsigned long profiler_percentage_idle_thread;

//Usado no Linux. 
//#define CT_TO_SECS(x)	((x) / HZ)
//#define CT_TO_USECS(x)	(((x) % HZ) * 1000000/HZ) 
//... 

// Em que tempo estávamos quando iniciamos a contagem;
//#define TIMER_PROFILE_TICKS_DEADLINE 1000
//unsigned long timer_profile_ticks;

//Counting how much ticks the kernel is running.
//unsigned long kernel_tick;
 
// #extra
// Aciona uma flag para que o ts.c realize atividades extras,
// como gc, dead thread collector, request.

extern int extra; 

// PIT 
#define TIMER_DRIVER_PATHNAME  "root:/volume2/drivers/timer.bin"  
 
 
/*
    040-05F = 8253 or 8254 Programmable Interval Timer (PIT, see ~8253~)
	040     = 8253 channel 0, counter divisor
	041     = 8253 channel 1, RAM refresh counter
	042     = 8253 channel 2, Cassette and speaker functions
	043     = 8253 mode control  (see 8253)
	044     = 8254 PS/2 extended timer
	047     = 8254 Channel 3 control byte
*/	
 
/*
 * Como o linux define um Quantum (time slice) para tasks RR.
 */
/*
 * default timeslice is 100 msecs (used only for SCHED_RR tasks).
 * Timeslices get refilled after they expire.
 */
//#define RR_TIMESLICE            (100 * HZ / 1000)

//a flag indica se o cursor está habilitado ou não.
extern int timer_cursor_used;
//status do cursor.
extern int timer_cursor_status;
extern unsigned long time_out;
extern unsigned long timer_handler_address;    //global _irq0:


/*
 * timer_d:
 * Estrutura do objeto timer que será usado pelos aplicativos.
 * Precisamos identificar quem está usando para podermos enviar 
 * mensagem para quem possui o timer.
 */
struct timer_d 
{
    object_type_t objectType;
    object_class_t objectClass;
    int used;
    int magic;

    int id;

	//1 = one shot 
	//2 = intermitent
    int type;

    // Owner
    struct process_d  *process;
    struct thread_d   *thread;

    int pid;
    int tid;

    // ??
    struct window_d   *window;

    // Counter
    int count_down;          // Decrementing
    int initial_count_down;  // base fixa

    // Quantas vezes o timer se esgotou.
    unsigned long times;

    // ??
    int status;
    unsigned long flag;
    unsigned long error;
//Navigation
    struct timer_d  *next;
};
// struct timer_d *XXXXXXTimer;

// lista de timers.
extern unsigned long timerList[32];

//
// == prototypes ===========================
//


//Global para inicialização do módulo interno.

int early_timer_init (void);
int timerInit (void);
int timerTimer (void);
void timerInit8253 ( unsigned long hz ); 

//...

//Global para acesso ao tempo imediatamente agora.
unsigned long now (void);

unsigned long get_timeout (void);
void set_timeout(unsigned long ticks);


void set_systime_hz ( unsigned long hz );
unsigned long get_systime_hz (void);

unsigned long get_systime_ms (void);
unsigned long get_systime_totalticks (void);

void timerEnableTextCursor (void);
void timerDisableTextCursor (void);

unsigned long get_systime_info (int n);


// IN: pid, ms, type
struct timer_d *create_timer ( 
    pid_t pid, 
    unsigned long ms, 
    int type  );

//#todo destroy timer.

int new_timer_id (void);

void DeviceInterface_PIT(void); 

#endif   

//
// End
//

