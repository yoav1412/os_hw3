
// Created by yoav on 12/7/17.
//

#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <unistd.h>

#define FAILURE -1
#define SUCCESS 0
#define MSG_SLOT_CHANNEL_CMND 1
#define MAX_MSG_LEN 128

int main(int argc, char** argv){
    if (argc != 3){
    	printf("Reader status: failed (invalid arguments).\n");
    	return FAILURE;
    }
	char* message_slot_file_path = argv[1];
    int channel_id = atoi(argv[2]);
    int fd = open(message_slot_file_path, O_RDWR);
    if (fd < 0) {
    	printf("Reader status: failed.\n");
    	return FAILURE;
    }
    char buffer[MAX_MSG_LEN];

    if (fd < 0) {
    	printf("Reader status: failed.\n");
        return FAILURE;
    }
    //set channel:
    if (ioctl(fd, MSG_SLOT_CHANNEL_CMND, channel_id) < 0){
    	printf("Reader status: failed.\n");
    	return FAILURE;
    }

    if (read(fd, buffer, MAX_MSG_LEN) == -1){
    	printf("Reader status: failed.\n");
    	return FAILURE;
    }
    if (close(fd) == -1){
        	printf("Reader status: failed\n");
        	return FAILURE;
    }

    printf("Reader status: success.\n Read message: %s\n", buffer);
    return SUCCESS;

}
