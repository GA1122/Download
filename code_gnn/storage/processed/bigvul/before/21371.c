SYSCALL_DEFINE1(swapoff, const char __user *, specialfile)
{
	struct swap_info_struct *p = NULL;
	unsigned char *swap_map;
	struct file *swap_file, *victim;
	struct address_space *mapping;
	struct inode *inode;
	char *pathname;
	int oom_score_adj;
	int i, type, prev;
	int err;

	if (!capable(CAP_SYS_ADMIN))
		return -EPERM;

	pathname = getname(specialfile);
	err = PTR_ERR(pathname);
	if (IS_ERR(pathname))
		goto out;

	victim = filp_open(pathname, O_RDWR|O_LARGEFILE, 0);
	putname(pathname);
	err = PTR_ERR(victim);
	if (IS_ERR(victim))
		goto out;

	mapping = victim->f_mapping;
	prev = -1;
	spin_lock(&swap_lock);
	for (type = swap_list.head; type >= 0; type = swap_info[type]->next) {
		p = swap_info[type];
		if (p->flags & SWP_WRITEOK) {
			if (p->swap_file->f_mapping == mapping)
				break;
		}
		prev = type;
	}
	if (type < 0) {
		err = -EINVAL;
		spin_unlock(&swap_lock);
		goto out_dput;
	}
	if (!security_vm_enough_memory(p->pages))
		vm_unacct_memory(p->pages);
	else {
		err = -ENOMEM;
		spin_unlock(&swap_lock);
		goto out_dput;
	}
	if (prev < 0)
		swap_list.head = p->next;
	else
		swap_info[prev]->next = p->next;
	if (type == swap_list.next) {
		 
		swap_list.next = swap_list.head;
	}
	if (p->prio < 0) {
		for (i = p->next; i >= 0; i = swap_info[i]->next)
			swap_info[i]->prio = p->prio--;
		least_priority++;
	}
	nr_swap_pages -= p->pages;
	total_swap_pages -= p->pages;
	p->flags &= ~SWP_WRITEOK;
	spin_unlock(&swap_lock);

	oom_score_adj = test_set_oom_score_adj(OOM_SCORE_ADJ_MAX);
	err = try_to_unuse(type);
	compare_swap_oom_score_adj(OOM_SCORE_ADJ_MAX, oom_score_adj);

	if (err) {
		 
		 
		enable_swap_info(p, p->prio, p->swap_map);
		goto out_dput;
	}

	destroy_swap_extents(p);
	if (p->flags & SWP_CONTINUED)
		free_swap_count_continuations(p);

	mutex_lock(&swapon_mutex);
	spin_lock(&swap_lock);
	drain_mmlist();

	 
	p->highest_bit = 0;		 
	while (p->flags >= SWP_SCANNING) {
		spin_unlock(&swap_lock);
		schedule_timeout_uninterruptible(1);
		spin_lock(&swap_lock);
	}

	swap_file = p->swap_file;
	p->swap_file = NULL;
	p->max = 0;
	swap_map = p->swap_map;
	p->swap_map = NULL;
	p->flags = 0;
	spin_unlock(&swap_lock);
	mutex_unlock(&swapon_mutex);
	vfree(swap_map);
	 
	swap_cgroup_swapoff(type);

	inode = mapping->host;
	if (S_ISBLK(inode->i_mode)) {
		struct block_device *bdev = I_BDEV(inode);
		set_blocksize(bdev, p->old_block_size);
		blkdev_put(bdev, FMODE_READ | FMODE_WRITE | FMODE_EXCL);
	} else {
		mutex_lock(&inode->i_mutex);
		inode->i_flags &= ~S_SWAPFILE;
		mutex_unlock(&inode->i_mutex);
	}
	filp_close(swap_file, NULL);
	err = 0;
	atomic_inc(&proc_poll_event);
	wake_up_interruptible(&proc_poll_wait);

out_dput:
	filp_close(victim, NULL);
out:
	return err;
}
