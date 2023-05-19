#include <keyboard.h>
#include <naiveGraphicsConsole.h>
#include <stdint.h>
#include <string.h>

#define BUFFER_SIZE 1024


static const uint8_t char_hex_map[256] = {       
        0,    0,  '1',  '2',  '3',  '4',  '5',  '6',   '7',  '8',  '9',   '0',   '-',  '=',    0x7F,
    '\t', 'q',  'w',  'e',  'r',  't',  'y',  'u',  'i',   'o',  'p',  '[',   ']', '\n',   
        0,     'a',  's', 'd',  'f',  'g',  'h',  'j',  'k',  'l',  ';',  '\"',
    0,    0,  '\\',   'z',  'x',     'c', 'v', 'b',  'n',  'm',  ',',  '.',  '/',    0,  
    '*',     0,  ' ',    0,     0,     0/*60*/,    0,       0,         0, 
    0,     0,     0,    0,      0,      0,      0,      0,         'U'/*up*/,
    0,      0,      'L'/*left*/,    0,     'R'/*right*/,     0,      0,      'D'/*down*/,      0,
};

static uint8_t buffer[BUFFER_SIZE] = {0};
static uint64_t q_elements = 0;
static uint64_t read_index = 0;          // iter first element
static uint64_t write_index = 0;         // iter last element


void keyboard_handler(uint64_t tecla_hex){
    if (tecla_hex < 0x53){
        if(q_elements >= BUFFER_SIZE) return;  // buffer is full
        
        // MAKE IT ROUNDY
        if (write_index >= BUFFER_SIZE)     write_index = 0;

        buffer[write_index] = char_hex_map[tecla_hex];

        // update iterators
        q_elements++;
        write_index++;
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
