#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>

#pragma comment(lib, "Ws2_32.lib")

#define DEFAULT_PORT "27015"
#define DEFAULT_BUFLEN 512

int main()
{
	WSADATA wsaData;
	int iResult;
	char recvbuf[DEFAULT_BUFLEN] = {};
	int recvbuflen = DEFAULT_BUFLEN;

	SOCKET ListenSocket = INVALID_SOCKET;

	struct addrinfo* result = NULL, hints;
	sockaddr_in clntAddr;
	int clntAddrSize = sizeof(clntAddr);

	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0)
	{
		printf("WSAStartup failed with error: %d\n", iResult);
		return 1;
	}

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_DGRAM;
	hints.ai_protocol = IPPROTO_UDP;
	hints.ai_flags = AI_PASSIVE;

	iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
	if (iResult != 0)
	{
		printf("getaddrinfo failed: %d\n", iResult);
		WSACleanup();
		return 1;
	}

	ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (ListenSocket == INVALID_SOCKET)
	{
		printf("Error at socket(): %ld\n", WSAGetLastError());
		freeaddrinfo(result);
		WSACleanup();
		return 1;
	}

	iResult = bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);
	if (iResult == SOCKET_ERROR)
	{
		printf("bind failed with error: %d\n", WSAGetLastError());
		freeaddrinfo(result);
		closesocket(ListenSocket);
		WSACleanup();
		return 1;
	}

	freeaddrinfo(result);

	printf("Socket has created, now running.\n");

	while(strcmp(recvbuf, "!q") != 0)
	{
		//recvfrom function transmits the information of senders to clntAddr variable
		iResult = recvfrom(ListenSocket, recvbuf, recvbuflen, NULL, (SOCKADDR*)&clntAddr, &clntAddrSize);
		//iResult = recv(ListenSocket, recvbuf, recvbuflen, NULL);
		printf("%s\n", recvbuf);
		//eco
		static const char* eco = "hi, this is eco.\n";
		static const int ecoLen = strlen(eco) + 1;
		sendto(ListenSocket, eco, ecoLen, 0, (SOCKADDR*)&clntAddr, (int)sizeof(clntAddr));

		printf("\n");
	}

	printf("Connection Terminated");

	// cleanup
	closesocket(ListenSocket);
	WSACleanup();

	return 0;
}