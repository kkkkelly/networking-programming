#include <iostream>
#include <process.h>
#include "TCPIP_Sync.cpp"
#define MAX 1000

int main()
{
	//1.setup Server
	SOCKET Sock, Sock1[MAX];
	sockaddr Addr;
	fd_set fdSock, fdRead;
	Start_TCP_Server(&Sock, 6000);
	FD_ZERO(&fdSock);
	FD_SET(Sock, &fdSock);//add sock to fdsock set
	printf("TCP Server(6000) 啟動完畢.....\r\n");

	//2.檢查是否有"連線/收資料"要求(多人)
	int i, j, k,Len = sizeof(sockaddr),No= 0;
	char S1[2000], S2[2000];
	while (1) {
		fdRead = fdSock;//保持原有資料
		i = select(0, &fdRead, 0, 0, 0);//有人連線才執行
		if (i <= 0) continue;
		for (j = 0; j < fdSock.fd_count; j++) {
			if (fdSock.fd_array[j] == fdRead.fd_array[0]) {
				if (fdSock.fd_array[j] == Sock) {//Server accept
					Sock1[No] = accept(Sock, &Addr, &Len);
					printf("遠端連入...\r\n");
					FD_SET(Sock1[No], &fdSock);
					No++;
				}
				else {//client送來資料
					k = recv(fdSock.fd_array[j],S1,sizeof(S1)-1,0);
					if (k > 0) {
						S1[k] = 0;
						printf("%s\r\n",S1);
						sprintf_s(S2, "已讀(%s)", S1);//製造已讀封包
						send(fdSock.fd_array[j], S2, strlen(S2), 0);
					}
					else {
						printf("遠端斷線\r\n");
						closesocket(fdSock.fd_array[j]);
						No--;
						FD_CLR(Sock1[No], &fdSock);
						//break;
						
					}
				}
			}
		}

	}




}

