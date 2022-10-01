#include <iostream>
#include <string>
#include <winsock2.h>
#include <WS2tcpip.h> //definition of function inet_pton()
#pragma comment (lib,"ws2_32.lib")
using namespace std;

inline void showMenu1() {
	cout
		<< "Please select a fuction: " << endl
		<< "Input 1 : Connect to server" << endl
		<< "Input 2 : Quit" << endl;
}

inline void showMenu2() {
	cout<<endl
		<< "Please select a function: " << endl
		<< "Input 1 : Get server time" << endl
		<< "Input 2 : Get server name" << endl
		<< "Input 3 : Get list of active clients" << endl
		<< "Input 4 : Send message to another client" << endl
		<< "Input 5 : Disconnect and exit" << endl;
}

int main()
{
	//Initialize DLL
	WSADATA wsaData;
	if (!(WSAStartup(MAKEWORD(2, 2), &wsaData) == 0)) {
		cerr << "WSAStartup failed. The program will exit with code -1." << endl;
		return -1;
	}

	int state;
	while (1) {
		showMenu1();
		cin >> state;
		if (state == 1) break;
		else if (state == 2) return 0;
		else cout << "Invalid input. Please input an number again." << endl;
	}

	//Create socket
	SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	
	//Request server
	sockaddr_in sockAddr;
	memset(&sockAddr, 0, sizeof(sockAddr));
	sockAddr.sin_family = AF_INET;
	string serverIP;
	u_short portNumber;
	cout << "Please input server IP." << endl;
	cin >> serverIP;
	cout << "Please input port number." << endl;
	cin >> portNumber;
	inet_pton(AF_INET, serverIP.c_str(), (void*)&sockAddr.sin_addr);
	sockAddr.sin_port = htons(portNumber);
	if (!( connect(sock, (SOCKADDR*)&sockAddr, sizeof(SOCKADDR)) == 0 )) {
		cerr << "Failed to connect to server. The process will exit with code -1.";
		return -1;
	};
	cout << "Successfully connected to the server." << endl;
	cout << endl;

	while (1) {
		showMenu2();
		cin >> state;
		if (state == 1) {
			//Get server time
			string msg = "1";
			send(sock, msg.c_str(), msg.length() + sizeof(char), 0);
			char szBuffer[MAXBYTE] = { 0 };
			recv(sock, szBuffer, MAXBYTE, 0);
			cout << "The server time is: " << szBuffer << endl;
		}
		else if (state == 2) {
			//Get server name
			string msg = "2";
			send(sock, msg.c_str(), msg.length() + sizeof(char), 0);
			char szBuffer[MAXBYTE] = { 0 };
			recv(sock, szBuffer, MAXBYTE, 0);
			
			cout << "The server name is: ";
			wcout << (WCHAR*)szBuffer << endl;
		}
		else if (state == 3) {
			//Get list of active clients
			string msg = "3";
			send(sock, msg.c_str(), msg.length() + sizeof(char), 0);
			char szBuffer[MAXBYTE] = { 0 };
			recv(sock, szBuffer, MAXBYTE, 0);
			cout << "The list of active clients is: "<< endl << szBuffer << endl;
		}
		else if (state == 4) {
			//Send message to another client
			string msg = "4";
			cout << "Please input number of target client: " << endl;
			string target;
			cin >> target;
			msg += target+" ";
			cout << "Please input your message and end with an empty line." << endl;
			string temp;
			getline(cin, temp); //Get the rest '\n'
			while (1) {
				getline(cin, temp);
				if (temp.empty()) break;
				else msg += temp + '\n';
			}
			send(sock, msg.c_str(), msg.length() + sizeof(char), 0);
			char szBuffer[MAXBYTE] = { 0 };
			recv(sock, szBuffer, MAXBYTE, 0);
			cout << szBuffer << endl;
		}
		else if (state == 5) {
			//Disconnect and exit
			string msg = "5";
			send(sock, msg.c_str(), msg.length() + sizeof(char), 0);
			break;
		}
		else cout << "Invalid input. Please input an number again." << endl;
	}

	//Close socket
	closesocket(sock);

	WSACleanup();
	
}
