get_futex_key(u32 __user *uaddr, int fshared, union futex_key *key, int rw)
{
	unsigned long address = (unsigned long)uaddr;
	struct mm_struct *mm = current->mm;
	struct page *page, *page_head;
	int err, ro = 0;

	 
	key->both.offset = address % PAGE_SIZE;
	if (unlikely((address % sizeof(u32)) != 0))
		return -EINVAL;
	address -= key->both.offset;

	 
	if (!fshared) {
		if (unlikely(!access_ok(VERIFY_WRITE, uaddr, sizeof(u32))))
			return -EFAULT;
		key->private.mm = mm;
		key->private.address = address;
		get_futex_key_refs(key);
		return 0;
	}

again:
	err = get_user_pages_fast(address, 1, 1, &page);
	 
	if (err == -EFAULT && rw == VERIFY_READ) {
		err = get_user_pages_fast(address, 1, 0, &page);
		ro = 1;
	}
	if (err < 0)
		return err;
	else
		err = 0;

#ifdef CONFIG_TRANSPARENT_HUGEPAGE
	page_head = page;
	if (unlikely(PageTail(page))) {
		put_page(page);
		 
		local_irq_disable();
		if (likely(__get_user_pages_fast(address, 1, 1, &page) == 1)) {
			page_head = compound_head(page);
			 
			if (page != page_head) {
				get_page(page_head);
				put_page(page);
			}
			local_irq_enable();
		} else {
			local_irq_enable();
			goto again;
		}
	}
#else
	page_head = compound_head(page);
	if (page != page_head) {
		get_page(page_head);
		put_page(page);
	}
#endif

	lock_page(page_head);

	 
	if (!page_head->mapping) {
		int shmem_swizzled = PageSwapCache(page_head);
		unlock_page(page_head);
		put_page(page_head);
		if (shmem_swizzled)
			goto again;
		return -EFAULT;
	}

	 
	if (PageAnon(page_head)) {
		 
		if (ro) {
			err = -EFAULT;
			goto out;
		}

		key->both.offset |= FUT_OFF_MMSHARED;  
		key->private.mm = mm;
		key->private.address = address;
	} else {
		key->both.offset |= FUT_OFF_INODE;  
		key->shared.inode = page_head->mapping->host;
		key->shared.pgoff = page_head->index;
	}

	get_futex_key_refs(key);

out:
	unlock_page(page_head);
	put_page(page_head);
	return err;
}
