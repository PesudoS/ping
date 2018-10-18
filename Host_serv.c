#include "unp.h"

struct addrinfo *
host_serv(const char *host, const char *serv, int family, int socktype)
{
	int n;
	struct addrinfo hints, *res;
	bzero(&hints, sizeof(struct addrinfo));	/* reset hints */
	hints.ai_flags = AI_CANONNAME; /* need a standard name */
	hints.ai_family = family;	/* AF_INET, AF_INET6, AF_UNSPEC*/
	hints.ai_socktype = socktype;	/* SOCK_DGRAM, SOCK_STREAM */
	if((n=getaddrinfo(host, serv, &hints, &res)) != 0)
		return NULL;
	return res;	/* return pointer to first on linked list */
}

struct addrinfo *
Host_serv(const char *host, const char *serv, int family, int socktype)
{
    int n;
    struct addrinfo hints, *res;
    bzero(&hints, sizeof(struct addrinfo)); /* reset hints */
    hints.ai_flags = AI_CANONNAME; /* need a standard name */
    hints.ai_family = family;   /* AF_INET, AF_INET6, AF_UNSPEC*/
    hints.ai_socktype = socktype;   /* SOCK_DGRAM, SOCK_STREAM */
    if((n=getaddrinfo(host, serv, &hints, &res)) != 0)
    	return NULL;
	return res; /* return pointer to first on linked list */
}

