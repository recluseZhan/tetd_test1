#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/kvm_host.h>
#include <linux/mm.h>
#include <asm/kvm_rme.h>
#include <linux/moduleparam.h>
#include <linux/page-flags.h>
#include <asm/rmi_cmds.h>

static unsigned long step = 0;
module_param(step, ulong, S_IRUGO);
MODULE_PARM_DESC(step, "This is step");

extern int realm_map(unsigned long step);
static int __init realm_map_init(void)
{
    realm_map(step);
    printk(KERN_INFO "Protected memory mapping2 successful.\n");
    return 0;
}

static void __exit realm_map_exit(void)
{
    printk(KERN_INFO "Exiting Realm Mapping2 Module...\n");
}

module_init(realm_map_init);
module_exit(realm_map_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("A kernel module to map protected memory in a Realm.");

