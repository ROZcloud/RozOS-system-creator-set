# Roz's official system development toolkit
### The set is made for LINUX, you can use it on Windows using WSL
[Windows Setup](#windows)
 
[Linux Setup](#linux) 
# Setup

## <a name="windows"><a>if you have Windows start here:
## On windows install wsl
```bash
wsl --install
```
### A restart will be required
### After restart enter in cmd:
```bash
wsl
```
### Fill in the login and password fields
### Next
# <a name="linux"><a>If you have windows with wsl or linux continue:

### Enter in wsl linux or linux command:
```bash
git clone https://github.com/ROZcloud/RozOS-system-creator-set.git
```
### Go to the downloaded folder with the command:
```bash
cd RozOS-system-creator-set
cd RozOS
```
### Next edit kernel.c
### Run 
```bash
make install
```
### once to install the required packages
 
### After editing, to get the iso, execute the command:
```bash
make build
```
### An iso file will be created, to run it, execute:
```bash
make run-build
```
### To run without building an iso do:
```bash
make run
```
### this will run ```kernel.c```

# Developer zone:

### Kernel.c:
```c
// Muj plik kernel dla systemu RozOS
typedef unsigned char uint8_t;
typedef unsigned short uint16_t;

#define VIDEO_MEMORY 0xB8000
#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 25

// Porty klawiatury
#define KEYBOARD_DATA_PORT 0x60
#define KEYBOARD_STATUS_PORT 0x64

// Mapowanie klawiszy
unsigned char keyboard_map[128] = {
    0, 27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', 
    '-', '=', '\b', '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 
    'i', 'o', 'p', '[', ']', '\n', 0, 'a', 's', 'd', 'f', 'g', 
    'h', 'j', 'k', 'l', ';', '\'', '`', 0, '\\', 'z', 'x', 'c', 
    'v', 'b', 'n', 'm', ',', '.', '/', 0, '*', 0, ' ', 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

void printf(const char* str, uint8_t color) {
    uint16_t* video = (uint16_t*)VIDEO_MEMORY;
    static int cursor_pos = 0;
    
    for(int i = 0; str[i] != '\0'; i++) {
        if(str[i] == '\n') {
            cursor_pos = (cursor_pos / SCREEN_WIDTH + 1) * SCREEN_WIDTH;
        } else {
            video[cursor_pos] = (color << 8) | str[i];
            cursor_pos++;
        }
        
        if(cursor_pos >= SCREEN_WIDTH * SCREEN_HEIGHT) {
            for(int j = 0; j < SCREEN_WIDTH * (SCREEN_HEIGHT - 1); j++) {
                video[j] = video[j + SCREEN_WIDTH];
            }
            for(int j = SCREEN_WIDTH * (SCREEN_HEIGHT - 1); j < SCREEN_WIDTH * SCREEN_HEIGHT; j++) {
                video[j] = (color << 8) | ' ';
            }
            cursor_pos = SCREEN_WIDTH * (SCREEN_HEIGHT - 1);
        }
    }
}

void clear(uint8_t color) {
    uint16_t* video = (uint16_t*)VIDEO_MEMORY;
    for(int i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT; i++) {
        video[i] = (color << 8) | ' ';
    }
}

// Funkcja do odczytu portu
uint8_t inb(uint16_t port) {
    uint8_t result;
    asm volatile("inb %1, %0" : "=a"(result) : "Nd"(port));
    return result;
}

// Funkcja jak input() w Pythonie - czeka na Enter
void input(char* buffer, int max_length, uint8_t color) {
    int pos = 0;
    printf("> ", color);
    
    while(1) {
        uint8_t status = inb(KEYBOARD_STATUS_PORT);
        if(status & 0x01) {
            uint8_t keycode = inb(KEYBOARD_DATA_PORT);
            
            if(keycode < 128 && keyboard_map[keycode]) {
                char key = keyboard_map[keycode];
                
                if(key == '\n') { // Enter
                    buffer[pos] = '\0';
                    printf("\n", color);
                    return;
                }
                else if(key == '\b') { // Backspace
                    if(pos > 0) {
                        pos--;
                        printf("\b \b", color);
                    }
                }
                else if(pos < max_length - 1) {
                    buffer[pos] = key;
                    pos++;
                    // Wyświetl znak
                    char str[2] = {key, '\0'};
                    printf(str, color);
                }
            }
        }
    }
}

// Funkcja jak strcmp - porównywanie stringów
int strcmp(const char* str1, const char* str2) {
    while(*str1 && (*str1 == *str2)) {
        str1++;
        str2++;
    }
    return *(unsigned char*)str1 - *(unsigned char*)str2;
}

// Funkcja "Press enter to continue"
void press_enter_to_continue(uint8_t color) {
    printf("Press Enter to continue...", color);
    
    while(1) {
        uint8_t status = inb(KEYBOARD_STATUS_PORT);
        if(status & 0x01) {
            uint8_t keycode = inb(KEYBOARD_DATA_PORT);
            if(keycode < 128 && keyboard_map[keycode] == '\n') {
                return;
            }
        }
    }
}

void kernel_main(void) {
    // Wyczyść ekran na niebiesko
    clear(0x01);
    
    printf("=== RozOS Kernel ===\n", 0x1F);
    printf("Kernel loaded successfully!\n", 0x0F);
    
    // Test input() jak w Pythonie
    printf("\nWhat's your name? ", 0x0E);
    char name[50];
    input(name, 50, 0x0F);
    
    printf("Hello, ", 0x0F);
    printf(name, 0x0A);
    printf("!\n", 0x0F);
    
    // Test if i porównywanie
    if(strcmp(name, "szymon") == 0) {
        printf("Welcome back, Szymon!\n", 0x0D);
    } else {
        printf("Nice to meet you!\n", 0x0B);
    }
    
    // Press enter to continue
    press_enter_to_continue(0x0E);
    
    // Czyszczenie i nowy ekran
    clear(0x01);
    printf("=== Interactive Shell ===\n", 0x1F);
    printf("Type 'exit' to quit\n", 0x0F);
    
    // Prosta pętla komend
    char command[50];
    while(1) {
        printf("RozOS> ", 0x0E);
        input(command, 50, 0x0F);
        
        if(strcmp(command, "exit") == 0) {
            printf("Goodbye!\n", 0x0C);
            break;
        }
        else if(strcmp(command, "help") == 0) {
            printf("Available commands: help, exit, hello\n", 0x0A);
        }
        else if(strcmp(command, "hello") == 0) {
            printf("Hello from RozOS!\n", 0x0B);
        }
        else if(strcmp(command, "") == 0) {
            // Pusta komenda - nic nie rób
        }
        else {
            printf("Unknown command: ", 0x0C);
            printf(command, 0x0C);
            printf("\n", 0x0C);
        }
    }
    
    printf("Kernel finished.\n", 0x0D);
    while(1);
}
```
###
### Linker.id:
```asm
ENTRY(loader)

SECTIONS {
    . = 0x00100000;

    .text ALIGN(0x1000) : {
        *(.multiboot)
        *(.text)
    }

    .data ALIGN(0x1000) : {
        *(.data)
    }

    .bss ALIGN(0x1000) : {
        *(COMMON)
        *(.bss)
    }
}
```
###
### Minimal grub.cfg:
```c
set timeout=5
set default=0

menuentry "RozOS" {
    multiboot /boot/kernel.elf
    boot
}
```
###
### Loading kernel script boot.s:
```asm
section .multiboot
align 4
    dd 0x1BADB002
    dd 0x00000003  
    dd -(0x1BADB002 + 0x00000003)

section .text
global loader
extern kernel_main

loader:
    call kernel_main
    hlt
```
###
### Makefile:
```Makefile

run:
	@qemu-system-x86_64 -kernel kernel.elf
	@echo Succes!
run-build:
	@qemu-system-x86_64 -cdrom rozos.iso
	@echo Succes!
clean:
	@rm -f *.o kernel.elf
	@rm -f rozos.iso 
	@rm -rf iso
	@echo Succes!
install:
	@sudo apt install grub-common grub-pc-bin
	@sudo apt install nasm
	@sudo apt install xorriso
	@sudo apt install qemu-system-x86_64
	@echo Succes!
build:
	@nasm -f elf boot.s -o boot.o
	@gcc -m32 -c kernel.c -o kernel.o -ffreestanding -nostdlib -O0
	@ld -m elf_i386 -T linker.ld -o kernel.elf boot.o kernel.o
	@mkdir -p iso/boot/grub
	@cp kernel.elf iso/boot/
	@cp grub.cfg iso/boot/grub/grub.cfg
	@grub-mkrescue --version
	@grub-mkrescue -o rozos.iso iso/
	@echo Succes!
help:
	@echo make build to build os
	@echo make install all requirements
	@echo make run-build run builded iso
	@echo make run to run kernel
```
