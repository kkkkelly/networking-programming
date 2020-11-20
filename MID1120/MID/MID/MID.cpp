// MID.cpp : 此檔案包含 'main' 函式。程式會於該處開始執行及結束執行。
//

#include <iostream>
#include "TCPIP_Sync.cpp"
#include "MD5.c"
#include <process.h>
#include <string> 

SOCKET Sock;

#define MAX_DIGITS 5
int Decode(char *code) {
	int i,j;
	char Resp[200][100];
	char S1[200];
	char num_char[MAX_DIGITS + sizeof(char)];
	printf("in decode: %s\r\n", code);
	//介於[0~32767]整數經過200次MD5攪亂(Hash)之結果
	for (i = 1; i < 32767;i++) {
		sprintf_s(num_char, "%d", i);
		printf("num_char %s\r\n",num_char);
		MD5_Encoder(num_char, Resp[0]);
		
		
		for (j = 1; j < 200; j++) {
			MD5_Encoder(Resp[j-1], Resp[j]);
			//printf("%d: %s\r\n", j,Resp[j]);
		}
		
		
		printf("%d Answer: %s\r\n", i,Resp[199]);

		//printf("Resp %d : %s",i, Resp);
		if (strcmp(code, Resp[199]) == 0) {
			printf("Answer: %s\r\n", Resp);
			printf("num is %d", i);
			sprintf_s(S1, "ANSWER/陳芳瑩/1090123/%d",i);
			send(Sock, S1, strlen(S1), 0);

			break;
		}
		
		
	}

	return 1;
}
void Fun(PVOID p) {
	int first=0;
	int i,j,Beg=-1,r;
	char S1[2000];
	char S2[200];//for code
	while (1) {
		i = recv(Sock, S1, sizeof(S1) - 1, 0);
		if (i > 0) {
			S1[i] = 0;
			printf("%s\r\n", S1);
			//the code
			if (first == 0) {
				for (j = 0; j< strlen(S1); j++) if (S1[j] == '/') { Beg = j;  break; }
				//printf("%d", Beg);
				if (Beg >= 0) {
					strncpy_s(S2, &S1[Beg + 1], 41 - Beg - 1);
					printf("S2: %s\n",S2);
				}
				r=Decode(S2);
				first = 1;
			}
			
			
		}
	}
	
	
}

int main()
{
	//setup TCPclient
	Start_TCP_Client(&Sock, 6000, (char*)"140.113.144.227");//port 80(http) port 21(ftp)
	//setup thread 
	_beginthread(Fun, 0, 0);

	//read&send
	char S1[2000];
	strcpy_s(S1, "REGISTER/陳芳瑩/0610764");
	send(Sock, S1, strlen(S1), 0);

	//多餘的迴圈，當初是為了不要讓main結束而加的
	while (1) {
		scanf_s("%s", S1, sizeof(S1));
		send(Sock, S1, strlen(S1), 0);
	}
}

