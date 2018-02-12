#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <error.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 5998
#define BUFF_LEN 2048
#define WRITE_BUFF_LEN 100009
#define HOST "127.0.0.1"

int main(int argc, char const *argv[]) {
	if (argc < 2) {
		fprintf(stderr, "Give file name to be retrieved!\n");
		exit(-1);
	}

	struct sockaddr_in server_addr;
	int server_fd;
	int file;
	ssize_t read_bytes;
	char *write_buffer = new char[WRITE_BUFF_LEN];

	server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (server_fd < 0) {
		fprintf(stderr, "Error opening socket\n");
	}

	memset(&server_addr, 0, sizeof(struct sockaddr_in));

	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(PORT);

	if (inet_pton(AF_INET, HOST, &(server_addr.sin_addr)) <= 0) {
		fprintf(stderr, "Invalid Host Address\n%s\n", HOST);
		exit(-1);
	}

	if (connect(server_fd, (struct sockaddr *)&server_addr, sizeof(struct sockaddr)) == -1) {
		perror("Connection to server Failed\n");
		exit(-1);
	}
	fprintf(stdout, "File Name sent to the server %s\n", argv[1]);
	send(server_fd, argv[1], strlen(argv[1]), 0);
	file = open(argv[1], O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR);

	while ((read_bytes = read(server_fd, write_buffer, WRITE_BUFF_LEN)) > 0) {
		write(file, write_buffer, read_bytes);
	}

	fprintf(stdout, "Completed transfer\n");
	return 0;
}
