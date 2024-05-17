void kvm_arch_memslots_updated(struct kvm *kvm, struct kvm_memslots *slots)
{
	 
	kvm_mmu_invalidate_mmio_sptes(kvm, slots);
}
