

#include <iostream>
#include <process.h>
#include "TCPIP_Sync.cpp"
int main()
{
	//setup Server
	SOCKET Sock,Sock1;
	sockaddr Addr;
	char S1[2000],S2[2000];
	int i,Len = sizeof(sockaddr);
	Start_TCP_Server(&Sock, 6000);
	//等待他人連線接受他
	while (1) {
		printf("等待連線中.......\n");
		Sock1 = accept(Sock, &Addr, &Len);
		while (1) {
			i = recv(Sock1, S1, sizeof(S1) - 1, 0);
			if (i > 0) {
				S1[i] = 0;
				printf("%s\r\n", S1);
				sprintf_s(S2, "已讀(%s)", S1);//製造已讀封包
				send(Sock1, S2, strlen(S2), 0);
			}
			else {
				printf("遠端斷線\r\n");
				closesocket(Sock1);
				break;
			}//斷線
			
		}
	}

}


