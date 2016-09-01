#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>

int main(int argc, char **argv)
{
	// Print usage if arguments were not properly defined.
	if (argc != 3)
	{
		printf("Usage : %s <ip> <port>\n", argv[0]);
		exit(1);
	}

	// Initialize WinSocket 2.2
	WSADATA wsa_data;
	if (WSAStartup(MAKEWORD(2, 2), &wsa_data) != 0)
	{
		printf("WSAStartup() error\n");
	}

	// Initialize socket
	SOCKET h_socket = socket(PF_INET, SOCK_STREAM, 0);
	if (h_socket == INVALID_SOCKET)
	{
		printf("socket(), error\n");
	}

	// Initialize server address and port
	SOCKADDR_IN server_address;
	memset(&server_address, 0, sizeof(server_address));
	server_address.sin_family = AF_INET;
	server_address.sin_addr.s_addr = inet_addr(argv[1]);
	server_address.sin_port = htons(atoi(argv[2]));

	// Connect to server
	if (connect(h_socket, (SOCKADDR*)&server_address, sizeof(server_address)) == SOCKET_ERROR)
	{
		printf("Connect() error\n");
	}

	// Receive data from server
	// Note that server is programmed to send hello message.
	char message_buffer[1024];
	int strLen = recv(h_socket, message_buffer, sizeof(message_buffer) - 1, 0);
	if (strLen == -1)
	{
		printf("read() error\n");
	}

	// Print the message received from the server
	message_buffer[strLen] = '\0';
	printf("Message from server : %s \n", message_buffer);

	fgets(message_buffer,sizeof(message_buffer) ,stdin);
	message_buffer[strlen(message_buffer) - 1] = 0;

	// Send message to server
	//strcpy(message_buffer, "Hello from client");
	send(h_socket, message_buffer, sizeof(message_buffer) -1, 0);

	// Close connection
	closesocket(h_socket);
	WSACleanup();

	return 0;
}