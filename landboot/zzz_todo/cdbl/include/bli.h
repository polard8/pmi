/*
 * File: bli.h
 *
 * Boot Loader Internal. Para rotinas globais come�adas com BlXXXX.
 * Acessiveis � todos os m�dulos do boot loader.
 */
 
 

//
// Services support.
//

void *blServices(unsigned long arg1,unsigned long arg2,unsigned long arg3,unsigned long arg4); 


//
// Procedure.
//

unsigned long bl_procedure( unsigned long window,
                            int msg, 
							unsigned long long1, 
							unsigned long long2 );


//
// Finalizations support.
//
void BlAbort();							
 
//
// End.
//

