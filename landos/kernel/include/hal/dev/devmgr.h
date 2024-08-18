/*
 * File: devmgr.h  
 *
 * Descrição:
 *     Header para o device manager.
 *     Para gerenciar e listar dispositivos.
 *
 *     Usado para dispositivos que não sejam pci
 * 
 *
 * Obs: Introduzindo aqui o conceito de 'Light' e 'Dark'.
 * O termo Light pode ser usado para descrever os dispositivos
 * mais rápidos, principalmente os que ficam mais pertos do processador,
 * tais como CPU, RAM, VIDEO MEMORY, PLACA DE REDE. Os outros disposisitivos
 * seriam classificados como Dark. Apesar de ser extremamente importante
 * que o disco do sistema seja rápido, ele ficaria sendo classificado como Dark
 * por estar longe do processador. Não há problema se no grupo dos dispositivos Dark
 * pararece algum que apresente bom desempenho, pois é apenas uma forma de se eleger
 * prioridades.
 *
 *     ... 
 */

#ifndef ____DEVMGR_H
#define ____DEVMGR_H


// struct more complete, with a lot of information.
struct device_class_d 
{
    //object_type_t  objectType;
    //object_class_t objectClass;
    
    int device_class;
    int device_subclass;    
};


// Device structure.
struct device_d 
{
    //
    // == Identification =============
    //

    object_type_t  objectType;
    object_class_t objectClass;

    int index;
    int used;
    int magic;

    //name for pci devices: "/DEV_8086_8086"  
    char name[64];
    size_t Name_len;    

    // #todo: merge.
    // char, block, network
    int __class;
    // struct more complete, with a lot of information.
    struct device_class_d *_class; 

    // pci, legacy ...
    int type;

    //
    // == (1) storage ========
    //

    // object? or buffer ?
    file *__file;  //??

    // #importante
    // Isso deve ser um pathname 
    // do mesmo tipo usado no sistema de arquivos.
    // /dev/tty0
    char *mount_point;
    
    // Se o tipo for pci.
    struct pci_device_d *pci_device;
    
    // se o dispositivo for do tipo legado,
    // como PIC, PIT, ps2, etc ...
    // Qualquer coisa que não esteja na interface pci.
    //struct legacy_device_d *legacy_device;

    //#todo: 
    //estruturas para outros grupos de dispositivos.

    //?? why light - suspenso.
    //Se o dispositivo petence ao grupo dos prioritários.
    //int Light;


    //Fila de dispositivos que está esperando
    //por esse dispositivo.
    //Na verdade podemos usar uma lista linkada ou outro recurso.
    //de gerenciamento.
    //int pid;
    //unsigned long queue[8];
    //struct process_d *list;


    // Driver.
    // ?? Talvez pudesse ser 'device driver' e não 'tty driver'
    // mas está bom assim.
    struct ttydrv_d *ttydrv;
    
    //
    // == (1) storage ========
    //

    
    struct tty_d *tty;

    //
    // == (2) synchronization ========
    //

    //int stopped;

    //
    // == (3) transmition ========
    //

	//
	// Continua ...
	//

    // maybe not.
    //struct device_d *next;
};

extern struct device_d  *devices;  //?? What

// List of legacy devices.
// 'file->device'
extern struct device_d  *PS2KeyboardDevice;
extern struct device_d  *PS2MouseDevice;
// ...

// #todo: 
// Parece uma lista muito grande para o número de dispositivos.
// mas se estamos falando de dispositivos PCI a lista é grande mesmo.

// #importante
// O número de dispositivos será o mesmo número de arquivos
// na lista Streams.
// Se o arquivo for um dispositivo então teremos
// um ponteiro na lista deviceList.

//
// The list.
//

#define DEVICE_LIST_MAX 512   //NUMBER_OF_FILES

extern unsigned long deviceList[DEVICE_LIST_MAX];    

//
// == Objects ======================
//

// #bugbug
// Temos um header no lugar certo para isso?

extern file *PS2KeyboardDeviceObject;
extern file *PS2MouseDeviceObject;
// ...

//
// == ttys ======================
//

// #bugbug
// Temos um header no lugar certo para isso?

extern struct tty_d *PS2KeyboardDeviceTTY;
extern struct tty_d *PS2MouseDeviceTTY;
// ...

//
// == tty drivers ======================
//

// #bugbug
// Temos um header no lugar certo para isso?

extern struct ttydrv_d *PS2KeyboardDeviceTTYDriver;
extern struct ttydrv_d *PS2MouseDeviceTTYDriver;
// ...


//
// == prototypes ========================================
//


int devmgr_init_device_list(void);

struct device_d *devmgr_device_object (void);


int 
devmgr_register_device ( 
    file *f, 
    char *name,
    int class,     // #bugbug Mudar esse nome de argumento.
    int type,
    struct pci_device_d *pci_device,
    struct ttydrv_d *tty_driver );


void devmgr_show_device_list(void);

void init_device_manager (void);


#endif    


//
// End.
//

