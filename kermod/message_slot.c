//
// Created by yoav on 12/7/17.
//

#include "message_slot.h"

// device major number:
static int majorNmber = MAJOR_NUM; //TODO change
MessageSlot *messageSlots = NULL;

static int device_open( struct inode* inode,
                        struct file*  file )
{
    printk("Invoking device_open(%p)\n", file); //todo rm

    int file_minor = iminor(inode);
    printk("device_open: fileminor = %d\n", file_minor);
    if (messageSlots == NULL){
        if ( (messageSlots = (MessageSlot*) kmalloc(sizeof(MessageSlot), GFP_KERNEL)) == NULL ) {
            printk("device_open : RET1\n"); //todo rm
            printk("device_open : mem allocation error.\n");
            return FAILURE;
        };
        messageSlots->minor = file_minor;
        if ( (messageSlots->messages = (Message*) kmalloc(sizeof(Message), GFP_KERNEL)) == NULL ) {
            printk("device_open : mem allocation error.\n");
            return FAILURE;
        };
        printk("device_open : RET2\n");//todo rm
        return SUCCESS;
    }


    MessageSlot* listBasePointer = messageSlots;
    MessageSlot* prv;
    while (messageSlots != NULL) {
        if (messageSlots->minor == file_minor){ //i.e, a MsgSlot already exists for this minor.
            printk("device_open : RET3\n");//todo rm
            return SUCCESS;
        }
        prv = messageSlots;
        messageSlots = messageSlots->next;
    }
    if ( (prv->next = (MessageSlot*) kmalloc(sizeof(MessageSlot), GFP_KERNEL)) == NULL ) {
        printk("device_open : RET4\n");//todo rm
        printk("device_open : mem allocation error.\n");
        return FAILURE;
    };
    prv->next->minor = file_minor;
    if ( (prv->next->messages = (Message*) kmalloc(sizeof(Message), GFP_KERNEL)) == NULL ) {
        printk("device_open : mem allocation error.\n");
        return FAILURE;
    };


    messageSlots = listBasePointer;
    printk("device_open : SUCCESS\n");
    return SUCCESS;
}


static ssize_t device_read(struct filr* filr, char__user* buffer, size_t length, loff_t* offset){
    printk("Invoking device_read(doing nothing) \n");
    return -1;
}


static ssize_t device_wrtie(struct file* file, const char__user* buffer, size_t length, loff_t* offset){
    int channel_to_write = (int) file->private_data;
    int file_minor = iminor(file->f_inode);
    printk("Invoking device_write, file_minor = %d, channel_to_write = %d\n", file_minor, channel_to_write);
    printk("DEBUG:: in devie_write: messageSlots->minor=%d\n", messageSlots->minor);
    MessageSlot *curr = messageSlots;
    printk("DEBUG:: in devie_write (before 1st loop): curr->next=%p | curr->minor=%d\n", curr->next, curr->minor);
    while (curr != NULL && (curr->minor != file_minor) ){
        printk("device_write: entered loop \n");
        curr = curr->next;
        printk("device_write: curr = %p \n", curr);
    }
    if (curr == NULL) {
        printk("Error in device_write: no such file created\n");
        return FAILURE;
    }

    Message *messages_on_file = curr->messages;
    while ((messages_on_file->channel != channel_to_write) && (messages_on_file!=NULL)) {
        messages_on_file = messages_on_file->next;
    }
    if (messages_on_file==NULL) {
        printk("Error in device_write.\n");
        return FAILURE;
    }
    if (strcpy(messages_on_file->content, buffer) == NULL){
        printk("Error in device_write.\n");
        return FAILURE;
    }
    return SUCCESS;
}

static long device_ioctl( struct   file* file,
                          unsigned int   ioctl_command,
                          unsigned long  ioctl_param )
{
    printk("in IOCTL.cmnd = %d, ioctl_param = %d\n",ioctl_command,ioctl_param);
    // Switch according to the ioctl called
    if( ioctl_command == MSG_SLOT_CHANNEL_CMND){
        // Get the parameter given to ioctl by the process
        printk("ioctl: recognized cmnd. param = %d\n",ioctl_param);
        file->private_data = (void *) ioctl_param;
        printk("ioctl: perfomred channek switch.\n");
    }
    return SUCCESS;
}


struct file_operations Fops =
        {
                .read           = device_read,
                .write          = device_write,
                .open           = device_open,
                //.release        = device_release,
                .unlocked_ioctl = device_ioctl,
        };





static int __init myinit(void){

    // Register driver capabilities. Obtain major num
    int rv = register_chrdev( majorNmber, DEVICE_RANGE_NAME, &Fops );

    // Negative values signify an error
    if(rv < 0)
    {
        printk( KERN_ALERT "%s registraion failed for  %d\n", DEVICE_FILE_NAME, majorNmber);
        return majorNmber;
    }

    printk("\nSuccessfully initialized module. Major = %d\n", majorNmber); //todo rm

    return 0;
}

static void __exit module_cleanup(){
    unregister_chrdev(majorNmber, DEVICE_FILE_NAME);
}


module_init(myinit);
module_exit(module_cleanup);