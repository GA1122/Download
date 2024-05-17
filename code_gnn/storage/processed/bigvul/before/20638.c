int kvm_vm_ioctl_get_dirty_log(struct kvm *kvm,
		struct kvm_dirty_log *log)
{
	int r;
	unsigned long n;
	struct kvm_memory_slot *memslot;
	int is_dirty = 0;

	mutex_lock(&kvm->slots_lock);

	r = -EINVAL;
	if (log->slot >= KVM_MEMORY_SLOTS)
		goto out;

	memslot = id_to_memslot(kvm->memslots, log->slot);
	r = -ENOENT;
	if (!memslot->dirty_bitmap)
		goto out;

	kvm_ia64_sync_dirty_log(kvm, memslot);
	r = kvm_get_dirty_log(kvm, log, &is_dirty);
	if (r)
		goto out;

	 
	if (is_dirty) {
		kvm_flush_remote_tlbs(kvm);
		n = kvm_dirty_bitmap_bytes(memslot);
		memset(memslot->dirty_bitmap, 0, n);
	}
	r = 0;
out:
	mutex_unlock(&kvm->slots_lock);
	return r;
}
