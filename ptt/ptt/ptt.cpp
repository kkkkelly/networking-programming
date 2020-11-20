// ptt.cpp : 此檔案包含 'main' 函式。程式會於該處開始執行及結束執行。
// BBS(ptt)終端程式設計

#include <iostream>
#include <process.h>		//for thread
#include <conio.h>
#include "TCPIP_Sync.cpp"	//tcp

SOCKET Sock;				//global socket for ptt connet

//read message form ptt 
void Fun(PVOID p) {
	while (1) {
		int i;
		char S1[2000];
		i = recv(Sock, S1, sizeof(S1) - 1, 0);	//收資料，等待收資料(沒收到資料不會繼續)
		if (i >= 0) {
			S1[i] = 0;	//補上字串結尾(BOS)
			printf("%s", S1);
		}
		else {
			printf("PTT 斷線.....\r\n");
			closesocket(Sock);
			break;
		}
	}
}

int main()
{	
	char S1[2000];
    //連線bbs server "bbs.badcow.com.tw 23"
	//連線 痞客幫
	Start_TCP_Client(&Sock, 23, (char*)"61.31.92.42");//"61.31.92.42""103.23.108.220"
	_beginthread(Fun,0,0);
	Sleep(2000);
	//
	strcpy_s(S1,"GET / HTTP/1.1\r\n\r\n" );
	send(Sock, S1,strlen(S1),0);
	//read keyboard => send command to server
	unsigned char c;
	char Code[4][3] = { {0x1B,0x5B,0x41} ,{0x1B,0x5B,0x42},
						{0x1B,0x5B,0x43} ,{0x1B,0x5B,0x44} };
	int F = 0;
	while (1) {
		c = _getch();	//getch讀取鍵盤不會秀出來 getche會
		//printf("c = %c code = 0x%2x \r\n",c, c);

		if (F == 1) {
			if (c == 0x48) send(Sock, &Code[0][0], 3, 0);
			else if (c == 0x50) send(Sock, &Code[1][0], 3, 0);
			else if (c == 0x4B) send(Sock, &Code[2][0], 3, 0);
			else if (c == 0x4D) send(Sock, &Code[0][0], 3, 0);
			F = 0;
		}
		else if(c == 0xE0) {
			F = 1;
		}
		else {
			send(Sock, (char *)&c, 1, 0);	//送按鍵給server
		}
		

	}
	/*
	*	BBS 上下左右碼
	*	按鍵:上	  鍵盤碼:E0 48		BBS接受碼: 1B 5B 41
	*	按鍵:下	  鍵盤碼:E0 50		BBS接受碼: 1B 5B 42
	*	按鍵:左   鍵盤碼:E0 4B		BBS接受碼: 1B 5B 44
	*	按鍵:右   鍵盤碼:E0 4D		BBS接受碼: 1B 5B 43
	*/
}

