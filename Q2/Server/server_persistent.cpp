#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/sendfile.h>

#define PORT 5998
#define MAX_BUFF_LEN 10007

int main(int argc, char const *argv[]) {
	int server_fd, new_socket;
	struct sockaddr_in address;  // sockaddr_in - references elements of the socket address. "in" for internet
	int opt = 1;
	int addrlen = sizeof(address);
	char buffer[1024] = {0};
	struct stat fileStat;

	// Creating socket file descriptor
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)  // creates socket, SOCK_STREAM is for TCP. SOCK_DGRAM for UDP
	{
		perror("socket failed");
		exit(EXIT_FAILURE);
	}

	// This is to lose the pesky "Address already in use" error message
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
												  &opt, sizeof(opt))) // SOL_SOCKET is the socket layer itself
	{
		perror("setsockopt");
		exit(EXIT_FAILURE);
	}
	address.sin_family = AF_INET;  // Address family. For IPv6, it's AF_INET6. 29 others exist like AF_UNIX etc.
	address.sin_addr.s_addr = INADDR_ANY;  // Accept connections from any IP address - listens from all interfaces.
	address.sin_port = htons( PORT );    // Server port to open. Htons converts to Big Endian - Left to Right. RTL is Little Endian

	// Forcefully attaching socket to the port 8080
	if (bind(server_fd, (struct sockaddr *)&address,
								 sizeof(address))<0)
	{
		perror("bind failed");
		exit(EXIT_FAILURE);
	}
	// Port bind is done. You want to wait for incoming connections and handle them in some way.
	// The process is two step: first you listen(), then you accept()
	if (listen(server_fd, 3) < 0) // 3 is the maximum size of queue - connections you haven't accepted
	{
		perror("listen");
		exit(EXIT_FAILURE);
	}
	// returns a brand new socket file descriptor to use for this single accepted connection. Once done, use send and recv
	if ((new_socket = accept(server_fd, (struct sockaddr *)&address,
					   (socklen_t*)&addrlen))<0)
	{
		perror("accept");
		exit(EXIT_FAILURE);
	}

	int bytesRead = 0;
	while(true) {
		bytesRead = read(new_socket , buffer, MAX_BUFF_LEN);

		char path[20];
		strcpy(path, "Data/");

		for (int i=0 ; i<bytesRead ; ++i)
			path[i + 5] = buffer[i];
		path[bytesRead + 5] = '\0';

		int fp = open(path, O_RDONLY);
		if(fp == -1) {
			send(new_socket, "N", 1, 0);
			continue;
		}

		send(new_socket, "Y", 1, 0);

		stat(path , &fileStat);

		sendfile(new_socket, fp, NULL, fileStat.st_size);
	}

	close(new_socket);

	return 0;
}
