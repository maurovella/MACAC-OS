#include <stdint.h>
#include <idt_loader.h>
#include <defs.h>
#include <interrupts.h>

#pragma pack(push)		/* Push current alignment */
#pragma pack (1) 		  /* Align following structures at 1 byte */

/* Interruption Descriptor */
typedef struct {
  uint16_t offset_l, selector;
  uint8_t cero, access;
  uint16_t offset_m;
  uint32_t offset_h, other_cero;
} DESCR_INT;

#pragma pack(pop)		/* Resets Current Alignment */

DESCR_INT * idt = (DESCR_INT *) 0;	// 255 entrys IDT

static void setup_IDT_entry (int index, uint64_t offset);

void load_idt() {

 	_cli(); // so that load_idt() can be called multiple times;

    // Exceptions
    setup_IDT_entry (0x00, (uint64_t)&_division_by_zero_interruption);
	setup_IDT_entry (0x06, (uint64_t)&_invalid_op_code_interruption);

    // Hardware Interruptions
    setup_IDT_entry (0x20, (uint64_t)&_irq00_handler);
    setup_IDT_entry (0x21, (uint64_t)&_irq01_handler);

    // Syscalls Interruptions
    setup_IDT_entry (0x80, (uint64_t)&_irq80_handler);
    

	// Only TimerTick interruptions on (0xFE for masterMask)
  	// Only Keyboard Interrupts on     (0xFD for masterMask)
  	// For both: 0xFC
	pic_master_mask(0b11111100);
	pic_slave_mask (0b11111111);
        
	_sti();
}

static void setup_IDT_entry (int index, uint64_t offset) {
    idt[index].offset_l = offset & 0xFFFF;
    idt[index].selector = 0x08;
    idt[index].cero = 0;
    idt[index].access = ACS_INT;
    idt[index].offset_m = (offset >> 16) & 0xFFFF;
    idt[index].offset_h = (offset >> 32) & 0xFFFFFFFF;
    idt[index].other_cero = (uint64_t) 0;
}
