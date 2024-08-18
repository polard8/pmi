/*
 * File: shell.h
 *
 * Descri��o:
 *     Header para o mini-shell dentro do Boot Loader 32bit.
 *
 * @todo: 
 *     Deixar o minishell enxuto, com poucos recursos.
 *     Deixar principalmente os recursos de configura��o 
 *     da inicializa��o.
 *
 * Vers�o 1.0, 2015 - Created.
 * Vers�o 1.0, 2016 - Revision.
 */


// 
//Command support.
//

//Prompt.
char prompt[250];             
unsigned long prompt_pos;
unsigned long g_cmd_status;    //Quando o comando terminou. (string completa).


//
//Shell.
//

//Main.
int shellMain( int argc, char* argv );
			   
//Initialization.
void shellInit();
int shellInitializePrompt();
//...

void shellWaitCmd();
unsigned long shellCompare();
void shellHelp();
//...

//
//End.
//

