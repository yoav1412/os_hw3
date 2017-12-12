//
// Created by yoav on 12/7/17.
//

#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <errno.h>

#define FAILURE -1
#define SUCCESS 0
#define MSG_SLOT_CHANNEL_CMND 1


int main(int argc, char** argv){
	if (argc != 4){
	    	printf("Writer status: failed (invalid arguments).\n");
	    	return FAILURE;
	}
    char* message_slot_file_path = argv[1];
    int channel_id = atoi(argv[2]);
    char message[128];
    strcpy(message, argv[3]);
    int retVal;

    int fd = open(message_slot_file_path, O_RDWR);
    if (fd < 0) {
        printf("Writer status: failed.\n");
        return FAILURE;
    }
    //set chanel:
    if ( ioctl(fd, MSG_SLOT_CHANNEL_CMND, channel_id) < 0 ){
        printf("Writer status: failed.\n");
        return FAILURE;
    }
    if ( write(fd, message, strlen(message) * sizeof(char)+1) < 0){
    	printf("Writer status: failed.\n");
    	return FAILURE;
    }
    if (close(fd) == -1){
    	printf("Sender status: failed\n");
    	return FAILURE;
    }
    printf("Sender status: success\n");
    return SUCCESS;
}
