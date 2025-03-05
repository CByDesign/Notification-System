#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mqueue.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>

#define MAX_MSG_SIZE 256
#define PRODUCER_QUEUE "/producer_queue"
#define CLIENT_QUEUE "/client_queue"
#define DISPATCHER_QUEUE "/dispatcher_queue"

typedef struct {
    char type[32];
    char message[MAX_MSG_SIZE];
} Notification;

typedef struct {
    char client_id[32];
    char type[32];
} Subscription;

Subscription subscriptions[100];
int subscription_count = 0;

mqd_t producer_queue, client_queue, dispatcher_queue;

void cleanup() {
    mq_close(producer_queue);
    mq_close(client_queue);
    mq_close(dispatcher_queue);
    mq_unlink(PRODUCER_QUEUE);
    mq_unlink(CLIENT_QUEUE);
    mq_unlink(DISPATCHER_QUEUE);
}

void handle_signal(int sig) {
    cleanup();
    exit(0);
}

void dispatch_notification(Notification *notification) {
    for (int i = 0; i < subscription_count; i++) {
        if (strcmp(subscriptions[i].type, notification->type) == 0) {
            mq_send(client_queue, (char *)notification, sizeof(Notification), 0);
        }
    }
}

int main() {
    signal(SIGINT, handle_signal);

    struct mq_attr attr = {0, 10, sizeof(Notification), 0};
    dispatcher_queue = mq_open(DISPATCHER_QUEUE, O_CREAT | O_RDONLY, 0644, &attr);
    producer_queue = mq_open(PRODUCER_QUEUE, O_CREAT | O_RDONLY, 0644, &attr);
    client_queue = mq_open(CLIENT_QUEUE, O_CREAT | O_WRONLY, 0644, &attr);

    if (dispatcher_queue == -1 || producer_queue == -1 || client_queue == -1) {
        perror("Error opening message queues");
        cleanup();
        exit(1);
    }

    printf("Dispatcher is running...\n");

    while (1) {
        Notification notification;
        if (mq_receive(producer_queue, (char *)&notification, sizeof(Notification), NULL) > 0) {
            dispatch_notification(&notification);
        }

        Subscription sub;
        if (mq_receive(dispatcher_queue, (char *)&sub, sizeof(Subscription), NULL) > 0) {
            strcpy(subscriptions[subscription_count].client_id, sub.client_id);
            strcpy(subscriptions[subscription_count].type, sub.type);
            subscription_count++;
        }
    }

    cleanup();
    return 0;
}
