
// gramado system calls.

// #bugbug: It is not properly stub routines,
// used by shared libraries. It's is o.s. dependent routines.
// We need to change the name of this document, and change
// the name in ALL apllications.
//


#ifndef __GRAMADO_CALL
#define __GRAMADO_CALL 1



#include <types.h>
#include <sys/types.h>
#include <stddef.h>

#include <pthread.h>


// Modes.
#define GRAMADO_JAIL        0x00
#define GRAMADO_P1          0x01
#define GRAMADO_HOME        0x02
#define GRAMADO_P2          0x03
#define GRAMADO_CASTLE      0x04
#define GRAMADO_CALIFORNIA  0x05
// ...



// usado em 'file sync'
#define ACTION_NULL       0
#define ACTION_REQUEST    1000
#define ACTION_REPLY      2000
#define ACTION_EVENT      3000
#define ACTION_ERROR      4000

#define SYNC_REQUEST_SET_ACTION 1
#define SYNC_REQUEST_GET_ACTION 2
// ...


// #define RTL_ABS(X)    (((X) < 0) ? (-(X)) : (X))


//
// == prototypes =============================================
//


// =====================================================

// system call
void *gramado_system_call ( 
    unsigned long a,  //Service number. 
    unsigned long b, 
    unsigned long c, 
    unsigned long d );


void *sc80 ( 
    unsigned long a,  //Service number. 
    unsigned long b, 
    unsigned long c, 
    unsigned long d );

void *sc81 ( 
    unsigned long a,  //Service number. 
    unsigned long b, 
    unsigned long c, 
    unsigned long d );

void *sc82 ( 
    unsigned long a,  //Service number. 
    unsigned long b, 
    unsigned long c, 
    unsigned long d );

// =====================================================


// input mode
int rtl_get_input_mode(void);
void rtl_set_input_mode(int mode);



// ========================
void rtl_set_file_sync(int fd, int request, int data);
int rtl_get_file_sync(int fd, int request);



//==========================================

unsigned char  rtl_to_uchar  (char ch);
unsigned short rtl_to_ushort (short ch);
unsigned long  rtl_to_ulong  (long ch);

//==========================================


//
// == events ===========================================
//

// System messages in the thread's event queue.

#define RTL_WAIT_FOR_EVENTS  1
#define RTL_POOL_FOR_EVENTS  2


// rtl.c
//  The buffer for the event elements.
extern unsigned long RTLEventBuffer[32];


// Get an event from the thread's event queue.
// That old 'get system message'
// Using a buffer
int xxxScanApplicationQueue(void);
int rtl_get_event (void);

// ===========================================================


// ===========================================================
//  rtl event

// #todo
// Not tested yet.

struct rtl_event_d
{
    // #bugbug
    // The system call do not know the limit of arguments
    // in this structure.
    // maybe we need a pad for a 32 longs array,
    
    void *window;  // opaque
    int msg;
    unsigned long long1;
    unsigned long long2;
    
    unsigned long long3;
    unsigned long long4;
    unsigned long long5;
    unsigned long long6;

    // 32-8
    //unsigned long pad[24];
};

struct rtl_event_d *rtl_next_event (void);

// ===========================================================


// critical section
void rtl_enter_critical_section (void);
void rtl_exit_critical_section (void);

//
// == thread ===============================
//

void *rtl_create_thread ( 
    unsigned long init_eip, 
    unsigned long init_stack, 
    char *name );

void rtl_start_thread (void *thread);


//vamos escrever em uma janela indefinida. NULL.
//provavelmente a janela principal.
int 
rtl_draw_text ( 
    unsigned long x, 
    unsigned long y, 
    unsigned long color, 
    char *string );
    
void rtl_show_backbuffer (void);



unsigned long rtl_get_system_metrics (int index);


pid_t rtl_current_process(void);
int rtl_current_thread(void);

pthread_t pthread_self(void);


// ms
// tempo total em ms.
// usado para calcular o tempo de execuçao de uma funcao.
unsigned long rtl_get_progress_time(void);


// #bugbug
// Not tested yet.
int 
rtl_copy_text ( 
    unsigned long src, 
    unsigned long dest, 
    int width, 
    int height );

void rtl_reboot(void);

int rtl_sleep_if_socket_is_empty(int fd);


void rtl_test_pipe (void);


size_t rtl_path_count (unsigned char *path);

int 
rtl_load_path ( 
    char *path, 
    unsigned long buffer, 
    unsigned long buffer_len );
    

ssize_t rtl_console_beep(void);


int rtl_clone_and_execute ( char *name );

// get current thread
// set foreground thread.
int rtl_focus_on_this_thread(void);

void rtl_yield(void);

#endif


