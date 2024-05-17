static int kvm_mmu_notifier_invalidate_range_start(struct mmu_notifier *mn,
					const struct mmu_notifier_range *range)
{
	struct kvm *kvm = mmu_notifier_to_kvm(mn);
	int need_tlb_flush = 0, idx;
	int ret;

	idx = srcu_read_lock(&kvm->srcu);
	spin_lock(&kvm->mmu_lock);
	 
	kvm->mmu_notifier_count++;
	need_tlb_flush = kvm_unmap_hva_range(kvm, range->start, range->end);
	need_tlb_flush |= kvm->tlbs_dirty;
	 
	if (need_tlb_flush)
		kvm_flush_remote_tlbs(kvm);

	spin_unlock(&kvm->mmu_lock);

	ret = kvm_arch_mmu_notifier_invalidate_range(kvm, range->start,
					range->end, range->blockable);

	srcu_read_unlock(&kvm->srcu, idx);

	return ret;
}
