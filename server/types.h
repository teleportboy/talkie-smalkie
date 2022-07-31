#ifndef TYPES_INCLUDED_H
#define TYPES_INCLUDED_H

#include <sys/socket.h>
#include <sys/un.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>

typedef struct sockaddr_in      sockaddr_in;
typedef struct sockaddr         sockaddr;
typedef struct addrinfo         addrinfo;

typedef int    socket_descriptor;




#endif