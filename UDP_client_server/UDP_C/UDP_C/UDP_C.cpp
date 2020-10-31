// UDP_C.cpp : 此檔案包含 'main' 函式。程式會於該處開始執行及結束執行。
//

#include <iostream>
#include <process.h>
#include <WinSock2.h>
#pragma comment(lib,"ws2_32.lib")

SOCKET Sock;

VOID Fun(PVOID pvoid) {
	int i, Len = sizeof(sockaddr);
	char S1[2000];//store message
	sockaddr Addr;//store client之ip port
	while (1) {
		i = recvfrom(Sock, S1, sizeof(S1) - 1, 0, &Addr, &Len);//make UDP recieve data
		if (i > 0) {
			S1[i] = 0;//last letter of message
			printf("recv = %s\r\n", S1);
		}
		//printf("OK");
	}
}


int main()
{
//網路變數宣告   
WSADATA Wsa;
//SOCKET Sock;
sockaddr_in Addr;
char S1[2000];
int Len = sizeof(sockaddr);
	//啟動socket
	WSAStartup(0x202, &Wsa);
	Sock = socket(AF_INET, SOCK_DGRAM, 0);
	//設定server端ip&port
	Addr.sin_family = AF_INET;//TCP/IP protocal
	Addr.sin_port = htons(6000);//setup port
	Addr.sin_addr.s_addr = (1<< 24) + (0 << 16) + (0 <<8 )+127;//192.168.13....

	_beginthread(Fun, 0, 0);

	//讀鍵盤送出資料
	while (1) {
		printf("Input :");
		scanf_s("%s", S1, sizeof(S1));
		sendto(Sock, S1, strlen(S1), 0, (sockaddr*)&Addr,Len);

	}

}

// 執行程式: Ctrl + F5 或 [偵錯] > [啟動但不偵錯] 功能表
// 偵錯程式: F5 或 [偵錯] > [啟動偵錯] 功能表

// 開始使用的提示: 
//   1. 使用 [方案總管] 視窗，新增/管理檔案
//   2. 使用 [Team Explorer] 視窗，連線到原始檔控制
//   3. 使用 [輸出] 視窗，參閱組建輸出與其他訊息
//   4. 使用 [錯誤清單] 視窗，檢視錯誤
//   5. 前往 [專案] > [新增項目]，建立新的程式碼檔案，或是前往 [專案] > [新增現有項目]，將現有程式碼檔案新增至專案
//   6. 之後要再次開啟此專案時，請前往 [檔案] > [開啟] > [專案]，然後選取 .sln 檔案
