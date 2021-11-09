// Server side C/C++ program to demonstrate Socket programming
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <pwd.h>
#define PORT 8080

typedef struct passwd defpasswd;
typedef struct sockaddr_in defsockaddr_in;

int main(int argc, char const *argv[])
{
    int server_fd, new_socket, valread;
    defsockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
    char *hello = "Hello from server";

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR,
                   &opt, sizeof(opt)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (defsockaddr_in *)&address,
             sizeof(address)) < 0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 3) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    if ((new_socket = accept(server_fd, (defsockaddr_in *)&address,
                             (socklen_t *)&addrlen)) < 0)
    {
        perror("accept");
        exit(EXIT_FAILURE);
    }
    int pid = fork();
    int STATUS = 0;
    if (pid < 0)
    {
        perror("Fork fucntion failed");
        return EXIT_FAILURE;
    }
    if (pid > 0)
    {
        waitpid(pid, &STATUS, 0);
        return EXIT_FAILURE;
    }
    if (pid == 0)
    {
        printf("Child UID is: %d\n", (int)getpwnam("nobody"));
        setuid(getpwnam("nobody")->pw_uid);

        char args_sock[150];
        sprintf(args_sock, "%d", new_socket);
        char args_original[150];
        sprintf(args_original, "%s", argv[0]);
        char *args[] = {args_original, args_sock, NULL};
        execvp(args[0], args);
    }
    else
    {
        new_socket = atoi(argv[1]);
        valread = read(new_socket, buffer, 1024);
        printf("Read %d bytes: %s\n", valread, buffer);
        send(new_socket, hello, strlen(hello), 0);
        printf("Hello message sent\n");
    }

    return 0;
}
