#ifndef PING_H
#define PING_H

#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>

#include <sys/socket.h>
#include <signal.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/time.h>
#include <sys/types.h>

#define BUFSIZE 4096

char sendbuf[BUFSIZE];

extern int datalen;	/* bytes of data following ICMP header */
char *host;
int nsent;	/* add 1 for each sendto() */
int nrecv;	/* add 1 for each recvmsg() */
pid_t pid;	/* our PID */
int sockfd;
int verbose;

/* function prototypes */
void init_v6(void);
void proc_v4(char *, ssize_t, struct msghdr *, struct timeval *);
void proc_v6(char *, ssize_t, struct msghdr *, struct timeval *);
void send_v4(void);
void send_v6(void);
void readloop(void);
void sig_alrm(int);
void tv_sub(struct timeval *, struct timeval *);

/* handle the difference between IPv4 and IPv6 */
struct proto 
{
	/* 3 function pointers */
	void (*fproc)(char *, ssize_t, struct msghdr *, struct timeval *);
	void (*fsend)(void);
	void (*finit)(void);

	/* 2 sockaddr pointers */
	struct sockaddr *sasend;	/* sockaddr() for send, from getaddrinfo() */
	struct sockaddr *sarecv;	/* sockaddr() for receiving */
	
	socklen_t salen;	/* length of sockaddr{}s */
	int icmpproto;	/* ICMP prototype value */	
} *pr;

#ifdef IPV6

#include <netinet/ip6.h>
#include <netinet/icmp6.h>

#endif
#endif








