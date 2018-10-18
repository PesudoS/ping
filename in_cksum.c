#include <stdint.h>

/* checksum algorithm */
uint16_t in_cksum(uint16_t *addr, int len)
{
	int nleft = len;
	uint32_t sum = 0;
	uint16_t *w = addr;
	uint16_t answer = 0;

	/*
 	 * using a 32 bit accumulator (sum), we add sequential 16 bit words to it, 
 	 * and at the end, fold back all the carry bits into the lower bits.
	 */
 
	/* ICMP header accumulate by 2 bytes */
	while(nleft > 1)
	{
		sum += *w++;
		nleft -= 2;
	}	

	/* mod up a byte, if necessary */
	if(nleft == 1)
	{
		/* If ICMP header has odd bytes, think the last byte as the high byte
 		 * of two bytes, then the low byte is 0, accumulate continually
		 */
		*(unsigned char *)(&answer) = *(unsigned char *)w;
		sum += answer;
	}

	/* add back carry outs from top 16 bits to low 16 bits */
	sum = (sum >> 16) + (sum & 0xffff);		/* add hi 16 to low 16*/
	sum += (sum >> 16);		/* add carry */
	answer = ~sum;		/* truncate to 16 bits */
	return answer;
}
