#include <arpa/inet.h>
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define PORT 9990
#define SIZE 1024

int main()
{
    int client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1) {
        std::cerr << "socket" << std::endl;
        return -1;
    }

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));

    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    inet_aton("127.0.0.1", &(addr.sin_addr));
    int conn = connect(client_socket, (struct sockaddr *)&addr, sizeof(addr));
    if (conn == -1) {
        std::cerr << "connect" << std::endl;
        return -1;
    }
    std::cout << "成功连接服务器。。。。" << std::endl;

    char buf[SIZE] = {0};

    while (1) {
        std::cout << "输入要发送的数据:" << std::endl;
        scanf("%s", buf);
        write(client_socket, buf, strlen(buf));

        int ret = read(client_socket, buf, strlen(buf));
        if (ret == -1) {
            std::cerr << "read" << std::endl;
            return -1;
        }
        printf("buf=%s", buf);
        if (strncmp(buf, "END", 3) == 0) {
            break;
        }
    }
    close(conn);
    return 0;
}