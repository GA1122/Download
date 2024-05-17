void kvm_arch_commit_memory_region(struct kvm *kvm,
				const struct kvm_userspace_memory_region *mem,
				const struct kvm_memory_slot *old,
				const struct kvm_memory_slot *new,
				enum kvm_mr_change change)
{
	int nr_mmu_pages = 0;

	if (!kvm->arch.n_requested_mmu_pages)
		nr_mmu_pages = kvm_mmu_calculate_mmu_pages(kvm);

	if (nr_mmu_pages)
		kvm_mmu_change_mmu_pages(kvm, nr_mmu_pages);

	 
	if ((change != KVM_MR_DELETE) &&
		(old->flags & KVM_MEM_LOG_DIRTY_PAGES) &&
		!(new->flags & KVM_MEM_LOG_DIRTY_PAGES))
		kvm_mmu_zap_collapsible_sptes(kvm, new);

	 
	if (change != KVM_MR_DELETE)
		kvm_mmu_slot_apply_flags(kvm, (struct kvm_memory_slot *) new);
}
