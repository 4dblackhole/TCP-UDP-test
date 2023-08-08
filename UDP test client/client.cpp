#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>

#pragma comment(lib, "Ws2_32.lib")

#define DEFAULT_PORT "27015"
#define DEFAULT_BUFLEN 512

int main(int argc, char** argv)
{
	WSADATA wsaData;
	SOCKET ConnectSocket = INVALID_SOCKET;

	int iResult;

	//Winsock Init
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0)
	{
		printf("WSAStartup failed: %d\n", iResult);
		return 1;
	}

	struct addrinfo* result = NULL, hints;

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_DGRAM;
	hints.ai_protocol = IPPROTO_UDP;

	iResult = getaddrinfo(argv[1], DEFAULT_PORT, &hints, &result);
	if (iResult != 0)
	{
		printf("getaddrinfo failed :%d\n", iResult);
		WSACleanup();
		return 1;
	}

	ConnectSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol); //家南 积己
	if (ConnectSocket == INVALID_SOCKET)
	{
		printf("socket() error: %ld\n", WSAGetLastError());
		freeaddrinfo(result);
		WSACleanup();
		return 1;
	}

	iResult = connect(ConnectSocket, result->ai_addr, (int)result->ai_addrlen); //辑滚 楷搬
	if (iResult == SOCKET_ERROR)
	{
		closesocket(ConnectSocket);
		ConnectSocket = INVALID_SOCKET;
	}

	//end of use
	freeaddrinfo(result);

	//const char* sendbuf = "this is a test";
	int recvbuflen = DEFAULT_BUFLEN;
	char recvbuf[DEFAULT_BUFLEN] = {};
	char sendbuf[DEFAULT_BUFLEN] = {};

	printf("Socket has created, enter the words.\n");

	while (strcmp(sendbuf, "!q") != 0)
	{
		sprintf_s(sendbuf, "");
		sprintf_s(recvbuf, "");

		printf(">> ");
		fgets(sendbuf, DEFAULT_BUFLEN, stdin);
		size_t lastIdx = strlen(sendbuf) - 1;
		sendbuf[lastIdx] = '\0';

		iResult = send(ConnectSocket, sendbuf, (int)strlen(sendbuf) + 1, 0);
		if (iResult == SOCKET_ERROR)
		{
			printf("send failed: %d\n", WSAGetLastError());
			closesocket(ConnectSocket);
			WSACleanup();
			return 1;
		}


		const static struct timeval optVal = { 1000,0 }; //ms unit
		const static int optLen = sizeof(optVal);
		setsockopt(ConnectSocket, SOL_SOCKET, SO_RCVTIMEO, (const char*)&optVal, optLen);

		iResult = recv(ConnectSocket, recvbuf, recvbuflen, NULL);
		if (iResult > 0)
			printf("Bytes: %d\nText: %s", iResult, recvbuf);
		else if (iResult == 0)
			printf("Connection closed\n");
		else
			printf("recv failed: %d\n", WSAGetLastError());

		printf("=======================================\n");
	}

	// cleanup
	closesocket(ConnectSocket);
	WSACleanup();

	return 0;
}