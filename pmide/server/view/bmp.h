// bmp.h
// BMP support for the 32bit display server.
// 2015 - Created by Fred Nora.

#ifndef __DS_BMP_H
#define __DS_BMP_H    1

/* "MB" */ 
#define BMP_TYPE 0x4D42        
 
#define  BMP_CHANGE_COLOR_NULL  0
#define  BMP_CHANGE_COLOR_TRANSPARENT  1000
#define  BMP_CHANGE_COLOR_SUBSTITUTE   2000
//...
  
 
//flag que avisa que dve haver alguma mudan�a nas cores. 
extern int bmp_change_color_flag; 
//salva-se aqui uma cor para substituir outra. 
extern unsigned long bmp_substitute_color; 
//cor selecionada para ser substitu�da ou ignorada. 
extern unsigned long bmp_selected_color; 


//
// ## BMP support ##
//

#define GWS_BMP_TYPE 0x4D42             /* "MB" */

//OFFSETS
#define GWS_BMP_OFFSET_WIDTH 18
#define GWS_BMP_OFFSET_HEIGHT 22
#define GWS_BMP_OFFSET_BITPLANES 26
#define GWS_BMP_OFFSET_BITCOUNT 28
//...

//typedef struct bmp_header_d bmp_header_t;   
struct gws_bmp_header_d                     
{
    unsigned short bmpType;           /* Magic number for file */
    unsigned long  bmpSize;           /* Size of file */
    unsigned short bmpReserved1;      /* Reserved */
    unsigned short bmpReserved2;      /* ... */
    unsigned long  bmpOffBits;        /* Offset to bitmap data */
};

//typedef struct bmp_infoheader_d bmp_infoheader_t;   
struct gws_bmp_infoheader_d                     
{
    unsigned long  bmpSize;           /* Size of info header */
    unsigned long  bmpWidth;          /* Width of image */
    unsigned long  bmpHeight;         /* Height of image */
    unsigned short bmpPlanes;         /* Number of color planes */
    unsigned short bmpBitCount;       /* Number of bits per pixel */
    unsigned long  bmpCompression;    /* Type of compression to use */
    unsigned long  bmpSizeImage;      /* Size of image data */
    unsigned long  bmpXPelsPerMeter;  /* X pixels per meter */
    unsigned long  bmpYPelsPerMeter;  /* Y pixels per meter */
    unsigned long  bmpClrUsed;        /* Number of colors used */
    unsigned long  bmpClrImportant;   /* Number of important colors */
};
 


//
// Pointer.
//

extern char *gwssrv_bmp_cursor_pointer;
extern char *gwssrv_bmp_mouse_pointer;

// ======================================


/*
 * bmpDisplayBMP:
 * Mostra na tela uma imagem bmp carregada na mem�ria. no backbuffer()
 * IN:
 *     address = endere�o base
 *     x       = posicionamento 
 *     y       = posicionamento
 */
 
int 
bmpDisplayBMP ( 
    char *address, 
    unsigned long x, 
    unsigned long y );

void *gwssrv_get_system_icon(int n);

//void __test_load_bmp2(void);
int gwssrv_display_system_icon ( int index, unsigned long x, unsigned long y );

#endif  

//
// End.
//


