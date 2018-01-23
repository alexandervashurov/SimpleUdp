#include <sys/types.h>
#include <cstdlib>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstring>
#include <cstdio>
#include <unistd.h>

#define PORT 66666
#define BUFFLENGTH 1000

int main(int argc, char **argv) {
    int Socket;

    char sendline[BUFFLENGTH], recvline[BUFFLENGTH];

    struct sockaddr_in server_addr, client_addr;

    if (argc != 2) {
        printf("Usage: client.out <IP address>\n");
        return EXIT_FAILURE;
    }

    /* Создаем UDP сокет */
    if ((Socket = socket(PF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("An error occurred while creating socket");
        return EXIT_FAILURE;
    }

    // Обнуляем структуру
    bzero(&client_addr, sizeof(client_addr));

    //Заполняем структуру для адреса клиента
    client_addr.sin_family = AF_INET;
    client_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    client_addr.sin_port = htons(0);


    /* Настраиваем адрес сокета */
    if (bind(Socket, (struct sockaddr *) &client_addr, sizeof(client_addr)) < 0) {
        perror("An error occurred while binding socket");
        close(Socket);
        return EXIT_FAILURE;
    }

    // Обнуляем структуру
    bzero(&server_addr, sizeof(server_addr));

    //Заполняем структуру для адреса сервера
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);

    if (inet_aton(argv[1], &server_addr.sin_addr) == 0) {
        printf("Invalid IP address\n");
        close(Socket);
        return (EXIT_FAILURE);
    }

    /* Отсылаем датаграмму */
    while (1) {
        printf("String => ");
        fgets(sendline, BUFFLENGTH, stdin);

        if (sendto(Socket, sendline, strlen(sendline) + 1, 0, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0) {
            perror("An error occurred while send message");
            close(Socket);
            return (EXIT_FAILURE);
        }

        if ((recvfrom(Socket, recvline, BUFFLENGTH, 0, (struct sockaddr *) NULL, NULL)) < 0) {
            perror("An error occurred while receive message");
            close(Socket);
            return (EXIT_FAILURE);
        }

        printf("%s\n", recvline);
    }
    close(Socket);
    return EXIT_SUCCESS;
}