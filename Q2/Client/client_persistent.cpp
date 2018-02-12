#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <arpa/inet.h>

#define PORT 5998
#define MAX_BUFF_LEN 1000009
#define HOST "127.0.0.1"

char buffer[MAX_BUFF_LEN];


int main(int argc, char const *argv[]) {
	if (argc < 2) {
		fprintf(stderr, "usage: ./client [<filenames>]\n");
		exit(-1);
	}

	struct sockaddr_in serv_addr;

	int server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (server_fd < 0) {
		fprintf(stderr, "Error: Socket could not be created.\n");
		exit(-1);
	}

	memset(&serv_addr, '0', sizeof(serv_addr));

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(PORT);

	if (inet_pton(AF_INET, HOST, &serv_addr.sin_addr) <= 0) {
		fprintf(stderr, "Error: Invalid HOST address (\"127.0.0.1\")");
		exit(-1);
	}

	if (connect(server_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
		fprintf(stderr, "Error: Connection failed.\n");
		exit(-1);
	}

	int bytesRead = 0, fp;
	for (int i=1 ; i<argc ; ++i) {
		send(server_fd , argv[i] , strlen(argv[i]) , 0);

		bytesRead = read(server_fd , buffer, 1);

		if (buffer[0] == 'N') {
			fprintf(stderr, "Error: File %s does not exist.\n", argv[i]);
			continue;
		}

		fp = open(argv[i], O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);

		fprintf(stdout, "Transferring %s...\n", argv[i]);

		while ((bytesRead = read(server_fd, buffer, MAX_BUFF_LEN)) > 0) {
			write(fp, buffer, bytesRead);
			if (bytesRead == -1)
				fprintf(stderr, "Error: Could not read from buffer.\n");
			if (bytesRead < MAX_BUFF_LEN)
				break;
		}

		fprintf(stdout, "Transferred %s.\n", argv[i]);

		close(fp);
	}

	close(server_fd);
	return 0;
}
