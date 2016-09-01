#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>

int main(int argc, char **argv)
{
	// Print usage if arguments were not properly defined.
    if(argc != 2)
    {
        printf("Usage : %s <port> \n", argv[0]);
        exit(1);
    }

    // Initialize Winsock 2.2
	WSADATA wsa_data;
    if(WSAStartup(MAKEWORD(2, 2), &wsa_data) != 0)
    {
        printf("WSAStartup() error! \n");
    }

    // Initialize server socker 
	SOCKET server_socket = socket(PF_INET, SOCK_STREAM, 0);
    if(server_socket == INVALID_SOCKET)
    {
        printf("socket() error\n");
    }

	SOCKADDR_IN server_address;
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    server_address.sin_port = htons(atoi(argv[1]));

    // Bind server socket and server address
    if(bind(server_socket, (SOCKADDR*) &server_address, sizeof(server_address)) == SOCKET_ERROR)
    {
        printf("bind() error\n");
    }

	printf("Waiting for client connection\n");

    // Wait for (listen) connection from client
    if(listen(server_socket, 5) == SOCKET_ERROR)
    {
        printf("listen() error\n");
    }

    // Accept connection from client
	SOCKADDR_IN client_address;
	int size_client_address = sizeof(client_address);
	SOCKET client_socket = accept(server_socket, (SOCKADDR*) &client_address, &size_client_address);
    if(client_socket == INVALID_SOCKET)
    {
		printf("accept() error\n");
    }

    // Send data to client
	char message_buffer[1024];
	strcpy(message_buffer, "Hello from server!~~~~~~~");
    send(client_socket, message_buffer, sizeof(message_buffer) -1, 0);

	// Receive data from client
	int strLen = recv(client_socket, message_buffer, sizeof(message_buffer) - 1, 0);
	if (strLen == -1)
	{
	    printf("read() error");
	}

	message_buffer[strLen] = '\0';
	printf("Message from client : %s \n", message_buffer);

    // close connection
    closesocket(client_socket);
    WSACleanup();

    return 0;
}