
#ifndef __DS_PACKET_H
#define __DS_PACKET_H    1

 // Socket packets.
 // Usados na troca de mensgens via socket.
 
// # Podemos usar esse esquema em todos os servidores.
// então isso deverá ir para bibliotecas.
// Isso foi baseado nos tipos de pacotes usados pelo
// x window server.
// See: https://en.wikipedia.org/wiki/X_Window_System_protocols_and_architecture

/*
    Request: 
        the client requests information from the server or 
        requests it to perform an action.
    Reply: 
        the server responds to a request. 
        Not all requests generate replies.
    Event: 
        the server sends an event to the client, 
        e.g., keyboard or mouse input, or 
        a window being moved, resized or exposed.
    Error: 
    the server sends an error packet if a request is invalid. 
    Since requests are queued, error packets generated by a 
    request may not be sent immediately. 
 */

#define SERVER_PACKET_TYPE_REQUEST    1000 
#define SERVER_PACKET_TYPE_REPLY      1001 
#define SERVER_PACKET_TYPE_EVENT      1002
#define SERVER_PACKET_TYPE_ERROR      1003
#define SERVER_PACKET_TYPE_DISCONNECT 1004


#endif  


