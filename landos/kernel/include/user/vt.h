// vt.h
// Created by Fred Nora.

#ifndef ____VT_H
#define ____VT_H  1

// This struct is an element of window_d struct.
struct vt_d
{
    int used;
    int magic;  
    int id;

    // The PID of the father that owns the terminal window.
    int father_pid;
    
    // The PID of the command running inside the window that is
    // a terminal.
    int child_pid;
    
    // what is the state of this terminal ?
    int state;
   
    //
    // pty channel.
    //
    
    struct tty_d *master;
    struct tty_d *slave;
     
    // Continue
    // ...
};

// Listar os terminais virtuais.
// Isso vai ajudar a mandar as mensagens;
extern unsigned long vtList[32];


//
// == prototypes ==================
//


int 
vt_ioctl ( 
    int fd, 
    unsigned long request, 
    unsigned long arg );

void _vt_outbyte ( int c, struct tty_d *tty );
void vt_outbyte ( int c, struct tty_d *tty );
void vt_scroll (struct tty_d *tty);

// Create a terminal for a window.
// The father is creating a terminal in one of its windows.
int vt_create (struct window_d *window, int father_pid);

// Set child pid given in a window structure.
int vt_set_child ( struct window_d *window, int child_pid );

//init module.
int vt_init_module (void);


#endif    


