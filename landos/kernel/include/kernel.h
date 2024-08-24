// kernel.h
// This is the main header for the base kernel of pmi interface.
// 2013 - Created by Fred Nora.

//
// == gramado modes =================================================
//

// See: 'current_mode'.
#define GRAMADO_JAIL        0x00
#define GRAMADO_P1          0x01
#define GRAMADO_HOME        0x02
#define GRAMADO_P2          0x03
#define GRAMADO_CASTLE      0x04
#define GRAMADO_CALIFORNIA  0x05
// ...

//
// IO Control
//

// #todo
// include/user/ is a better place for this structure.
// But it needs to a global thing.

// This is the idea:
// Instead of selecting the io model,
// we will select the components that will be used in the input system.

struct io_control_d
{
    int initialized;
    int useTTY;
    int useEventQueue;
};
// main.c
extern struct io_control_d  IOControl;

//
// == input modes ==================================================
//

// See: 'current_input_mode'.


// ================
// fred:
// Talvez o bom mesmo seja usarmos apenas o input mode tty.
// Nesse caso para todos os tipos de aplicativo eles precisaram ler
// a tty para pegar o input no seu modo raw.
// Todo esse trabalho so existe porque o kernel esta construindo
// o evento de input ao inves de mandar o input no formato raw.
// ================


// Setup input mode.
// Used by applications using the kgws window server.
// This is the mode used when we do not have a 
// loadble ring3 window server.
// Send the input event to the thread associated with the
// window with focus in the window server embedded inside the kernel.
#define INPUT_MODE_EVENTS  1000
#define INPUT_MODE_SETUP   INPUT_MODE_EVENTS

// tty input mode.
// In this mode we're gonna send the input to the tty buffer.
// This way a virtual terminal can share the input with its client.
#define INPUT_MODE_TTY    2000

// ws input mode.
// Used for the applications running on an environment
// with a loadable window server. Just like gws window server.
#define INPUT_MODE_WS     3000

// ...




//
// Externs.
//

//Stack for fome threads in kernel mode. 
extern unsigned long task0_stack;
extern unsigned long task1_stack;
extern unsigned long task2_stack;
extern unsigned long task3_stack;
extern unsigned long stack0_krn_ptr;
// ...


//#test 
//typedef int (*fn_ptr)();


// =========================================================

// (NT)
// Land OS headers.


// config/
#include "config/version.h"    // Product. 
#include "config/u.h"          // User
#include "config/config.h"     // Compiling.

// globals
#include "limits.h"
#include "gpa.h"
#include "gva.h"
#include "gentry.h"
#include "gdef.h"
#include "gdevice.h"
#include "gobject.h"
#include "gspin.h"
#include "ginput.h"  // input manager support.
#include "gwd.h"     // whatch dogs


// rtl 
// Inside the base kernel
#include "rtl/errno.h"
#include "rtl/cdefs.h"
#include "rtl/stddef.h"
#include "rtl/types.h"
#include "rtl/types2.h"
#include "rtl/limits.h"
#include "rtl/kstdio.h"
#include "rtl/stdlib.h"
#include "rtl/string.h"
#include "rtl/ctype.h"
#include "rtl/iso646.h"
#include "rtl/signal.h"
#include "rtl/unistd.h"
#include "rtl/fcntl.h"
#include "rtl/ioctl.h"
#include "rtl/ioctls.h"
#include "rtl/termios.h"
#include "rtl/ttydef.h"

// memory management support.
#include "land/mm/mm.h"
#include "land/mm/memmap.h" 
#include "land/mm/intelmm.h"

// rawgr -
// #view presentation output.
#include "user/view/display.h"
#include "user/view/screen.h"
#include "user/view/video.h"

// Render
#include "user/render/r_render.h"

#include "user/model/fonts.h"
#include "user/model/ascii.h" 

// fs
#include "rtl/fs/path.h"       // path.
#include "rtl/fs/fat.h"        // fat16.
#include "rtl/fs/fat_lfn.h"    // fat16 lfn.
#include "rtl/fs/inode.h"
#include "rtl/fs/exec_elf.h"
#include "rtl/fs/pipe.h" 
#include "rtl/fs/fs.h"
#include "rtl/fs/vfs.h"


// view - input and presentation
#include "user/view/prompt.h"

// view - showing the data.
#include "user/view/bmp2.h"

// hal
#include "../1pump/arch/x86/portsx86.h"
#include "hal/halvid.h"
#include "hal/halmm.h"
#include "hal/cpu.h"
#include "../1pump/arch/x86/pte.h"
#include "../1pump/arch/x86/tss.h" 
#include "hal/cputypes.h"
#include "../1pump/arch/x86/x86pte.h"
#include "../1pump/arch/x86/x86.h"
#include "../1pump/arch/x86/x86iv.h"
#include "../1pump/arch/x86/x86gdt.h"
#include "hal/cpuamd.h"
#include "hal/dmi.h"
#include "hal/detect.h"
#include "hal/dev/tty/serial.h"

//todo: 
// podemos mover isso de lugar para perto de net/
#include "rtl/net/mac.h"


// hal/dev/blkdev
#include "hal/dev/blkdev/floppy.h"
#include "hal/dev/blkdev/ata.h"
#include "hal/dev/blkdev/super.h"
#include "hal/dev/blkdev/volume.h"
#include "hal/dev/blkdev/disk.h"  
#include "hal/dev/blkdev/storage.h" 

// hal
#include "../1pump/arch/x86/pit.h"
#include "../1pump/arch/x86/pic.h"
#include "../1pump/arch/x86/apic.h"
#include "hal/cpuid.h"
#include "../1pump/arch/x86/rtc.h"

// rawgr - window server
// inputs
#include "user/view/xproc.h"
#include "user/view/keyboard.h"
#include "user/view/mouse.h"
#include "user/view/vk.h"
#include "user/view/kbdabnt2.h"

// #view. drawing routines.
#include "user/view/ldisc.h"

//storage (^)
#include "hal/dev/blkdev/ide.h"

//pci
#include "../2io/bus/pcibus/pci.h"

//storage (^)
#include "hal/dev/blkdev/ahci.h"
#include "hal/dev/blkdev/sata.h"

#include "../2io/bus/usbbus/usb.h"

#include "hal/up.h"
#include "hal/mp.h"
#include "hal/breaker.h"
#include "hal/hal.h"


// ps
#include "land/ps/x86cont.h"
#include "land/ps/ts.h"
#include "land/ps/tasks.h"
#include "land/ps/image.h"
#include "land/ps/clone.h"
#include "land/ps/process.h"
#include "land/ps/thread.h"
#include "land/ps/sched.h"
#include "land/ps/ipc.h"
#include "land/ps/ipccore.h"
#include "land/ps/sem.h"
#include "land/ps/queue.h"
#include "land/ps/realtime.h"
#include "land/ps/dispatch.h"
#include "land/ps/event.h"
#include "land/ps/ps.h"
#include "land/ps/mk.h"


// rawgr - 
// model business logic
#include "user/kgwm.h"
#include "user/ws.h"

// #view - designers
#include "user/view/window.h"
#include "user/view/menu.h"
#include "user/view/pbar.h"
#include "user/view/bmp.h"
#include "user/view/line.h"

// #view. presentation
#include "user/terminal.h"
#include "user/view/guiconf.h"


//
// == Security ===============================
//

#include "user/security/usession.h"
#include "user/security/room.h"
#include "user/security/desktop.h"
#include "user/security/user.h"
#include "user/security/security.h"


// rawgr - window server, and beyond

#include "user/logon.h"
#include "user/logoff.h"
#include "user/kgws.h"

// tty
#include "hal/dev/tty/ttyldisc.h"
#include "hal/dev/tty/ttydrv.h"
#include "hal/dev/tty/tty.h"
#include "hal/dev/tty/pty.h"

#include "user/vt.h"
#include "user/console.h"

// =============================
// rawgr - i8042 ps2 controller.
#include "user/view/i8042/i8042.h"
#include "user/view/i8042/ps2mouse.h"
#include "user/view/i8042/ps2kbd.h"
#include "user/view/i8042/ps2.h"

//
//  ==== NETWORK ====
//

#include "rtl/net/connect.h" 
#include "rtl/net/host.h"
#include "rtl/net/ethernet.h"
#include "rtl/net/arp.h"
#include "rtl/net/udp.h"
#include "rtl/net/tcp.h"

// intel nic - network interface controller.
#include "../2io/dev/netdev/e1000/nicintel.h"

#include "rtl/net/in.h"
#include "rtl/net/un.h"
#include "rtl/net/nports.h"     //(network) Network Ports  (sw)
#include "rtl/net/ip.h"         //(network) IP info.      (sw)
#include "rtl/net/ipv4.h" 
//#include "rtl/net/ipv6.h" 
#include "rtl/net/ipv4mac.h" 
#include "rtl/net/icmp.h" 
#include "rtl/net/packet.h"      // network packets.
#include "rtl/net/channel.h"     //(network) Channel       (sw)
#include "rtl/net/client.h"      //(network) Client process support. 
#include "rtl/net/ns.h"          //(network) Network Server.
#include "rtl/net/network.h"     //(network) Gerenciamento de rede.  
#include "rtl/net/socket.h"      //last always


//
// ==== DEVICES ====
//


// devices
#include "../2io/dev/devices.h"
#include "../2io/dev/devmgr.h"      
#include "../2io/io.h"               //io.

#include "land/modules.h"     //module manager.
#include "land/debug.h"
#include "land/system.h"    //system manager.
#include "land/init.h"
#include "land/execve.h"  
// mm
#include "land/mm/mmglobal.h"  // Deve ficar mais acima.
#include "land/mm/heap.h"      // Heap pointer support.
#include "land/mm/aspace.h"    // Address Space, (data base account).

// dspace
#include "hal/dev/blkdev/dspace.h"    // Disk Space, (data base account). storage stuff

// mm
#include "land/mm/bank.h"      // Bank. database
#include "land/mm/x86mm.h"     // mm, memory manager support.

// rawgr - 
// view. input support.
#include "user/view/cursor.h"

#include "land/messages.h"
#include "land/events.h"
#include "land/object.h"
#include "land/ss.h"

// profiler
#include "land/pints.h"

#include "rtl/runtime.h"

// kernel
#include "land/ke.h"
#include "land/ki.h"
#include "land/info.h"

#include "../0mem/request.h"
#include "../0mem/reboot.h"

#include "land/sub.h"       // gramado sub systems
#include "land/utsname.h"
#include "land/gpid.h"      // Globals. PIDs support.


// sci - system call interface
// All the functions in this folder can be called
// by the ring3 apps via system call.
// This is the last #include. :^)

#include "rtl/sci/syscall.h"        
#include "rtl/sci/sys.h"

// si - services interface
#include "rtl/si/sifs.h"
#include "rtl/si/sins.h"
#include "rtl/si/siws.h"


// Moving some stuff from here to kernel00.h
#include "kernel00.h"

//
// End
//

