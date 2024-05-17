get_futex_key(u32 __user *uaddr, int fshared, union futex_key *key)
{
	unsigned long address = (unsigned long)uaddr;
	struct mm_struct *mm = current->mm;
	struct page *page;
	int err;

	 
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
	if (err < 0)
		return err;

	page = compound_head(page);
	lock_page(page);
	if (!page->mapping) {
		unlock_page(page);
		put_page(page);
		goto again;
	}

	 
	if (PageAnon(page)) {
		key->both.offset |= FUT_OFF_MMSHARED;  
		key->private.mm = mm;
		key->private.address = address;
	} else {
		key->both.offset |= FUT_OFF_INODE;  
		key->shared.inode = page->mapping->host;
		key->shared.pgoff = page->index;
	}

	get_futex_key_refs(key);

	unlock_page(page);
	put_page(page);
	return 0;
}
