static void kvm_ia64_sync_dirty_log(struct kvm *kvm,
				    struct kvm_memory_slot *memslot)
{
	int i;
	long base;
	unsigned long n;
	unsigned long *dirty_bitmap = (unsigned long *)(kvm->arch.vm_base +
			offsetof(struct kvm_vm_data, kvm_mem_dirty_log));

	n = kvm_dirty_bitmap_bytes(memslot);
	base = memslot->base_gfn / BITS_PER_LONG;

	spin_lock(&kvm->arch.dirty_log_lock);
	for (i = 0; i < n/sizeof(long); ++i) {
		memslot->dirty_bitmap[i] = dirty_bitmap[base + i];
		dirty_bitmap[base + i] = 0;
	}
	spin_unlock(&kvm->arch.dirty_log_lock);
}
