#include "ping.h"

void readloop()
{
	int size;
	char recvbuf[BUFSIZE];
	char controlbuf[BUFSIZE];

	struct msghdr msg;
	struct iovec iov;

	ssize_t n;
	struct timeval tval;

	/* create the raw socket of ICMP, root */
	if((sockfd = socket(pr->sasend->sa_family, SOCK_RAW, pr->icmpproto)) < 0)
	{
		perror("socket error");
		exit(1);
	}

	/* recycle the root permission and set the current user permission */
	setuid(getuid());

	/* initialize IPv6 */
	if(pr->finit)
		(*pr->finit)();

	/* set the buffer size 60k to decrease buffer overflow */
	size = 60 * 1024;
	setsockopt(sockfd, SOL_SOCKET, SO_RCVBUF, &size, sizeof(size));
	
	/* send the first packet */
	sig_alrm(SIGALRM);

	/* initialize recvbuf */
	iov.iov_base = recvbuf;
	iov.iov_len = sizeof(recvbuf);
	msg.msg_name = pr->sarecv;
	msg.msg_iov = &iov;
	msg.msg_iovlen = 1;
	msg.msg_control = controlbuf;

	while(1)
	{
		/* receive the ICMP packet */
		msg.msg_namelen = pr->salen;
		msg.msg_controllen = sizeof(controlbuf);
		/* receive data from socket */
		n = recvmsg(sockfd, &msg, 0);
		if(n < 0)
		{
			if(errno == EINTR)
				continue;
			else
			{
				perror("recvmsg error");
				exit(1);
			}
		}
		/* record receive time */
		gettimeofday(&tval, NULL);
		/* call process function */
		(*pr->fproc)(recvbuf, n, &msg, &tval);
	} 
}
