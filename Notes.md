# WORD & MAKEWORD()

WORD就是所谓的一个“字”，一个字是16位，也就是两个字节。

MAKEWORD接受两个BYTE型的参数`a`和`b`，并把这两个BYTE合成一个WORD，其中`a`在低8位，`b`在高8位。

# WSA & WSAStartup() & WSADATA

WSA即`Windows Sockets Asynchronous，Windows异步套接字`。

WSAStartup是Windows下的网络编程接口软件Winsock1 或 Winsock2 里面的一个命令，用于完成对Winsock服务的初始化。WSAStartup接受两个参数，其中第一个WORD型参数指明程序请求使用的Socket版本，低8位指明主版本，高8位指明副版本，所以`MAKEWORD(a,b)`就代表调用`a.b`版本的Socket；第二个WSADATA型参数用于记录WSAStartup执行后返回的一些信息，其中包括DLL期望调用者使用的Socket版本、DLL能支持的Socket最高版本等信息。

与WSAStartup相对应的还有一个叫`WSACleanup()`的函数，用于终止winsock 2 DLL的调用。

# socket()

`socket()`函数用于创建套接字。其原型为`SOCKET socket(int af, int type, int protocol)`，其中各参数的意义如下：

1. `int af`：af的全称是`address family（地址族）`，也就是IP地址的类型。`AF_INET`表示IPv4地址，`AF_INET6`表示IPv6地址。
2. `int type`：type为数据传输方式/套接字类型，`SOCK_STREAM`表示流格式套接字/面向连接的套接字，常与TCP协议搭配使用，`SOCK_DGRAM`表示数据报套接字/无连接的套接字，常与UDP协议搭配使用。
3. `int protocol`：protocol为传输协议，`IPPROTO_TCP`表示TCP协议，`IPPROTO_UDP`表示UDP协议。

由以上三个参数可知，socket()函数并不包含具体的IP地址或端口，只是指定了IP地址类型、传输方式和传输类型，所以socket()函数仅仅只是创建了一个应用层和运输层之间的接口，后续还要通过`bind()`函数来指定具体的IP地址和接口等信息。

# bind() & sockaddr_in

`bind()`函数用于将具体的地址信息绑定到上面创建的套接字上。其原型为`int bind(SOCKET s, const sockaddr *name, int namelen)`，其中各参数的意义如下：

1. `SOCKET s`：待绑定的套接字。
2. `const sockaddr *name`：指向包含地址信息的结构体（`sockaddr`)的指针。
3. `int namelen`：结构体的大小。

bind传入的参数是指向`sockaddr`的指针，但是在实操中，我们往往会先用另一个名为`sockaddr_in`的结构体来写入参数，再将这个结构体的地址强制转换为`sockaddr*`传入到`bind()`函数中。

`sockaddr_in`的定义为：

```cpp
typedef struct sockaddr_in {
    
#if(_WIN32_WINNT < 0x0600)
    short   sin_family;
#else //(_WIN32_WINNT < 0x0600)
    ADDRESS_FAMILY sin_family;
#endif //(_WIN32_WINNT < 0x0600)
    
    USHORT sin_port;
    IN_ADDR sin_addr;
    CHAR sin_zero[8];
}
```

`sockaddr`的定义为：

```cpp
typedef struct sockaddr {

#if (_WIN32_WINNT < 0x0600)
    u_short sa_family;
#else
    ADDRESS_FAMILY sa_family;           // Address family.
#endif //(_WIN32_WINNT < 0x0600)

    CHAR sa_data[14];                   // Up to 14 bytes of direct address.
}
```

观察一下两者的区别，我们发现`sockaddr`结构体的`CHAR sa_data[14]`的14字节恰好对应了`sockaddr_in`结构体的`USHORT sin_port`、`IN_ADDR sin_addr`、`CHAR sin_zero[8]`的14字节。所以代码的设计者是为了同时满足人类理解和机器读取、传输数据的需求才写了两个相似但不同的结构体的。

另附上`IN_ADDR`的定义：

```cpp
typedef struct in_addr {
        union {
                struct { UCHAR s_b1,s_b2,s_b3,s_b4; } S_un_b;
                struct { USHORT s_w1,s_w2; } S_un_w;
                ULONG S_addr;
        } S_un;
#define s_addr  S_un.S_addr /* can be used for most tcp & ip code */
#define s_host  S_un.S_un_b.s_b2    // host on imp
#define s_net   S_un.S_un_b.s_b1    // network
#define s_imp   S_un.S_un_w.s_w2    // imp
#define s_impno S_un.S_un_b.s_b4    // imp #
#define s_lh    S_un.S_un_b.s_b3    // logical host
} IN_ADDR;
```

`IN_ADDR`似乎是把地址的4字节分成了`1+1+1+1`、`2+2`、`4`三种写法，只需要取其中一种写法就可以了。

# listen() & accept()

`listen()`函数的原型是`int listen(SOCKET sock, int backlog)`，用于使一个套接字进入**监听状态**。如果函数正常返回，则返回值为0，否则会返回`SOCKET_ERROR`。其接受两个参数：第一个为待操作的套接字，第二个为请求队列的最大长度。

在`listen()`函数正常执行后，程序会一直运行**直到**`accept()`函数被调用，`accept()`的原型为`SOCKET accept(SOCKET sock, struct sockaddr *addr, int *addrlen)`。它的返回值是一个新的套接字，用于和连入的客户端进行通信。它接受三个参数：第一个是服务端上面创建的处于监听状态的套接字，第二个参数为指向sockaddr型结构体的指针，第三个参数为第二个参数的（字节）长度，同样以指针的形式传入。



