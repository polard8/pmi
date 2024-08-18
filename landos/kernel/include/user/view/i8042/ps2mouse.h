// ps2mouse.h
// Created by Fred Nora.

#ifndef ____PS2MOUSE_H
#define ____PS2MOUSE_H    1

// #bugbug
// We have these same definitions on ps2mouse.c ??


//Standard PS/2 Mouse Commands
//Byte	Data byte	Description
//0xFF	None	Reset
//0xFE	None	Resend
//0xF6	None	Set Defaults
//0xF5	None	Disable Data Reporting
//0xF4	None	Enable Data Reporting
//0xF3	Sample rate, ranges from 10-200.	Set Sample Rate
//0xF2	None	Get Device ID. See Detecting PS/2 Device Types for the response bytes.
//0xF0	None	Set Remote Mode
//0xEE	None	Set Wrap Mode
//0xEC	None	Reset Wrap Mode
//0xEB	None	Read Data
//0xEA	None	Set Stream Mode
//0xE9	None	Status Request

// 8042 mouse commands.
#define MOUSE_SET_RESOLUTION           0xE8
#define MOUSE_READ_STATUS              0xE9
#define MOUSE_GET_DEVICE_ID            0xF2
#define MOUSE_SET_SAMPLING_RATE        0xF3
#define MOUSE_ENABLE_DATA_REPORTING    0xF4  // Enable mouse transmition.
#define MOUSE_SET_DEFAULTS             0xF6
#define MOUSE_RESEND                   0xFE
#define MOUSE_RESET                    0xFF

// 8042 mouse responses.
#define MOUSE_ID_BYTE       0x00
#define WHEELMOUSE_ID_BYTE  0x03 
#define MOUSE_COMPLETE      0xAA 
//...
 
 
// Generic PS/2 Mouse Packet Bits
#define  MOUSE_LEFT_BUTTON    0x01
#define  MOUSE_RIGHT_BUTTON   0x02
#define  MOUSE_MIDDLE_BUTTON  0x04
#define  MOUSE_X_DATA_SIGN    0x10
#define  MOUSE_Y_DATA_SIGN    0x20
#define  MOUSE_X_OVERFLOW     0x40
#define  MOUSE_Y_OVERFLOW     0x80
 
//=======================================================
//++ Usadas pelo mouse.
// hardwarelib.inc
//

#define MOUSE_X_SIGN	0x10
#define MOUSE_Y_SIGN	0x20


//
// =====================================================
//

// this is gonna be used by PS2 structure in ps2.h
struct ps2_mouse_d
{
    object_type_t objectType;
    object_class_t objectClass;
    int dummy;

    // #todo
    // int control_fd;
    // int input_fd;
    // pid_t pid;
};

//bmp - buffer para o bmp
extern void *mouseBMPBuffer; 


//usado pra exibir provisoriamente alguma imagem em cima da janela 
//que o mouse passa por cima.
extern unsigned long savedmouseoverwindowX;
extern unsigned long savedmouseoverwindowY;

//usado pelo ponteiro.
extern int saved_mouse_x;
extern int saved_mouse_y;

//Estado dos botões do mouse
extern int mouse_buttom_1; 
extern int mouse_buttom_2;
extern int mouse_buttom_3;

//Estado anterior dos botões do mouse.
extern int old_mouse_buttom_1; 
extern int old_mouse_buttom_2;
extern int old_mouse_buttom_3;

//se ouve alguma modificação no estado 
//dos botões.
extern int mouse_button_action;

//--
//=========================================================

extern unsigned long g_mousepointer_width;
extern unsigned long g_mousepointer_height;

//
// =======================================================
//

// read and write the controller.
// ?? not used ??
void xxx_mouse_write (unsigned char data);
unsigned char xxx_mouse_read (void);

void expect_ack (void);

// #test
// pegamos os tres char do inout de mouse e transformamos em
// uma mensagem que será enviada para uma thread.

int MOUSE_SEND_MESSAGE (void *buffer);

static unsigned char getMouseData (void);

void kernelPS2MouseDriverReadData (void);

int load_mouse_bmp (void);

void ps2mouse_parse_data_packet (void);

//ps2mouse.c
void DeviceInterface_PS2Mouse(void);

int ps2_mouse_globals_initialize (void);
void ps2mouse_initialize_device (void);   

void set_ps2_mouse_status(int status);
int get_ps2_mouse_status(void);

// Conversando com o driver para fazer configurações.
unsigned long 
ps2_mouse_dialog ( 
    int msg,
    unsigned long long1,
    unsigned long long2 );
                   
                  
unsigned long ps2_mouse_get_info(int i);

// ?
void ps2mouse_change_and_show_pointer_bmp (int number);

#endif   

//
// End
//

