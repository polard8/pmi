/*
 * File: ps2kbd.c
 *
 *     Keyboard controller support.     
 *     Ring 0. Kernel base persistent code.
 *
 * Credits:
 *     2018 - Created by Fred Nora.
 *     2018 - Nelson Cole.
 */


// See:
// http://www.brokenthorn.com/Resources/OSDev19.html



#include <kernel.h>


struct ioControl_d *ioControl_keyboard;   //0

int scStatus=0;

//
// ## keybuffer ##
//

// #bugbug
// Esses offsets precisar estar dentro da estrutura de tty
// pois lidam com um buffer dentro dela.
 
int keybuffer_index=0;
int keybuffer_head=0;
int keybuffer_tail=0;
int keyboard_message_head=0;
int keyboard_message_tail=0;

struct tty_d PS2keyboardTTY;

struct keyboard_message_d  keyboard_queue[8]; 


//see: events.h
int current_keyboard_responder=0;

//Se há uma nova mensagem de teclado. 
int kbMsgStatus=0;

//Status
//@todo: Status pode ser (int).
//variáveis usadas pelo line discipline para controlar o 
//estado das teclas de controle.
unsigned long key_status=0;
unsigned long escape_status=0;
unsigned long tab_status=0;
unsigned long winkey_status=0;
unsigned long ctrl_status=0;
unsigned long alt_status=0;
unsigned long shift_status=0;
unsigned long capslock_status=0;
unsigned long numlock_status=0;
unsigned long scrolllock_status=0;
//...


// Driver handler support.
unsigned long keyboard_handler_address=0;


/*
  //Credits: serenity os
#define IRQ_KEYBOARD 1
#define I8042_BUFFER 0x60
#define I8042_STATUS 0x64
#define I8042_ACK 0xFA
#define I8042_BUFFER_FULL 0x01
#define I8042_WHICH_BUFFER 0x20
#define I8042_MOUSE_BUFFER 0x20
#define I8042_KEYBOARD_BUFFER 0x00
 */


/*
enum KYBRD_ENCODER_IO {
 
	KYBRD_ENC_INPUT_BUF	=	0x60,
	KYBRD_ENC_CMD_REG	=	0x60
};
 
enum KYBRD_CTRL_IO {
 
	KYBRD_CTRL_STATS_REG	=	0x64,
	KYBRD_CTRL_CMD_REG	=	0x64
};
*/


// Keyboard Encoder Commands
/*
Command	Descripton
0xED	Set LEDs
0xEE	Echo command. Returns 0xEE to port 0x60 as a diagnostic test
0xF0	Set alternate scan code set
0xF2	Send 2 byte keyboard ID code as the next two bytes to be read from port 0x60
0xF3	Set autrepeat delay and repeat rate
0xF4	Enable keyboard
0xF5	Reset to power on condition and wait for enable command
0xF6	Reset to power on condition and begin scanning keyboard
0xF7	Set all keys to autorepeat (PS/2 only)
0xF8	Set all keys to send make code and break code (PS/2 only)
0xF9	Set all keys to generate only make codes
0xFA	Set all keys to autorepeat and generate make/break codes
0xFB	Set a single key to autorepeat
0xFC	Set a single key to generate make and break codes
0xFD	Set a single key to generate only break codes
0xFE	Resend last result
0xFF	Reset keyboard to power on state and start self test
*/


//Return Codes
/*
Value	Descripton
0x0	Internal buffer overrun
0x1-0x58, 0x81-0xD8	Keypress scan code
0x83AB	Keyboard ID code returned from F2 command
0xAA	Returned during Basic Assurance Test (BAT) after reset. Also L. shift key make code
0xEE	Returned from the ECHO command
0xF0	Prefix of certain make codes (Does not apply to PS/2)
0xFA	Keyboard acknowledge to keyboard command
0xFC	Basic Assurance Test (BAT) failed (PS/2 only)
0xFD	Diagonstic failure (Except PS/2)
0xFE	Keyboard requests for system to resend last command
0xFF	Key error (PS/2 only)
*/



// Onboard Keyboard Controller Commands
// Command Listing
/*
Command	Descripton
Common Commands
0x20	Read command byte
0x60	Write command byte
0xAA	Self Test
0xAB	Interface Test
0xAD	Disable Keyboard
0xAE	Enable Keyboard
0xC0	Read Input Port
0xD0	Read Output Port
0xD1	Write Output Port
0xE0	Read Test Inputs
0xFE	System Reset
0xA7	Disable Mousr Port
0xA8	Enable Mouse Port
0xA9	Test Mouse Port
0xD4	Write To Mouse
Non Standard Commands
0x00-0x1F	Read Controller RAM
0x20-0x3F	Read Controller RAM
0x40-0x5F	Write Controller RAM
0x60-0x7F	Write Controller RAM
0x90-0x93	Synaptics Multiplexer Prefix
0x90-0x9F	Write port 13-Port 10
0xA0	Read Copyright
0xA1	Read Firmware Version
0xA2	Change Speed
0xA3	Change Speed
0xA4	Check if password is installed
0xA5	Load Password
0xA6	Check Password
0xAC	Disagnostic Dump
0xAF	Read Keyboard Version
0xB0-0xB5	Reset Controller Line
0xB8-0xBD	Set Controller Line
0xC1	Continuous input port poll, low
0xC2	Continuous input port poll, high
0xC8	Unblock Controller lines P22 and P23
0xC9	Block Controller lines P22 and P23
0xCA	Read Controller Mode
0xCB	Write Controller Mode
0xD2	Write Output Buffer
0xD3	Write Mouse Output Buffer
0xDD	Disable A20 address line
0xDF	Enable A20 address line
0xF0-0xFF	Pulse output bit
*/


//
// Propotypes
//


int BAT_TEST (void);



// #test
// Loadng a file using a pathname as parameter.
// vamos apenas carregar um arquivo qualquer.

void __load_path_test(void)
{
    int status = -1;

    char __path[] = "/ETC/TEST1.CPP";

    void *__address;

    // Endereço para carregar o arquivo
    // 400 KB.
 
    size_t Size = (400*1024);




    __address = (void *) kmalloc(Size);

    status = fs_load_path (
                 (const char *)  __path,
                 (unsigned long) __address,
                 (unsigned long) Size );

    if ( status < 0 ){
        printf ("__load_path_test: fs_load_path fail\n");
    }

    // Show file.

    if ( status == 0 ){
        printf("%s \n",__address);    
    }

    refresh_screen();
}



/*
 *************************************** 
 * xxx_keyboard_write: 
 * 
 */

// Esta função será usada para escrever dados do teclado 
// na porta 0x60, fora do IRQ1.

void xxx_keyboard_write (uint8_t data)
{
    kbdc_wait(1);

    out8 ( 0x60, data );

    wait_ns(400);
}


/*
 *************************************** 
 * xxx_keyboard_read: 
 * 
 */

// Esta função será usada para ler dados do teclado na 
// porta 0x60, fora do IRQ1.

uint8_t xxx_keyboard_read (void)
{
    kbdc_wait (0);

    uint8_t val = in8(0x60);
    
    wait_ns(400);
    
    return (uint8_t) val;
}



// Esta rotina faz o Auto-teste 0xaa êxito, 0xfc erro
// Credits: Nelson Cole.

int BAT_TEST (void){

    int val = -1;
    int i=0;

	// #todo:
	// Cuidado.
	// Diminuir isso se for possivel.
	// Nao funciona na maquina reala sem esse delay.

    for (i=0; i<99000; i++)
    {
        wait_ns(400);
    };


    for ( i=0; i<999; i++ )
    {
        val = (int) xxx_keyboard_read();

        // Ok funcionou o auto teste
        if (val == 0xAA){
            printf ("ps2kbd.c: BAT_TEST OK\n");
            return (int) 0;
        // falhou
        }else if ( val == 0xFC ){
            printf ("ps2kbd.c: BAT_TEST fail\n");
            return (int) -1; 
        }

        // #bugbug
        // Tentar novamente, indefinidas vezes.
        // Esse é o problema. Vamos tentar apenas algumas vezes e falhar se não der certo.
        // Reenviar o comando. 
        // obs: este comando não é colocado em buffer
        
        //printf ("ps2kbd.c: BAT_TEST %d\n", i);
        xxx_keyboard_write (0xFE);
    };

    // Fail
    printf ("ps2kbd.c: BAT_TEST %d times\n",i);
    return (int) -1; 
}


/*
 *************************
 * ps2kbd_globals_initialize:
 * 
 *     ioControl_keyboard,
 *     PS2keyboardTTY
 *     
 * 
 */

// Initialize globals.

int ps2kbd_globals_initialize (void){

    int i=0;

    // slot na file table.
    int slot=-1;  


    // See: user.h

    ioControl_keyboard = (struct ioControl_d *) kmalloc ( sizeof(struct ioControl_d) );

    if ( (void *) ioControl_keyboard == NULL ){
        panic ("ps2_keyboard_initialize: [FAIL] ioControl_keyboard\n");
    }else{

        ioControl_keyboard->used  = TRUE;
        ioControl_keyboard->magic = 1234;

        ioControl_keyboard->id = 0;

		// qual thread está usando o dispositivo ?
        ioControl_keyboard->tid = 0;  
	    //ioControl_keyboard->
    };



    //int Type = 0;

	// #todo: 
	// Checar se o teclado é do tipo abnt2.   
	// É necessário sondar parâmetros de hardware,
	// como fabricante, modelo para configirar estruturas e variáveis.


/*
    switch(Type)
	{
	    //NULL
		case 0:	
		    break;
			
	    //Americano.
		case 1:	
		    break;

		//pt-ABNT2	
	    case 2:	
		    break;
			
		//...
		
		//Modelo americano
		default:	
		    break;
	}
	
*/
	
	//#debug
	//Tentando suprimir esse delay. OK
	//printf ("ps2_keyboard_initialize: 2\n");
	//refresh_screen();	



    //
    // == _rbuffer =========================================
    //

    // Raw input buffer.
    
    file *current_stdin;

    slot = get_free_slots_in_the_file_table();
    if (slot<0 || slot >=NUMBER_OF_FILES){
        panic("ps2kbd_globals_initialize: current_stdin file slot\n");
    }
    current_stdin =  (file *) file_table[slot];
    current_stdin->filetable_index = slot;
    current_stdin->____object = ObjectTypeFile; //Regular file (tty buffer)
    current_stdin->used   = TRUE;
    current_stdin->magic  = 1234;
    current_stdin->_flags = (__SWR | __SRD); 
    
    // Struct.
    // Allocate memory for the struct.
    unsigned char *current_stdin_struct_buffer;
    current_stdin_struct_buffer = (unsigned char *) newPage();
    current_stdin = (file *) &current_stdin_struct_buffer[0];

    // Buffer.
    // Allocate memory for the buffer.
    unsigned char *current_stdin_data_buffer;
    current_stdin_data_buffer   = (unsigned char *) newPage ();
    current_stdin->_base = (unsigned char *) &current_stdin_data_buffer[0];
    current_stdin->_bf._base = current_stdin->_base;
    current_stdin->_lbfsize = 128;
    current_stdin->_p = (unsigned char *) &current_stdin_data_buffer[0];    
    current_stdin->_r = 0;
    current_stdin->_w = 0;
    current_stdin->_cnt       = 128;  //Limitando. na verdade e' 4KB.
    current_stdin->_tmpfname  = "KBDIN   TXT";
    current_stdin->fd_counter = 1;

    // #test
    // Inicializando o buffer de teclado.
    // #bugbug Esses offsets são inicializados em wm também.
    // Elees deveriam estar dentro da estrutura de tty.
    for ( i=0; i< current_stdin->_cnt; i++ ){
        current_stdin->_base[i] = (char) 0;
    };
    keybuffer_head = 0;
    keybuffer_tail = 0;

    // Exportando o buffer para o driver usar.
    // Acessível através da tty.
    PS2keyboardTTY._rbuffer = current_stdin;



    //
    // == _obuffer =========================================
    //
    
    // Output buffer.

    file *current_stdout;

    slot = get_free_slots_in_the_file_table();
    if(slot<0 || slot >=NUMBER_OF_FILES){
        panic("ps2kbd_globals_initialize: current_stdout file slot\n");
    }
    current_stdout = (file *) file_table[slot];
    current_stdout->filetable_index = slot;
    current_stdout->____object = ObjectTypeFile; //Regular file (tty buffer)
    current_stdout->used   = TRUE;
    current_stdout->magic  = 1234;
    current_stdout->_flags = (__SWR | __SRD); 
    
    // Struct.
    // Allocate memory for the struct.
    unsigned char *current_stdout_struct_buffer;
    current_stdout_struct_buffer = (unsigned char *) newPage();
    current_stdout = (file *) &current_stdout_struct_buffer[0];

    // Buffer.
    // Allocate memory for the buffer.
    unsigned char *current_stdout_data_buffer;
    current_stdout_data_buffer   = (unsigned char *) newPage ();
    current_stdout->_base = (unsigned char *) &current_stdout_data_buffer[0];
    current_stdout->_bf._base = current_stdout->_base;
    current_stdout->_lbfsize = 128;
    current_stdout->_p = (unsigned char *) &current_stdout_data_buffer[0];    
    current_stdout->_r = 0;
    current_stdout->_w = 0;
    current_stdout->_cnt       = 128;  //Limitando. na verdade e' 4KB.
    current_stdout->_tmpfname  = "KBDOUT  TXT";
    current_stdout->fd_counter = 1;

    PS2keyboardTTY._obuffer = current_stdout;




    //Checar quem está tentando inicializar o módulo.

	//model.
	
	//handler.


    //Key status.
 
    key_status        = FALSE;
    escape_status     = FALSE;
    tab_status        = FALSE;
    winkey_status     = FALSE;
    ctrl_status       = FALSE;
    alt_status        = FALSE;
    shift_status      = FALSE;
    capslock_status   = FALSE;
    scrolllock_status = FALSE;
    numlock_status    = FALSE;
    // ...


	//
	// Set abnt2.
	//

    abnt2 = (int) TRUE;


	//Debug support.
	scStatus = 0;

    return 0;
}

/*
 **********************************
 * ps2kbd_initialize_device
 *     Inicializa o driver de teclado.
 *
 * #todo: 
 * Enviar para o driver de teclado o que for de lá.
 * Criar a variável keyboard_type ;;; ABNT2 
 * 2018 - Fred Nora.
 */

void ps2kbd_initialize_device (void)
{
    // register device.
    file *__file;   //device object
    char __tmpname[64];
    char *newname;



    __breaker_ps2keyboard_initialized = 0;


    ps2kbd_globals_initialize ();

//
// ==========================================
//


//__reset_keyboard:

    //++
    //=================================================
    
    //
    // Reset
    //
    
    // #obs:
    // A rotina abaixo reseta o teclado.
    // #bugbug: Isso reseta o controlador ps2.

    // #todo:
    // Podemos melhorar esse ack, e construirmos uma rotina
    // de ack igual a que existe na inicialização de mouser.

    // Reset.
    wait_then_write ( 0x60, 0xFF );
	// ACK
	// #bugbug: Danger, Danger !!
	// loop infinito
    wait_ns (400);
    wait_ns (400);
    wait_ns (400);
    wait_ns (400);
    while ( xxx_keyboard_read() != 0xFA );  

    // #todo
    // Talvez tenhamos que pegar mais coisa depois do reset,
    // assim como acontece no reset do mouse.
    
    //=================================================
    //--


//__BasicAssuranceTest:

    //++
    //=================================================
        
    // #obs:
    // Essa é uma rotina de auto-teste.
    // Conhecida como: Basic Assurance Test - (BAT).

    if ( BAT_TEST() != 0 ){
        printf ("[WARMING] ps2kbd.c:  BAT_TEST ignored\n");
    }  

    //=================================================
    //--



//__BasicAssuranceTest:

    //++
    //=================================================

    // #obs:
    // Configurando leds do teclado.
    
	//Leds.
	//LED_SCROLLLOCK 
	//LED_NUMLOCK 
	//LED_CAPSLOCK 
	//keyboard_set_leds(LED_NUMLOCK);

    //=================================================
    //--



//__empty_buffer:

    //++
    //=================================================

    // Clean buffer
    
    //while (IO::in8(I8042_STATUS) & I8042_BUFFER_FULL)
        //IO::in8(I8042_BUFFER);

    //=================================================
    //--

    // Wait for nothing!
    kbdc_wait (1);
    kbdc_wait (1);
    kbdc_wait (1);
    kbdc_wait (1);

//
// ==========================================
//

    //
    // name
    //
    
    //#test
    // isso não é o ponto de montagem.
    sprintf( (char *) &__tmpname[0], "/DEV_I8042_KEYBOARD");
    newname = (char *) kmalloc (64);
    if ( (void*) newname == NULL )
        panic("ps2kbd_initialize_device: newname");
    strcpy (newname,__tmpname);



    // Agora registra o dispositivo pci na lista genérica
    // de dispositivos.
    // #importante: ele precisa de um arquivo 'file'.
    
    int register_status = -1;
    
    __file = (file *) kmalloc ( sizeof(file) );
    
    if ( (void *) __file == NULL ){
        panic ("ps2kbd_initialize_device: __file fail, can't register device");
    }else{
        __file->used = 1;
        __file->magic = 1234;
        __file->isDevice = 1;


        //
        // Register.
        //

        // #importante
        // Essa é a tabela de montagem de dispositivos.
        // O nome do dispositivo deve ser um pathname.
        // Mas podemos ter mais de um nome.
        // vamos criar uma string aqui usando sprint e depois duplicala.
     
        register_status = devmgr_register_device ( 
            (file *) __file,             // file
             newname,                    // device name.                  
             0,                          //class (char, block, network)
             1,                          //type (pci, legacy
             (struct pci_device_d *) 0,  //pci device
             NULL );                     //tty driver
    
        if (register_status<0){
            panic("ps2kbd_initialize_device: devmgr_register_device fail");
        }
        
        // object
        PS2KeyboardDeviceObject = (file *) __file;               // object file
        if ( (void *) PS2KeyboardDeviceObject == NULL ){
           panic("ps2kbd_initialize_device: PS2KeyboardDeviceObject fail");   
        }

        //device
        PS2KeyboardDevice = (struct device_d *) __file->device;  // device struct
        if ( (void *) PS2KeyboardDevice == NULL ){
           panic("ps2kbd_initialize_device: PS2KeyboardDevice fail");   
        }

        //driver
        //PS2KeyboardDeviceTTYDriver = (struct ttydrv_d *) __file->device->ttydrv; // driver struct.
        //if ( (void *) PS2KeyboardDeviceTTYDriver == NULL ){
        //   panic("ps2kbd_initialize_device: PS2KeyboardDeviceTTYDriver fail");   
        //}

        
        // #hackhack
        // Actually we need to create a tty for all the devices.
        // And it needs to be done in devmgr_register_device
        // But we do not have all these resources roght now.

        // tty
        PS2KeyboardDeviceTTY = (struct tty_d *) tty_create();
        if ( (void *) PS2KeyboardDeviceTTY == NULL ){
           panic("ps2kbd_initialize_device: PS2KeyboardDeviceTTY fail");   
        }

        // ...
    };


    // #test
    // Drain the output buffer for the first time. Residual.

    I8042Controller_do_drain();

//
// ==========================================
//

    g_driver_ps2keyboard_initialized = (int) 1;
    
    __breaker_ps2keyboard_initialized = 1;
}


/*
 ************************ 
 * keyboardGetKeyState: 
 * 
 */

//Pega o status das teclas de modificação.
unsigned long keyboardGetKeyState ( unsigned char key ){
	
	unsigned long State = 0;
	
	switch (key){
		
		case VK_LSHIFT: 
		    State = shift_status; 
			break;

	    case VK_LCONTROL:
		    State = ctrl_status;
		    break;

	    case VK_LWIN:
		    State = winkey_status;
		    break;

	    case VK_LMENU:
		    State = alt_status;
		    break;

	    case VK_RWIN:
		    State = winkey_status;
		    break;

	    case VK_RCONTROL:
		    State = ctrl_status;
		    break;
			
	    case VK_RSHIFT:
		    State = shift_status;
		    break;

	    case VK_CAPITAL:
		    State = capslock_status;
		    break;

	    case VK_NUMLOCK:
		    State = numlock_status;
		    break;
			
		case VK_SCROLL:
            State = scrolllock_status;
            break;			
			
		//...
	};


    return (unsigned long) State;
}


/*
 ************* 
 * ldisc_init_modifier_keys: 
 * 
 */

// #todo
// Rever esse nome. 

// Inicializa o status das teclas de modificação.
// são usadas em comjunto com outras teclas para criar atalhos.
// modificam temporariamente a função de outra tecla.

void ldisc_init_modifier_keys (void){
	
	// Modifier keys.
	
	shift_status = 0;
	ctrl_status = 0;
	winkey_status = 0;
    
	// Alternate.
    alt_status = 0;

	// Alternate Graphic.
    //altgr_status = 0; //@todo
    
	// Function.
	//fn_status = 0;  //@todo
	
	//...
}



/*
 ************* 
 * ldisc_init_lock_keys: 
 * 
 */

// #todo
// Rever esse nome. 

// modificam permanentemente a função de outra tecla.
//ativa as teclas extendidas.

void ldisc_init_lock_keys (void){
	
    // Capital Lock.	
	capslock_status = 0;
	
	// Scrolling Lock.
	scrolllock_status = 0;
	
	// Number Lock.
	numlock_status = 0;	
}


/*
 **********************************
 * keyboardEnable:
 *     Enable keyboard.
 */

void keyboardEnable (void){


    // #bugbug
    // Dizem que isso pode travar o sistema.

	// Wait for bit 1 of status reg to be zero.
    while ( ( in8 (0x64) & 2) != 0 )
    {
		//Nothing.
    };

	//Send code for setting Enable command.
    out8 (0x60,0xF4);
    //sleep(100);
}


/*
 * keyboardDisable:
 *     Disable keyboard.
 */

// Wait for bit 1 of status reg to be zero.
// Send code for setting disable command.
    
void keyboardDisable (void){

    while ( ( in8 (0x64) & 2) != 0 )
    { 
         // Nothing.
    };

    out8 (0x60,0xF5);
    //sleep(100);
}


/*
 ***************************************
 * keyboard_set_leds:
 *     Set keyboard flags.
 *     ED = Set led.
 */

void keyboard_set_leds (char flag){

    //#todo: Filtro.

    // Wait for bit 1 of status reg to be zero.
    while ( ( in8 (0x64) & 2) != 0 )
    {
        // Nothing.
    };
    // Send code for setting the flag.
    out8 (0x60,0xED); 
    sleep (100);


    // Wait for bit 1 of status reg to be zero.
    while ( ( in8 (0x64) & 2) != 0 )
    {
        // Nothing.
    };
    // Send flag. 
    out8 (0x60,flag);
    sleep (100);

    // #todo: Mudar o status.
    // switch(flag){}
}



/*
 * KdGetWindowPointer:
 *     Retorna o ponteiro da estrutura de janela que pertence a thread.
 *     Dado o id de uma thread, retorna o ponteiro de estrutura da janela 
 * à qual a thread pertence.
 */


// #bugbug
// Um driver de teclado não tem essa relação com janela.


void *KdGetWindowPointer (int tid)
{
    tid=0;
    panic("KdGetWindowPointer: Deprecated");
    return NULL;

    /* 
    struct thread_d *t;

    // #todo: 
    // Filtrar argumento. 

    if ( tid < 0 )
        return NULL;


    // Structure.
    t = (void *) threadList[tid];

    if ( (void *) t == NULL ){ return NULL; }

    return (void *) t->window;

    */
}


/*
 * KbGetMessage:
 *     Pega a mensagem na fila de mensagens na estrutura da thread
 * com foco de entrada.
 *
 * Na estrutura da thread com foco de entrada tem uma fila de mensagens.
 * Pegar a mensagem.
 * 
 * Para falha, retorna -1.
 *
 * @todo: bugbug: A mensagem deve estar na fila do processo, na
 *                estrutura do proceso. (Talvez não na thread e nem na janela.)
 */

int KbGetMessage (int tid)
{
    tid=0;
    panic("KbGetMessage: Deprecated");
    return -1;

    /*
	int ret_val;
	struct thread_d *t;
	
	// Structure.
	t = (void*) threadList[tid];

	if ( (void *) t != NULL )
	{
        ret_val = (int) t->msg;
	}else{
	    ret_val = (int) -1;    //Fail.
	};

	WindowProcedure->msgStatus = 0;    //Muda o status.
	return (int) ret_val;              //Retorna a mensagem.
	
	*/
}


/*
 * KbGetLongParam1:
 *    Pega o parametro "long1" do procedimento de janela de uma thread.
 */

unsigned long KbGetLongParam1 (int tid)
{

    tid=0;
    panic("KbGetLongParam1: Deprecated");
    return 0;

    /*
	struct thread_d *t;
	
	// Structure.
	t = (void *) threadList[tid];

    if ( (void *) t == NULL){
        return (unsigned long) 0; 
    }

    return (unsigned long) t->long1;
    
    */
}


/*
 * KbGetLongParam2:
 *     Pega o parametro "long2" do procedimento de janela de uma thread.
 */
unsigned long KbGetLongParam2 (int tid){

    tid=0;
    panic("KbGetLongParam2: Deprecated");
    return 0;

    /*
	struct thread_d *t;
	
	// Structure.
	t = (void *) threadList[tid];

    if ( (void *) t == NULL){
        return (unsigned long) 0; 
    }

    return (unsigned long) t->long2;
    */
}



// Get alt Status.
int get_alt_status (void)
{
    return (int) alt_status;
}


// Get control status.
int get_ctrl_status (void)
{
    return (int) ctrl_status;
}
 
 
//Get shift status.
int get_shift_status (void)
{
    return (int) shift_status;
}


// events.h
void set_current_keyboard_responder ( int i )
{
    current_keyboard_responder = i;
}


// events.h
int get_current_keyboard_responder (void)
{
    return (int) current_keyboard_responder;
}



//
// ===========================================================
//


/*
 *********************** 
 * __do_35: 
 *     Getting message from thread's queue.
 */

    // #todo: 
    // 35 - Getting message from the thread's queue. 
    // Vamos pegar uma mensagem na fila de mesagens da thread
    // e colocarmos nos elementos de single message para 
    // o aplicativo pegar.


void *__do_35 ( unsigned long buffer )
{

    panic ("__do_32: #todo");

    return NULL;

    /*
    unsigned long *message_address = (unsigned long *) buffer;
    
    unsigned char SC=0;
    
    struct thread_d *t;

    struct msg_d *msg;


    // ==================================
    //             STOPPED !!!
    // ==================================

    // #todo
    // Trabalhando nessa rotina de servir uma mensagem que está na fila.
    // Ainda nem existe a rotina que coloca a mensagem na fila.
    
    // #debug breakpoint.
    panic ("gde_serv-__do_32: #todo");

    // ==================================
    //             STOPPED !!!
    // ==================================
  
    
    
    if ( (void *) t == NULL ){
         panic ("__do_35: t");
    }


    //
    // Trying ...
    //

    
    
    //
    // #todo
    //
    
    // Vamos pegar uma mensagem na fila de mesagens da thread
    // e colocarmos nos elementos de single message para o aplicativo pegar.    
    // msg = (struct msg_d *) t->MsgQueue[ t->MsgQueueHead++ ]
    
    
    // pega o ponteiro da estrutura.
    msg = ( struct msg_d * ) t->MsgQueue[ t->MsgQueueHead ];
    

    // Ok temos uma mensagem
    // ja pegamos vamos limpar a fila, mesmo que a mensagem não preste.

    //limpa elemento da fila.
    t->MsgQueue[ t->MsgQueueHead ] = (unsigned long) 0;
    
    // avança
    t->MsgQueueHead++;
    
    // circula.
    if ( t->MsgQueueHead >= 32 )
    {
        t->MsgQueueHead = 0;
    }

    
    // #todo
    // Checar validade da estrutura de mensagem.
    // Checar validade da estrutura de thread;.    
    
    // A estrutura é inválida ou não há mensagens
    if ( (void *) msg == NULL )
    {
        return NULL;
    } else {
    
        if ( msg->used != 1 || msg->magic != 1234 )
        {
            panic ("__do_35: msg struct");
        }
        

        // #todo
        // Se o head for igual ao tail, então o processo não está respondendo.
        // Exceto na inicialização.
    
        // #todo
        // Pegar os elementos da estrutura de mensagem e colocar
        // no buffer me single message usado para pegar uma mensagem.
    
	    //padrão
	    message_address[0] = (unsigned long) msg->window;
	    message_address[1] = (unsigned long) msg->msg;
	    message_address[2] = (unsigned long) msg->long1;
	    message_address[3] = (unsigned long) msg->long2;
 
	    //sinalizamos que a mensagem foi consumida.
	    //#todo: nesse momento a estrutura da thread também precisa ser limpa.
        t->newmessageFlag = 0;    
        return (void *) 1;    
    };
    
    */
}


/*
 *********************** 
 * __do_111: 
 *     Getting single system message from the process queue.
 */

// Getting single message.
// No queue.
// See: thread structure.

	// #importante:
	// Esse é o get message usado pelos aplicativos.
	// O aplicativo envia um endereço de array e devemos colocar 4 longs 
	// como mensagem.
	// Isso funcionou. 
	// Esse será o padrão até usarmos ponteiro para estrutura.
	// A intenção agora é pegar a mensagem na estrutura de thread atual.
	// Desse modo cada aplicativo, quando estiver rodando, pegará as 
	// suas mensagens em sua própria fila.  
	// Se não houver mensagem na estrutura da thread, então tentaremos colocar alguma.
	// Vamos colocar alguma coisa do buffer de teclado.

// #todo
// Trocar por 'ubuf'

// #bugbug
// Isso eh valido para todos os tipos de mensagens nao somente teclado
// isso deveria sair daqui desse arquivo.

void *__do_111 ( unsigned long buffer )
{
    struct thread_d *t;

    unsigned long *message_address = (unsigned long *) buffer;


    // Buffer
    // Se o buffer for inválido, não há o que fazer.
    if ( buffer == 0 ){ 
        panic ("__do_111: buffer\n"); 
    }

   // Again. 
   // Se o buffer for inválido.
   
   if ( &message_address[0] == 0 ){
       panic ("__do_111: &message_address[0] buffer\n");
   }else{

        t = (void *) threadList[current_thread];

        if ( (void *) t == NULL ){
            panic ("__do_111: Invalid thread calling \n");
        }

        if ( t->used != 1 || t->magic != 1234 ){
            panic ("__do_111: Validation. Invalid thread calling \n");
        }

        // Get system message.
        message_address[0] = (unsigned long) t->window_list[ t->head_pos ];
        message_address[1] = (unsigned long) t->msg_list[ t->head_pos ];
        message_address[2] = (unsigned long) t->long1_list[ t->head_pos ];
        message_address[3] = (unsigned long) t->long2_list[ t->head_pos ];

        // Extra. 
        // Usado pelos servidores e drivers.
        message_address[4] = (unsigned long) t->long3_list[ t->head_pos ];
        message_address[5] = (unsigned long) t->long4_list[ t->head_pos ];

        // Clean
        t->window_list[ t->head_pos ] = NULL;
        t->msg_list[ t->head_pos ]    = 0;
        t->long1_list[ t->head_pos ]  = 0;
        t->long2_list[ t->head_pos ]  = 0;
        t->long3_list[ t->head_pos ]  = 0;
        t->long4_list[ t->head_pos ]  = 0;
        //...
        
        // Circula
        t->head_pos++;
        if ( t->head_pos >= 31 )
            t->head_pos = 0;
   
        //Sinaliza que há mensagem
        return (void *) 1; 
    };

    // No message.
    return NULL;
} 



/*
 *************************** 
 * ps2tty_get_byte_from_input_buffer:
 */

// Low level keyboard reader.
// Isso poderia usar uma rotina de tty

// #importante
// Isso é usado pelo serviço que pega mensagens de input. (111).
// Pega o scancode.
// Renova a fila do teclado
// O teclado esta lidando no momento com um buffer pequeno, 128 bytes.

// Called by thread_getchar in thread.c

// #bugbug
// Nada foi colocado do buffer de input ainda.
// KGWS_SEND_KEYBOARD_MESSAGE colocou ascii code no buffer canonico.

int ps2tty_get_byte_from_input_buffer (void)
{
    unsigned long SC = 0;

    // Getting a byte from the input buffer
    // of the ps2 keyboard tty.

    SC = (unsigned char) PS2keyboardTTY._rbuffer->_base[keybuffer_head];

    // Clean the slot.

    PS2keyboardTTY._rbuffer->_base[keybuffer_head] = 0;

    // Increment and round the queue.

    keybuffer_head++;

    if ( keybuffer_head >= PS2keyboardTTY._rbuffer->_lbfsize )
    { 
        keybuffer_head = 0; 
    }

    // Return the byte.

    return (int) SC; 
}


void ps2tty_put_byte_into_input_buffer( char c )
{
    debug_print ("ps2tty_put_byte_into_input_buffer: [FIXME]\n");
    
    // #todo: 
    // Aqui podemos retornar.
    // Pois vamos precisar dessa estrutura para o buffer.
    
    if ( (void *) PS2keyboardTTY._rbuffer == NULL )
    {
        panic ("put_scancode: PS2keyboardTTY._rbuffer \n");
    }

    // #bugbug
    // Checar a validade.

    PS2keyboardTTY._rbuffer->_base[keybuffer_tail++] = (char) c;
    
    if ( keybuffer_tail >= PS2keyboardTTY._rbuffer->_lbfsize )
    {
        keybuffer_tail = 0;
    }

}



/*
 * *******************************************************
 * DeviceInterface_PS2Keyboard: 
 * 
 *     Vamos pegar o raw code.
 * 
 *     Keyboard handler for abnt2 keyboard.
 *     fica dentro do driver de teclado.
 *
 *     A interrupção de teclado vai chamar essa rotina.
 *     @todo: Usar keyboardABNT2Handler().
 * void keyboardABNT2Handler() 
 * Esse será o handler do driver de teclado
 * ele pega o scacode e passa para a entrada do line discipline dentro do kernel.
 *
 * @TODO: ISSO DEVERÁ IR PARA UM ARQUIVO MENOR ... OU AINDA PARA UM DRIVER.
 * Pega o scacode cru e envia para a disciplina de linhas que deve ficar no kernelbase.
 * Essa é a parte do driver de dispositivo de caractere.
 *
 * #importante:
 * O driver deverá de alguma maneira notificar o kernel sobrea a ocorrência
 * do evento de input. Para que o kernel acorde as trheads que estão esperando 
 * por eventos desse tipo.
 */

	//#importante:
	// Provavelmente uma interrupção irá fazer esse trabalho de 
	// enviar o scancode para o kernel para que ele coloque na fila.
	// Nesse momento o kernel de sentir-se alertado sobre o evento de 
	// input e acordar a threa que está esperando por esse tipo de evento. 

    // #obs: 
    // Esse buffer está em gws/user.h 

// Low level keyboard writter.
// Isso poderia usar uma rotina de tty
// O teclado esta lidando no momento com um buffer pequeno, 128 bytes.

// PUT SCANCODE

void DeviceInterface_PS2Keyboard(void){

    static int __has_e0_prefix = 0;
    static int __has_e1_prefix = 0;


    // ??
    // See: Serenity os.
    //u8 status = IO::in8(I8042_STATUS);
    //if (!(((status & I8042_WHICH_BUFFER) == I8042_KEYBOARD_BUFFER) && (status & I8042_BUFFER_FULL)))
        //return;


    //não precisamos perguntar para o controlador se
    //podemos ler, porque foi uma interrupção que nos trouxe aqui.
    // #obs:
    // O byte pode ser uma resposta à um comando ou 
    // um scancode.

    unsigned char __raw = 0;
    unsigned char val   = 0;

sc_again:

    //===========================================
    // #test
    // Testing with ack
    // credits: minix
    // #define KEYBD   0x60  /* I/O port for keyboard data */
    // #define PORT_B  0x61  /* I/O port for 8255 port B (kbd, beeper...) */
    // #define KBIT    0x80  /* bit used to ack characters to keyboard */

//
// Get the raw byte for the key struck.
//

    __raw = in8(0x60);


    //===========================================
    
    // Get
    
    // strobe the keyboard to ack the char
    val = in8(0x61); 
    
    // Send back
    
    // Strobe the bit high 
    out8(0x61, val | 0x80);  
    
    // now strobe it low
    out8(0x61, val);         
    //===========================================



    //===========================================
    // #todo
    // Temos que checar se o primeiro byte é um ack ou um resend.
    // isso acontece logo apos a inicialização.

    // #todo
    // me parece que o primeiro byte pode ser um ack ou resend.
    
    // #define ACKNOWLEDGE         0xFA	
    // #define RESEND              0xFE

    if ( __raw == 0xFA ){

        //#test
        printf ("DeviceInterface_PS2Keyboard: [test.first_byte] ack\n");
        refresh_screen();
    }

    if ( __raw == 0xFE ){
        
        //#test
        printf ("DeviceInterface_PS2Keyboard: [test.first_byte] resend\n");
        refresh_screen();
    }
    //===========================================



//
// == Queue ====================================
//

     // #bugbug
     // [Enter] in the numerical keyboard isn't working.
     // teclas do teclado extendido.
     // Nesse caso pegaremos dois sc da fila.
    // #obs:
    // O scancode é enviado para a rotina,
    // mas ela precisa conferir ke0 antes de construir a mensagem,
    // para assim usar o array certo.
    // See: ws/ps2kbd.c
    
    // #bugbug
    // Esse tratamento do scancode não faz sentido quando temos um
    // window server instalado. Nesse caso deveríamos deixar o
    // window server pegar os scancodes.
    // Mas por enquanto, essa rotina manda mensagens para o ws
    // caso tenha um instalado.


     if ( __raw == 0 )   {                      goto done;  }
     if ( __raw == 0xE0 ){ __has_e0_prefix = 1; goto done;  }
     if ( __raw == 0xE1 ){ __has_e1_prefix = 1; goto done;  }


// do_put:

    // + Build the message and send it to the thread's queue.
    // This routine will select the target thread.
    // + Or send the message to the input TTY.
    // This way the foreground process is able to get this data.
    // See: ps2kbd.c
    // See: user/console.c

    // IN: 
    // device type, data.
    // 1=keyboard

    if ( foreground_thread < 0 ){
        debug_print ("DeviceInterface_PS2Keyboard: Invalid foreground_thread\n");
        // Clean the mess.
        __has_e0_prefix = 0;
        __has_e1_prefix = 0;
        goto done;
    }

    console_interrupt (
        foreground_thread,
        CONSOLE_DEVICE_KEYBOARD,
        __raw );


    // Clean the mess.
    __has_e0_prefix = 0;
    __has_e1_prefix = 0;

done:
    return;
}





//
// End.
//





