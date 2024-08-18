/*
 * File: limits.h 
 * 
 *     It defines some limits in the libc.
 *     ring 0.
 * 
 *     2005 - Created by Fred Nora.
 */


#ifndef ____LIMITS_H
#define  ____LIMITS_H

// Max number of open files in a process.
#define OPEN_MAX 32   

// Number of bits in a 'char'.  
#define CHAR_BIT  8

// Minimum and maximum values a 'signed char' can hold.  
#define SCHAR_MIN  (-128)
#define SCHAR_MAX  127    

// Maximum value an 'unsigned char' can hold.  (Minimum is 0.)  
#define UCHAR_MAX  255

// Minimum and maximum values a 'signed short int' can hold.  
#define SHRT_MIN  (-32768)
#define SHRT_MAX  32767
 
// Maximum value an 'unsigned short int' can hold.  (Minimum is 0.) 
#define USHRT_MAX  65535

#endif    





