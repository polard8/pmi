/*
 * File: x86\drivers\timer.c 
 *
 * Descri��o:
 *     Handler da irq0 para o Boot Loader.
 *
 * ATEN��O:
 *    O timer no Boot Loader n�o faz task switch.
 *    O timer ser� usado principalmente para timeout de driver.
 *    O timer pode ser usado para contar tempo de inicializa��o.
 *    
 * 2015 - Created by Fred Nora.
 */

 
#include <bootloader.h>


unsigned long timerTicks;


/*
 * timer:
 *     Timer handler for Boot Loader.
 *     #bugbug: Nem sei se devemos habilitar o timer durante o boot loader.
 */
 
void timer (){
	
	// Incrementa o contador.
	// De tempos em tempos, faz alguma coisa.
	
    timerTicks++;    
	
    if ( timerTicks % 100 == 0 )
	{		
        
    };
    
    //Nothing for now.
	
//done:
    //return;      
};


/*
 * BltimerInit:
 *     Inicializa o m�dulo timer.
 */
int BltimerInit (){
	
    int Status = 0;
	
	timerTicks = 0;
    
	//...
	
//done:
	
	return (int) Status;	
};


//
// End.
//

