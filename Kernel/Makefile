include Makefile.inc

KERNEL=kernel.bin

SOURCES=$(wildcard *.c)
SOURCES_ASM=$(wildcard asm/*.asm)
SOURCES_UTILS=$(wildcard utils/*.c)
SOURCES_SYSCALLS=$(wildcard syscalls/*.c)

OBJECTS=$(SOURCES:.c=.o)
OBJECTS_ASM=$(SOURCES_ASM:.asm=.o)
OBJECTS_UTILS=$(SOURCES_UTILS:.c=.o)
OBJECTS_SYSCALLS=$(SOURCES_SYSCALLS:.c=.o)


LOADERSRC=loader.asm

MEMORY_MANAGER=-D WITH_BUDDY_MM_
#MEMORY_MANAGER=

LOADEROBJECT=$(LOADERSRC:.asm=.o)
STATICLIBS=

all: $(KERNEL)

$(KERNEL): $(LOADEROBJECT) $(OBJECTS) $(STATICLIBS) $(OBJECTS_ASM) $(OBJECTS_UTILS) $(OBJECTS_SYSCALLS)
	$(LD) $(LDFLAGS) -T kernel.ld -o $(KERNEL) $(LOADEROBJECT) $(OBJECTS) $(OBJECTS_ASM) $(STATICLIBS) $(OBJECTS_UTILS) $(OBJECTS_SYSCALLS)
	$(LD) $(LDFLAGS) -T kernel.ld --oformat=elf64-x86-64 -o kernel.elf $(LOADEROBJECT) $(OBJECTS) $(OBJECTS_ASM) $(STATICLIBS) $(OBJECTS_UTILS) $(OBJECTS_SYSCALLS)
%.o: %.c
	$(GCC) $(GCCFLAGS) -I./include -c $< -o $@ $(MEMORY_MANAGER)

%.o : %.asm
	$(ASM) $(ASMFLAGS) $< -o $@

$(LOADEROBJECT):
	$(ASM) $(ASMFLAGS) $(LOADERSRC) -o $(LOADEROBJECT)

clean:
	rm -rf asm/*.o syscalls/*.o utils/*.o *.o *.bin

.PHONY: all clean
