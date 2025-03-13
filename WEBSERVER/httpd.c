/* httpd.c */
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>

#define LISTENADDR "127.0.0.1"

/* global */
char *error;

/* dummy for testing purposes only - needs implementation */
void cli_conn()
{
        return;
}

/* returns 0 on error, or returns the new client's socket fd */
int cli_accept(int s)
{
        int c;
        socklen_t addrlen;
        struct sockaddr_in cli;

        addrlen = 0;
        memset(&cli, 0, sizeof(cli));
        c = accept(s, (struct sockaddr *)&cli, &addrlen);
        if (c < 0)
        {
                error = "accept() error!\n";
                return 0;
        }

        return c;
}


/* returns 0 on error, or it returns a socket fd */
int srv_init(int portno)
{
        int s;
        struct sockaddr_in srv;

        s = socket(AF_INET, SOCK_STREAM, 0);
        if (s < 0)
        {
                error = "socket() error!\n";
                return 0;
        }

        srv.sin_family = AF_INET;
        srv.sin_addr.s_addr = inet_addr(LISTENADDR);
        srv.sin_port = portno;

        if (bind(s, (struct sockaddr *)&srv, sizeof(srv)))
        {
                error = "bind() error!\n";
                close(s);
                return 0;
        }

        if (listen(s, 5))
        {
                close(s);
                error = "listen() error!\n";
                return 0;
        }

        return s;
}

int main(int argc, char *argv[])
{
        int s;
        char *port;
        int c;

        if (argc < 2)
        {
                fprintf(stderr, "\n[+] Usage: %s <listening_port>\n", argv[0]);
                return -1;
        }
        else
                port = argv[1];

        s = srv_init(atoi(port));

        if (!s)
        {
                fprintf(stderr, "[+] %s\n", error);
                return -1;
        }

        printf("[+] Listening on %s:%s\n", LISTENADDR, port);
        while (1)
        {
                c = cli_accept(s);
                if (!c)
                {
                        fprintf(stderr, "[+] %s\n", error);
                        continue;
                }

                printf("[+] Incoming connection\n");
                if (!fork())
                        cli_conn(s, c);
        }

        return -1;
}

