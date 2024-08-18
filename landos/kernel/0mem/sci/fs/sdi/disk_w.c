/*
 * File: fs/write.c 
 *
 *     Low level routines to write data into the disk.
 *
 * History:
 *    2015 - Created by Fred Nora.
 *    ...
 */


#include <kernel.h>


/*
 * fatWriteCluster:
 *     Salva um cluster no disco.
 *
 * Argumentos:
 *   setor   ~ Primeiro setor do cluster.
 *   address ~ Endere�o do primeiro setor do cluster.
 *   spc     ~ N�mero de setores por cluster.
 */

	//Come�a do primeiro setor do cluster.

//int fatWriteCluster ( unsigned long sector, 
//                      unsigned long address, 
//                      int spc )


// #todo
// Return 'int'.

void 
fatWriteCluster ( 
    unsigned long sector, 
    unsigned long address, 
    unsigned long spc )
{
    unsigned long i=0;

    // #todo
    // We need some limits here for now.

    if (spc == 0){
        debug_print("fatWriteCluster: spc\n");
        return;
    }

    for ( i=0; i < spc; i++ )
    {
        write_lba ( address, (sector + i) );
        address = (address +512); 
    };

    //...

    //return 0;  //#todo
}


/*
 *************************************************
 * write_lba:
 *     Grava um setor no disco dado o endere�o do buffer e o lba. 
 */


// #bugbug
// Essa rotina e' independente do sistema de arquivos.

//int write_lba ( unsigned long address, unsigned long lba ) 

void write_lba ( unsigned long address, unsigned long lba )
{
	// #todo: 
	// Check lba limits.

    if (address == 0){
        debug_print ("write_lba: [FAIL] Limits\n");
        goto fail;
    }

    // See: volume.h
    switch (g_currentvolume_fatbits){

        case 32:
            printf ("write_lba: [ERROR] FAT32 not supported\n");
            goto fail;
            break;

        //See: hdd.c
        case 16:
            my_write_hd_sector ( address, lba, 0, 0 ); 
            return;
            break;

        case 12:
            printf ("write_lba: [ERROR] FAT12 not supported\n");
            goto fail;
            break;

        default:
            printf ("write_lba: [ERROR] g_currentvolume_fatbits NOT SUPPORTED\n");
            goto fail;
            break;
    };

    // Nothing.

fail:
    refresh_screen();
    return;
}


/*
int 
write_clusters ( 
    int volume_number,  // partition index in a table.
    char *buffer, 
    unsigned long cluster_number, 
    unsigned long count );
int 
write_clusters ( 
    int volume_number,  // partition index in a table.
    char *buffer, 
    unsigned long cluster_number, 
    unsigned long count )  // how many
{
    struct volume_d *v;
    int i=0;
    int Max=0;
    
    v = (struct volume_d *) volumeList[volume_number];

    if ( (void*)v == NULL )
        return -1;
    
    i=0;
    Max=count;
    while (i<Max)
    {
        // write ...
        
        i++;
    };
    
    return 0;
}
*/



/*
 ***************************************************
 * fs_save_rootdir:
 *     Salva o diret�rio raiz no disco.
 *     @todo: Identificar par�metros do sistema de arquivos atual. 
 */

int 
fs_save_rootdir ( 
    unsigned long root_address, 
    unsigned long root_lba, 
    size_t root_size )
{

    // #todo
    // Descrever essas vari�veis.
    
    int r=0;
    int roff=0;
    int rlbaoff=0;

    unsigned long RootAddress = 0;
    unsigned long RootLBA     = 0;
    size_t        RootSize    = 0;


    RootAddress = root_address;
    RootLBA     = root_lba;
    RootSize    = root_size;    // number of sectors.

    // size = number of sectors
    // 512*32
    // 512 entradas de 32 bytes.

    // #obs:
    // N�o precisamos fazer isso o tempo todo.
    // Podemos apenas sinalizar que a sincroniza��o est� pendente.
    
    debug_print ("fs_save_rootdir:\n");
    printf ("Saving root..\n");
    refresh_screen ();


	// #bugbug: 
	// Precisamos saber o tamanho do root ... 
	// Precismos de estrututra de root

    // size = number of sectors
    // 512*32
    // 512 entradas de 32 bytes.
    
    //for ( r=0; r<32; r++ )
    for ( r=0; r< RootSize; r++ )
    {
        // #debug
        //printf("write_lba n={%d} \n",r); 
        //refresh_screen();

        // Wait interrupt. Isso funcionou.
        // #bugbug
        // ?? Estamos esperando antes de gravarmos o pr�ximo.
        // wait irq
        disk_ata_wait_irq ();

        // #bugbug: 
        // N�o podemos determinar os valores. Precisamos de estruturas.

        //my_write_hd_sector ( 
        //    (unsigned long) ( VOLUME1_ROOTDIR_ADDRESS + roff), 
        //    (unsigned long) ( VOLUME1_ROOTDIR_LBA     + rlbaoff ), 
        //    0, 
        //    0 );

        my_write_hd_sector ( 
            (unsigned long) ( RootAddress + roff), 
            (unsigned long) ( RootLBA + rlbaoff ), 
            0, 
            0 );

        roff    = (roff    + 0x200);
        rlbaoff = (rlbaoff + 1);
        
        // ?? esperar
    };

    return 0;
}


// Save fat into the disk.
int 
fs_save_fat ( 
    unsigned long fat_address, 
    unsigned long fat_lba, 
    size_t fat_size )
{

    // #todo
    // Descrever essas vari�veis.

    int f=0;
    int off=0;
    int lbaoff=0;

    unsigned long __fatAddress = 0;
    unsigned long __fatLBA     = 0;
    size_t        __fatSize    = 0;


    __fatAddress = fat_address;
    __fatLBA     = fat_lba;
    __fatSize    = fat_size;   //fat size in sectors. 246?
    
    
    //#bugbug: provisorio
    debug_print ("fs_save_fat:\n");
    printf ("Saving fat..\n");
    refresh_screen ();



	// #bugbug: 
	// Precisamos saber o tamanho da fat ... 
	// Precismos de estrututra de fat
	//#obs: 
	// Estamos salvando 246 setores da FAT,


    //for ( f=0; f<246; f++ )
    for ( f=0; f<__fatSize; f++ )
    {
 
       //#debug
       //printf("write_lba n={%d} \n",f);  
       //refresh_screen();

        // Esperando antes do pr�ximo.
        disk_ata_wait_irq ();


        //my_write_hd_sector ( 
        //    (unsigned long) ( VOLUME1_FAT_ADDRESS + off), 
        //    (unsigned long) ( VOLUME1_FAT_LBA     + lbaoff ), 
        //    0, 
        //    0 );

        my_write_hd_sector ( 
            (unsigned long) ( __fatAddress + off), 
            (unsigned long) ( __fatLBA     + lbaoff ), 
            0, 
            0 );


        off    = (off    + 0x200);
        lbaoff = (lbaoff + 1);
        
        // esperar ??
    };


    // #debug
    debug_print ("fs_save_fat: Done\n");
    printf      ("fs_save_fat: Done\n"); 

    refresh_screen ();

    return 0;
}


//
// End.
//

