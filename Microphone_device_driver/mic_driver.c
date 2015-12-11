/*
 *  mic_driver.c
 *
 *  Mikrofon periferia meghajto
 *  Diplomatervezes 2015
 *
 *  Author: Bodnar Tibor <piratetibi@gmail.com>
 *
 *  2015
 */

#define LINUX

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>

#include <linux/slab.h>
#include <linux/fs.h>
#include <linux/errno.h>
#include <linux/types.h>
#include <linux/fcntl.h>
#include <asm/uaccess.h>
#include <linux/platform_device.h>
#include <asm/io.h>
#include <linux/io.h>
#include <linux/interrupt.h>
#include <linux/poll.h>
#include <linux/sched.h>
#include <linux/workqueue.h>

// 4 byte adatot tudunk irni
#define BUFF_SIZE 4

#define CLASS_NAME "Microphones_class"
#define PLATFORM_DRIVER_NAME "FPGA_registers"
#define DEVICE_NAME "Microphones_device"

// A string, ami alapjan a device-tree-ben keresunk
#define COMPATIBLE_STRING "Microphones"

// Beregisztralando fajlmuveletek
int reg_open(struct inode *inode, struct file *filep);
int reg_release(struct inode *inode, struct file *filep);
static ssize_t reg_read(struct file *filep, char *buf, size_t count, loff_t *f_pos);
static ssize_t reg_write(struct file *filep, const char *buf, size_t count, loff_t *f_pos);
unsigned int reg_poll(struct file *filp, poll_table *wait);
static void work_fn(struct work_struct* work);

//Globalis valtozok

// A major szamot felul fogjuk irni
int registers_major = 200;
// A harom regiszter
int reg[3] = {0,0,0};
char *input_buffer;
// A harom periferia lekepzett cimei
void *registers_addr[3];
// platform_get_resource visszateresi ertekeneinek
struct resource *resource_mem[3];
int IRQ[3] = {0,0,0};
unsigned long remap_size[3];
int reg_ready[3] = {0,0,0};
static struct class* regs_class;

// File-ok nevei
const char* file_names[] = { "Mic_1_reg", "Mic_2_reg", "Mic_3_reg" };
// Memoria regiok nevei
const char* mem_region_names[] = { "MIC_1", "MIC_2", "MIC_3" };
// Irq nevek
const char* irq_names[] = { "MIC_1_IRQ", "MIC_2_IRQ", "MIC_3_IRQ" };

static struct workqueue_struct* workQ;
// Kulon taskok kellenek,
// mivel ha egy task a queueban van, azt nem rakjuk be ismet
static struct work_struct task[3];

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

static void work_fn(struct work_struct* work) {
  // Ha valamelyik regiszterben uj adat szerepel,
  // a hozza tartozo fajlnak jelezzuk, hogy a poll visszaterhet
  if (reg_ready[0]) {
    printk(KERN_INFO "Reg 0 new value: %x -- %u\n", reg[0], reg[0]);
    wake_up(&wq0);
  }
  if (reg_ready[1]) {
    printk(KERN_INFO "Reg 1 new value: %x -- %u\n", reg[1], reg[1]);
    wake_up(&wq1);
  }
  if (reg_ready[2]) {
    printk(KERN_INFO "Reg 2 new value: %x -- %u\n", reg[2], reg[2]);
    wake_up(&wq2);
  }
}

unsigned int reg_poll(struct file *filep, poll_table *wait )
{
  unsigned int mask = 0;
  switch(MINOR(filep->f_dentry->d_inode->i_rdev)){
    case 0:
      poll_wait( filep, &wq0, wait );
      if (reg_ready[0]) {
        mask |= ( POLLIN | POLLRDNORM );
      }
      break;
    case 1:
      poll_wait( filep, &wq1, wait );
      if (reg_ready[1]) {
        mask |= ( POLLIN | POLLRDNORM );
      }
      break;
    case 2:
      poll_wait( filep, &wq2, wait );
      if (reg_ready[2]) {
        mask |= ( POLLIN | POLLRDNORM );
      }
      break;
    default:
      return -EINVAL;
  }
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
  // igy a masodik korben mar koran visszaterunk.
  // Az olvasas elott poll eseten nem akarjuk,
  // hogy 0-val terjunk vissza, mert akkor lezarul a fajl,
  // ezert ha reg_ready van, azaz az interrupt miatt olvasunk,
  // akkor nem terunk vissza.
  if (*f_pos != 0 && !reg_ready[minor]) {
    return 0;
  }

  // minor 1: mic 0
  // minor 2: mic 1
  // minor 3: mic 2
  reg_value = reg[minor];

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
  printk(KERN_INFO "Removing: ");
  for (index = 0; index < 3; index++) {
    printk(KERN_INFO "device destroy index %d, ", index);
    device_destroy(regs_class, MKDEV(registers_major, index));
    printk(KERN_INFO "iounmap index %d, ", index);
    iounmap(registers_addr[index]);
    printk(KERN_INFO "release mem region index %d, ", index);
    release_mem_region(resource_mem[index]->start, remap_size[index]);
    free_irq(IRQ[index], pdev);
  }

  //class_unregister(regs_class);
  printk(KERN_INFO "\nclass destroy, ");
  class_destroy(regs_class);
  printk(KERN_INFO "unregister chrdev, ");
  unregister_chrdev(registers_major, DEVICE_NAME);
  printk(KERN_INFO "free buffer, ");
  if (input_buffer) {
    kfree(input_buffer);
  }

  printk(KERN_INFO "destroy workqueue\n");
  destroy_workqueue(workQ);
  printk(KERN_INFO "Removing myreg modul\n");

  return 0;
}

static irqreturn_t myregister_irq_handler(int irq, void *dev_id)
{
  int reg_index;
  if (irq == IRQ[0]) {
    reg_index = 0;
  }
  else if (irq == IRQ[1]) {
    reg_index = 1;
  }
  else if (irq == IRQ[2]) {
    reg_index = 2;
  }
  else {
    printk(KERN_ERR "interrupt: invalid irq %d\n", irq);
    return IRQ_NONE;
  }
  queue_work(workQ, &task[reg_index]);
  reg[reg_index] = ioread32(registers_addr[reg_index]);
  reg_ready[reg_index] = 1;
  return IRQ_HANDLED;
}


static int myregister_probe(struct platform_device *pdev)
{
  int result;
  struct device* err;
  int index, index2;

  printk(KERN_INFO "Probe start\n");

  // Regisztraljuk az eszkozvezerlot
  // dinamikus majorszamot kerunk
  result = register_chrdev(0, DEVICE_NAME, &reg_fops);
  if (result < 0) {
    printk(KERN_ERR "cannot register chrdev: %d\n", result);
    goto fail_register_chrdev;
  }

  registers_major = result;

  // input buffernek helyet foglalunk
  input_buffer = kmalloc(BUFF_SIZE, GFP_KERNEL);
  if (!input_buffer) {
    result = -ENOMEM;
    printk(KERN_ERR "cannot allocate buffer memory\n");
    goto fail_mem;
  }
  memset(input_buffer, 0, BUFF_SIZE);

  // Az udev számára jelzés, hogy hozza létre az eszközállományt.
  regs_class = class_create(THIS_MODULE, CLASS_NAME);
  if(!regs_class) {
    printk(KERN_ERR "cannot create class\n");
    goto fail_class_create;
  }

  // Workqueue létrehozás
  workQ = create_workqueue("FPGA_registers_workqueue");
  if(!workQ) {
    printk(KERN_ERR "create_workqueue return with 0");
    goto fail_create_workqueue;
  }

  for (index = 0; index < 3 ; index++) {

    // Device-ok letrehozasa a megfelelo major szamokkal es fajlnevekkel
    err = device_create(regs_class, NULL, MKDEV(registers_major, index), NULL, file_names[index]);
    if(!err){
      printk(KERN_ERR "cannot create device index %d\n",index);
      goto fail_device_create;
    }

    // Lekerdezzuk a periferiahoz tartozo informaciokat
    // Masodik parameter az ioport.h-ban talalhato
    resource_mem[index] = platform_get_resource(pdev, IORESOURCE_MEM, index);

    // Kiszamoljuk a mereteket
    printk(KERN_INFO "Addresses: start: %x end: %x\n",resource_mem[index]->start, resource_mem[index]->end);
    remap_size[index] = resource_mem[index]->end - resource_mem[index]->start + 1;

    if (NULL == request_mem_region(resource_mem[index]->start, remap_size[index], mem_region_names[index])) {
      printk(KERN_ERR "request mem region index %d\n",index);
      goto fail_request_mem;
    }

    // Fizikai cimeket lekepezzuk virtualis cimekre
    registers_addr[index] = ioremap(resource_mem[index]->start, remap_size[index]);
    printk(KERN_INFO "Remap address: %x\n",(unsigned int)registers_addr[index]);

    if(!registers_addr[index]) {
      printk(KERN_ERR "ioremap index %d\n",index);
      goto fail_map;
    }

    // A megszakitas regisztralasa elott mar inicializalnunk kell a work-ot
    INIT_WORK(&task[index], work_fn);

    // Lekerjuk a priferiakhoz tartozo megszakitasok szamat
    IRQ[index] = platform_get_irq(pdev,index);
    printk(KERN_INFO "IRQ%d: %x\n", index, IRQ[index]);

    // Beregisztraljuk a megszakitasvonalhoz a kezelofuggvenyt
    result = request_irq(IRQ[index], myregister_irq_handler, 0, irq_names[index], pdev);
    if (result < 0) {
      printk(KERN_ERR "cannot request IRQ %d: %d\n", index, result);
      goto fail_irq;
    }

  }

  printk(KERN_INFO "Inserting myreg module succes\n");
  return 0;

  fail_irq:
  for(index2 = 0; index2 < index; index2++){
    free_irq(IRQ[index2],pdev);
  }
  index = 3;

  fail_map:
  for(index2 = 0; index2 < index; index2++){
    iounmap(registers_addr[index2]);
  }
  index = 3;

  fail_request_mem:
  for(index2 = 0; index2 < index; index2++){
    release_mem_region(resource_mem[index2]->start, remap_size[index2]);
  }
  index = 3;

  fail_device_create:
  for(index2 = 0; index2 < index; index2++){
    device_destroy(regs_class, MKDEV(registers_major, index2));
  }
  index = 3;

  fail_create_workqueue:
  class_destroy(regs_class);
  fail_class_create:
  kfree(input_buffer);
  fail_mem:
  unregister_chrdev(registers_major, DEVICE_NAME);
  fail_register_chrdev:
  return result;

}

static const struct of_device_id myregister_match[] = {
        { .compatible = COMPATIBLE_STRING },
        {},
};
MODULE_DEVICE_TABLE(of, myregister_match);

static struct platform_driver myregister_driver = {
        .probe = myregister_probe,
        .remove = myregister_remove,
        .driver = {
                .name = PLATFORM_DRIVER_NAME,
                .owner = THIS_MODULE,
                .of_match_table = myregister_match
        },
};

module_platform_driver(myregister_driver);

MODULE_LICENSE("GPL");
