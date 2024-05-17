static void write_protect_slot(struct kvm *kvm,
			       struct kvm_memory_slot *memslot,
			       unsigned long *dirty_bitmap,
			       unsigned long nr_dirty_pages)
{
	spin_lock(&kvm->mmu_lock);

	 
	if (nr_dirty_pages < kvm->arch.n_used_mmu_pages) {
		unsigned long gfn_offset;

		for_each_set_bit(gfn_offset, dirty_bitmap, memslot->npages) {
			unsigned long gfn = memslot->base_gfn + gfn_offset;

			kvm_mmu_rmap_write_protect(kvm, gfn, memslot);
		}
		kvm_flush_remote_tlbs(kvm);
	} else
		kvm_mmu_slot_remove_write_access(kvm, memslot->id);

	spin_unlock(&kvm->mmu_lock);
}
