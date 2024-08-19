// libgws00.h

#ifndef __LIBGWS00_H
#define __LIBGWS00_H    1

//
// ==== Version ====
//


#define GWS_LIB_PROTOCOL           0
#define GWS_LIB_PROTOCOL_REVISION  1


struct libgws_version_d
{
    unsigned long majorVersion;
    unsigned long minorVersion;
    // ...
};
extern struct libgws_version_d  libgwsVersion;



// System call.

void *gws_system_call ( 
    unsigned long a, 
    unsigned long b, 
    unsigned long c, 
    unsigned long d );

void gws_debug_print (char *string);

// Initialize the library.
int gws_initialize_library (void);

// Services.
// Dialog with the library
// IN: service number, ...

void *gws_services ( 
    int service,
    unsigned long arg2,
    unsigned long arg3,
    unsigned long arg4 );

// Send message to a process.                     
int
gws_send_message_to_process ( 
    int pid, 
    int window, 
    int message,
    unsigned long long1,
    unsigned long long2 );


// Send message to a thread.
int 
gws_send_message_to_thread ( 
    int tid, 
    int window, 
    int message,
    unsigned long long1,
    unsigned long long2 ); 



//
// == 3d ==============================================
//

// Plot a point.
int 
gws_plot0 (
    int fd,
    unsigned long x,
    unsigned long y,
    unsigned long z,
    unsigned long color );

// Plot a cube.
int 
gws_plotcube (
    int fd,
    struct gr_cube_d *cube );


// Plot a rectangle.
int 
gws_plotrectangle (
    int fd,
    struct gr_rectangle_d *rect );

// ...

//
// == 2d ==============================================
//


// Draw a char.
int 
gws_draw_char (
    int fd, 
    int window,
    unsigned long x,
    unsigned long y,
    unsigned long color,
    unsigned long c );

// Draw a text.
int 
gws_draw_text (
    int fd, 
    int window,
    unsigned long x,
    unsigned long y,
    unsigned long color,
    char *string );

// Redraw a window.
int 
gws_redraw_window (
    int fd, 
    int window, 
    unsigned long flags );

// Refresh a window.
int gws_refresh_window (int fd, int window );

// Change window position.
int 
gws_change_window_position (
    int fd, 
    int window, 
    unsigned long x, 
    unsigned long y );

// Resize a window.
int 
gws_resize_window ( 
    int fd, 
    int window, 
    unsigned long w, 
    unsigned long h );

// Create a window.
int
gws_create_window ( 
    int fd,
    unsigned long type,        //1, Tipo de janela (popup,normal,...)
    unsigned long status,      //2, Estado da janela (ativa ou nao)
    unsigned long view,        //3, (min, max ...)
    char *windowname,          //4, Título. #todo maybe const char.
    unsigned long x,           //5, Deslocamento em relação às margens do Desktop. 
    unsigned long y,           //6, Deslocamento em relação às margens do Desktop.
    unsigned long width,       //7, Largura da janela.
    unsigned long height,      //8, Altura da janela.
    int parentwindow,          //9, Endereço da estrutura da janela mãe.
    unsigned long onde,        //10, Ambiente.( Está no desktop, barra, cliente ...)
    unsigned long clientcolor, //11, Cor da área de cliente
    unsigned long color );     //12, Color (bg) (para janela simples).

// Reboot.
void gws_reboot (void);

// #test 
// Load a file given a path.
// We are testing the path support.
int 
gws_load_path ( 
    char *path, 
    unsigned long buffer, 
    unsigned long buffer_len );

// Yield the current thread.
void gws_yield(void);

// Refresh the background and yield the current thread.
void gws_refresh_yield (int fd);

// Refresh a given window and yield the current thread.
void gws_refresh_yield2 (int fd, int window);

// Yield the current thread n times.
void gws_yield_n_times (unsigned long n);



void *gws_create_thread ( 
    unsigned long init_eip, 
    unsigned long init_stack, 
    char *name );

void gws_start_thread (void *thread);

int gws_clone_and_execute ( char *name );

unsigned long gws_get_system_metrics (int index);


void gws_enter_critical_section(void);
void gws_exit_critical_section(void);

int gws_create_empty_file ( char *file_name );
int gws_create_empty_directory ( char *dir_name );


// menu
struct gws_menu_d *gws_create_menu (
    int fd,
    int parent,
    int highlight,
    int count,
    unsigned long x,
    unsigned long y,
    unsigned long width,
    unsigned long height,
    unsigned long color );

//menu item
struct gws_menu_item_d *gws_create_menu_item (
    int fd,
    char *label,
    int id,
    struct gws_menu_d *menu);


// get next event.
// the window server return the next event
// from the queue of a client.
struct gws_event_d *gws_next_event (int fd);


// Send event.
// It is gonna be used by the window manager to send 
// events to the window server.
// Remember: The window manager gets the system messages
// end sends window events to the window server.
void 
gws_send_event ( 
    int fd, 
    int window, 
    struct gws_event_d *event );


unsigned long gws_explode_byte (unsigned char data);



void
gws_async_command ( 
    int fd, 
    unsigned long request,
    unsigned long sub_request,
    unsigned long data );


void gws_send_wm_magic ( int fd, int pid );

struct gws_display_d *gws_open_display(char *display_name);

void gws_close_display( struct gws_display_d *display);


int gws_enable_input_method(int method);

int 
gws_default_procedure ( 
    int fd,
    int window, 
    int msg, 
    unsigned long long1, 
    unsigned long long2 );


#endif   

