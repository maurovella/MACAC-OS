#include <stdint.h>
#include <inout.h>
#include <syscalls.h>
#include <tron.h>
#include <colors.h>


#define SIZE 8
#define MAX_HEIGHT 1280
#define MAX_WIDTH 1024

typedef struct {
    uint64_t pos_x;
    uint64_t pos_y;
    uint8_t direction;
    uint8_t state;
    uint64_t color;    
} Player;

Player player1;
Player player2;



uint16_t board[MAX_WIDTH][MAX_HEIGHT];

static void get_screen_data(uint16_t * data){ 
    return sys_get_screen_data(data);
}

static void do_paint_rec(uint64_t from_x, uint64_t from_y, uint16_t width, uint16_t height, uint64_t color){
    return sys_paint_rect(from_x, from_y, height, width, color);
}

static void do_beep(uint64_t frequency, uint64_t nano_seconds){
    sys_beeper(frequency, nano_seconds);
}
//ESTAMOS PONIENDO BOARD COMO B[Y][X] 
//BOARD NO TIENE LOS BORDES EN CUENTA 
static int is_alive(Player  p, uint16_t width, uint16_t height,uint16_t board[MAX_WIDTH][MAX_HEIGHT]) {
    if (p.pos_x < width && p.pos_x > 1 && p.pos_y < height && p.pos_y > 1 ){/*&& board[p.pos_y][p.pos_x]==0*/ //chequeo rango
        board[p.pos_x][p.pos_y]=1;
        return 1;
    }
    return 0;
}

static void update_position(uint64_t pressed_key){
    switch (pressed_key)
            {
            // caso default no existe ya q tiene q hacer algo siempre
            case 'w':
                if (player1.direction != 's' /* && player1.direction != 'w'*/){ //opposite 
                    player1.pos_y--;
                    player1.direction = 'w';
                    do_paint_rec((player1.pos_x)*SIZE, (player1.pos_y)*SIZE, SIZE, SIZE, player1.color);
                }
                break;
            case 'a':
                if(player1.direction != 'd' /*&& player1.direction != 'a'*/){
                    player1.pos_x--;
                    player1.direction = 'a';
                    do_paint_rec((player1.pos_x)*SIZE, (player1.pos_y)*SIZE, SIZE, SIZE, player1.color);
                }
                break;
            case 's':
                if(player1.direction != 'w' /*&& player1.direction != 's'*/){
                    player1.pos_y++;   
                    player1.direction = 's';
                    do_paint_rec((player1.pos_x)*SIZE, (player1.pos_y)*SIZE, SIZE, SIZE, player1.color);
                }
                break;
            case 'd':
                if (player1.direction != 'a' /*&& player1.direction != 'd'*/){
                    player1.pos_x++;
                    player1.direction = 'd';
                    do_paint_rec((player1.pos_x)*SIZE, (player1.pos_y)*SIZE, SIZE, SIZE, player1.color);
                }
                break;
            //Player 2
            case 'U': //up
                if (player2.direction != 'D' /*&& player2.direction != 'U'*/){ //opp opp?
                    player2.pos_y--;
                    player2.direction = 'U';
                    do_paint_rec((player2.pos_x)*SIZE, (player2.pos_y)*SIZE, SIZE, SIZE, player2.color);
                }
                break;
            case 'L': //left
                if(player2.direction != 'R' /*&& player2.direction != 'L'*/){
                    player2.pos_x--;
                    player2.direction = 'L';
                    do_paint_rec((player2.pos_x)*SIZE, (player2.pos_y)*SIZE, SIZE, SIZE, player2.color);
                }
                break;
            case 'D': //down
                if(player2.direction != 'U'/* &&  player2.direction != 'D'*/){
                    player2.pos_y++;   
                    player2.direction = 'D';
                    do_paint_rec((player2.pos_x)*SIZE, (player2.pos_y)*SIZE, SIZE, SIZE, player2.color);
                }
                break;
            case 'R': //right
                if (player2.direction != 'L'/* && player2.direction != 'R'*/){
                    player2.pos_x++;
                    player2.direction = 'R';
                    do_paint_rec((player2.pos_x)*SIZE, (player2.pos_y)*SIZE, SIZE, SIZE, player2.color);
                }
                break;
            }
}



//ejecuta el tron
void play_tron(){
    //seteamos pantalla -> players -> lectura para teclado (wasd player1 y flechas player2) -> 
    uint16_t screen[2] = {0}; 
    get_screen_data(screen); //syscall que nos da los datos de la pantalla . por el momento 0 = width y 1 = height
    set_screen(screen[0], screen[1]);
    set_players(screen[0],screen[1]);
    
    const uint16_t width = (screen[0]-16)/8 ;
    const uint16_t height = (screen[1]-16)/8;
    for(int i = 0; i < width;i++) for(int j = 0; j< height; j++)  board[i][j]=0; 
    uint64_t speed = 1;
    uint64_t ticks = sys_get_ticks();
    uint64_t lastTicks = 0;
    char c;
    char t;
    while(player1.state && player2.state){
         
        if (lastTicks != ticks && ticks % speed == 0){
            c = do_get_char();
            t = do_get_char();
            if (player1.state && player2.state){
                if(c != player1.direction && c != player2.direction)
                update_position(c);
                if(t != player1.direction && t != player2.direction)
                update_position(t);
                update_position(player1.direction);
                update_position(player2.direction);
            }
            if(player1.pos_x == player2.pos_x && player2.pos_y == player1.pos_y){ //Choque de frente!!
                player1.state = 0;
                player2.state = 0;
            } else {
                player2.state = !board[player2.pos_x][player2.pos_y] && is_alive(player2, width+1, height+1, board);
                player1.state = !board[player1.pos_x][player1.pos_y] && is_alive(player1, width+1, height+1, board);
            }
            lastTicks = ticks;
        }
        ticks = sys_get_ticks();

    }
    //prompt: want to play again? y/n => n vuelve al menu
    do_paint_rec(0, 0, screen[0], screen[1]/SIZE*8, BLACK);
    do_change_font_size(2);

    uint64_t freq = 400;
    uint64_t nano_seconds = 30000000; //30000000 nano_seconds = 0.03 seconds
    do_beep(freq, nano_seconds);
    char d;
   
    if(player1.state==1 || player2.state == 1){
        printf("\n\n\n\n\n\n\n\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t   THE WINNER IS: \n");
        printf("\n\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t");
        do_change_font_size(3);
        if(player1.state==1){
            do_print_color("PLAYER PINK!",PINK);
        }
        else if(player2.state==1){
            do_print_color("PLAYER GREEN!",GREEN);
        }
        do_change_font_size(2);
    }
    else{
        printf("\n\n\n\n\n\n\n\n\n\t\t\t\t\t\t\t\t  IT'S A TIE!\n");
    }
    printf("\n\n\n\n\t\t\t\t\t\t\t\t\t\t\t\t\t\tWant to play again? Press y/n \n");
    do_change_font_size(1);
    while(d != 'y' && d != 'n'){
        d = do_get_char();
    }
    if(d == 'y'){
        play_tron();
        return;
    } else {
        do_clear_screen(BLACK);
    }
    
    return;
}


static void paint_borders(uint16_t width, uint16_t height, uint64_t color) {
    do_paint_rec(0, 0, width, SIZE, DARK_BLUE);             // top border 
    do_paint_rec(0, 0, SIZE, height, DARK_BLUE);            // left border
    do_paint_rec(width-SIZE, 0, SIZE, height, DARK_BLUE);   // right bor
    do_paint_rec(0, height-SIZE, width, SIZE, DARK_BLUE);   // bottom border
}

//hace la ventana en donde se juega al tron
void set_screen(uint16_t width, uint16_t height){
    
    do_clear_screen(SCREEN_TRON); //pintar pantalla d azul
    paint_borders(width, height, TRON_COLOR_3);
}


void set_players(uint16_t width, uint16_t height){

    player1.pos_x = (width/4)/SIZE;
    player1.pos_y = (height/2)/SIZE;
    player1.direction = 'd'; 
    player1.color = PINK;
    player1.state = 1;
    do_paint_rec((player1.pos_x)*SIZE, (player1.pos_y)*SIZE, SIZE, SIZE, player1.color);          
    

    player2.pos_x = (width - width/4)/SIZE;
    player2.pos_y = (height/2)/SIZE;
    player2.direction = 'L';
    player2.color = GREEN;
    player2.state = 1;
    do_paint_rec((player2.pos_x)*SIZE, (player2.pos_y)*SIZE, SIZE, SIZE, player2.color); 
}

