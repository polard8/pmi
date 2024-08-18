// kgwm.h
// Created by Fred Nora.

#ifndef __KGWM_H
#define __KGWM_H    1

struct powertrio_client_d
{
    int used;
    int magic;    
    // Position in the list.
    // Maximum 3 clients.
    int index;
    struct window_d *window;
    struct powertrio_client_d *next;
};

struct powertrio_wm_d
{
    int initialized;
    
    // Se o PowerTrio não está em uso,
    // então as combinações de tecla precisam gerar
    // mensagens que serão entregues para outro window manager,
    // no caso o window manager carregável. gwm.bin

    //int Enabled;

    struct desktop_d *desktop;
    
    int selected;
    int head;
    int tail;
    
    unsigned long list[3];
};
extern struct powertrio_wm_d  PowerTrio;


//
// == prototypes ===============================
//

int powertrio_initialize(void);
int powertrio_set_window ( int index, struct window_d *window );
int powertrio_queue( struct window_d *window );
int powertrio_arrange_and_update(void);
int powertrio_next(void);
int powertrio_select_client(int index);

void __kgwm_SendMessageToInitProcess ( int message );

unsigned long 
__kgwm_ps2kbd_procedure ( 
    struct window_d *window, 
    int msg, 
    unsigned long long1, 
    unsigned long long2 ); 

// Registrar um window manager.
int kgwmRegisterWindowManager( int pid );

// Aqui o servidor de janelas escaneia as janelas para saber 
// se o mouse está sobre alguma ... durante a rotina
// são solicitadas informações diretamente no driver de mouse ps2.
int kgwm_mouse_scan_windows (void);

// Mouse dialog.
unsigned long
kgwm_mouse_dialog ( 
    struct window_d *window,
    int msg,
    unsigned long long1,
    unsigned long long2 );

// controles de janela.
unsigned long
kgwm_window_control_dialog ( 
    struct window_d *window,
    int msg,
    unsigned long long1,
    unsigned long long2 );

#endif    


