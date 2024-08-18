# pmi - Protected Mode Interface for FN-DOS.

This repository has the source code for:

> * Gramado boot loader
> * Gramado kernel
> * Gramado Window Server
> * Applications
> * Commands

## pmi is based on Gramado 32bit.

  Gramado 32bit

```
  This is the last 32bit version of Gramado OS.
  Codename = (V 1.1 241)
  Date = June 6, 2021.
```

------------------------------------
Goal:

```
  This is gonna be a 32bit interface for the FN-DOS (16bit OS) project.
  A kind of 32bit DMPI for the 16bit FN-DOS.
```


## What is Gramado?

    Gramado (c) is a 32 bit multithreaded operating system.

## Kernel features.

> * Paging.
> * Threads.
> * FAT16 file system.
> * ps/2 keyboard. 
> * ps/2 mouse works fine only on qemu.
> * serial port. 
> * IDE PATA. 
> * e1000 NIC.

## Userland features.

> * Working on a Window Server. (gws)
> * Working on a virtual terminal. 
> * unix-like commands running in the virtual console.

## Libraries.

> * unix-like library. (rtl)
> * API for setup environment. (libcore)
> * Lib for the window server's clients. (libgws)

## Fred's compilation on Linux.
```
    Linux 5.4.0-42-generic x86_64
    gcc (Ubuntu) 7.5.0
    GNU ld (GNU Binutils for Ubuntu) 2.30
    NASM version 2.13.02
```
## How to Build?

> See the [Gramado OS build instructions](https://github.com/frednora/gramado/blob/master/docs/build.md)

## Testing

```
	$ ./run
	$ make qemu-test
```

You can run the VHD on Oracle Virtual Box and qemu.
Please, give us some feedback.

## Documentation

> See the [docs](https://github.com/frednora/gramado/tree/master/docs/).
> See the [DOCS](https://github.com/frednora/gramado/blob/master/base/GRAMADO/DOCS/).

## More information

Gramado is a Free and Open Source operating system.
The source code uses the BSD license.

# Quotes:
> Oh boy, there is no spoon!

