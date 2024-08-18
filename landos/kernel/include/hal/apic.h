/*
 * File: apic.h
 *
 * Descrição:
 *     Header para o driver do controlador APIC.
 *     Advanced Programmable Interrupt Controller
 *
 *     Exemplo:
 *     82489DX.
 *     82093AA for PIIX3-based systems.
 *         
 * Variantes:
 *    xAPIC, x2APIC.
 *
 * 2015 - Created by Fred Nora.
 */

// See:
// https://opensource.apple.com/source/xnu/xnu-2050.24.15/osfmk/i386/lapic.h.auto.html


#ifndef ____APIC_H
#define ____APIC_H



/*
    LAPIC registers. 

Software interacts with the local APIC by reading and writing its registers.
APIC registers are memory-mapped to a 4-KByte region of the 
processor’s physical address space with an initial 
starting address of FEE00000H. 
For correct APIC operation, this address space must be mapped to 
an area of memory that has been designated as strong uncacheable (UC).
(Credits: terenceli)

The default memory mapped address of the LAPIC is 0xFEE00000 but should be read
from the MADT or MSRs. Each CPU has it's own LAPIC mapped at this location,
meaning CPUs cannot change each other's LAPICs.
(Credits: https://nemez.net/osdev/lapic.txt)

See:
losethos os - Adam1a.HPP.

// #todo: Vamos precisar de um endereço virtual para acessarmos isso.
#define LAPIC_BASE		0xFEE00000

#define LAPIC_APIC_ID		        (LAPIC_BASE+0x020)
#define LAPIC_APIC_VERSION	        (LAPIC_BASE+0x030)
#define LAPIC_TASK_PRIORITY	        (LAPIC_BASE+0x080)
#define LAPIC_ARIBITRATION_PRIORITY (LAPIC_BASE+0x090)
#define LAPIC_PROCESSOR_PRIORITY    (LAPIC_BASE+0x0A0)
#define LAPIC_EOI		            (LAPIC_BASE+0x0B0)
#define LAPIC_LOG_DST		        (LAPIC_BASE+0x0D0)
#define LAPIC_DFR		            (LAPIC_BASE+0x0E0)
#define LAPIC_LDR		            (LAPIC_BASE+0x0D0)

#define LAPICF_APIC_ENABLED		0x100
#define LAPIC_SVR		(LAPIC_BASE+0x0F0)

#define LAPIC_ISR		    (LAPIC_BASE+0x100)
#define LAPIC_TMR		    (LAPIC_BASE+0x180)
#define LAPIC_IRR		    (LAPIC_BASE+0x200)
#define LAPIC_ICR_LOW		(LAPIC_BASE+0x300)
#define LAPIC_ICR_HIGH		(LAPIC_BASE+0x310)

#define LAPIC_LVT_TIMER		(LAPIC_BASE+0x320)
#define LAPIC_LVT_THERMAL	(LAPIC_BASE+0x330)
#define LAPIC_LVT_PERF		(LAPIC_BASE+0x340)
#define LAPIC_LVT_LINT0		(LAPIC_BASE+0x350)
#define LAPIC_LVT_LINT1		(LAPIC_BASE+0x360)
#define LAPIC_LVT_ERR		(LAPIC_BASE+0x370)

#define MPN_VECT		0x7F
#define MP_VECT_ADDR	(MPN_VECT*0x1000)

//I/O APIC Memory mapped window
#define IOAPIC_REG		0xFEC00000 //U8
#define IOAPIC_DATA		0xFEC00010 //U32
//I/O APIC Registers
#define IOAPICID		0x00
#define IOAPICVER		0x01
#define IOAPICARB		0x02
#define IOREDTAB		0x10

// msr
// Model specific registers.
#define IA32F_SCE	0x001
#define IA32F_LME	0x100
#define IA32_LAPIC_BASE	0x01B
#define IA32_EFER	0xC0000080
#define IA32_FS_BASE	0xC0000100
#define IA32_GS_BASE	0xC0000101
*/



/*
typedef enum {
	ID			= 0x02,
	VERSION			= 0x03,
	TPR			= 0x08,
	APR			= 0x09,
	PPR			= 0x0A,
	EOI			= 0x0B,
	REMOTE_READ		= 0x0C,
	LDR			= 0x0D,
	DFR			= 0x0E,
	SVR			= 0x0F,
	ISR_BASE		= 0x10,
	TMR_BASE		= 0x18,
	IRR_BASE		= 0x20,
	ERROR_STATUS		= 0x28,
	LVT_CMCI		= 0x2F,
	ICR			= 0x30,
	ICRD			= 0x31,
	LVT_TIMER		= 0x32,
	LVT_THERMAL		= 0x33,
	LVT_PERFCNT		= 0x34,
	LVT_LINT0		= 0x35,
	LVT_LINT1		= 0x36,
	LVT_ERROR		= 0x37,
	TIMER_INITIAL_COUNT	= 0x38,
	TIMER_CURRENT_COUNT	= 0x39,
	TIMER_DIVIDE_CONFIG	= 0x3E,
} lapic_register_t;
 
See: apple opensource
*/



// Check presence of apic.
int check_apic (void);

void cpu_set_apic_base(unsigned long apic);
unsigned long cpu_get_apic_base(void); 


// apic stuffs for x86.
static inline void imcr_pic_to_apic (void);
static inline void imcr_apic_to_pic (void);


#endif    //____APIC_H


//
// End.
//

