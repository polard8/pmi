/*
 * File: kgwm.c
 * 
 *     kgwm - kernel gramado window manager.
 * 
 * History:
 *     2017 -  Created by Fred Nora.
 */

// Window manager into the kernel base.
 
// #obs:
// Aplicativo poder�o se conectar com o servidor de recusros
// gr�ficos, principalmente a GUI, que ser� chamada de 
// Gramado Window Manager.
// Outros window managers poder�o ser criados, e eles precisar�o 
// ser registrados e abrirem o servidor para usarem seus recursos.
// do mesmo modo o shell atual poder� se registrar,
// o processo de logon poder� se registrar em um servidor de logon.


#include <kernel.h>


struct powertrio_wm_d  PowerTrio;


int kgwm_status=0;
int kgwm_wm_PID=0;
int kgwm_wm_status=0;


//
// # internal
//

void gui_create_screen (void);
void gui_create_background (void);
void gui_create_logo (void);
void gui_create_taskbar (void);
void gui_create_mainwindow (void);
void gui_create_controlmenu (void);
void gui_create_infobox (void);
void gui_create_messagebox (void);
void gui_create_debug (void);
void gui_create_navigationbar (void);
void gui_create_grid (void);


// =======================================

// #bugbug
// A fun��o diz que estamos tratando do ws, mas a flag afeta o wm.
int gwsOpen (void)
{
    kgwm_status = TRUE;
    return 0;
}

// #bugbug
// A fun��o diz que estamos tratando do ws, mas a flag afeta o wm.
int gwsClose (void)
{
    kgwm_status = FALSE;
    return 0;
}


// #bugbug
// Registrar um window manager.
// #todo: We need a global structure to handle this situation.
// not a global variable.

int kgwmRegisterWindowManager ( int pid )
{
    int Status = FALSE;


    debug_print ("kgwmRegisterWindowManager: [FIXME]\n");

    if ( pid < 0 ){
        printf("kgwmRegisterWindowManager: pid\n");
    }

    // #bugbug
    // We need to fix everything here.

    // Se o kgwm est� em uso?
    // Ent�o n�o podemos realiza essa opera��o.
    if ( kgwm_status != TRUE ){
        Status = FALSE;
        goto fail;

    // Se o kgwm n�o est� em uso ent�o podemos
    // selecionar um processo que ser� usado como wm ?
    }else{
        // kgwm_status = FALSE;
        kgwm_wm_PID = (int) pid;
        kgwm_wm_status = TRUE;
        return TRUE;
    };

fail:
    printf("kgwmRegisterWindowManager: fail\n");
    return Status;
}


/*
 * SetGuiParameters:
 *     Configura a inicializa��o das janelas gerenciadas pelo kernel.
 *     @todo: Limite de janelas (+- 10)
 *
 */
 
void 
SetGuiParameters(
    int refresh,        //Flag. Colocar ou n�o o backbuffer na tela.
    int screen,         //Tela do computador.
    int background,     //Background da area de trabalho. 
    int logo,           //Logo, Janela para imagem da �rea de trabalho.
    int taskbar,        //Barra de tarefas.(�cones para programas).
    int main,           //Janela principal.(Desktop, ou Kernel Setup Utility). 
    int menu,           //Control menu da �rea de trabalho.
    int infobox,        //Janelinha no canto, para alerta sobre sobre eventos ocorridos no sistema.
    int messagebox,     //O message box do kernel.(Cada app cria seu messagebox). 
    int debug,          //Janela do debug do kernel.(sonda por problemas).
    int navigationbar,  //Janela de navega��o(Semelhante a navigation bar dos dispositivos mobile). 
    int grid )          //Grid da janela principal.
{ 

    //Passados via argumento.
	
	//
	//@todo: gui status. fail or not.
	//
	
	// Checa validade da estrutura.

    if ( (void *) gui == NULL ){
        debug_print ("SetGuiParameters: [FAIL] gui\n");
        return;
    }else{
        gui->refresh          = refresh; 
        gui->screenStatus     = screen;
        gui->backgroundStatus = background; 
        gui->mainStatus       = main;
        gui->logoStatus       = logo; 
        gui->taskbarStatus    = taskbar;
        gui->menuStatus       = menu;
        gui->infoboxStatus    = infobox;
        gui->messageboxStatus = messagebox;
        gui->debugStatus      = debug;
        gui->navigationbarStatus = navigationbar; 
        gui->gridStatus = grid;
        // ...
    };


    //
    // #todo: 
    // More ?!!
    //
}


/*
 *****************************************
 * gui_create_screen:
 *     Cria a tela. (Screen)
 *     Atribuindo as dimens�es.
 *     ...
 * #todo: 
 * Cria buffer dedicado.
 */

void gui_create_screen (void){

    struct window_d  *hWindow; 


    unsigned long Left   = (unsigned long) SCREEN_DEFAULT_LEFT;
    unsigned long Top    = (unsigned long) SCREEN_DEFAULT_TOP;
    unsigned long Width  = (unsigned long) screenGetWidth();
    unsigned long Height = (unsigned long) screenGetHeight();


	// #important: 
	// N�o tem Parent Window!

    if ( (void *) gui == NULL ){
        debug_print ("gui_create_screen: [FAIL] gui\n");
        return;
    }

	//Window:
	//Pintado uma janela simples, toda preta, do tamanho da tela 
	//do sispositivo.

    hWindow = (void *) CreateWindow ( 
                           1, 0, VIEW_MINIMIZED, "Screen",
                           Left, Top, Width, Height,
                           0, 0, 0, COLOR_BLACK );

    if ( (void *) hWindow == NULL ){
         panic ("gui_create_screen: [FAIL] hWindow\n");
    }else{
        RegisterWindow (hWindow);
        kgwmSetActiveWindow (hWindow); 

        // Isso impede que rotinas mudem as caracter�scicas 
        // da janela principal 
        // sem antes destravar ou sem ter autoriza��o para isso.

        windowLock (hWindow); 

		// Estrutura gui.
        if ( (void *) gui != NULL ){
            gui->screen = (void *) hWindow;
        }

        //Desktop.
        //a janela pertence ao desktop 0
        //hWindow->desktop = (void*) desktop0;

		//Nothing.
    };

	// @todo: 
	// More ??!!

    // #bugbug: 
	// #importante:
	// N�o usar set focus nessa que � a primeira janela.
}


/*
 * gui_create_background:
 *     Cria o background. Pinta ele de uma cor.
 *     O background � filha da janela screen.
 *
 * Criando um pano de fundo do mesmo tamaho da tela,
 * Penso que � nessa janela que podemos carregar uma imagem de 
 * pano de fundo.
 *
 * #todo: 
 * Cria buffer dedicado.
 */

// #todo
// Change the return type to 'int'.

void gui_create_background (void){
 
    struct window_d  *hWindow; 

    unsigned long Left   = (unsigned long) SCREEN_DEFAULT_LEFT;
    unsigned long Top    = (unsigned long) SCREEN_DEFAULT_TOP;
    unsigned long Width  = (unsigned long) screenGetWidth();
    unsigned long Height = (unsigned long) screenGetHeight();


    if ( (void *) gui == NULL ){
        panic ("gui_create_background: [FAIL] gui\n");
    }


	//Window:
	//0x00808000 (verde)
	//Criando um pano de fundo do mesmo tamaho da tela,
	//Penso que � nessa janela que podemos carregar uma imagem de 
	//pano de fundo.

    hWindow = (void *) CreateWindow ( 
                           WT_SIMPLE, 0, VIEW_MINIMIZED, "Background", 
                           Left, Top, Width, Height,
                           gui->screen, 0, 0, 0x00008080 );

    if ( (void *) hWindow == NULL ){
        panic ("gui_create_background: hWindow\n");
    }

    if ( hWindow->used != TRUE || hWindow->magic != 1234 )
    {
        panic ("gui_create_background: hWindow validation\n");
    }

    // Register

    RegisterWindow (hWindow);

    // Activate.
    // ?? Is this right ??

    kgwmSetActiveWindow (hWindow); 
    
    // Set focus

    kgwmSetFocus (hWindow);

    // Lock
    
    windowLock (hWindow);  


    // Save the pointe in the main structure.
    
    if ( (void *) gui != NULL )
    {
        gui->background = (void *) hWindow;
    }

    // Desktop.
    // a janela pertence ao desktop 0
    // hWindow->desktop = (void*) desktop0; 
}


/*
 *********************************************************
 * gui_create_taskbar:
 *      Cria a taskbar da �rea de trabalho. gui->taskbar.
 *      Cria somente a janela que ser� uada pelo file manager.
 * e que servir� de refer�ncia para a janela gui->main
 */

void gui_create_taskbar (void)
{

    struct window_d *hWindow; 


    unsigned long Left   = (unsigned long) SCREEN_DEFAULT_LEFT;
    unsigned long Top    = (unsigned long) SCREEN_DEFAULT_TOP;
    unsigned long Width  = (unsigned long) screenGetWidth();
    unsigned long Height = (unsigned long) screenGetHeight();

    if ( (void *) gui == NULL ){
        debug_print ("gui_create_taskbar: gui\n");
        return;
    }

    // draw bar

    Height = (Height/8);

    hWindow = (void *) CreateWindow ( 
                           1, 0, VIEW_MINIMIZED, "taskbar", 
                           Left, Top, Width, Height, 
                           gui->screen, 0, 0, COLOR_WHITE );

    if ( (void *) hWindow == NULL){
        panic ("gui_create_taskbar: [FAIL] \n");
    }else{
        RegisterWindow (hWindow);
        windowLock (hWindow); 

        if ( (void *) gui != NULL )
        {
            gui->taskbarStatus = TRUE;
            gui->taskbar       = (void *) hWindow;
        }

		//...
    };

	// ...
}


/*
 ***********************************************
 * gui_create_mainwindow:
 *      A �rea de trabalho.
 *
 *      *Importante: � a �rea dispon�vel na tela para o aplicativo. 
 *      Onde ficam os �cones.
 *      
 *     A �rea de trabalho � toda a tela menos a GMB(Global Menu Bar) e 
 * menos a TaskBar. 
 * #todo: 
 * Criar o buffer dedicado para a janela principal.
 *
 * #Obs: 
 * Essa janela � especial e usar� o Backbuffer como buffer dedicado.
 */

void gui_create_mainwindow (void){

    struct window_d *hWindow; 


    unsigned long Left   = (unsigned long) SCREEN_DEFAULT_LEFT;
    unsigned long Top    = (unsigned long) SCREEN_DEFAULT_TOP;
    unsigned long Width  = (unsigned long) screenGetWidth();
    unsigned long Height = (unsigned long) screenGetHeight();


	//estrutura gui.
    if ( (void *) gui == NULL ){
        panic ("gui_create_mainwindow: [FAIL] gui\n");
    }

	//janela taskbar.
    if ( (void *) gui->taskbar == NULL ){
        panic ("gui_create_mainwindow: [FAIL] taskbar\n");
    }


	//
	// Usando a janela taskbar como refer�ncia para criar a janela main.
	//

	//Top = gui->taskbar->bottom;


	//Window:
	//Color? (?? Nem precisa pintar, mas precisa criar ??)
	//�rea de trabalho.
	//Um ret�ngulo que pode ser menor que o tamanho da tela do 
	//dispositivo.
	//� onde ficam as janelas dos aplicativos.
	//A janela principal perence ao desktop

    hWindow = (void *) CreateWindow ( 
                           1, 0, VIEW_MINIMIZED, "desktop window", 
                           Left, Top, Width, Height,           
                           gui->screen, 0, 0, COLOR_WHITE );   

    if ( (void *) hWindow == NULL){
        panic ("gui_create_mainwindow: [FAIL] hWindow\n");
     }else{   
        RegisterWindow (hWindow);
        windowLock (hWindow); 
        kgwmSetActiveWindow (hWindow); 

        //a janela pertence ao desktop 0
        //hWindow->desktop = (void*) desktop0;

        if ( (void *) gui == NULL ){
            panic ("gui_create_mainwindow: [FAIL] gui\n");
            //return;
        }else{
            gui->main = (void *) hWindow;
        };
    };


    //  Desktop Window:

    gui->root_window = (void *) gui->main;

    if ( (void *) gui->root_window == NULL)
    {
        gui->root_window = (void *) gui->screen;

        if ( (void *) gui->root_window == NULL )
        { 
            kgwmSetFocus (hWindow);
            return; 
        }
    }


	//N�o registrar pois main menu ja est� registrada.
	//RegisterWindow(gui->desktop);

    kgwmSetFocus (hWindow);
}


void gui_create_logo (void)
{ 
    debug_print ("gui_create_logo: Nothing for now\n");
}

void gui_create_controlmenu (void)
{ 
    debug_print ("gui_create_controlmenu: deprecated\n");
}



/*
 *gui_create_infobox:
 * janelinha de informa��es do sistema.
 * avisa o usuario sobre eventos do sistema.
 *como novos dispositivos encontrados.
 * @todo: Cria buffer dedicado.
 */

void gui_create_infobox (void){
 
    if ( (void *) gui == NULL ){
        debug_print ("gui_create_infobox: gui\n");
        return;
    }else{
        gui->infobox = NULL;
    };    
}


/*
 * Cria a janela para message box.
 */

void gui_create_messagebox (void){
 
    if ( (void *) gui == NULL ){
        debug_print ("gui_create_messagebox: gui\n");
        return;
    }else{
        gui->messagebox = NULL;
    };
}

// #deprecated
void gui_create_debug (void)
{
    panic("gui_create_debug: deprecated\n");
    return NULL;
}


void gui_create_navigationbar (void)
{
    debug_print ("gui_create_navigationbar: Nothing for now\n");
}


/*
 **************************
 * gui_create_grid:
 *
 *     Apenas autoriza a utiliza��o de um grid na area de trabalho. 
 *     Havendo essa altoriza��o, o gerenciador de arquivos poder�
 *     desenh�-lo. 
 *     cada item ser� um arquivo da pasta "area de trabalho".
 * 
 * #todo: 
 * Cria buffer dedicado.
 */

void gui_create_grid (void){
 
    if ( (void *) gui == NULL ){
        return;
    }else{
		//initScreenGrid();
        gui->grid = NULL;
    };
}


//screen
//m�trica do dispositivo.

void *guiGetScreenWindow (void){

    if ( (void *) gui == NULL )
    {
        return NULL;
    }

    return (void *) gui->screen;
}


// #deprecated
void *guiGetDeveloperScreenWindow (void)
{
    panic("guiGetDeveloperScreenWindow: deprecated\n");
    return NULL;
}


//background
void *guiGetBackgroundWindow (void){

    if ( (void *) gui == NULL )
    {
        //message
        return NULL;
    }

    return (void *) gui->background;
}


//logo

void *guiGetLogoWindow (void){

    if ( (void *) gui == NULL )
    {
        return NULL;
    }

    return (void *) gui->logo;
}


//desktop window
void *guiGetDesktopWindow (void){

    if ( (void *) gui == NULL )
    {
        return NULL;
    }

    return (void *) gui->root_window;
}


//task bar, top window, mac style

void *guiGetTaskbarWindow (void){

    if ( (void *) gui == NULL )
    {
         return NULL;
    }


    return (void *) gui->taskbar;
}


//main window
//GetDesktopWindow

void *guiGetMainWindow (void){

    if ( (void *) gui == NULL )
    {
        return NULL;
    }

    return (void *) gui->main;
}


// #deprecated.
void *guiGetStatusbarWindow (void)
{
    panic("guiGetStatusbarWindow: deprecated\n");
    return NULL;
}


//grid window (do kernel base)

void *guiGetGridWindow (void){

    if ( (void *) gui == NULL )
    {
        return NULL;
    }

    return (void *) gui->grid;
}


//janela do 'control menu' atual.

void *guiGetMenuWindow (void){

    if ( (void *) gui == NULL )
    {
        return NULL;
    }

    return (void *) gui->menu;
}


//infobox (o sistema envia mensagens de informa��o)

void *guiGetInfoboxWindow (void){

    if ( (void *) gui == NULL )
    {
        return NULL;
    }


    return (void *) gui->infobox;
}


//tooltip  Janelinha que aparece quando repousa o mouse sobre elemento gr�fico.

void *guiGetTooltipWindow (void){

    if ( (void *) gui == NULL )
    {
        return NULL;
    }

    return (void *) gui->tooltip;
}


//message box do kernel base. 

void *guiGetMessageboxWindow (void){

    if ( (void *) gui == NULL )
    {
        return NULL;
    }

    return (void *) gui->messagebox;
}


//dialogbox do kernel base. 

void *guiGetDialogboxWindow (void){

    if ( (void *) gui == NULL )
    {
        return NULL;
    }

    return (void *) gui->dialogbox;
}


// #deprecated
void *guiGetDebugWindow (void)
{
    panic("guiGetDebugWindow: deprecated\n");
    return NULL;
}


// ?
// menubar usada pelo kernel base.

void *guiGetMbhWindowWindow (void){

    if ( (void *) gui == NULL )
    {
        return NULL;
    }

    return (void *) gui->mbhWindow;
}


//top bar.
void *guiGetTopbarWindow (void){

    if ( (void *) gui == NULL )
    {
        return NULL;
    }

    return (void *) gui->topbar;
}


//navigation bar, estilo mobile.
//gerenciada pelo kernel base.

void *guiGetNavigationbarWindow (void){

    if ( (void *) gui == NULL )
    {
        return NULL;
    }

    return (void *) gui->navigationbar;
}


// janela do shell do kernel base.
// na� � um processo.

void *guiGetShellWindowWindow (void)
{
    panic("guiGetShellWindowWindow: deprecated\n");
    return NULL;
}


//uma janela filha do shell do kernel base.
void *guiGetShellClientWindowWindow (void)
{
    panic("guiGetShellClientWindowWindow: deprecated\n");
    return NULL;
}


// Reposiciona e muda o tamanho da gui->main window.
// configura a �rea de trabalho.

void 
guiSetUpMainWindow ( 
    unsigned long x, 
    unsigned long y, 
    unsigned long width, 
    unsigned long height )
{

    unsigned long Left   = (unsigned long) SCREEN_DEFAULT_LEFT;
    unsigned long Top    = (unsigned long) SCREEN_DEFAULT_TOP;
    unsigned long Width  = (unsigned long) screenGetWidth ();
    unsigned long Height = (unsigned long) screenGetHeight ();


	//check limits

    if ( x > Width || y > Height )
    { return; }

    if ( width > Width || height > Height ) 
    { return; }

    if ( (void *) gui == NULL )
    { return; }


    if ( (void *) gui->main == NULL ){
        // msg?
        return;

    }else{
        replace_window (gui->main, x, y);
        resize_window ( gui->main, width, height);
    };

	//Nothing.
}




// =====================================================
// service 301
// Change active window.
// Switch to the next window.
// It changes the foreground thread.
// The new foreground thread will be the thread
// associated with this new window.
// Valid for overlapped windows.

// control + [f5~f8] 


// The list is a element o a given desktop, ex: CurrentDesktop->list[i].
// So, this way we can have differentt desktops using this list.
// #test: Maybe one desktop, using the gws, can use the window structure
// that belongs to kgws to handle the app windows, given to the kgwm
// the opportunity to switch between those windows, 
// changing the active window. 

int kgwm_next(void)
{
    struct window_d  *wActive;  // active window
    struct window_d  *n;        // next window

    struct thread_d *t;


    // #todo:
    // Talvez possamos receber esse ponteiro via argumento.
    // Por enquanto vamos usar o CurrentDesktop.

    struct desktop_d  *TargetDesktop;

    TargetDesktop = (struct desktop_d  *) CurrentDesktop;


    debug_print("kgwm_next: [FIXME]\n");

    //#debug
    //return -1;
    
    
    // #test
    //===============
    // main window
    // See: logon.c
    if ( (void*) gui == NULL ){
        panic ("kgwm_next: gui\n");
    }
    // Do now refresh now.
    if ( (void*) gui->main != NULL ){
        redraw_window ( gui->main, FALSE );
    }
    //===============

    // #todo
    // Se n�o temos uma janela ativa 
    // ent�o temos que providenciar uma.

    if ( active_window < 0){
        panic ("kgwm_next: [FAIL] active_window\n");
        //return -1;
    }

    wActive = (struct window_d *) windowList[active_window];

    if ( (void*) wActive == NULL ){
        panic ("kgwm_next: wActive\n");
    }
    
    if (wActive->used != TRUE || wActive->magic != 1234 )
    {
        panic ("kgwm_next: wActive validation\n");
    }

    // Somente para esse tipo.
    if ( wActive->type != WT_OVERLAPPED ){
        panic("kgwm_next: wActive Invalid type\n");
    }

    //
    // Get next
    //
    
    n = (struct window_d *) wActive->next;
    
    // Se a pr�xima indicada na navega��o � v�lida. Usaremos ela.
    if ( (void*) n != NULL )
    {
        if (n->used == TRUE && n->magic == 1234 )
        {
            debug_print ("kgwm_next: next gotten\n");
            goto __go;
        }
    }

    // A next � inv�lida,
    // ent�o vamos usar a lista que est� no desktop.

    if ( (void *) TargetDesktop == NULL ){
        panic ("kgwm_next: TargetDesktop\n");
    }

    if ( TargetDesktop->desktopUsed  != TRUE && 
         TargetDesktop->desktopMagic != 1234 )
    {
        panic ("kgwm_next: TargetDesktop validation\n");
    }


    //
    //  Check list
    //


    // Procurar a janela ativa dentro da lista.
    // � uma list de janelas que pertencem ao desktop atual.

    int i=0;
    struct window_d *tmp;


    int NewWindowSelected=FALSE;

    // limits
    if (TargetDesktop->lHead < 0 || TargetDesktop->lHead >= 8)
    {
        TargetDesktop->lHead = 0;
    }

    i = TargetDesktop->lHead;

    // Get the window

    tmp = (struct window_d *) TargetDesktop->list[i]; 
    
    // circula
    // se der certo ou nao
    TargetDesktop->lHead++;
    if (TargetDesktop->lHead >= 8)
    {
        TargetDesktop->lHead = 0;
    }

    // fail
    if ( (void*) tmp == NULL )
    {
        n = (struct window_d *) wActive;
        debug_print("kgwm_next: [fail] list FAIL. Back to active\n");
        goto __go;
    }        

    // ok
    if ( (void*) tmp != NULL )
    {
        if ( tmp->used != TRUE && tmp->magic != 1234 )
        {
            n = (struct window_d *) wActive;
            debug_print("kgwm_next: [fail] list FAIL. Back to active\n");
            goto __go;
        }

        // New window!
        
        if ( tmp->used == TRUE && tmp->magic == 1234 )
        {
            NewWindowSelected = TRUE;
            n = (struct window_d *) tmp;
            debug_print("kgwm_next: [ok] One from the list\n");
            goto __go;
        }
    }


//
// == Go ===========================
//

__go:

    // Last check.

    if ( (void*) n == NULL ){
        panic("kgwm_next: [__go] invalid n\n");
    }

    if (n->used != TRUE || n->magic != 1234 )
    {
        panic("kgwm_next: [__go] n validation\n");
    }

    // Is it a overlapped window?
    if (n->type != WT_OVERLAPPED){
        panic("kgwm_next: [__go] not WT_OVERLAPPED\n");
    }

    //
    // Thread
    //

    // tid
    // #todo: overflow?

    if (n->tid < 0){
        panic("kgwm_next: [__go] no tid for this window\n");
    }

    // Change the foreground thread.
    
    // Temos que tomar cuidado pra n�o colocarmos
    // como foreground thread, uma thread que est� num estado
    // ruim, como zumbi.
    
    t = (struct thread_d *) threadList[ n->tid ];

    if ( (void*) t == NULL ){
        panic("kgwm_next: [__go] invalid thread\n");
    } 

     if ( t->used != TRUE || t->magic != 1234 )
     {
          panic("kgwm_next: [FAIL] thread validation\n");
     } 

     if ( t->state == READY || 
          t->state == RUNNING )
     {
         // ok
         // A next � v�lida.

         debug_print("kgwm_next: [DONE]\n");
         
         
         // Old active window
         debug_print("kgwm_next:  [DONE] kill focus of old active window\n");
         if ( NewWindowSelected == TRUE )
         {
             wActive->active = FALSE;
             kgwmKillFocus(wActive);
             redraw_window(wActive,TRUE);
         }
         
         // New active window.
         debug_print("kgwm_next:  [DONE] redraw new active window\n");
         active_window = n->id;
         n->active = TRUE;
         kgwmSetFocus(n);  // Focus
         foreground_thread = n->tid;          // Change the foreground window.
         redraw_window(n,TRUE);


         // #debug
         draw_text(n,8,8,COLOR_RED,n->name);
         refresh_screen();

         // Envia uma mensagem para a thread pedindo
         // pra ela atualizar a janela principal.
         // 11216.
         kgws_send_to_tid (
             (int) n->tid,               // tid
             (struct window_d *) n,      // NULL
             (int)               11216,  // Message Code
             (unsigned long)     0,      // MAGIC signature, ascii code
             (unsigned long)     0 );    // MAGIC signature, raw byte
            
         debug_print("kgwm_next:  [DONE] done\n");
         return 0;
     } 
     
     // drop
     // Se a condi��o acima n�o foi atendida
     // ent�o continuaremos com a configua��o antiga.
     
     // panic("kgwm_next: [FAIL] thread state\n");
}


/*
 *********************** 
 * register_wm_process: 
 * 
 *     Register the loadable wm process.
 */

// #todo
// We need to have a loadable window manager first,
// this is not valid for the kgws environmet.

int register_wm_process ( pid_t pid ){

    if ( pid<0 || pid >= PROCESS_COUNT_MAX )
    {
        debug_print("register_wm_process: [FAIL] pid\n");
        return -1;
    }

    // We can't
    // The system already has a wm.

    if ( __gpidWindowManager != 0 ){
        debug_print("register_wm_process: [FAIL] __gpidWindowManager\n");
        return -1;
    }

    // Register.

    __gpidWindowManager = (pid_t) pid;

    return 0;
}


/*
 ****************************** 
 * kgws_mouse_scan_windows:
 *     Com essa fun��o o window server pega uma mensagem vinda do
 * driver de mouse e escaneia as janelas pra comparar com o 
 * posicionamento do ponteiro do mouse.
 * 
 * #todo
 * Por enquando o pr�prio handler do mouse est� chamando essa rotina.
 */

// #bugbug
// Essa fun�ao perdeu seu proposito aqui no ws dentro do kernel
// Podemos copiar ela para o gwssrv.bin em ring3.


// #bugbug
// Algumas vari�veis usadas aqui est�o no driver de mouse ps2
// Precisamos delas aqui, pois n�o possuem utilidade l�.
// aqui o servidor de janelas escaneia as janelas para saber 
// se o mouse est� sobre alguma ... durante a rotina
// s�o solicitadas informa��es diretamente no driver de mouse ps2.

// #obs
// Isso � chamado pelo mouse em ps2mouse.c

// Estamos mandando o evento para a thread associada `a
// janela 'a qual o mouse esta passando por cima.
// Isso nao muda a thread que esta em foreground.


// helper structure
struct PS2MouseEvent_d
{
    int buttom_1;
    int buttom_2;
    int buttom_3;

    int old_buttom_1;
    int old_buttom_2;
    int old_buttom_3;

    unsigned long saved_x;
    unsigned long saved_y;
    unsigned long x;
    unsigned long y;

    int moving;
    int drag_status;
    int pressed;

    // Se houve ou n�o alguma a��o envolvendo bot�es.
    int button_action;

    // Salvaremos aqui o �ltimo total ticks pra pegarmos um delta, 
    // se o delta for menor que o limite ent�o temos um duploclick.

    unsigned long current_totalticks;
    unsigned long last_totalticks;
    unsigned long delta_totalticks;
};

// #todo: Rever isso.
int flagRefreshMouseOver;

// Declaration.
// Used in kgwm_mouse_scan_windows.

struct PS2MouseEvent_d PS2MouseEvent;

// Scan.

int kgwm_mouse_scan_windows (void)
{
    // #importante:
    // Essa ser� a thread que receber� a mensagem.

    struct thread_d *t;

	// #importante:
	// Essa ser� a janela afetada por qualquer evento de mouse.
	// ID de janela.

    struct window_d *Window;
    int wID = -1;


    // Chamar o driver de mouse ps2 pra pegar as informa��es
    // sobre o mouse;
    
    // #todo
    // Temos que pegar um pacote com todas as informa��es de uma vez.

    // Pegando as informa��es.
    PS2MouseEvent.saved_x  = ps2_mouse_get_info (1);
    PS2MouseEvent.saved_y  = ps2_mouse_get_info (2);
    PS2MouseEvent.x        = ps2_mouse_get_info (3);
    PS2MouseEvent.y        = ps2_mouse_get_info (4);
    PS2MouseEvent.moving   = ps2_mouse_get_info (5);
    //PS2MouseEvent.drag_status  = ps2_mouse_get_info (6);
    PS2MouseEvent.button_action  = ps2_mouse_get_info (7);
    PS2MouseEvent.buttom_1 = ps2_mouse_get_info (8);
    PS2MouseEvent.buttom_2 = ps2_mouse_get_info (9);
    PS2MouseEvent.buttom_3 = ps2_mouse_get_info (10);
    PS2MouseEvent.old_buttom_1 = ps2_mouse_get_info (11);
    PS2MouseEvent.old_buttom_2 = ps2_mouse_get_info (12);
    PS2MouseEvent.old_buttom_3 = ps2_mouse_get_info (13);
    //PS2MouseEvent.pressed = ps2_mouse_get_info (14);
    // ...

    //printf ("b=%d ",buttom_1);

	// #refletindo: 
	// ?? E no caso de apenas considerarmos que o mouse est� se movendo, 
	// mandaremos para janela over. ???
	
	// #refletindo:
	// Obs: A mensagem over pode ser enviada apenas uma vez. 
	// ser� usada para 'capturar' o mouse ... 
	// e depois tem a mensagem para 'descapturar'.
	
	
    // ## Sem escaneamento de janelas por enquanto, apenas mostre e mova o ponteiro ##
	
	//## ent�o n�o enviaremos mensagens para a thread ###
	
	//
	//  ## Scan ##
	//
	
	//===========
	// (capture) - On mouse over. 
	//

	// wID = ID da janela.
	// Escaneamos para achar qual janela bate com os valores indicados.
	// Ou seja. Sobre qual janela o mouse est� passando.
	
	// #BUGBUG
	// O problema nessa t�cnica s�o as sobreposi��o de janelas.
	// Quando uma janela est� dentro da outr, ent�o duas janelas
	// est�o �ptas a serem selecionadas.
	// Talvez devamos filtrar e s� aceitarmos sondar procurando 
	// por controles.

	// #IMPORTANTE
	// Se for v�lido e diferente da atual, significa que 
	// estamos dentro de uma janela.
	// -1 significa que ouve algum problema no escaneamento.
	
	//wID = (int) windowScan ( mouse_x, mouse_y );

    
	//#importante:
	//Ja que n�o passamos em cima de um bot�o ou editbox,
	//ent�o vamos ver se estamos em cima de uma janela overlapped.
    //#obs: isso ficou bom ... estamos testando
    
    // #bugbug
    // Com isso o sistema trava quando tentamos mover o mouse
    // no programa gdeshell, que � fullscreen.
    //Podemos tentar desabilitar o mouse quando entrarmos em fullscreen
    // no gdeshell(teste)?
    
    //#todo: 
    //N�o h� porque sondar janelas se tivermos em full screen.
    //pois somente teremos a �rea de cliente de uma das janelas.

    int __saved = 0;
    
    
    /*
    wID = (int) windowOverLappedScan ( x, y );
    __saved = wID;
    
    //se pegamos uma overlapped, vamos tentar pegar um bot�o ou editbox.
    if ( wID != -1 )
    {
        wID = (int) windowScan ( x, y );
    
        //Se n�o pegamos um bot�o ou editbox.
        // ent�o ficaremos com a janela overlapped salva.
        if ( wID == -1)
        {
            wID = __saved;
        }
    }
    */    
    

    // #test
    // Estamos em cima de qual janela?

    wID = (int) top_at ( PS2MouseEvent.x, PS2MouseEvent.y );

    // #bugbug
    // Estamos sempre recebendo a janela gui->screen

    //================
    // -1 = Se n�o temos uma janela.
    // if ( wID < 0 )
    if ( wID == -1 )
    { 
		//printf ("x");

		// Essa flag indica que podemos fazer o refresh da mouse ouver,
		// mas somente uma vez.
        if ( flagRefreshMouseOver == 1 )
        {
            Window = (struct window_d *) windowList[mouseover_window];

			//#bugbug:
			//precisamos checar a validade da estrutura antes de usa-la.

			//#importante:
			//isso apaga o que pintamos na janela, o valor foi salvo logo abaixo.

            if ( (void *) Window != NULL )
            {
                //refresh_rectangle ( Window->left, Window->top, 20, 20 );
                //refresh_rectangle ( savedmouseoverwindowX, savedmouseoverwindowY, 20, 20 );

            }

			// N�o podemos mais fazer refresh.
            flagRefreshMouseOver = 0;

			//#importante inicializa.
			//isso � global do sistema?
            mouseover_window = 0;
        }
     
        return -1;
     }


    //++
	// Se n�o houve problema no escaneamento de janela ou seja, se encontramos 
	// uma janela. Ent�o essa janela deve estar associada � uma thread para qual 
	// mandaremos a mensagem. Caso a thread for null ... apenas n�o enviamos.
	// A janela tem uma thread de controle, igual ao processo.
	
	// #importante
	// Mandaremos mensagem para a thread de controle da janela
	// ao qual o mouse est� passando por cima.
	// Isso est� funcionando. Mas qual � a thread das janelas filhas?
	// Elas herdam a thread de controle ??

    //============================ 
    //Se estamos sobre uma janela v�lida.
    if ( wID >= 0 )  //if ( wID > -1 )
    {
		//printf ("w ");

        Window = (struct window_d *) windowList[wID];

        if ( (void *) Window == NULL ) { return -1; }
        if ( Window->used != 1 || Window->magic != 1234){ return -1; }


		//#importante:
		//Nesse momento temos uma janela v�lida, ent�o devemos 
		//pegar a thread associada � essa janela, dessa forma 
		//enviaremos a mensagem para a thread do aplicativo ao qual 
		//a janela pertence.
		//E se a janela for um bot�o? ser� a mesma thread de controle
		//da janela overlapped ao qual ela � filha?

        t = (void *) Window->control;

        if ( (void *) t == NULL ){ return -1; }
        if ( t->used != 1 || t->magic != 1234 ){ return -1; }

		//#bugbug 
		//#todo:
		// Deveriamos aqui checarmos a validade da estrutura ??

        //
        // ==== Button events ====
        //
 
		// #importante
		// Se um bot�o foi pressionado ou liberado, ent�o enviaremos uma 
		// mensagem relativa ao estado do bot�o, caso contr�rio, enviaremos 
		// uma mensagem sobre a movimenta��o do mouse.

		//Qual bot�o mudou seu estado??
		//Checaremos um por um.


		//===============================================
		// ***Se houve mudan�a em rela��o ao estado anterior.
		// Nesse momento um drag pode terminar
        if ( PS2MouseEvent.button_action == TRUE )
        {
			//printf ("[Action ");
			
			 //debug
			 //if( Window->type == WT_OVERLAPPED)
             //{ printf ("[Action overlapped "); refresh_screen(); }
              
			// >> BOT�O 1 ==================
			//Igual ao estado anterior
            if ( PS2MouseEvent.buttom_1 == PS2MouseEvent.old_buttom_1 )
            {
				//...
			
			//Diferente do estado anterior.
            }else{
				
				//printf ("Dif ");
				
				// down - O bot�o 1 foi pressionado.
                if ( PS2MouseEvent.buttom_1 == 1 )
                {
					//printf ("Press\n");
					 
					//clicou
					// se o old estava em 0 ent�o o atual est� em um.
                    if ( PS2MouseEvent.old_buttom_1 == 0 )
                    {
						// flag: um bot�o foi pressionado.
						PS2MouseEvent.pressed = 1;
						
						// Enviaremos a mensagem para a thread atual.
						// houve altera��o no estado do bot�o 1 e estamos em cima de uma janela.
                        if ( (void *) Window != NULL )
                        {
							//ps2mouse_change_and_show_pointer_bmp(4); //folder bmp
							//pegamos o total tick
                            PS2MouseEvent.current_totalticks = (unsigned long) get_systime_totalticks();
                            PS2MouseEvent.delta_totalticks = (PS2MouseEvent.current_totalticks - PS2MouseEvent.last_totalticks); 
                            //printf ( "x=%d l=%d d=%d \n",
                               //current_totalticks, last_totalticks, delta_totalticks ); 
                               //refresh_screen();
                            PS2MouseEvent.last_totalticks = PS2MouseEvent.current_totalticks;
                            
                            // Kernel single event.
                            // #bugbug: 
                            // N�o estamos usando esse tipo de mensagem,
                            // somente a fila.


                            // New way. Use this one!
                            t->window_list[ t->tail_pos ] = Window;
                            t->msg_list[ t->tail_pos ]    = MSG_MOUSEKEYDOWN;
                            t->long1_list[ t->tail_pos ]  = 1;
                            t->long2_list[ t->tail_pos ]  = 0;

                            // Modifying event type.
                            if (PS2MouseEvent.delta_totalticks < 1000) //2000
                            {
                                t->ke_msg = MSG_MOUSE_DOUBLECLICKED; 
                                t->msg_list[ t->tail_pos ] = MSG_MOUSE_DOUBLECLICKED;
                                PS2MouseEvent.delta_totalticks=8000; // delta inv�lido.
                            }

                            t->tail_pos++;
                            if ( t->tail_pos >= 31 )
                                t->tail_pos = 0;

							//estamos carregando o objeto
							//PS2MouseEvent.drag_status = 1;                        
                        }
                        //else: // houve altera��o no estado do bot�o 1 mas n�o estamos em cima de uma janela.
                        
                        
						//Atualiza o estado anterior.
                        //old_buttom_1 = 1;
                        PS2MouseEvent.old_buttom_1 = PS2MouseEvent.buttom_1;
                    }

				// up - O bot�o 1 foi liberado.
                }else{
					
					//printf ("Rel\n");

				    // flag: um bot�o foi liberado.
				    PS2MouseEvent.pressed = 0;
						
					// #importante 
					// Enviaremos a mensagem para a thread atual.
                    if ( (void *) Window != NULL )
                    { 
                        // New way. Use this one!
                        t->window_list[ t->tail_pos ] = Window;
                        t->msg_list[ t->tail_pos ]    = MSG_MOUSEKEYUP;
                        t->long1_list[ t->tail_pos ]  = 1;
                        t->long2_list[ t->tail_pos ]  = 0;
                        t->tail_pos++;
                        if ( t->tail_pos >= 31 )
                           t->tail_pos = 0;
                      
                        // Old way. Delete it!
                        //t->window = Window;
                        //t->msg = MSG_MOUSEKEYUP;
                        //t->long1 = 1;
                        //t->long2 = 0;
                        //t->newmessageFlag = 1;
                    
                        //� up, mas se estamos carregando ent�o � drop.
                        //if ( PS2MouseEvent.drag_status == 1 )
                        //{
                            //t->window = Window;
                            //t->msg = MSG_MOUSE_DROP;
                            //t->long1 = 1;
                            //t->long2 = 0;
                            //t->newmessageFlag = 1;
                        //}
						// N�o estamos mais carregando um objeto.
						PS2MouseEvent.drag_status = 0;
                    }

                    //old_buttom_1 = 0;
                    PS2MouseEvent.old_buttom_1 = PS2MouseEvent.buttom_1;
                };
            }; 

       
       
			// >> BOT�O 2 ==================
			// Igual ao estado anterior
            if ( PS2MouseEvent.buttom_2 == PS2MouseEvent.old_buttom_2 )
            {
				//...

			// Diferente do estado anterior.
            }else{

				// down - O bot�o 2 foi pressionado.
                if ( PS2MouseEvent.buttom_2 == 1 )
                {

					//clicou
                    if ( PS2MouseEvent.old_buttom_2 == 0 )
                    {
						// flag: um bot�o foi pressionado.
						PS2MouseEvent.pressed = 1;
						
						// houve altera��o no estado do bot�o 2 e estamos em cima de uma janela.
                        if ( (void *) Window != NULL )
                        {
							//pegamos o total tick
							PS2MouseEvent.current_totalticks = (unsigned long) get_systime_totalticks();
                            PS2MouseEvent.delta_totalticks = (PS2MouseEvent.current_totalticks - PS2MouseEvent.last_totalticks); 
                            //printf ( "x=%d l=%d d=%d \n",
                               //current_totalticks, last_totalticks, delta_totalticks ); 
                               //refresh_screen();
                            PS2MouseEvent.last_totalticks = PS2MouseEvent.current_totalticks;
                            
                            
                            // New way. Use this one!
                            t->window_list[ t->tail_pos ] = Window;
                            t->msg_list[ t->tail_pos ] = MSG_MOUSEKEYDOWN;
                            if (PS2MouseEvent.delta_totalticks < 1000){
                                t->msg_list[ t->tail_pos ] = MSG_MOUSE_DOUBLECLICKED; 
                                PS2MouseEvent.delta_totalticks=8000; // delta inv�lido.
                            }
                            t->long1_list[ t->tail_pos ] = 2;
                            t->long2_list[ t->tail_pos ] = 0;
                            t->tail_pos++;
                            if ( t->tail_pos >= 31 )
                                t->tail_pos = 0;

							
							//estamos carregando o objeto
							//PS2MouseEvent.drag_status = 1;
                        }
                        //else: // houve altera��o no estado do bot�o 2 mas n�o estamos em cima de uma janela.

						// atualiza o estado anterior.
                        //old_buttom_2 = 1;
                        PS2MouseEvent.old_buttom_2 = PS2MouseEvent.buttom_2;
                    }

				// up - O bot�o 2 foi liberado.
                }else{

				    // flag: um bot�o foi liberado.
				    PS2MouseEvent.pressed = 0;
				    
                   if ( (void *) Window != NULL )
                   {

                        // New way. Use this one!
                        t->window_list[ t->tail_pos ] = Window;
                        t->msg_list[ t->tail_pos ]    = MSG_MOUSEKEYUP;
                        t->long1_list[ t->tail_pos ]  = 2;
                        t->long2_list[ t->tail_pos ]  = 0;
                        t->tail_pos++;
                        if ( t->tail_pos >= 31 )
                           t->tail_pos = 0;


						// N�o estamos mais carregando um objeto.
						PS2MouseEvent.drag_status = 0;                        
                    }

                    //old_buttom_2 = 0;
                    PS2MouseEvent.old_buttom_2 = PS2MouseEvent.buttom_2;
                }
            }; 
            


			// >> BOT�O 3 ==================
			// Igual ao estado anterior
            if ( PS2MouseEvent.buttom_3 == PS2MouseEvent.old_buttom_3 )
            {
				//...

			// Diferente do estado anterior.
            }else{

				// down - O bot�o 3 foi pressionado.
                if ( PS2MouseEvent.buttom_3 == 1 )
                {
					//clicou
                    if ( PS2MouseEvent.old_buttom_3 == 0 )
                    {
						// flag: um bot�o foi pressionado.
						PS2MouseEvent.pressed = 1;
						
                        // houve altera��o no estado do bot�o 3 e estamos em cima de uma janela.
                        if ( (void *) Window != NULL )
                        {
							//pegamos o total tick
							PS2MouseEvent.current_totalticks = (unsigned long) get_systime_totalticks();
                            PS2MouseEvent.delta_totalticks = (PS2MouseEvent.current_totalticks - PS2MouseEvent.last_totalticks); 
                            //printf ( "x=%d l=%d d=%d \n",
                               //current_totalticks, last_totalticks, delta_totalticks ); 
                               //refresh_screen();
                            PS2MouseEvent.last_totalticks = PS2MouseEvent.current_totalticks;
                            

                            // New way. Use this one!
                            t->window_list[ t->tail_pos ] = Window;
                            t->msg_list[ t->tail_pos ]    = MSG_MOUSEKEYDOWN;
                            if (PS2MouseEvent.delta_totalticks < 1000){
                                t->msg_list[ t->tail_pos ] = MSG_MOUSE_DOUBLECLICKED; 
                                PS2MouseEvent.delta_totalticks=8000; // delta inv�lido.
                            }
                            t->long1_list[ t->tail_pos ] = 3;
                            t->long2_list[ t->tail_pos ] = 0;
                            t->tail_pos++;
                            if ( t->tail_pos >= 31 ){
                                t->tail_pos = 0;
                            }


							//estamos carregando o objeto
							//PS2MouseEvent.drag_status = 1;
                        }
                        //else: // houve altera��o no estado do bot�o 1 mas n�o estamos em cima de uma janela.

						// Atualiza o estado anterior.
                        //old_buttom_3 = 1;
                        PS2MouseEvent.old_buttom_3 = PS2MouseEvent.buttom_3;
                    }

				// up - O bot�o 3 foi liberado.
                }else{

				    // flag: um bot�o foi liberado.
				    PS2MouseEvent.pressed = 0;
				    
                    if ( (void *) Window != NULL )
                    {


                        // New way. Use this one!
                        t->window_list[ t->tail_pos ] = Window;
                        t->msg_list[ t->tail_pos ]    = MSG_MOUSEKEYUP;
                        t->long1_list[ t->tail_pos ]  = 3;
                        t->long2_list[ t->tail_pos ]  = 0;
                        t->tail_pos++;
                        if ( t->tail_pos >= 31 ){
                           t->tail_pos = 0;
                        }


						// N�o estamos mais carregando um objeto.
						PS2MouseEvent.drag_status = 0;   
                    }

                    //old_buttom_3 = 0;
                    PS2MouseEvent.old_buttom_3 = PS2MouseEvent.buttom_3;
                }
            }; 

			// A��o conclu�da.
            PS2MouseEvent.button_action = FALSE;
            return 0;
        };



        //===============================================
        // *** Se N�O ouve altera��o no estado dos bot�es, ent�o apenas 
        // enviaremos a mensagem de movimento do mouse e sinalizamos 
        // qual � a janela que o mouse est� em cima.
        // N�o houve altera��o no estado dos bot�es, mas o mouse
        // pode estar se movendo com o bot�o pressionado.
        //a n�o ser que quando pressionamos o bot�o ele envie v�rias
        //interrup��es, igual no teclado.

        if ( PS2MouseEvent.button_action == FALSE )
        {
			//printf ("[ No Action \n");

			// #importante
			// Lembrando que estamos dentro de uma janela ...
			// por isso a mensagem � over e n�o move.

			//Obs: Se a janela for a mesma que capturou o mouse,
			//ent�o n�o precisamos reenviar a mensagem.
			//Mas se o mouse estiver em cima de uma janela diferente da 
			//que ele estava anteriormente, ent�o precisamos enviar uma 
			//mensagem pra essa nova janela.

			//#bugbug:
			//estamos acessando a estrutura, mas precisamos antes saber se ela � v�lida.

            //estamos em cima de uma janela e n�o houve altera��o no estado dos bot�es
            if ( (void *) Window != NULL )
            {
				//estamos em cima da janela que est�vamos antes.
                //ent�o estamos apenas se movendo
				if ( Window->id == mouseover_window )
				{

                    // New way. Use this one!
                    t->window_list[ t->tail_pos ] = Window;
                    t->msg_list[ t->tail_pos ]    = MSG_MOUSEMOVE;
                    t->long1_list[ t->tail_pos ]  = 0;
                    t->long2_list[ t->tail_pos ]  = 0;
                    t->tail_pos++;
                    if ( t->tail_pos >= 31 ){
                        t->tail_pos = 0;
                    }

                    
                    if ( Window->type == WT_EDITBOX )
                    {
                        ps2mouse_change_and_show_pointer_bmp(6); //cursor
                    }
                    if ( PS2MouseEvent.drag_status == 1 )
                    {
                        // New way. Use this one!
                        t->window_list[ t->tail_pos ] = Window;
                        t->msg_list[ t->tail_pos ]    = MSG_MOUSE_DRAG;
                        t->long1_list[ t->tail_pos ]  = 0;
                        t->long2_list[ t->tail_pos ]  = 0;
                        t->tail_pos++;
                        if ( t->tail_pos >= 31 ){
                            t->tail_pos = 0;
                        }
                    }
                }

                // N�o estamos em cima da janela que est�vamos antes.
                // Ent�o estamos em cima de outra janela.
                // OU seja, um mouse over novo.
                // devemos enviar mensagem de mouse over somente nessa
                //situa��o.
                if ( Window->id != mouseover_window )
                {
					//Temos ent�o uma nova mouse over.
                    mouseover_window = Window->id;
                   
                   
                    // New way. Use this one!
                    t->window_list[ t->tail_pos ] = (struct window_d *) windowList[mouseover_window];
                    t->msg_list[ t->tail_pos ]    = MSG_MOUSEOVER;
                    t->long1_list[ t->tail_pos ]  = 0;
                    t->long2_list[ t->tail_pos ]  = 0;
                    t->tail_pos++;
                    if ( t->tail_pos >= 31 ){
                        t->tail_pos = 0;
                    }

                   //#bugbug
                   //aqui entraria a fila de mensagens.
                   //onde dir�amos que tamb�m sa�mos de uma janela.
                   //MSG_MOUSEEXITED;

					//#importante:
					//flag que ativa o refresh do mouseover somente uma vez.
                    flagRefreshMouseOver = 1;

					// J� que entramos em uma nova janela, vamos mostra isso.

					//bot�o.
					//#provis�rio ...
					//Isso � um sinalizador quando mouse passa por cima.
					//#test: Vamos tentar modificar as caracter�sticas do bot�o.

                    //#ok isso funciona, vamos deixar oapp fazer isso.
                    //faremos isso somente para os bot�es do sistema.  
                    //if ( Window->isButton == 1 )
                    //{
						//isso funciona.
                        //bmpDisplayCursorBMP ( fileIconBuffer, Window->left, Window->top );
						//#test
                        //update_button ( (struct button_d *) Window->button,
                            //(unsigned char *) Window->button->string,
                            //(int) Window->button->style,
                            //(int) BS_HOVER,
                            //(int) Window->button->type,
                            //(unsigned long) Window->button->x, 
                            //(unsigned long) Window->button->y, 
                            //(unsigned long) Window->button->width, 
                            //(unsigned long) Window->button->height, 
                            //(unsigned long) Window->button->color );
						//redraw_button ( (struct button_d *) Window->button );
						//show_window_rect (Window);
					//};
					

				    //n�o bot�o.
				    //if ( Window->isButton == 0 )
				    //{
				    //    bmpDisplayCursorBMP ( folderIconBuffer, Window->left, Window->top );
				    //};


				    //nova mouse over
                    //mouseover_window = Window->id;

					//#importante:
					//flag que ativa o refresh do mouseover somente uma vez.
                    //flagRefreshMouseOver = 1;
                    

					// Agora enviamos uma mensagem pra a nova janela que 
					// o mouse est� passando por cima.
                    //#todo: reagir a isso l� nos apps.
                    
                    //t->window = Window;
                    //t->msg = MSG_MOUSEOVER;
                    //t->long1 = 0;
                    //t->long2 = 0;
                    //t->newmessageFlag = 1;
                    
                    //return 0;

                // � mouse over window.
                // n�o estamos em cima de uma janela e n�o houve altera��o no estado dos bot�es
                }
                
            };

			// A��o conclu�da.
			// Para o caso de um valor incostante na flag.
            PS2MouseEvent.button_action = FALSE;
            return 0;
        };
    };
    //--
    
    //fail!
    return -1;
}




/*
 *********************************************
 * kgwm_mouse_dialog:
 * 
 *     O system_procedure redireciona para c� as mensagens de mouse.
 *     Lembrando que o aplicativo em ring3 chamou o system_procedure
 *  quando invocou o defered procedure. (defered/default)
 */

unsigned long
kgwm_mouse_dialog ( 
    struct window_d *window,
    int msg,
    unsigned long long1,
    unsigned long long2 )
{

    //#todo: No more mouse support for this window server.
    debug_print("kgwm_mouse_dialog: [DEPRECATED]\n");
    
    // #todo
    // Checar validade da estrutura.

    if ( window->isControl == TRUE ){
        return (unsigned long) kgwm_window_control_dialog ( 
                                   window, msg, long1, long2 ); 
    } 

    return 0;
}



unsigned long
kgwm_window_control_dialog ( 
    struct window_d *window,
    int msg,
    unsigned long long1,
    unsigned long long2 )
{

    // #todo: Check pointer.


    //#bugbug
    if ( window->isControl != TRUE ){
        panic ("kgwm_window_control_dialog: isControl ?\n");
    }


    switch (msg){

        // mouse button down
        case 30:
			//qual bot�o do mouse?
			switch (long1)
			{
				//bot�o 1	
				case 1:
				    // Se esse controle � um bot�o.
				    if ( window->isButton == 1 )
				    {
						button_down ( window );

			            //#debug
			            //printf ("kgwm_window_control_dialog: mouse keydown, window name %s \n",
			             //   window->name ); 
			            //refresh_screen();
			            return 0;
					}
				    break;
				    
			};
				
		// mouse button up
		case 31:
		    switch (long1)
		    {
				case 1:
				    // Se esse controle � um bot�o.
				    if ( window->isButton == 1 )
				    {
						button_up ( window );

			        //#debug
			        //printf ("kgwm_window_control_dialog: mouse keyup, window name %s \n",
			            //window->name ); 
			        //refresh_screen();
			        
                    if ( window->isMinimize == 1 )
                    {
                        window->control->ke_window = window; //afeta esse bot�o
                        window->control->ke_msg    = MSG_HIDE;
                        window->control->ke_long1  = 0;
                        window->control->ke_long2  = 0;

                        window->control->ke_newmessageFlag = TRUE;
                    }
                    //if ( window->isRestore == 1 ){} //#todo: Criar esse elemento na struct
                    if ( window->isMaximize == 1 )
                    {
                        window->control->ke_window = window; //afeta esse bot�o
                        window->control->ke_msg    = MSG_MAXIMIZE;
                        window->control->ke_long1  = 0;
                        window->control->ke_long2  = 0;

                        window->control->ke_newmessageFlag = TRUE;
                    }
                    if ( window->isClose == 1 )
                    {
                        window->control->ke_window = window; //afeta esse bot�o
                        window->control->ke_msg    = MSG_CLOSE;
                        window->control->ke_long1  = 0;
                        window->control->ke_long2  = 0;

                        window->control->ke_newmessageFlag = TRUE;
                    }

                    return 0;
                }
                break;
            };
            break;
    };

    return 0;
}


// #todo
// Send a message to the control thread of a given pid.
/*
void __kgwm_pidSendMessage ( pid_t pid, int message );
void __kgwm_pidSendMessage ( pid_t pid, int message )
{}
*/


// Send a message to the Init process.
// Only one parameter.
// Remember, the input process's control thread 
// has a defined input model.

void __kgwm_SendMessageToInitProcess ( int message )
{
    struct thread_d  *t;
    int tid = -1;

    thread_type_t InputModel;


    if (message < 0){
        debug_print("__kgwm_SendMessageToInitProcess: [FAIL] message\n");
        return;
    }

    // #todo
    // Check validations.


    // process pointer
    if ( (void*) InitProcess == NULL ){
        panic ("__kgwm_SendMessageToInitProcess: InitProcess\n");
    }

    if ( InitProcess->used != TRUE || InitProcess->magic != 1234 )
    {
        panic ("__kgwm_SendMessageToInitProcess: InitProcess validation\n");
    }

    // thread pointer
    t = (struct thread_d  *) InitProcess->control;

    if ( (void*) t == NULL ){
        panic ("__kgwm_SendMessageToInitProcess: t\n");
    }

    if ( t->used != TRUE || t->magic != 1234 )
    {
        panic ("__kgwm_SendMessageToInitProcess: t validation\n");
    }

    // Get the input model of this thread.
    InputModel = t->input_model;

    // #todo
    // Check input model.

    // Get tid.

    tid = (int) t->tid;

    // #todo: Check overflow

    if (tid<0){
        panic("__kgwm_SendMessageToInitProcess: [ERROR] tid\n");
    }

    // Calling the init process.
    // keyboard events only

    // IN: tid, window, message code, ascii code, raw byte.

    kgws_send_to_tid (
        (int) tid,
        (struct window_d *) 0,
        (int)               message,
        (unsigned long) 12, (unsigned long) 34 );
}




/*
 ******************************************************************* 
 * __kgwm_ps2kbd_procedure:
 * 
 *       Some combinations with control + F1~F12
 */

// Local function.

// Called by kgws_event_dialog in kgws.c
// Called by si_send_longmessage_to_ws and si_send_message_to_ws
// in siws.c 
// #bugbug: We don't wanna call the window server. Not now.

// #important:
// Isso garante que o usu�rio sempre podera alterar o foco
// entre as janelas do kgws usando o teclado, pois essa rotina
// � independente da thread que est� em foreground.

// #todo
// Talvez a gente possa usar algo semelhando quando o window
// server estiver ativo. Mas possivelmente precisaremos 
// usar outra rotina e n�o essa. Pois lidaremos com uma estrutura
// de janela diferente, que esta localizada em ring3.

// From Windows:
// Because the mouse, like the keyboard, 
// must be shared among all the different threads, the OS 
// must not allow a single thread to monopolize the mouse cursor 
// by altering its shape or confining it to a small area of the screen.

// #todo
// This functions is the moment to check the current input model,
// and take a decision. It will help us to compose the event message.
// It is because each environment uses its own event format.

unsigned long 
__kgwm_ps2kbd_procedure ( 
    struct window_d *window, 
    int msg, 
    unsigned long long1, 
    unsigned long long2 )
{

    // #test
    // Testando uma rotina de pintura que usa escape sequence.
    // Queremos que ela funcione na m�quina real.
    // Vamos testar os ponteiros.


    // #todo
    // We need the structure of the current thread.
    // This way we have the current input model for this thread
    // and we can compose an event for this environment.

    // struct thread_d  *t;


    char buffer[128];
    sprintf (buffer,"My super \x1b[8C string!!\n"); 

    int Status = -1;


    if (msg<0){
        return 0;
    }

    switch (msg){


        case MSG_KEYDOWN:
            switch (long1){
            case VK_TAB: printf("TAB\n"); refresh_screen(); break;
            };
            break;

        // Pressionadas: teclas de fun�ao
        case MSG_SYSKEYDOWN: 
            switch (long1){

                case VK_F1:
                    if (ctrl_status == TRUE){
                        powertrio_select_client(0);
                    }
                    if (alt_status == 1){
                        printf ("__kgwm_ps2kbd_procedure: alt + f1\n");
                        refresh_screen();
                    }
                    if (shift_status == 1){
                        jobcontrol_switch_console(0);
                    }
                    return 0;
                    break;

                case VK_F2:
                    if (ctrl_status == TRUE){
                         powertrio_select_client(1);
                    }
                    if (alt_status == 1){
                        printf ("__kgwm_ps2kbd_procedure: alt + f2\n");
                        refresh_screen();
                    }
                    if (shift_status == 1){
                        jobcontrol_switch_console(1);
                    }
                    return 0;
                    break;

                case VK_F3:
                    if (ctrl_status == TRUE){
                        powertrio_select_client(2);
                    }
                    if (alt_status == 1){
                        printf ("__kgwm_ps2kbd_procedure: alt + f3\n");
                        refresh_screen();
                    }
                    if (shift_status == 1){
                        jobcontrol_switch_console(2);
                    }
                    return 0;
                    break;

                case VK_F4:
                    if (ctrl_status == TRUE){
                        powertrio_next();
                    }
                    if (alt_status == 1){
                        printf ("__kgwm_ps2kbd_procedure: alt + f4\n");
                        refresh_screen();
                    }
                    if (shift_status == 1){
                        jobcontrol_switch_console(3);
                    }
                    return 0;
                    break;


                // Reboot
                case VK_F5:
                    if (ctrl_status == TRUE){
                        powertrio_select_client(0);
                        //reboot();
                    }
                    if (alt_status == TRUE){
                        printf ("__kgwm_ps2kbd_procedure: alt + f5\n");
                        refresh_screen();
                    }
                    if (shift_status == TRUE){
                        kgwm_next();
                    }
                    return 0;
                    break;

                // Send a message to the Init process.
                // 9216 - Launch the redpill application
                case VK_F6:
                    if (ctrl_status == TRUE){
                        powertrio_select_client(1);
                        // #todo: 
                        // shutdown. Only the ring3 applications
                        // can shutdown via qemu for now. 
                        //__kgwm_SendMessageToInitProcess(9216); 
                        return 0; 
                    }
                    if (alt_status == TRUE){
                        printf ("__kgwm_ps2kbd_procedure: alt + f6\n");
                        refresh_screen();
                    }
                    if (shift_status == TRUE){
                        kgwm_next();
                    }
                    return 0;
                    break;

                // Test 1.
                case VK_F7:
                    if (ctrl_status == TRUE){
                        powertrio_select_client(2);
                       // Send message to init process to launch gdeshell.
                        //__kgwm_SendMessageToInitProcess(9217);
                    }
                    if (alt_status == TRUE){
                        printf ("__kgwm_ps2kbd_procedure: alt + f7\n");
                        refresh_screen();
                    }
                    if (shift_status == TRUE){
                        kgwm_next();
                    }
                    return 0;
                    break;

                // Test 2.
                case VK_F8:
                    if (ctrl_status == TRUE){
                        powertrio_next();
                        // Send message to init process to launch the launcher.
                        //__kgwm_SendMessageToInitProcess(9216); 
                        //__kgwm_SendMessageToInitProcess(9218);  // launch sysmon
                    }
                    if (alt_status == TRUE){
                        printf ("__kgwm_ps2kbd_procedure: alt + f8\n");
                        refresh_screen();
                    }
                    if (shift_status == TRUE){
                        kgwm_next();
                    }
                    return 0;
                    break;

                case VK_F9:
                    if (ctrl_status == TRUE){
                        powertrio_select_client(0);
                    }
                    if (alt_status == TRUE){
                        printf ("__kgwm_ps2kbd_procedure: alt + f9\n");
                        refresh_screen();
                    }
                    if (shift_status == TRUE){
                        // #goal
                        // This is a emergency keycombination.
                        // We can't call another process and 
                        // we want to reboot the machine.
                        sys_reboot();
                        //__kgwm_SendMessageToInitProcess(9216);  //reboot
                    }
                    return 0;
                    break;

                case VK_F10:
                    if (ctrl_status == TRUE){
                        powertrio_select_client(1);
                    }
                    if (alt_status == TRUE){
                        printf ("__kgwm_ps2kbd_procedure: alt + f10\n");
                        refresh_screen();
                    }
                    if (shift_status == TRUE){
                        __kgwm_SendMessageToInitProcess(9217);  //gdeshell
                    }
                    return 0;
                    break;

                case VK_F11:
                    if (ctrl_status == TRUE){
                        powertrio_select_client(2);
                    }
                    if (alt_status == TRUE){
                        printf ("__kgwm_ps2kbd_procedure: alt + f11\n");
                        refresh_screen();
                    }
                    if (shift_status == TRUE){
                       // #bugbug: Something is wrong with this routine.
                       __kgwm_SendMessageToInitProcess(9218);  // redpill application
                    }
                    return 0;
                    break;

                case VK_F12:
                    if (ctrl_status == TRUE){
                        powertrio_next();
                    }
                    if (alt_status == TRUE){
                        printf ("__kgwm_ps2kbd_procedure: alt + f12\n");
                        refresh_screen();
                    }
                    if (shift_status == TRUE){
                        __kgwm_SendMessageToInitProcess(9219);  // sysmon
                    }
                    return 0;
                    break;

                default:
                    // nothing
                    return 0;
            
            }
    };

    refresh_screen();
    return 0;
}




int init_gramado (void)
{
    debug_print ("init_gramado: [?? fixme]\n");
    return 0;
}


//
// End.
//

