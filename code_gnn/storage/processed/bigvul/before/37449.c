static unsigned int kvm_current_mmio_generation(struct kvm *kvm)
{
	 
	return (kvm_memslots(kvm)->generation +
		      MMIO_MAX_GEN - 150) & MMIO_GEN_MASK;
}
