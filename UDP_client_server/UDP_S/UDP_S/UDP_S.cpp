// UDP_S.cpp : 此檔案包含 'main' 函式。程式會於該處開始執行及結束執行。
//

#include <iostream>
#include <WinSock2.h>
#pragma comment(lib,"ws2_32.lib")

//啟動UDP server
int main()
{
	//網路變數宣告   
	WSADATA Wsa;
	SOCKET Sock;
	sockaddr_in Addr, Addr1;
	char S1[2000];
	int i,Len = sizeof(sockaddr);

	//啟動socket
	WSAStartup(0x202, &Wsa);
	Sock = socket(AF_INET, SOCK_DGRAM, 0);

	//設定client端連線進入條件
	Addr.sin_family = AF_INET;//TCP/IP protocal
	Addr.sin_port = htons(6000);//setup port
	Addr.sin_addr.s_addr = htonl(INADDR_ANY);
	bind(Sock, (sockaddr*)&Addr, Len);//綁定port

	//讀取網路資料列印
	while (1) {
		i = recvfrom(Sock, S1, sizeof(S1) - 1,0, (sockaddr*)&Addr1,&Len);
		if (i > 0) {
			S1[i] = 0;
			printf("Recv %s ", S1);
			printf("from :%d.%d.%d.%d \n", (Addr1.sin_addr.s_addr)&0xFF, (Addr1.sin_addr.s_addr >> 8)&0xFF, (Addr1.sin_addr.s_addr >>16)&0xFF, Addr1.sin_addr.s_addr>>24);
		}
		

	}
}

