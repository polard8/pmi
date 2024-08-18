/*
 * Arquivo: desktop.h 
 *
 * Descri��o:
 *     Header para o m�dulo gerenciador de desktops.
 *     Um desktop pertence � uma window station.
 *     Um desktop cont�m janelas, menus e outras coisas.
 *     ...
 *
 * #importante:
 * Cada desktop deve ter seu backbuffer.
 * Um ponteiro para o backbuffer deve ser criado na 
 * estrutura de desktop.
 *
 * WINDOW -> DESKTOP -> WINDOW STATION -> USER SECTION.
 * 
 *  2015 - Created by Fred Nora.
 */


#ifndef ____DESKTOP_H
#define ____DESKTOP_H    1


/*
 * desktop_d:
 *     Desktop structure.
 *     It handles a set of gui objects like 'windows'.
 */

struct desktop_d
{
    object_type_t objectType;
    object_class_t objectClass;

    struct object_d *object;

    int desktopId;

    int desktopUsed;
    int desktopMagic;


    // usando o mesmo esquema do usu�rio.
    char __name[64];
    int name_lenght;

    // The window server and the window manager for this desktop.
    
    pid_t ws;   // window server
    pid_t wm;   // window manager
    pid_t ns;   // network server
    //pid_t fs;   // file system
    // ...


    //
    // Background
    //

    // #todo
    // We need a structure to handle the background image 
    // used by this desktop.
    

    // #todo
    // A estrutura de eventos tamb�m desve estar associado
    // a um desktop;
    // Cada wm poder� pegar eventos em um desktop espec�fico.
    // Dessa forma poderemos ter v�rios wm, um para cada desktop.
    // no desktop 0 ficam o ws e o wm que pertencem ao base kernel.
    // kgws e kgwm, :)
    

	// #importante
	// #quantidades
	// Isso limita as quantidades que podem ser criadas em um desktop.
	// permitindo o f�cil gerenciamento de drivers em desktops espec�ficos.
	// Pois alguns desktops podem ter seus pr�prios gerenciadores de servidores.
	// Como o caso do gramado core. Que fica a cargo apenas do kernel 
	// gerenciar esses servidores especiais.
	
	
	int drivers_max;
	int servers_max;
	int apps_max;
	

	// #importante
	//unsigned long backbuffer_address;
	

    int userID;
	
	//prompt de comandos. ??
	//int terminal_TID; 
	
	//Heap do desktop.
    struct heap_d *heap;

//
// Windows and menus
//

    // #todo
    // The root window fot this desktop

    // struct window_d *root_window;


    // Control Menu.
    struct window_d *menuWindow;   //Ponteiro para a janela do menu.
    struct menu_d *sysMenu;        //control menu do sistema.(no icone da janela.)
    struct menu_d *dialogsysMenu;  //control menu para dialogo. (todos outros control menus)
    struct menu_d *menuHScroll;    //menu gen�rico horizontal. (usado no momento) ??
    struct menu_d *menuVScroll;    //menu gen�rico vertical.   (usado no momento) ??

	//Parametros da janela ativa.
	int active_window;             //Id da janela ativa.  
	struct window_d *activeWindow;    //Ponteiro da estrutura da janela ativa.
	
	//Par�metros da janela com o foco de entrada.
	int window_with_focus;            //Id da janela com foco de entrada.
	struct window_d *focusWindow;     //Ponteiro da estrutura da janela com foco de entrada.
	unsigned long procedure;          //Procedimento da janela ativa.	
	
	
	struct window_d *foregroundWindow;    //A janela que em cima das outras.
	struct window_d *messageWindow;       //Janelinha amarela de mensagem. 
	                                      //(tem bot�o fechar)
										  //(N�O � uma message box). 
	//Janela para uma tray do sistema (bandeija atual). 
	//( bandeija de uma janela) 
	//(uma janela pode ter mais de uma badeija).
    struct window_d *trayWindow;         
	
	//Janelinha de informa��o sobre itens 
	//que o mouse repousa em cima. (quadrado).	
    struct window_d *tooltipWindow;      
	
	
	//OBS:
	//O TAB troca a bandeija.
	//As SETAS trocam o item dentro da bandeija.
	//As bandeijas podem estar ligadas por *next.
	
	//struct window_d *arrayWindow;    //Ponteiro para um array de estruturas de janelas.
	//struct window_d *desktop;
	//struct window_d *shell; 
	//struct window_d *taskmanager;
	//struct window_d *programmanager;

    //
    // List of overlapped windows.
    //
    
    // Only application windows,
    // that one with a frame.

    int lTail;
    int lHead;
    unsigned long list[8];

    //struct window_d *o_list;


    // Only the app window. 'Overlapped'.

    //unsigned long windows[256];

    //
    // Menus
    //

    // unsigned long menus[256]; 


	//Continua ...
	
	//Linked List. 
	//( a linked list de janelas no desktop ).
	//struct linkedlist_d *linkedlist;
	

	//Window Station. 
	//( � qual window station pertence esse desktop ? ).
	//struct wstation_d *wstation;
	
	struct room_d *room;
	
	//Lista de processos nessse desktop.
	//unsigned long processList[32];
	//struct process_d processes[32];
	
	//
	// @todo: user ??, Something more ??
	//
	
    struct desktop_d *next;
};
// This is the desktop used by the base kernel.
// So, the kgws and the kgwm can be here. 
extern struct desktop_d *desktop0;
// O desktop atual.
extern struct desktop_d *CurrentDesktop;
//List.
extern unsigned long desktopList[DESKTOP_COUNT_MAX];

// Desktop info
struct desktop_info_d 
{
    // struct desktop_d *desktop; 
    struct window_d  *window; 
    int pid;
    int tid;
};

//
// == prototypes ==================================
//


int RegisterDesktop(struct desktop_d *d); 
void set_current_desktop(struct desktop_d *desktop);

void *get_current_desktop (void);

int get_current_desktop_id (void);


void *CreateDesktop (struct room_d *room);

void 
set_current_menuwindow ( 
    struct desktop_d *desktop, 
    struct window_d *window ); 

// ??
//  what is this?
void 
set_current_foreground ( 
    struct desktop_d *desktop, 
    struct window_d *window ); 

// ??
void change_foreground (struct desktop_d *desktop);

// Window for messages on desktop
void 
set_current_messagewindow ( 
    struct desktop_d *desktop, 
    struct window_d *window ); 

// main tray window for a desktop.
void 
set_current_traywindow ( 
    struct desktop_d *desktop, 
    struct window_d *window ); 


void 
set_current_tooltipwindow ( 
    struct desktop_d *desktop, 
     struct window_d *window ); 


// sysmenu is the default menu when we click
// on the main window with the right mouse button

void 
set_current_sysMenu ( 
    struct desktop_d *desktop, 
    struct menu_d *menu );


void 
set_current_dialogsysMenu ( 
    struct desktop_d *desktop, 
    struct menu_d *menu );
                            
void 
set_current_menuHScroll ( 
    struct desktop_d *desktop, 
    struct menu_d *menu );


void 
set_current_menuVScroll ( 
    struct desktop_d *desktop, 
    struct menu_d *menu );


// Set up the current window server for a desktop.
int desktop_setup_ws ( struct desktop_d *desktop, int ws_pid );

// Set up the current window manager for a desktop.
int desktop_setup_wm ( struct desktop_d *desktop, int wm_pid );


void init_desktop_list (void);
void init_desktop (void);


#endif   


//
// End
//

