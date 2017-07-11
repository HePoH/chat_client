#include "../include/core.h"
/*#include "../include/graph.h"
#include "../include/popup.h"*/

int main(){
	char client_name[MAX_MSG_SIZE];
	mqd_t qd_server, qd_client;

	memset(client_name, 0, MAX_MSG_SIZE);
	sprintf(client_name, "client-%d", getpid());

	qd_server = mq_open(SERVER_QUEUE_NAME, O_RDWR);
	if (qd_server == -1) {
		perror("Client: mq_open(server)");
		exit(EXIT_FAILURE);
	}

	qd_client = mq_open(CLIENT_QUEUE_NAME, O_RDWR);
	if (qd_client == -1) {
		perror("Client: mq_open(client)");
		exit(EXIT_FAILURE);
	}

	char in_buffer[MSG_BUFFER_SIZE];
	printf("Ask for a token (Press <ENTER>): ");
	char temp_buf[10];

	while (fgets(temp_buf, 2, stdin)) {
		if (mq_send(qd_server, client_name, strlen(client_name), 0) == -1) {
			perror("Client: Not able to send message to server");
			continue;
		}

		if (mq_receive(qd_client, in_buffer, MSG_BUFFER_SIZE, NULL) == -1) {
			perror("Client: mq_receive(client)");
			exit(EXIT_FAILURE);
		}

		printf("Client: Token received from server: %s\n\n", in_buffer);
		printf("Ask for a token (Press): ");
	}

	if (mq_close(qd_server) == -1) {
		perror ("Client: mq_close(server)");
		exit(EXIT_FAILURE);
	}

	if (mq_close(qd_client) == -1) {
		perror ("Client: mq_close(client)");
		exit(EXIT_FAILURE);
	}

	printf("Client: bye\n");

	exit(EXIT_SUCCESS);
}
