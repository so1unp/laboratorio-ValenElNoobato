#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>           /* For O_* constants */
#include <sys/stat.h>        /* For mode constants */
#include <mqueue.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>

#define USERNAME_MAXSIZE    15  // Máximo tamaño en caracteres del nombre del remitente.
#define TXT_SIZE            100 // Máximo tamaño del texto del mensaje.

/**
 * Estructura del mensaje:
 * - sender: nombre del usuario que envió el mensaje.
 * - text: texto del mensaje.
 */
struct msg {
    char sender[USERNAME_MAXSIZE];
    char text[TXT_SIZE];
};

typedef struct msg msg_t;

/**
 * Imprime información acerca del uso del programa.
 */
void usage(char *argv[])
{
    fprintf(stderr, "Uso: %s comando parametro\n", argv[0]);
    fprintf(stderr, "Comandos:\n");
    fprintf(stderr, "\t-s queue mensaje: escribe el mensaje en queue.\n");
    fprintf(stderr, "\t-r queue: imprime el primer mensaje en queue.\n");
    fprintf(stderr, "\t-a queue: imprime todos los mensaje en queue.\n");
    fprintf(stderr, "\t-l queue: vigila por mensajes en queue.\n");
    fprintf(stderr, "\t-c queue: crea una cola de mensaje queue.\n");
    fprintf(stderr, "\t-d queue: elimina la cola de mensajes queue.\n");
    fprintf(stderr, "\t-i queue: imprime información de la cola de mensajes queue.\n");
    fprintf(stderr, "\t-h imprime este mensaje.\n");
}

void send_message(char *queue_name, char *msg_text)
{
    mqd_t mq;
    msg_t msg;
    struct mq_attr attr;
    
    attr.mq_flags = 0;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = sizeof(msg_t);
    attr.mq_curmsgs = 0;

    mq = mq_open(queue_name, O_WRONLY | O_CREAT, 0644, &attr);
    if (mq == -1) {
        perror("mq_open");
        exit(EXIT_FAILURE);
    }

    if (getlogin_r(msg.sender, USERNAME_MAXSIZE) != 0) {
        perror("getlogin_r");
        exit(EXIT_FAILURE);
    }
    strncpy(msg.text, msg_text, TXT_SIZE);

    if (mq_send(mq, (char*)&msg, sizeof(msg_t), 0) == -1) {
        perror("mq_send");
        exit(EXIT_FAILURE);
    }

    mq_close(mq);
}

void receive_message(char *queue_name)
{
    mqd_t mq;
    msg_t msg;
    
    mq = mq_open(queue_name, O_RDONLY | O_NONBLOCK);
    if (mq == -1) {
        perror("mq_open");
        exit(EXIT_FAILURE);
    }

    if (mq_receive(mq, (char*)&msg, sizeof(msg_t), NULL) == -1) {
        if (errno == EAGAIN) {
            printf("No hay mensajes\n");
            mq_close(mq);
            exit(EXIT_SUCCESS);
        } 
        perror("mq_receive");
        mq_close(mq);
        exit(EXIT_FAILURE);
    }

    printf("Remitente: %s\n", msg.sender);
    printf("Mensaje: %s\n", msg.text);

    mq_close(mq);
}

void receive_all_messages(char *queue_name)
{
    mqd_t mq;
    msg_t msg;
    struct mq_attr attr;
    
    mq = mq_open(queue_name, O_RDONLY | O_NONBLOCK);
    if (mq == -1) {
        perror("mq_open");
        exit(EXIT_FAILURE);
    }

    if (mq_getattr(mq, &attr) == -1) {
        perror("mq_getattr");
        exit(EXIT_FAILURE);
    }

    if (attr.mq_curmsgs == 0) {
        printf("No hay mensajes\n");
        exit(EXIT_SUCCESS);
    }

    while (mq_receive(mq, (char*)&msg, sizeof(msg_t), NULL) != -1) {
        printf("Remitente: %s\n", msg.sender);
        printf("Mensaje: %s\n", msg.text);
    }

    if (errno != EAGAIN) {
        perror("mq_receive");
        exit(EXIT_FAILURE);
    }

    mq_close(mq);
}

void listen_for_messages(char *queue_name)
{
    mqd_t mq;
    msg_t msg;
    struct mq_attr attr;
    
    mq = mq_open(queue_name, O_RDONLY);
    if (mq == -1) {
        perror("mq_open");
        exit(EXIT_FAILURE);
    }

    if (mq_getattr(mq, &attr) == -1) {
        perror("mq_getattr");
        exit(EXIT_FAILURE);
    }

    while (1) {
        if (mq_receive(mq, (char*)&msg, sizeof(msg_t), NULL) != -1) {
            printf("Remitente: %s\n", msg.sender);
            printf("Mensaje: %s\n", msg.text);
        } else {
            perror("mq_receive");
        }
    }

    mq_close(mq);
}

void create_queue(char *queue_name)
{
    mqd_t mq;
    struct mq_attr attr;
    
    attr.mq_flags = 0;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = sizeof(msg_t);
    attr.mq_curmsgs = 0;

    mq = mq_open(queue_name, O_CREAT | O_RDONLY, 0644, &attr);
    if (mq == -1) {
        perror("mq_open");
        exit(EXIT_FAILURE);
    }

    mq_close(mq);
}

void delete_queue(char *queue_name)
{
    if (mq_unlink(queue_name) == -1) {
        perror("mq_unlink");
        exit(EXIT_FAILURE);
    }
}

void print_queue_info(char *queue_name)
{
    mqd_t mq;
    struct mq_attr attr;
    
    mq = mq_open(queue_name, O_RDONLY);
    if (mq == -1) {
        perror("mq_open");
        exit(EXIT_FAILURE);
    }

    if (mq_getattr(mq, &attr) == -1) {
        perror("mq_getattr");
        exit(EXIT_FAILURE);
    }

    printf("Máximo número de mensajes: %ld\n", attr.mq_maxmsg);
    printf("Tamaño de los mensajes: %ld\n", attr.mq_msgsize);
    printf("Número actual de mensajes en la cola: %ld\n", attr.mq_curmsgs);

    mq_close(mq);
}

int main(int argc, char *argv[])
{
    if (argc < 2) {
        usage(argv);
        exit(EXIT_FAILURE);
    }

    if (argv[1][0] != '-') {
        usage(argv);
        exit(EXIT_FAILURE);
    }

    char option = argv[1][1];

    switch(option) {
        case 's':
            if (argc < 4) {
                usage(argv);
                exit(EXIT_FAILURE);
            }
            send_message(argv[2], argv[3]);
            break;
        case 'r':
            if (argc < 3) {
                usage(argv);
                exit(EXIT_FAILURE);
            }
            receive_message(argv[2]);
            break;
        case 'a':
            if (argc < 3) {
                usage(argv);
                exit(EXIT_FAILURE);
            }
            receive_all_messages(argv[2]);
            break;
        case 'l':
            if (argc < 3) {
                usage(argv);
                exit(EXIT_FAILURE);
            }
            listen_for_messages(argv[2]);
            break;
        case 'c':
            if (argc < 3) {
                usage(argv);
                exit(EXIT_FAILURE);
            }
            create_queue(argv[2]);
            break;
        case 'd':
            if (argc < 3) {
                usage(argv);
                exit(EXIT_FAILURE);
            }
            delete_queue(argv[2]);
            break;
        case 'i':
            if (argc < 3) {
                usage(argv);
                exit(EXIT_FAILURE);
            }
            print_queue_info(argv[2]);
            break;
        case 'h':
            usage(argv);
            break;
        default:
            fprintf(stderr, "Comando desconocido: %s\n", argv[1]);
            exit(EXIT_FAILURE);
    }
    
    exit(EXIT_SUCCESS);
}
