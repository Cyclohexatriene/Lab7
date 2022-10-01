#include <iostream>
#include <string>
#include <winsock2.h>
#include <WS2tcpip.h> //definition of function inet_pton() 
#pragma comment (lib,"ws2_32.lib")
using namespace std;

int main()
{
    //Initialize DLL
    WSADATA wsaData;
    if (!(WSAStartup(MAKEWORD(2, 2), &wsaData) == 0)) {
        cerr << "WSAStartup failed. The program will exit with code -1." << endl;
        return -1;
    }
    //Create socket
    SOCKET servSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    
    //Bind IP address and port number to the socket
    sockaddr_in sockAddr;
    memset(&sockAddr, 0, sizeof(sockAddr));
    sockAddr.sin_family = PF_INET;
    string serverIP = "127.0.0.1";
    u_short portNumber = 4203;
    inet_pton(AF_INET, serverIP.c_str(), (void*)&sockAddr.sin_addr); //Server IP address
    sockAddr.sin_port = htons(portNumber);
    bind(servSock, (SOCKADDR*)&sockAddr, sizeof(SOCKADDR));
    
    //Start listening from network.
    listen(servSock, 20);
    SOCKADDR clntAddr;
    int nSize = sizeof(SOCKADDR);
    SOCKET clntSock = accept(servSock, (SOCKADDR*)&clntAddr, &nSize);

    //Send messages to client.
    string msg = "Hello World!";
    send(clntSock, msg.c_str(), msg.length() + sizeof(char), 0);
    
    //Close socket
    closesocket(clntSock);
    closesocket(servSock);

    //Cleanup DLL
    WSACleanup();
}

