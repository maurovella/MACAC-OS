#include <stdint.h>
#include <idt_loader.h>
#include <defs.h>
#include <interrupts.h>

#pragma pack(push)		/* Push de la alineación actual */
#pragma pack (1) 		/* Alinear las siguiente estructuras a 1 byte */

/* Descriptor de interrupcion */
typedef struct {
  uint16_t offset_l, selector;
  uint8_t cero, access;
  uint16_t offset_m;
  uint32_t offset_h, other_cero;
} DESCR_INT;

#pragma pack(pop)		/* Reestablece la alineación actual */



DESCR_INT * idt = (DESCR_INT *) 0;	// IDT de 255 entradas

static void setup_IDT_entry (int index, uint64_t offset);

void load_idt() {

  _cli(); //para llamar varias veces al load_idt();

    // Exceptions
    setup_IDT_entry (0x00, (uint64_t)&_division_by_zero_interruption);
	  setup_IDT_entry (0x06, (uint64_t)&_invalid_op_code_interruption);

    // Hardware Interruptions
    setup_IDT_entry (0x20, (uint64_t)&_irq00_handler);
    setup_IDT_entry (0x21, (uint64_t)&_irq01_handler);

    // Syscalls Interruptions
    setup_IDT_entry (0x80, (uint64_t)&_irq80_handler);
    

	// Solo interrupcion timer tick habilitadas (0xFE en masterMask)
  // Para habilitar las interrupciones de teclado 0xFD en masterMask
  // Para habilitar ambas 0xFC en masterMask
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