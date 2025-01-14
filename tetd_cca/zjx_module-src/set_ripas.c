#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/kvm_host.h>
#include <linux/mm.h>
#include <asm/kvm_rme.h>
#include <linux/moduleparam.h>
#include <linux/page-flags.h>
#include <asm/rmi_cmds.h>
static unsigned long vcpu_addr = 0x0;
module_param(vcpu_addr, ulong, S_IRUGO);
MODULE_PARM_DESC(vcpu_addr, "The address of the VCPU to map protected memory");

static unsigned long base_ipa = 0x20000000;
module_param(base_ipa, ulong, S_IRUGO);
MODULE_PARM_DESC(base_ipa, "The address of the base_ipa");

static unsigned long top_size = 0x1000;
module_param(top_size, ulong, S_IRUGO);
MODULE_PARM_DESC(top_size, "The address of the top_size");

static unsigned long ripas = 1;
module_param(ripas, ulong, S_IRUGO);
MODULE_PARM_DESC(ripas, "The address of the ripas");

static struct kvm_vcpu *vcpu;
static struct kvm *kvm;
static struct realm *realm;
static struct realm_rec *rec;
static unsigned long size = PAGE_SIZE;
static phys_addr_t rd;
static phys_addr_t rec_phys;

void realm_ripas(void){
    unsigned long next;
    unsigned long end;
    int ret = 0;

    end = base_ipa + top_size;

    ret = realm_set_ipa_state(vcpu,base_ipa,end,ripas);
    //ret = rmi_rtt_set_ripas(rd,rec_phys,base_ipa,end,&next);
    printk(KERN_INFO "ret=%lx,next=%lx",ret,next); 
}

static int __init realm_ripas_init(void)
{
    vcpu = (struct kvm_vcpu *)vcpu_addr;
    if (!vcpu) {
        printk(KERN_ERR "Failed to get current running VCPU.\n");
        return -EINVAL;
    }

    kvm = vcpu->kvm;
    if (!kvm) {
        printk(KERN_ERR "Failed to get KVM instance.\n");
        return -EINVAL;
    }

    realm = &kvm->arch.realm;
    if (!realm) {
        printk(KERN_ERR "Failed to get Realm object.\n");
        return -EINVAL;
    }

    rec = &vcpu->arch.rec;
    if (!rec) {
        printk(KERN_ERR "Failed to get rec.\n");
        return -EINVAL;
    }
    rec_phys = virt_to_phys(rec->rec_page);

    printk(KERN_INFO "Realm object acquired successfully.\n");
    
    rd = virt_to_phys(realm->rd);
    realm_ripas(); 
    return 0;
}

static void __exit realm_ripas_exit(void)
{
    printk(KERN_INFO "Exiting Realm Mapping Module...\n");
}

module_init(realm_ripas_init);
module_exit(realm_ripas_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("A kernel module to map protected memory in a Realm.");

