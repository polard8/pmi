/*
 * File: pages.c
 *
 * Ambiente: 
 *     Boot Loader, Ring0.
 *
 * Descri��o: 
 *     Configura��o da pagina��o de mem�ria.
 *     O Boot Loader faz uma pr�via configura��o de p�ginas. Podendo 
 * ser alteradas futuramente pelo Kernel.
 * Obs: Essa configura��o b�sica n�o impede que o Kernel fa�a uma 
 * reconfigura��o completa.
 *
 * Hist�rico:
 *     Vers�o: 1.0, 2015 - Created by Fred Nora.
 *     Vers�o: 1.0, 2016 - Revision.
 *     ...
 */


#include <bootloader.h>


/*
 * SetUpPaging:
 *     Configura as p�ginas. Mapeamento.
 *
 * In this function:
 *     (Phase 1) Endere�os da mem�ria f�sicas acess�veis em Kernel Mode. 
 *     (Phase 2) Inicializando o diret�rio.
 *     (Phase 3) Cria tabelas de p�ginas e coloca o ponteiro de cada uma 
 * delas na sua respectiva entrada de diret�rio.
 *     (Phase 4) **** CRIANDO OUTROS DIRET�RIOS ****
 *
 * @diretorio:
 *   page_directory = 0x9C000.
 *    
 *   Page directory do kernel.
 *
 * Obs: Esse diret�rio criado ser� usado pelas primeiros processos durante
 * essa fase de constru��o do sistema. O ideal � um diret�rio por processo.
 *      Toda vez que o Kernel inicia a execu��o de um processo, ele deve 
 * carregar o endere�o do diret�rio do processo em CR3. Por enquanto s� tem 
 * um diret�rio criado e todos os processos est�o definindo o diret�rio do
 * Kernel como sendo seu pr�prio. Ou seja, eles podem usar as mesmas tabelas
 * de p�ginas que o processo kernel usa. O problema � que com isso os 
 * processos precisam ter endere�o virtual diferentes. Quando cada processo
 * tiver seu pr�prio diret�rio ent�o eles poder�o ter o mesmo endere�o virtual.
 *
 * @todo: Esses endere�os precisam ser registrados em vari�veis globais ou
 * dentro de uma estrutura para serem passados para o Kernel.
 * 
 * Obs: Essa deve ser uma interface que chama as rotinas de configura��o de 
 * pagina��o. (Mapeamento). 
 *
 * Obs: Essa configura��o inicial feita no Boot Loader, n�o impede o Kernel de 
 * refazer as configura��es b�sicas de pagina��o. O fato � que o kernel ser� o 
 * gerenciador de p�ginas de mem�ria. O que n�o impede de haver gerenciamento
 * um user mode.
 * 
 * 2015 - Created.
 * Oct 2016, Revision.
 * ...
 */
//int pagesSetUpPages()
 
int SetUpPaging (){
	
    unsigned int i;
	
    //
	//(Phase 1) Endere�os da mem�ria f�sicas acess�veis em Kernel Mode.
    //	
	
	//
	// Kernel.
	//
	
	//Address. para os 4 primeiros mega da mem�ria fisica.
	//Base, 0x100000, para o kernel que come�a no primeiro mega.
	unsigned long kernel_address = 0;           //In�cio da mem�ria RAM.                
	unsigned long kernel_base = KERNEL_BASE;    //In�cio da imagem do kernel.    
	
    //
	// (Phase 2) Endere�os da mem�ria f�sicas acess�veis em User Mode.
    //
	
	// User, 0x00400000.
	unsigned long user_address =  USER_BASE; // USER_BASE32MB_PA;	
	
	// VGA, F�sico=0x000B8000.
	unsigned long vga_address  = VM_BASE;   
	
	// VESA LFB, foi passado pelo Boot Manager.
	unsigned long lfb_address  = (unsigned long) g_lbf_pa;
    
	// BUFFER, provis�rio. @todo: Mudar. (para sistemas com pouca mem�ria.)
	//16mb. (16*1024*1024) = 0x01000000.
	unsigned long buff_address  = (unsigned long) 0x01000000; 

	
	//
	// DIRECTORY:
	// Diret�rios de p�ginas.
    // Esse valor � salvo em cr3. Cada diret�rio tem seu endere�o.
    // Esse diret�rio, configurado aqui no Boot Loader � o mesmo usado 
	// pelo Kernel.
	//
	// Obs: 
	// A id�ia � que os diret�rio sejam criados em ordem decrescente.
	// Os espa�os entre os diret�rio ser�o preenchidos com p�ginas 
	// avulsas. Depois, no kernel, haver� �rea de aloca��o para 
	// diret�rios e p�ginas.
	//
	// Teste:
	// Criando diret�rios para os primeiros processos do sistema. 
	// (antes de 32MB).
    //
	// Obs: Essa �rea onde est�o os diret�rios � uma �rea desprotegiada ainda.
	// Obs: O kernel est� reconfigurando e usando outro endere�o para o 
	// diret�rio de p�ginas.
	//
	// @todo: Alertar o kernel sobre esses endere�os de diret�rios.
	//
	
	
	unsigned long *page_directory         = (unsigned long *) (0x01000000 - 0x900000);   	
	unsigned long *idle_page_directory    = (unsigned long *) (0x01000000 - 0x901000); 
	unsigned long *shell_page_directory   = (unsigned long *) (0x01000000 - 0x902000); 
	unsigned long *taskman_page_directory = (unsigned long *) (0x01000000 - 0x903000); 
	unsigned long *x_page_directory       = (unsigned long *) (0x01000000 - 0x904000); 
	unsigned long *xx_page_directory      = (unsigned long *) (0x01000000 - 0x905000); 
	unsigned long *xxx_page_directory     = (unsigned long *) (0x01000000 - 0x906000); 	
	//Continua descendo.
	
	
	//
	// TABLES: 
	// Tabelas de p�ginas.
	// Uma tabela para cada �rea de mem�ria que se deseja usar.
	// Usando endere�os decrescentes.     
	//
	
	//kernel mode. (Endere�os).
	unsigned long *km_page_table   = (unsigned long *) 0x8F000;    
    
	//kernel mode. (O kernel).
    unsigned long *km2_page_table  = (unsigned long *) 0x8E000;    
    
	//user mode.
	unsigned long *um_page_table   = (unsigned long *) 0x8D000;    

	//user mode. (vga).
	unsigned long *vga_page_table  = (unsigned long *) 0x8C000;     
    
	//user mode. (LFB).
	unsigned long *lfb_page_table  = (unsigned long *) 0x8B000;        
	
	//user mode. (buffer).
    unsigned long *buff_page_table = (unsigned long *) 0x8A000;        
    
	//
	// Continua... ??
    // 
 
 
	// Message. (verbose)
	//printf("SetUpPaging: Initializing Pages..\n");
	
	//
	// (Phase 2) Inicializando o diret�rio.
	//
	
	/*
	 * Inicializando o diret�rio de p�ginas usado pelo processo kernel.
	 * Isso preenche todo o diret�rio com p�ginas n�o presentes.
	 * 'kernel_address' � o in�cio da mem�ria RAM.
	 */	
	
ClearDirectory:	
		 
	for ( i=0; i < 1024; i++ ){
	    page_directory[i] = kernel_address | 2;    //0010 em bin�rio.
	};

	
	//
	// (Phase 3) Cria tabelas de p�ginas e coloca o ponteiro de cada uma delas 
	// na sua respectiva entrada de diret�rio.
	//

Entry_0:	
	/*
	 * kernel mode pages 
	 * =================
	 * (kernel mode)(0fis = 0virt).
	 *
	 * Configurando o in�cio da mem�ria RAM
	 * como �rea em kernel mode.
	 *
	 * + Mapeando os primeiros 4MB da mem�ria.  
	 * + Preenchendo a tabela km_page_table.
	 * + A entrada '0' do diret�rio aponta para
	 *   uma tabela que mapeia os primeiros 4 mega 
	 *   de endere�o virtual.
	 *
	 * 'kernel_address' � o in�cio da mem�ria RAM.
	 *  Cada pagina tem 4KB.
	 */	
    for(i = 0; i < 1024; i++){
	    km_page_table[i] = kernel_address | 3;     //0011 bin�rio.
	    kernel_address = kernel_address + 4096;    //4KB page.
    };
    //Criando a primeira entrada do diret�rio.
	//Isso mapeia os primeiros 4MB da mem�ria RAM.
    page_directory[0] = (unsigned long) &km_page_table[0];
    page_directory[0] = page_directory[0] | 3;    //Configurando atributos.
    
Entry_1:
	/*
	 * user mode pages 
	 * ===============
	 * (400000fis = 400000virt).
	 *
	 * Configurando uma �rea de mem�ria em user mode,
	 * usada por processos em user mode.
	 * Mapear 4MB da mem�ria come�ando em 400000fis. 
	 * (user mode).
	 * user_address = (400000fis = 400000virt).
	 *
	 */
    for(i = 0; i < 1024; i++){
	    um_page_table[i] = user_address | 7;    //0111 em bin�rio.
	    user_address = user_address + 4096;     //4096 = 4KB.
    };
    //Criando a entrada n�mero 1 do diret�rio.
    page_directory[1] = (unsigned long) &um_page_table[0];
    page_directory[1] = page_directory[1] | 7;    //Configurando atributos.
	 
	 
Entry_2:	 
    /*
	 * User Mode VGA pages: 
	 * ===================
	 * (0xB8000fis = 800000virt).
	 * 
	 * Mapeando a �rea de mem�ria usada pela mem�ria
	 * de v�deo em modo texto, 0xB8000.
	 *
	 * Mapear 4MB da mem�ria come�ando em B8000fis.
	 *
	 * Obs:
	 *     Aqui, na verdade n�o precisaria configurar 4 megas, 
	 *     apenas o tamanho da mem�ria de v�deo presente em 0xb8000.
	 * vga_address = 0xB8000.
	 */
    for(i = 0; i < 1024; i++){
	    vga_page_table[i] = vga_address | 7;    //0111 em bin�rio.
	    vga_address = vga_address + 4096;       //4KB.
    };
    //Criando a entrada n�mero 2 no diret�rio.
    page_directory[2] = (unsigned long) &vga_page_table[0];
    page_directory[2] = page_directory[2] | 7;    //Configurando atributos.

Entry_768:
	/*
	 * kernel mode pages 
	 * =================
	 * (0x100000fis = 0xc0000000virt).
	 *  
	 * Configurando a �rea de mem�ria onde ficar� a imagem do kernel.
	 * Isso mapeia 4MB come�ando do primeiro mega. 
	 * (kernel mode).
	 *
	 * Preenchendo a tabela km_page_table.
	 * 'kernel_base' � o endere�o f�sico da imagem do kernel.
	 */	
    for(i = 0; i < 1024; i++){
	    km2_page_table[i] = kernel_base | 3;    //0011 bin�rio.
	    kernel_base = kernel_base + 4096;       //4KB.
    };
    //Criando a entrada de n�mero 768 do diret�rio.
    page_directory[768] = (unsigned long) &km2_page_table[0];
    page_directory[768] = page_directory[768] | 3;    //Configurando atributos.	
	
	
Entry_769:	
    /*
	 * user mode LFB pages 
	 * ===================
	 * (0X??fis = 0xC0400000virt).
	 *
	 * Mapear 4MB � partir do endere�o configurado
	 * como in�cio do LFB.
	 * 
	 * O Boot Manager configurou VESA e obteve o endere�o do LFB.
	 * O Boot Manager passou para o Boot Loader esse endere�o.
	 * Mapeando 4MB da mem�ria fisica come�ando no 
	 * endere�o passado pelo Boot Manager.
	 * O endere�o de mem�ria virtual utilizada � 0xC0400000.
	 * lfb_address = Endere�o do LFB, passado pelo Boot Manager.
	 *
	 */
    for(i = 0; i < 1024; i++){
	    lfb_page_table[i] = lfb_address | 7;    //0111 em bin�rio.
	    lfb_address = lfb_address + 4096;       //4KB.
    };
    //Criando a entrada n�mero 769 do diret�rio.
    page_directory[769] = (unsigned long) &lfb_page_table[0];
    page_directory[769] = page_directory[769] | 7;    //Configurando atributos.	
	

Entry_770:	
    /*
	 * user mode BUFFER1 pages 
	 * =======================
	 * (0X??fis = 0xC0800000virt) (BackBuffer).
	 * 
	 * Esse � o backbuffer para a mem�ria de v�deo.
	 * O conte�do desse buffer � transferido para o LFB. 
	 * O endere�o de mem�ria virtual utilizada � 0xC0800000.
	 * buff_address = 0x01000000. (16MB), provis�rio.
	 * //16mb. (16*1024*1024) = 0x01000000.
	 */
    for(i = 0; i < 1024; i++){
	    buff_page_table[i] = buff_address | 7;    //0111 em bin�rio.
	    buff_address = buff_address + 4096;       //4KB.
    };
    //Criando a entrada n�mero 770 do diret�rio.
    page_directory[770] = (unsigned long) &buff_page_table[0];
    page_directory[770] = page_directory[770] | 7;    //Configurando atributos.	
	
	
	//
	// Continua ?!
	//     Mais p�ginas podem ser criadas agora.
	//     Mas ficar� � cargo do Kernel fazer o
	//     gerenciamento do conjunto completo de
	//     p�ginas usadas pelo sistema.
	//
	
	
	//
	// (Phase 4) **** CRIANDO OUTROS DIRET�RIOS ****
	//
	
	
	//
	// **** IDLE ****
	//
	
	//Iniciando o diret�rio idle.
	for(i = 0; i < 1024; i++){
	    idle_page_directory[i] = kernel_address | 2;    //0010 em bin�rio.
	};	

    //Entrada 0.
	//aproveitando a tabela criada antes.
    //Criando a primeira entrada do diret�rio da idle.
	//Isso mapeia os primeiros 4MB da me�ria RAM.
    idle_page_directory[0] = (unsigned long) &km_page_table[0];
    idle_page_directory[0] = idle_page_directory[0] | 3;    //Configurando atributos.
	
	//Entrada 1.
	//aproveitando outra tabela.
    //Criando a entrada n�mero 1 do diret�rio.
    idle_page_directory[1] = (unsigned long) &um_page_table[0];
    idle_page_directory[1] = idle_page_directory[1] | 7;    //Configurando atributos.
	
	//Entrada 2.
	//vga.
    //Criando a entrada n�mero 2 no diret�rio.
    idle_page_directory[2] = (unsigned long) &vga_page_table[0];
    idle_page_directory[2] = idle_page_directory[2] | 7;    //Configurando atributos.	
	
    //Entrada 768.
	//Criando a entrada de n�mero 768 do diret�rio.
    idle_page_directory[768] = (unsigned long) &km2_page_table[0];
    idle_page_directory[768] = idle_page_directory[768] | 3;    //Configurando atributos.		
	
	//Entrada 769.
	//LFB
    //Criando a entrada n�mero 769 do diret�rio.
    idle_page_directory[769] = (unsigned long) &lfb_page_table[0];
    idle_page_directory[769] = idle_page_directory[769] | 7;    //Configurando atributos.		
	
	//Entrada 770.
	//backbuffer.
    //Criando a entrada n�mero 770 do diret�rio.
    idle_page_directory[770] = (unsigned long) &buff_page_table[0];
    idle_page_directory[770] = idle_page_directory[770] | 7;    //Configurando atributos.		
	
	
	
	//
	// **** SHELL ****
	//

    //Iniciando o diret�rio shell.
	for(i = 0; i < 1024; i++){
	    shell_page_directory[i] = kernel_address | 2;    //0010 em bin�rio.
	};	

    //Entrada 0.
	//aproveitando a tabela criada antes.
    //Criando a primeira entrada do diret�rio da shell.
	//Isso mapeia os primeiros 4MB da me�ria RAM.
    shell_page_directory[0] = (unsigned long) &km_page_table[0];
    shell_page_directory[0] = shell_page_directory[0] | 3;    //Configurando atributos.
	
	//Entrada 1.
	//aproveitando outra tabela.
    //Criando a entrada n�mero 1 do diret�rio.
    shell_page_directory[1] = (unsigned long) &um_page_table[0];
    shell_page_directory[1] = shell_page_directory[1] | 7;    //Configurando atributos.
	
	//Entrada 2.
	//vga.
    //Criando a entrada n�mero 2 no diret�rio.
    shell_page_directory[2] = (unsigned long) &vga_page_table[0];
    shell_page_directory[2] = shell_page_directory[2] | 7;    //Configurando atributos.	
	
    //Entrada 768.
	//Criando a entrada de n�mero 768 do diret�rio.
    shell_page_directory[768] = (unsigned long) &km2_page_table[0];
    shell_page_directory[768] = shell_page_directory[768] | 3;    //Configurando atributos.		
	
	//Entrada 769.
	//LFB
    //Criando a entrada n�mero 769 do diret�rio.
    shell_page_directory[769] = (unsigned long) &lfb_page_table[0];
    shell_page_directory[769] = shell_page_directory[769] | 7;    //Configurando atributos.		
	
	//Entrada 770.
	//backbuffer.
    //Criando a entrada n�mero 770 do diret�rio.
    shell_page_directory[770] = (unsigned long) &buff_page_table[0];
    shell_page_directory[770] = shell_page_directory[770] | 7;    //Configurando atributos.		

	//
	// **** TASKMAN ****
	//
	
    //Iniciando o diret�rio taskman.
	for(i = 0; i < 1024; i++){
	    taskman_page_directory[i] = kernel_address | 2;    //0010 em bin�rio.
	};	

    //Entrada 0.
	//aproveitando a tabela criada antes.
    //Criando a primeira entrada do diret�rio da taskman.
	//Isso mapeia os primeiros 4MB da me�ria RAM.
    taskman_page_directory[0] = (unsigned long) &km_page_table[0];
    taskman_page_directory[0] = taskman_page_directory[0] | 3;    //Configurando atributos.
	
	//Entrada 1.
	//aproveitando outra tabela.
    //Criando a entrada n�mero 1 do diret�rio.
    taskman_page_directory[1] = (unsigned long) &um_page_table[0];
    taskman_page_directory[1] = taskman_page_directory[1] | 7;    //Configurando atributos.
	
	//Entrada 2.
	//vga.
    //Criando a entrada n�mero 2 no diret�rio.
    taskman_page_directory[2] = (unsigned long) &vga_page_table[0];
    taskman_page_directory[2] = taskman_page_directory[2] | 7;    //Configurando atributos.	
	
    //Entrada 768.
	//Criando a entrada de n�mero 768 do diret�rio.
    taskman_page_directory[768] = (unsigned long) &km2_page_table[0];
    taskman_page_directory[768] = taskman_page_directory[768] | 3;    //Configurando atributos.		
	
	//Entrada 769.
	//LFB
    //Criando a entrada n�mero 769 do diret�rio.
    taskman_page_directory[769] = (unsigned long) &lfb_page_table[0];
    taskman_page_directory[769] = taskman_page_directory[769] | 7;    //Configurando atributos.		
	
	//Entrada 770.
	//backbuffer.
    //Criando a entrada n�mero 770 do diret�rio.
    taskman_page_directory[770] = (unsigned long) &buff_page_table[0];
    taskman_page_directory[770] = taskman_page_directory[770] | 7;    //Configurando atributos.		
	
	//
	// O que foi feito:
	// ================
	//    Foram criados 4 diret�rios.
    //    kernel, idle, shell e taskman. 
    //    As tabelas configuradas para 
    //    o diret�rio kernel foram
    //    reaproveitadas nos demais diret�rios.
    //
	

	
// Done.
done:
	//printf("Done.\n");	 
    return (int) 0;
};

//
// End.
//

