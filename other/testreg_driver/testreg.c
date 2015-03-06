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


#define BUFF_SIZE 16
int reg_open(struct inode *inode, struct file *filp);
int reg_release(struct inode *inode, struct file *filp);
ssize_t reg_read(struct file *filp, char *buf, size_t count, loff_t *f_pos);
ssize_t reg_write(struct file *filp, char *buf, size_t count, loff_t *f_pos);
void testreg_exit(void);
int testreg_init(void);


//Global

int testreg_major = 200;
int reg[1];
char *input_buffer;

struct file_operations testreg_fops = {
  read: reg_read,
  write: reg_write,
  open: reg_open,
  release: reg_release
};


/*
static struct of_device_id testreg_of_match[] __devinitdata = {
  { .compatible = "test_register_1.00.a", },
  {}
};

static struct platform_driver testreg_platform_driver = {
    .probe = testreg_drv_probe,
    .remove = testreg_drv_remove,
    .driver = {
    .name = "testreg",
    .owner = THIS_MODULE,
    .of_match_table = testreg_of_match,
    },
};
*/

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
  copy_to_user(buf,input_buffer,BUFF_SIZE);

  /* Changing reading position as best suits */ 
  if (*f_pos == 0) { 
    *f_pos+=1; 
    return 1; 
  } else { 
    return 0; 
  }
}


ssize_t reg_write(struct file *filp, char *buf, size_t count, loff_t *f_pos)
{
  char *tmp;

  tmp = buf + count - 1;
  copy_from_user(input_buffer,tmp,1);
  return 1;
}


void testreg_exit(void)
{
  /* Freeing the major number */
  unregister_chrdev(testreg_major, "testreg");

  /* Freeing buffer memory */
  if (input_buffer) {
    kfree(input_buffer);
  }
  printk(KERN_ALERT "Removing testreg modul\n");

}

int testreg_init(void)
{
  int result;

  /* Registering device */
  result = register_chrdev(testreg_major, "testreg", &testreg_fops);
  if (result < 0) {
    printk("<1>testreg: cannot obtain major number %d result: %d\n", testreg_major,result);
    return result;
  }

  /* Allocating memory for the buffer */
  input_buffer = kmalloc(BUFF_SIZE, GFP_KERNEL); 
  if (!input_buffer) { 
    result = -ENOMEM;
    goto fail; 
  } 
  memset(input_buffer, 0, BUFF_SIZE);

  printk("<1>Inserting testreg module\n"); 
  return 0;

  fail: 
  testreg_exit(); 
  return result;	
  
}

MODULE_LICENSE("GPL");
//MODULE_DEVICE_TABLE(of, testreg_of_match);
module_init(testreg_init);
module_exit(testreg_exit);
