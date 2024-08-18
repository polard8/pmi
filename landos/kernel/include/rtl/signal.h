// signal.h
// Created by Fred Nora.

/*
 *     C library signal support.
 *     Obs: @todo: O suporte à outros tipos de sinais além do padrão C devem
 * ir para outro arquivo.
 *
 *     Header para o Signal Manager.
 *     Um sinal é uma forma limitada de comunicação entre processos e entre 
 * threads.
 *     A característica mais importante de um sinal é que ele não envolve a
 * transferência de mensagem na forma de dados, apenas o sinal propriamente
 * dito.
 *     Um sinal pode interromper a ação de um processo ou thread.
 *     A execução pode ser interrompida em qualquer operação que não seja atômica. 
 *     Um processo pode registrar uma rotina de tratamento de sinal 
 * (signal handler em inglês) para tratar um determinado sinal, se não for 
 * desejado que a rotina padrão para este sinal seja executada.
 * Rotinas de tratamento de sinais podem ser instaladas através de 
 * chamada de sistema.
 * Se uma rotina de tratamento não é instalada para um determinado sinal, 
 * a rotina padrão é utilizada. Obs: Isso está me parecendo interrupções
 * de software.
 *
 * Obs: signal.h é um arquivo cabeçalho da biblioteca padrão da linguagem de programação C??
 * Obs:  PODEM SER IMPLMENTADOS POUCOS SINAIS, SEPARADOS EM CATEGORIAS.
 *
 * Histórico:
 *      Versão: 1.0, 2016 - Created.
 *      ...
 */

#ifndef __RTL_SIGNAL_H
#define __RTL_SIGNAL_H    1

/*
The C standard defines only 6 signals. 
They are all defined in signal.h header (csignal header in C++):[1]

SIGABRT - "abort", abnormal termination.
SIGFPE  - floating point exception.
SIGILL  - "illegal", invalid instruction.
SIGINT  - "interrupt", interactive attention request sent to the program.
SIGSEGV - "segmentation violation, invalid memory access.
SIGTERM - "terminate", termination request sent to the program. 
 
 */
 
/*

Constante	Significado	Disponibilidade
SIGHUP	Desconexão	POSIX	
SIGINT	Interrupção	ANSI
SIGQUIT	Sair	POSIX
SIGILL	Instrução ilegal	ANSI
SIGABRT	Abortar	ANSI
SIGTRAP	Armadilha de rastreio	POSIX
SIGIOT	Armadilha para IOT	4.2 BSD
SIGEMT	Armadilha para EMT	4.2 BSD
SIGFPE	Exceção de ponto flutuante	ANSI
SIGKILL	Matar processo	POSIX
SIGBUS	Erro no BUS	4.2 BSD
SIGSEGV	Falha de segmentação	ANSI
SIGSYS	Argumento incorreto para syscall	4.2 BSD
SIGPIPE	Pipe quebrado	POSIX
SIGALRM	Alarme	POSIX
SIGTERM	Término	ANSI
SIGUSR1	Sinal definido pelo usuário 1	POSIX
SIGUSR2	Sinal definido pelo usuário 2	POSIX
SIGCHLD	Mudança de estado no processo filho	POSIX
SIGCLD	Mesmo que SIGCHLD	System V
SIGPWR	Reinício por falha de energia	System V
SIGXCPU	Excedeu o tempo de CPU	POSIX
SIGSTOP	Pausar execução	POSIX
SIGCONT	Continuar execução	POSIX

 */ 
 
/*
POSIX signals[edit]
The list below documents the signals specified in the Single Unix Specification. All signals are defined as macro constants in <signal.h> header file. The name of the macro constant consists of a "SIG" prefix followed by a mnemonic name for the signal.

SIGABRT
The SIGABRT signal is sent to a process to tell it to abort, i.e. to terminate. The signal is usually initiated by the process itself when it calls abort function of the C Standard Library, but it can be sent to the process from outside like any other signal.
SIGALRM, SIGVTALRM and SIGPROF
The SIGALRM, SIGVTALRM and SIGPROF signal is sent to a process when the time limit specified in a call to a preceding alarm setting function (such as setitimer) elapses. SIGALRM is sent when real or clock time elapses. SIGVTALRM is sent when CPU time used by the process elapses. SIGPROF is sent when CPU time used by the process and by the system on behalf of the process elapses.
SIGBUS
The SIGBUS signal is sent to a process when it causes a bus error. The conditions that lead to the signal being sent are, for example, incorrect memory access alignment or non-existent physical address.
SIGCHLD
The SIGCHLD signal is sent to a process when a child process terminates, is interrupted, or resumes after being interrupted. One common usage of the signal is to instruct the operating system to clean up the resources used by a child process after its termination without an explicit call to the wait system call.
SIGCONT
The SIGCONT signal instructs the operating system to continue (restart) a process previously paused by the SIGSTOP or SIGTSTP signal. One important use of this signal is in job control in the Unix shell.
SIGFPE
The SIGFPE signal is sent to a process when it executes an erroneous arithmetic operation, such as division by zero (the name "FPE", standing for floating-point exception, is a misnomer as the signal covers integer-arithmetic errors as well).[2]
SIGHUP
The SIGHUP signal is sent to a process when its controlling terminal is closed. It was originally designed to notify the process of a serial line drop (a hangup). In modern systems, this signal usually means that the controlling pseudo or virtual terminal has been closed.[3] Many daemons will reload their configuration files and reopen their logfiles instead of exiting when receiving this signal.[4] nohup is a command to make a command ignore the signal.
SIGILL
The SIGILL signal is sent to a process when it attempts to execute an illegal, malformed, unknown, or privileged instruction.
SIGINT
The SIGINT signal is sent to a process by its controlling terminal when a user wishes to interrupt the process. This is typically initiated by pressing Ctrl-C, but on some systems, the "delete" character or "break" key can be used.[5]
SIGKILL
The SIGKILL signal is sent to a process to cause it to terminate immediately (kill). In contrast to SIGTERM and SIGINT, this signal cannot be caught or ignored, and the receiving process cannot perform any clean-up upon receiving this signal.
SIGPIPE
The SIGPIPE signal is sent to a process when it attempts to write to a pipe without a process connected to the other end.
SIGPOLL
The SIGPOLL signal is sent when an event occurred on an explicitly watched file descriptor.[6] Using it effectively leads to making asynchronous I/O requests since the kernel will poll the descriptor in place of the caller. It provides an alternative to active polling.
SIGRTMIN to SIGRTMAX
The SIGRTMIN to SIGRTMAX signals are intended to be used for user-defined purposes. They are real-time signals.
SIGQUIT
The SIGQUIT signal is sent to a process by its controlling terminal when the user requests that the process quit and perform a core dump.
SIGSEGV
The SIGSEGV signal is sent to a process when it makes an invalid virtual memory reference, or segmentation fault, i.e. when it performs a segmentation violation.[7]
SIGSTOP
The SIGSTOP signal instructs the operating system to stop a process for later resumption.
SIGSYS
The SIGSYS signal is sent to a process when it passes a bad argument to a system call. In practice, this kind of signal is rarely encountered since applications rely on libraries (e.g. libc) to make the call for them.
SIGTERM
The SIGTERM signal is sent to a process to request its termination. Unlike the SIGKILL signal, it can be caught and interpreted or ignored by the process. This allows the process to perform nice termination releasing resources and saving state if appropriate. SIGINT is nearly identical to SIGTERM.
SIGTSTP
The SIGTSTP signal is sent to a process by its controlling terminal to request it to stop (terminal stop). It is commonly initiated by the user pressing Ctrl+Z. Unlike SIGSTOP, the process can register a signal handler for or ignore the signal.
SIGTTIN and SIGTTOU
The SIGTTIN and SIGTTOU signals are sent to a process when it attempts to read in or write out respectively from the tty while in the background. Typically, these signals are received only by processes under job control; daemons do not have controlling terminals and, therefore, should never receive these signals.
SIGTRAP
The SIGTRAP signal is sent to a process when an exception (or trap) occurs: a condition that a debugger has requested to be informed of — for example, when a particular function is executed, or when a particular variable changes value.
SIGURG
The SIGURG signal is sent to a process when a socket has urgent or out-of-band data available to read.
SIGUSR1 and SIGUSR2
The SIGUSR1 and SIGUSR2 signals are sent to a process to indicate user-defined conditions.
SIGXCPU
The SIGXCPU signal is sent to a process when it has used up the CPU for a duration that exceeds a certain predetermined user-settable value.[8] The arrival of a SIGXCPU signal provides the receiving process a chance to quickly save any intermediate results and to exit gracefully, before it is terminated by the operating system using the SIGKILL signal.
SIGXFSZ
The SIGXFSZ signal is sent to a process when it grows a file larger than the maximum allowed size.
*/ 


/*
Macro constants
type	macro	signal
int (signals)	SIGABRT	(Signal Abort) Abnormal termination, such as is initiated by the abort function.
SIGFPE	(Signal Floating-Point Exception) Erroneous arithmetic operation, such as zero divide or an operation resulting in overflow (not necessarily with a floating-point operation).
SIGILL	(Signal Illegal Instruction) Invalid function image, such as an illegal instruction. This is generally due to a corruption in the code or to an attempt to execute data.
SIGINT	(Signal Interrupt) Interactive attention signal. Generally generated by the application user.
SIGSEGV	(Signal Segmentation Violation) Invalid access to storage: When a program tries to read or write outside the memory it has allocated.
SIGTERM	(Signal Terminate) Termination request sent to program.
functions (handlers)	SIG_DFL	Default handling: The signal is handled by the default action for that particular signal.
SIG_IGN	Ignore Signal: The signal is ignored.
SIG_ERR	Special return value indicating failure.
*/


/*
#define SIGTERM //implementation defined
#define SIGSEGV //implementation defined
#define SIGINT //implementation defined
#define SIGILL //implementation defined
#define SIGABRT //implementation defined
#define SIGFPE //implementation defined
SIGTERM	termination request, sent to the program
SIGSEGV	invalid memory access (segmentation fault)
SIGINT	external interrupt, usually initiated by the user
SIGILL	invalid program image, such as invalid instruction
SIGABRT	abnormal termination condition, as is e.g. initiated by abort()
SIGFPE	erroneous arithmetic operation such as divide by zero
References
C11 standard (ISO/IEC 9899:2011):
7.14/3 Signal handling <signal.h> (p: 265)
C99 standard (ISO/IEC 9899:1999):
7.14/3 Signal handling <signal.h> (p: 246)
C89/C90 standard (ISO/IEC 9899:1990):
4.7 SIGNAL HANDLING <signal.h>
*/


/*
SIGABRT Encerramento anormal
SIGFPE Erro de ponto flutuante
SIGILL Instrução ilegal
SIGINT Sinal de CTRL+C
SIGSEGV Acesso ilegal ao armazenamento
SIGTERM Solicitação de finalização
*/


/*
Comentários (microsoft)
O argumento de sig deve ser uma das constantes manifestas listadas abaixo (definido em SIGNAL.H).
SIGABRT
Encerramento anormal. A ação padrão encerra o programa de chamada com o código de saída 3.
SIGABRT_COMPAT
Mesmo que SIGABRT. Para compatibilidade com outras plataformas.
SIGFPE
Erro de ponto flutuante, como o estouro, de divisão por zero, ou a operação inválida. A ação padrão encerra o programa de chamada.
SIGILL
Instrução ilegal. A ação padrão encerra o programa de chamada.
SIGINT
CTRL+C de interrupção. A ação padrão encerra o programa de chamada com o código de saída 3.
SIGSEGV
Acesso ilegal de armazenamento. A ação padrão encerra o programa de chamada.
SIGTERM
Solicitação de término enviada ao programa. A ação padrão encerra o programa de chamada com o código de saída 3.
SIG_ERR
Um tipo de retorno de um sinal que indica um erro ocorreu
*/

//
// Definições. padrão C.
//


//
// @todo: Criar estrutura e objeto do tipo signal
//


//tipo.
//sig_atomic_t


//
// Prototypes. padrão C.
//

//Set function to handle signal
//sig Valor do sinal.
//func Função a ser executada. O primeiro parâmetro é um valor de sinal e o 
//segundo parâmetro é um subcódigo que pode ser usado quando o primeiro parâmetro é SIGFPE.
//Valor Retornado
//signal retorna o valor anterior func que está associado ao sinal especificado. 
//Por exemplo, se o valor anterior de func era SIG_IGN, o valor de retorno também é SIG_IGN. 
//Um valor de retorno de SIG_ERR indica um erro; nesse caso, errno é definido como EINVAL.


//void (*signal(int sig, void (*func)(int)))(int);


//Generates a signal
//int raise(int sig);



//=====================================================
// Possíveis categorias de sinais para esse sistema:
// Process/Thread
// Nesse caso essas 3 estruturas teriam a o elemento 'signal;'
//
//	unsigned long signal;
//	unsigned long signalMask;
//
// que receberiam sinais de todos os tipos.
// Nesse caso não estamos falando de enfileiramento de sinais.
// Obs: Os sinais do sistema gramado começariam em 1000 ... 1001 1002 ... 
// Os outros sinais seriam os sinais padrão da linguagem C, posix e outros.
//======================================================

//
//sinais para o sistema gramado.
//

//Sinais para a categoria Process.
// GSIGP0 1000
// GSIGP1 1001
// GSIGP2 1002
// GSIGP3 1003
// GSIGP4 1004
//...

//Sinais para a categoria Thread.
// GSIGT0 2000
// GSIGT1 2001
// GSIGT2 2002
// GSIGT3 2003
// GSIGT4 2004
//...


//Criando primeiro sinal para teste.
#define SIGNULL  0

/*
#define SIGHUP  1       // hangup 
#define SIGINT  2       // interrupt 
#define SIGQUIT 3       // quit 
#define SIGILL  4       // illegal instruction (not reset when caught) 
#define SIGTRAP 5       // trace trap (not reset when caught) 
#define SIGABRT 6       // abort() 
#define SIGPOLL 7       // pollable event ([XSR] generated, not supported) 
#define SIGFPE  8       // floating point exception 
#define SIGKILL 9       // kill (cannot be caught or ignored) 
//(...)
*/


//credits: serenity os
#define SIGINVAL   0
#define SIGHUP     1
#define SIGINT     2
#define SIGQUIT    3
#define SIGILL     4
#define SIGTRAP    5
#define SIGABRT    6
#define SIGBUS     7
#define SIGFPE     8
#define SIGKILL    9
#define SIGUSR1    10
#define SIGSEGV    11
#define SIGUSR2    12
#define SIGPIPE    13
#define SIGALRM    14
#define SIGTERM    15
#define SIGSTKFLT  16
#define SIGCHLD    17
#define SIGCONT    18
#define SIGSTOP    19
#define SIGTSTP    20
#define SIGTTIN    21
#define SIGTTOU    22
#define SIGURG     23
#define SIGXCPU    24
#define SIGXFSZ    25
#define SIGVTALRM  26
#define SIGPROF    27
#define SIGWINCH   28
#define SIGIO      29
#define SIGPWR     30
#define SIGSYS     31
#define NSIG       32


//
//==================================
//
//int	(*signal())();
//#define	SIG_DFL	(int (*)())0
//#define	SIG_IGN	(int (*)())1

//#bugbug
#define SIG_DFL  ((__sighandler_t)0)
#define SIG_ERR  ((__sighandler_t)-1)
#define SIG_IGN  ((__sighandler_t)1)


//se precisar!!
/*
typedef void (*__sighandler_t)(int);
typedef __sighandler_t sighandler_t;
typedef uint32_t sigset_t;
typedef void siginfo_t;
typedef uint32_t sig_atomic_t;
*/



//#test
//coloquei aqui pra compilar o lua.
typedef int  sig_atomic_t;

/* Cygwin defines it's own sigset_t in include/cygwin/signal.h */
//#ifndef __CYGWIN__
//typedef unsigned long sigset_t;
//#endif

typedef unsigned int  sigset_t;

/* Type of a signal handler.  */
typedef void (*sighandler_t)(int);

typedef sighandler_t  __sighandler_t;
typedef sighandler_t  sig_t;
//...

//
//==============================
//

//============
/* Type for data associated with a signal.  */
/* Data passed with notification */
typedef union sigval {          
     int     sival_int;         /* Integer value */
     void   *sival_ptr;         /* Pointer value */
}sigval_t;

typedef sigval_t  __sigval_t;
//...

//
//==============================
//

struct sigevent 
{
    int          sigev_notify; /* Notification method */
    int          sigev_signo;  /* Notification signal */

    union sigval sigev_value;  /* Data passed with
                                         notification */
    void       (*sigev_notify_function) (union sigval);
                            /* Function used for thread
                               notification (SIGEV_THREAD) */
    void        *sigev_notify_attributes;
                            /* Attributes for notification thread
                               (SIGEV_THREAD) */
    pid_t        sigev_notify_thread_id;
                           /* ID of thread to signal (SIGEV_THREAD_ID) */
};


//
//==============================
//


typedef struct siginfo {
    int      si_signo;     /* Signal number */
    int      si_errno;     /* An errno value */
    int      si_code;      /* Signal code */
    int      si_trapno;    /* Trap number that caused hardware-generated signal (unused on most architectures) */
    pid_t    si_pid;       /* Sending process ID */
    uid_t    si_uid;       /* Real user ID of sending process */
    int      si_status;    /* Exit value or signal */
    clock_t  si_utime;     /* User time consumed */
    clock_t  si_stime;     /* System time consumed */
    sigval_t si_value;     /* Signal value */
    int      si_int;       /* POSIX.1b signal */
    void    *si_ptr;       /* POSIX.1b signal */
    int      si_overrun;   /* Timer overrun count;
                                         POSIX.1b timers */
    int      si_timerid;   /* Timer ID; POSIX.1b timers */
    void    *si_addr;      /* Memory location which caused fault */
    long     si_band;      /* Band event (was int in
                                         glibc 2.3.2 and earlier) */
    int      si_fd;        /* File descriptor */
    short    si_addr_lsb;  /* Least significant bit of address
                                         (since Linux 2.6.32) */
    void    *si_lower;     /* Lower bound when address violation
                                         occurred (since Linux 3.19) */
    void    *si_upper;     /* Upper bound when address violation
                                         occurred (since Linux 3.19) */
    int      si_pkey;      /* Protection key on PTE that caused
                                         fault (since Linux 4.6) */
    void    *si_call_addr; /* Address of system call instruction
                                         (since Linux 3.5) */
    int      si_syscall;   /* Number of attempted system call
                                         (since Linux 3.5) */
    unsigned int si_arch;  /* Architecture of attempted system call
                                         (since Linux 3.5) */
}siginfo_t ;


//
//==============================
//

typedef struct sigaction {

    void  (*sa_handler)(int);
    void  (*sa_sigaction)(int, siginfo_t *, void *);
    sigset_t  sa_mask;
    int  sa_flags;
    void  (*sa_restorer)(void);

}sigaction_t;

//
//==============================
//


#ifndef _POSIX_SOURCE
#define SA_ONSTACK  0x0001  /* take signal on signal stack */
#define SA_RESTART  0x0002  /* do not restart system on signal return */
#endif
#define SA_NOCLDSTOP  0x0004	/* do not generate SIGCHLD on child stop */

//se precisar!
/*
#define SA_NOCLDSTOP 1
#define SA_NOCLDWAIT 2
#define SA_SIGINFO 4
#define SA_ONSTACK 0x08000000
#define SA_RESTART 0x10000000
#define SA_NODEFER 0x40000000
#define SA_RESETHAND 0x80000000
#define SA_NOMASK SA_NODEFER
#define SA_ONESHOT SA_RESETHAND
*/


/*
 * Flags for sigprocmask:
 */
#define	SIG_BLOCK    1  /* block specified signal set */
#define	SIG_UNBLOCK  2  /* unblock specified signal set */
#define	SIG_SETMASK  3  /* set specified signal set */


/*
#define SIG_BLOCK 0
#define SIG_UNBLOCK 1
#define SIG_SETMASK 2
*/


//
//==============================
//


#ifndef _POSIX_SOURCE

//#todo
/*
 * 4.3 compatibility:
 * Signal vector "template" used in sigvec call.
 */
/*
struct sigvec {
    void  (*sv_handler)();  //signal handler 
    int     sv_mask;        //signal mask to apply 
    int     sv_flags;       //see signal options below 
};
*/

#define SV_ONSTACK	SA_ONSTACK
#define SV_INTERRUPT	SA_RESTART	/* same bit, opposite sense */
#define sv_onstack sv_flags	/* isn't compatibility wonderful! */

/*
 * Structure used in sigaltstack call.
 */
struct	sigaltstack {
	char  *ss_base;     /* signal stack base */
	int    ss_len;      /* signal stack length */
	int    ss_onstack;  /* current status */
};

/*
 * Structure used in sigstack call.
 */
struct	sigstack {
	char  *ss_sp;       /* signal stack pointer */
	int    ss_onstack;  /* current status */
};


/*
 * Information pushed on stack when a signal is delivered.
 * This is used by the kernel to restore state following
 * execution of the signal handler.  It is also made available
 * to the handler to allow it to restore state properly if
 * a non-standard exit is performed.
 */
struct	sigcontext {
	int	sc_onstack;  /* sigstack state to restore */
	int	sc_mask;     /* signal mask to restore */
	int	sc_sp;       /* sp to restore */
	int	sc_fp;       /* fp to restore */
	int	sc_ap;       /* ap to restore */
	int	sc_pc;       /* pc to restore */
	int	sc_ps;       /* psl to restore */
};


/*
 * Macro for converting signal number to a mask suitable for
 * sigblock().
 */
#define sigmask(m)	(1 << ((m)-1))

#define	BADSIG		(void (*)())-1
#endif	/* _POSIX_SOURCE */

#endif   

//
// End
//



