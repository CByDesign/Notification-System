#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mqueue.h>
#include <fcntl.h>
#include <unistd.h>

#define PRODUCER_QUEUE "/producer_queue"

typedef struct {
    char type[32];
    char message[256];
} Notification;

int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <notification_type> <message>\n", argv[0]);
        exit(1);
    }

    const char *type = argv[1];
    const char *message = argv[2];

    mqd_t producer_queue = mq_open(PRODUCER_QUEUE, O_WRONLY);
    if (producer_queue == -1) {
        perror("Failed to open producer queue");
        exit(1);
    }

    Notification notification;
    strncpy(notification.type, type, sizeof(notification.type));
    strncpy(notification.message, message, sizeof(notification.message));

    if (mq_send(producer_queue, (char *)&notification, sizeof(notification), 0) == -1) {
        perror("Failed to send notification");
        mq_close(producer_queue);
        exit(1);
    }

    printf("Sent notification of type '%s' with message: '%s'\n", type, message);
    mq_close(producer_queue);
    return 0;
}
