/* 
#importante:

>> o frame pertence � uma janela.
>> caption bar esta dentro de uma janela e chamado pela rotina de draw frame..

-----
primeiro cria-se uma janela...
depois cria-se um frame para ela,
quando for criar o frame ent�o a rotina chama a fun��o que cria 
a caption bar.
 */

/*
 * File: createw.c
 *
 * Descri��o: 
 *     Gerencia os m�todos de cria��o de molduras de janelas e de janelas simples.
 *     Rotina principal de cria��o de janelas.
 *     Faz parte do m�dulo Window Manager do tipo MB.
 *
 * Obs: Os m�todos de cria��o de moldura de janelas podem coexistirem e ser�o
 *      gerenciados por esse m�dulo.
 *
 * *Importante:
 * Um dos m�todos inclui v�rias chamadas ao sistema, sendo que a promeira 
 * chamada ser� uma uma inicializa��o da rotina de constru��o da moldura. Essa
 * inicializa��o deve garantir exclusividade � um processo, para que outro processo
 * n�o interrompa a cria��o da moldura enquanto a rotina em user mode estiver
 * enviando par�metros para o kernel.
 *
 *
 * @todo: 
 * Toda vez que criarmos uma estrutura de janela devemos alocar mem�ria para 
 * o seu buffer dedicado de acordo com as dimens�es da janela e registrar o 
 * ponteiro para o buffer na estrutura de janela.
 * Uma das primeiras coisa a fazer � criar o buffer dedicado, que ser�
 * passado para as rotinas de pintura, para a pintura ser feita no buffer
 * dedicado. 
 *
 * IMPORTANTE: De toda forma, se a rotina de pintura, n�o pintar a janela no 
 * seu buffer dedicado, ela pintar� no buffer default, que � o backbuffer e
 * perceberemos o erro facilmente, para corrigi-lo. Ent�o, na hora de passar o 
 * conte�do dos buffers para o LFB deve-se seguir a ordem de uma lista linkada 
 * cujo primeiro ponteiro � o Backbuffer. Por observa��o poderemos ver se
 * ela � flutuante ou n�o. Se � flutuante, � porque foi pintada em buffer dedicado,
 * se n�o � flutuante � porque foi pintada no buffer default, que � o Backbuffer.
 *
 * History:
 *     2015 - Created by Fred Nora.
 *     2016 - 2018 New features.
 *     ...
 */

 
/*
 Importante:
    
    Sobre janelas e frames:
    =======================

    Uma janela na verdade � um container, na forma de ret�ngulo, que
abrigar� elemntos gr�ficos como bot�es, textos, etiquetas  etc...
    Uma Janela pode ou n�o conter uma moldura, chamada de Frame,
que � formada pela barra de t�tulos, pela borda, pela sombra...
    Uma janela em modo full screen n�o cont�m frame.

   Sobre a fun��o CreateWindow(.):
   ==============================
   O prop�sito dessa fun��o � a cria��o de v�rios tipos de janelas
   que ser�o utilizadas pelas aplica��es. A fun��o pode criar uma janela
   que contenha frame ou n�o.
   No momento da cria��o da janela de um aplicativo, a fun��o CreateWindow
   chamar� o procedimento de janela do aplicativo para ver as solicita��es
   de cria��o de elementos extras, que devem ser inclu�dos na janela.

 */

/*
    *Importante:
	 Poder� haver mais de um m�todo de cria��o de uma frame window.   
 */
 

#include <kernel.h>


//Herdadas do Boot Loader.
// De onde vem isso ?? head.s
// @todo: Devemos chamar o m�dulo hal para obtermos esses valores.
//depois salvamos em vari�veis internas usadas pela gui.

extern unsigned long SavedBootBlock;
extern unsigned long SavedLFB;
extern unsigned long SavedX;
extern unsigned long SavedY;
extern unsigned long SavedBPP; 




// ?? n
int DrawIcon ( struct window_d *window, int n )
{
    debug_print ("DrawIcon: [FIXME] n \n");

    // #todo: 
    // Checar validades.

    if ( (void*) window == NULL )
        return -1;

    // n = seleciona um icon de uma lista.
    // switch()
    
    bmpDisplayBMP ( 
        shared_buffer_app_icon, 
        (window->left +8), (window->top +8) );


    return 0;
}


// #importante
// Criaremos a barra de t�tulos depois que a janela estiver pronta.
// #todo: change string to 'const char *'

int 
DrawTitleBar ( 
    struct window_d *window,
    unsigned long x,
    unsigned long y,
    unsigned long width,
    unsigned long height,
    int style,
    char *string )         
{

    unsigned long __tmp_color;


    // podemos desenhar a string e os bot�es de controle.
    // Isso poder� ser chamado pelo wm.

    // #todo: 
    // Checar validades.

    if ( (void*) window == NULL )
        return -1;

    // barra de t�tulos;   
    //todo: usar o esquema de cores.      

    if (window->focus == 1)
    { __tmp_color = xCOLOR_GRAY1; }      // mais escuro
    if (window->focus == 0)
    { __tmp_color = window->bg_color; }  // escolhida pelo aplicativo


    // ret�ngulo da barra de t�tulos.
    drawDataRectangle ( 
        (window->left +2), (window->top +2), 
        (window->width -4), 30, __tmp_color );        

    // String
    draw_string ( 
        (window->left +16 +8 +8), ( window->top + (32/3) ), 
        xCOLOR_GRAY7, window->name );  

     return 0;
}



// #importante:
// essa rotina ser� chamada depois que criarmos uma janela b�sica
// mas s� para alguns tipos de janelas
// pois nem todos os tipos precisam de um frame.
// ou ainda, cada tipo de janela tem um frame diferente ...
// por exemplo, podemos considerar que um checkbox tem um tipo de frame.
// toda janela criada pode ter um frame.
// >> durante a rotina de cria��o do frame para uma janela que ja existe
// podemos chamar a rotina de cria��o da caption bar, que vai criar os
// bot�es de controle ... mas nem toda janela que tem frame precisa
// de uma caption bar.

//estilo do frame.
//dependendo do estilo do frame, podemos ou nao criar a caption bar.
// por exemplo: uma editbox tem um frame mas n�o tem uma caption bar;


int 
DrawFrame ( 
    struct window_d *window,
    unsigned long x,
    unsigned long y,
    unsigned long width,
    unsigned long height,
    int style )          //estilo do frame.
{

    unsigned long border_color=0;
    unsigned long border_size=0;
    unsigned long __tmp_color=0;


    // #todo
    // desenhar o frame e depois desenhar a barra de t�tulos
    // caso esse estilo de frame precise de uma barra.

    // #todo
    // Chech pointer validation.

    if ( (void*) window == NULL )
        return -1;

    // ============
    // editbox
    // EDITBOX N�O PRECISA DE BARRA DE T�TULOS.
    // MAS PRECISA DE FRAME ... QUE SER�O AS BORDAS.

    if ( window->type == WT_EDITBOX )
    {
        // Se tiver o foco.
        if ( window->focus == TRUE ){
            border_color = COLOR_BLUE;
            border_size = 2;
        }else{
            border_color = COLOR_PINK; //COLOR_INACTIVEBORDER;
            border_size = 1;
        };
            
        drawDataRectangle ( 
            window->left, window->top, 
            window->width, border_size, 
            border_color );

        drawDataRectangle ( 
            window->left, window->top, 
            border_size, window->height, 
            border_color );

        // board2, borda direita e baixo.
        drawDataRectangle ( 
            (window->left + window->width - border_size), window->top, 
            border_size, window->height, 
            border_color );

        drawDataRectangle ( 
            window->left, (window->top + window->height - border_size), 
            window->width, border_size, 
            border_color );

        return 0;
    }

    // ============
    // overlapped

    if ( window->type == WT_OVERLAPPED)
    {
        // Se tiver o foco.
        if ( window->focus == TRUE ){
            border_color = COLOR_BLUE;
            border_size = 3;
        }else{
            border_color = 0xF4F7FC;
            border_size = 2;
        };
            
        drawDataRectangle ( 
            window->left, window->top, 
            window->width, border_size, 
            border_color );

        drawDataRectangle ( 
            window->left, window->top, 
            border_size, window->height, 
            border_color );

        // board2, borda direita e baixo.
        drawDataRectangle ( 
            (window->left + window->width - border_size), window->top, 
            border_size, window->height, 
            border_color );

        drawDataRectangle ( 
            window->left, (window->top + window->height - border_size), 
            window->width, border_size, 
            border_color );

       // title bar and string
       // #todo: Isso � trabalho do wm
       //DrawTitleBar ( (struct window_d *) window, 
       //     x, y,
       //     width, height, 
       //     1,
       //     (char *) window->name );  //style


        // Icon.
        // #todo: Isso � trabalho do wm
        //DrawIcon ( window, 1 );
            
        return 0;
    }

    // ============
    // checkbox
    // quadradinho de verifica��o.
    // quadradinho pequeno com borda grossa.

    if ( window->type == WT_CHECKBOX )
    {
        // Se tiver o foco.
        if ( window->focus == TRUE ){
            border_color = xCOLOR_GRAY1;
            border_size = 8;
        }else{
            border_color = xCOLOR_GRAY2;
            border_size = 4;
        };
  
        drawDataRectangle( 
            window->left, window->top, 
            window->width, border_size, 
            border_color );

        drawDataRectangle( 
            window->left, window->top, 
            border_size, window->height, 
            border_color );

        // board2, borda direita e baixo.
        drawDataRectangle( 
            (window->left + window->width - border_size), window->top, 
            border_size, window->height, 
            border_color );

        drawDataRectangle ( 
            window->left, (window->top + window->height - border_size), 
            window->width, border_size, 
            border_color );

        return 0;
    }

    //??

    return 1;
    //return -1;
}




/*
 ****************************************************************
 * CreateWindow: 
 *     Worker function called by kgws_create_window();
 * 
 */


//1  - Tipo de janela (popup,normal,...) 
//2  - Estado da janela. (poderia ter v�rios bits ??)
//3  - (min, max ...)
//4  - T�tulo.
//5  - Deslocamento em rela��o �s margens do Desktop.
//6  - Deslocamento em rela��o �s margens do Desktop.
//7  - Largura da janela.
//8  - Altura da janela.
//9  - Endere�o da estrutura da janela m�e.
//10 - desktop ID. (# Para levarmos em conta as caracter�sticas de cada desktop).
//11 - Client Area Color.
//12 - color (bg) (para janela simples)


// # importante
// Essa rotina cria janelas simples, sem molduras, dos seguintes tipos:
// WT_SIMPLE, WT_POPUP, WT_BUTTON, WT_STATUSBAR e WT_ICON

// #todo
// use 'const char *' for name

// #todo
// Use window->locked to lock the parent when the created window 
// is a child window.

void *CreateWindow ( 
    unsigned long type, 
    unsigned long status, 
    unsigned long view, 
    char *windowname, 
    unsigned long x, 
    unsigned long y, 
    unsigned long width, 
    unsigned long height, 
    struct window_d *pWindow, 
    int desktop_id, 
    unsigned long clientcolor, 
    unsigned long color ) 
{

	// #todo: 
	// O argumento style est� faltando.
	// cada tipo de tanela poder� ter v�rios estilos.
	// Obs: Podemos ir usando apenas um estilo padr�o por enquanto.
	
	// (min, max ...).
	//int View;

    int Maximized = FALSE;
    int Minimized = FALSE;

    // Bars.
    int TitleBar  = FALSE;
    int MenuBar   = FALSE;
    int ToolBar   = FALSE;
    int StatusBar = FALSE;
    // ...

    // Title bar buttons. 
    // [v][^][X] 

    int MinimizeButton = FALSE;
    int MaximizeButton = FALSE;
    int CloseButton    = FALSE;
    // ...

    // Flags.
    // Parts of a window.
    int Background    = FALSE;
    int ClientArea    = FALSE;
    int Shadow        = FALSE;
    int ButtonDown    = FALSE;  // ??
    int ButtonUp      = FALSE;  // ??
    int ButtonSysMenu = FALSE;  // system menu na barra de t�tulos.
    int Border        = FALSE;  // New !!! usado no edit box.
    // ...

	// Desktop support.
    int ParentWindowDesktopId;    //Id do desktop da parent window.
    int WindowDesktopId;          //Id do desktop da janela a ser criada.

    // Controls: minimize, maximize and close.
    struct window_d *windowButton1;
    struct window_d *windowButton2;
    struct window_d *windowButton3;

    // Scroll bar buttons
	struct window_d *windowButton4;
	struct window_d *windowButton5;
	struct window_d *windowButton6;

    //
    // Core components.
    //

    // The window itself.
    // The parent window. Som windows really needs a parent. 
    // Desktop.
    // The client rectangle of the window.

    struct window_d   *window;
    struct window_d   *Parent;
    struct desktop_d  *Desktop;
    struct rect_d     *clientRect;


	// #improvisando uma largura de borda.
	// @todo: isso deve ir para outro lugar.

    unsigned long border_size = 0;
    unsigned long border_color = 0xC0C0C0;

    unsigned long __tmp_color=0;

    // Usado para manipular lista encadeada.
    struct window_d   *tmp;

	//salvar para depois restaurar os valores originais no fim da rotina.
	//unsigned long saveLeft;
	//unsigned long saveTop;
	
	
	// Desktop:
	// @todo: Configurar desktop antes de tudo. 
	// @todo: Quando criamos uma janela temos de definir que ela
	// pertence ao desktop atual se n�o for enviado por argumento 
	// o desktop que desejamos que a janela perten�a.

	
	// O argumento onde:
	// Indica onde devemos pintar a janela. Serve para indicar as janelas 
	// principais. Ex: Se o valor do argumento for 0, indica que devemos 
	// pintar na tela screen(background) etc...


	// full screen mode ??
	// Se a janela a ser criada estiver no modo full screen, ela n�o deve ter
	// um frame, ent�o as dimens�es da janela ser�o as dimens�es do ret�ngulo
	// que forma a janela. Talvez chamado de Client Area.
	
	//
	// Parent window.
	//
	
	// Se a parent window enviada por argumento for inv�lida, 
	// ent�o usaremos a janela gui->screen. ?? 
	// Talvez o certo fosse retornar com erro.
	// ?? Qual deve ser a janela m�e ? Limites ?
	// @todo: devemos checar used e magic da janela m�e.
	// #bugbug: E quando formos criar a gui->screen, quem ser� a janela m�e ?


    // #bugbug
    // Some types really needs a parent window.
    // For example, buttons, taht do not exists by it self.
    
    // #todo
    // If we have a parent window, make sure that
    // it is in the same desktop.

    if ( (void *) pWindow == NULL ){
        Parent = (void *) gui->screen;  // #hackhack
    } else {
        Parent = (void *) pWindow;
    };

    // #todo
    // A janela que estamos criando deve entrar numa lista de filhas,
    // provavelmente usando lista encadeada.


    // Make sure that we have a desktop.
    // If not, so maybe we can use this window as root window
    // for this desktop.
 
    // Desktop id.
    if ( desktop_id < 0 ){
        debug_print ("CreateWindow: [FIXME] Invalid desktop_id\n");
    }


	// Devemos checar se a janela est� no mesmo desktop 
	// que a ajnela m�e.
	// No caso aqui, criarmos uma janela no mesmo desktop que a 
	// janela m�e.
	
	// Devemos setar uma flag que indique que essa 
	// � uma janela filha, caso seja uma. Essa flag 
	// deve ser passada via argumento @todo.

	// @todo: Checar se � uma janela filha, 
	// se for uma janela filha e n�o tiver uma janela m�e associada a ela, 
	// n�o permita e encerre a fun��o.

	
	//if(FlagChild == 1)
	//{
		//if(pWindow = NULL) return NULL;
	//}
	

	// @todo: A atualiza��o da contagem de janela deve ficar aqui,
	// mas me parece que est� em outro lugar, ou n�o tem. ainda.
	
	// @todo: Se essa n�o for uma janela filha, ent�o temos que resetar 
	// as informa��es sobre a janela m�e. porque n�o procedem.	
	
	//ms. e se essa � uma janela filha de uma janela m�e que pertence �
	//outra thread e n�o est� no desktop ??
	

    // #importante: 
    // Checando se o esquema de cores est� funcionando.

    // #todo
    // We need a helper function.

    if ( (void *) CurrentColorScheme == NULL ){
        panic ("CreateWindow: CurrentColorScheme\n");
    }else{
        if ( CurrentColorScheme->used != TRUE || 
             CurrentColorScheme->magic != 1234 )
        {
            panic ("CreateWindow: CurrentColorScheme validation\n");
        }
        // Nothing
    };


	//
	// ## New window ##
	//

//CreateStruct:

	//Alocando mem�ria para a estrutura da janela.

    window = (void *) kmalloc ( sizeof(struct window_d) );

    // #todo: 
    // Onde o Kernel colocar� essa mensagem de erro ?
    // Por enquanto no backbuffer.

    if ( (void *) window == NULL ){
        printf ("CreateWindow: [ERROR] window\n");
        refresh_screen();
        return NULL; 
    }else{

		// Obs: 
		// Conseguimos alocar mem�ria para a estrutura da janela. 
		// Agora vamos inicializar os elementos da estrutura de acordo 
		// com os par�metros passados via argumento.

        window->objectType  = ObjectTypeWindow;
        window->objectClass = ObjectClassGuiObjects;
        window->used  = TRUE;
        window->magic = 1234;

        // #importante
        // Id. 
        // A janela recebe um id somente na hora de registrar.
        // window->id = ??.

        // Name.
        // #todo: 
        // Check validation and change the parameter to 'const char *'.

        if ( (void*) windowname == NULL ){ 
            panic("CreateWindow: windowname\n"); 
        }

        if ( *windowname == 0 ){ 
            panic("CreateWindow: *windowname\n"); 
        }
        
        window->name = windowname;
        
        window->tid = current_thread;

        // Window type.
        // Tipo � unsigned long pois poder� ser um conjunto de flags.

        window->type = (unsigned long) type;


        //lista de oberlapped windows no desktop atual.
        // #todo: We need a helper function for that.
        if ( type == WT_OVERLAPPED )
        {
            if ( (void *) CurrentDesktop == NULL ){
                panic("CreateWindow: [FAIL] CurrentDesktop\n");
            }
            
            if ( CurrentDesktop->desktopUsed != TRUE ||
                 CurrentDesktop->desktopMagic != 1234 )
            {
                panic("CreateWindow: [FAIL] CurrentDesktop validation\n");
            }
            
            // put it in the list.
            
            if ( CurrentDesktop->lTail < 0 || CurrentDesktop->lTail >= 8 )
            {
                CurrentDesktop->lTail = 0;
            }
            
            // #todo: melhorar isso.
            // coloca no lugar certo.
            
            // coloca no tail
            
            CurrentDesktop->list[ CurrentDesktop->lTail  ] = (unsigned long) window;
            
            // circula tail
            CurrentDesktop->lTail++;
            if ( CurrentDesktop->lTail >= 8 )
            {
                CurrentDesktop->lTail = 0;
            }
        }

        // #todo: 
        // Criar inst�ncia.

        // #todo: 
        // Procedure support.
        // � importante definir o procedimento de janela desde j�,
        // sen�o d� problemas quando cham�-lo e ele na� estiver pronto.
        // #todo: Devemos receber esses par�metros e configurar aqui.

        //window->procedure = (unsigned long) &system_procedure;
        //window->wProcedure = NULL;  //Estrutura.

        // Modo de exibi��o

        window->view = (int) view;

        // #bugbug
        // Se n�o foi oferecido um modo de exibi��o, 
        // ent�o temos um problema.
        // ?? Talvez devamos retornar um erro. 

        // if( (int) view == VIEW_NULL ){ return NULL; };

        // status
        // ?? Qual � o status da janela, se ela � a janela ativa ou n�o.
        // ?? Devemos definir quais s�o os status poss�veis da janela.

        // active 
        if ( status == WINDOW_STATUS_ACTIVE )
        {

            if ( type == WT_OVERLAPPED ){
                active_window = (int) window->id;
            }
            
            // set_active_window(window); 
            // window->active = WINDOW_STATUS_ACTIVE;
            // window->status = (unsigned long) WINDOW_STATUS_ACTIVE;
            
            window->relationship_status = (unsigned long) WINDOW_REALATIONSHIPSTATUS_FOREGROUND; 
            window->z = 0;  //z_order_get_free_slot()
            
            // ...
        }

        // inactive
        if ( status == WINDOW_STATUS_INACTIVE )
        { 
            // window->active = WINDOW_STATUS_INACTIVE;
            // window->status = (unsigned long) WINDOW_STATUS_INACTIVE;

            window->relationship_status = (unsigned long) WINDOW_REALATIONSHIPSTATUS_BACKGROUND;
            window->z = 0;  //z_order_get_free_slot()

            // ...
        }

        // Focus = No!
        // Determinando que n�o temos o foco.

        window->focus = FALSE;

		// @todo:
		// Se for uma janela filha o posicionamento deve ser somado �s margens 
		// da �rea de cliente da janela que ser� a janela m�e.

		// #bugbug @todo 
		// Esses valores de dimens�es recebidos via argumento na verdade 
		// devem ser os valores para a janela, sem contar o frame, que 
		// inclui as bordas e a barra de t�tulos.

        // Dimens�es
        window->width  = width;
        window->height = height;

        // Margens
        window->left   = x;
        window->top    = y;
        window->right  = (unsigned long) ( window->left + window->width );
        window->bottom = (unsigned long) ( window->top  + window->height ); 

        // Deslocamentos em rela��o �s margens.
        // Os deslocamentos servem para inserir elementos na janela, 
        // como barras, bot�es e textos.

        window->x = 0;
        window->y = 0;

		// ?? saving.
		//saveLeft = window->left;
		//saveTop  = window->top;

		//
		//  color
		//

        // Background support.

        window->bg_color            = (unsigned long) color; 
        window->clientrect_bg_color = (unsigned long) clientcolor;

		// ??
		// O ret�ngulo da janela.
		//window->rcWindow = NULL;


        //
        // Cursor support
        //

        // Cursor position inside the window in chars. ??
        window->CursorX = 0;
        window->CursorY = 0;

        // Cursor position inside the window in pixels. ??
        // window->CursorXInPixels = 0;
        // window->CursorYInPixels = 0;

        window->CursorColor = 0x00333333; 

        // #todo: 
        // As outras caracter�sticas do cursor.

        //
        // cursor structure
        //
        
        window->cursor = NULL;


        // #todo: 
        // The cursor structure initialization.
 
		// Uma op��o � inicializarmos a estrutura de ponteiro depois ...
		// pois tem janela que n�o tem ponteiro. 
		// J� QUE NO MOMENTO ESTAMOS ENFRENTANDO ALGUNS TRAVAMENTOS.
		//
		//window->cursor = (void*) kmalloc( sizeof(struct cursor_d) );

		//@@todo: Criar uma fun��o: Inicializarcursor(struct cursor_d *cursor).
		//if(window->cursor != NULL)
		//{
		//    window->cursor->objectType = ObjectTypeCursor;
		//    window->cursor->objectClass = ObjectClassGuiObjects;
		//    window->cursor->x = 0;
		//    window->cursor->y = 0;
		//    window->cursor->imageBuffer = NULL;
		//    window->cursor->imagePathName = NULL;
		//    window->cursor->cursorFile = ??; //@todo: Dif�cil definir o tipo.
		//    window->cursor->cursorType = cursorTypeDefault;
		//};


		//Barras.
		//As flags que representam a presen�a de cada uma das barras
		//ser�o acionadas mais tarde, na hora da pintuda, de acordo com
		//o tipo de janela � ser pintada.

        //
        // Message support.
        //

        // Single message.
        // This method is not used for now.

        window->msg_window = (void *) window;
        window->msg        = 0;
        window->long1      = 0;
        window->long2      = 0;

        // Se tem ou n�o uma nova mensagem.
        window->newmessageFlag = FALSE;

        // Message queue.

        // Filas de mensagens.
        // S�o dessas filas que os programas em user mode
        // s�o pegar as mensagens, solicitando atrav�s de system call.
        // #todo: Talvez usar um loop pra limpar a fila.

        window->hwndList[0]  = 0;
        window->msgList[0]   = 0;
        window->long1List[0] = 0;
        window->long2List[0] = 0;

        // Offsets
        window->sendOffset = 0;
        window->receiveOffset = 0;

        //
        // buffers
        //

        // Dedicated buffer 
        // Cria um buffer dedicado de acordo com as dimens�es da janela.
        // Suspended

        window->DedicatedBuffer = NULL;
        // window->DedicatedBuffer = (void*) windowCreateDedicatedBuffer(window);


        // #todo
        // backbuffer and front buffer.
        // Why the window needs these values?
        // Check the validation.
 
        
        window->BackBuffer  = (void *) g_backbuffer_va;
        window->FrontBuffer = (void *) g_frontbuffer_va;

        // #todo
        //if ( (void*) g_backbuffer_va == NULL )
            //panic ("g_backbuffer_va");

        // #todo 
        //if ( (void*) g_frontbuffer_va == NULL )
            //panic ("g_frontbuffer_va");


        // Child window linked list.
        // Essa janela ainda n�o tem uma lista de filhas,
        // pois acabou se ser criada.
        // Mas essa janela deve entrar na lista da sua janela m�e.
        
        window->childListHead = NULL;


		// (#importante)
		// Client window support.
		// Obs: A �rea de cliente ser� um ret�ngulo e n�o uma janela.
		// Mas nda impede da estrutra gerenciar uma janela que fique 
		// em cima da �rea de cliente.
        
        window->client_window = NULL;  // window 
        window->rcClient      = NULL;  // rect

		// #importante
		// Terminal support.
		// Suporte n�o tradicional � terminais.
		// manipulando vari�veis ao inv�s de uma estrutura.

        window->terminal_used  = FALSE;
        window->terminal_magic = (int) 0;
        window->terminal_left   = (unsigned long) 0;
        window->terminal_top    = (unsigned long) 0;
        window->terminal_width  = (unsigned long) 0;
        window->terminal_height = (unsigned long) 0;
        window->terminal_tab   = FALSE;

		//Desktop support.
		//window->desktop = (void*) Desktop; //configurado anteriormente.
		//window->desktop_id = Desktop->id;  //@todo: verificar elemento.

        // # Menu support #

        window->menu_window = NULL;
        window->sysMenu     = NULL;
        window->barMenu     = NULL;
        window->defaultMenu = NULL;


        // What is the thing ?

        window->isMenu    = FALSE;
        window->isButton  = FALSE;
        window->isEditBox = FALSE;
        // ...
        
        // Selected ?
        // Caso a janela seja um �tem de menu.
        window->selected = FALSE;  

        // #??
        // text
        // Texto, caso a janela seja um �tem de menu
        // window->text = NULL; 

        //
        // Visualization support.
        //
        
        //draw
        //@todo: Cuidado com isso.
        window->draw = FALSE;
        
        //redraw
        window->redraw = FALSE;
        
        // Inicialmente presumimos que precisamos mostrar essa janela.
        window->show = TRUE; 
 
         // Continua ...

        //Abaixo, elementos referenciados com menor frequ�ncia.

        //@todo: Definir � qual desktop a janela perence.
        window->desktop = NULL;   //desktopList[current_desktop];
        
        // process and thread
        // #todo: We need these for jog control.
        window->process = NULL;  // processList[current_process];
        window->control = NULL;  // threadList[current_thread];

        window->locked = FALSE;

        //Linked list.
        // #bugbug: What is this?
        //window->linkedlist = NULL;

        // navigation
        
        // #bugbug: 
        // It is wrong.
        // It is not defined yet what will be next or prev.
        // This kind of navigation is not defined yet.

        window->prev = (void *) Parent;  //#bugbug
        
        window->next = NULL;
    };


    //Exemplos de tipos de janelas, segundo MS.	
    //Overlapped Windows
    //Pop-up Windows
    //Child Windows
    //Layered Windows
    //Message-Only Windows

	//
	// Preparando os parametros da pintura de acordo com o tipo.
	//

	// De acordo com o tipo de janela, preparamos a configura��o
	// e a pr�pria rotina create window est� pintando.
	// Por�m nesse momento o 'case' pode chamar rotinas de pintura em 
	// draw.c e retornar. 
	// CreateWindow � para criar a moldura principal ...
	// para so outros tipos de janelas podemos usar draw.c
	// pois quando chamarmos draw.c a estrutura de janela ja deve estar 
	// inicializada.
	// Rotinas grandes como pintar um barra de rolagem podem ficar em draw.c

	// #importante
	// Deveria ter uma vari�vel global indicando o tipo de 
	// design atual, se � 3D ou flat.

	// Configurando os elementos de acordo com o tipo.
	// @todo: Salvar as flags para os elementos presentes
	// na estrutura da janela.

    switch (type){

        case WT_NULL:
            return NULL; 
            break;

        // Simple window. (Sem barra de t�tulos).
        case WT_SIMPLE:
            Background = TRUE;
            window->backgroundUsed = TRUE;
            break;
 
        case WT_OVERLAPPED:
            Background = TRUE;
            window->backgroundUsed = TRUE;
            Border = TRUE;
            window->borderUsed = TRUE;
            break;


        // Popup. (um tipo de overlapped mais simples).
        case WT_POPUP:
            Shadow = TRUE; 
            window->shadowUsed = TRUE; 
            Background = TRUE;
            window->backgroundUsed = TRUE;
           //if(titulo){} TitleBar = 1;    //titulo + borda
            break;
 
        // Button.
        //Obs: existe uma rotina que desenha um bot�o.
        //quando create window for chamada para criarmos um bot�o 
        //podemos chamar essa rotina que desenha o bot�o ...
        //apenas isso.	
        //aqui nesse case podemos selecionar algumas caractrer�sticas 
        //do bot�o que s�o acionadas atrav�s das caracter�sticas 
        //da janela.
        
        // Nothing for now ...
        // Deixaremos a rotina de desenhar o bot�o 
        // fazer tudo por enquanto. 
        case WT_BUTTON:
            break;

        // Status bar.
        case WT_STATUSBAR:
            Background = TRUE;
            window->backgroundUsed = TRUE;
            break;

        // �cone da �rea de trabalho.
        case WT_ICON:
            Background = TRUE;
            window->backgroundUsed = TRUE;
            break;

        // Continua ...

        // Ainda n�o implementada.
        default:
            panic("CreateWindow: default\n");
            break;
    };

	//
	// ==== Draw ====
	//
	
	// Hora de pintar. 
	// Os elementos ser�o inclu�dos se foram selecionados anteriormente.
	
//drawBegin:

	// Obs: 
	// Se for uma janela, pintaremos apenas a janela.
	// Se for um frame, pintaremos todos os elementos
	// presentes nesse frame de acordo com as flags.
	// Obs:
	// A janela precisa ser pintada em seu buffer dedicado.
	// Nesse momento o buffer dedicado da janela j� est� na estutura
	// da janela. Rotinas de pintura que tenham acesso � estrutura da
	// janela devem utilizar o buffer dedicado que for indicado na estrutura.
	// Para que seja poss�vel pintar uma janela em seu buffer dedicado,
	// devemos passar um ponteiro para a estrutura da janela em todas
	// as rotinas de pintura chamadas daqui pra baixo.
	// #todo: 
	// Passar estrutura de janela via argumento, para a rotina
	// de pintura ter acesso ao buffer dedicado.


	//if(DedicatedBuffer == 1){};


	// Se o view for igual NULL talvez signifique n�o pintar.
    if ( window->view == VIEW_NULL )
    {
		//#bugbug: fail.
		//window->show = 0;
		//window->redraw = 0;
		//return (void*) window;
    }


	// Minimized ? (Hide ?)
	// Se tiver minimizada, n�o precisa mostrar a janela, por�m
	// � necess�rio pintar a janela no buffer dedicado, se essa t�cnica 
	// estiver dispon�vel.
	// Talvez antes de retornarmos nesse caso seja necess�rio configurar 
	// mais elementos da estrutura.
	
	//#bugbug
	//se estamos contruindo a janela, ent�o ela n�o foi registrada 
	//n�o podemos checar as coisas na estrutura ainda,
	//mas a estrutura ja existe a algumas coisas foram inicializadas.
	
	// #importante
	// Pois retornaremos no caso de janelas minimizadas.
	// Provavelmente isso foi usado quando criamos janelas 
	// de refer�ncia na inicializa��o da GUI.(root)

    Minimized = FALSE;
    Minimized = (int) is_window_minimized (window);

    if ( Minimized == TRUE )
    {
        // window->draw = 1; //Devemos pint�la no buffer dedicado.

        window->show = FALSE;
        window->redraw = FALSE;
        // ...

		//@todo: N�o retornar. 
		//como teste estamos retornando.

        goto done;
    }



	// #todo: 
	// Maximized ?
	// Para maximizar devemos considerar as dimens�es da �rea de cliente
	// da janela m�e.
	// Se a jenela estiver maximizada, ent�o deve ter o tamanho da �rea de 
	// cliente da janela main.
	// Essa �rea de cliente poder� ser a �rea de trabalho, caso a
	// janela m�e seja a janela principal.
	// Obs: se estiver maximizada, devemos usar as dimens�o e coordenadas 
	// da janela gui->main.
	
	// #bugbug
	// Temos um problema com essa limita��o.
	// N�o conseguimos pintar janelas simples al�m do height da janela gui->main
	// para janelas overlapped funciona.
	
	
    Maximized = 0;
    Maximized = (int) is_window_maximized (window);

    if ( Maximized == 1 )
    {

        // #debug
		printf("file: createw.c: #debug\n");
		printf ("original: l=%d t=%d w=%d h=%d \n", 
		    window->left, gui->main->top, 
		    window->width, window->height );

        // Margens da janela gui->main
        window->left = gui->main->left;    
        window->top  = gui->main->top;

        // Dimens�es da janela gui->main.
        window->width  = gui->main->width;
        window->height = gui->main->height; 
        
        window->right  = (unsigned long) (window->left + window->width);
        window->bottom = (unsigned long) (window->top  + window->height);

		// ??
		// Deslocamentos em rela��o �s margens.
		// Os deslocamentos servem para inserir elementos na janela, 
		// como barras, bot�es e textos.

        window->x = 0;
        window->y = 0;

		//#debug
		printf ("corrigido: l=%d t=%d w=%d h=%d \n", 
		    window->left, gui->main->top, 
		    window->width, window->height );
		
		//#debug
        refresh_screen ();
        while (1){}
     }

	//
	// == ## draw ## ===========================
	//

    //  # FULL SCREEN #

	//??
	//entrar no modo full screen deve ser sempre uma iniciativa do usu�rio.
	//apenas um comando do usu�rio pode colocar uma janela no modo full screen.
	
    //View = 0;
	//View = (int) is_window_full(window);
    //if(View == 1){
	//	//...
	//}
	
	//Se estivermos em full screen, mudaremos algumas caracter�ticas.
	// N�o usaremos frame: nem borda nem barra de t�tulos.
	//if( window->view == VIEW_FULL )
	//{
    //    Shadow = 0;
	//	TitleBar = 0;
		
        //Dimens�es:
		//??@todo: dever�amos chamar m�todos que peguem
		//a m�trica do dispositivo. ex getmetricsDeviceWidth();
	//	window->left = 0; 
	//	window->top = 0; 
	//	window->width = 800;  //@todo: getmetricsDeviceWidth();
	//	window->height = 600;
						           
	//	window->titlebarUsed = 0;
	//}
	

	// Color:
	// Obs: @todo: Isso foi definido acima, foi passado por argumento e
	// colocado na estrutura. Fiacr� assim somente por teste,
	// depois deletaremos essa defini��o aqui.
	//*Importante: Devemos utilizar a cor que foi passada via argumento?!
	// sen�o todas as janelas ter�o a mesma cor.


	// Shadow
	//     A sombra pertence � janela e ao frame.
	//     A sombra � maior que a pr�pria janela.
	//     ?? Se estivermos em full screen n�o tem sombra ??

    if ( Shadow == TRUE )
    {
        window->shadowUsed = TRUE;

		//CurrentColorScheme->elements[??]
		
		//@todo: 
		// ?? Se tiver barra de rolagem a largura da 
		// sombra deve ser maior. ?? N�o ...
		//if()
		
        // @todo: Adicionar a largura das bordas verticais 
		// e barra de rolagem se tiver.
		// @todo: Adicionar as larguras das 
		// bordas horizontais e da barra de t�tulos.
		// Cinza escuro.  CurrentColorScheme->elements[??] 
		// @TODO: criar elemento sombra no esquema. 
		


	    if (window->focus == 1)
	    { __tmp_color = xCOLOR_GRAY1; }    //mais escuro
		if (window->focus == 0)
		{ __tmp_color = xCOLOR_GRAY2; }    //mais claro

        drawDataRectangle ( 
            (window->left +1), (window->top +1), 
            (window->width +1 +1), (window->height +1 +1), 
            __tmp_color ); 

        // ??
        // E os outros tipos, n�o tem sombra ??
        // Os outros tipos devem ter escolha para sombra ou n�o ??
        // Flat design pode usar sombra para definir se o bot�o 
        // foi pressionado ou n�o.

        // ...
    }

    // Background
    // Background para todo o espa�o ocupado pela janela e pelo seu frame.
    // O posicionamento do background depende do tipo de janela.
    // Um controlador ou um editbox deve ter um posicionamento relativo
    // � sua janela m�e. J� uma overlapped pode ser relativo a janela 
    // gui->main ou relativo � janela m�e.

    if ( Background == TRUE )
    {
        window->backgroundUsed = TRUE;
        window->bg_color = CurrentColorScheme->elements[csiWindowBackground]; 

        // O argumento 'color' ser� a cor do bg para alguns tipos.
        // Talvez n�o deva ser assim. Talvez tenha que se 
        // respeitar o tema instalado.
        if ( (unsigned long) type == WT_SIMPLE )
        { window->bg_color = color; }
        if ( (unsigned long) type == WT_POPUP ) 
        { window->bg_color = color; }
        if ( (unsigned long) type == WT_EDITBOX) 
        { window->bg_color = color; }
        if ( (unsigned long) type == WT_CHECKBOX)
        { window->bg_color = color; }
        if ( (unsigned long) type == WT_ICON )
        { window->bg_color = color; }
        // ...


		// Pintar o ret�ngulo.
		// #todo: 
		// ?? width Adicionar a largura da bordas bordas verticais.
		// #todo: 
		// ?? height Adicionar as larguras das bordas horizontais e da barra de t�tulos.

        // #todo
        // It does not exist by itself. It needs a parent window.

        if ( (unsigned long) type == WT_STATUSBAR )
        {
            drawDataRectangle ( 
                window->left, window->top, 
                window->width -1, window->height, 
                window->bg_color ); 

            draw_string ( 
                (window->left +8), (window->top +8), 
                0x00333333, window->name ); 
            goto done;
        }

        // Draw!

        drawDataRectangle ( 
            window->left, window->top, 
            window->width, window->height, 
            window->bg_color );

        //?? More ...
    }


    // # Client Area #
    // #importante
    // #todo: 
    //     RESPEITAR A IMPORT�NCIA DESSE ELEMENTO.
    //     PERMITIR QUE MAIS TIPOS DE JANELA TENHAM CLIENT AREA.
    // Nesse caso deveremos considerar o tipo de janela na hora de  
    // criarmos a client area, pois tem janela que tem barra de t�tulos 
    // e tem janela que n�o tem, tem janela que bordas e tem janela que 
    // n�o tem.
    // #Testando para diferentes tipos de janela.

    // #todo
    // It does not exist by itself. It needs a parent window.

    if ( ClientArea == TRUE )
    {
        window->clientAreaUsed = TRUE;

        // #obs: 
        // A Client Area � apenas um ret�ngulo.

        clientRect = (void *) kmalloc ( sizeof(struct rect_d) ); 

        if ( (void *) clientRect == NULL )
        {
            window->rcClient = NULL;

            // free(clientRect);

            // O que acontece nesse caso?? 
            // Ficaremos sem �rea de cliente ??
            
            // #bugbug
            // Nenhum alerta ser� emitido aqui por enquanto.

            //panic (?);
            //debug_print(?);

        } else {

			//@todo: Criar fun��o rectSetup(....)
			// Testando, isso pode falhar.

			// ?? DIMENS�ES DA JANELA: ??
			// A janela � um ret�ngulo que pode conter elementos como:
			// barra de menu, barra de ferramentas.
			// O frame n�o pertence a jenela. O frame � composto
			//pela barra de t�tulos e a borda.
			//
			// Obs: A �rea de cliente deve ter as dimens�es passadas
			//  via argumento.

            // Salva o ponteiro.
            window->rcClient = (void *) clientRect;

            window->rcClient->objectType  = ObjectTypeRectangle;
            window->rcClient->objectClass = ObjectClassGuiObjects;
            window->rcClient->used  = TRUE;
            window->rcClient->magic = 1234;


			// #importante
			// ( Posicionamento e dimencionamento. )
            // + Ajustando o posicionamento da �rea de cliente com base
            // no posicionamento e dimens�es da janela.
			// + Nesse espa�o ser�o inclu�dos elementos adicionais,
			// desenhados e repintados pelo pr�prio aplicativo.
			// + Essa � a �nica parte que deve aparecer quando o aplicativo
			// entrar em modo full screen.
			//
			
			// ## SIMPLE ##
			
			//#bugbug
			//Janela simples n�o deve ter �rea de cliente.

            //if ( window->type == WT_SIMPLE )
            if ( window->type == WT_OVERLAPPED || window->type == WT_SIMPLE )
            {
                 //#debug
                 //printf("createw: l={%d} t={%d} w={%d} h={%d}\n", 
                 //    window->left, window->top, window->width, window->height ); 	
                 //refresh_screen();
                 //while(1){}

                // left top
                window->rcClient->left = (unsigned long) (window->left);
                window->rcClient->top  = (unsigned long) (window->top);
            
                // width e height.
                // width = window widdth - as bordas verticais.
                // height = windows height - as bordas horizontais - a barra de t�tulos.
                window->rcClient->width  = (unsigned long) (window->width);
                window->rcClient->height = (unsigned long) (window->height);

				//window->rcClient->right = window->rcClient->left + window->rcClient->width;
				//window->rcClient->bottom = window->rcClient->top + window->rcClient->height;
            }

			//right bottom.
			//v�lido para todos os tipos de janela.

            window->rcClient->right  = (unsigned long) (window->rcClient->left + window->rcClient->width);
            window->rcClient->bottom = (unsigned long) (window->rcClient->top  + window->rcClient->height);

            // #bugbug Isso pode dar problemas.
            // Deslocamento dentro da �rea de cliente.
            // Zerando o deslocamento.

            window->rcClient->x = 0;
            window->rcClient->y = 0;

            //
            //  ESSA COR FOI PASSADA VIA ARGUMENTO.
            //

			//#debug
			//window->rcClient->bg_color = (unsigned long) COLOR_YELLOW; 

            window->rcClient->bg_color = (unsigned long) window->clientrect_bg_color;


            // Draw

            drawDataRectangle ( 
                (unsigned long) window->rcClient->left, 
                (unsigned long) window->rcClient->top, 
                (unsigned long) window->rcClient->width, 
                (unsigned long) window->rcClient->height, 
                (unsigned long) window->rcClient->bg_color );

            // Done
        };
        // Nothing
    }


    // #todo
    // It does not exist by itself. It needs a parent window.
        
    // # Status bar #
    if ( (unsigned long) type == WT_STATUSBAR )
    {
        //#test
        window->parent = Parent;
    }



	//
	//  ## Create button ##
	//
	
	// #BUGBUG: 
	// JANELA DO TIPO BOT�O.
    // N�O TEMOS INFORMA��ES SOBRE O TIPO DE BOT�O QUE DEVEMOS CRIAR. 
    // S� SABEMOS QUE A JANELA � DO TIPO BOT�O.
	// As posi��es de left e right s�o da janela do bot�o.
    // #obs: 
    // Essa fun��o retorna uma estrutura de bot�o.
    // Ent�o, uma janela, quando acianoada sua flag que indica que ela � um bot�o,
    // podemos usar a estrutura de bot�o para acessar as caracter�sticas do bot�o.
	// #obs
	// As cores das bordas dependem do status.
	
	// #obs:
	// Aqui o bot�o sempre � desenhado no mesmo estado.
	
	// #bugbug
	// Se o tipo � um botao ent�o o posicionamento � relativo.
	// tentaremos transformalo em absoludo, somando ao deslocamento da
	// janela m�e. Isso depois de pintarmos.
	// #bugbug: provavelmente isso dar� problemas na hora de repintar,
	//tentaremos corrigir depois.

    // #todo
    // It does not exist by itself. It needs a parent window.

    if ( (unsigned long) type == WT_BUTTON )
    {

        // Draw the button. (worker function)
        // Retorna a estrutura de bot�o.
        // See: button.c

        window->button = (struct button_d *) draw_button ( 
                                                 windowname, 
                                                 0, BS_DEFAULT, 0,
                                                 window->left, window->top, 
                                                 window->width, window->height, 
                                                 window->bg_color );

        if ( (void*) window->button == NULL ){
            panic ("CreateWindow: [DEBUG] window->button fail\n");
            //return NULL;
        }

        // Para repintar precisamos olhar na estrutura de bot�o
        // e encontrarmos a janela.
        
        window->button->window = (struct window_d *) window;

        // #bugbug
        // E se retornar NULL ?
        //if ( (void *) window->button == NULL ){} 
        
        // #test
        // A janela m�e da janela do bot�o.
        window->parent = (struct window_d *) Parent;
        
        // Yes, we are a button.
        window->isButton = TRUE;
    }


    // #test
    // #simple #

    if ( (unsigned long) type == WT_SIMPLE )
    {
        window->parent = Parent;
    }


	//
	// Continua ... 
	// ( Pinta detalhes da janela, caso ouver )
	//
	
	//Barras de rolagem ...
	//efeitos ...
	//itens de menus...
	//itens de barras ...
	//carrega �cones ...
	//Continua ...
	
	
	// Message: 
	// #todo: 
	// Enviar uma mensagem de CREATE pra janela.
	// Assim o procedimento cria os elementos restantes.
	
	// #todo: 
	// Atualizar a linkedlist da parent window.


    // Icon.
    if (window->type == WT_ICON)
    {
        bmpDisplayBMP ( 
            shared_buffer_app_icon, 
            (window->left +8), (window->top +8) );

        draw_string ( 
            (window->left +8), (window->top +16 +8 +8), 
            xCOLOR_GRAY7, window->name );  
    }

done:
    return (void *) window;
}



/*
 ******************************
 * kgws_create_window: (API)
 * 
 */

// Chamada por serviceCreateWindow em sci.c

void *kgws_create_window ( 
    unsigned long type, 
    unsigned long status, 
    unsigned long view, 
    char *windowname, 
    unsigned long x, 
    unsigned long y, 
    unsigned long width, 
    unsigned long height, 
    struct window_d *pWindow, 
    int desktopid, 
    unsigned long clientcolor, 
    unsigned long color ) 
{

    struct window_d *__w;
    int z=0;

    debug_print ("kgws_create_window: [API]\n");

    // No caso dos tipos com moldura ent�o criaremos em duas etapas.
    // no futuro todas ser�o criadas em duas etapas e 
    // CreateWindow ser� mais imples.
    
    if ( type == WT_OVERLAPPED )
    {

       // # importante
       // Essa rotina cria janelas simples, sem molduras, dos seguintes tipos:
       // WT_SIMPLE, WT_POPUP, WT_BUTTON, WT_STATUSBAR e WT_ICON

        //__w = (void *) CreateWindow ( 
        //                   WT_SIMPLE, status, view, 
        //                   (char *) windowname, 
        //                   x, y, width, height, 
        //                   (struct window_d *) pWindow, 
        //                   desktopid, 
        //                   clientcolor, color );      

        __w = (void *) CreateWindow ( 
                           WT_OVERLAPPED, status, view, 
                           (char *) windowname, 
                           x, y, width, height, 
                           (struct window_d *) pWindow, 
                           desktopid, 
                           clientcolor, color );      
    
    
        //pintamos simples, mas a tipagem ser�  overlapped
        __w->type = WT_OVERLAPPED;   
       goto draw_frame;
    }


    if ( type == WT_EDITBOX )
    {

       // # importante
       // Essa rotina cria janelas simples, sem molduras, dos seguintes tipos:
       // WT_SIMPLE, WT_POPUP, WT_BUTTON, WT_STATUSBAR e WT_ICON

		// Podemos usar o esquema padr�o de cores ...
        __w = (void *) CreateWindow ( 
                           WT_SIMPLE, status, view, 
                           (char *) windowname, 
                           x, y, width, height, 
                           (struct window_d *) pWindow, 
                           desktopid, 
                           clientcolor, color );      
    
        //pintamos simples, mas a tipagem ser�  overlapped
        __w->type = WT_EDITBOX;   
        goto draw_frame;
    }

    if ( type == WT_CHECKBOX )
    {
       // # importante
       // Essa rotina cria janelas simples, sem molduras, dos seguintes tipos:
       // WT_SIMPLE, WT_POPUP, WT_BUTTON, WT_STATUSBAR e WT_ICON

		// Podemos usar o esquema padr�o de cores ...
        __w = (void *) CreateWindow ( 
                           WT_SIMPLE, status, view, 
                           (char *) windowname, 
                           x, y, width, height, 
                           (struct window_d *) pWindow, 
                           desktopid, 
                           clientcolor, color );      
    
        //pintamos simples, mas a tipagem ser�  overlapped
        __w->type = WT_CHECKBOX;   
        goto draw_frame;
    }


    // #bugbug
    // Precisamos checar se o tipo eh valido.

    //================
    
    
    // OS TIPOS ABAIXO N�O PRECISAM DE FRAME.
    // PODERIAMOS DEIXAR ESSA ROTINA L� EM CIMA COM UM IF PARA SEUS TIPOS.
       // # importante
       // Essa rotina cria janelas simples, sem molduras, dos seguintes tipos:
       // WT_SIMPLE, WT_POPUP, WT_BUTTON, WT_STATUSBAR e WT_ICON
   
    __w = (void *) CreateWindow ( 
                       type, status, view, 
                       (char *) windowname, 
                       x, y, width, height, 
                       (struct window_d *) pWindow, 
                       desktopid, 
                       clientcolor, color );      

    goto done;


// draw frame.

draw_frame:

    //#IMPORTANTE
    //DESENHA O FRAME DOS TIPOS QUE PRECISAM DE FRAME.
    //OVERLAPED, EDITBOX, CHECKBOX ...

    // draw frame.
    // Nessa hora essa rotina podera criar a barra de t�tulos.
    // o wm poder� chamar a rotina de criar frame.

    if ( type == WT_OVERLAPPED || 
         type == WT_EDITBOX || 
         type == WT_CHECKBOX )
    {
        if (type == WT_EDITBOX)  {  __w->isEditBox  = TRUE;  }
        if (type == WT_CHECKBOX) {  __w->isCheckBox = TRUE;  }

        DrawFrame ( 
            (struct window_d *) __w, 
            x, y, width, height, 
            1 );  //style

        goto done;
   }

done:

    //
    // Thread.
    //

    // Associando a janela criada a trhead atual, 
    // que chamou essa rotina. O problema � na hora da inicializa��o.
    // #importante: Quando criarmos uma janela do tipo controle
    // uma thread ser� associada � ela. Ent�o o window manager
    // poder� mandar mensagem para o procedimento de janela
    // do aplicativo avisando dos efeitos dobre o controle.

    __w->control = (struct thread_d *) threadList[current_thread];

    if ( (void *) __w->control == NULL ){
        debug_print ("CreateWindow: [FAIL] control thread\n");
    }

    if ( (void *) __w->control != NULL )
    {
        if ( __w->used != TRUE || __w->magic != 1234 )
        {
            __w->control = NULL;
            debug_print ("CreateWindow: [FAIL] control thread validation\n");
        }
        // Thread ok.
    }


    //
    // MSG_CREATE
    //
        
    // #test
    // O procedimento de janelas do aplicativo receber� essa mensagem
    // quando criar uma overlapped, pedindo pra ele terminar de criar
    // as janelas filhas da overlapped.

    if (__w->type == WT_OVERLAPPED)
    {
        if ( (void *) __w->control != NULL )
        {
            if ( __w->used == TRUE || __w->magic == 1234 )
            {

                // Message queue.
                
                __w->control->window_list[ __w->control->tail_pos ] = __w;
                   __w->control->msg_list[ __w->control->tail_pos ] = MSG_CREATE;
                 __w->control->long1_list[ __w->control->tail_pos ] = 0;
                 __w->control->long2_list[ __w->control->tail_pos ] = 0;
                 
                __w->control->tail_pos++;
                if ( __w->control->tail_pos >= 31 )
                    __w->control->tail_pos = 0;

            }
        } 
    }


    // z order support.

    z = (int) z_order_get_free_slot();
    
    if ( z >= 0 && z < KGWS_ZORDER_MAX )
    {
        __w->z = z;
        Windows[z] = (unsigned long) __w;
        
    }else{
        panic ("kgws_create_window: No free slot on zorder\n");
    };

    return (void *) __w;
}


//
// End.
//
