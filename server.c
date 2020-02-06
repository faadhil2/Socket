#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <sys/errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>
#include <arpa/inet.h>

#define MAXLINE 100

int c,s,port;
struct sockaddr_in server, client;
char *ipaddress = "127.0.1.172"; // what IP address are we supposed to used?
int clientlen = sizeof(client);
int portset=0;
char *message;
FILE * fp;
char config[MAXLINE];
extern int errno;

int main(int argc, char* argv[]){

  //  process the specified command-line arguments of taking port number
  if (argc != 4)
  {
    fprintf(stderr, "Sender: Client-Usage: %s -p <port number> -d \n", argv[0]);
    exit(1);
  }

  port=atoi(argv[2]); // setting port

  bzero(&server, sizeof(server));
  server.sin_family = AF_INET;
  server.sin_port = htons(port);

  // processing configuration file -- check with config file if it is html
  if((fp = fopen("myhttpd.conf","r")) != NULL){
     while(fgets(config,MAXLINE,fp)!=NULL){
       printf("%s", config);
     }
     printf("\n");
        /* fgets returned null */
     if(errno != 0){
        perror("Error: Nothing Here");
     }
   }

   // check the extension file and version of http with config file
   else {                    /* there was an error on open */
        perror("Error: Could not open config file.");
        exit(1);
    }

  if (!inet_aton(ipaddress, &server.sin_addr))
    fprintf (stderr, "inet_addr() Conversion Error\n");
  else
     printf("Attempting to create socket. \n");


  s = socket(AF_INET, SOCK_STREAM, 0);

  // Creating socket
  if (!s){
      perror("Error: Could not create socket");
      exit(0);
  }
  else {
      printf("Socket Created. \n");
  }

  // Binding
  if (bind(s, (struct sockaddr *) &server, sizeof(server)) < 0){
    perror("Error: Bind Failed");
    exit(0);
  }
  else {
      printf("Bind Succeeded.\n");
      printf("Socket and address bound.\n");
      printf("Setting socket to listen...\n");
  }

  // Listening
  if (listen(s, SOMAXCONN) < 0){
     perror("Error: Couldn't listen.");
     exit(0);
   }
  else {
      printf("Listening...\n");
      printf("Waiting for connection.\n");
  }

 char *hello = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello world!";
 long valread;

 while ((c = accept(s, (struct sockaddr *)&client, (socklen_t *)&clientlen)) > 0){
    printf ("Connection Established\n");

    printf("Connection established with port : %d\n",port);

    char buffer[30000] = {0};
    valread = read(c, buffer, 30000);
    printf("%s\n",buffer );
    write(c, hello , strlen(hello));
    printf("------------------Hello message sent-------------------\n");
    close(c);

  }

  return (0);
}
