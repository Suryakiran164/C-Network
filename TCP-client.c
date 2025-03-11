/* ************************************ *
 * Simple implementation of TCP client  *
 * 11/03/2025 - by @Suryakiran164       *
 * ************************************ */
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>

#define IP "142.250.183.46"     /*     IP of Google.com */
#define PORT 80                 /*      HTTPS PORT      */

int main()
{
        int s;
        struct sockaddr_in sock;
        char *data;
        char buf[512];

        data = "HEAD / HTTP/1.1\r\n\r\n";

        s = socket(AF_INET, SOCK_STREAM, 0);
        if ( s < 0)
        {
                printf("socker() error!\n");
                return -1;
        }

        sock.sin_addr.s_addr = inet_addr(IP);
        sock.sin_port = htons(PORT);
        sock.sin_family = AF_INET;

        if(connect(s, (struct sockaddr *)&sock, sizeof(struct sockaddr_in)) != 0)
        {
                printf("connet() error!\n");
                close(s);

                return -1;
        }

        write(s, data, strlen(data));
        memset(buf, 0, 512);

        read(s, buf, 511);
        close(s);

        printf("[+] RESPONSE RECIEVED :\n\n%s\n\n[+] RESPONSE ENDS HERE [+]\n\n", buf);

        return 0;
}
