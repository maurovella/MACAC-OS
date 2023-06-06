#include <keyboard.h>
#include <naive_graphics_console.h>
#include <stdint.h>
#include <string.h>
#include <scheduler.h>
#define SPECIAL_CHARS 3

#define BUFFER_SIZE 1024


static const uint8_t char_hex_map[256] = {       
        0,    0,  '1',  '2',  '3',  '4',  '5',  '6',   '7',  '8',  '9',   '0',   '-',  '=',    0x7F,
    '\t', 'q',  'w',  'e',  'r',  't',  'y',  'u',  'i',   'o',  'p',  '[',   ']', '\n',   
        '|',     'a',  's', 'd',  'f',  'g',  'h',  'j',  'k',  'l',  ';',  '\"',
    0,    0,  '\\',   'z',  'x',     'c', 'v', 'b',  'n',  'm',  ',',  '.',  '/',    0,  
    '*',     0,  ' ',    0,     0,     0/*60*/,    0,       0,         0, 
    0,     0,     0,    0,      0,      0,      0,      0,         'U'/*up*/,
    0,      0,      'L'/*left*/,    0,     'R'/*right*/,     0,      0,      'D'/*down*/,      0,
};

static const uint8_t special_hex_chars[SPECIAL_CHARS] = {0,'&','|'};

static uint8_t buffer[BUFFER_SIZE] = {0};
static uint64_t q_elements = 0;
static uint64_t read_index = 0;          // iter first element
static uint64_t write_index = 0;         // iter last element
static uint8_t ctrl_pressed = 0;

static void ctrl_c_handler() {
    kill_all_processes();
    ngc_print("Ctrl+C pressed\n");
}

static void ctrl_d_handler() {
    ngc_print("Ctrl+D pressed\n");
}

void keyboard_handler(uint64_t tecla_hex){
    uint8_t special_char = 0;
    if (tecla_hex == 0x1D) {
        ctrl_pressed = 1;
        return;
    } else if (ctrl_pressed && tecla_hex == 0x2E) {
        ctrl_c_handler();
        ctrl_pressed = 0;
        return;
    } else if (ctrl_pressed && tecla_hex == 0x20) {
        ctrl_d_handler();
        ctrl_pressed = 0;
        return;
    } else if (ctrl_pressed && tecla_hex == 0x07) {
        //ctrl_ampersand_handler();
        special_char = 1;
        ctrl_pressed = 0;
    } else if (ctrl_pressed && tecla_hex == 0x02) {
        //ctrl_pipe_handler(&tecla_hex);
        special_char = 2;
        ctrl_pressed = 0;
    } else {
        ctrl_pressed = 0;
    }
    
    if (tecla_hex < 0x53 || special_char){
        if(q_elements >= BUFFER_SIZE) return;  // buffer is full
        // MAKE IT ROUNDY
        if (write_index >= BUFFER_SIZE)     write_index = 0;

        if(special_char == 0)
            buffer[write_index] = char_hex_map[tecla_hex];
        else {
            buffer[write_index] = special_hex_chars[special_char];
        }

        // update iterators
        q_elements++;
        write_index++;
        special_char = 0;
    }
}

char get_first_char(){
    if (q_elements == 0) { 
        return 0xFF; // buffer is empty
    }

    char to_return = buffer[read_index];
    
    // update iterators
    q_elements--;
    read_index++;

    // MAKE IT ROUNDY
    if (read_index == BUFFER_SIZE) read_index = 0;
    
    return to_return;
}
