#include "ping.h"

struct proto proto_v4 =
	{ proc_v4, send_v4, NULL, NULL, NULL, 0, IPPROTO_ICMP };
#ifdef IPV6
struct proto proto_v6 =
	{ proc_v6, send_v6, init_v6, NULL, NULL, 0, IPPROTO_ICMPV6 };
#endif

typedef void Sigfunc(int);
extern int datalen = 56;	/* data goes with ICMP echo request */

extern Sigfunc *MySignal(int signo, Sigfunc *func);
extern struct addrinfo *host_serv(const char *host, const char *serv, int family, int socktype);
extern char *Sock_ntop_host(const struct sockaddr *sa, socklen_t salen);
extern void *Calloc(size_t n, size_t size);
void statistics(int signo);

int main(int argc, char **argv)
{
	int n;
	struct addrinfo *ai;
	char *h;

	opterr=0;	/* don't want to getopt() writing to stderr */
	
	/* only realize an option '-v' for querying */
	while((n = getopt(argc, argv, "v")) != -1)
	{
		switch(n)
		{
			case 'v':
				verbose++;
				break;
			case '?':
				printf("unrecgonized option: %c\n", n);
				exit(1);
		}
	}

	if(optind != argc - 1)
	{
		/* according to global variable errno decide the output string */
		perror("usage: ping [-v] <hostname>");
		exit(1);
	}

	host = argv[optind];

	pid = getpid() & 0xffff;	/* ICMP ID field is 16 bits. */

	MySignal(SIGALRM, sig_alrm);
	/* Ctrl+C inturrupt, make statistics */
	MySignal(SIGINT, statistics);

	/* map host name and service name to an address, and return the pointer to addrinfo */
	ai = host_serv(host, NULL, 0, 0);
	/* map the network byte order address to string format address, and return */
	h = Sock_ntop_host(ai->ai_addr, ai->ai_addrlen);

	/* show the host name, address and data bytes */
	printf("PING %s (%s): %d data bytes\n",
			ai->ai_canonname ? ai->ai_canonname : h, h, datalen);
	
	/* Initialize according to protocol */
	if(ai->ai_family == AF_INET)
	{
		pr = &proto_v4;
#ifdef IPv6		
	}
	else if(ai->ai_family == AF_INET)
	{
		pr = &proto_v6;
		if(IN6_IS_ADDR_V4MAPPED(&(((struct sockaddr_in6 *)
								ai->ai_addr)->sin6_addr)))
		{
			perror("cannot ping IPv4-mapped IPv6 address");
			exit(1);
		}
#endif
	}
	else
	{
		printf("unknown address family %d", ai->ai_family);
		exit(1);	
	}
	pr->sasend = ai->ai_addr;	/* assignment of send address */
	pr->sarecv = Calloc(1, ai->ai_addrlen);
	pr->salen = ai->ai_addrlen;		/* size of address */
	readloop();		/* handle data */
	exit(0);
}

/* show the number of sendmsg and recvmsg, and compute the packet loss rate */
void statistics(int signo)
{
	printf("\n----------- %s ping statistics -----------\n", Sock_ntop_host(pr->sarecv, pr->salen));
	int loss = 100 * (nsent - nrecv) / nsent;
	printf("%d packets transmitted, %d received, %d%% packet loss\n", nsent, nrecv, loss);
	close(sockfd);
	exit(1);
}
