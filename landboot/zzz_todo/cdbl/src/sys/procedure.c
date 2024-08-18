/*
 * File: procedure.c 
 *
 * Descri��o:
 *     Procedimento de janela do Boot Loader.
 *     Obs: O Boot Loader tem procedimento de janela, pois trata interven��es 
 * feitas pelo usu�rio na hora da inicializa��o.
 *
 * 2015 - Created by Fred Nora.
 */

 
#include <bootloader.h>


/*
 * system_procedure: 
 *    Procedimento default do bootloader.
 *    (Trata mensagens de sistema como WIN KEY, F1, F2 ... ).
 *
 * ATEN��O:
 *    O bootloader aceita poucos comandos na hora da inicializa��o.
 *    Principalmente de F1 � F4.
 *    @todo: Mudar para BlProcedure(...).
 */

unsigned long 
bl_procedure ( unsigned long window, 
               int msg, 
               unsigned long long1, 
               unsigned long long2 ) 
{ 

    //printf("Procedure do bootloader!\n");  
  
    switch (msg)
    {             
        case MSG_KEYDOWN:
            switch (long1)
            {   
                /* 			
			     * Cancela a atividade atual que o Boot Loader 
				 * estiver realizando e reiniciar o computador.
                 */
				case VK_ESCAPE:
                    //@todo
				    break;
                              
                default:
				    //Nothing.
                    break; 
            };
        break;
          
        case MSG_SYSKEYDOWN:                 
            switch (long1)	       
            {   
                //4 possibilidades apenas.
				case VK_F1:
				    printf("F1");
					//Reboot ?
                    break;
                case VK_F2:
				    printf("F2");
                    break;
                case VK_F3:
				    printf("F3");
                    break;
                case VK_F4:
				    printf("F4");
                    break;

                default:
				    //Nothing.
                    break; 				
             };              
        break;
		
		default:
		    //Nothing.
			break;
    };
	
	
// Done.
//done:
	
    return (unsigned long) 0;	
};  


//
// End.
//

