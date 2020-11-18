

#include <iostream>
#include <process.h>
#include "TCPIP_Sync.cpp"

SOCKET Sock;
void Fun(PVOID p) {
	int i;
	char S1[2000];
	while (1) {
		i = recv(Sock, S1, sizeof(S1) - 1, 0);
		if (i > 0) {
			S1[i] = 0;
			printf("%s\r\n", S1);
		}
	}
}

int main()
{
	//setup TCPclient
	Start_TCP_Client(&Sock, 6000, (char*)"127.0.0.1");//port 80(http) port 21(ftp)
	//setup thread 
	_beginthread(Fun, 0, 0);
	
	//read&send
	char S1[2000];
	while (1) {
		scanf_s("%s",S1,sizeof(S1));
		//strcpy_s(S1, "USER abc\r\n");
		send(Sock, S1, strlen(S1), 0);
	}
	
}

