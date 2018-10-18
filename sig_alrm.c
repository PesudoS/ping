#include "ping.h"

/* send packet, set alarm clock and send the SIGALRM signal to process after one second */
void sig_alrm(int signo)
{
	(*pr->fsend)();
	alarm(1);
	return;
}
