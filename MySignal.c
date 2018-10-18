#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
typedef void Sigfunc(int);

static Sigfunc *M_signal(int signo, Sigfunc *func);
Sigfunc *MySignal(int signo, Sigfunc *func)
{
	Sigfunc *sigfunc;
	if((sigfunc = M_signal(signo, func)) == SIG_ERR)
	{
		perror("signal error");
		exit(1);
	}
	return sigfunc;
}

static Sigfunc *M_signal(int signo, Sigfunc *func)
{
	struct sigaction act, oact;
	/* set the signal processing function*/
	act.sa_handler = func;
	/* initialize the signal set*/
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;
	if(signo == SIGALRM)
	{
		/* SIGALRM signal, system won't restart automatically */
#ifdef SA_INTERRUPT
	act.sa_flags |= SA_INTERRUPT;
#endif		
	}
	else
	{
		/* other signal——system can restart automatically */
#ifdef SA_RESTART
	act.sa_flags |= SA_RESTART;
#endif		
	}
	/* call sigaction() */
	if(sigaction(signo, &act, &oact) < 0)
		return SIG_ERR;
	return oact.sa_handler;
}
