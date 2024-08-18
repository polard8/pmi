/*
 * File: security/usession.c
 *
 * Descri��o:
 *     User Session.
 *     Faz parte do m�dulo Window Manager do tipo MB.
 *     Arquivo principal do gerenciador de user session. 
 *     Sec�es usu�rio.
 *
 * *importante:
 *     Quando um usu�rio loga, ele cria uma 'se��o', que conter� uma 
 * 'window station', que conter� um 'desktop', que conter� 'janelas'.
 *
 * 2015 - Created by Fred Nora.
 */


#include <kernel.h>


unsigned long usersession_start=0;

// usession.h
struct raw_graphics_d  *RawGraphics;
struct window_server_d *WindowServer;
struct window_manager_d *WindowManager;
struct desktop_environment_d *DesktopEnvironment;
struct usession_d *usession0;
struct usession_d *CurrentUserSession;
unsigned long usessionList[USER_SESSION_COUNT_MAX];


/*
 * get_current_user_session:
 *     Get the current user session handle.
 */

void *get_current_user_session (void)
{
    if ( current_usersession < 0 || 
         current_usersession >= USER_SESSION_COUNT_MAX )
    {
        return NULL;
    }

    return (void *) usessionList[current_usersession];
}

void 
set_current_user_session ( 
    struct usession_d *usession )
{

    if ( (void *) usession == NULL ){ 
        debug_print ("set_current_user_session: [FAIL] usession \n");
        return; 
    }
    if ( usession->sid < 0 ){
        debug_print ("set_current_user_session: [FAIL] sid \n");
        return; 
    }

    current_usersession = (int) usession->sid;
    CurrentUserSession = usession;
}

/*
 * CreateUserSection:
 *     Cria uma user section para um usu�rio v�lido.
 */

void *CreateUserSession (int user_id)
{

    struct usession_d *NewUserSession;
    int i=0;


	// Check limits.
    if ( user_id < 0 || user_id >= USER_COUNT_MAX )
    {
        debug_print ("CreateUserSession: [FAIL] user_id\n");
        return NULL;
    }


    //
    // User session structure.
    //

    // Create a new user section struct.

    NewUserSession = (void *) kmalloc ( sizeof(struct usession_d) );

    if ( (void *) NewUserSession == NULL ){
        panic ("CreateUserSection: [FAIL] NewUserSession\n");
    } else {

        NewUserSession->used  = (int) TRUE;
        NewUserSession->magic = (int) 1234;

        // user for this session.
        NewUserSession->uid  = (int) user_id;

        // Not initialized yet.
        NewUserSession->initialized = FALSE;

        // ...

        // #todo
        // CurrentUserSession = (void *) NewUserSession;
    };


    // We need an empty slot in the list usessionList[].
    // Coloca na lista em um lugar vazio.

    while ( i < USER_SESSION_COUNT_MAX )
    {
        if ( (void *) usessionList[i] == NULL )
        {
            // The structure pointer.
            usessionList[i] = (unsigned long) NewUserSession; 

            // User session id.
            NewUserSession->sid = i;

            return (void *) NewUserSession;
        }

        i++;
    };
    
    // #debug
    // hanging for debug.

    panic ("CreateUserSession: [DEBUG] Can't create!\n");
    return NULL;
}


// ??
// Open current user session.
void open_user_session (void){

    if ( (void *) CurrentUserSession == NULL )
    {
        printf ("open_user_session: [FAIL] CurrentUserSession\n");
        return;
    }

	// #todo: 
	// Criar tempo de in�cio de sess�o.
	// tempo de inicio de sess�o

    CurrentUserSession->BeginTime = (unsigned long) 0;

    // ...

    CurrentUserSession->initialized = TRUE; 
}


// ??
// Close current user session.
void close_user_session (void)
{

    if ( (void *) CurrentUserSession == NULL )
    {
        printf ("close_user_session: [FAIL] CurrentUserSession \n");
        return;
    }

	// #todo: 
	// Criar tempo de fim de sess�o.
	// tempo de fim de sess�o

    CurrentUserSession->EndTime = (unsigned long) 0;


    CurrentUserSession->sid   = 0; 

    CurrentUserSession->used  = FALSE; 
    CurrentUserSession->magic = 0; 

    CurrentUserSession->uid = 0; 

    // ...

    CurrentUserSession->initialized = FALSE; 
}



/*
 *****************************
 * init_user_session:
 * 
 *     Inicializa user session support.
 */

void init_user_session (void){

    uid_t uid = -1;

    int i = 0;


    debug_print ("init_user_session:\n");


    //  Clear the list of user sessions.

    for (i=0; i<USER_SESSION_COUNT_MAX; i++)
    {
        usessionList[i] = (unsigned long) 0;
    };


    //
    // User
    //

    // We need a user to create a user session.

    uid = (int) GetCurrentUserId();

    if ( uid < 0 || uid >= USER_COUNT_MAX )
    {
        panic ("init_user_session: uid\n");
    }


	//
	// User Session.
	//
	
	//Struct.
	
    usession0 = (void *) CreateUserSession (uid);

    if ( (void *) usession0 == NULL ){
        panic ("init_user_session: [FAIL] usession0\n");
    }

    // Set current user session.
    set_current_user_session(usession0);

    //Open
    open_user_session();

    // ...

    // Initialize this session.
    usession0->initialized = TRUE;
}


//
// End.
//

