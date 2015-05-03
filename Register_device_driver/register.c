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
#include <linux/io.h>

#define BUFF_SIZE 16

int reg_open(struct inode *inode, struct file *filp);
int reg_release(struct inode *inode, struct file *filp);
static ssize_t reg_read(struct file *filp, char *buf, size_t count, loff_t *f_pos);
static ssize_t reg_write(struct file *filp, const char *buf, size_t count, loff_t *f_pos);

//Global

int testreg_major = 200;
int reg[1];
char *input_buffer;
void *regs;
struct resource *resource; // platform_get_resource visszateresi ertekenek
unsigned long remap_size;


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

  unsigned int reg_value;
  /* Transfering data to user space */
  input_buffer[BUFF_SIZE -1] = '\0';
  //copy_to_user(buf, input_buffer, BUFF_SIZE);
  //printk("<1> read %s\n", input_buffer);

  //TODO ioread, igazitani a regisztermerethez
  reg_value = ioread8(regs);
 
  printk(KERN_INFO "read %u\n", reg_value);

  copy_to_user(buf, &reg_value, sizeof(reg_value));
  /* Changing reading position as best suits */
  if (*f_pos == 0) {
    *f_pos += sizeof(reg_value);
    return sizeof(reg_value);
  } else {
    return 0;
  }
  //return sizeof(reg_value);
}


static ssize_t reg_write(struct file *filp, const char *buf, size_t count, loff_t *f_pos)
{
  int c;
  u8 value;
  //tmp = buf + count - 1;

  if (count > BUFF_SIZE - 1) {
    c = BUFF_SIZE - 1;
  }
  else {
    c = count;
  }

  copy_from_user(input_buffer, buf, c);
  //f_pos += c;
  //printk("write %s\n", input_buffer);

  //TODO iowrite
  value = (u8)buf[0];
  printk(KERN_INFO "write: try to write %c\n", value);
  iowrite8(value, regs);
  
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
  release_mem_region(resource->start, remap_size);
  printk(KERN_INFO "Removing myreg modul\n");

  return 0;
}

static int myregister_probe(struct platform_device *pdev)
{
  int result;
 
  printk(KERN_INFO "Probe start\n");

//  match = of_match_device(myregister_match, &op->dev);

//  if (!match)
//    return -EINVAL;

  // Regisztraljuk az eszkozvezerlot
  result = register_chrdev(testreg_major, "my_FPGA_register", &testreg_fops);
  if (result < 0) {
    printk(KERN_ERR "cannot obtain major number %d result: %d\n", testreg_major,result);
    goto fail_reg;
  }

  // input buffernek helyet foglalunk
  input_buffer = kmalloc(BUFF_SIZE, GFP_KERNEL); 
  if (!input_buffer) { 
    result = -ENOMEM;
    goto fail_mem; 
  } 
  memset(input_buffer, 0, BUFF_SIZE);

  // lekerdezzuk a periferiahoz tartozo informaciokat
  resource = platform_get_resource(pdev, IORESOURCE_MEM, 0);
  // kiszamoljuk a mereteket
  printk(KERN_INFO "Addresses: start: %x end: %x\n",resource->start, resource->end);
  remap_size = resource->end - resource->start + 1;

  if (NULL == request_mem_region(resource->start, remap_size, "my_FPGA_register")) {
  printk(KERN_INFO "request mem region\n");
  //TODO hibakezelés mert ez nem elég itt
  // ezt jo helye void release_mem_region(unsigned long start, unsigned long len);
    goto fail_req;
  }  

  regs = ioremap(resource->start, remap_size);
  if(!regs) {
     printk(KERN_ERR "ERROR ioremap\n");
  //TODO rendes hibakezelés
    goto fail_map;
  }
  printk(KERN_INFO "Inserting myreg module\n"); 
  return 0;

  fail_map:
  release_mem_region(resource->start, remap_size);
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
                .name = "myregister",
                .owner = THIS_MODULE,
                .of_match_table = myregister_match
        },
};

module_platform_driver(myregister_driver);

MODULE_LICENSE("GPL");
//module_init(myreg_init);
//module_exit(myreg_exit);
