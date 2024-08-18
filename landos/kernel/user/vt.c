// vt.c
// 2019 - Created by Fred Nora.


// #importante
// Um dispositivo terminal virtual
// usará o objeto tty para configurar suas características.

#include <kernel.h>


int fg_console=0;

// Listar os terminais virtuais.
// Isso vai ajudar a mandar as mensagens;
unsigned long vtList[32];


// ==================================================================

// Wrapper to tty_ioctl
// #bugbug:
// We already have this thing on terminal.c in sci/
int 
vt_ioctl ( 
    int fd, 
    unsigned long request, 
    unsigned long arg )
{
    debug_print ("vt_ioctl:\n");

    //if(fd<0)
        //return -1;

    return (int) tty_ioctl ( 
                     (int) fd, 
                     (unsigned long) request, 
                     (unsigned long) arg );
}


/*
 ***************************************
 * _vt_outbyte:
 *
 *     Draw the char into the backbuffer ?
 *     Low level helper.
 */

// #todo
// precisamos de uma flag para vt vt_stdio_terminalmode_flag.

void _vt_outbyte ( int c, struct tty_d *tty )
{
    // unsigned long i=0;

    unsigned long x=0;
    unsigned long y=0;


    //O caractere.

    // char Ch = (char) c;



    //
    // Check tty
    //

    if ( (void *) tty == NULL ){
        debug_print ("_vt_outbyte: tty\n");
        return;
    }else{

         if ( tty->used != TRUE || tty->magic != 1234 ){
             debug_print ("_vt_outbyte: tty validation\n");
             return;
         }
         
         
         // tty ok.
    };


	// #test
	// Tentando pegar as dimensões do char.
	// #importante: 
	// Não pode ser 0, pois poderíamos ter divisão por zero.
	

    int cWidth  = get_char_width();
    int cHeight = get_char_height();

    if ( cWidth == 0 || cHeight == 0 )
    {
        debug_print ("_vt_outbyte: char w h\n");
        panic       ("_vt_outbyte: fail w h\n");
    }

    // #bugbug
    // Caso estejamos em modo texto.
    // Isso ainda não é suportado.

    if ( VideoBlock.useGui == FALSE )
    {
        debug_print ("_vt_outbyte: kernel in text mode\n");
        panic       ("_vt_outbyte: kernel in text mode\n");
    }


    // #Importante: 
    // Essa rotina não sabe nada sobre janela, ela escreve na tela como 
    // um todo. Só está considerando as dimensões do 'char'.


    // Caso estivermos em modo gráfico.

    if ( VideoBlock.useGui == TRUE )
    {

        //#importante: Essa rotina de pintura deveria ser exclusiva para 
        //dentro do terminal.
        //então essa flag não faz sentido.		

        // ## NÃO TRANPARENTE ##
        // se estamos no modo terminal então usaremos as cores 
        // configuradas na estrutura do terminal atual.
        // Branco no preto é um padrão para terminal.
        // ## TRANSPARENTE ##
        // se não estamos no modo terminal então usaremos
        // char transparente.
        // Não sabemos o fundo. Vamos selecionar o foreground.

        x = (cWidth  * tty->cursor_x);
        y = (cHeight * tty->cursor_y);

        if ( stdio_terminalmode_flag == TRUE ){
            d_draw_char ( 
                x, y, c, COLOR_WHITE, 0x303030 );
        }else{
            d_drawchar_transparent ( 
                x, y, tty->cursor_color, c );
        };
    };
}


/*
 *******************************************************
 * vt_outbyte:
 *
 *    Outputs char on the pseudo terminal device.
 */

// #importante
// Isso é rotina de terminal virtual.

// #todo
// precisamos de uma flag para vt vt_stdio_terminalmode_flag.

void vt_outbyte ( int c, struct tty_d *tty ){

    static char prev = 0;


    //
    // Check tty
    //

    if ( (void *) tty == NULL ){
        debug_print ("vt_outbyte: tty\n");
        return;
    }else{

         if ( tty->used != TRUE || tty->magic != 1234 ){
             debug_print ("vt_outbyte: tty validation\n");
             return;
         }
         
         // tty ok.
    };


	// Obs:
	// Podemos setar a posição do curso usando método,
	// simulando uma variável protegida.
	
//checkChar:        
      
    //Opção  
    //switch ?? 


    //form feed - Nova tela.
    if ( c == '\f' ){
        tty->cursor_y = tty->cursor_top;
        tty->cursor_x = tty->cursor_left;
        return;
    }

    // #m$. 
    // É normal \n retornar sem imprimir nada.
    
    //Início da próxima linha. 
    if ( c == '\n' && prev == '\r' ) 
    {
		//#todo: melhorar esse limite.
        if ( tty->cursor_y >= (tty->cursor_bottom-1) )
        {
            vt_scroll (tty);

            tty->cursor_y = (tty->cursor_bottom-1);

            prev = c; 

        }else{

            tty->cursor_y++;
            tty->cursor_x = tty->cursor_left;
            prev = c;
        };

        return;
    };


    //Próxima linha no modo terminal.
    if ( c == '\n' && prev != '\r' ) 
    {
        if ( tty->cursor_y >= (tty->cursor_bottom-1) ){

            vt_scroll (tty);
            
            tty->cursor_y = ( tty->cursor_bottom-1);
            prev = c;

        }else{

            tty->cursor_y++;

			//Retornaremos mesmo assim ao início da linha 
			//se estivermos imprimindo no terminal.
            if ( stdio_terminalmode_flag == 1 )
            {
                tty->cursor_x = tty->cursor_left;	
            } 

			//verbose mode do kernel.
			//permite que a tela do kernel funcione igual a um 
			//terminal, imprimindo os printfs um abaixo do outro.
			//sempre reiniciando x.
            if ( stdio_verbosemode_flag == 1 )
            {
                tty->cursor_x = tty->cursor_left;
            } 

			//Obs: No caso estarmos imprimindo em um editor 
			//então não devemos voltar ao início da linha.

            prev = c;
        };

        return;
    };


	//tab
	//@todo: Criar a variável 'g_tab_size'.
    if ( c == '\t' )  
    {
        tty->cursor_x += (8);
        //g_cursor_x += (4); 
        prev = c;
        
        return; 

		//Não adianta só avançar, tem que apagar o caminho até lá.
		
		//int tOffset;
		//tOffset = 8 - ( g_cursor_left % 8 );
		//while(tOffset--){
		//	_outbyte(' ');
		//}
		//Olha que coisa idiota, e se tOffset for 0.
		//set_up_cursor( g_cursor_x +tOffset, g_cursor_y );
		//return;        
    };


	//liberando esse limite.
	//permitindo os caracteres menores que 32.
	//if( c <  ' '  && c != '\r' && c != '\n' && c != '\t' && c != '\b' )
	//{
    //    return;
    //};
 

    //Apenas voltar ao início da linha.
    if( c == '\r' )
    {
        tty->cursor_x = tty->cursor_left;  
        prev = c;
        return;
    }


    // #todo 
    // #bugbug 
    // retirei esse retorno para o espaço, com isso 
    // o ascii 32 foi pintado, mas como todos os 
    // bits estão desligados, não pintou nada.
    // space 
    //if( c == ' ' )  
    //{
    //    g_cursor_x++; 
    //    prev = c;
    //    return;
    //}

    // Delete
    // #bugbug: Limits.
    if ( c == 8 )  
    {
        tty->cursor_x--; 
        prev = c;
        return;
    }


	//
	// limits
	//

    // Filtra as dimensões da janela onde está pintando.
    // #todo: 
    // Esses limites precisam de variável global.
    // mas estamos usando printf pra tudo.
    // cada elemento terá que impor seu próprio limite.
    // O processo shell impõe seu limite.
    // a janela impõe seu limite etc...
    // Esse aqui é o limite máximo de uma linha.
    // Poderia ser o limite imposto pela disciplina de linha
    // do kernel para o máximo de input. Pois o input é
    // armazenado em uma linha.

//checkLimits:

    // Limites para o número de caracteres numa linha.
    if ( tty->cursor_x >= (tty->cursor_right-1) )
    {
        tty->cursor_x = tty->cursor_left;
        tty->cursor_y++;
    }else{

		// Incrementando.
		// Apenas incrementa a coluna.

        tty->cursor_x++;
    };

    // #bugbug
    // Tem um scroll logo acima que considera um valor
    // de limite diferente desse.

    // Número máximo de linhas. (8 pixels por linha.)
    if ( tty->cursor_y >= tty->cursor_bottom )  
    { 
        vt_scroll (tty);
        tty->cursor_y = tty->cursor_bottom;
    }

    // Imprime os caracteres normais.
    // Nesse momento imprimiremos os caracteres.
    // Imprime os caracteres normais.
    // Atualisa o prev.

    _vt_outbyte (c,tty);

    prev = c;
}



// scroll no pseudo terminal
// #bugbug

void vt_scroll (struct tty_d *tty)
{

    // Salvar cursor.

    unsigned long OldX=0; 
    unsigned long OldY=0;
    int i=0;

    // Check tty

    if ( (void *) tty == NULL ){
        debug_print ("vt_scroll: tty\n");
        return;
    }else{

         if ( tty->used != TRUE || tty->magic != 1234 )
         {
             debug_print ("vt_scroll: tty validation\n");
             return;
         }
         // tty ok.
    };

    // We are in graphics mode.

    if ( VideoBlock.useGui == TRUE )
    {

		// copia o retângulo.
		// #todo: olhar as rotinas de copiar retângulo.
		//See: comp/rect.c

        scroll_screen_rect ();

        //Limpa a última linha.

        // Save cursor
        OldX = tty->cursor_x;
        OldY = tty->cursor_y;

		//cursor na ultima linha.

        tty->cursor_x = tty->cursor_left;
        tty->cursor_y = ( tty->cursor_bottom -1);

        // Clear line
        // Put the cursor in the last line

        for ( i = tty->cursor_x; i < tty->cursor_right; i++ )
        {
            _vt_outbyte ( ' ',tty); 
        };

        tty->cursor_x = tty->cursor_left; 
        tty->cursor_y = OldY;

        refresh_screen ();
    }
}



// Create a terminal for a window.
// The father is creating a terminal in one of its windows.
int 
vt_create (
    struct window_d *window, 
    int father_pid )
{

    struct vt_d  *terminal;
    int i=0;


    if ( father_pid < 0 ){
        debug_print ("vt_create: father_ip\n");
        return -1;
    }


    if ( (void *) window == NULL ){
        debug_print ("vt_create: window\n");
        return -1;
    }

    if ( window->used == TRUE && window->magic == 1234 )
    {
         terminal = (struct vt_d *) kmalloc ( sizeof (struct vt_d) );
         
         if ( (void *) terminal == NULL ){
             debug_print ("vt_create: terminal\n");
             return -1;
         }
             
         //terminal->id = ?;
         terminal->used  = TRUE;
         terminal->magic = 1234;

         terminal->father_pid = father_pid;
         terminal->child_pid  = -1;

         terminal->state = 0;
         
         window->isTerminal = TRUE;
         window->terminal = terminal;
    }

    // register
    // Pra usar isso pela primeira vez tem que inicializar.
    // #todo: Check this limit.
    // Save and return the index.

    for (i=0; i<32; i++)
    {
        if ( vtList[i] == 0 )
        {
            vtList[i] = (unsigned long) terminal;
            return (int) i;
        }
    };

    // fail 
    panic ("vt_create: [FIXME] no empty slot\n");

    // Not reached
    return -1;
}


// Set child pid given in a window structure.
int 
vt_set_child ( 
    struct window_d *window, 
    int child_pid )
{

    if ( child_pid < 0){
        debug_print ("vt_set_child: child_pid\n");
        return -1;
    }

    if ( (void *) window == NULL ){
        debug_print ("vt_set_child: window\n");
        return -1;
    }

    if ( window->isTerminal != TRUE ){
        debug_print ("vt_set_child: not terminal\n");
        return -1;
    }

    if ( (void *) window->terminal == NULL ){
        debug_print ("vt_set_child: terminal\n");
        return -1;
    }


    if ( window->terminal->used  == TRUE && 
         window->terminal->magic == 1234 )
    {
        window->terminal->child_pid = child_pid;
        return 0;
    }
    
    // ??
    // Do we need a message here ?

    //fail
    return -1;
}



int 
vt_set_pty ( 
    struct vt_d *terminal, 
    struct tty_d *master, 
    struct tty_d *slave )
{

    if ( (void *) terminal == NULL ){
        debug_print ("vt_set_pty: terminal\n");
        return -1;
    }

    if ( terminal->used == TRUE && terminal->magic == 1234 )
    {
        terminal->master = master;
        terminal->slave  = slave;
        return 0;
    }

    return -1;
}


// init module.
int vt_init_module (void){

    register int i=0;

    debug_print("vt_init_module:\n");

    // init list
    // #todo: Check this limit

    for (i=0; i<32; ++i)
    {
        vtList[i] = (unsigned long) 0;
    };

    // ...
    
    return 0;
}


//
// End.
//




