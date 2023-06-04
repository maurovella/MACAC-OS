/*#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <signal.h>
#include <time.h>

#define MAX_FILOSOFOS 10
#define IZQUIERDA(id) ((id) + num_filosofos - 1) % num_filosofos
#define DERECHA(id) ((id) + 1) % num_filosofos

enum { PENSANDO, HAMBRIENTO, COMIENDO };

typedef struct {
    int id;
    int estado;
} Filosofo;

int semid;
int num_filosofos;
Filosofo* filosofos;


void tomar_tenedores(Filosofo* filosofo) {
    int tenedor_izquierdo = filosofo->id;
    int tenedor_derecho = (filosofo->id + 1) % num_filosofos;

    // Intentar tomar el tenedor izquierdo
    if (semctl(semid, tenedor_izquierdo, GETVAL, 0) == 1) {
        printf("Filósofo %d: Esperando tenedor izquierdo\n", filosofo->id);
        sem_wait(&semid, tenedor_izquierdo);
    }

    // Intentar tomar el tenedor derecho
    if (semctl(semid, tenedor_derecho, GETVAL, 0) == 1) {
        printf("Filósofo %d: Esperando tenedor derecho\n", filosofo->id);
        sem_wait(&semid, tenedor_derecho);
    }
}

void soltar_tenedores(Filosofo* filosofo) {
    int tenedor_izquierdo = filosofo->id;
    int tenedor_derecho = (filosofo->id + 1) % num_filosofos;

    sem_post(&semid, tenedor_izquierdo);
    sem_post(&semid, tenedor_derecho);
}

void verificar(Filosofo* filosofo) {
    if (filosofo->estado == HAMBRIENTO &&
        filosofos[IZQUIERDA(filosofo->id)].estado != COMIENDO &&
        filosofos[DERECHA(filosofo->id)].estado != COMIENDO) {
        filosofo->estado = COMIENDO;
        sem_post(&semid, filosofo->id);
    }
}

void filosofo(Filosofo* filosofo) {
    while (1) {
        // Filósofo pensando
        // ...

        // Filósofo tiene hambre y quiere comer
        tomar_tenedores(filosofo);

        // Filósofo comiendo
        // ...

        // Filósofo terminó de comer y suelta los tenedores
        soltar_tenedores(filosofo);
    }
}

// Resto del código...

int main() {
    num_filosofos = 5;
    filosofos = memory_alloc(num_filosofos * sizeof(Filosofo));

    // Crear e inicializar los semáforos
    semid = semget(IPC_PRIVATE, num_filosofos, IPC_CREAT | 0666);
    for (int i = 0; i < num_filosofos; i++) {
        semctl(semid, i, SETVAL, 1);
    }

    // Configurar el manejo de la señal SIGINT (Ctrl+C)
    signal(SIGINT, manejar_senal);

    // Crear los procesos de los filósofos
    pid_t pid;
    for (int i = 0; i < num_filosofos; i++) {
        filosofos[i].id = i;
        filosofos[i].estado = PENSANDO;

        pid = fork();
        if (pid < 0) {
            printf("Error al crear el proceso del filósofo %d\n", i);
            return 1;
        } else if (pid == 0) {
            // Proceso hijo (filósofo)
            filosofo(&filosofos[i]);
            return 0;
        }
    }

    // Ciclo principal
    while (1) {
        // Leer la entrada del usuario
        char opcion;
        scanf(" %c", &opcion);

        // Realizar la acción correspondiente según la opción seleccionada
        switch (opcion) {
            case 'a':
                if (num_filosofos < MAX_FILOSOFOS) {
                    agregar_filosofo(filosofos);
                } else {
                    printf("Se ha alcanzado el máximo número de filósofos.\n");
                }
                break;
            case 'e':
                if (num_filosofos > 1) {
                    eliminar_filosofo(filosofos);
                } else {
                    printf("No se puede eliminar más filósofos. Mínimo alcanzado.\n");
                }
                break;
            default:
                printf("Opción inválida. Intenta de nuevo.\n");
                break;
        }
    }

    // Liberar memoria
    free(filosofos);

    return 0;
}


void agregar_filosofo(Filosofo* filosofos) {
    if (num_filosofos >= MAX_FILOSOFOS) {
        printf("Se ha alcanzado el máximo número de filósofos.\n");
        return;
    }

    // Incrementar el contador de filósofos
    num_filosofos++;

    // Asignar el nuevo filósofo
    filosofos[num_filosofos - 1].id = num_filosofos - 1;
    filosofos[num_filosofos - 1].estado = PENSANDO;

    // Crear el proceso del nuevo filósofo
    pid_t pid = fork();
    if (pid < 0) {
        printf("Error al crear el proceso del filósofo %d\n", num_filosofos - 1);
        return;
    } else if (pid == 0) {
        // Proceso hijo (nuevo filósofo)
        filosofo(&filosofos[num_filosofos - 1]);
        return;
    }
}

void eliminar_filosofo(Filosofo* filosofos) {
    if (num_filosofos <= 1) {
        printf("No se puede eliminar más filósofos. Mínimo alcanzado.\n");
        return;
    }

    // Obtener un índice de filósofo aleatorio para eliminar
    int indice_eliminar = rand() % num_filosofos;

    // Filósofo a eliminar
    Filosofo filosofo_eliminar = filosofos[indice_eliminar];

    // Esperar a que el filósofo termine su ejecución
    int estado_hijo;
    waitpid(filosofo_eliminar.pid, &estado_hijo, 0);

    // Desplazar los filósofos restantes hacia la izquierda para llenar el espacio vacío
    for (int i = indice_eliminar; i < num_filosofos - 1; i++) {
        filosofos[i] = filosofos[i + 1];
    }

    // Decrementar el contador de filósofos
    num_filosofos--;
}
*/