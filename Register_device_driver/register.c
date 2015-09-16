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
#include <linux/interrupt.h> /*interrupt*/
#include <linux/poll.h>
#include <linux/sched.h>

#define BUFF_SIZE 16

// Memory Mapped Register
// C_BASEADDR + 0x0
// C_BASEADDR + 0x4
// C_BASEADDR + 0x8
// C_BASEADDR + 0xC

#define REG_STATUS_OFFSET 0x0
#define REG_MIC_1_OFFSET 0x4
#define REG_MIC_2_OFFSET 0x8
#define REG_MIC_3_OFFSET 0xC

int reg_open(struct inode *inode, struct file *filep);
int reg_release(struct inode *inode, struct file *filep);
static ssize_t reg_read(struct file *filep, char *buf, size_t count, loff_t *f_pos);
static ssize_t reg_write(struct file *filep, const char *buf, size_t count, loff_t *f_pos);
unsigned int reg_poll(struct file *filp, poll_table *wait);

//Global

// Felul fogjuk irni
int registers_major = 200;
int reg[4];
char *input_buffer;
void *regs;
// platform_get_resource visszateresi ertekeneinek
struct resource *resource_mem;
struct resource *resource_irq;
unsigned long remap_size;
int reg_ready;

#define CLASS_NAME "FPGA_registers"
static struct class* regs_class;

#define STATUS_REG_NAME "Mic_status_reg"
#define MIC_1_REG_NAME "Mic_1_reg"
#define MIC_2_REG_NAME "Mic_2_reg"
#define MIC_3_REG_NAME "Mic_3_reg"

struct file_operations reg_fops = {
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
  printk(KERN_INFO "poll: called\n");
  poll_wait( filp, &wq, wait );
  if (reg_ready) {
    printk(KERN_INFO "poll: ready\n");
  	mask |= ( POLLIN | POLLRDNORM );
  }
  printk(KERN_INFO "poll: end\n");
  return mask;
}

int reg_open(struct inode *inode, struct file *filep)
{
  return 0;
}


int reg_release(struct inode *inode, struct file *filep)
{
  return 0;
}


static ssize_t reg_read(struct file *filep, char *buf, size_t count, loff_t *f_pos)
{

  unsigned int reg_value;
	
  // Magat a regisztert csak egyszer akarjuk kiolvasni egy fajlmuvelet alatt,
  // igy a msodik korben mar koran visszaterunk, ay olvasas elott
  // poll eseten nem akarjuk, hogy 0-val terjunk vissza, mert akkor lezarul az
  // olvasas. Ezert ha reg_ready van, azaz az interrupt miatt van olvasas,
  // akkor nem terunk vissza
  if (*f_pos != 0 && !reg_ready) {
    return 0;
  }
  
	// minor 0: status
	// minor 1: mic 1
	// minor 2: mic 2
	// minor 3: mic 3
	switch(MINOR(filep->f_dentry->d_inode->i_rdev)){
		case 0:
	  	//reg_value = ioread32(regs + REG_STATUS_OFFSET);
      //TEMP
      reg_value = reg[0];
			break;
		case 1:
	  	reg_value = ioread32(regs + REG_MIC_1_OFFSET);
			break;
		case 2:
	  	reg_value = ioread32(regs + REG_MIC_2_OFFSET);
			break;
		case 3:
	  	reg_value = ioread32(regs + REG_MIC_3_OFFSET);
			break;
		default:
			return -EINVAL;
	}

 
  printk(KERN_INFO "read %x\n", reg_value);

  copy_to_user(buf, &reg_value, sizeof(reg_value));

  reg_ready = 0;
  *f_pos += sizeof(reg_value);
  return sizeof(reg_value);

}


static ssize_t reg_write(struct file *filep, const char *buf, size_t count, loff_t *f_pos)
{
  int c;
  u32 value;
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
  value = *(u32*)buf;
  printk(KERN_INFO "write: try to write %x\n", value);

	switch(MINOR(filep->f_dentry->d_inode->i_rdev)){
		case 0:
	  	iowrite32(value, regs + REG_STATUS_OFFSET);
			break;
		case 1:
	  	iowrite32(value, regs + REG_MIC_1_OFFSET);
			break;
		case 2:
	  	iowrite32(value, regs + REG_MIC_2_OFFSET);
			break;
		case 3:
	  	iowrite32(value, regs + REG_MIC_3_OFFSET);
			break;
		default:
			return -EINVAL;
	}
  
  
  return c;
}

static int myregister_remove(struct platform_device *pdev)
{

  device_destroy(regs_class, MKDEV(registers_major, 0));
  device_destroy(regs_class, MKDEV(registers_major, 1));
  device_destroy(regs_class, MKDEV(registers_major, 2));
  device_destroy(regs_class, MKDEV(registers_major, 3));
  class_unregister(regs_class);
  class_destroy(regs_class);
  /* Freeing the major number */
  unregister_chrdev(registers_major, "my_FPGA_registers_device");

  /* Freeing buffer memory */
  if (input_buffer) {
    kfree(input_buffer);
  }

  iounmap(regs);
  release_mem_region(resource_mem->start, remap_size);
  free_irq(resource_irq->start,pdev);
  printk(KERN_INFO "Removing myreg modul\n");

  return 0;
}
static irqreturn_t myregister_irq_handler(int irq, void *dev_id)
{
	reg[0] = ioread32(regs + REG_STATUS_OFFSET);
	printk(KERN_INFO "interrupt: read %x\n", reg[0]);
  reg_ready = 1;
  wake_up(&wq);
  printk(KERN_INFO "interrupt: after wake_up \n");
	return IRQ_HANDLED;
}

static int myregister_probe(struct platform_device *pdev)
{
  int result;
  struct device* err;

  reg_ready = 0;
  printk(KERN_INFO "Probe start\n");

//  match = of_match_device(myregister_match, &op->dev);

//  if (!match)
//    return -EINVAL;

  // Regisztraljuk az eszkozvezerlot
  // dinamukis majorszamot kerunk
  result = register_chrdev(0, "my_FPGA_registers_device", &reg_fops);
  if (result < 0) {
    printk(KERN_ERR "cannot register chrdev: %d\n", result);
    goto fail_reg;
  }

  registers_major = result;
  /* Az udev számára jelzés, hogy hozza létre az eszközállományt. */
  regs_class = class_create(THIS_MODULE, CLASS_NAME);
  err = device_create(regs_class, NULL, MKDEV(registers_major, 0), NULL, STATUS_REG_NAME);
  err = device_create(regs_class, NULL, MKDEV(registers_major, 1), NULL, MIC_1_REG_NAME);
  err = device_create(regs_class, NULL, MKDEV(registers_major, 2), NULL, MIC_2_REG_NAME );
  err = device_create(regs_class, NULL, MKDEV(registers_major, 3), NULL, MIC_3_REG_NAME );

  // input buffernek helyet foglalunk
  input_buffer = kmalloc(BUFF_SIZE, GFP_KERNEL); 
  if (!input_buffer) { 
    result = -ENOMEM;
    goto fail_mem; 
  } 
  memset(input_buffer, 0, BUFF_SIZE);

  // lekerdezzuk a periferiahoz tartozo informaciokat
  // masodik parameter az ioport.h-ban talalhato
  resource_mem = platform_get_resource(pdev, IORESOURCE_MEM, 0);
  // kiszamoljuk a mereteket
  printk(KERN_INFO "Addresses: start: %x end: %x\n",resource_mem->start, resource_mem->end);
  remap_size = resource_mem->end - resource_mem->start + 1;

  resource_irq = platform_get_resource(pdev, IORESOURCE_IRQ, 0);
  printk(KERN_INFO "IRQ: start: %x end: %x\n",resource_irq->start, resource_irq->end);
  result = request_irq(resource_irq->start, myregister_irq_handler, 0, "my_FPGA_IRQ", pdev);
  if (result < 0) {
    printk(KERN_ERR "cannot request IRQ: %d\n", result);
    goto fail_irq;
  }
  
  if (NULL == request_mem_region(resource_mem->start, remap_size, "my_FPGA_register")) {
  printk(KERN_INFO "request mem region\n");
  //TODO hibakezelés mert ez nem elég itt
  // ezt jo helye void release_mem_region(unsigned long start, unsigned long len);
    goto fail_req;
  }  

  regs = ioremap(resource_mem->start, remap_size);
  if(!regs) {
     printk(KERN_ERR "ERROR ioremap\n");
  //TODO rendes hibakezelés
    goto fail_map;
  }
  printk(KERN_INFO "Inserting myreg module\n"); 
  return 0;

  fail_map:
  release_mem_region(resource_mem->start, remap_size);
  fail_req:
  free_irq(resource_irq->start,pdev);
  fail_irq:
  kfree(input_buffer);
  fail_mem:
  unregister_chrdev(registers_major, "my_FPGA_registers_device");
  fail_reg:
  return result;  
  
}

static const struct of_device_id myregister_match[] = {
        { .compatible = "my_FPGA_registers" },
        {},
};
MODULE_DEVICE_TABLE(of, myregister_match);

static struct platform_driver myregister_driver = {
        .probe = myregister_probe,
        .remove = myregister_remove,
        .driver = {
                .name = "FPGA_registers",
                .owner = THIS_MODULE,
                .of_match_table = myregister_match
        },
};

module_platform_driver(myregister_driver);

MODULE_LICENSE("GPL");
//module_init(myreg_init);
//module_exit(myreg_exit);
