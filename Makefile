Ping: main.o Calloc.o init_v6.o readloop.o sig_alrm.o Host_serv.o proc_v4.o proc_v6.o send_v4.o send_v6.o sock_ntop_host.o in_cksum.o MySignal.o tv_sub.o
		gcc -o Ping main.o Calloc.o init_v6.o readloop.o sig_alrm.o Host_serv.o proc_v4.o proc_v6.o send_v4.o send_v6.o sock_ntop_host.o in_cksum.o MySignal.o tv_sub.o

main.o:main.c
Calloc.o:Calloc.c
init_v6.o:init_v6.c
readloop.o:readloop.c
sig_alrm.o:sig_alrm.c
Host_serv.o:Host_serv.c
proc_v4.o:proc_v4.c
proc_v6.o:proc_v6.c
send_v4.o:send_v4.c
send_v6.o:send_v6.c
sock_ntop_host.o:sock_ntop_host.c
in_cksum.o:in_cksum.c
MySignal.o:MySignal.c
tv_sub.o:tv_sub.c

clean:
		rm -f *.o




