// inode.h
// inode support.
// it's a test yet.
// See: https://en.wikipedia.org/wiki/Inode

#ifndef __INODE_H
#define __INODE_H    1

struct inode_d
{
    int id;

    int used;
    int magic;
    
    // Pathname para o arquivo.
    // lembrando que na estrutura de fat teremos apenas
    // o nome do arquivo, talvez um lfn.
    char path[256];

    // Quantas estruturas de arquivo estão apontando para esse
    // mesmo inode.
    int filestruct_counter;

    uid_t uid;  // User 
    gid_t gid;  // Group

    // File, directory, device, symbolic link, socket, pipe ...
    int ____object;

    // Contador de hardlinks.
    int hardlink_counter;

    // ...
    
    // fat16 support.
    int isfat16;
    struct fat16_directory_entry_d fat16_dirent;

    // Onde foi carregado o arquivo ou diretorio.
    void *buffer;

    // Isso já existe na fat16 entry ... mas talvez seja útil
    // quando for outro tipo de arquivo.
    unsigned long size_in_bytes;

    //
    // Blocks
    //
    
    // #todo
    // Precisamos fazer uma lista com os clusters
    // usados por esse arquivo.
    // Talvez fazermos uma lista que indique quais 
    // setores da fat precisa ser salvos no disco
    // quando salvarmos o arquivo.

    //ext2
    
    struct inode_d *next;
};

// fs.c
extern unsigned long inode_table[32];


//
// prototypes ========
//

// #bugbug
// Not tested yet.
// See: search.c
int search_path_in_the_inode_table( const char *path );

//OUT: inode structure.
//See: fs.c
struct inode_d *file_inode (file *f);

void fs_show_inode_info (struct inode_d *i);
void fs_show_inode_table(void);

int get_free_slots_in_the_inode_table(void);

#endif    



//
// End.
//




