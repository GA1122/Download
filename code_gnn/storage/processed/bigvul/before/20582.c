void kvm_arch_exit(void)
{
	kvm_free_vmm_area();
	kfree(kvm_vmm_info);
	kvm_vmm_info = NULL;
}
