int __kvm_set_memory_region(struct kvm *kvm,
			    struct kvm_userspace_memory_region *mem)
{
	int r;
	gfn_t base_gfn;
	unsigned long npages;
	struct kvm_memory_slot *slot;
	struct kvm_memory_slot old, new;
	struct kvm_memslots *slots = NULL, *old_memslots;
	enum kvm_mr_change change;

	r = check_memory_region_flags(mem);
	if (r)
		goto out;

	r = -EINVAL;
	 
	if (mem->memory_size & (PAGE_SIZE - 1))
		goto out;
	if (mem->guest_phys_addr & (PAGE_SIZE - 1))
		goto out;
	 
	if ((mem->slot < KVM_USER_MEM_SLOTS) &&
	    ((mem->userspace_addr & (PAGE_SIZE - 1)) ||
	     !access_ok(VERIFY_WRITE,
			(void __user *)(unsigned long)mem->userspace_addr,
			mem->memory_size)))
		goto out;
	if (mem->slot >= KVM_MEM_SLOTS_NUM)
		goto out;
	if (mem->guest_phys_addr + mem->memory_size < mem->guest_phys_addr)
		goto out;

	slot = id_to_memslot(kvm->memslots, mem->slot);
	base_gfn = mem->guest_phys_addr >> PAGE_SHIFT;
	npages = mem->memory_size >> PAGE_SHIFT;

	r = -EINVAL;
	if (npages > KVM_MEM_MAX_NR_PAGES)
		goto out;

	if (!npages)
		mem->flags &= ~KVM_MEM_LOG_DIRTY_PAGES;

	new = old = *slot;

	new.id = mem->slot;
	new.base_gfn = base_gfn;
	new.npages = npages;
	new.flags = mem->flags;

	r = -EINVAL;
	if (npages) {
		if (!old.npages)
			change = KVM_MR_CREATE;
		else {  
			if ((mem->userspace_addr != old.userspace_addr) ||
			    (npages != old.npages) ||
			    ((new.flags ^ old.flags) & KVM_MEM_READONLY))
				goto out;

			if (base_gfn != old.base_gfn)
				change = KVM_MR_MOVE;
			else if (new.flags != old.flags)
				change = KVM_MR_FLAGS_ONLY;
			else {  
				r = 0;
				goto out;
			}
		}
	} else if (old.npages) {
		change = KVM_MR_DELETE;
	} else  
		goto out;

	if ((change == KVM_MR_CREATE) || (change == KVM_MR_MOVE)) {
		 
		r = -EEXIST;
		kvm_for_each_memslot(slot, kvm->memslots) {
			if ((slot->id >= KVM_USER_MEM_SLOTS) ||
			    (slot->id == mem->slot))
				continue;
			if (!((base_gfn + npages <= slot->base_gfn) ||
			      (base_gfn >= slot->base_gfn + slot->npages)))
				goto out;
		}
	}

	 
	if (!(new.flags & KVM_MEM_LOG_DIRTY_PAGES))
		new.dirty_bitmap = NULL;

	r = -ENOMEM;
	if (change == KVM_MR_CREATE) {
		new.userspace_addr = mem->userspace_addr;

		if (kvm_arch_create_memslot(kvm, &new, npages))
			goto out_free;
	}

	 
	if ((new.flags & KVM_MEM_LOG_DIRTY_PAGES) && !new.dirty_bitmap) {
		if (kvm_create_dirty_bitmap(&new) < 0)
			goto out_free;
	}

	if ((change == KVM_MR_DELETE) || (change == KVM_MR_MOVE)) {
		r = -ENOMEM;
		slots = kmemdup(kvm->memslots, sizeof(struct kvm_memslots),
				GFP_KERNEL);
		if (!slots)
			goto out_free;
		slot = id_to_memslot(slots, mem->slot);
		slot->flags |= KVM_MEMSLOT_INVALID;

		old_memslots = install_new_memslots(kvm, slots, NULL);

		 
		kvm_iommu_unmap_pages(kvm, &old);
		 
		kvm_arch_flush_shadow_memslot(kvm, slot);
		slots = old_memslots;
	}

	r = kvm_arch_prepare_memory_region(kvm, &new, mem, change);
	if (r)
		goto out_slots;

	r = -ENOMEM;
	 
	if (!slots) {
		slots = kmemdup(kvm->memslots, sizeof(struct kvm_memslots),
				GFP_KERNEL);
		if (!slots)
			goto out_free;
	}

	 
	if (change == KVM_MR_DELETE) {
		new.dirty_bitmap = NULL;
		memset(&new.arch, 0, sizeof(new.arch));
	}

	old_memslots = install_new_memslots(kvm, slots, &new);

	kvm_arch_commit_memory_region(kvm, mem, &old, change);

	kvm_free_physmem_slot(kvm, &old, &new);
	kfree(old_memslots);

	 
	if ((change == KVM_MR_CREATE) || (change == KVM_MR_MOVE)) {
		r = kvm_iommu_map_pages(kvm, &new);
		return r;
	}

	return 0;

out_slots:
	kfree(slots);
out_free:
	kvm_free_physmem_slot(kvm, &new, &old);
out:
	return r;
}
