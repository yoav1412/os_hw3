//
// Created by yoav on 12/7/17.
//

#include "message_slot.h"

// device major number:
static int majorNmber = MAJOR_NUM;
MessageSlot *messageSlots = NULL;


static int device_open( struct inode* inode, struct file*  file ){
    int file_minor = iminor(inode);
    if (messageSlots == NULL){
        if ( (messageSlots = (MessageSlot*) kmalloc(sizeof(MessageSlot), GFP_KERNEL)) == NULL ) {
            return -ENOMEM;
        };
        messageSlots->minor = file_minor;
        messageSlots->next = NULL;
        /*messageSlots->messages->next = NULL;*/
        messageSlots->messages = NULL;
        return SUCCESS;
    }
    MessageSlot* listBasePointer = messageSlots;
    MessageSlot* prv;
    while (messageSlots != NULL) {
        if (messageSlots->minor == file_minor){ //i.e, a MsgSlot already exists for this minor.
        	messageSlots = listBasePointer;
        	return SUCCESS;
        }
        prv = messageSlots;
        messageSlots = messageSlots->next;
    }
    //Get here if a messageSlot was not allocated for this minor yet:
    if ( (prv->next = (MessageSlot*) kmalloc(sizeof(MessageSlot), GFP_KERNEL)) == NULL ) {
    	return -ENOMEM;
    };
    prv->next->minor = file_minor;
    prv->next->next = NULL;
    prv->next->messages = NULL;
    messageSlots = listBasePointer;
    return SUCCESS;
}

static ssize_t device_read(struct file* file, char __user* buffer, size_t length, loff_t* offset){
    int channel_to_read = (int) file->private_data;
    int file_minor = iminor(file->f_inode);
    if ( length <= 0 || length > MAX_MSG_LEN ) { return -EINVAL; }
    MessageSlot *curr = messageSlots;
    while (curr != NULL && (curr->minor != file_minor) ){
        curr = curr->next;
    }
    if (curr == NULL) {
    	// Error, trying to read form file that has no msgSlot:
        return -EINVAL;
    }
    Message *messages_on_file = curr->messages;
    if (messages_on_file == NULL) {
        //Error, channel was never set:
        return -EINVAL;
        }
    Message *prv;
    while ((messages_on_file!=NULL) && (messages_on_file->channel != channel_to_read)) {
        prv = messages_on_file;
    	messages_on_file = messages_on_file->next;
    }
    if (messages_on_file == NULL) {
    	//Error, trying to read from channel that wasn't written to (no message exists on channel):
    	return -EWOULDBLOCK;
    }
    // Here, messages_on_file points to correct message-channel in correct file:

    if (length < strlen(messages_on_file->content)) {//provided buff is too small to hold message.
    	return -ENOSPC;
    }
    if ( copy_to_user(buffer, messages_on_file->content, strlen(messages_on_file->content) + 1) != 0 ) { return FAILURE; }
    return length; //return num of bytes read;
}


static ssize_t device_write(struct file* file, const char __user* buffer, size_t length, loff_t* offset){
    int channel_to_write = (int) file->private_data;
    int file_minor = iminor(file->f_inode);
    if ( length <= 0 || length > MAX_MSG_LEN ) { return -EINVAL; }
    MessageSlot *curr = messageSlots;
    while (curr != NULL && (curr->minor != file_minor) ){
        curr = curr->next;
    }
    if (curr == NULL) {
    	//Error, tried to write to a file that wasn't opened:
        return -EINVAL;
    }

    Message *messages_on_file = curr->messages;
    Message *prv = NULL;
    while ((messages_on_file!=NULL) && (messages_on_file->channel != channel_to_write)) {
        prv = messages_on_file;
    	messages_on_file = messages_on_file->next;
    }

    if (messages_on_file == NULL) { //i.e, no message has been written on this channel in this file yet (so alloc one):
    	if ( (messages_on_file = (Message*) kmalloc(sizeof(Message), GFP_KERNEL)) == NULL ) {
    		return -ENOMEM;
    	};
    	messages_on_file->next = NULL;
    	messages_on_file->channel = channel_to_write;
    	if (prv != NULL){
    	//i.e, there were other channels existing in file before current write, need to link them to newly allocated channel:
    		prv->next = messages_on_file;
    	} else {
    		//got here -> the channel we are writing to is the first channel of this file.
    		curr->messages = messages_on_file;
    	}
    }
    // Here, messages_on_file points to correct message-channel in correct file:
    if ( copy_from_user(messages_on_file->content, buffer, length) != 0 ) { return FAILURE; }
	return length; //return num of bytes written;
}

static long device_ioctl( struct   file* file,
                          unsigned int   ioctl_command,
                          unsigned long  ioctl_param )
{
    if( ioctl_command == MSG_SLOT_CHANNEL_CMND){
        // Get the parameter given to ioctl by the process
        file->private_data = (void *) ioctl_param;
        return SUCCESS;
    }
    return -EINVAL;
}

static int device_release( struct inode* inode, struct file*  file){
	return SUCCESS;
}

struct file_operations Fops =
        {
                .read           = device_read,
                .write          = device_write,
                .open           = device_open,
                .release        = device_release,
                .unlocked_ioctl = device_ioctl,
        };





static int __init myinit(void){
    int rv = register_chrdev( majorNmber, DEVICE_FILE_NAME, &Fops );
    if(rv < 0)
    {
        printk( KERN_ALERT "%s registration failed for  %d\n", DEVICE_FILE_NAME, majorNmber);
        return majorNmber;
    }
    printk(KERN_INFO "message_slot: registered major number %d\n", majorNmber);
    return 0;
}

static void __exit module_cleanup(void){
	Message *messages, *msg_to_free;
	MessageSlot *slot_to_free;
	while (messageSlots != NULL){
		messages = messageSlots->messages;
		while (messages != NULL){
			msg_to_free = messages;
			messages = messages->next;
			kfree(msg_to_free);
		}
		slot_to_free = messageSlots;
		messageSlots = messageSlots->next;
		kfree(slot_to_free);
	}
    unregister_chrdev(majorNmber, DEVICE_FILE_NAME);
}


module_init(myinit);
module_exit(module_cleanup);
