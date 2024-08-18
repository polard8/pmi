// sys.h 
// System call interface.
// All the functions here can be called
// by and ring3 app via system call.
// Created by Fred Nora.

// #todo
// Organizar por ordem alfabética ou por grupo.

#ifndef ____SYS_H
#define ____SYS_H  1

// Helper function to initialize system's component
// after kernel initialization.
int sys_initialize_component( int n );

unsigned long sys_get_system_metrics ( int n );


int sys_ioctl ( int fd, unsigned long request, unsigned long arg );
int sys_fcntl ( int fd, int cmd, unsigned long arg);


//SVr4,  4.3BSD,  POSIX.1-2001. 
int sys_open ( const char *pathname, int flags, mode_t mode );
int sys_close ( int fd );

int sys_sleep_if_socket_is_empty ( int fd );

//
// read and write
//

// helper functions for 18 and 19.
int file_read_buffer ( file *f, char *buffer, int len );
int file_write_buffer ( file *f, char *string, int len );

// 18 and 19
int sys_read (unsigned int fd,char *ubuf,int count);
int sys_write (unsigned int fd,char *ubuf,int count);


//
// debug.
//

// 289
// ??
int sys_serial_debug_printk ( char *s );




//
// PROCESSES
//


// 73
void *sys_create_process ( 
    struct room_d     *room,
    struct desktop_d  *desktop,
    struct window_d   *window,   //#bugbug: Valid only for kgws.
    unsigned long res1, 
    unsigned long priority, 
    int ppid, 
    char *name, 
    unsigned long iopl );


int sys_getpid (void);
int sys_getppid (void);

int sys_fork (void);

void sys_exit_process (int pid, int code);




//
// THREADS
//


// 72
void *sys_create_thread ( 
    struct room_d     *room,
    struct desktop_d  *desktop,
    struct window_d   *window,    //#bugbug: Valid only for kgws.
    unsigned long init_eip, 
    unsigned long priority, 
    int ppid, 
    char *name );

void sys_exit_thread (int tid);

void sys_dead_thread_collector (void);


//Sincronização do retraço vertical.
void sys_vsync (void);


//Show kernel info.
void sys_showkernelinfo (void);

//Show pci info.
int sys_showpciinfo (void);


// 178
unsigned long sys_get_file_size ( char *path );



// Usada por vários serviços de debug.
// Usada para debug.
void sys_show_system_info ( int n );


//
// reboot and shutdown
//

void sys_reboot (void);
void sys_shutdown (void);


#endif    


//
// End.
//

