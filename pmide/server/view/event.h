
#ifndef __VIEW_EVENT_H
#define __VIEW_EVENT_H    1

// #todo: 
// no gramado as mensagens do systema
// podem se tornar 'gramado_ev' um tipo de evento gramado.
// e a 'mensagem' determina o tipo de evento.
// alem disso podemos ter mais detalhes
// como se o evento vem do sistema ou do window server ...
//


struct gws_keyboard_event_d
{
    int dummy;
};

struct gws_mouse_event_d
{
    int dummy;
};


struct gws_window_event_d
{
    int dummy;
};


struct gws_event_d
{

    //todo: the client
    // ...client

    //standard msg block
    int wid;    // window id.
    int msg;   // msg. (event type).
    unsigned long long1;
    unsigned long long2;
    
    // extra
    //unsigned long long3;
    //unsigned long long4;
    //unsigned long long5;
    //unsigned long long6;

    
    struct gws_keyboard_event_d  kEvent;
    struct gws_mouse_event_d     mEvent;
    struct gws_window_event_d    wEvent;
    // ...
    
    struct gws_event_d *next;
};

// see: main.c
//#test
// Main structure for event.
extern struct gws_event_d gwsEvent;
// see: main.c
//#test
extern struct gws_event_d *gwsCurrentEvent;


/*
 // uma janela tem o foco de entrada e o cursor esta piscando nela.
struct gws_keyboad_focus_d
{
    struct gws_cliend_d *client;
    struct gws_window_d* window;
    
    //...
};
*/

#endif   





