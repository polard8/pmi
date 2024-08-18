// 'reboot' command for Gramado.

// rtl
#include <rtl/gramado.h>
#include <stddef.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// libio
#include <libio.h>


// isso eh uma rotina de test
// Vai escrever em uma porta ja inicializada pelo kernel.
void serial_write_char (char data) 
{
    while (( libio_inport8(0x3F8 + 5) & 0x20 ) == 0);

    libio_outport8 (0x3F8, data);
}


//
// main
//

int main ( int argc, char *argv[] ){

    char *env_host;
    char *env_user;
    unsigned char good=0;



   /*
   //=================
   // testing read() stdin
    char buf[32];
    int n;
    while(1){

       //sprintf(buf,"durty");
       //write(0,buf,5);

       n = read(0,buf,1);
       if(n>0)
           printf(">>>>%c\n",buf[0]);
    }
   //=================
   */


    // ==============================================


    // #test
    // Testing shutdown in virtual machines.
    // #todo:
    // We can ask the system if we are in qemu or not.
    // See: https://wiki.osdev.org/Shutdown


     //int isQEMU = FALSE;
     //isQEMU = rtl_get_system_metrics(300);


    // In Bochs, and older versions of QEMU(than 2.0), you can do the following:
    // outw(0xB004, 0x2000);
    // libio_outport16(0xB004, 0x2000);

    // In newer versions of QEMU, you can do shutdown with:
    //if (isQEMU==TRUE){
        // outw(0x604, 0x2000);
    //    libio_outport16(0x604, 0x2000);
    //}


    // In Virtualbox, you can do shutdown with:
    // outw(0x4004, 0x3400);
    // libio_outport16(0x4004, 0x3400);


    // #status
    // OK. it is working on qemu. 
    
    // ==============================================

    //testing serial port
    //ok isso funcionou.
    serial_write_char('r');
    serial_write_char('e');
    serial_write_char('b');
    serial_write_char('o');
    serial_write_char('o');
    serial_write_char('t');
    serial_write_char('i');
    serial_write_char('n');
    serial_write_char('g');
    serial_write_char('.');
    serial_write_char('.');
    serial_write_char('.');


    puts        ("\n");
    printf      ("REBOOT.BIN: Rebooting ...\n");
    debug_print ("REBOOT.BIN: Rebooting ...\n");


    // get info from crt0.o

    env_host = (char *) getenv("HOSTNAME");
    env_user = (char *) getenv("USER");

    // Showing the info.
    
    if ( (void *) env_host != NULL ){
        printf ("Host name:  %s \n", env_host);
    }

    if ( (void *) env_user != NULL ){
        printf ("User:       %s \n", env_user);
    }

    printf ("Good Bye!\n");


    //===================================
    // reboot via libio.
via_libio:
    printf ("reboot via libio\n");
    good = 0x02;
    while (good & 0x02)
    {
        good = libio_inport8(0x64);
    };

    debug_print("reboot.bin: Go!\n");
         printf("reboot.bin: Go!\n");
    
    libio_outport8(0x64, 0xFE);
    //halt();       
     
    debug_print("reboot.bin: via_libio fail\n");
         printf("reboot.bin: via_libio fail\n");


    //===================================
    // reboot via rtl
via_rtl:
    printf ("reboot via rtl\n");
    debug_print("reboot.bin: Go!\n");
         printf("reboot.bin: Go!\n");
    rtl_reboot();
    exit(1);

    return (int) (-1);
}


