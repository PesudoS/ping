#Realization of ping using raw socket ———— Page 584, UNIX network programming

##Summary of msghdr and cmsghdr
[引用](https://my.oschina.net/NGINX08/blog/114140)
###1. descriptor pass
- After fork() return, child processes share all file descriptors opened of the parent processes.
- After the call of exec(), all file descriptors usually keep unchanged.
- *Problem*: 
1. child process pass the descriptor to its parent process?
2. pass the descriptor between two uncorrelated processes?
- **create an UNIX domain socket between two processes, and use sendmsg() to send a specific message across a socket ** 
###2. regular descriptor pass
####struct msghdr:
```
/*
 * s: fd of socket
 * buf: icmp packet to be sent 
 * len: length of buf
 * flags: BOR of some flags
 * to: the destination address 
 * tolen: length of to
 */
ssize_t sendto(int s, const void *buf, size_t len, int flags, const struct sockaddr *to, socklen_t tolen);
```
- flags:
1. MSG_DONTROUTE
Only hosts connectting to the current network can receive data. NO use gateway when sending packets. This flag is usually used to diagnosing and routing programs. Protocol family supporting route can use this flag and packet sockets cannot use it.
2. MSG_DONTWAIT
Nonblock
3. MSG_NOSIGNAL 
- When the other end of SOCK_STREAM interrupts connection, no send SIGPIPE signal, but still return EPIPE error.
- When a server close a connection, if client continues send data it will receive a RST response. The client continues to send data to the server, system will send a SIGPIPE signal to the process, tell the process the connection has interrupt, don't write any more.
- SIPIPE signal kills the process acquiescently.
[引用](https://www.cnblogs.com/lit10050528/p/5116566.html)
4. MSG_CONFIRM
Inform the link layer of the forwarding process: get a success reply from the other end. If the link layer doesn't receive the message, it will detect the neighbor hosts as a matter of routine(such as free arp). It can only be used for SOCK_DGRAM and SOCK_RAW and valid for IPv4  and IPv6.

sendto() finally call kernel function:
```
asmlinkage long sys_sendto(int fd, void __user * buff, size_t len, unsigned flags, struct sockaddr __user *addr, int addr_len)
``` 
The function sys_sendto() builds a struct msghdr to receive packets from the application layer.

The content of the struct can be divided into four groups.
1. msg_name/msg_namelen
- record the name of the message, that is the des address.
- msg_name: a pointer pointing to struct sockaddr, the length is 20.
```
struct sockaddr
{
    uint8_t sa_len;
    sa_family_t sa_family;
    char sa_addr[14];
}
```
The length of msg_namelen is 20. It's noteworthy that struct sockaddr is used when param pass. Either in user mode or kernel mode, we forced it to be struct sockaddr_in.
```
struct sockaddr_in
{
    uint8_t sin_len;
    sa_family_t sin_family;    // 1 byte
    unsigned short int sin_port;   
    struct in_addr sin_addr;    // 4 bytes
    char sin_zero[8];    // 8 bytes
}
struct in_addr
{
    in_addr_t s_addr;
}
``` 
