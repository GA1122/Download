int kvm_vm_ioctl_get_dirty_log(struct kvm *kvm,
				      struct kvm_dirty_log *log)
{
	int r;
	struct kvm_memory_slot *memslot;
	unsigned long n, nr_dirty_pages;

	mutex_lock(&kvm->slots_lock);

	r = -EINVAL;
	if (log->slot >= KVM_MEMORY_SLOTS)
		goto out;

	memslot = id_to_memslot(kvm->memslots, log->slot);
	r = -ENOENT;
	if (!memslot->dirty_bitmap)
		goto out;

	n = kvm_dirty_bitmap_bytes(memslot);
	nr_dirty_pages = memslot->nr_dirty_pages;

	 
	if (nr_dirty_pages) {
		struct kvm_memslots *slots, *old_slots;
		unsigned long *dirty_bitmap, *dirty_bitmap_head;

		dirty_bitmap = memslot->dirty_bitmap;
		dirty_bitmap_head = memslot->dirty_bitmap_head;
		if (dirty_bitmap == dirty_bitmap_head)
			dirty_bitmap_head += n / sizeof(long);
		memset(dirty_bitmap_head, 0, n);

		r = -ENOMEM;
		slots = kmemdup(kvm->memslots, sizeof(*kvm->memslots), GFP_KERNEL);
		if (!slots)
			goto out;

		memslot = id_to_memslot(slots, log->slot);
		memslot->nr_dirty_pages = 0;
		memslot->dirty_bitmap = dirty_bitmap_head;
		update_memslots(slots, NULL);

		old_slots = kvm->memslots;
		rcu_assign_pointer(kvm->memslots, slots);
		synchronize_srcu_expedited(&kvm->srcu);
		kfree(old_slots);

		write_protect_slot(kvm, memslot, dirty_bitmap, nr_dirty_pages);

		r = -EFAULT;
		if (copy_to_user(log->dirty_bitmap, dirty_bitmap, n))
			goto out;
	} else {
		r = -EFAULT;
		if (clear_user(log->dirty_bitmap, n))
			goto out;
	}

	r = 0;
out:
	mutex_unlock(&kvm->slots_lock);
	return r;
}
