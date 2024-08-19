/*
 * File: faults.c 
 *
 * Description: 
 *     Rotinas de tratamento de faults e exception para o Boot Loader em 
 * sistema 32bit para desktop.
 * Obs: Essa rotina pertence ao Boot Loader, que est� em ring 0.
 *
 * Vers�o: 1.0, 2015.
 */
 
 
#include <bootloader.h>


/*
 **************************************************
 * cpu_falts:
 *     Tratamento de faults do Boot Loader.
 *     Trata a fault de acordo com o n�mero.
 * Obs: Isso pertence ao Boot Loader. N�o h� threads para serem fechadas.
 * @todo: Exibir as falhas de inicializa��o.
 *
 * 2015 - Created.
 */
void cpu_falts (unsigned long fault_number){
	
    g_cursor_x = 0;
	g_cursor_y = 0;
	
	printf("faults-cpu_falts: %d\n", fault_number );
	
	// Mostra o erro de acordo com o n�mero.	 
    switch (fault_number)
	{
	    //EXCEPTION
		case 1:
		case 3:
		    printf("EXCEPTION \n");
			mostra_reg(current_task);
			break;
		
		//DOUBLE FAULT
	    case 8:
			printf("DOUBLE FAULT \n");
			mostra_reg(current_task);
		    break;

	    //STACK
	    case 12:
			printf("STACK FAULT \n");
			mostra_reg(current_task);
		    break;

	    //GP
	    case 13:
		    printf("GP \n");
			mostra_reg(current_task);
		    break;
		
		//PAGE FAULT
	    case 14:
		    printf("PAGE FAULT \n");
			mostra_reg(current_task);
		    break;
	    
	    default:			
			printf("Default number\n");
            mostra_reg(current_task);			
			break;
	};
	
	//
	// More ?! Tratamento, informa��es.
	//
  
//
// Done. 
//

done:
    die();
	
	//
    //  No return.
    //	     
};



/*
 * mostra_reg:
 *     Mostra os valores dos registradores.
 * @todo: Isso deve ir para outro arquivo. ???!!!
 *        Se ficar aqui. mudar o nome para faultShowRegisters();.
 */
//void faultShowRegisters(int tid) ??

void mostra_reg(unsigned long id){
	return;    //suspensa. 
};

//
// End.
//
