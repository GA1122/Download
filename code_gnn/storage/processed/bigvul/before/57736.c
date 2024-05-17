static void kvm_mmu_slot_apply_flags(struct kvm *kvm,
				     struct kvm_memory_slot *new)
{
	 
	if (new->flags & KVM_MEM_READONLY) {
		kvm_mmu_slot_remove_write_access(kvm, new);
		return;
	}

	 
	if (new->flags & KVM_MEM_LOG_DIRTY_PAGES) {
		if (kvm_x86_ops->slot_enable_log_dirty)
			kvm_x86_ops->slot_enable_log_dirty(kvm, new);
		else
			kvm_mmu_slot_remove_write_access(kvm, new);
	} else {
		if (kvm_x86_ops->slot_disable_log_dirty)
			kvm_x86_ops->slot_disable_log_dirty(kvm, new);
	}
}
