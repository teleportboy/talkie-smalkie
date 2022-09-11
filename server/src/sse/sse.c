#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <fcntl.h>

void send_message(char* message, char* chat_uuid) {
    int fd = open("/var/run/hassesfifo", O_WRONLY);
    printf("is opened? %d\n", fd);
    int malloc_size = strlen(message) + strlen(chat_uuid) + 1;
    char* command = calloc(malloc_size, sizeof(char));
    sprintf(command, "%s=%s", chat_uuid, message);

    write(fd, command, malloc_size);

    free(command);
    close(fd);
}

void set_debug() {
    char* command = "loglevel_normal";
    int fd = open("/var/run/hassesfifo", O_WRONLY);
    write(fd, command, strlen(command));
    close(fd);
}