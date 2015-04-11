/* Kernel Programming */
#define LINUX

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
#include <linux/fcntl.h> /* O_ACCMODE */
#include <asm/uaccess.h> /* copy_from/to_user */
#include <linux/platform_device.h>
#include <asm/io.h>

#define BUFF_SIZE 16
//TODO ne beégetett címekkel
#define MYREG_START 0x75c0000
#define MYREG_LEN 0x10000

int reg_open(struct inode *inode, struct file *filp);
int reg_release(struct inode *inode, struct file *filp);
static ssize_t reg_read(struct file *filp, char *buf, size_t count, loff_t *f_pos);
static ssize_t reg_write(struct file *filp, const char *buf, size_t count, loff_t *f_pos);

//Global

int testreg_major = 200;
int reg[1];
char *input_buffer;
void *regs;

struct file_operations testreg_fops = {
  read: reg_read,
  write: reg_write,
  open: reg_open,
  release: reg_release
};

int reg_open(struct inode *inode, struct file *filp)
{
  return 0;
}


int reg_release(struct inode *inode, struct file *filp)
{
  return 0;
}


static ssize_t reg_read(struct file *filp, char *buf, size_t count, loff_t *f_pos)
{

  /* Transfering data to user space */
  input_buffer[BUFF_SIZE -1] = '\0';
  copy_to_user(buf, input_buffer, BUFF_SIZE);
  printk("<1> read %s\n", input_buffer);

	//TODO ioread 
 
  /* Changing reading position as best suits */
  if (*f_pos == 0) {
    *f_pos += BUFF_SIZE;
    return BUFF_SIZE;
  } else {
    return 0;
  }
}


static ssize_t reg_write(struct file *filp, const char *buf, size_t count, loff_t *f_pos)
{
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
  printk("write %s\n", input_buffer);

	//TODO iowrite

  return c;
}

static int myregister_remove(struct platform_device *pdev)
{
  /* Freeing the major number */
  unregister_chrdev(testreg_major, "myreg");

  /* Freeing buffer memory */
  if (input_buffer) {
    kfree(input_buffer);
  }

	iounmap(regs);
	release_mem_region(MYREG_START, MYREG_LEN);
  printk(KERN_ALERT "Removing myreg modul\n");

  return 0;
}

static int myregister_probe(struct platform_device *pdev)
{
  int result;
  
  printk("<1>Probe start\n");

//  match = of_match_device(myregister_match, &op->dev);

//  if (!match)
//    return -EINVAL;

  /* Registering device */
  result = register_chrdev(testreg_major, "myreg", &testreg_fops);
  if (result < 0) {
    printk("<1>testreg: cannot obtain major number %d result: %d\n", testreg_major,result);
    goto fail_reg;
  }

  /* Allocating memory for the buffer */
  input_buffer = kmalloc(BUFF_SIZE, GFP_KERNEL); 
  if (!input_buffer) { 
    result = -ENOMEM;
    goto fail_mem; 
  } 
  memset(input_buffer, 0, BUFF_SIZE);

	//TODO ne beégetett címmel/hosszal
	if(!(check_mem_region(MYREG_START, MYREG_LEN))) {
		printk("<1>ERROR check mem region\n");
		goto fail_req;
	}

  //TODO ne beégetett címmel/hosszal
  if (NULL == request_mem_region(MYREG_START, MYREG_LEN, "myreg_mem")) {
  printk("<1>ERROR request mem region\n");
  //TODO hibakezelés mert ez nem elég itt
  // ezt jo helye void release_mem_region(unsigned long start, unsigned long len);
    goto fail_req;
  }  

  regs = ioremap(MYREG_START, MYREG_LEN);
  if(!regs) {
     printk("<1>ERROR ioremap\n");
  //TODO rendes hibakezelés
    goto fail_map;
  }
  printk("<1>Inserting myreg module\n"); 
  return 0;

	fail_map:
	release_mem_region(MYREG_START, MYREG_LEN);
  fail_req: 
	kfree(input_buffer);
	fail_mem:
	unregister_chrdev(testreg_major, "myreg");
	fail_reg:
  return result;	
  
}

static const struct of_device_id myregister_match[] = {
        { .compatible = "test_register_1.00.a" },
        {},
};
MODULE_DEVICE_TABLE(of, myregister_match);

static struct platform_driver myregister_driver = {
        .probe = myregister_probe,
        .remove = myregister_remove,
        .driver = {
                .name           = "myregister",
		.owner		= THIS_MODULE,
                .of_match_table = myregister_match
        },
};

module_platform_driver(myregister_driver);

MODULE_LICENSE("GPL");
//module_init(myreg_init);
//module_exit(myreg_exit);
