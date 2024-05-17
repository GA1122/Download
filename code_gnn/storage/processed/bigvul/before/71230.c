static void kvm_mmu_notifier_invalidate_page(struct mmu_notifier *mn,
					     struct mm_struct *mm,
					     unsigned long address)
{
	struct kvm *kvm = mmu_notifier_to_kvm(mn);
	int need_tlb_flush, idx;

	 
	idx = srcu_read_lock(&kvm->srcu);
	spin_lock(&kvm->mmu_lock);

	kvm->mmu_notifier_seq++;
	need_tlb_flush = kvm_unmap_hva(kvm, address) | kvm->tlbs_dirty;
	 
	if (need_tlb_flush)
		kvm_flush_remote_tlbs(kvm);

	spin_unlock(&kvm->mmu_lock);

	kvm_arch_mmu_notifier_invalidate_page(kvm, address);

	srcu_read_unlock(&kvm->srcu, idx);
}
