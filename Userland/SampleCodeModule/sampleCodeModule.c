// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <shell.h>
#include <syscalls.h>
          
char * draw = "\n\n\n\n\n\n\n\n                                                       __,__\n"
        "                                              .--.  .-\"     \"-.  .--.\n"
        "                                             / .. \\/  .-. .-.  \\/ .. \\\n"
        "                                            | |  '|  /   Y   \\  |'  | |\n"
        "                                            | \\   \\  \\ 0 | 0 /  /   / |\n"
        "                                             \\ '- ,\\.-\"`` ``\"-./, -' /\n"
        "                                              `'-'/_   ^ ^   _\\ '-'`\n"
        "                                              .--'|  \\._   _./  |'--.\n"
        "                                            /`    \\   \\ `~` /   /    `\\\n"
        "                                           /       '._ '---' _.'       \\\n"
        "                                          /           '~---~'   |       \\\n"
        "                                         /        _.             \\       \\\n"
        "                                        /   .'-./`/        .'~'-.|\\       \\\n"
        "                                       /   /    `\\:       /      `\\'.      \\\n"
        "                                      /   |       ;      |         '.`;    /\n"
        "                                      \\   \\       ;      \\           \\/   /\n"
        "                                       '.  \\      ;       \\       \\   `  /\n"
        "                                         '._'.     \\       '.      |   ;/_\n"
        "                                           /__>     '.       \\_ _ _/   ,  '--.\n"
        "                                         .'   '.   .-~~~~~-. /     |--'`~~-.  \\\n"
        "                                        // / .---'/  .-~~-._/ / / /---..__.'  /\n"
        "                                       ((_(_/    /  /      (_(_(_(---.__    .'\n"
        "                                                 | |     _              `~~`\n"
        "                                                 | |     \\'.\n"
        "                                                  \\ '....' |\n"
        "                                                   '.,___.'\n";

char *welcome_message = "\n\n\n\n\n\n\n\n\n\n\n\n                     ____    __    ____  _______  __        ______   ______   .___  ___.  _______   \n"
                        "                     \\   \\  /  \\  /   / |   ____||  |      /      | /  __  \\  |   \\/   | |   ____|  \n"
                        "                      \\   \\/    \\/   /  |  |__   |  |     |  ,----'|  |  |  | |  \\  /  | |  |__     \n"
                        "                       \\            /   |   __|  |  |     |  |     |  |  |  | |  |\\/|  | |   __|    \n"
                        "                        \\    /\\    /    |  |____ |  `----.|  `----.|  `--'  | |  |  |  | |  |____   \n"
                        "                         \\__/  \\__/     |_______||_______| \\______| \\______/  |__|  |__| |_______|  \n"
                        "                                             .___________.  ______                                 \n"
                        "                                             |           | /  __  \\                                 \n"
                        "                                             `---|  |----`|  |  |  |                                \n"
                        "                                                 |  |     |  |  |  |                                \n"
                        "                                                 |  |     |  `--'  |                                \n"
                        "                                                 |__|      \\______/                                 \n"
                        "                     .___  ___.      ___       ______      ___       ______   ______        _______.\n"
                        "                     |   \\/   |     /   \\     /      |    /   \\     /      | /  __  \\      /       |\n"
                        "                     |  \\  /  |    /  ^  \\   |  ,----'   /  ^  \\   |  ,----'|  |  |  |    |   (----` \n"
                        "                     |  |\\/|  |   /  /_\\  \\  |  |       /  /_\\  \\  |  |     |  |  |  |     \\   \\     \n"
                        "                     |  |  |  |  /  _____  \\ |  `----. /  _____  \\ |  `----.|  `--'  | .----)   |    \n"
                        "                     |__|  |__| /__/     \\__\\ \\______|/__/     \\__\\ \\______| \\______/  |_______/    \n";

int main() {
	do_print_color(welcome_message, YELLOW);
        sys_wait(300000000);
        for(int i = 0; i < 60; i++) {
                sys_wait(6000000);
                printf("\n");
        }
        sys_clear_screen(BLACK);
        do_print_color(draw, RED);
        sys_wait(300000000);
        for(int i = 0; i < 60; i++) {
                sys_wait(6000000);
                printf("\n");
        }
	sys_clear_screen(BLACK);
	do_print_color("WELCOME! Type \"HELP\" for command list\n", WHITE);
	shell();
	return 0;
}
