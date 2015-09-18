/* Kernel Programming */
#define MODULE
#define LINUX
#define __KERNEL__

#include <linux/module.h>  /* Needed by all modules */
#include <linux/kernel.h>  /* Needed for KERN_ALERT */
#include <linux/init.h>
//#include <linux/config.h>
#include <linux/module.h>
#include <linux/kernel.h> /* printk() */
#include <linux/slab.h> /* kmalloc() */
#include <linux/fs.h> /* everything... */
#include <linux/errno.h> /* error codes */
#include <linux/types.h> /* size_t */
#include <linux/proc_fs.h>
#include <linux/fcntl.h> /* O_ACCMODE */
#include <asm/uaccess.h> /* copy_from/to_user */


#include <linux/poll.h>
#include <linux/device.h>
#include <linux/sched.h>

#define BUFF_SIZE 16
int reg_open(struct inode *inode, struct file *filp);
int reg_release(struct inode *inode, struct file *filp);
ssize_t reg_read(struct file *filp, char *buf, size_t count, loff_t *f_pos);
ssize_t reg_write(struct file *filp, char *buf, size_t count, loff_t *f_pos);
unsigned int reg_poll(struct file *filp, poll_table *wait);
void testreg_exit(void);
int testreg_init(void);


//Global

#define DEVICE_NAME "polltest"
#define CLASS_NAME "polltestclass"
static struct class* test_class;

int testreg_major = 200;
int reg[1];
char *input_buffer;
char module_name[] = "polltest";
int ready = 0;

struct file_operations testreg_fops = {
  read: reg_read,
  write: reg_write,
  open: reg_open,
  release: reg_release,
  poll: reg_poll
};

DECLARE_WAIT_QUEUE_HEAD(wq);

unsigned int reg_poll(struct file *filp, poll_table *wait )
{
  unsigned int mask = 0;
  printk("<1> poll begin\n");
  poll_wait( filp, &wq, wait );
  if (ready){
        ready = 0;
  	mask |= ( POLLIN | POLLRDNORM );
        printk("<1> poll ready\n");
  }
  printk("<1> poll end\n");
  return mask;
}

int reg_open(struct inode *inode, struct file *filp)
{
  return 0;
}


int reg_release(struct inode *inode, struct file *filp)
{
  return 0;
}


ssize_t reg_read(struct file *filp, char *buf, size_t count, loff_t *f_pos)
{
  /* Transfering data to user space */
  input_buffer[BUFF_SIZE -1] = '\0'; 
  copy_to_user(buf, input_buffer, BUFF_SIZE);
  printk("<1> read %s\n", input_buffer);
  /* Changing reading position as best suits */ 
//  if (*f_pos == 0) { 
    *f_pos += BUFF_SIZE; 
    return BUFF_SIZE; 
//  } else { 
//    return 0; 
//  }
}


ssize_t reg_write(struct file *filp, char *buf, size_t count, loff_t *f_pos)
{
  char *tmp;
  int c;
  //tmp = buf + count - 1;

  if (count > BUFF_SIZE - 1) {
    c = BUFF_SIZE - 1;
  }
  else {
    c = count;
  }
  
  copy_from_user(input_buffer, buf, c);
  //f_pos += c;
  ready = 1;
  wake_up(&wq);
  printk("write %s\n", input_buffer);
  return c;

}


void testreg_exit(void)
{
  /* Freeing buffer memory */
  if (input_buffer) {
    kfree(input_buffer);
  }

  device_destroy(test_class, MKDEV(testreg_major, 0));
  class_unregister(test_class);
  class_destroy(test_class);

  /* Freeing the major number */
  unregister_chrdev(testreg_major, module_name);  
  printk(KERN_ALERT "Removing testreg modul\n");

}

int testreg_init(void)
{
  int result;
  int mode = 0644;
	struct device* err;
  // need a char device
  mode |= S_IFCHR;


//  testreg_major = register_chrdev_region(60, 1, module_name);
//  if(testreg_major < 0) {
//    return -1;
//  }
//  printk("<1> registered chrdev");

  /* Regisztraljuk a karakter tipusu eszkozt. */
  result = register_chrdev(0, DEVICE_NAME, &testreg_fops);
	if(result < 0)
  {
    printk(DEVICE_NAME " a major szam: %d nem elerheto\n",result);
    return testreg_major;
  }
	
	testreg_major = result;
  /* Az udev számára jelzés, hogy hozza létre az eszközállományt. */
  test_class = class_create(THIS_MODULE, CLASS_NAME);
  err = device_create(test_class, NULL, MKDEV(testreg_major, 0), NULL, DEVICE_NAME);

  /* Allocating memory for the buffer */
  input_buffer = kmalloc(BUFF_SIZE, GFP_KERNEL); 
  if (!input_buffer) { 
    result = -ENOMEM;
    goto fail; 
  } 
  memset(input_buffer, 0, BUFF_SIZE);

  printk(KERN_INFO "--- Inserting testreg module\n"); 
  return 0;

  fail: 
  testreg_exit(); 
  return result;	
  
}

MODULE_LICENSE("GPL");
//MODULE_DEVICE_TABLE(of, testreg_of_match);
module_init(testreg_init);
module_exit(testreg_exit);
