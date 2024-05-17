SYSCALL_DEFINE2(swapon, const char __user *, specialfile, int, swap_flags)
{
	struct swap_info_struct *p;
	char *name;
	struct file *swap_file = NULL;
	struct address_space *mapping;
	int i;
	int prio;
	int error;
	union swap_header *swap_header;
	int nr_extents;
	sector_t span;
	unsigned long maxpages;
	unsigned char *swap_map = NULL;
	struct page *page = NULL;
	struct inode *inode = NULL;

	if (!capable(CAP_SYS_ADMIN))
		return -EPERM;

	p = alloc_swap_info();
	if (IS_ERR(p))
		return PTR_ERR(p);

	name = getname(specialfile);
	if (IS_ERR(name)) {
		error = PTR_ERR(name);
		name = NULL;
		goto bad_swap;
	}
	swap_file = filp_open(name, O_RDWR|O_LARGEFILE, 0);
	if (IS_ERR(swap_file)) {
		error = PTR_ERR(swap_file);
		swap_file = NULL;
		goto bad_swap;
	}

	p->swap_file = swap_file;
	mapping = swap_file->f_mapping;

	for (i = 0; i < nr_swapfiles; i++) {
		struct swap_info_struct *q = swap_info[i];

		if (q == p || !q->swap_file)
			continue;
		if (mapping == q->swap_file->f_mapping) {
			error = -EBUSY;
			goto bad_swap;
		}
	}

	inode = mapping->host;
	 
	error = claim_swapfile(p, inode);
	if (unlikely(error))
		goto bad_swap;

	 
	if (!mapping->a_ops->readpage) {
		error = -EINVAL;
		goto bad_swap;
	}
	page = read_mapping_page(mapping, 0, swap_file);
	if (IS_ERR(page)) {
		error = PTR_ERR(page);
		goto bad_swap;
	}
	swap_header = kmap(page);

	maxpages = read_swap_header(p, swap_header, inode);
	if (unlikely(!maxpages)) {
		error = -EINVAL;
		goto bad_swap;
	}

	 
	swap_map = vzalloc(maxpages);
	if (!swap_map) {
		error = -ENOMEM;
		goto bad_swap;
	}

	error = swap_cgroup_swapon(p->type, maxpages);
	if (error)
		goto bad_swap;

	nr_extents = setup_swap_map_and_extents(p, swap_header, swap_map,
		maxpages, &span);
	if (unlikely(nr_extents < 0)) {
		error = nr_extents;
		goto bad_swap;
	}

	if (p->bdev) {
		if (blk_queue_nonrot(bdev_get_queue(p->bdev))) {
			p->flags |= SWP_SOLIDSTATE;
			p->cluster_next = 1 + (random32() % p->highest_bit);
		}
		if (discard_swap(p) == 0 && (swap_flags & SWAP_FLAG_DISCARD))
			p->flags |= SWP_DISCARDABLE;
	}

	mutex_lock(&swapon_mutex);
	prio = -1;
	if (swap_flags & SWAP_FLAG_PREFER)
		prio =
		  (swap_flags & SWAP_FLAG_PRIO_MASK) >> SWAP_FLAG_PRIO_SHIFT;
	enable_swap_info(p, prio, swap_map);

	printk(KERN_INFO "Adding %uk swap on %s.  "
			"Priority:%d extents:%d across:%lluk %s%s\n",
		p->pages<<(PAGE_SHIFT-10), name, p->prio,
		nr_extents, (unsigned long long)span<<(PAGE_SHIFT-10),
		(p->flags & SWP_SOLIDSTATE) ? "SS" : "",
		(p->flags & SWP_DISCARDABLE) ? "D" : "");

	mutex_unlock(&swapon_mutex);
	atomic_inc(&proc_poll_event);
	wake_up_interruptible(&proc_poll_wait);

	if (S_ISREG(inode->i_mode))
		inode->i_flags |= S_SWAPFILE;
	error = 0;
	goto out;
bad_swap:
	if (inode && S_ISBLK(inode->i_mode) && p->bdev) {
		set_blocksize(p->bdev, p->old_block_size);
		blkdev_put(p->bdev, FMODE_READ | FMODE_WRITE | FMODE_EXCL);
	}
	destroy_swap_extents(p);
	swap_cgroup_swapoff(p->type);
	spin_lock(&swap_lock);
	p->swap_file = NULL;
	p->flags = 0;
	spin_unlock(&swap_lock);
	vfree(swap_map);
	if (swap_file) {
		if (inode && S_ISREG(inode->i_mode)) {
			mutex_unlock(&inode->i_mutex);
			inode = NULL;
		}
		filp_close(swap_file, NULL);
	}
out:
	if (page && !IS_ERR(page)) {
		kunmap(page);
		page_cache_release(page);
	}
	if (name)
		putname(name);
	if (inode && S_ISREG(inode->i_mode))
		mutex_unlock(&inode->i_mutex);
	return error;
}
