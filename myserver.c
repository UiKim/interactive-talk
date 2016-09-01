#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>

int main(int argc, char **argv)
{
	// Print usage if arguments were not properly defined.
	if (argc != 2)
	{
		printf("Usage : %s <port> \n", argv[0]);
		exit(1);
	}

	// Initialize Winsock 2.2
	WSADATA wsa_data;
	if (WSAStartup(MAKEWORD(2, 2), &wsa_data) != 0)
	{
		printf("WSAStartup() error! \n");
	}

	// Initialize server socker 
	SOCKET server_socket = socket(PF_INET, SOCK_STREAM, 0);
	if (server_socket == INVALID_SOCKET)
	{
		printf("socket() error\n");
	}

	SOCKADDR_IN server_address;
	memset(&server_address, 0, sizeof(server_address));
	server_address.sin_family = AF_INET;
	server_address.sin_addr.s_addr = htonl(INADDR_ANY);
	server_address.sin_port = htons(atoi(argv[1]));

	// Bind server socket and server address
	if (bind(server_socket, (SOCKADDR*)&server_address, sizeof(server_address)) == SOCKET_ERROR)
	{
		printf("bind() error\n");
	}

	printf("Waiting for client connection\n");

	// Wait for (listen) connection from client
	if (listen(server_socket, 5) == SOCKET_ERROR)
	{
		printf("listen() error\n");
	}

	// Accept connection from client
	SOCKADDR_IN client_address;
	int size_client_address = sizeof(client_address);
	SOCKET client_socket = accept(server_socket, (SOCKADDR*)&client_address, &size_client_address);
	if (client_socket == INVALID_SOCKET)
	{
		printf("accept() error\n");
	}

	// Send data to client
	char message_server[1024];
	char message_client[1024];
	strcpy(message_server, "message from server: Hi bro~~\n");
	send(client_socket, message_server, sizeof(message_server)-1, 0);

	while (1){
		// Receive data from client
		int strLen = recv(client_socket, message_client, sizeof(message_client)-1, 0);
		if (strLen == -1)
		{
			printf("read() error");
		}

		message_client[strLen] = '\0';
		printf("Message from client : %s \n", message_client);
		strcpy(message_client,"\0");

		printf("Your message to clinet:");
		fgets(message_server, sizeof(message_server), stdin);
		message_server[strLen-1] = 0;
		send(client_socket, message_server, sizeof(message_server)-1, 0);

		if (strcmp(message_server, "end\n") == 0)
			break;
		strcpy(message_server, "\0");
	}

	// close connection
	closesocket(client_socket);
	WSACleanup();

	return 0;
}