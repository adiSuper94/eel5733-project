#include <linux/cdev.h> /* cdev utilities */
#include <linux/device.h>
#include <linux/fs.h>   /* file_operations */
#include <linux/init.h> /* module_init, module_exit */
#include <linux/kernel.h>
#include <linux/module.h> /* for modules */
#include <linux/moduleparam.h>
#include <linux/slab.h> /* kmalloc */
#include <linux/string.h>
#include <linux/types.h>
#include <linux/uaccess.h> /* copy_(to,from)_user */

#define MYDEV_NAME "mycdrv"

// NUM_DEVICES defaults to 3 unless specified during insmod
static int NUM_DEVICES = 3;

#define CDRV_IOC_MAGIC 'Z'
#define ASP_CLEAR_BUF _IO(CDRV_IOC_MAGIC, 1)
#define ramdisk_size (size_t)(16 * PAGE_SIZE)
int major = -1;
int minor = -1;
dev_t first = 0;

static struct class *mydev_class;
struct asp_mycdrv {
  struct cdev dev;
  char *ramdisk;
  struct semaphore sem;
  int devNo;
  // any other field you may want to add
};

struct asp_mycdrv *devices; // Array of devices

static loff_t mycdrv_lseek(struct file *file, loff_t offset, int orig) {
  loff_t pos;
  struct asp_mycdrv *dev_ptr = file->private_data;
  down_interruptible(&dev_ptr->sem);
  printk(KERN_INFO ">lseek offset=%lld orig=%d\n", offset, orig);

  switch (orig) {
  case SEEK_CUR:
    pos = file->f_pos + offset;
    break;
  case SEEK_END:
    pos = ramdisk_size + offset;
    break;
  case SEEK_SET:
    pos = offset;
    break;

  default:
    up(&dev_ptr->sem);
    return -EINVAL;
  }

  if (pos < 0) {
    up(&dev_ptr->sem);
    return -1;
  }

  file->f_pos = pos;
  pr_info("Seeking to pos=%ld\n", (long)pos);
  up(&dev_ptr->sem);

  return pos;
}

static int mycdrv_open(struct inode *inode, struct file *file) {
  struct asp_mycdrv *dev;
  dev = container_of(inode->i_cdev, struct asp_mycdrv, dev);
  file->private_data = dev;
  pr_info(" OPENED device: %s:\n\n", MYDEV_NAME);
  return 0;
}

static int mycdrv_release(struct inode *inode, struct file *file) {
  pr_info(" CLOSING device: %s:\n\n", MYDEV_NAME);
  return 0;
}

static ssize_t mycdrv_read(struct file *file, char __user *buf, size_t lbuf, loff_t *ppos) {
  struct asp_mycdrv *dev = file->private_data;
  down_interruptible(&dev->sem);
  int nbytes;
  if ((lbuf + *ppos) > ramdisk_size) {
    pr_info("trying to read past end of device,"
            "aborting because this is just a stub!\n");
    up(&dev->sem);
    return 0;
  }
  nbytes = lbuf - copy_to_user(buf, &dev->ramdisk[*ppos], lbuf);
  *ppos += nbytes;
  pr_info("\n READING function, nbytes=%d, pos=%d\n", nbytes, (int)*ppos);
  up(&dev->sem);
  return nbytes;
}

static ssize_t mycdrv_write(struct file *file, const char __user *buf, size_t lbuf, loff_t *ppos) {
  struct asp_mycdrv *dev = file->private_data;
  down_interruptible(&dev->sem);
  int nbytes;
  if ((lbuf + *ppos) > ramdisk_size) {
    pr_info("trying to read past end of device,"
            "aborting because this is just a stub!\n");
    up(&dev->sem);
    return 0;
  }
  nbytes = lbuf - copy_from_user(dev->ramdisk + *ppos, buf, lbuf);
  *ppos += nbytes;
  up(&dev->sem);
  pr_info("\n WRITING function, nbytes=%d, pos=%d\n", nbytes, (int)*ppos);
  return nbytes;
}

static ssize_t mycdrv_ioctl(struct file *file, unsigned int cmd, unsigned long arg) {
  int retval = 0;
  struct asp_mycdrv *dev = file->private_data;
  if (_IOC_TYPE(cmd) != CDRV_IOC_MAGIC) {
    return -ENOTTY;
  }
  switch (cmd) {
  case ASP_CLEAR_BUF:
    down_interruptible(&dev->sem);
    memset(dev->ramdisk, 0, ramdisk_size);
    file->f_pos = 0;
    up(&dev->sem);
    break;
  default:
    break;
  }
  return 0;
}

static const struct file_operations mycdrv_fops = {
    .owner = THIS_MODULE,
    .read = mycdrv_read,
    .write = mycdrv_write,
    .open = mycdrv_open,
    .release = mycdrv_release,
    .unlocked_ioctl = mycdrv_ioctl,
    .llseek = mycdrv_lseek,
};

static int __init my_init(void) {
  int result;
  result = alloc_chrdev_region(&first, 0, NUM_DEVICES, MYDEV_NAME);
  if (result < 0) {
    printk(KERN_WARNING "can't get major");
    return result;
  }
  major = MAJOR(first);
  if ((mydev_class = class_create(THIS_MODULE, MYDEV_NAME)) == NULL) {
    pr_info("Error while creating device class");
    return -1;
  }
  devices = kmalloc(NUM_DEVICES * sizeof(struct asp_mycdrv), GFP_KERNEL);
  memset(devices, 0, NUM_DEVICES * sizeof(struct asp_mycdrv));
  for (int i = 0; i < NUM_DEVICES; i++) {
    dev_t dev_i = MKDEV(major, i);
    devices[i].devNo = i;
    sema_init(&devices[i].sem, 1);
    devices[i].ramdisk = (char *)kmalloc(ramdisk_size, GFP_KERNEL);
    cdev_init(&devices[i].dev, &mycdrv_fops);
    cdev_add(&devices[i].dev, dev_i, 1);
    device_create(mydev_class, NULL, dev_i, NULL, MYDEV_NAME "%d", i);
  }
  pr_info("\nSucceeded in registering character device %s\n", MYDEV_NAME);
  return 0;
}

static void __exit my_exit(void) {
  for (int i = 0; i < NUM_DEVICES; i++) {
    kfree(devices[i].ramdisk);
    device_destroy(mydev_class, MKDEV(major, i));
    cdev_del(&devices[i].dev);
  }
  kfree(devices);
  class_destroy(mydev_class);
  unregister_chrdev_region(first, NUM_DEVICES);
  pr_info("\ndevice unregistered\n");
}

module_init(my_init);
module_exit(my_exit);
module_param(NUM_DEVICES, int, S_IRUGO);

MODULE_AUTHOR("Aditya Subramanian");
MODULE_LICENSE("GPL v2");
