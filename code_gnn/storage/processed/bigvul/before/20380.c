static int kvm_reboot(struct notifier_block *notifier, unsigned long val,
		      void *v)
{
	 
	printk(KERN_INFO "kvm: exiting hardware virtualization\n");
	kvm_rebooting = true;
	on_each_cpu(hardware_disable_nolock, NULL, 1);
	return NOTIFY_OK;
}
