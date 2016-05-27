//============================================================================
// Name        : simple-web-server-c.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

int main(int argc, char **argv)
{
  int listenfd,connfd;
  struct sockaddr_in servaddr,cliaddr;
  socklen_t len = sizeof(cliaddr);
  char cli_ip[32];
  char mesg[1024];
  char response[1028];
  //char reply[] = "Hello World!";

  listenfd = socket(PF_INET,SOCK_STREAM,0);
  bzero(&servaddr, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  servaddr.sin_port = htons(54321);

  if ( bind( listenfd, (struct sockaddr*) &servaddr, sizeof(servaddr) ) < 0 ){
      perror(NULL);
      exit(-1);
  }

  while(1){
    //not present in udp server
    connfd = accept(listenfd,(struct sockaddr *)&cliaddr,&len);
    inet_ntop(AF_INET,(struct in_addr *) &cliaddr.sin_addr, cli_ip, sizeof(cli_ip) );
    printf("Client %s connected. \n",cli_ip);

    while(1){
      memset(mesg,0,sizeof(mesg));
      if( recvfrom(connfd,mesg,sizeof(mesg),0,(/*const*/ struct sockaddr *)&cliaddr,&len) > 0 ){
            printf("From %s port %d: %s",cli_ip,ntohs(cliaddr.sin_port),mesg);
        **sprintf(response,"HTTP/1.1 200 OK \n Content-type:text/html \n Content-length: 200 \r\n <html><body> Hello, World! </body></html>");
        sendto(connfd,response,strlen(response),0,(const struct sockaddr *)&servaddr,len);
        //break;**
      }
      else {
        printf("Client %s disconnected.\n",cli_ip);
        break;
      }
    }

    close(connfd);
  }

  close(listenfd);

  return 0;
}
