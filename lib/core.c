#include "../include/core.h"

void* srv_event_hndl(void* args) {
	mqd_t qd_srv;
        ssize_t bts_num;
        SERVER_MSG srv_msg;
	char* cln_name;

	cln_name = (char*) args;

	qd_srv = mq_open(SERVER_QUEUE_NAME, O_RDWR);
	if (qd_srv == -1) {
		perror("Client: mq_open(server)");
		pthread_exit(EXIT_FAILURE);
	}

	/* Reg */
	srv_msg.msg_type = CONNECTED;
	strncpy(&srv_msg.data, cln_name, MAX_NAME_SIZE);

	bts_num = mq_send(qd_srv, (char*)&srv_msg, SERVER_MSG_SIZE, 0);
	if (bts_num == -1) {
		perror("Client: mq_send(server)");
		pthread_exit(EXIT_FAILURE);
	}

        printf("Client: mq_send(server) send message to server\n");
        printf("Client: SERVER_MSG { msg_type = %d, data = %s }\n", srv_msg.msg_type, srv_msg.data);

	while (1) {
		bts_num = mq_receive(qd_srv, (char*)&srv_msg, SERVER_MSG_SIZE, NULL);
		if (bts_num == -1) {
			perror("Client: mq_receive(server)");
			pthread_exit(EXIT_FAILURE);
		}

		printf("Client: mq_receive(server) message received\n");
		printf("Client: SERVER_MSG { msg_type = %d, data = %s }\n", srv_msg.msg_type, srv_msg.data);

		/*bts_num = mq_send(qd_srv, (char*)&srv_msg, SERVER_MSG_SIZE, 0);
		if (bts_num == -1) {
			perror("Client: mq_send(server)");
			continue;
		}

		printf("Client: mq_send(server) send message to server\n");
		printf("Client: SERVER_MSG { msg_type = %d, data = %s }\n", srv_msg.msg_type, srv_msg.data);*/
	}

	if (mq_close(qd_srv) == -1) {
		perror("Client: mq_close(server)");
		pthread_exit(EXIT_FAILURE);
	}

        printf("Client: server thread bye\n");
        pthread_exit(EXIT_SUCCESS);
}

void* cln_inc_msg_hndl(void* args) {
	mqd_t qd_cln;
	ssize_t bts_num;
	CLIENT_MSG cln_msg;
	char* cln_name;

	cln_name = (char*) args;

	qd_cln = mq_open(CLIENT_QUEUE_NAME, O_RDWR);
	if (qd_cln == -1) {
		perror("Client: mq_open(client)");
		pthread_exit(EXIT_FAILURE);
	}

	while (1) {
		bts_num = mq_receive(qd_cln, (char*)&cln_msg, CLIENT_MSG_SIZE, NULL);
		if (bts_num == -1) {
			perror("Client: mq_receive(client)");
			pthread_exit(EXIT_FAILURE);
		}

		printf("Client: mq_receive(client) message received\n");
		printf("Client: CLIENT_MSG { msg_type = %d, sender = %s [PID: %d], recipient = %s [PID: %d] }\n", cln_msg.msg_type, cln_msg.snd_name,
		cln_msg.snd_pid, cln_msg.rcp_name, cln_msg.rcp_pid);

		/*bts_num = mq_send(qd_cln, (char*)&cln_msg, CLIENT_MSG_SIZE, 0);
		if (bts_num == -1) {
			perror("Client: mq_send(client)");
			continue;
		}

		printf("Client: mq_send(client) response sent to client\n");
		printf("Client: CLIENT_MSG { msg_type = %d, sender = %s [PID: %d], recipient = %s [PID: %d] }\n", cln_msg.msg_type, cln_msg.snd_name,
		cln_msg.snd_pid, cln_msg.rcp_name, cln_msg.rcp_pid);*/
	}

	if (mq_close(qd_cln) == -1) {
		perror("Client: mq_close(client)");
		pthread_exit(EXIT_FAILURE);
	}

	printf("Client: client thread bye\n");
	pthread_exit(EXIT_SUCCESS);
}

void* cln_out_msg_hndl(void* args) {
	mqd_t qd_cln;
	CLIENT_MSG cln_msg;
	char* cln_name, str_msg[MAX_MSG_SIZE];

	cln_name = (char*) args;

	qd_cln = mq_open(CLIENT_QUEUE_NAME, O_RDWR);
	if (qd_cln == -1) {
		perror("Client: mq_open(client)");
		pthread_exit(EXIT_FAILURE);
	}

	while(1) {
		fgets(str_msg, MAX_MSG_SIZE, stdin);

		if (str_msg[strlen(str_msg) - 1] == '\n')
			str_msg[strlen(str_msg) - 1] = '\0';

                if (strcmp("quit", str_msg) == 0 || strcmp("exit", str_msg) == 0)
                        pthread_exit(EXIT_SUCCESS);
	}

	if (mq_close(qd_cln) == -1) {
		perror("Client: mq_close(client)");
		pthread_exit(EXIT_FAILURE);
	}

	printf("Client: client thread bye\n");
        pthread_exit(EXIT_SUCCESS);
}
