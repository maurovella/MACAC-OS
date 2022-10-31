#include <keyboard.h>
#include <naiveGraphicsConsole.h>
#include <stdint.h>
#include <string.h>

#define BUFFER_SIZE 256


static const uint8_t charHexMap[256] = {       
        0,    0,  '1',  '2',  '3',  '4',  '5',  '6',   '7',  '8',  '9',   '0',   '-',  '=',    0x7F,
    '    ', 'q',  'w',  'e',  'r',  't',  'y',  'u',  'i',   'o',  'p',  '[',   ']', '\n',   
        0,     'a',  's', 'd',  'f',  'g',  'h',  'j',  'k',  'l',  ';',  '\"',
    0,    0,  '\\',   'z',  'x',     'c', 'v', 'b',  'n',  'm',  ',',  '.',  '/',    0,  
    '*',     0,  ' ',    0,     0,     0/*60*/,    0,       0,         0, //yo creo q deberia ir ahi pero a //viendo el teclado... creo asjdjaj
    0,     0,     0,    0,      0,      0,      0/*70*/,      0,         'U'/*up*/,
    0,      0,      'L'/*left*/,    0,     'R'/*right*/,     0,      0,      'D'/*down*/,      0,
};
//flechas: arriba abajo der izq para probar

static uint8_t buffer[BUFFER_SIZE] = {0};
static uint64_t qElements = 0;
static uint64_t readIndex = 0;          // iter first element
static uint64_t writeIndex = 0;         // iter last element


void keyboard_handler(uint64_t teclahex){
    if (teclahex < 0x53){
        if(qElements >= BUFFER_SIZE) return;  // buffer is full
        
        // MAKE IT ROUNDY
        if (writeIndex >= BUFFER_SIZE)     writeIndex = 0;

        buffer[writeIndex] = charHexMap[teclahex];

        // update iterators
        qElements++;
        writeIndex++;
    }
}

char getFirstChar(){
    if (qElements == 0) { 
        return -1;
    }

    char toReturn = buffer[readIndex];

    // update iterators
    qElements--;
    readIndex++;

    // MAKE IT ROUNDY
    if (readIndex == BUFFER_SIZE) readIndex = 0;
    
    return toReturn;
}
