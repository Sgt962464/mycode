#include <arpa/inet.h>
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define PORT 9990 // 端口号
#define SIZE 1024 // 数据大小

int Creat_socket() // 创建socket及初始化
{
    int ser_socket = socket(AF_INET, SOCK_STREAM, 0); // 用于监听套接字
    if (ser_socket == -1) {
        std::cerr << "scoket" << std::endl;
        return -1;
    }

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));

    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);

    int ret = bind(ser_socket, (struct sockaddr *)&addr, sizeof(addr));
    if (ret == -1) {
        std::cerr << "bind" << std::endl;
        return -1;
    }

    ret = listen(ser_socket, 3);
    if (ret == -1) {
        std::cerr << "listen" << std::endl;
        return -1;
    }

    return ser_socket;
}

int wait_client(int ser_socket)
{
    struct sockaddr_in cli_addr;
    int len = sizeof(cli_addr);
    std::cout << "等待连接。。。" << std::endl;
    int cli_socket = accept(ser_socket, (struct sockaddr *)&cli_addr, (socklen_t *)&len);
    if (cli_socket == -1) {
        std::cerr << "accept" << std::endl;
        return -1;
    }

    std::cout << "成功接收:" << inet_ntoa(cli_addr.sin_addr);
    return cli_socket;
}

void hand_client(int listen_socket, int cli_socket)
{
    char buf[SIZE];
    while (1) {
        int ret = read(cli_socket, buf, SIZE - 1);
        if (ret == -1) {
            std::cerr << "read" << std::endl;
        }
        if (ret == 0)
            break;
        buf[ret] = '\0';
        for (int i = 0; i < ret; ++i) {
            buf[i] = buf[i] + 'A' - 'a';
        }

        printf("%s\n", buf);
        write(cli_socket, buf, ret);

        if (strncmp(buf, "end", 3) == 0) {
            break;
        }
    }

    close(cli_socket);
}

int main()
{
    int ser_socket = Creat_socket();
    int cli_socket = wait_client(ser_socket);
    hand_client(ser_socket, cli_socket);
    close(ser_socket);
    return 0;
}
