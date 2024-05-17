get_futex_key(u32 __user *uaddr, int fshared, union futex_key *key, int rw)
{
	unsigned long address = (unsigned long)uaddr;
	struct mm_struct *mm = current->mm;
	struct page *page, *tail;
	struct address_space *mapping;
	int err, ro = 0;

	 
	key->both.offset = address % PAGE_SIZE;
	if (unlikely((address % sizeof(u32)) != 0))
		return -EINVAL;
	address -= key->both.offset;

	if (unlikely(!access_ok(rw, uaddr, sizeof(u32))))
		return -EFAULT;

	if (unlikely(should_fail_futex(fshared)))
		return -EFAULT;

	 
	if (!fshared) {
		key->private.mm = mm;
		key->private.address = address;
		get_futex_key_refs(key);   
		return 0;
	}

again:
	 
	if (unlikely(should_fail_futex(fshared)))
		return -EFAULT;

	err = get_user_pages_fast(address, 1, 1, &page);
	 
	if (err == -EFAULT && rw == VERIFY_READ) {
		err = get_user_pages_fast(address, 1, 0, &page);
		ro = 1;
	}
	if (err < 0)
		return err;
	else
		err = 0;

	 
	tail = page;
	page = compound_head(page);
	mapping = READ_ONCE(page->mapping);

	 
	if (unlikely(!mapping)) {
		int shmem_swizzled;

		 
		lock_page(page);
		shmem_swizzled = PageSwapCache(page) || page->mapping;
		unlock_page(page);
		put_page(page);

		if (shmem_swizzled)
			goto again;

		return -EFAULT;
	}

	 
	if (PageAnon(page)) {
		 
		if (unlikely(should_fail_futex(fshared)) || ro) {
			err = -EFAULT;
			goto out;
		}

		key->both.offset |= FUT_OFF_MMSHARED;  
		key->private.mm = mm;
		key->private.address = address;

		get_futex_key_refs(key);  

	} else {
		struct inode *inode;

		 
		rcu_read_lock();

		if (READ_ONCE(page->mapping) != mapping) {
			rcu_read_unlock();
			put_page(page);

			goto again;
		}

		inode = READ_ONCE(mapping->host);
		if (!inode) {
			rcu_read_unlock();
			put_page(page);

			goto again;
		}

		 
		if (!atomic_inc_not_zero(&inode->i_count)) {
			rcu_read_unlock();
			put_page(page);

			goto again;
		}

		 
		if (WARN_ON_ONCE(inode->i_mapping != mapping)) {
			err = -EFAULT;
			rcu_read_unlock();
			iput(inode);

			goto out;
		}

		key->both.offset |= FUT_OFF_INODE;  
		key->shared.inode = inode;
		key->shared.pgoff = basepage_index(tail);
		rcu_read_unlock();
	}

out:
	put_page(page);
	return err;
}
