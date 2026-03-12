#
#	Name: Ty Ahrens
#	Date: 3/11/2026
#	
#	Makefile - version 0.2.0
#

# NOTE: for Mac M1, M2, or M3 users, remove the "aarch64-linux-gnu-"
#       from each of the commands below.
all: kernel.elf


kernel.o: kernel.c
	@echo "compile kernel.c into kernel.o..."
	aarch64-linux-gnu-gcc -g -pedantic -Wall -Wextra -fPIC -std=gnu2x -MMD -c kernel.c -o kernel.o
	
queue.o: queue.c
	@echo "compile queue.c into queue.o..."
	aarch64-linux-gnu-gcc -g -pedantic -Wall -Wextra -fPIC -std=gnu2x -MMD -c queue.c -o queue.o
	
process.o: process.c
	@echo "compile process.c into process.o..."
	aarch64-linux-gnu-gcc -g -pedantic -Wall -Wextra -fPIC -std=gnu2x -MMD -c process.c -o process.o

boot.o: boot.S
	@echo "assemble boot.S into boot.o..."
	aarch64-linux-gnu-gcc -g -MMD -c boot.S -o boot.o

box.o: box.S
	@echo "assemble box.S into box.o..."
	aarch64-linux-gnu-gcc -g -MMD -c box.S -o box.o
	
process_asm.o: process_asm.S
	@echo "assemble process_asm.S into process_asm.o..."
	aarch64-linux-gnu-gcc -g -MMD -c process_asm.S -o process_asm.o

kernel.elf: kernel.o queue.o process.o boot.o box.o process_asm.o libos.a
	@echo "link kernel.o, boot.o, box.o, and libos.a together to make kernel.elf"
	aarch64-linux-gnu-ld -g -N -Ttext=0x10000 -o kernel.elf kernel.o queue.o process.o boot.o box.o process_asm.o libos.a
	@echo "Note that the linker may give the following complaint. Ignore it."
	@echo " "
	@echo "=============================================="
	@echo "      kernel.elf created successfully!"
	@echo "=============================================="
	@echo " "
# aarch64-linux-gnu-ld: warning: kernel.elf has a LOAD segment with RWX permissions

#################################################################################################

# clean up all created files
clean:
	@echo "Cleaning up..."
	rm -f kernel.o queue.o process.o boot.o box.o process_asm.o kernel.elf *.d
	
# run the OS
run:
	@echo "Booting up operating system..."
	qemu-system-aarch64 -machine raspi3b -kernel kernel.elf
	
# open the debugger tab for the OS
debug:
	@echo "Starting debugger..."
	qemu-system-aarch64 -machine raspi3b  -S -s -kernel kernel.elf &
	ddd --debugger 'gdb-multiarch -ex "target remote localhost:1234" -ex "break main" -ex "continue"' kernel.elf
	
#################################################################################################

.PHONY:	all clean
