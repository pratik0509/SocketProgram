#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <unistd.h>

#define PORT 5998
#define MAX_CONNCT 20
#define BUFF_LEN 2048
#define WRITE_BUFF_LEN 100009
#define FILE_NOT_FOUND "Specified File either doesn't exist or permission denied!"

char *buffer;
char *write_buffer;
unsigned int read_len;

int main(int argc, char const *argv[]) {
	int sockfd;										// Socket file descriptor
	struct sockaddr_in addr;						// Details related to socket address
	socklen_t sin_size;
	int client_addr;
	int client_fd;
	int fd;
	int opt = 1;
	ssize_t read_bytes;
	off_t offset;

	buffer = new char[BUFF_LEN];
	write_buffer = new char[WRITE_BUFF_LEN];
	sockfd = socket(AF_INET, SOCK_STREAM, 0);		// Create Socket

	if (sockfd <= 0) {
		perror("Socket creation failed!");
		exit(-1);
	}
	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
	                                                  &opt, sizeof(opt))) {
	        perror("setsockopt");
	        exit(EXIT_FAILURE);
    }

	memset(&addr, 0, sizeof(addr));	// Setting the struct to 0s

	addr.sin_family = AF_INET;
	addr.sin_port = htons(PORT);
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
		offset = 0;
		sin_size = sizeof(struct sockaddr);
    	client_fd = accept(sockfd, (struct sockaddr *)&addr, (socklen_t*)&sin_size);

		if (client_fd < 0) {
			perror("Error connecting to the Client!");
			exit(-1);
		}

		read_len = read(client_fd, buffer, BUFF_LEN);
		fprintf(stderr, "Reading file %s\n", buffer);

		if (read_len == 0) {
			fprintf(stderr, "Client has closed the connection!");
		}

		if (read_len < 0) {
			perror("Error reading from Socket!");
			exit(-1);
		}

		fd = open(buffer, O_RDONLY);
		if (fd < 0) {
			perror("Error reading file!");
			write(client_fd, FILE_NOT_FOUND, strlen(FILE_NOT_FOUND));
			close(client_fd);
			continue;
		}
		offset = 0;
		read_bytes = 0;
		fprintf(stdout, "Reading and writing the file %s\n", buffer);
		while ((read_bytes = read(fd, write_buffer, WRITE_BUFF_LEN)) > 0) {
			write(client_fd, write_buffer, read_bytes);
			offset += read_bytes;
		}
		if (read_bytes < 0) {
			fprintf(stderr, "Error encountered while writing!");
			exit(-1);
		} else {
			fprintf(stdout, "File written successfully!");
		}
		close(fd);
		close(client_fd);
	}

	delete buffer;
	delete write_buffer;
	return 0;
}
