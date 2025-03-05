#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mqueue.h>
#include <fcntl.h>
#include <unistd.h>

#define CLIENT_QUEUE "/client_queue"
#define DISPATCHER_QUEUE "/dispatcher_queue"

typedef struct {
    char type[32];
    char message[256];
} Notification;

typedef struct {
    char client_id[32];
    char type[32];
} Subscription;

int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <client_id> <subscription_type>\n", argv[0]);
        exit(1);
    }

    const char *client_id = argv[1];
    const char *subscription_type = argv[2];

    mqd_t dispatcher_queue = mq_open(DISPATCHER_QUEUE, O_WRONLY);
    mqd_t client_queue = mq_open(CLIENT_QUEUE, O_RDONLY);
    if (dispatcher_queue == -1 || client_queue == -1) {
        perror("Failed to open queues");
        exit(1);
    }

    Subscription sub;
    strncpy(sub.client_id, client_id, sizeof(sub.client_id));
    strncpy(sub.type, subscription_type, sizeof(sub.type));

    if (mq_send(dispatcher_queue, (char *)&sub, sizeof(sub), 0) == -1) {
        perror("Failed to send subscription");
        mq_close(dispatcher_queue);
        mq_close(client_queue);
        exit(1);
    }

    printf("Subscribed to notifications of type '%s'\n", subscription_type);

    while (1) {
        Notification notification;
        if (mq_receive(client_queue, (char *)&notification, sizeof(Notification), NULL) > 0) {
            printf("Received notification: %s\n", notification.message);
        }
    }

    mq_close(dispatcher_queue);
    mq_close(client_queue);
    return 0;
}
