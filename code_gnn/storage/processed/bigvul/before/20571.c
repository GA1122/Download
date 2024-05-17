static int kvm_alloc_vmm_area(void)
{
	if (!kvm_vmm_base && (kvm_vm_buffer_size < KVM_VM_BUFFER_SIZE)) {
		kvm_vmm_base = __get_free_pages(GFP_KERNEL,
				get_order(KVM_VMM_SIZE));
		if (!kvm_vmm_base)
			return -ENOMEM;

		memset((void *)kvm_vmm_base, 0, KVM_VMM_SIZE);
		kvm_vm_buffer = kvm_vmm_base + VMM_SIZE;

		printk(KERN_DEBUG"kvm:VMM's Base Addr:0x%lx, vm_buffer:0x%lx\n",
				kvm_vmm_base, kvm_vm_buffer);
	}

	return 0;
}
