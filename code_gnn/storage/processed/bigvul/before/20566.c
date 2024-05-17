static int handle_vcpu_debug(struct kvm_vcpu *vcpu,
				struct kvm_run *kvm_run)
{
	printk("VMM: %s", vcpu->arch.log_buf);
	return 1;
}
