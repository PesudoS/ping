# Realization of ping using raw socket ———— Page 584, UNIX network programming
[引用](https://blog.csdn.net/chenhanzhun/article/details/42080777?utm_source=blogxgwz0)

## Summary of msghdr and cmsghdr
[引用](https://my.oschina.net/NGINX08/blog/114140)
### 1. descriptor pass
- After fork() return, child processes share all file descriptors opened of the parent processes.
- After the call of exec(), all file descriptors usually keep unchanged.
- *Problem*: 
1. child process pass the descriptor to its parent process?
2. pass the descriptor between two uncorrelated processes?
- **create an UNIX domain socket between two processes, and use sendmsg() to send a specific message across a socket** 
### 2. regular descriptor pass
#### struct msghdr:
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
- **flags**:
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
```
/* 32-bit IPv4 address */
/* in_addr_t:
 *     32-bit unsigned int, in network byte order(big-endian)
 *     print —— inet_ntoa(), convert it to char*
 *     #include <arpa/inet.h>		
 */
struct in_addr
{
	in_addr_t s_addr;
}

/* convert decimal in network byte order to string in dotted decimal format  */
char *inet_ntoa(struct in_addr in);
```
In our ping example, introduce to struct msghdr of kernel:
```
msg.msg_name = {sa_family_t = MY_AF_INET, sin_port = 0, sin_addr.s_addr = 172.16.48.1}
msg.msg_namelen = 16
```
The echo request ICMP packet doesn't have port of the des address.  
2. msg_iov/msg_iovlen
record content of msg, msg_iov is a pointer that points to struct iovec. Actually, msg_iov should be an array of struct iovec. struct iovec is defined as follows:
```
/*
 * iov_base: point to buff
 * iov_len: buff length
 * msghdr: allow passing buffs once, saved in msg_iov as an array
 * msg_iovlen: record array size
 */
struct iovec
{
    void __user *iov_base;
    _kernel_size_t iov_len;
};
```
In our ping program:
```
msg.msg_iov = {struct iovec = {iov_base = {icmp头 + 填充字符'E'}, iov_len = 40}}
msg.msg_len = 1
```
3. msg_control/msg_controllen
They can be used to send any control information.
4. msg_flags

#### struct cmsghdr
Here we focus on the file descriptor and certificate structure. Before every object there exists a struct cmsghdr. After header, padding bytes, then the object itself. Finally, after the accessory object, maybe more padding bytes before next cmsghdr.

```
/*
 * cmsg_len: byte count of attached data, including header. CMSG_LEN()
 * cmsg_level: raw protocol level(SOL_SOCKET)
 * cmsg_type: control info type(SCM_RIGHTS)
 */
struct cmsghdr
{
    socklen_t cmsg_len;
    int		  cmsg_level; 
    int 	  cmsg_type;
};
```
##### some related macros
**CMSG_LEN()**
- accept object size as input param. It can be used to set cmsg_len. 
- ending padding bytes exclusive  
**CMSG_SPACE()** 
- compute total space that accessory data and its header need.
- determine buff size
```
int fd;
char buf[CMSG_SPACE(sizeof(fd))];
```
**CMSG_DATA()**
- accept a struct cmsghdr pointer
- return the first byte of accessory data
```
struct cmsghdr *mptr;
int fd;
fd = *(int *)CMSG_DATA(mptr);
```
**CMSG_FIRSTHDR()**
- return a struct cmsghdr pointer that points to the first accessory object in the buff
- ascertain if there exists accessory object according to msg_control/msg_controllen. If not, return NULL, otherwise point to the first struct cmsghdr
- be used at the start point of for loop to traverse accessory objects.  
**CMSG_NXTHDR()**
- return pointer of the next accessory object
- 2 param:  
		pointer that points to struct msghdr  
		pointer that points to current struct cmsghdr  





