 /*
 *   interrupts.h
 *
 *  Created on: Apr 18, 2010
 *      Author: anizzomc
 */

#ifndef INTERRUPS_H_
#define INTERRUPS_H_

#include <idt_loader.h>

extern void _irq00_handler(void);
extern void _irq01_handler(void);
extern void _irq02_handler(void);
extern void _irq03_handler(void);
extern void _irq04_handler(void);
extern void _irq05_handler(void);
extern void _irq10_handler(void);
extern void _irq80_handler(void);

extern void _division_by_zero_interruption(void);
extern void _invalid_op_code_interruption(void);

extern void _cli(void);

extern void _sti(void);

extern void _hlt(void);

extern void pic_master_mask(uint8_t mask);

extern void pic_slave_mask(uint8_t mask);

//Termina la ejecución de la cpu.
extern void halt_cpu(void);

extern void force_timer_tick(void);
extern void force_current_process(void);
extern uint8_t consume_tick(void);

#endif /* INTERRUPS_H_ */
