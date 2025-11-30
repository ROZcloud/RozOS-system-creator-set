// Muj plik kernel dla systemu RozOS
typedef unsigned char uint8_t;
typedef unsigned short uint16_t;

#define VIDEO_MEMORY 0xB8000
#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 25

// Porty klawiatury
#define KEYBOARD_DATA_PORT 0x60
#define KEYBOARD_STATUS_PORT 0x64

// Globalna pozycja kursora
int cursor_position = 0;

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
    
    for(int i = 0; str[i] != '\0'; i++) {
        if(str[i] == '\n') {
            cursor_position = (cursor_position / SCREEN_WIDTH + 1) * SCREEN_WIDTH;
        } else {
            video[cursor_position] = (color << 8) | str[i];
            cursor_position++;
        }
        
        if(cursor_position >= SCREEN_WIDTH * SCREEN_HEIGHT) {
            for(int j = 0; j < SCREEN_WIDTH * (SCREEN_HEIGHT - 1); j++) {
                video[j] = video[j + SCREEN_WIDTH];
            }
            for(int j = SCREEN_WIDTH * (SCREEN_HEIGHT - 1); j < SCREEN_WIDTH * SCREEN_HEIGHT; j++) {
                video[j] = (color << 8) | ' ';
            }
            cursor_position = SCREEN_WIDTH * (SCREEN_HEIGHT - 1);
        }
    }
}

void clear(uint8_t color) {
    uint16_t* video = (uint16_t*)VIDEO_MEMORY;
    for(int i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT; i++) {
        video[i] = (color << 8) | ' ';
    }
    cursor_position = 0;
}

// Funkcja do ustawiania kursora
void set_cursor(int position) {
    cursor_position = position;
    if(cursor_position < 0) cursor_position = 0;
    if(cursor_position >= SCREEN_WIDTH * SCREEN_HEIGHT) 
        cursor_position = SCREEN_WIDTH * SCREEN_HEIGHT - 1;
}

// Funkcja do ustawiania kursora na pozycji (x,y)
void set_cursor_pos(int x, int y) {
    cursor_position = y * SCREEN_WIDTH + x;
    if(cursor_position < 0) cursor_position = 0;
    if(cursor_position >= SCREEN_WIDTH * SCREEN_HEIGHT) 
        cursor_position = SCREEN_WIDTH * SCREEN_HEIGHT - 1;
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
    uint16_t* video = (uint16_t*)VIDEO_MEMORY;
    
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
                        cursor_position--; 
                        video[cursor_position] = (color << 8) | ' '; // Wyczyść znak
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
void pause(uint8_t color) {
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

void clear_and_print(uint8_t color, const char* str, uint8_t text_color) {
    clear(color);
    printf(str, text_color);
}

void kernel_main(void) {

    clear(0x01);
    //RozOS COMMANDS
    //if(strcmp(name, "szymon") == 0)
    //press_enter_to_continue(0x0Ef;
    //set_cursor_pos(0, 0); 
    //clear(0x01);
    printf("=== RozOS Kernel ===\n", 0x1F);
    printf("Kernel loaded successfully!\n", 0x0F);
    pause(0x0F);
    clear(0x01);

    printf("This is not a full system, just a kit to build a system!\n", 0x1F);
    while (1);
    
}