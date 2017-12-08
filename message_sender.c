//
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

#define MSG_SLOT_CHANNEL_CMND 1 //just in order not to include kernel shit.. todo fix


int main(int argc, char** argv){
    //TODO: take arguments more carefully..
    char* message_slot_file_path = argv[1];
    int channel_id = atoi(argv[2]);
    char message[128];
    strcpy(message, argv[3]);
    int retVal;
    printf("Args at msgsndr: message_slot_file_path=%s,channel_id=%d,message=%s\n", message_slot_file_path, channel_id, message);
    int fd = open(message_slot_file_path, O_RDWR);
    printf("sender: finished opening. fd = %d\n", fd);
    if (fd < 0) {
        printf("Can't open device file :%s", message_slot_file_path);
        return FAILURE;
    }

    //set chanel:
    retVal = ioctl(fd, MSG_SLOT_CHANNEL_CMND, channel_id);
    printf("sender: aFter IOCTL. retVal = %d\n", retVal);//todo rm

    retVal = write(fd, message, strlen(message) * sizeof(char));
    printf("sender: after write. retVal = %d\n", retVal);//todo rm

    return SUCCESS;
}