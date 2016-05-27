/*
 ============================================================================
 Name        : somple-web-server-c.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <err.h>

//#include "timer.h"



int main()
{
  int one = 1, client_fd;
  struct sockaddr_in svr_addr, cli_addr;
  socklen_t sin_len = sizeof(cli_addr);

  //hrt_init(); hrt_sync_clock();


  int sock = socket(AF_INET, SOCK_STREAM, 0);
  if (sock < 0)
    err(1, "can't open socket");

  setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &one, sizeof(int));

  int port = 8080;
  svr_addr.sin_family = AF_INET;
  svr_addr.sin_addr.s_addr = INADDR_ANY;
  svr_addr.sin_port = htons(port);

  if (bind(sock, (struct sockaddr *) &svr_addr, sizeof(svr_addr)) == -1) {
    close(sock);
    err(1, "Can't bind");
  }

  listen(sock, 5);
  while (1) {
	char *connected_ip; //= inet_ntoa(client.sin_addr);
	//uint64_t t1=hrt_timer_now();
    client_fd = accept(sock, (struct sockaddr *) &cli_addr, &sin_len);
    connected_ip=inet_ntoa(cli_addr.sin_addr);
    //uint64_t t2=hrt_timer_now();

    printf("got connection from %s \n", connected_ip);

    if (client_fd == -1) {
      perror("Can't accept");
      continue;
    }
    char buff[1024];
    char* response = "HTTP/1.1 200 OK\r\n"
    "Content-Type: text/html; charset=UTF-8\r\n\r\n"
    "<doctype !html><html><head><title>simple-page</title>"
    "<style>body { background-color: #999 }"
    "h1 { font-size:1cm; text-align: left; color: black;"
    " text-shadow: 0 0 2mm white}</style></head>";
    //"<body><h1>test-page</h1></body></html>\r\n";
    sprintf(buff,"%s <body><h1>Clients IP is %s</h1></body></html>\r\n ",response,connected_ip);
    write(client_fd, buff, strlen(buff) - 1); /*-1:'\0'*/
    close(client_fd);
  }
}
