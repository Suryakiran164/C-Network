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

/* structures */
struct sHttpRequest
{
        char method[10];
        char url[128];
};
typedef struct sHttpRequest httpreq;

/* global */
char *error;

/* dummy for testing purposes only - needs implementation */
void cli_conn(int s, int c)
{
        return;
}

/* return 0 on error, or returns a httpreq structure */
httpreq *parse_http(char *str)
{
        httpreq *req;
        char *p;

        req = malloc(sizeof(httpreq));

        for (p = str; p && *p != ' '; p++);

        if (*p == ' ')
                *p = 0;
        else
        {
                error = "parse_http() NOSPACE error\n";
                free(req);

                return 0;
        }

        strncpy(req->method, str, 7);
        return req;

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
        char buff[512];
        char *template;
        httpreq *req;

        template =
                "GET / HTTP/1.1\n"
                "Host: localhost:8081GET / HTTP/1.1\n"
                "User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:136.0) Gecko/20100101 Firefox/136.0\n"
                "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\n"
                "Accept-Language: en-US,en;q=0.5\n"
                "Accept-Encoding: gzip, deflate, br, zstd\n"
                "Connection: keep-alive\n"
                "Upgrade-Insecure-Requests: 1\n"
                "Sec-Fetch-Dest: document\n"
                "Sec-Fetch-Mode: navigate\n"
                "Sec-Fetch-Site: none\n"
                "Sec-Fetch-User: ?1\n"
                "Priority: u=0, i\n"
                "\n"
                "\n";

        memset(buff, 0, 512);
        strncpy(buff, template, 511);

        parse_http(buff);
        if (!req)
                fprintf(stderr, "%s\n", error);
        else
                printf("Method: '%s'\nURL: '%s'\n", req->method, req->url);

        return 0;

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