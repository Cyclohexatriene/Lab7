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
	SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	//Request server
	sockaddr_in sockAddr;
	memset(&sockAddr, 0, sizeof(sockAddr));
	sockAddr.sin_family = AF_INET;
	string serverIP = "127.0.0.1";
	u_short portNumber = 4203;
	inet_pton(AF_INET, serverIP.c_str(), (void*)&sockAddr.sin_addr);
	sockAddr.sin_port = htons(portNumber);
	if (!( connect(sock, (SOCKADDR*)&sockAddr, sizeof(SOCKADDR)) == 0 )) {
		cerr << "Failed to connect to server. The process will exit with code -1.";
		return -1;
	};

	//Get data from server
	char szBuffer[MAXBYTE] = { 0 };
	recv(sock, szBuffer, MAXBYTE, 0);

	//Output message
	cout << "Message from server: " << szBuffer << endl;

	//Close socket
	closesocket(sock);

	WSACleanup();
	
}
