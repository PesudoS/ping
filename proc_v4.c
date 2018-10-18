#include "ping.h"

extern char *Sock_ntop_host(const struct sockaddr *sa, socklen_t salen);
void proc_v4(char *ptr, ssize_t len, struct msghdr *msg, struct timeval *tvrecv)
{
	int hlen1, icmplen;
	double rtt;
	struct ip *ip;
	struct icmp *icmp;
	struct timeval *tvsend;
	
	ip = (struct ip *)ptr;	/* start of IP header */
	hlen1 = ip->ip_hl << 2;	/* Length of IP header */
	if(ip->ip_p != IPPROTO_ICMP)
		return;		/* not ICMP */

	/* cross the IP header, point to the ICMP header */
	icmp = (struct icmp *)(ptr + hlen1);	/* start of ICMP header */
	/* total length of ICMP header and ICMP datagram, if less than 8, unreasonable */
	if((icmplen = len - hlen1) < 8)
		return;		/* malformed packet */
	
	/* make sure all packets received are ICMP echo reply */
	if(icmp->icmp_type == ICMP_ECHOREPLY)
	{
		if(icmp->icmp_id != pid)
			return;		/* not a response to our ECHO_REQUEST */
		if(icmplen < 16)
			return;		/* not enough data to use */
		tvsend = (struct timeval *)icmp->icmp_data;
		tv_sub(tvrecv, tvsend);		/* compute the time difference between receiving and sending */
		rtt = tvrecv->tv_sec * 1000.0 + tvrecv->tv_usec / 1000.0;	/* compute rtt measured in ms */
		printf("%d bytes from %s: icmp_seq=%u ttl=%d rtt=%.3f ms\n", 
						icmplen, Sock_ntop_host(pr->sarecv, pr->salen),
						icmp->icmp_seq, ip->ip_ttl, rtt);
		nrecv++;
	} 
	else if(verbose)
	{
		printf("  %d bytes from %s: icmp_type = %d, icmp_code = %d\n",
						icmplen, Sock_ntop_host(pr->sarecv, pr->salen),
						icmp->icmp_type, icmp->icmp_code);
	}
}
