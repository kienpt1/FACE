#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8888
#define BUFFER_SIZE 2048

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <string1> [<string2> ...]\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int sock = 0;
    struct sockaddr_in serv_addr;
    char buffer[BUFFER_SIZE] = {0};

    // Create socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation error");
        exit(EXIT_FAILURE);
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Convert IPv4 and IPv6 addresses from text to binary form
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        perror("Invalid address/ Address not supported");
        exit(EXIT_FAILURE);
    }

    // Connect to server
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Connection failed");
        exit(EXIT_FAILURE);
    }

    // For each string argument
    for (int i = 1; i < argc; i++) {
        // Send string to server
        send(sock, argv[i], strlen(argv[i]), 0);
        printf("String sent to server: %s\n", argv[i]);

        // Get client's address and port
        struct sockaddr_in client_addr;
        int addr_len = sizeof(client_addr);
        getsockname(sock, (struct sockaddr *)&client_addr, (socklen_t *)&addr_len);
        printf("Client address: %s:%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

        // Receive response from server
        read(sock, buffer, BUFFER_SIZE);
        printf("Response from server: %s\n", buffer);
    }

    // Close socket
    close(sock);

    return 0;
}
