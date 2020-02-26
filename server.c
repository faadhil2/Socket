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
#include <time.h>
#define MAXLINE 100

int c,s,port;
struct sockaddr_in server, client;
struct tm *timeStruct;
char *ipaddress = "141.117.57.46"; // what IP address are we supposed to used?
int clientlen = sizeof(client);
int portset=0;
char *message;
FILE * fp;
char config[MAXLINE];
extern int errno;
char *date;
char *get_request="GET", *post_request="POST", *head_request="HEAD";

void ok() {
  write(c, "\r\n", 2);
  write(c, "HTTP/1.0 200 OK\r\n", 17);
  time_t currentTime;
  time(&currentTime);
  timeStruct = gmtime(&currentTime);
  date = asctime(timeStruct);

  write(c, "Date: ", 5);
  write(c,  date, strlen(date));
  write(c, "Server: AServer 0.1\r\n", 21);
  write(c, "Content-Length: ", 15);
  write(c, " 2 \r\n", 6);
  write(c, "Connection: close\r\n", 19);
  write(c, "Content-Type: text/plain; charset=iso-8859-1\r\n", 46);
  write(c, "\r\n", 2);
  write(c, "200 OK",  7);
  write(c, "\r\n\r\n", 4);
  close(c);
}

void created() {
  write(c, "\r\n", 2);
  write(c, "HTTP/1.0 201 Created\r\n", 22);
  time_t currentTime;
  time(&currentTime);
  timeStruct = gmtime(&currentTime);
  date = asctime(timeStruct);

  write(c, "Date: ", 5);
  write(c,  date, strlen(date));
  write(c, "Server: AServer 0.1\r\n", 21);
  write(c, "Connection: close\r\n", 19);
  write(c, "Content-Type: text/plain; charset=iso-8859-1\r\n", 46);
  write(c, "Content-Length: ", 15);
  write(c, "\r\n", 2);
  write(c, "201 Created", 11);
  write(c, "\r\n\r\n", 4);
  close(c);
}

void head() {
  time_t currentTime;
  time(&currentTime);
  timeStruct = gmtime(&currentTime);
  date = asctime(timeStruct);

  write(c, "\r\n", 2);
  write(c, "Date: ", 5);
  write(c,  date, strlen(date));
  write(c, "Server: AServer 0.1\r\n", 21);
  write(c, "Content-Length: ", 15);
  write(c, "Connection: close\r\n", 19);
  write(c, "Content-Type: text/plain; charset=iso-8859-1\r\n", 46);
  write(c, "\r\n", 2);
  close(c);
}

void badRequest(){
  write(c, "\r\n", 2);
  write(c, "HTTP/1.0 400 Bad Request\r\n", 27);
  write(c, "Server: AServer 0.1\r\n", 21);
  write(c, "Connection: close\r\n", 19);
  write(c, "Content-Type: text/plain; charset=iso-8859-1\r\n", 46);
  write(c, "\r\n", 2);
  write(c, "400 Bad Request", 15);
  write(c, "\r\n\r\n", 4);
  close(c);
}

void noRead(){
  write(c, "\r\n", 2);
  write(c, "HTTP/1.0 403 No Read Permissions\r\n", 34);
  write(c, "Server: AServer 0.1\r\n", 21);
  write(c, "Connection: close\r\n", 19);
  write(c, "Content-Type: text/plain; charset=iso-8859-1\r\n", 46);
  write(c, "\r\n", 2);
  write(c, "403 No Read Permissions", 23);
  write(c, "\r\n\r\n", 4);
  close(c);
}

void fileDNE(){
  write(c, "\r\n", 2);
  write(c, "HTTP/1.0 404 File Does Not Exist\r\n", 34);
  write(c, "Server: AServer 0.1\r\n", 21);
  write(c, "Connection: close\r\n", 19);
  write(c, "Content-Type: text/plain; charset=iso-8859-1\r\n", 46);
  write(c, "\r\n", 2);
  write(c, "404 File Does Not Exist", 23);
  write(c, "\r\n\r\n", 4);
  close(c);
}

void notImplemented(){
  write(c, "\r\n", 2);
  write(c, "HTTP/1.0 501 Not Implemented\r\n", 30);
  write(c, "Server: AServer 0.1\r\n", 21);
  write(c, "Connection: close\r\n", 19);
  write(c, "Content-Type: text/plain; charset=iso-8859-1\r\n", 46);
  write(c, "\r\n", 2);
  write(c, "501 Not Implemented", 19);
  write(c, "\r\n\r\n", 4);
  close(c);
}

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

 while ((c = accept(s, (struct sockaddr *)&client, (socklen_t *)&clientlen)) > 0){
    char buffer[30000] = {0};
    read(c, buffer, 2000);
    printf("\n%s\n",buffer);
    notImplemented();
    printf("Disconnected. \n");
    printf("Waiting for connection. \n");
  }
  return (0);
}
