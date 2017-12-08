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
            return FAILURE;
        };
        return SUCCESS;
    }


    MessageSlot* listBasePointer = messageSlots;
    MessageSlot* prv;
    while (messageSlots != NULL) {
        messageSlots++;
        if (messageSlots->minor == file_minor){ //i.e, a MsgSlot already exists for this minor.
            return SUCCESS;
        }
        prv = messageSlots;
        messageSlots = messageSlots->next;
    }
    if ( (prv->next = (MessageSlot*) kmalloc(sizeof(MessageSlot), GFP_KERNEL)) == NULL ) {
        return FAILURE;
    };
    messageSlots = listBasePointer;
    return SUCCESS;
}

static ssize_t device_read( struct file* file, char __user* buffer, size_t length, loff_t* offset){

    printk("Invoking device_read(doing nothing) \n");
    return -1;
}


static ssize_t device_write( struct file* file, const char __user* buffer,size_t length, loff_t* offset){

    int channel_to_write = (int) file->private_data;
    int file_minor = iminor(file->f_inode);
    printk("Invoking device_write, file_minor = %d, currChannel = %d\n", file_minor, current_channel);


    MessageSlot *curr = messageSlots;
    while ((curr->minor != file_minor) && curr != NULL){
        curr = curr->next;
    }
    if (curr==NULL) {
        printk("Error in device_write: no such file created\n");
        return FAILURE;
    }

    Message *messages_on_file = curr->messages;
    while ((messages_on_file->channel != channel_to_write) && messages_on_file!=NULL) {
        messages_on_file = messages_on_file->next;
    }
    if (messages_on_file==NULL) {
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
        printk( KERN_ALERT "%s registraion failed for  %d\n", DEVICE_FILE_NAME, majorNmber );
        return majorNmber;
    }

    printk("\nSuccessfully initialized module. Major = %d\n", majorNmber); //todo rm

    return 0;
}

static void __exit module_cleanup(void)
{
    // Unregister the device:
    unregister_chrdev(majorNmber, DEVICE_FILE_NAME);
}




module_init(myinit);
module_exit(module_cleanup);