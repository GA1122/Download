static int handle_vm_shutdown(struct kvm_vcpu *vcpu,
		struct kvm_run *kvm_run)
{
	kvm_run->exit_reason = KVM_EXIT_SHUTDOWN;
	return 0;
}
