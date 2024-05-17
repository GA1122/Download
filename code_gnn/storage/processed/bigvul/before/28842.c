void kvm_arch_memslots_updated(struct kvm *kvm)
{
	 
	kvm_mmu_invalidate_mmio_sptes(kvm);
}
