/*
 * File: bootloader.h 
 * 
 *     #todo: Change the name to 'bl.h'
 *
 * Descri��o:
 *     Header principal do Boot Loader de 32 bits, BL.BIN.
 *     Feito em 32bit C/Assembly.
 *     Declara��es e prot�tipos para as fun��es principais do Boot Loader.
 *     Oferecer o suporte necess�rio nessa fase de inicializa��o.
 *     ...
 * 
 * History:
 * =========
 *     2013 - Created by Fred Nora.
 */

// Modes.
#define GRAMADO_JAIL        0x00
#define GRAMADO_P1          0x01
#define GRAMADO_HOME        0x02
#define GRAMADO_P2          0x03
#define GRAMADO_CASTLE      0x04
#define GRAMADO_CALIFORNIA  0x05
// ...

extern int current_mode;

//
// Vari�veis para habilita��es de verbose pra debug.
//


#include <config/config.h>  
#include <gdef.h>  

 
/*
 * Vari�veis importadas.
 * Stack pointers ??
 */
 
extern unsigned long task0_stack;
extern unsigned long task1_stack;
extern unsigned long task2_stack;
 
 
//
// Fun��es importadas.
//
 
extern void refresh_screen();
 
 
// Boot Loader version.
#define BL_VERSION  "0.1"


/*
 * Status do carregamento de um arquivo.
 */
 
//#define LOAD_SUCCESS  0
//#define LOAD_FAILURE  1    //Erro gen�rico.
//#define LOAD_
//Continua...		 
 
 

/*
 * FDC support.
 *
 * PORTS: Constants for support ports.
 *        @todo: N�o oferecer mais suporte ao FDC. 
 */
 
#define DOR1  0x3F2    //Digital Output Register. 
#define DOR2  0x372  
#define MSR1  0x3F4    //Main Status Register. 
#define MSR2  0x374  
#define DR1   0x3F5    //Data Register. 
#define DR2   0x375
#define DIR1  0x3F7    //Digital Input Register. 
#define DIR2  0x377
#define CCR1  0x3F7    //Configuration Control Register. 
#define CCR2  0x377
#define DOR   DOR1
#define DSR   MSR1


/*
 * Processor mode constants.
 */

#define SI_RM 0    //Real mode.
#define SI_PM 1    //Protected mode.
#define SI_LM 2    //Long mode.
#define SI_SM 3    //SMM, System Management Mode (Supervisor Mode).


/*
 * Boot Loader page directory constants.
 */
 

/*
 * Memory page tables:
 *     Mapeando mem�ria para o Boot Loader e os arquivos que ele tem 
 * que carregar.
 * Obs: Cada processo ter� seu pr�prio diret�rio de p�ginas. Isso significa 
 * que para cada processo, os endere�os l�gicos podem mudar, mas n�o deve-se 
 * mecher no endere�o l�gico do kernel. Todos os processos ter�o a parte
 * mais alta da sua mem�ria l�ca resercada para o Kernel. Tipicamente 
 * reserva-se o �ltimo gigabyte da mem�ria virtual de cada processo
 * para uso do Kernel.
 * Obs: O arquivo head.s configura CR3 com 0x01F00000, que � o endere�o
 * do primeiro page_directory. Os diret�rios e p�ginas s�o configurados 
 * em pages.c. De toda forma o Kernel pode refazer essa configura��o,
 * carregando os diret�rios de p�gina onde bem entender.
 * 
 */
#define BOOTLOADER_PAGEDIRECTORY  0x9C000    //Directory.
#define KM_PAGETABLE              0x8C000    //Kernel page table.
#define KM2_PAGETABLE             0x8D000    //Kernel page table 2.
#define UM_PAGETABLE              0x8E000    //User Mode page table.
#define VGA_PAGETABLE             0x8F000    //VGA page table..
//...

/*
 * Kernel memory constants.
 */

#define KERNEL_BASE     0x00100000    //1MB f�sico.
#define KERNEL_ADDRESS  0x00100000    //F�sico.
#define KERNEL_VA       0xC0000000    //L�gico. 


/*
 * User mode memory constants.
 */
 
#define  USER_BASE       0x00400000    //4MB f�sico.
//#define  USER_BASE32MB_PA   0x02000000    //32MB f�sico.


/*
 * Video Memory constans.
 */
#define VM_BASE   0x000B8000    //F�sico.
#define MEM_BASE  0x000A0000    //F�sico.


/*
 * Tasks constans.
 */
 
/*
 * APP and DLL:
 *     Endere�os virtuais do in�cio dos programas.
 * @todo: Os programas do sistema em User Mode poderiam ser carregados 
 * na parte mais alta da mem�ria virtual disponivel para aplicativos 
 * em user mode. Isso fica antes da �rea destinada ao Kernel. � um
 * forma de prote��o.
 *
 */
//#define DLL_VA 0             //V�nculo din�mico, .DLL, .SO.  
#define APP_VA 0x00400000    //Padr�o para aplicativos. 4MB. 
//...


// File support: 
// Usado pelo loader pra carregar arquivos do sistema.
// Obs: Esses s�o endere�os f�sicos para carregamento das aplica���es
// do sistema. Por�m o endere�o l�gico para o carregamento das
// aplica��es ser� 0x400000 e das bibliotecas compartilhadas ser� 0x0.


// #todo: delete it!
#define INIT_ADDRESS         0x00400000    //F�sico.
#define SHELL_ADDRESS        0x00450000    //F�sico. 
#define TASKMANAGER_ADDRESS  0x004A0000    //F�sico.
//... 

//Endere�os f�sicos.
//#define SERVER_ADDRESS       0x00400000    //Gen�rico.
//#define APP_ADDRESS          0x00400000    //Gen�rico.
 
 
/*
 * Disk constants.
 */ 

#define MBR_SIGNATURE  (0xAA55) 
#define SECTOR_SIZE    512  
#define INIT_SECTOR    20
//...



/*
 * Constants for text support.
 *     @todo: Definir tamanho padr�o.
 */

//Char.
#define CHAR_WIDTH   8
#define CHAR_HEIGHT  8
 
//Line 
#define LINE         1024    //800.??
#define LINE_MAX     768     //600. ?? 

//Column
//#define COL          1024    //800. ??
#define COL_MAX      1024    //800. ??


//...


/*
 * Constants for window support.
 *     @todo: Boot Loader n�o deve ter suporte � janelas.
 *            Apenas suporte ao tamanho ta tela.
 */

#define HWND_DESKTOP 0   //errado.
#define MAX_WINDOWS 256
#define DESKTOP_X        0
#define DESKTOP_Y        0
#define DESKTOP_LARGURA 1024    //640.
#define DESKTOP_ALTURA   768    //480.
//...


/*
 * Constants for menu support.
 */
 
#define menu_size  (MENU_ALTURA * 8)
#define MENU_X         0
#define MENU_Y         748    
#define MENU_LARGURA   200
#define MENU_ALTURA    200
#define MENU_INICIAR_X   0
#define MENU_INICIAR_Y  748    
#define MENU_INICIAR_LARGURA   60
#define MENU_INICIAR_ALTURA    20
//...


//
// Boot loader colors.
//


#define COLOR_WHITE 0x00FFFFFF
#define COLOR_BLACK 0x00000000
#define COLOR_RED   0x0000FF00
#define COLOR_GRAY  0x00808080
#define COLOR_BLUE  0x000000FF
// ...


/*
 * Includes.
 */
 
#include "en/strings.h"

#include <memmap.h>     //Memory Map - address.
#include <diskmap.h>    //Disk Map   - sectors.


//#test.
#include <heap.h> 

/*
 * Lib C support.
 */
#include <types.h>
#include <stddef.h>

//#test
#include <stdlib.h>

#include <stdio.h>
#include <string.h>
//...


/*
 * Boot Loader i386 specific support.
 */
#include <bli386.h>

/*
 * File System support.
 */
#include <sys/exec_elf.h>
#include <fs.h>


#include <pci.h>

/*
 * IRQs support.
 */

//#include <timer.h>     //irq 0.
#include <keyboard.h>    //irq 1.


#include <ata.h>
#include <ide.h>


//...


/*
 * Shell.
 * Boot Loader mini-shell support. 
 * Obs: This must be the last one.
 */
#include <shell.h>


//
// (Boot Loader Internal). Para rotinas globais come�adas com BlXXXX().
// Acessiveis � todos os m�dulos do boot loader.
//
#include <bli.h>


#include <render/r_render.h>

//
// Vari�veis globais.
//

//Se o boot loader est� inicializado.
extern int g_initialized;
extern int g_driver_hdd_initialized;


/*
 * Variables for text support.
 *     @todo: Usar o padr�o 'g_' para todas vari�veis globais
 * aqui no suporte � texto.
 */

extern unsigned long g_cursor_x;        //Cursor.
extern unsigned long g_cursor_y;        //Cursor. 
extern unsigned long g_system_color;    //Color. 
extern unsigned long g_char_attrib;     //Color.  
extern unsigned long CURSOR_X;
extern unsigned long CURSOR_Y; 
extern unsigned long STATUS_X;
extern unsigned long STATUS_Y;
extern unsigned long EDITBOX_X;
extern unsigned long EDITBOX_Y;
extern unsigned long EDITBOX_LARGURA;
extern unsigned long EDITBOX_ALTURA;
extern unsigned long EDITBOX_BG_COLOR;
extern unsigned long EDITBOX_TEXT_COLOR;


//
// CR3 support.
//

extern unsigned long LegacyCR3;        //Valor existente.
extern unsigned long BootLoaderCR3;    //Valor configurado pelo Boot Loader.

//CR0
//unsigned long registerCR0;


/*
 *  Variables for GUI support. Usar ou n�o o modo gr�fico.
 */
extern int useGUI;         


/*
 * Variables for window procedure support.
 */

extern unsigned long g_proc_status; 
extern unsigned long g_next_app;
extern unsigned long g_next_proc;
extern unsigned long g_nova_mensagem;
extern unsigned long next_address;  //??

/*
 * Variables for task support.
 * @todo: N�o usar o termo task.
 */ 
//unsigned long current_task;

//unsigned long proximo;  //??	


/*
 * Variables for LFB Address support. 
 */
extern unsigned long g_lbf_pa;    //Endere�o F�sico do LFB.
extern unsigned long g_lbf_va;    //Endere�o L�gico do LFB.

/*
 * Structures for Boot Loader.
 */
 


/*
 * system_d:
 *     Structure for global system informations.
 */
 
typedef struct system_d system_t;
struct system_d
{
    unsigned long processor_mode;        
    unsigned long video_mode;   
    //Continua...        
};
// main.c
extern system_t system_info;

 
/*
 * Structure for global video parameters.
 */ 
 
/*
 * VideoBlock:
 *     Estrutura para informa��es sobre par�metros de v�deo.
 *     @todo: criar um header para video. video.h.
 *
 */
typedef struct video_d VideoBlock_t;
struct video_d
{
	int useGui;      //Se estamos ou n�o usando gui. text mode, video mode.
	int vgaMode;     //N�mero do modo de v�deo se estiver usando vga.
	int vesaMode;    //N�mero do modo vesa.
	
	unsigned long LFB_PA;    //Endere�o F�sico do LFB.
	unsigned long LFB_VA;    //Endere�o L�gico do LFB.
	
	//...
	
	//currente video memory. Ponteiro para a base atual da memoria de video
	// em text mode.
	
	unsigned long currentVM; 
	char ch;                   //Caractere.
	char chAtrib;              //Atributo do caractere. 
	//...           
};
// main.c
extern VideoBlock_t VideoBlock;    //@todo: Usar um ponteiro.


/*
 * VESA support.
 */ 
 
typedef struct vesa_d vesa_t;
struct vesa_d
{
    unsigned long mode_number;    //N�mero do modo.
    unsigned long x_res;          //Pixels por linha.
    unsigned long y_res;          //Pixels por coluna.
    unsigned long bpp;            //Bits per pixel.		
	//Continua...
};
//main.c
extern vesa_t vesa_mode;


/*
 * Structure for support window procedure parameters.
 *
 */

typedef struct procedure_d procedure_t;
struct procedure_d
{
    unsigned long next;      //Endere�o do procimo procedimento.     
    unsigned long status;    //Status do procedimento do sistema. 
    unsigned long res1;      //Reservado.
    unsigned long res2;      //Reservado.
    // ...        	
};
// main.c
extern procedure_t procedure_info;


/*
  // gdtr support.
typedef struct gdtr_d gdtr_t;
struct gdtr_d 
{
    unsigned short  Limit;
    unsigned long   Base;
};
gdtr_t bl_gdtr;    //Sem ponteiro. 
*/


/*
 *  Boot Loader prototypes.
 */
 
 
 
struct menuitem_d
{
    int id;
    int used;
    int magic;
    
    char string[80];
    
    char image_path[128];    // "/BOOT/KERNEL.BIN"
    unsigned long image_address;
    
};

// main.c
extern int menu_highlight;
extern struct menuitem_d MENU[8];



/*
 * Ports support.
 */


unsigned char in8 (int port);
unsigned short in16 (int port);
unsigned long in32 (int port);
void out8 ( int port, unsigned char data );
void out16 (int port, unsigned short data);
void out32 ( int port, unsigned long data );


/*
 * CPU instructions support.
 */
 
void enable();
void disable();
void stopCpu();      
void intReturn();
void farReturn();
int getFlags(int variable);
int setFlags(int variable);
int Push(int value);
int Pop(int variable);
void pushRegs();
void popRegs();
void pushFlags();
void popFlags();
int getStackPointer(int addr);
int setStackPointer(int addr);


//
// Initialization support.
//

int init();
void init_globals();
void boot(); 



//
// Text support.
//

void set_up_color(unsigned long color);
void set_up_text_color(unsigned char forecolor, unsigned char backcolor); 
unsigned long get_cursor_x();
unsigned long get_cursor_y();
unsigned long input(unsigned long ch);
void panic(const char *msg); 



//
// Timer support.
//
void timer();
int BltimerInit();

//
// Paging support.
//
int SetUpPaging();


								   
//
// Disk support.
//  @todo: usar ingl�s.
//
void limpa_root(); //@todo: usar ingl�s.
void limpa_fat();  //@todo: usar ingl�s.

void 
my_read_hd_sector ( 
    unsigned long ax, 
    unsigned long bx, 
    unsigned long cx, 
    unsigned long dx);
    
void 
my_write_hd_sector ( 
    unsigned long ax, 
    unsigned long bx, 
    unsigned long cx, 
    unsigned long dx );
    
void write_lba( unsigned long address, unsigned long lba);
void read_lba( unsigned long address, unsigned long lba);



//
// PCI support.
//

int pciInit(); 
int pciInfo();
unsigned char pciGetClassCode(unsigned char bus, unsigned char slot);
unsigned short pciCheckVendor(unsigned char bus, unsigned char slot);
unsigned short pciCheckDevice(unsigned char bus, unsigned char slot);

unsigned short 
pciConfigReadWord ( 
    unsigned char bus, 
    unsigned char slot, 
    unsigned char func, 
    unsigned char offset );


//
// Boot Loader services.
//

void system_services();


void reboot();

// See: abort.c
//
void abort(void);

// See: main.c
void die(void);


//
// End.
//
