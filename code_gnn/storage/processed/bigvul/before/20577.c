struct kvm *kvm_arch_alloc_vm(void)
{

	struct kvm *kvm;
	uint64_t  vm_base;

	BUG_ON(sizeof(struct kvm) > KVM_VM_STRUCT_SIZE);

	vm_base = __get_free_pages(GFP_KERNEL, get_order(KVM_VM_DATA_SIZE));

	if (!vm_base)
		return NULL;

	memset((void *)vm_base, 0, KVM_VM_DATA_SIZE);
	kvm = (struct kvm *)(vm_base +
			offsetof(struct kvm_vm_data, kvm_vm_struct));
	kvm->arch.vm_base = vm_base;
	printk(KERN_DEBUG"kvm: vm's data area:0x%lx\n", vm_base);

	return kvm;
}
