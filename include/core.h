#ifndef CORE_H
#define CORE_H

#define SERVER_QUEUE_NAME "/queue-chat-server"
#define CLIENT_QUEUE_NAME "/queue-chat-client"

#define QUEUE_PERMISSIONS 0666
#define MAX_MESSAGES 256
#define MAX_MSG_SIZE 256
#define MSG_BUFFER_SIZE MAX_MSG_SIZE + 10

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>

#include <unistd.h>
#include <fcntl.h>
#include <mqueue.h>

#endif

