#include <linux/init.h>
#include <linux/module.h>
#include <linux/ioctl.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/err.h>
#include <linux/list.h>
#include <linux/errno.h>
#include <linux/mutex.h>
#include <linux/slab.h>
#include <linux/compat.h>
#include <linux/of.h>
#include <linux/of_device.h>
#include <linux/acpi.h>

#include <linux/spi/spi.h>
#include <linux/spi/spidev.h>

#include <linux/uaccess.h>

#define SPI_IOC_WR 123


static struct spi_device *dac;

static int major;
static long
spidev_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
	int val;
	int err;

	unsigned char tx_buf[2];
	unsigned char rx_buf[2];

	struct spi_message msg;
	struct spi_transfer xfer[1];
	/*copy_from_user*/
	err = copy_from_user(&val,(const void __user *)arg,sizeof(int));
	printk("spidev_ioctl get val from user: %d\n", val);
	/*发起SPI传输*/
	/*1.把val修改为正确格式*/
	val <<= 2;
	val &= 0xFFC;
	tx_buf[1] = val & 0xff;
	tx_buf[0] = (val >> 8) & 0xff;
	/*2.1构造transfer*/
	/*2.2加入message*/
	/*2.3调用spi_sync*/
	memset(&xfer[0],0,sizeof(xfer));
	xfer[0].tx_buf = tx_buf;
	xfer[0].rx_buf = rx_buf;
	xfer[0].len = 2;

	spi_message_init(&msg);
	spi_message_add_tail(&xfer[0],&msg);
	spi_sync(dac,&msg);
	/*3.修改读到的数据为正确格式*/
	val = (rx_buf[0] << 8) | (rx_buf[1]);
	val >>= 2;
	/*copy_to_user*/
	err = copy_to_user((void __user *)arg,&val,sizeof(int));
    return 0;
}

static const struct file_operations spidev_fops = {
	.owner =	THIS_MODULE,
	/* REVISIT switch to aio primitives, so that userspace
	 * gets more complete API coverage.  It'll simplify things
	 * too, except for the locking.
	 */
	.unlocked_ioctl = spidev_ioctl,
};

/*-------------------------------------------------------------------------*/

/* The main reason to have this class is to make mdev/udev create the
 * /dev/spidevB.C character device nodes exposing our userspace API.
 * It also simplifies memory management.
 */

static struct class *spidev_class;

static const struct of_device_id spidev_dt_ids[] = {
	{ .compatible = "zh,dac" },
	{},
};

/*-------------------------------------------------------------------------*/

static int spidev_probe(struct spi_device *spi)
{
    /*1.记录spi_device*/
	dac = spi;
    /*2.注册字符设备*/
	major = register_chrdev(0,"dac",&spidev_fops);

	spidev_class = class_create(THIS_MODULE,"dac");

	device_create(spidev_class,NULL,MKDEV(major,0),NULL,"zh_dac");

	return 0;
}

static int spidev_remove(struct spi_device *spi)
{
    /*反注册字符设备驱动程序*/
	device_destroy(spidev_class,MKDEV(major,0));

	class_destroy(spidev_class);

	unregister_chrdev(major,"dac");

	return 0;
}

static struct spi_driver spidev_spi_driver = {
	.driver = {
		.name =		"zh_dac",
		.of_match_table = of_match_ptr(spidev_dt_ids),
	},
	.probe =	spidev_probe,
	.remove =	spidev_remove,

	/* NOTE:  suspend/resume methods are not necessary here.
	 * We don't do anything except pass the requests to/from
	 * the underlying controller.  The refrigerator handles
	 * most issues; the controller driver handles the rest.
	 */
};

/*-------------------------------------------------------------------------*/

static int __init spidev_init(void)
{
    int status;
	printk("%s %s %d\n", __FILE__, __FUNCTION__, __LINE__);
	status = spi_register_driver(&spidev_spi_driver);
	if (status < 0) {

	}
	return status;
}
module_init(spidev_init);

static void __exit spidev_exit(void)
{
	printk("%s %s %d\n", __FILE__, __FUNCTION__, __LINE__);
	spi_unregister_driver(&spidev_spi_driver);
}
module_exit(spidev_exit);

MODULE_LICENSE("GPL");
