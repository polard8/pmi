// console.h
// Created by Fred Nora.

#ifndef ____CONSOLE_H
#define ____CONSOLE_H  1

#define CONSOLE_DEVICE_KEYBOARD  1
#define CONSOLE_DEVICE_SERIAL    2
#define CONSOLE_DEVICE_NETWORK   3

// Control sequence introducer 
// Parameter    - (zero or more characters)
// Intermediate - (zero or more characters)
// Final        - (one character)
// See: https://vt100.net/emu/ctrlseq_dec.html

struct virtual_console_login_d
{
    int used;
    int magic;
    int id;

    // Index in the list CONSOLE[i].
    // See: tty.h
    int virtual_console_index;

    // 0 = 'text'
    // 1 = 'x-server'
    int mode;
    
    struct user_info_d *user_info;
    
    struct usession_d *usession;
    struct room_d     *room;     //window station
    struct desktop_d  *desktop;
};



//
// == prototypes ================================================
//


//
// output on the console device;
//

//see: tty/console.c
void _console_outbyte (int c, int console_number);
void console_outbyte (int c, int console_number);
void console_putchar ( int c, int console_number );

// Não tem escape sequence
// Funciona na máquina real
ssize_t 
__console_write (int console_number, const void *buf, size_t count);

ssize_t 
console_read (int console_number, const void *buf, size_t count);  

// Tem escape sequence
ssize_t 
console_write (int console_number, const void *buf, size_t count);


void console_scroll (int console_number);

int kclear (int color, int console_number);

int kclearClientArea (int color);

int insert_line ( char *string, int line );

void REFRESH_STREAM ( file *f );

// Virtual console support.
void console_set_current_virtual_console ( int n );
int console_get_current_virtual_console (void);
void console_init_virtual_console (int n);

void jobcontrol_switch_console(int n);

void consoleBlinkTextCursor(void);

int 
console_ioctl ( 
    int fd, 
    unsigned long request, 
    unsigned long arg );

int VirtualConsole_initialize(void);

void console_interrupt(int target_thread, int device_type, int data);

#endif    



