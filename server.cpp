#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 66666
#define BUFFLENGTH 999

int main(int argc, char **argv) {
    char recvline[BUFFLENGTH];
    socklen_t client_len;

    int Socket;
    struct sockaddr_in server_addr, client_addr;

    // Создаем UDP сокет
    if ((Socket = socket(PF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("An error occurred while creating socket");
        return EXIT_FAILURE;
    }

    // Обнуляем структуру
    bzero(&server_addr, sizeof(server_addr));

    //Заполняем структуру для одного сервера
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(PORT);

    /* Настраиваем адрес сокета */
    if (bind(Socket, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0) {
        perror("An error occurred while binding socket");
        close(Socket);
        return EXIT_FAILURE;
    }

    while (true) {
        //client_len - максимальная длина для ожидаемого адреса клиента
        client_len = sizeof(client_addr);
        if (recvfrom(Socket, recvline, BUFFLENGTH, 0, (struct sockaddr *) &client_addr, &client_len) < 0) {
            perror("An error occurred while receive message");
            close(Socket);
            return (EXIT_FAILURE);
        }

        printf("%s\n", recvline);

        if (sendto(Socket, recvline, strlen(recvline), 0, (struct sockaddr *) &client_addr, client_len) < 0) {
            perror("An error occurred while send message");
            close(Socket);
            return (EXIT_FAILURE);
        }
    }

    close(Socket);
    return EXIT_SUCCESS;
}