#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <error.h>
#include <string.h>
#include <netinet/in.h>
#include <stdlib.h>

#define PORT 5998
#define MAX_CONNCT 20
#define BUFF_LEN 2048

char *buffer;
unsigned int read_len;

int main(int argc, char const *argv[]) {
	int sockfd;										// Socket file descriptor
	struct sockaddr_in addr;						// Details related to socket address
	socklen_t sin_size;
	int client_addr;
	int client_fd;

	buffer = new char[BUFF_LEN];

	sockfd = socket(AF_INET, SOCK_STREAM, 0);		// Create Socket

	if (sockfd < 0) {
		perror("Socket creation failed!");
		exit(-1);
	}

	memset(&addr, 0, sizeof(addr));	// Setting the struct to 0s

	addr.sin_family = AF_INET;
	addr.sin_port = htonl(PORT);
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	// Error Checking
	if (bind(sockfd, (struct sockaddr *)&addr, sizeof(struct sockaddr)) < 0) {
		perror("Binding to socket failed!");
		exit(-1);
	}

	if (listen(sockfd, MAX_CONNCT) < 0) {
		perror("Listening failed!");
		exit(-1);
	}

	fprintf(stdout, "Server is listening on Port: %d\n", PORT);
	
	// Forever loop to accept connections
	while (1) {
		sin_size = sizeof(struct sockaddr_in);
    	client_fd = accept(sockfd, (struct sockaddr *)&client_addr, &sin_size);

		if (client_fd < 0) {
			perror("Error connecting to the Client!");
			exit(-1);
		}

		read_len = recv(client_fd, buffer, MAX_CONNCT, 0);

		if (read_len == 0) {
			fprintf(stderr, "Client has closed the connection!");
		}
		if (read_len < 0) {
			perror("Error reading from Socket!");
			exit(-1);
		}
		// std::cout << buffer;
	}

	delete buffer;
	return 0;
}
