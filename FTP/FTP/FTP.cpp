// FTP.cpp : 此檔案包含 'main' 函式。程式會於該處開始執行及結束執行。

#include <iostream>
#include <conio.h>
#include <fstream>
#include <string>
#include <process.h>      // <= _beginthread
#include "TCPIP_Sync.cpp" // <= TCP SDK
// Global Variable
SOCKET  Sock, Sock_NLST, Sock_RETR, Sock_STOR;
int     PASV_Flag = 0; // 0:一般, 1:ls, 2:get a.c, 3:put a.c
char    RETR_File[100], STOR_File[100];

// 從字串()中，擷取IP & Port 
void Get_IP(char *S1, char *IP, int *Port)
{	// 1.找出()位置 Beg, End
	int i, j, k, Beg = -1, End = -1, i1, i2, i3, i4, i5, i6;
	char S2[200];
	for (i = 0; i < strlen(S1); i++) if (S1[i] == '(') { Beg = i;  break; }
	for (i = 0; i < strlen(S1); i++) if (S1[i] == ')') { End = i;  break; }
	// 2.取出字串
	if ((End > (Beg + 1)) && (Beg >= 0) && (End >= 0))
		strncpy_s(S2, &S1[Beg + 1], End - Beg - 1);
	sscanf_s(S2, "%d,%d,%d,%d,%d,%d", &i1, &i2, &i3, &i4, &i5, &i6);
	// 3.還原IP, Port	
	sprintf_s(IP, 199, "%d.%d.%d.%d", i1, i2, i3, i4);
	*Port = i5 * 256 + i6;
}
void Fun_RETR(PVOID p) // 子執行續，接收(a.c檔案內容)資料
{
	int i, Port;
	char S1[2000], IP[200];
	while (1) // NLST 指令接收資料
	{
		i = recv(Sock_RETR, S1, sizeof(S1) - 1, 0);
		if (i > 0) { S1[i] = 0; printf("%s", S1); } // 列印
		else { closesocket(Sock_RETR); break; } // 斷線
	}
}
void Fun_STOR(PVOID p) 
{
	printf("in FUN_STOR");
	int i;
	char S1[2000];
	while (1)
	{
		i = recv(Sock_STOR, S1, sizeof(S1) - 1, 0);
		if (i > 0) { S1[i] = 0; printf("%s", S1); } // 列印
		else { closesocket(Sock_STOR); break; } // 斷線
	}
	

}
void Fun_NLST(PVOID p) // 子執行續，接收(檔案目錄)資料
{
	int i, Port;
	char S1[2000], IP[200];
	while (1) // NLST 指令接收資料
	{
		i = recv(Sock_NLST, S1, sizeof(S1) - 1, 0);
		if (i > 0) { S1[i] = 0; printf("%s", S1); } // 列印
		else { closesocket(Sock_NLST); break; } // 斷線
	}
}void Fun(PVOID p) // 子執行續，接收(命令溝通)資料
{
	int i, Port;
	char S1[2000], IP[200];
	while (1)
	{
		i = recv(Sock, S1, sizeof(S1) - 1, 0);
		if (i > 0) { S1[i] = 0; printf("%s", S1); }
		// ls: 下載目錄
		if (PASV_Flag == 1)
		{
			Get_IP(S1, IP, &Port); PASV_Flag = 0;
			Start_TCP_Client(&Sock_NLST, Port, IP);
			_beginthread(Fun_NLST, 0, 0);
			strcpy_s(S1, sizeof(S1) - 1, "NLST\r\n");
			send(Sock, S1, strlen(S1), 0);
		}
		// get a.c: 下載檔案
		else if (PASV_Flag == 2)
		{
			Get_IP(S1, IP, &Port); PASV_Flag = 0;
			Start_TCP_Client(&Sock_RETR, Port, IP);
			_beginthread(Fun_RETR, 0, 0);
			sprintf_s(S1, sizeof(S1) - 1, "RETR %s\r\n", RETR_File);
			send(Sock, S1, strlen(S1), 0);
		}
		else if (PASV_Flag == 3) {
			/*Get_IP(S1, IP, &Port); PASV_Flag = 0;
			printf("pasv_flag=3");
			Start_TCP_Client(&Sock_STOR, Port, IP);
			_beginthread(Fun_STOR, 0, 0);
			sprintf_s(S1, sizeof(S1) - 1, "STOR %s\r\n", STOR_File);
			send(Sock, S1, strlen(S1), 0);*/
			std::string content;
			std::ifstream uploadfile("./../FTP/FTP.cpp");
			char content_c[5000];
			Get_IP(S1, IP, &Port); PASV_Flag = 0;
			Start_TCP_Client(&Sock_STOR, Port, IP);
			_beginthread(Fun_STOR, 0, 0);
			sprintf_s(S1, sizeof(S1) - 1, "STOR %s\r\n", STOR_File);
			send(Sock, S1, strlen(S1), 0);

			while (std::getline(uploadfile, content)) {
				printf("%s \n", content.c_str());
				strcpy_s(content_c, content.c_str());
				strcat_s(content_c, "\r\n");
				send(Sock_STOR, content_c, strlen(content_c), 0);
			}
			Sleep(1000);
			closesocket(Sock_STOR);
			uploadfile.close();
			

		}
	}
}
/*
 *hw:
 *ip 140.113.170.87  port 13579
 *帳:nctu123 密 : 123nctu
 *這次作業請在根路徑下建立資料夾（名為學號），再上傳學號.cpp（你自己的源碼）
 *in class:
 *"140.113.9.151" Port = 21 E3account
 */
int main()
{   // 1.宣告變數
	char    IP[100] = "140.113.170.87";
	int     i, Port = 13579;
	char    S1[2000], Acnt[100], Pass[100];
	system("chcp 65001"); // 啟動螢幕UTF8編碼格式顯示

	// 2.連線FTP Site(140.113.9.151:21)
	Start_TCP_Client(&Sock, Port, IP);
	_beginthread(Fun, 0, 0); // 啟動執行續接收recv訊息
	// 3.送帳號("USER abcd\r\n")
	Sleep(1000); // 暫停1秒
	printf("Account="); scanf_s("%s", Acnt, sizeof(Acnt) - 1);
	sprintf_s(S1, "USER %s\r\n", Acnt);
	send(Sock, S1, strlen(S1), 0);
	// 4.送密碼("PASS 1234\r\n")
	Sleep(1000); // 暫停1秒
	printf("Password=");
	//scanf_s("%s",Pass,sizeof(Pass)-1);
	char c;
	i = 0;
	do {
		c = _getch(); Pass[i++] = c; printf(".");
	} while ((c != 0x0a) && (c != 0x0d));
	Pass[i - 1] = 0; // 字串結尾
	sprintf_s(S1, "PASS %s\r\n", Pass);
	send(Sock, S1, strlen(S1), 0);
	// 5.進入讀鍵盤，送send命令
	Sleep(1000); // 暫停1秒
	while (1)
	{
		printf("FTP> ");    i = 0;
		do {
			c = _getche(); S1[i++] = c;
		} while ((c != 0x0a) && (c != 0x0d));
		S1[i - 1] = 0; // 去跳行，加字串結尾
		// 1.檢查指令 ls
		if ((!strcmp(S1, "ls")) || (!strcmp(S1, "LS")))
		{
			strcpy_s(S1, sizeof(S1), "PASV"); PASV_Flag = 1;
		}
		// 2.檢查指令 get a.c
		else if ((!strncmp(S1, "get", 3)) || (!strncmp(S1, "GET", 3)))
		{
			strcpy_s(RETR_File, &S1[4]);
			strcpy_s(S1, sizeof(S1), "PASV"); PASV_Flag = 2;
		}
		// 3.檢查指令 put a.c
		else if ((!strncmp(S1, "put", 3)) || (!strncmp(S1, "PUT", 3)))
		{
			strcpy_s(STOR_File, &S1[4]);
			strcpy_s(S1, sizeof(S1), "PASV"); PASV_Flag = 3;
		}

		printf("\r\n"); // 跳行
		strcat_s(S1, "\r\n"); // 補跳行(FTP協定要求)
		send(Sock, S1, strlen(S1), 0);
	}
	// 註:(不帶參數指令)
	//     PWD, HELP, QUIT, STAT..... 
	// 註:(帶參數指令)
	//     CWD For_Linux   進入子目錄 For_Linux
	//     CWD ..          跳上一層目錄
	//     TYPE A, TYPE I  設定傳輸模式(文字 or Binary)
	//     MKD abc         建立子目錄
	//     DELE a.c        刪除檔案a.c
	// 註:(特殊指令)
	//     1.下載目錄(採用被動模式: PASV+NLST)
	//       a. "PASV\r\n" => 送出
	//       b. Get IP & Port
	//       c. Start_TCP_Client(IP & Port)，建第二條連線
	//       d. _beginthread(Fun_NLST)
	//       e. "NLST\r\n" => 送出
	//       f. Fun_NLST中，讀取(recv)目錄資料
	//     2.下載檔案(採用被動模式: PASV+RETR a.c)
	//       ...  
	//       e. "RETR a.c\r\n" => 送出 
	//       ...
	//     3.上傳檔案(採用被動模式: PASV+STOR a.c)
	//       ...  
	//       e. "STOR a.c\r\n" => 送出 
	//       ...
}




