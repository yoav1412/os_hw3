//
// Created by yoav on 12/7/17.
//

#ifndef OS_HW3_MESSAGE_SLOT_H
#define OS_HW3_MESSAGE_SLOT_H

#define __KERNEL__
#define MODULE
#define MAX_MSG_LEN 128

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/ioctl.h>
#include <linux/fs.h>
#include <asm/uaccess.h>
#include <linux/string.h>
#include <linux/slab.h>
#define MAJOR_NUM 244
#define DEVICE_RANGE_NAME "mslot"
#define DEVICE_FILE_NAME "device_file"
#define SUCCESS 0
#define FAILURE -1
#define MSG_SLOT_CHANNEL_CMND 1

typedef struct message{
    char content[MAX_MSG_LEN];
    int channel;
    struct message *next;
} Message;

typedef struct message_slot
{
    struct message_slot *next;
    int minor; //ID of this message slot
    Message *messages;
}MessageSlot;



#endif //OS_HW3_MESSAGE_SLOT_H
