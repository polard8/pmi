/*
 * File: x86.c 
 *
 * Descri��o:
 *     Acesso ao hardware na arquitetura x86.
 *
 * Ambiente:
 *    Boot Loader. BL.BIN. Ring 0.
 *
 * Revision history:
 *
 *     2005 - Created by Fred Nora.
 *     2019 - Revision.
 */


#include <bootloader.h>


/*
 * enable:
 *     Habilita as interrup��es. 
 */ 

void enable ()
{
    asm (" sti ");
}


/*
 * disable:
 *     Desabilita as interrup��es.  
 */ 

void disable ()
{
    asm (" cli ");
}


void stopCpu ()
{
    asm ( " cli \n\t" 
          " hlt ");
}


void intReturn (){

    asm (" iret ");
}


void farReturn (){

    asm (" lret ");
}


// #todo: 
// Rever isso.
int getFlags (int variable) 
{
    asm (" pushfl \n\t"   
         " popl %0"       
         : "=r" (variable) );
}


// #todo: 
// Rever isso.
int setFlags (int variable)
{
    asm (" pushl %0 \n\t"   
         " popfl"           
          : : "r" (variable) );
}




int Push (int value)
{
    //__asm ("pushl %0" : : "r" (value) : "%esp" );
}


int Pop (int variable)
{
    //__asm ("popl %0" : "=r" (variable) : : "%esp" );
}


void pushRegs ()
{
    //__asm ("pushal" : : : "%esp");
}


void popRegs ()
{
    //__asm ("popal" : : : "%esp");
}


void pushFlags (){

    //__asm ("pushfl" : : : "%esp" );
}


void popFlags (){

    //__asm ("popfl" : : : "%esp" );
}


int getStackPointer (int addr){

    asm (" movl %%esp, %0" : "=r" (addr) );
}


int setStackPointer (int addr){

    //__asm ("movl %0, %%esp" : : "r" (addr) : "%esp" );
}


//
// End.
//

