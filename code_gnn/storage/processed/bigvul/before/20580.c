void kvm_arch_destroy_vm(struct kvm *kvm)
{
	kvm_iommu_unmap_guest(kvm);
#ifdef  KVM_CAP_DEVICE_ASSIGNMENT
	kvm_free_all_assigned_devices(kvm);
#endif
	kfree(kvm->arch.vioapic);
	kvm_release_vm_pages(kvm);
}
