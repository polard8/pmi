/*
 * File: security/room.c
 *
 * Descrição:
 *     Window Station Manager. (WSM).
 *     "Desktop Pool" Manager. (DPM). 
 *     Rotinas para gerenciamento de window stations.
 *     Faz parte do módulo Window Manager do tipo MB.
 *
 * Obs:
 *     Os aplicativos poderão acessar esses serviços em user mode.
 *     Atraves de um subsistema em user mode que gerencia janelas.
 *
 *    2015 - Created by Fred Nora.
 */


#include <kernel.h>

struct room_d *room0;
struct room_d *CurrentRoom;
//...
unsigned long roomList[ROOM_COUNT_MAX];


/*
 * get_current_windowstation:
 */

void *get_current_room (void)
{

    if ( current_room < 0 ){ return NULL; }

    return (void *) roomList[current_room];
}


/*
 * set_current_room:
 *
 */

void set_current_room (struct room_d *room)
{
    if ( (void *) room == NULL ){ return; }

    current_room = (int) room->id;
    CurrentRoom = room;
}


/*
 * CreateWindowStation:
 *     Cria uma window station em uma seção de usuário.
 */

// Create a room given an empty object.

void *CreateRoom (struct room_d *room)
{
    int i=0;

	struct room_d *Current;    
    struct room_d *Empty;	
	
	//User Session Struct.
    
	if ( (void *) room == NULL )
	{
	    return NULL;
	}
	
	//Window Station Struct.
    
	Current = (void *) kmalloc ( sizeof(struct room_d) );

    if ( (void *) Current == NULL )
    {
        panic ("CreateWindowStation: Current");
    } else {
		
	    //room.
	    Current->room = (struct room_d *) room;
		
		//continua...
	};
	
	
	while ( i < ROOM_COUNT_MAX )
	{	
        Empty = (void *) roomList[i]; 			
        
		if ( (void *) Empty == NULL )
		{
		    roomList[i] = (unsigned long) Current;
		    Current->id = i;    	
		    
			//set_current_room(Current);	
			
			return (void *) Current;
		};	
		
		i++;
    };

    return NULL;    
}


/*
 * RegisterWindowStation:
 *     Registrando uma windowstation numa lista de window stations.
 */

int RegisterRoom (struct room_d *room)
{
    int i=0;


    if ( (void *) room == NULL )
    {
        debug_print ("RegisterRoom: [FAIL] room\n");
        return (int) 1;    
    }


    while (i < ROOM_COUNT_MAX)
    {
        if ( (void *) roomList[i] == NULL )
        {
            roomList[i] = (unsigned long) room;

            return 0;
        }

        i++;
    };

    return (int) 1;
}


/*
 * init_room_list:
 */

void init_room_list (void)
{
    int i=0;
    while ( i < ROOM_COUNT_MAX )
    {
        roomList[i] = 0;
        i++; 
    };
}


/*
 ********************************
 * init_room_manager:
 *     Inicializa o gerenciamento de room. (window stations).
 */

void init_room_manager (void)
{

    debug_print ("init_room_manager:\n");
    //printf("init_window_station: Initializing ...\n");

    rooms_count = 0;

    // List.
    init_room_list();

    // #todo
    // Where is it defined?

    room0 = (void *) kmalloc ( sizeof(struct room_d) );

    if ( (void *) room0 == NULL ){
        panic ("init_room_manager: [FAIL] room0\n");
    }else{

        // #todo
        // Initialize some elements?

        roomList[0] = (unsigned long) room0;
        //RegisterRoom (room0);
        set_current_room (room0);   
    };
}


//
// End.
//

