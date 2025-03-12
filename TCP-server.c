/* *********************************** *
 * Simple implemantation of TCP server *
 * 12/03/2025 - by @Suryakiran164      *
 * *********************************** */

#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>

#define PORT 8081

int main()
{
        int s, c;
        socklen_t addrlen;
        struct sockaddr_in srv, cli;
        char buf[512];
        char *data ;

        data = "\n\n[+] Connection established successfully on port 8081\n\n";
        addrlen = 0;
        memset(&srv, 0, sizeof(srv));
        memset(&cli, 0, sizeof(cli));
        s = socket(AF_INET, SOCK_STREAM, 0);
        if (s < 0)
        {
                printf("socket() error!\n");
                close(s);

                return -1;
        }

        srv.sin_family = AF_INET;
        srv.sin_addr.s_addr = 0;
        srv.sin_port = htons(PORT);

        if (bind(s, (struct sockaddr *)&srv, sizeof(srv)))
        {
                printf("bind() fail!\n");
                close(s);

                return -1;
        }

        if (listen(s, 5))
        {
                printf("listen() fail!\n");
                close(c);

                return -1;
        }

        c = accept(s, (struct sockaddr *)&srv, &addrlen);

        if (c < 0)
        {
                printf("accept() fail!\n");
                close(c);

                return -1;
        }

        printf("\n[+] Client connected on port 8081!\n\n");

        read(c, buf, 511);

        write(c, data, strlen(data));

        data = "\n[+] Closing Connection...\n";
        write(c, data, strlen(data));
        close(c);

        printf("\n[+] Closing Server...\n");
        close(s);

        return 0;
}
