// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <naive_graphics_console.h>
#include <font.h>
#include <stdint.h>
#include <string.h>
#include <lib.h>

struct vbe_mode_info_structure {
    uint16_t attributes;        // deprecated, only bit 7 should be of interest to you, and it indicates the mode supports a linear frame buffer.
    uint8_t window_a;           // deprecated
    uint8_t window_b;           // deprecated
    uint16_t granularity;       // deprecated; used while calculating bank numbers
    uint16_t window_size;
    uint16_t segment_a;
    uint16_t segment_b;
    uint32_t win_func_ptr;      // deprecated; used to switch banks from protected mode without returning to real mode
    uint16_t pitch;         // number of bytes per horizontal line
    uint16_t width;         // width in pixels
    uint16_t height;        // height in pixels
    uint8_t w_char;         // unused...
    uint8_t y_char;         // ...
    uint8_t planes;
    uint8_t bpp;            // bits per pixel in this mode
    uint8_t banks;          // deprecated; total number of banks in this mode
    uint8_t memory_model;
    uint8_t bank_size;      // deprecated; size of a bank, almost always 64 KB but may be 16 KB...
    uint8_t image_pages;
    uint8_t reserved0;

    uint8_t red_mask;
    uint8_t red_position;
    uint8_t green_mask;
    uint8_t green_position;
    uint8_t blue_mask;
    uint8_t blue_position;
    uint8_t reserved_mask;
    uint8_t reserved_position;
    uint8_t direct_color_attributes;

    uint32_t frame_buffer;       // physical address of the linear frame buffer; write here to draw to the screen
    uint32_t off_screen_mem_off;
    uint16_t off_screen_mem_size;   // size of memory in the frame_buffer but not being displayed on the screen
    uint8_t reserved1[206];

    //uint8_t pixel_height; // alto del char, x1 = 16, x2 = 32
    //uint8_t pixel_width; // ancho del char, x1 = 9, x2 = 16

} __attribute__ ((packed));

struct vbe_mode_info_structure* screen_data = (void*)0x5C00;

// base
static int level = 1; //que empiece en 1
//    STATIC METHODS DECLARATION
static void* get_ptr_to_pixel(uint16_t x, uint16_t y) {
    return (void*)(screen_data->frame_buffer + 3 * (x + (y * (uint64_t)screen_data->width)));
}

uint16_t pen_x = 0, pen_y = 0;
uint16_t last_pen_x = 0;
Color pen_color = {0x7F, 0x7F, 0x7F};



// ==============================================================================
// PUBLIC GETTERS && NGC_PRINT METHODS 

uint16_t ngc_get_height(){
    return screen_data->height;
}

uint16_t ngc_get_width(){
    return screen_data->width;
}

#define BLUE        0xFF0000
#define GREEN       0x00FF00
#define RED         0x0000FF

static void color_int_to_color(uint64_t color_int, Color* color) {
    //0xBBGGRR
    color->b = (color_int>>16) & 0xFF;
    color->g = (color_int>>8) & 0xFF;
    color->r = color_int & 0xFF;
}

static void ngc_set_color(Color new_color){
    pen_color = new_color;
}


void ngc_print_color(const char * string, uint64_t color_int){
    Color color;
    color_int_to_color(color_int, &color);
    ngc_set_color(color);
    ngc_print(string);
    Color gray = {0x7F, 0x7F, 0x7F}; //reset to default print color
    ngc_set_color(gray);
}

void change_font_size(uint64_t new_level){
    level = new_level;
}

static void ngc_font_size_1(const char * data){
    for(int h=0; h < CHAR_HEIGHT*level ; h+=level){
        Color* pos = (Color*)get_ptr_to_pixel(pen_x, pen_y+h);
            // si el bit en cada posicion esta prendido: pinto, sino dejo
    		if (*data & 0b00000001) pos[0] = pen_color;
    		if (*data & 0b00000010) pos[1] = pen_color;
    		if (*data & 0b00000100) pos[2] = pen_color;
    		if (*data & 0b00001000) pos[3] = pen_color;
    		if (*data & 0b00010000) pos[4] = pen_color;
    		if (*data & 0b00100000) pos[5] = pen_color;
    		if (*data & 0b01000000) pos[6] = pen_color;
    		if (*data & 0b10000000) pos[7] = pen_color;
    		data++;
            // salteo un pixel para espacear
    		if (*data & 0b00000001) pos[8] = pen_color;
    		data++;
    }
}

static void ngc_font_size_2(const char *data){
    for(int h=0; h < CHAR_HEIGHT*level ; h+=level){
        Color* pos = (Color*)get_ptr_to_pixel(pen_x, pen_y+h);
        Color* pos2 = (Color*)get_ptr_to_pixel(pen_x, pen_y+h+1);
        // si el bit en cada posicion esta prendido: pinto, sino dejo
        if (*data & 0b00000001) pos[0] = pen_color, pos[1] = pen_color, pos2[0] = pen_color, pos2[1] = pen_color;       
        if (*data & 0b00000010) pos[2] = pen_color, pos[3] = pen_color, pos2[2] = pen_color, pos2[3] = pen_color;       
        if (*data & 0b00000100) pos[4] = pen_color, pos[5] = pen_color, pos2[4] = pen_color, pos2[5] = pen_color;
        if (*data & 0b00001000) pos[6] = pen_color, pos[7] = pen_color , pos2[6] = pen_color, pos2[7] = pen_color;     
        if (*data & 0b00010000) pos[8] = pen_color, pos[9] = pen_color , pos2[8] = pen_color, pos2[9] = pen_color;
        if (*data & 0b00100000) pos[10] = pen_color, pos[11] = pen_color, pos2[10] = pen_color, pos2[11] = pen_color;
        if (*data & 0b01000000) pos[12] = pen_color, pos[13] = pen_color, pos2[12] = pen_color, pos2[13] = pen_color;
        if (*data & 0b10000000) pos[14] = pen_color, pos[15] = pen_color, pos2[14] = pen_color, pos2[15] = pen_color;
        data++;
        // salteo un pixel para espacear
        if (*data & 0b00000001) pos[16] = pen_color, pos[17] = pen_color,  pos2[16] = pen_color, pos2[17] = pen_color;
        data++;
    }
}

static void ngc_font_size_3(const char *data){
    for(int h=0; h < CHAR_HEIGHT*level ; h+=level){
        Color* pos = (Color*)get_ptr_to_pixel(pen_x, pen_y+h);
        Color* pos2 = (Color*)get_ptr_to_pixel(pen_x, pen_y+h+1);
        Color* pos3 = (Color*)get_ptr_to_pixel(pen_x, pen_y+h+2);
        // si el bit en cada posicion esta prendido: pinto, sino dejo
        if (*data & 0b00000001) pos[0] = pen_color, pos[1] = pen_color, pos[2] = pen_color, pos2[0] = pen_color, pos2[1] = pen_color, pos2[2] = pen_color, pos3[0] = pen_color, pos3[1] = pen_color, pos3[2] = pen_color;
        if (*data & 0b00000010) pos[3] = pen_color, pos[4] = pen_color, pos[5] = pen_color, pos2[3] = pen_color, pos2[4] = pen_color, pos2[5] = pen_color, pos3[3] = pen_color, pos3[4] = pen_color, pos3[5] = pen_color;      
        if (*data & 0b00000100) pos[6] = pen_color, pos[7] = pen_color, pos[8] = pen_color, pos2[6] = pen_color, pos2[7] = pen_color, pos2[8] = pen_color, pos3[6] = pen_color, pos3[7] = pen_color, pos3[8] = pen_color;
        if (*data & 0b00001000) pos[9] = pen_color, pos[10] = pen_color, pos[11] = pen_color, pos2[9] = pen_color, pos2[10] = pen_color, pos2[11] = pen_color, pos3[9] = pen_color, pos3[10] = pen_color, pos3[11] = pen_color;    
        if (*data & 0b00010000) pos[12] = pen_color, pos[13] = pen_color, pos[14] = pen_color, pos2[12] = pen_color, pos2[13] = pen_color, pos2[14] = pen_color, pos3[12] = pen_color, pos3[13] = pen_color, pos3[14] = pen_color;
        if (*data & 0b00100000) pos[15] = pen_color, pos[16] = pen_color, pos[17] = pen_color, pos2[15] = pen_color, pos2[16] = pen_color, pos2[17] = pen_color, pos3[15] = pen_color, pos3[16] = pen_color, pos3[17] = pen_color;
        if (*data & 0b01000000) pos[18] = pen_color, pos[19] = pen_color, pos[20] = pen_color, pos2[18] = pen_color, pos2[19] = pen_color, pos2[20] = pen_color, pos3[18] = pen_color, pos3[19] = pen_color, pos3[20] = pen_color;
        if (*data & 0b10000000) pos[21] = pen_color, pos[22] = pen_color, pos[23] = pen_color, pos2[21] = pen_color, pos2[22] = pen_color, pos2[23] = pen_color, pos3[21] = pen_color, pos3[22] = pen_color, pos3[23] = pen_color;
        data++;
        // salteo un pixel para espacear
        if (*data & 0b00000001) pos[24] = pen_color, pos[25] = pen_color, pos[26] = pen_color, pos2[24] = pen_color, pos2[25] = pen_color, pos2[26] = pen_color, pos3[24] = pen_color, pos3[25] = pen_color, pos3[26] = pen_color;
        data++;
    }
}
void ngc_print_char(char c) {
    if (c == '\n') {
        ngc_print_new_line();
        return;
    } else 
    if (c == 0x7F) // BACKSPACE code
    {
        delete_last_char();
        return;
    } else
    if (c >= FIRST_CHAR && c <= LAST_CHAR) {
        const char* data = font + 32*(c-33);
        switch (level){
            case 1: ngc_font_size_1(data);
                    break;
            case 2: ngc_font_size_2(data);
                    break;
            case 3: ngc_font_size_3(data);
                    break;
        }

    }
    pen_x += CHAR_WIDTH * level;
    if (pen_x > screen_data->width - CHAR_WIDTH * level)
        ngc_print_new_line();
}

void ngc_print(const char * string){
    for(int i = 0; string[i]!= '\0';i++){
        ngc_print_char(string[i]);
    }
}

void ngc_print_new_line(void) {
    last_pen_x = pen_x;
    pen_x = 0; // pen x is set to full left.

    // If there is space for another line, we simply advance the pen y. Otherwise, we move up the entire screen and clear the lower part.
    if (pen_y + (level*CHAR_HEIGHT) <= (screen_data->height - (CHAR_HEIGHT*3))) { //CHAR_HEIGHT*3 es para dejar lugar para la font mas grande (level = 3)
        pen_y += level*(CHAR_HEIGHT); // con un -3 se agarra un poco mas arriba y lo hace mas legible pero tan chantas no somos
    } else {
        //AGREGANDO X2 PARA EL NUEVO TAM
        // dst: posicion de inicio de la pantalla
        void* dst = (void*)((uint64_t)screen_data->frame_buffer);

        // src: ultima posicion de la pantalla 
        // (3 por rgb, CHAR_HEIGHT es el tamaño de cada linea, width es el ancho de la pantalla)
        void* src = (void*)(dst + 3 * (CHAR_HEIGHT*level * (uint64_t)screen_data->width)); //char_height*2 para el aumento del size
        
        // len: cantidad de pixeles en toda la pantalla menos la ultima linea
        // 3 por rgb, width es el ancho, height - CHAR_HEIGHT es todas las lineas excepto la ultima
        uint64_t len = 3 * ((uint64_t)screen_data->width * ((screen_data->height)*level - (CHAR_HEIGHT*level))); //height*2 char_height*2 para el aumento del size
        
        /*
            este bloque copia todo lo escrito en pantalla a una linea mas arriba
            y deja la posicion para escribir en la ultima linea
        */
        mem_cpy(dst, src, len);
        mem_set(dst+len, 0, 3 * (uint64_t)screen_data->width * CHAR_HEIGHT*level); 
    }
}
void ngc_print_pixel(uint16_t x, uint16_t y, uint64_t color_int) {
    Color color;
    color_int_to_color(color_int, &color);
    Color* pos = (Color*) get_ptr_to_pixel(x, y);
    *pos = color;
}

void ngc_print_pixels(uint64_t from_x, uint64_t from_y, uint16_t width, uint16_t height, uint64_t color) {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            ngc_print_pixel(from_x + i, from_y + j, color);
        }
    }
}



void ngc_paint_screen(uint64_t bg_colorInt) {
    Color bg_color;
    color_int_to_color(bg_colorInt, &bg_color);
    uint8_t* pos = (uint8_t*)((uint64_t)screen_data->frame_buffer);
    for (uint32_t len = (uint32_t)screen_data->width * screen_data->height ; len; len--, pos+=3) {
        pos[0] = bg_color.b;
        pos[1] = bg_color.g;
        pos[2] = bg_color.r;
    }
    
    pen_x = 0; pen_y = 0;
}

// ==============================================================================

void delete_last_char() {
    if (pen_x == CHAR_WIDTH*2*level) //Para que no se pueda borrar el prompt
    {
        return;
    }
    // caso: Delete last char
    else {

        pen_x -= CHAR_WIDTH*level;

        Color aux = {0,0,0};
        
        for (int h=0; h < CHAR_HEIGHT*level; h++) {
            // se llenan todos los pixeles del ultimo char en {0,0,0}
    		Color* pos = (Color*)get_ptr_to_pixel(pen_x, pen_y+h);
            
            for (int i = 0 ;i < CHAR_HEIGHT*level; i++){
                pos[i]=aux;
            }
    	}
    }
    
}
