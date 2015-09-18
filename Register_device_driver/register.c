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

// 4 byte adatot tudunk irni
#define BUFF_SIZE 5

int reg_open(struct inode *inode, struct file *filep);
int reg_release(struct inode *inode, struct file *filep);
static ssize_t reg_read(struct file *filep, char *buf, size_t count, loff_t *f_pos);
static ssize_t reg_write(struct file *filep, const char *buf, size_t count, loff_t *f_pos);
unsigned int reg_poll(struct file *filp, poll_table *wait);

//Global

// Felul fogjuk irni
int registers_major = 200;
int reg[3] = {0,0,0};
char *input_buffer;
void *registers_addr[3];
// platform_get_resource visszateresi ertekeneinek
struct resource *resource_mem[3];
int IRQ[3] = {0,0,0};
unsigned long remap_size[3];
int reg_ready[3] = {0,0,0};

#define CLASS_NAME "FPGA_registers"
static struct class* regs_class;

// File-ok nevei
const char* file_names[] = { "Mic_1_reg", "Mic_2_reg", "Mic_3_reg" };
// Memoria regiok nevei
const char* mem_region_names[] = { "FPGA_MIC_1", "FPGA_MIC_2", "FPGA_MIC_3" };

const char* irq_names[] = { "my_FPGA_IRQ1", "my_FPGA_IRQ2", "my_FPGA_IRQ3" };

struct file_operations reg_fops = {
  read: reg_read,
  write: reg_write,
  open: reg_open,
  release: reg_release,
  poll: reg_poll
};

DECLARE_WAIT_QUEUE_HEAD(wq0);
DECLARE_WAIT_QUEUE_HEAD(wq1);
DECLARE_WAIT_QUEUE_HEAD(wq2);

unsigned int reg_poll(struct file *filep, poll_table *wait )
{
  unsigned int mask = 0;
  printk(KERN_INFO "poll: called\n");
  switch(MINOR(filep->f_dentry->d_inode->i_rdev)){
    case 0:
      poll_wait( filep, &wq0, wait );
      if (reg_ready[0]) {
        printk(KERN_INFO "poll 0: ready\n");
        mask |= ( POLLIN | POLLRDNORM );
      }
      break;
    case 1:
      poll_wait( filep, &wq1, wait );
      if (reg_ready[1]) {
        printk(KERN_INFO "poll 1: ready\n");
        mask |= ( POLLIN | POLLRDNORM );
      }
      break;
    case 2:
      poll_wait( filep, &wq2, wait );
      if (reg_ready[2]) {
        printk(KERN_INFO "poll 2: ready\n");
        mask |= ( POLLIN | POLLRDNORM );
      }
      break;
    default:
      return -EINVAL;
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
  int minor;

  minor = MINOR(filep->f_dentry->d_inode->i_rdev);
  // Magat a regisztert csak egyszer akarjuk kiolvasni egy fajlmuvelet alatt,
  // igy a msodik korben mar koran visszaterunk, ay olvasas elott
  // poll eseten nem akarjuk, hogy 0-val terjunk vissza, mert akkor lezarul az
  // olvasas. Ezert ha reg_ready van, azaz az interrupt miatt van olvasas,
  // akkor nem terunk vissza
  if (*f_pos != 0 && !reg_ready[minor]) {
    return 0;
  }

  // minor 1: mic 0
  // minor 2: mic 1
  // minor 3: mic 2
  reg_value = reg[minor];

  printk(KERN_INFO "read from address %x: %x\n", (unsigned int)registers_addr[minor], reg_value);
  copy_to_user(buf, &reg_value, sizeof(reg_value));

  reg_ready[minor] = 0;
  *f_pos += sizeof(reg_value);
  return sizeof(reg_value);
}


static ssize_t reg_write(struct file *filep, const char *buf, size_t count, loff_t *f_pos)
{
  int c;
  u32 value;
  int minor;

  minor = MINOR(filep->f_dentry->d_inode->i_rdev);

  // 32 bit (4 byte) adatot tudunk egyszerre irni
  // felkeszulunk arra, hogy egy stringet kapunk (+1)
  // mindenkepp az elso 4 byte lesz kiirva
  if (count > BUFF_SIZE) {
    c = BUFF_SIZE;
  }
  else {
    c = count;
  }

  copy_from_user(input_buffer, buf, c);
  value = *(u32*)buf;
  printk(KERN_INFO "write: try to write into reg %x: %x\n", *(unsigned int*)registers_addr[minor], value);

  iowrite32(value, registers_addr[minor]);

  return c;
}

static int myregister_remove(struct platform_device *pdev)
{
  int index;

  device_destroy(regs_class, MKDEV(registers_major, 0));
  device_destroy(regs_class, MKDEV(registers_major, 1));
  device_destroy(regs_class, MKDEV(registers_major, 2));
  class_unregister(regs_class);
  class_destroy(regs_class);
  /* Freeing the major number */
  unregister_chrdev(registers_major, "my_FPGA_registers_device");

  /* Freeing buffer memory */
  if (input_buffer) {
    kfree(input_buffer);
  }

  for (index = 0; index < 3; index++) {
    iounmap(registers_addr[index]);
    release_mem_region(resource_mem[index]->start, remap_size[index]);
    free_irq(IRQ[index], pdev);
  }
  printk(KERN_INFO "Removing myreg modul\n");

  return 0;
}

static irqreturn_t myregister_irq_handler(int irq, void *dev_id)
{
  int reg_index;
  printk(KERN_ERR "interrupt: called irq %d\n", irq);
  if (irq == IRQ[0]) {
    reg_index = 0;
    wake_up(&wq0);
  }
  else if (irq == IRQ[1]) {
    reg_index = 1;
    wake_up(&wq1);
  }
  else if (irq == IRQ[2]) {
    reg_index = 2;
    wake_up(&wq2);
  }
  else {
    printk(KERN_ERR "interrupt: invalid irq %d\n", irq);
    return IRQ_NONE;
  }
  printk(KERN_INFO "interrupt %d: after wake_up \n", irq);

  reg[reg_index] = ioread32(registers_addr[reg_index]);
  printk(KERN_INFO "interrupt %d: read %x index %d address %x\n", irq, reg[reg_index], reg_index, (unsigned int)registers_addr[reg_index]);
  reg_ready[reg_index] = 1;

  return IRQ_HANDLED;
}


static int myregister_probe(struct platform_device *pdev)
{
  int result;
  struct device* err;
  int index;

  printk(KERN_INFO "Probe start\n");

  // Regisztraljuk az eszkozvezerlot
  // dinamukis majorszamot kerunk
  result = register_chrdev(0, "my_FPGA_registers_device", &reg_fops);
  if (result < 0) {
    printk(KERN_ERR "cannot register chrdev: %d\n", result);
    goto fail_reg;
  }

  registers_major = result;

  // input buffernek helyet foglalunk
  input_buffer = kmalloc(BUFF_SIZE, GFP_KERNEL);
  if (!input_buffer) {
    result = -ENOMEM;
    goto fail_mem;
  }
  memset(input_buffer, 0, BUFF_SIZE);

  /* Az udev számára jelzés, hogy hozza létre az eszközállományt. */
  regs_class = class_create(THIS_MODULE, CLASS_NAME);
  //TODO hibakezeles

  for (index = 0; index < 3 ; index++) {

    err = device_create(regs_class, NULL, MKDEV(registers_major, index), NULL, file_names[index]);

    // lekerdezzuk a periferiahoz tartozo informaciokat
    // masodik parameter az ioport.h-ban talalhato
    resource_mem[index] = platform_get_resource(pdev, IORESOURCE_MEM, index);

    // kiszamoljuk a mereteket
    printk(KERN_INFO "Addresses: start: %x end: %x\n",resource_mem[index]->start, resource_mem[index]->end);
    remap_size[index] = resource_mem[index]->end - resource_mem[index]->start + 1;

    if (NULL == request_mem_region(resource_mem[index]->start, remap_size[index], mem_region_names[index])) {
      printk(KERN_ERR "request mem region\n");
      //TODO hibakezelés mert ez nem elég itt
      // ezt jo helye void release_mem_region(unsigned long start, unsigned long len);
      goto fail_req;
    }

    registers_addr[index] = ioremap(resource_mem[index]->start, remap_size[index]);
    printk(KERN_INFO "Remap address: %x\n",(unsigned int)registers_addr[index]);

    if(!registers_addr[index]) {
      printk(KERN_ERR "ERROR ioremap\n");
      //TODO rendes hibakezelés
      goto fail_map;
    }

    IRQ[index] = platform_get_irq(pdev,index);
    printk(KERN_INFO "IRQ%d: %x\n", index, IRQ[index]);

    result = request_irq(IRQ[index], myregister_irq_handler, 0, irq_names[index], pdev);
    if (result < 0) {
      printk(KERN_ERR "cannot request IRQ %d: %d\n", index, result);
      goto fail_irq;
    }
  }

  printk(KERN_INFO "Inserting myreg module succes\n");
  return 0;

  fail_map:
  release_mem_region(resource_mem[0]->start, remap_size[0]);
  release_mem_region(resource_mem[1]->start, remap_size[1]);
  release_mem_region(resource_mem[2]->start, remap_size[2]);
  fail_req:
  free_irq(IRQ[0],pdev);
  free_irq(IRQ[1],pdev);
  free_irq(IRQ[2],pdev);
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
