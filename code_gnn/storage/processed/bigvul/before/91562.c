int kvm_get_dirty_log_protect(struct kvm *kvm,
			struct kvm_dirty_log *log, bool *flush)
{
	struct kvm_memslots *slots;
	struct kvm_memory_slot *memslot;
	int i, as_id, id;
	unsigned long n;
	unsigned long *dirty_bitmap;
	unsigned long *dirty_bitmap_buffer;

	as_id = log->slot >> 16;
	id = (u16)log->slot;
	if (as_id >= KVM_ADDRESS_SPACE_NUM || id >= KVM_USER_MEM_SLOTS)
		return -EINVAL;

	slots = __kvm_memslots(kvm, as_id);
	memslot = id_to_memslot(slots, id);

	dirty_bitmap = memslot->dirty_bitmap;
	if (!dirty_bitmap)
		return -ENOENT;

	n = kvm_dirty_bitmap_bytes(memslot);
	*flush = false;
	if (kvm->manual_dirty_log_protect) {
		 
		dirty_bitmap_buffer = dirty_bitmap;
	} else {
		dirty_bitmap_buffer = kvm_second_dirty_bitmap(memslot);
		memset(dirty_bitmap_buffer, 0, n);

		spin_lock(&kvm->mmu_lock);
		for (i = 0; i < n / sizeof(long); i++) {
			unsigned long mask;
			gfn_t offset;

			if (!dirty_bitmap[i])
				continue;

			*flush = true;
			mask = xchg(&dirty_bitmap[i], 0);
			dirty_bitmap_buffer[i] = mask;

			if (mask) {
				offset = i * BITS_PER_LONG;
				kvm_arch_mmu_enable_log_dirty_pt_masked(kvm, memslot,
									offset, mask);
			}
		}
		spin_unlock(&kvm->mmu_lock);
	}

	if (copy_to_user(log->dirty_bitmap, dirty_bitmap_buffer, n))
		return -EFAULT;
	return 0;
}