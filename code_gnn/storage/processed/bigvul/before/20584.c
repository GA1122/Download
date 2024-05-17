void kvm_arch_free_vm(struct kvm *kvm)
{
	unsigned long vm_base = kvm->arch.vm_base;

	if (vm_base) {
		memset((void *)vm_base, 0, KVM_VM_DATA_SIZE);
		free_pages(vm_base, get_order(KVM_VM_DATA_SIZE));
	}

}
